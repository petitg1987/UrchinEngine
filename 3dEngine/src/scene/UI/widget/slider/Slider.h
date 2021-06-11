#pragma once

#include <string>
#include <memory>
#include <vector>

#include <scene/UI/widget/Widget.h>
#include <scene/UI/widget/Position.h>
#include <scene/UI/widget/Size.h>
#include <scene/UI/widget/text/Text.h>
#include <scene/UI/EventListener.h>
namespace urchin {

    class Slider : public Widget {
        public:
            static Slider* newSlider(Widget*, Position, Size, std::string, const std::vector<std::string>&);
            static Slider* newTranslatableSlider(Widget*, Position, Size, std::string, const std::vector<std::string>&);

            void allowLoopOnValues(bool);

            unsigned int getSelectedIndex() const;
            void setSelectedIndex(unsigned int);

            void setLeftButtonEventListener(const std::shared_ptr<EventListener>&);
            void setRightButtonEventListener(const std::shared_ptr<EventListener>&);

        protected:
            void createOrUpdateWidget() override;
            void prepareWidgetRendering(float) override;

        private:
            Slider(Widget*, Position, Size, std::string, const std::vector<std::string>&, bool);

            std::string buttonsTextSkin, valuesTextSkin;
            std::string leftButtonString, rightButtonString;
            const std::vector<std::string> values;
            bool translatableValues;
            bool loopOnValuesEnabled;
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
                    ButtonSliderEventListener(Slider*, bool, bool);
                    void onMouseLeftClickRelease(Widget*) override;

                private:
                    Slider* slider;
                    bool isLeftButton;
                    bool loopOnValuesEnabled;
            };
    };

}
