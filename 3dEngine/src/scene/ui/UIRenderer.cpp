#include <algorithm>
#include <utility>

#include <scene/ui/UIRenderer.h>
#include <resources/MediaManager.h>
#include <resources/font/Font.h>
#include <graphic/render/shader/builder/ShaderBuilder.h>

namespace urchin {

    //debug parameters
    bool DEBUG_DISPLAY_FONT_TEXTURE = false;

    UIRenderer::UIRenderer(RenderTarget& renderTarget, const std::unique_ptr<I18nService>& i18nService) :
            renderTarget(renderTarget),
            i18nService(i18nService.get()) {
        uiShader = ShaderBuilder::createShader("ui.vert.spv", "", "ui.frag.spv", std::unique_ptr<ShaderConstants>());
    }

    UIRenderer::~UIRenderer() {
        for (long i = (long)widgets.size() - 1; i >= 0; --i) {
            delete widgets[(std::size_t)i];
        }
    }

    void UIRenderer::onResize(unsigned int sceneWidth, unsigned int sceneHeight) {
        //widgets resize
        for (long i = (long)widgets.size() - 1; i >= 0; --i) {
            widgets[(std::size_t)i]->onResize(sceneWidth, sceneHeight);
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

    bool UIRenderer::onChar(char32_t unicodeCharacter) {
        if (unicodeCharacter > 0x00 && unicodeCharacter < 0xFF //accept 'Basic Latin' and 'Latin-1 Supplement'
                && unicodeCharacter > 0x1F //ignore 'Controls C0' unicode
                && (unicodeCharacter < 0x80 || unicodeCharacter > 0x9F) //ignore 'Controls C1' unicode
                && unicodeCharacter != 127 //ignore 'Delete' unicode
        ) {
            for (long i = (long) widgets.size() - 1; i >= 0; --i) {
                if (!widgets[(std::size_t) i]->onChar(unicodeCharacter)) {
                    return false;
                }
            }
        }
        return true;
    }

    bool UIRenderer::onMouseMove(double mouseX, double mouseY) {
        for (long i = (long)widgets.size() - 1; i >= 0; --i) {
            if (!widgets[(std::size_t)i]->onMouseMove((int)mouseX, (int)mouseY)) {
                return false;
            }
        }
        return true;
    }

    void UIRenderer::onDisable() {
        for (long i = (long)widgets.size() - 1; i >= 0; --i) {
            widgets[(std::size_t)i]->onResetState();
        }
    }

    void UIRenderer::addWidget(Widget* widget) {
        if (widget->getParent()) {
            throw std::runtime_error("Cannot add a widget having a parent to the UI renderer");
        }
        widgets.push_back(widget);

        widget->initialize(renderTarget, *uiShader, i18nService);
        widget->addObserver(this, Widget::SET_IN_FOREGROUND);
    }

    void UIRenderer::removeWidget(Widget* widget) {
        if (widget) {
            auto it = std::find(widgets.begin(), widgets.end(), widget);
            delete widget;
            widgets.erase(it);
        }
    }

    void UIRenderer::prepareRendering(float dt) {
        ScopeProfiler sp(Profiler::graphic(), "uiPreRendering");

        for (auto& widget : widgets) {
            widget->prepareRendering(dt);
        }

        //debug
        if (debugFont) {
            debugFont->prepareRendering();
        }
    }

}
