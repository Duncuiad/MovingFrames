#include "OpenGLPG_Base.h"

#include "Filepath.h"

#include "Serializer.h"

#include <format>

Filepath::Filepath()
    : Filepath {""}
{}

Filepath::Filepath(const char* aPath)
    : myPath {aPath}
{}

Filepath::Filepath(const std::string& aPath)
    : myPath {aPath}
{}

Filepath::Filepath(const Filepath& aPath)
    : myPath {aPath.GetBuffer()}
{}

void Filepath::Serialize(Serializer& aSerializer)
{
    aSerializer.Process("myPath", myPath);
}

const char* Filepath::GetBuffer() const
{
    return myPath.data();
}

bool Filepath::HasExtension(const char* anExtension) const
{
    return myPath.ends_with(std::format(".{}", anExtension));
}

void Filepath::Append(const char* aString)
{
    myPath.append(aString);
}

bool Filepath::operator==(const Filepath& anOther) const
{
    return myPath == anOther.myPath;
}
