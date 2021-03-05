#ifndef URCHINENGINE_SLIDER_H
#define URCHINENGINE_SLIDER_H

#include <string>
#include <memory>
#include <vector>

#include "scene/UI/widget/Widget.h"
#include "scene/UI/widget/Position.h"
#include "scene/UI/widget/Size.h"
#include "scene/UI/widget/text/Text.h"
#include "scene/UI/EventListener.h"
namespace urchin {

    class Slider : public Widget {
        public:
            Slider(Widget*, Position, Size, std::string, const std::vector<std::string>&);
            Slider(Position, Size, std::string, const std::vector<std::string>&);

            void createOrUpdateWidget() override;

            unsigned int getSelectedIndex() const;
            void setSelectedIndex(unsigned int);

            void setLeftButtonEventListener(const std::shared_ptr<EventListener>&);
            void setRightButtonEventListener(const std::shared_ptr<EventListener>&);

            void display(const ShaderVar&, float) override;

        private:
            std::string buttonsTextSkin, valuesTextSkin;
            std::string leftButtonString, rightButtonString;
            const std::vector<std::string> values;
            unsigned int selectedIndex;

            //visual
            Text* leftButton;
            Text* rightButton;
            std::vector<Text*> valuesText;

            std::shared_ptr<EventListener> leftButtonEventListener;
            std::shared_ptr<EventListener> rightButtonEventListener;

            float timeInClickingState;
            float timeSinceLastChange;

            class ButtonSliderEventListener : public EventListener {
                public:
                    ButtonSliderEventListener(Slider*, bool);
                    void onClickRelease(Widget*) override;

                private:
                    Slider* slider;
                    bool isLeftButton;
            };
    };

}

#endif
