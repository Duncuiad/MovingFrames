#include "OpenGLPG_Base.h"

#include "SerializationTest.h"

#include "Component.h"
#include "Filepath.h"
#include "GraphCmp.h"
#include "TransformCmp.h"

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

    {
        std::vector<Component::Ptr> compArray;
        TransformCmp* trCmp {new TransformCmp {}};
        trCmp->SetTransform(Transform {glm::identity<glm::mat3>(), Vec3 {1.f, 2.f, 3.f}});
        compArray.emplace_back(trCmp);
        compArray.emplace_back(new GraphCmp {"backpack/backpack.obj", "basic.shader"});

        Array<SerializableDynamic*, true> test4 {};
        for (const auto& cmp : compArray)
        {
            test4.PushBack(cmp.get());
        }

        SerializerSaver saver4("../Data/Assets/Test/test4.json");
        test4.Serialize(saver4);
    }

    {
        Array<SerializableDynamic*, true> test6 {};

        SerializerLoader loader5("../Data/Assets/Test/test4.json");
        test6.Serialize(loader5);

        std::vector<Component::Ptr> compArray;
        for (int i = 0; i < test6.Count(); ++i)
        {
            compArray.emplace_back(static_cast<Component*>(test6[i]));
        }
    }

    {
        {
            Array<Component::Ptr, true> test7;
            TransformCmp* trCmp {new TransformCmp {}};
            trCmp->SetTransform(Transform {glm::identity<glm::mat3>(), Vec3 {1.f, 2.f, 3.f}});
            test7.EmplaceBack(trCmp);
            test7.EmplaceBack(new GraphCmp {"backpack/backpack.obj", "basic.shader"});

            SerializerSaver saver7("../Data/Assets/Test/test7.json");
            test7.Serialize(saver7);
        }

        {
            Array<Component::Ptr, true> test8 {};

            SerializerLoader loader8("../Data/Assets/Test/test7.json");
            test8.Serialize(loader8);
        }
    }
}
} // namespace Test
