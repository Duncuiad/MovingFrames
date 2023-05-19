#include "OpenGLPG_Base.h"

#include "Core.h"

int main()
{
    Client client {};

    client.Init();

    while (client.IsRunning())
    {
        client.Update();
    }

    client.Shutdown();
    return 0;
}
