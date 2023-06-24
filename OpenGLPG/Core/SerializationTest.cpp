#include "OpenGLPG_Base.h"

#include "SerializationTest.h"

#include "Filepath.h"

namespace Test
{
void Test1::Serialize(Serializer& aSerializer)
{
    aSerializer.Process("myInt1", myInt1);
    aSerializer.Process("myInt2", myInt2);
}

void Test2::Serialize(Serializer& aSerializer)
{
    aSerializer.Process("myIntArray", myIntArray);
    aSerializer.Process("myTestArray", myTestArray);
}

void SerializationTest()
{
    {
        Test1 test1 {145, -127};
        SerializerSaver saver1("../Data/Assets/Test/test1.json");
        test1.Serialize(saver1);
    }

    {
        Test1 test1_01 {};
        SerializerLoader loader1("../Data/Assets/Test/test1.json");
        test1_01.Serialize(loader1);

        Test1 test1_02 {100, -35};
        Test2 test2;
        test2.myIntArray.PushBack(11);
        test2.myIntArray.PushBack(22);
        test2.myIntArray.PushBack(33);
        test2.myIntArray.PushBack(44);
        test2.myTestArray.PushBack(test1_01);
        test2.myTestArray.PushBack(test1_02);

        SerializerSaver saver2("../Data/Assets/Test/test2.json");
        test2.Serialize(saver2);
    }

    {
        Test2 test2_01 {};
        SerializerLoader loader2("../Data/Assets/Test/test2.json");
        test2_01.Serialize(loader2);
        SerializerSaver saver3("../Data/Assets/Test/test3.json");
        test2_01.Serialize(saver3);
    }
}
} // namespace Test
