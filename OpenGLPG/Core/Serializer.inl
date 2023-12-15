#include "Serializer.h"
#pragma once

template <typename Policy>
inline SerializerT<Policy>::SerializerT(const Filepath& aFile)
{
    myFile.open(aFile.GetBuffer());
    ASSERT(myFile.is_open(), "Failed opening file to serialize {}", aFile.GetBuffer());
}

template <typename Policy>
inline SerializerT<Policy>::~SerializerT()
{
    myFile.close();
}

template <typename Policy>
inline void SerializerT<Policy>::Process(const char* aVariableName, Serializable& aVariable)
{
    Policy::Process(aVariableName, aVariable, *this);
}

template <typename Policy>
inline void SerializerT<Policy>::Process(const char* aVariableName, SerializableDynamic*& aPtr)
{
    Policy::Process(aVariableName, aPtr, *this);
}

template <typename Policy>
inline void SerializerT<Policy>::Process(const char* aVariableName, bool& aVariable)
{
    Policy::Process(aVariableName, aVariable, *this);
}

template <typename Policy>
inline void SerializerT<Policy>::Process(const char* aVariableName, int& aVariable)
{
    Policy::Process(aVariableName, aVariable, *this);
}
template <typename Policy>
inline void SerializerT<Policy>::Process(const char* aVariableName, unsigned int& aVariable)
{
    Policy::Process(aVariableName, aVariable, *this);
}

template <typename Policy>
inline void SerializerT<Policy>::Process(const char* aVariableName, float& aVariable)
{
    Policy::Process(aVariableName, aVariable, *this);
}

template <typename Policy>
inline void SerializerT<Policy>::Process(const char* aVariableName, Vec2& aVariable)
{
    Policy::Process(aVariableName, aVariable, *this);
}

template <typename Policy>
inline void SerializerT<Policy>::Process(const char* aVariableName, Vec3& aVariable)
{
    Policy::Process(aVariableName, aVariable, *this);
}

template <typename Policy>
inline void SerializerT<Policy>::Process(const char* aVariableName, Vec4& aVariable)
{
    Policy::Process(aVariableName, aVariable, *this);
}

template <typename Policy>
inline void SerializerT<Policy>::Process(const char* aVariableName, Quat& aVariable)
{
    Policy::Process(aVariableName, aVariable, *this);
}

template <typename Policy>
inline void SerializerT<Policy>::Process(const char* aVariableName, Dodec& aVariable)
{
    Policy::Process(aVariableName, aVariable, *this);
}

template <typename Policy>
inline void SerializerT<Policy>::Process(const char* aVariableName, std::string& aVariable)
{
    Policy::Process(aVariableName, aVariable, *this);
}

template <typename SerializableT>
inline void SavePolicy::Process(const char* aVariableName, SerializableT& aVariable, SerializerType& aSerializer)
{
    StreamType& openStream {aSerializer.myFile};
    ASSERT(openStream.is_open(), "File is not open!");

    openStream << Indent(aVariableName, aSerializer) << std::endl;
    EnterObject(aSerializer);
    Serialize<SerializableT>(aSerializer, aVariable);
    ExitObject(aSerializer);
}

template <typename SerializableT>
inline void LoadPolicy::Process(const char* aVariableName, SerializableT& aVariable, SerializerType& aSerializer)
{
    StreamType& openStream {aSerializer.myFile};
    ASSERT(openStream.is_open(), "File is not open!");
    ASSERT(!openStream.fail(), "Error while reading file");

    bool foundVariable {false};
    const std::streampos oldPos {openStream.tellg()};

    std::string lineBuffer;
    do
    {
        std::getline(openStream, lineBuffer);
        lineBuffer = Indent(lineBuffer, aSerializer);
        if (lineBuffer == std::string(aVariableName))
        {
            foundVariable = true;
            EnterObject(aSerializer);
            Serialize<SerializableT>(aSerializer, aVariable);
            ExitObject(aSerializer);
            break;
        }
    } while (!openStream.eof());

    if (!foundVariable)
    {
        openStream.clear();
        openStream.seekg(oldPos);
        std::cerr << "Serialization Error: couldn't find variable '" << aVariableName << "'" << std::endl;
    }
}

template <>
inline void Serialize(SerializerT<SavePolicy>& aSerializer, Serializable& aVariable)
{
    aVariable.Serialize(aSerializer);
}

template <>
inline void Serialize(SerializerT<LoadPolicy>& aSerializer, Serializable& aVariable)
{
    aVariable.Serialize(aSerializer);
}

template <>
inline void Serialize(SerializerT<SavePolicy>& aSerializer, SerializableDynamic*& aPtr)
{
    ASSERT(aPtr, "Serializing a nullptr");
    aSerializer.myFile << SavePolicy::Indent(aPtr->GetSubtypeId(), aSerializer) << std::endl;
    aPtr->Serialize(aSerializer);
}

template <>
inline void Serialize(SerializerT<LoadPolicy>& aSerializer, SerializableDynamic*& aPtr)
{
    ASSERT(!aPtr, "Serializing a variable that already exists");

    std::string subtypeId;
    std::getline(aSerializer.myFile, subtypeId);
    subtypeId = LoadPolicy::Indent(subtypeId, aSerializer);
    ASSERT(SerializationFactory::GetBuilders().contains(subtypeId),
           "The provided subtype {} hasn't been registered. Have you forgotten calling REGISTER_SUBTYPE?", subtypeId);
    aPtr = SerializationFactory::GetBuilders()[subtypeId]();
    aPtr->Serialize(aSerializer);
}

template <>
inline void Serialize(SerializerT<SavePolicy>& aSerializer, bool& aVariable)
{
    aSerializer.myFile << SavePolicy::Indent(aVariable ? "1" : "0", aSerializer) << std::endl;
}

template <>
inline void Serialize(SerializerT<LoadPolicy>& aSerializer, bool& aVariable)
{
    aSerializer.myFile >> aVariable;
}

template <>
inline void Serialize(SerializerT<SavePolicy>& aSerializer, int& aVariable)
{
    aSerializer.myFile << SavePolicy::Indent(std::to_string(aVariable), aSerializer) << std::endl;
}

template <>
inline void Serialize(SerializerT<LoadPolicy>& aSerializer, int& aVariable)
{
    aSerializer.myFile >> aVariable;
}

template <>
inline void Serialize(SerializerT<SavePolicy>& aSerializer, unsigned int& aVariable)
{
    aSerializer.myFile << SavePolicy::Indent(std::to_string(aVariable), aSerializer) << std::endl;
}

template <>
inline void Serialize(SerializerT<LoadPolicy>& aSerializer, unsigned int& aVariable)
{
    aSerializer.myFile >> aVariable;
}

template <>
inline void Serialize(SerializerT<SavePolicy>& aSerializer, float& aVariable)
{
    aSerializer.myFile << SavePolicy::Indent(std::to_string(aVariable), aSerializer) << std::endl;
}

template <>
inline void Serialize(SerializerT<LoadPolicy>& aSerializer, float& aVariable)
{
    aSerializer.myFile >> aVariable;
}

template <>
inline void Serialize(SerializerT<SavePolicy>& aSerializer, Vec2& aVariable)
{
    aSerializer.myFile << SavePolicy::Indent(std::to_string(aVariable[0]), aSerializer)
                       << SavePolicy::Indent(std::to_string(aVariable[1]), aSerializer) << std::endl;
}

template <>
inline void Serialize(SerializerT<LoadPolicy>& aSerializer, Vec2& aVariable)
{
    aSerializer.myFile >> aVariable[0] >> aVariable[1];
}

template <>
inline void Serialize(SerializerT<SavePolicy>& aSerializer, Vec3& aVariable)
{
    aSerializer.myFile << SavePolicy::Indent(std::to_string(aVariable[0]), aSerializer)
                       << SavePolicy::Indent(std::to_string(aVariable[1]), aSerializer)
                       << SavePolicy::Indent(std::to_string(aVariable[2]), aSerializer) << std::endl;
}

template <>
inline void Serialize(SerializerT<LoadPolicy>& aSerializer, Vec3& aVariable)
{
    aSerializer.myFile >> aVariable[0] >> aVariable[1] >> aVariable[2];
}

template <>
inline void Serialize(SerializerT<SavePolicy>& aSerializer, Vec4& aVariable)
{
    aSerializer.myFile << SavePolicy::Indent(std::to_string(aVariable[0]), aSerializer)
                       << SavePolicy::Indent(std::to_string(aVariable[1]), aSerializer)
                       << SavePolicy::Indent(std::to_string(aVariable[2]), aSerializer)
                       << SavePolicy::Indent(std::to_string(aVariable[3]), aSerializer) << std::endl;
}

template <>
inline void Serialize(SerializerT<LoadPolicy>& aSerializer, Vec4& aVariable)
{
    aSerializer.myFile >> aVariable[0] >> aVariable[1] >> aVariable[2] >> aVariable[3];
}

template <>
inline void Serialize(SerializerT<SavePolicy>& aSerializer, Quat& aVariable)
{
    aSerializer.myFile << SavePolicy::Indent(std::to_string(aVariable.w), aSerializer)
                       << SavePolicy::Indent(std::to_string(aVariable.x), aSerializer)
                       << SavePolicy::Indent(std::to_string(aVariable.y), aSerializer)
                       << SavePolicy::Indent(std::to_string(aVariable.z), aSerializer) << std::endl;
}

template <>
inline void Serialize(SerializerT<LoadPolicy>& aSerializer, Quat& aVariable)
{
    aSerializer.myFile >> aVariable.w >> aVariable.x >> aVariable.y >> aVariable.z;
}

template <>
inline void Serialize(SerializerT<SavePolicy>& aSerializer, Dodec& aVariable)
{
    const auto [o, i, n, in] = aVariable.GetCoordsININ();
    aSerializer.myFile << SavePolicy::Indent(std::to_string(o), aSerializer)
                       << SavePolicy::Indent(std::to_string(i), aSerializer)
                       << SavePolicy::Indent(std::to_string(n), aSerializer)
                       << SavePolicy::Indent(std::to_string(in), aSerializer) << std::endl;
}

template <>
inline void Serialize(SerializerT<LoadPolicy>& aSerializer, Dodec& aVariable)
{
    int o, i, n, in;
    aSerializer.myFile >> o >> i >> n >> in;
    aVariable = {o, i, n, in};
}

template <>
inline void Serialize(SerializerT<SavePolicy>& aSerializer, std::string& aVariable)
{
    aSerializer.myFile << SavePolicy::Indent(aVariable, aSerializer) << std::endl;
}

template <>
inline void Serialize(SerializerT<LoadPolicy>& aSerializer, std::string& aVariable)
{
    aSerializer.myFile >> aVariable;
}

template <typename SerializableT>
inline void Serialize(SerializerT<SavePolicy>& aSerializer, SerializableT& aVariable)
{
    ASSERT(false, "Trying to serialize a type that is not Serializable");
}

template <typename SerializableT>
inline void Serialize(SerializerT<LoadPolicy>& aSerializer, SerializableT& aVariable)
{
    ASSERT(false, "Trying to serialize a type that is not Serializable");
}

// template <typename FundamentalT, std::enable_if_t<std::is_fundamental<FundamentalT>::value, bool> = true>
// inline void Serialize(SerializerT<SavePolicy>& aSerializer, FundamentalT& aVariable)
//{
//     aSerializer.myFile << SavePolicy::Indent(std::to_string(aVariable), aSerializer) << std::endl;
// }
//
// template <typename FundamentalT, std::enable_if_t<std::is_fundamental<FundamentalT>::value, bool> = true>
// inline void Serialize(SerializerT<LoadPolicy>& aSerializer, FundamentalT& aVariable)
//{
//     aSerializer.myFile >> aVariable;
// }
//
// template <typename SerializableT, std::enable_if_t<!std::is_fundamental<SerializableT> {}, bool>>
// inline void Serialize(SerializerT<SavePolicy>& aSerializer, SerializableT& aVariable)
//{
//     ASSERT(false, "Trying to serialize a type that is not Serializable");
// }
//
// template <typename SerializableT, std::enable_if_t<!std::is_fundamental<SerializableT> {}, bool>>
// inline void Serialize(SerializerT<LoadPolicy>& aSerializer, SerializableT& aVariable)
//{
//     ASSERT(false, "Trying to serialize a type that is not Serializable");
// }
