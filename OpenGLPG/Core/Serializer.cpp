#include "OpenGLPG_Base.h"

#include "Serializer.h"

bool SavePolicy::EnterObject(SerializerType& aSerializer)
{
    aSerializer.myFile << Indent("{", aSerializer) << std::endl;
    ++aSerializer.myFileIndentation;
    return true;
}

bool SavePolicy::ExitObject(SerializerType& aSerializer)
{
    --aSerializer.myFileIndentation;
    ASSERT(aSerializer.myFileIndentation >= 0, "Invalid file indentation");
    aSerializer.myFile << std::string(aSerializer.myFileIndentation, '\t') << "}" << std::endl << std::endl;
    return true;
}

std::string SavePolicy::Indent(const std::string& aLine, const SerializerType& aSerializer)
{
    return std::string(aSerializer.myFileIndentation, '\t') + aLine;
}

bool LoadPolicy::EnterObject(SerializerType& aSerializer)
{
    std::string lineBuffer;
    std::getline(aSerializer.myFile, lineBuffer);
    lineBuffer = Indent(lineBuffer, aSerializer);
    ASSERT(lineBuffer == "{", "Expected an open brace, found: {}", lineBuffer.data());
    ++aSerializer.myFileIndentation;
    return true;
}

bool LoadPolicy::ExitObject(SerializerType& aSerializer)
{
    std::string lineBuffer;
    std::getline(aSerializer.myFile, lineBuffer);
    lineBuffer = Indent(lineBuffer, aSerializer);
    if (lineBuffer == "}")
    {
        std::getline(aSerializer.myFile, lineBuffer);
        ASSERT(Indent(lineBuffer, aSerializer) == "", "Invalid file format");
        --aSerializer.myFileIndentation;
        ASSERT(aSerializer.myFileIndentation >= 0, "Invalid file indentation");
        return true;
    }
    return false;
}

std::string LoadPolicy::Indent(const std::string& aLine, const SerializerType& /*aSerializer*/)
{
    const size_t indentEnd {aLine.find_first_not_of('\t')};
    return indentEnd == std::string::npos ? "" : aLine.substr(indentEnd);
}
