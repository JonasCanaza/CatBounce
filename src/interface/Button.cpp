#include "Button.h"
#include "sl.h"

Button CreateButton(double x, double y, double width, double height, std::string text)
{
    Button button;

    button.x = x + width / 2.0;
    button.y = y - height / 2.0;
    button.width = width;
    button.height = height;
    button.normalTexture = 0;
    button.hoverTexture = 0;
    button.pressedTexture = 0;
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

        textureToUse = button.normalTexture;
        slSetForeColor(1, 1, 1, 1);

        break;
    case ButtonState::Hover:

        textureToUse = button.hoverTexture;
        slSetForeColor(0.5, 0.5, 0.5, 1);

        break;
    case ButtonState::Pressed:

        textureToUse = button.pressedTexture;
        slSetForeColor(0.2, 0.2, 0.2, 1);

        break;
    default:

        // THERE ARE NO MORE BUTTON STATES

        break;
    }

    slRectangleFill(button.x, button.y, button.width, button.height);
    //slSprite(textureToUse, button.x, button.y, button.width, button.height);
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