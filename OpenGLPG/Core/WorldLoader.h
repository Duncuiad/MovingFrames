#pragma once

#include "Array.h"
#include "Filepath.h"

class World;

class WorldLoader
{
public:
    WorldLoader(const Filepath& aWorldFolder);
    void OnUnload();

    void SaveWorld(const Filepath& aFilename, World* aWorld);
    void LoadWorld(const Filepath& aFilename, World* aWorld);
    const Array<Filepath>& GetAvailableWorlds() const;

private:
    void AddAvailableWorld(const Filepath& aFilename);
    bool CheckFilepathValidity(const Filepath& aFilepath);

    Filepath myWorldFolder;
    Array<Filepath> myAvailableWorlds;
};
