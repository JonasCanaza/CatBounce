#include "InputSystem.h"
#include "sl.h"

void InitKey(KeyTracker& key)
{
    key.wasPressed = false;
    key.isPressed = false;
}

void UpdateKey(KeyTracker& key, int keyCode)
{
    key.wasPressed = key.isPressed;
    key.isPressed = slGetKey(keyCode);
}

KeyState GetKeyState(KeyTracker key)
{
    if (key.isPressed && !key.wasPressed)
    {
        return KeyState::KeyDown;
    }
    else if (key.isPressed && key.wasPressed)
    {
        return KeyState::KeyHeld;
    }
    else if (!key.isPressed && key.wasPressed)
    {
        return KeyState::KeyUp;
    }
    else
    {
        return KeyState::KeyIdle;
    }
}