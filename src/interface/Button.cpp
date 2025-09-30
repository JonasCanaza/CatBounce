#include "Button.h"
#include "../Game.h"
#include "../utilities/Constants.h"

#include "sl.h"

// TEXTURES

int normalButtonTexture;
int hoverButtonTexture;
int pressedButtonTexture;

// SOUND

int buttonPressedSound;

void InitButton()
{
    normalButtonTexture = slLoadTexture("res/images/ui/normalButton.png");
    hoverButtonTexture = slLoadTexture("res/images/ui/hoverButton.png");
    pressedButtonTexture = slLoadTexture("res/images/ui/pressedButton.png");

    buttonPressedSound = slLoadWAV("res/sound/ui/buttonPressed.wav");
}

Button CreateButton(double x, double y, double width, double height, std::string text)
{
    Button button;

    button.x = x + width / 2.0;
    button.y = y - height / 2.0;
    button.width = width;
    button.height = height;
    button.text = text;
    button.state = ButtonState::Normal;
    button.clicked = false;

    return button;
}

void UpdateButton(Button& button)
{
    button.clicked = false;

    if (IsMouseOverButton(button))
    {
        if (slGetMouseButton(SL_MOUSE_BUTTON_LEFT))
        {
            button.state = ButtonState::Pressed;
        }
        else
        {
            if (button.state == ButtonState::Pressed)
            {
                button.clicked = true;
            }

            button.state = ButtonState::Hover;
        }
    }
    else
    {
        button.state = ButtonState::Normal;
    }
}

void DrawButton(Button button)
{
    int textureToUse = 0;

    switch (button.state)
    {
    case ButtonState::Normal:

        textureToUse = normalButtonTexture;

        break;
    case ButtonState::Hover:

        textureToUse = hoverButtonTexture;

        break;
    case ButtonState::Pressed:

        textureToUse = pressedButtonTexture;

        break;
    default:

        // THERE ARE NO MORE BUTTON STATES

        break;
    }

    slSprite(textureToUse, button.x, button.y, button.width, button.height);
    slSetFont(CatBounce::specialFont, MENU_TEXT_SIZE);
    double textWidth = slGetTextWidth(button.text.c_str());
    double textHeight = slGetTextHeight(button.text.c_str());;
    slText(button.x - textWidth / 2.0, button.y - textHeight / 2.0, button.text.c_str());
}

bool IsMouseOverButton(Button button)
{
    double mouseX = slGetMouseX();
    double mouseY = slGetMouseY();

    return (mouseX >= button.x - button.width / 2 &&
        mouseX <= button.x + button.width / 2 &&
        mouseY >= button.y - button.height / 2 &&
        mouseY <= button.y + button.height / 2);
}