#pragma once

#include "Filepath.h"
#include "Model.h"

#include <unordered_map>

class ModelLoader
{
public:
    ModelLoader(const Filepath& aMeshFolder);

    Model::Ptr GetModel(const Filepath& aFilepath);

private:
    std::unordered_map<Filepath, Model::Ptr> myModels;
    Filepath myModelFolder;
};
