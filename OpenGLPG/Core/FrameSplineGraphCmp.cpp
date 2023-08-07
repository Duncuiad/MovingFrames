#include "OpenGLPG_Base.h"

#include "FrameSplineGraphCmp.h"

#include "Serializer.h"

REGISTER_SUBTYPE(FrameSplineGraphCmp)

FrameSplineGraphCmp::FrameSplineGraphCmp()
{
    glGenVertexArrays(1, &myVAO);
    glGenBuffers(1, &myVBO);
    glGenBuffers(1, &myEBO);
}

FrameSplineGraphCmp::~FrameSplineGraphCmp()
{
    glDeleteBuffers(1, &myEBO);
    glDeleteBuffers(1, &myVBO);
    glDeleteVertexArrays(1, &myVAO);
}

void FrameSplineGraphCmp::Draw(const DrawParams& someParams) const
{
    myShader->Use();
    myShader->SetUniformMat4("Model", someParams.myModelMatrix);
    myShader->SetUniformMat4("View", someParams.myViewMatrix);
    myShader->SetUniformMat4("ModelView", someParams.myModelViewMatrix);
    myShader->SetUniformMat4("Projection", someParams.myProjectionMatrix);
    myShader->SetUniformMat4("WorldToClip", someParams.myWorldToClipMatrix);
    glBindVertexArray(myVAO);
    glDrawElements(GL_LINES, myIndices.Count(), GL_UNSIGNED_INT, 0);
    glBindVertexArray(0);
}

void FrameSplineGraphCmp::Serialize(Serializer& aSerializer)
{
    aSerializer.Process("myShader", myShaderAsset);
    aSerializer.Process("myKeys", myKeys);

    {
        int style {static_cast<int>(myStyle)};
        aSerializer.Process("myStyle", style);
        myStyle = static_cast<DisplayStyle>(style);
    }

    UpdateBuffers();
}

void FrameSplineGraphCmp::SetDisplayStyle(DisplayStyle aStyle)
{
    myStyle = aStyle;
    UpdateBuffers();
}

void FrameSplineGraphCmp::SetKeys(const Array<Key>& someKeys)
{
    myKeys = someKeys;
    UpdateBuffers();
}

void FrameSplineGraphCmp::UpdateBuffers()
{
    glBindVertexArray(myVAO);
    {
        glBindBuffer(GL_ARRAY_BUFFER, myVBO);
        glBufferData(GL_ARRAY_BUFFER, myKeys.Count() * sizeof(Key), myKeys.GetBuffer(), GL_DYNAMIC_DRAW);

        myIndices.RemoveAll();
        const unsigned int keyCount {static_cast<unsigned int>(myKeys.Count())};
        for (unsigned int i = 0; i + 1 < keyCount; ++i)
        {
            myIndices.PushBack(i);
            myIndices.PushBack(i + 1);
        }
        glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, myEBO);
        glBufferData(GL_ELEMENT_ARRAY_BUFFER, myIndices.Count() * sizeof(unsigned int), myIndices.GetBuffer(),
                     GL_DYNAMIC_DRAW);
    }

    {
        // key positions
        glEnableVertexAttribArray(0);
        glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, sizeof(Key), (void*)offsetof(Key, myPosition));
        // key right
        glEnableVertexAttribArray(1);
        glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, sizeof(Key), (void*)offsetof(Key, myRight));
        // key up
        glEnableVertexAttribArray(2);
        glVertexAttribPointer(2, 3, GL_FLOAT, GL_FALSE, sizeof(Key), (void*)offsetof(Key, myUp));
        // key forward
        glEnableVertexAttribArray(3);
        glVertexAttribPointer(3, 3, GL_FLOAT, GL_FALSE, sizeof(Key), (void*)offsetof(Key, myForward));
        // key velocity
        glEnableVertexAttribArray(4);
        glVertexAttribPointer(4, 3, GL_FLOAT, GL_FALSE, sizeof(Key), (void*)offsetof(Key, myVelocity));
        // key angular velocity
        glEnableVertexAttribArray(5);
        glVertexAttribPointer(5, 3, GL_FLOAT, GL_FALSE, sizeof(Key), (void*)offsetof(Key, myAngularVelocity));
    }
    glBindVertexArray(0);
}

FrameSplineGraphCmp::Key::Key(const Vec3& aPosition, const Vec3& aRight, const Vec3& anUp, const Vec3& aForward,
                              const Vec3& aVelocity, const Vec3& anAngularVelocity)
    : myPosition {aPosition}
    , myRight {aRight}
    , myUp {anUp}
    , myForward {aForward}
    , myVelocity {aVelocity}
    , myAngularVelocity {anAngularVelocity}
{}

void FrameSplineGraphCmp::Key::Serialize(Serializer& aSerializer)
{
    aSerializer.Process("myPosition", myPosition);
    aSerializer.Process("myRight", myRight);
    aSerializer.Process("myUp", myUp);
    aSerializer.Process("myForward", myForward);
    aSerializer.Process("myVelocity", myVelocity);
    aSerializer.Process("myAngularVelocity", myAngularVelocity);
}
