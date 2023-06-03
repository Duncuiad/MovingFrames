#include "OpenGLPG_Base.h"

#include "GameInputData.h"

#include <string>
#include <vector>

GameInputData::GameInputData()
{
    for (const char* input : GameInput::theInputs)
    {
        myInputs[input] = 0.f;
    }
}

void GameInputData::Reset()
{
    for (auto& [name, value] : myInputs)
    {
        value = 0.f;
    }
}
