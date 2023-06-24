#pragma once

#include "Array.h"
#include "Serializable.h"
#include "Serializer.h"

namespace Test
{
class Test1 : public Serializable
{
public:
    Test1()
        : Test1(0, 0)
    {}
    Test1(int anInt1, int anInt2)
        : myInt1 {anInt1}
        , myInt2 {anInt2}
    {}
    Test1(const Test1& aTest1) { *this = aTest1; }
    const Test1& operator=(const Test1& aTest1)
    {
        myInt1 = aTest1.myInt1;
        myInt2 = aTest1.myInt2;
        return *this;
    }
    void Serialize(Serializer& aSerializer) override;

    int myInt1;
    int myInt2;
};

class Test2 : public Serializable
{
public:
    Test2() = default;
    void Serialize(Serializer& aSerializer) override;

    Array<int> myIntArray;
    Array<Test1> myTestArray;
};

void SerializationTest();
} // namespace Test
