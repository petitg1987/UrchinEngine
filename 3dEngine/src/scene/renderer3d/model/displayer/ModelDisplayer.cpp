#include <memory>

#include "ModelDisplayer.h"
#include "graphic/render/GenericRendererBuilder.h"
#include "resources/geometry/aabbox/AABBoxModel.h"

namespace urchin {

    ModelDisplayer::ModelDisplayer(Model *model, const Matrix4<float>& projectionMatrix, DisplayMode displayMode, std::shared_ptr<RenderTarget> renderTarget,
                                   std::shared_ptr<Shader> shader, CustomModelShaderVariable* customModelShaderVariable) :
            model(model),
            displayMode(displayMode),
            renderTarget(std::move(renderTarget)),
            shader(std::move(shader)),
            customModelShaderVariable(customModelShaderVariable) {

        for (auto& constMesh : model->getConstMeshes()->getConstMeshes()) {
            auto meshRendererBuilder = std::make_unique<GenericRendererBuilder>(this->renderTarget, this->shader, ShapeType::TRIANGLE);
            meshRendererBuilder
                ->enableDepthOperations()
                ->addData(constMesh->getBaseVertices())
                ->addData(constMesh->getTextureCoordinates())
                ->addData(constMesh->getBaseNormals())
                ->addData(constMesh->getBaseTangents())
                ->indices(constMesh->getTrianglesIndices())
                ->addShaderData(sizeof(projectionMatrix), &projectionMatrix) //binding 0
                ->addShaderData(sizeof(meshData), &meshData); //binding 1
            if (customModelShaderVariable) {
                customModelShaderVariable->setupMeshRenderer(meshRendererBuilder); //binding 2 & 3
            }

            if (displayMode == DEFAULT_MODE) {
                TextureParam::ReadMode textureReadMode = constMesh->getMaterial()->isRepeatableTextures() ? TextureParam::ReadMode::REPEAT : TextureParam::ReadMode::EDGE_CLAMP;
                TextureParam textureParam = TextureParam::build(textureReadMode, TextureParam::LINEAR, TextureParam::ANISOTROPY);

                meshRendererBuilder
                    ->addTextureReader(TextureReader::build(constMesh->getMaterial()->getDiffuseTexture(), textureParam))
                    ->addTextureReader(TextureReader::build(constMesh->getMaterial()->getNormalTexture(), textureParam));
            }

            meshRenderers.push_back(meshRendererBuilder->build());
        }

        model->addObserver(this, Model::MESH_UPDATED);
    }

    ModelDisplayer::~ModelDisplayer() {
        model->removeObserver(this, Model::MESH_UPDATED);
    }

    void ModelDisplayer::onCameraProjectionUpdate(const Camera* camera) {
        for (auto& meshRenderer : meshRenderers) {
            meshRenderer->updateShaderData(0, &camera->getProjectionMatrix());
        }
    }

    void ModelDisplayer::notify(Observable* observable, int notificationType) {
        if (auto* model = dynamic_cast<Model*>(observable)) {
            if (notificationType == Model::MESH_UPDATED) {
                unsigned int meshIndex = 0;
                for (auto& meshRenderer : meshRenderers) {
                    const Mesh* mesh = model->getMeshes()->getMesh(meshIndex);
                    meshRenderer->updateData(0, mesh->getVertices());
                    meshRenderer->updateData(2, mesh->getNormals());
                    meshRenderer->updateData(3, mesh->getTangents());

                    meshIndex++;
                }
            }
        }
    }

    void ModelDisplayer::display(const Matrix4<float>& viewMatrix) const {
        unsigned int meshIndex = 0;
        for (auto& meshRenderer : meshRenderers) {
            meshData.viewMatrix = viewMatrix;
            meshData.modelMatrix = model->getTransform().getTransformMatrix();
            meshData.normalMatrix = (displayMode == DEFAULT_MODE) ? model->getTransform().getTransformMatrix().inverse().transpose() : Matrix4<float>();
            meshData.ambientFactor = model->getConstMeshes()->getConstMesh(meshIndex)->getMaterial()->getAmbientFactor();

            meshRenderer->updateShaderData(1, &meshData);

            if (customModelShaderVariable) {
                customModelShaderVariable->loadCustomShaderVariables(meshRenderer);
            }

            //TODO renderTarget->display(meshRenderer);
            meshIndex++;
        }
    }

    void ModelDisplayer::drawBBox(const Matrix4<float>& projectionMatrix, const Matrix4<float>& viewMatrix) const {
        AABBoxModel aabboxModel(model->getAABBox());
        aabboxModel.initialize(renderTarget);
        aabboxModel.onCameraProjectionUpdate(projectionMatrix);
        aabboxModel.display(viewMatrix);
    }

    void ModelDisplayer::drawBaseBones(const Matrix4<float>& projectionMatrix, const Matrix4<float>& viewMatrix) const {
        if (model->getMeshes()) {
            for (unsigned int m = 0; m < model->getMeshes()->getNumberMeshes(); ++m) {
                Matrix4<float> modelViewMatrix = viewMatrix * model->getTransform().getTransformMatrix();
                model->getMeshes()->getMesh(m)->drawBaseBones(renderTarget, projectionMatrix, modelViewMatrix);
            }
        }
    }

}
