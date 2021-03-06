#pragma once

#include <string>
#include <memory>

#include <scene/ui/widget/Widget.h>
#include <scene/ui/widget/Position.h>
#include <scene/ui/widget/Size.h>
#include <scene/ui/widget/text/Text.h>
#include <graphic/render/GenericRenderer.h>

namespace urchin {

    class TextBox : public Widget {
        public:
            TextBox(Widget*, Position, Size, std::string);

            std::string getText();

        protected:
            void createOrUpdateWidget() override;
            void prepareWidgetRendering(float) override;

        private:
            bool onKeyPressEvent(unsigned int) override;
            bool onCharEvent(char32_t) override;
            void onResetState() override;

            void refreshText(unsigned int, bool);
            void computeCursorPosition();
            void computeCursorIndex(int);

            //properties
            static const unsigned int LETTER_SHIFT;
            static const unsigned int LETTER_AND_CURSOR_SHIFT;
            static const float CURSOR_BLINK_SPEED;
            const std::string nameSkin;

            //display information
            Text* text; //text of the text box (widget)
            std::u32string allText; //text of the text box (string)
            unsigned int maxWidthText; //maximum length of text can be displayed
            unsigned int startTextIndex; //index of the first letter to display
            unsigned int cursorIndex; //index of the cursor
            unsigned int cursorPosition; //position (in pixel) of the cursor
            float cursorBlink;

            //state
            enum TextBoxStates {
                ACTIVE,
                INACTIVE
            } state;

            //visual
            std::shared_ptr<Texture> texTextBoxDefault, texTextBoxFocus;
            std::shared_ptr<Texture> texCursorDiffuse;
            std::unique_ptr<GenericRenderer> textBoxRenderer, cursorRenderer;
    };

}
