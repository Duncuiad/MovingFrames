#include "OpenGLPG_Base.h"

#include "WorldLoader.h"

#include "Assert.h"
#include "Defines.h"
#include "Serializer.h"
#include "World.h"

#include <fstream>

WorldLoader::WorldLoader(const Filepath& aWorldFolder)
    : myWorldFolder {aWorldFolder}
{
    SerializerLoader loader {myWorldFolder + GLOBALNAME_WORLDINDEX};
    myAvailableWorlds.Serialize(loader);

    for (auto it = myAvailableWorlds.begin(); it < myAvailableWorlds.end();)
    {

        if (!CheckFilepathValidity(myWorldFolder + *it))
        {
            it = myAvailableWorlds.RemoveAt(it);
        }
        else
        {
            ++it;
        }
    }
}

void WorldLoader::OnUnload()
{
    SerializerSaver saver {myWorldFolder + GLOBALNAME_WORLDINDEX};
    myAvailableWorlds.Serialize(saver);
}

void WorldLoader::SaveWorld(const Filepath& aFilename, World* aWorld)
{
    ASSERT(aFilename != Filepath {}, "Invalid name for asset!");
    ASSERT(aWorld != nullptr, "Invalid world");

    {
        SerializerSaver saver {myWorldFolder + aFilename};
        saver.Process("myWorldSettings", aWorld->myWorldSettings);

        // @improvement: This needs to be kept in sync with the Array class deserialization parsing. Maybe let's make it
        // smell a bit less
        int entityCount {static_cast<int>(aWorld->myEntities.size())};
        saver.Process("myCount", entityCount);
        // @note: I could use the uid instead of this counter. It wouldn't have much meaning to have uids in the level
        // file at the moment though
        int counter {0};
        for (auto& [uid, entity] : aWorld->myEntities)
        {
            saver.Process(std::to_string(counter).data(), entity);
            ++counter;
        }
    }

    AddAvailableWorld(aFilename);
}

void WorldLoader::LoadWorld(const Filepath& aFilename, World* aWorld)
{
    if (aFilename != "")
    {
        SerializerLoader loader(myWorldFolder + aFilename);
        loader.Process("myWorldSettings", aWorld->myWorldSettings);
        aWorld->myEntityQueue.Serialize(loader);
    }
}

const Array<Filepath>& WorldLoader::GetAvailableWorlds() const
{
    return myAvailableWorlds;
}

void WorldLoader::AddAvailableWorld(const Filepath& aFilename)
{
    if (CheckFilepathValidity(myWorldFolder + aFilename) && !myAvailableWorlds.Contains(aFilename))
    {
        myAvailableWorlds.PushBack(aFilename);
    }
}

bool WorldLoader::CheckFilepathValidity(const Filepath& aFilepath)
{
    std::ifstream stream;
    stream.open(aFilepath.GetBuffer());
    return stream.is_open();
}
