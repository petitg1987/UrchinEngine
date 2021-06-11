#include <algorithm>
#include <utility>

#include <scene/UI/UIRenderer.h>
#include <resources/MediaManager.h>
#include <resources/font/Font.h>
#include <graphic/render/shader/builder/ShaderBuilder.h>

namespace urchin {

    //debug parameters
    bool DEBUG_DISPLAY_FONT_TEXTURE = false;

    UIRenderer::UIRenderer(std::shared_ptr<RenderTarget> renderTarget) :
            renderTarget(std::move(renderTarget)),
            i18nService(std::make_unique<I18nService>()){
        uiShader = ShaderBuilder::createShader("ui.vert.spv", "", "ui.frag.spv", std::unique_ptr<ShaderConstants>());
    }

    UIRenderer::~UIRenderer() {
        for (long i = (long)widgets.size() - 1; i >= 0; --i) {
            delete widgets[(std::size_t)i];
        }
    }

    void UIRenderer::onResize(unsigned int sceneWidth, unsigned int sceneHeight) {
        //orthogonal matrix with origin at top left screen
        mProjection.setValues(2.0f / (float)sceneWidth, 0.0f, -1.0f, 0.0f,
            0.0f, 2.0f / (float)sceneHeight, -1.0f, 0.0f,
            0.0f, 0.0f, 1.0f, 0.0f,
            0.0f, 0.0f, 0.0f, 1.0f);

        //widgets resize
        for (long i = (long)widgets.size() - 1; i >= 0; --i) {
            widgets[(std::size_t)i]->onResize(sceneWidth, sceneHeight, mProjection);
        }

        //debug
        if (DEBUG_DISPLAY_FONT_TEXTURE) {
            Font* font = MediaManager::instance()->getMedia<Font>("UI/fontText.ttf", {{"fontSize", "16"}, {"fontColor", "1.0 1.0 1.0"}});

            auto textureDisplayer = std::make_unique<TextureRenderer>(font->getTexture(), TextureRenderer::DEFAULT_VALUE);
            textureDisplayer->setPosition(TextureRenderer::USER_DEFINED_X, TextureRenderer::USER_DEFINED_Y);
            textureDisplayer->setSize(20.0f, (float)font->getDimensionTexture() + 20.0f, 20.0f, (float)font->getDimensionTexture() + 20.0f);
            textureDisplayer->enableTransparency();
            textureDisplayer->initialize("[DEBUG] font texture", renderTarget, sceneWidth, sceneHeight, -1.0f, -1.0f);
            debugFont = std::move(textureDisplayer);
            font->release();
        }
    }

    void UIRenderer::notify(Observable* observable, int notificationType) {
        if (auto* widget = dynamic_cast<Widget*>(observable)) {
            if (notificationType == Widget::SET_IN_FOREGROUND) {
                auto it = std::find(widgets.begin(), widgets.end(), widget);
                widgets.erase(it);
                widgets.push_back(widget);

                //reset the others widgets
                for (long i = (long)widgets.size() - 2; i >= 0; --i) {
                    widgets[(std::size_t)i]->onResetState();
                }
            }
        }
    }

    bool UIRenderer::onKeyPress(unsigned int key) {
        for (long i = (long)widgets.size() - 1; i >= 0; --i) {
            if (!widgets[(std::size_t)i]->onKeyPress(key)) {
                return false;
            }
        }
        return true;
    }

    bool UIRenderer::onKeyRelease(unsigned int key) {
        for (long i = (long)widgets.size() - 1; i >= 0; --i) {
            if (!widgets[(std::size_t)i]->onKeyRelease(key)) {
                return false;
            }
        }
        return true;
    }

    bool UIRenderer::onChar(unsigned int character) {
        for (long i = (long)widgets.size() - 1; i >= 0; --i) {
            if (!widgets[(std::size_t)i]->onChar(character)) {
                return false;
            }
        }
        return true;
    }

    bool UIRenderer::onMouseMove(int mouseX, int mouseY) {
        for (long i = (long)widgets.size() - 1; i >= 0; --i) {
            if (!widgets[(std::size_t)i]->onMouseMove(mouseX, mouseY)) {
                return false;
            }
        }
        return true;
    }

    void UIRenderer::onDisable() {
        for (long i = (long)widgets.size() - 1; i >= 0; --i) {
            if (widgets[(std::size_t)i]->isVisible()) {
                widgets[(std::size_t)i]->onResetState();
            }
        }
    }

    void UIRenderer::addWidget(Widget* widget) {
        if (widget->getParent()) {
            throw std::runtime_error("Cannot add a widget having a parent to the UI renderer");
        }
        widgets.push_back(widget);

        widget->initialize(renderTarget, uiShader, i18nService.get());
        widget->addObserver(this, Widget::SET_IN_FOREGROUND);
    }

    void UIRenderer::removeWidget(Widget* widget) {
        auto it = std::find(widgets.begin(), widgets.end(), widget);
        delete widget;
        widgets.erase(it);
    }

    void UIRenderer::prepareRendering(float dt) {
        ScopeProfiler sp(Profiler::graphic(), "uiPreRendering");

        for (auto& widget : widgets) {
            if (widget->isVisible()) {
                widget->prepareRendering(dt);
            }
        }

        //debug
        if (debugFont) {
            debugFont->prepareRendering();
        }
    }

}
