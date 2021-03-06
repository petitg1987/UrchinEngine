#include <scene/ui/widget/checkbox/CheckBox.h>
#include <resources/MediaManager.h>

namespace urchin {
    CheckBox::CheckBox(Widget* parent, Position position, Size size, std::string nameSkin) :
            Widget(parent, position, size),
            nameSkin(std::move(nameSkin)),
            bIsChecked(false) {
        if(parent) {
            CheckBox::createOrUpdateWidget();
        }
    }

    void CheckBox::createOrUpdateWidget() {
        //skin information
        auto checkBoxChunk = UISkinService::instance()->getSkinReader()->getUniqueChunk(true, "checkBox", DataAttribute("nameSkin", nameSkin));
        texChecked = loadTexture(checkBoxChunk.get(), "imageChecked");
        texUnchecked = loadTexture(checkBoxChunk.get(), "imageUnchecked");

        //visual
        std::vector<Point2<float>> vertexCoord = {
                Point2<float>(0.0f, 0.0f), Point2<float>((float)getWidth(), 0.0f), Point2<float>((float)getWidth(), (float)getHeight()),
                Point2<float>(0.0f, 0.0f), Point2<float>((float)getWidth(), (float)getHeight()), Point2<float>(0.0f, (float)getHeight())
        };
        std::vector<Point2<float>> textureCoord = {
                Point2<float>(0.0f, 0.0f), Point2<float>(1.0f, 0.0f), Point2<float>(1.0f, 1.0f),
                Point2<float>(0.0f, 0.0f), Point2<float>(1.0f, 1.0f), Point2<float>(0.0f, 1.0f)
        };
        checkBoxRenderer = setupUiRenderer("check box", ShapeType::TRIANGLE)
                ->addData(vertexCoord)
                ->addData(textureCoord)
                ->addUniformTextureReader(TextureReader::build(isChecked() ? texChecked : texUnchecked, TextureParam::buildLinear())) //binding 3
                ->enableTransparency()
                ->build();
    }

    std::shared_ptr<Texture> CheckBox::loadTexture(const DataChunk* checkBoxChunk, const std::string& chunkName) const {
        auto imageElem = UISkinService::instance()->getSkinReader()->getUniqueChunk(true, chunkName, DataAttribute(), checkBoxChunk);

        auto* img = MediaManager::instance()->getMedia<Image>(imageElem->getStringValue());
        auto tex = img->createTexture(false);
        img->release();
        return tex;
    }

    void CheckBox::setChecked(bool isChecked) {
        this->bIsChecked = isChecked;
        refreshTexture();
    }

    bool CheckBox::isChecked() const {
        return bIsChecked;
    }

    void CheckBox::refreshTexture() {
        if(checkBoxRenderer) {
            if (bIsChecked) {
                checkBoxRenderer->updateUniformTextureReader(0, TextureReader::build(texChecked, TextureParam::buildLinear()));
            } else {
                checkBoxRenderer->updateUniformTextureReader(0, TextureReader::build(texUnchecked, TextureParam::buildLinear()));
            }
        }
    }

    bool CheckBox::onKeyReleaseEvent(unsigned int) {
        if (getWidgetState() == FOCUS) {
            switchValue();
            return false;
        }
        return true;
    }

    void CheckBox::switchValue() {
        setChecked(!bIsChecked);

        for (auto& eventListener : getEventListeners()) {
            eventListener->onValueChange(this);
        }
    }

    void CheckBox::prepareWidgetRendering(float) {
        updateTranslateVector(checkBoxRenderer.get(), Vector2<int>(getGlobalPositionX(), getGlobalPositionY()));
        checkBoxRenderer->enableRenderer();
    }
}
