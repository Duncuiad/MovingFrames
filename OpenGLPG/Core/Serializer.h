#pragma once

#include "Assert.h"
#include "Filepath.h"
#include "MathDefines.h"
#include "Serializable.h"

#include <fstream>
#include <iostream>

class Serializer
{
public:
    virtual void Process(const char* aVariableName, Serializable& aVariable) = 0;
    virtual void Process(const char* aVariableName, SerializableDynamic*& aPtr) = 0;
    virtual void Process(const char* aVariableName, bool& aVariable) = 0;
    virtual void Process(const char* aVariableName, int& aVariable) = 0;
    virtual void Process(const char* aVariableName, unsigned int& aVariable) = 0;
    virtual void Process(const char* aVariableName, float& aVariable) = 0;
    virtual void Process(const char* aVariableName, Vec2& aVariable) = 0;
    virtual void Process(const char* aVariableName, Vec3& aVariable) = 0;
    virtual void Process(const char* aVariableName, Vec4& aVariable) = 0;
    virtual void Process(const char* aVariableName, Quat& aVariable) = 0;
    virtual void Process(const char* aVariableName, std::string& aVariable) = 0;
};

template <typename Policy>
class SerializerT : public Serializer
{
public:
    SerializerT(const Filepath& aFile);
    ~SerializerT();

    void Process(const char* aVariableName, Serializable& aVariable) override;
    void Process(const char* aVariableName, SerializableDynamic*& aPtr) override;
    void Process(const char* aVariableName, bool& aVariable) override;
    void Process(const char* aVariableName, int& aVariable) override;
    void Process(const char* aVariableName, unsigned int& aVariable) override;
    void Process(const char* aVariableName, float& aVariable) override;
    void Process(const char* aVariableName, Vec2& aVariable) override;
    void Process(const char* aVariableName, Vec3& aVariable) override;
    void Process(const char* aVariableName, Vec4& aVariable) override;
    void Process(const char* aVariableName, Quat& aVariable) override;
    void Process(const char* aVariableName, std::string& aVariable) override;

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

#include "Serializer.inl"
