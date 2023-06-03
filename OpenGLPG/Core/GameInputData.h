#pragma once

#include <initializer_list>
#include <string>
#include <string_view>
#include <unordered_map>

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

// const std::initializer_list<std::string_view> theInputs = {
//     "KeyboardLookRight",   "KeyboardLookLeft",    "KeyboardLookUp",       "KeyboardLookDown",
//     "GamepadLookHorizontal", "GamepadLookVertical", "KeyboardMoveForward", "KeyboardMoveBackward",
//     "KeyboardMoveLeft",   "KeyboardMoveRight", "KeyboardMoveUp",      "KeyboardMoveDown",    "GamepadMoveFrontal",
//     "GamepadMoveLateral", "GamepadMoveVertical"};
} // namespace GameInput

struct GameInputData
{
    GameInputData();
    void Reset();

    using InputContainer = std::unordered_map<std::string, float>;
    InputContainer myInputs;
};
