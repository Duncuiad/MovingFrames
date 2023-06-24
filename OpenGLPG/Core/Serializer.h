#pragma once

#include "Assert.h"
#include "Filepath.h"
#include "Serializable.h"

#include <fstream>
#include <iostream>

class Serializer
{
public:
    virtual void Process(const char* aVariableName, Serializable& aVariable) = 0;
    virtual void Process(const char* aVariableName, int& aVariable) = 0;
};

template <typename Policy>
class SerializerT : public Serializer
{
public:
    SerializerT(const Filepath& aFile);
    ~SerializerT();

    void Process(const char* aVariableName, Serializable& aVariable) override;
    void Process(const char* aVariableName, int& aVariable) override;

private:
    friend class SavePolicy;
    friend class LoadPolicy;

    template <typename SerializableT>
    friend void Serialize(SerializerT<Policy>& aSerializer, SerializableT& aVariable);

    Policy::StreamType myFile;
    int myFileIndentation {0};
};

class SavePolicy
{
public:
    using StreamType = std::ofstream;
    using SerializerType = SerializerT<SavePolicy>;

    static bool EnterObject(SerializerType& aSerializer);
    static bool ExitObject(SerializerType& aSerializer);
    static std::string Indent(const std::string& aLine, const SerializerType& aSerializer);

    template <typename SerializableT>
    static void Process(const char* aVariableName, SerializableT& aVariable, SerializerType& aSerializer);
};

class LoadPolicy
{
public:
    using StreamType = std::ifstream;
    using SerializerType = SerializerT<LoadPolicy>;

    static bool EnterObject(SerializerType& aSerializer);
    static bool ExitObject(SerializerType& aSerializer);
    static std::string Indent(const std::string& aLine, const SerializerType& aSerializer);

    template <typename SerializableT>
    static void Process(const char* aVariableName, SerializableT& aVariable, SerializerType& aSerializer);
};

class SerializerSaver : public SerializerT<SavePolicy>
{
public:
    using Base = SerializerT<SavePolicy>;
    using Base::Base;
};
class SerializerLoader : public SerializerT<LoadPolicy>
{
public:
    using Base = SerializerT<LoadPolicy>;
    using Base::Base;
};

template <typename SerializableT>
void Serialize(SerializerT<SavePolicy>& aSerializer, SerializableT& aVariable);
template <typename SerializableT>
void Serialize(SerializerT<LoadPolicy>& aSerializer, SerializableT& aVariable);

// Implementations

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
inline void SerializerT<Policy>::Process(const char* aVariableName, int& aVariable)
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

    std::string lineBuffer;
    do
    {
        std::getline(openStream, lineBuffer);
        lineBuffer = Indent(lineBuffer, aSerializer);
        if (lineBuffer == std::string(aVariableName))
        {
            EnterObject(aSerializer);
            Serialize<SerializableT>(aSerializer, aVariable);
            ExitObject(aSerializer);
            break;
        }
    } while (!openStream.eof());
}

template <>
inline void Serialize<Serializable>(SerializerT<SavePolicy>& aSerializer, Serializable& aVariable)
{
    aVariable.Serialize(aSerializer);
}

template <>
inline void Serialize<Serializable>(SerializerT<LoadPolicy>& aSerializer, Serializable& aVariable)
{
    aVariable.Serialize(aSerializer);
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
