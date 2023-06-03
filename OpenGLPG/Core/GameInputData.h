#pragma once

#include <initializer_list>
#include <map>
#include <string>

namespace GameInput
{
constexpr const char* theKeyboardLookRight {"KeyboardLookRight"};
constexpr const char* theKeyboardLookLeft {"KeyboardLookLeft"};
constexpr const char* theKeyboardLookUp {"KeyboardLookUp"};
constexpr const char* theKeyboardLookDown {"KeyboardLookDown"};
constexpr const char* theGamepadLookHorizontal {"GamepadLookHorizontal"};
constexpr const char* theGamepadLookVertical {"GamepadLookVertical"};

constexpr const char* theKeyboardMoveForward {"KeyboardMoveForward"};
constexpr const char* theKeyboardMoveBackward {"KeyboardMoveBackward"};
constexpr const char* theKeyboardMoveLeft {"KeyboardMoveLeft"};
constexpr const char* theKeyboardMoveRight {"KeyboardMoveRight"};
constexpr const char* theKeyboardMoveUp {"KeyboardMoveUp"};
constexpr const char* theKeyboardMoveDown {"KeyboardMoveDown"};
constexpr const char* theGamepadMoveFrontal {"GamepadMoveFrontal"};
constexpr const char* theGamepadMoveLateral {"GamepadMoveLateral"};
constexpr const char* theGamepadMoveVertical {"GamepadMoveVertical"};

const std::initializer_list<const char*> theInputs {
    theKeyboardLookRight,     theKeyboardLookLeft,    theKeyboardLookUp,      theKeyboardLookDown,
    theGamepadLookHorizontal, theGamepadLookVertical, theKeyboardMoveForward, theKeyboardMoveBackward,
    theKeyboardMoveLeft,      theKeyboardMoveRight,   theKeyboardMoveUp,      theKeyboardMoveDown,
    theGamepadMoveFrontal,    theGamepadMoveLateral,  theGamepadMoveVertical};
} // namespace GameInput

struct GameInputData
{
    GameInputData();
    void Reset();

    using InputContainer = std::map<std::string, float>;
    InputContainer myInputs;
};
