#pragma once

class Serializer;

class Serializable
{
public:
    virtual void Serialize(Serializer& aSerializer) = 0;
};
