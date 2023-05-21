#include "OpenGLPG_Base.h"

#include "ModelLoader.h"

#include "Assert.h"

ModelLoader::ModelLoader(const Filepath& aModelFolder)
    : myModelFolder {aModelFolder}
{}

Model::Ptr ModelLoader::GetModel(const Filepath& aFilepath)
{
    if (myModels.contains(aFilepath))
    {
        return myModels.at(aFilepath);
    }
    Filepath fullPath {myModelFolder};
    fullPath.Append(aFilepath.GetBuffer());

    const auto& pair {myModels.try_emplace(aFilepath, new Model {fullPath})};
    return pair.first->second;
}
