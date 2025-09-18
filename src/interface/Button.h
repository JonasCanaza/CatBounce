#pragma once
#include <string>

enum class ButtonState
{
    Normal,
    Hover,
    Pressed
};

struct Button
{
    double width;
    double height;
    double x;
    double y;
    int normalTexture;
    int hoverTexture;
    int pressedTexture;
    std::string text;
    ButtonState state;
    bool clicked;
};

Button CreateButton(double x, double y, double width, double height, std::string text);
void UpdateButton(Button& button);
void DrawButton(Button button);
bool IsMouseOverButton(Button button);