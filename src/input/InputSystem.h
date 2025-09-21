#pragma once

enum class KeyState
{
    KeyIdle,
    KeyDown,
    KeyHeld,
    KeyUp
};

struct KeyTracker
{
    bool wasPressed;
    bool isPressed;
};

void InitKey(KeyTracker& key);

void UpdateKey(KeyTracker& key, int keyCode);

KeyState GetKeyState(KeyTracker key);