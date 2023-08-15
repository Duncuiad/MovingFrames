#include "OpenGLPG_Base.h"

#include "FrameSplineGraphCmp.h"

#include "Serializer.h"

#include <glm/gtc/matrix_inverse.hpp>

REGISTER_SUBTYPE(FrameSplineGraphCmp)

FrameSplineGraphCmp::FrameSplineGraphCmp()
{
    GenerateBuffers(myKeyBuffers);
    GenerateBuffers(myControlKeyBuffers);
}

FrameSplineGraphCmp::~FrameSplineGraphCmp()
{
    DeleteBuffers(myControlKeyBuffers);
    DeleteBuffers(myKeyBuffers);
}

void FrameSplineGraphCmp::OnLoad(const LoadParams& someParams)
{
    Base::OnLoad(someParams);
    myControlKeyShader = someParams.myClientLoader.GetShaderLoader().GetShader(myControlKeyShaderAsset);
}

void FrameSplineGraphCmp::Draw(const DrawParams& someParams) const
{
    myShader->Use();
    myShader->SetUniformFloat("KeyScale", myDisplayData.myKeyScale);
    myShader->SetUniformFloat("TangentScale", myDisplayData.myTangentScale);

    myShader->SetUniformInt("IsControlKey", 0);
    myShader->SetUniformMat4("Model", someParams.myModelMatrix);
    myShader->SetUniformMat4("View", someParams.myViewMatrix);
    myShader->SetUniformMat4("ModelView", someParams.myModelViewMatrix);
    myShader->SetUniformMat3("ModelViewIT", glm::inverseTranspose(glm::mat3(someParams.myModelViewMatrix)));
    myShader->SetUniformMat4("Projection", someParams.myProjectionMatrix);
    myShader->SetUniformMat4("WorldToClip", someParams.myWorldToClipMatrix);
    glBindVertexArray(myKeyBuffers.myVAO);
    glDrawElements(GL_LINES, myKeys.Count() * 2, GL_UNSIGNED_INT, 0);
    glBindVertexArray(0);

    if (myShowControlKeys)
    {
        myControlKeyShader->Use();

        myControlKeyShader->SetUniformInt("IsControlKey", 1);

        myControlKeyShader->SetUniformMat4("Model", someParams.myModelMatrix);
        myControlKeyShader->SetUniformMat4("View", someParams.myViewMatrix);
        myControlKeyShader->SetUniformMat4("ModelView", someParams.myModelViewMatrix);
        myControlKeyShader->SetUniformMat3("ModelViewIT",
                                           glm::inverseTranspose(glm::mat3(someParams.myModelViewMatrix)));
        myControlKeyShader->SetUniformMat4("Projection", someParams.myProjectionMatrix);
        myControlKeyShader->SetUniformMat4("WorldToClip", someParams.myWorldToClipMatrix);
        glBindVertexArray(myControlKeyBuffers.myVAO);
        glDrawElements(GL_LINES, myControlKeys.Count() * 2, GL_UNSIGNED_INT, 0);
        glBindVertexArray(0);

        myControlKeyShader->SetUniformInt("IsControlKey", 0);
    }
}

void FrameSplineGraphCmp::Serialize(Serializer& aSerializer)
{
    aSerializer.Process("myShader", myShaderAsset);
    aSerializer.Process("myControlKeyShader", myControlKeyShaderAsset);

    {
        int style {static_cast<int>(myStyle)};
        aSerializer.Process("myStyle", style);
        myStyle = static_cast<DisplayStyle>(style);
    }

    aSerializer.Process("myKeyScale", myDisplayData.myKeyScale);
    aSerializer.Process("myTangentScale", myDisplayData.myTangentScale);
}

float FrameSplineGraphCmp::GetKeyScale() const
{
    return myDisplayData.myKeyScale;
}

float FrameSplineGraphCmp::GetTangentScale() const
{
    return myDisplayData.myTangentScale;
}

void FrameSplineGraphCmp::SetDisplayStyle(DisplayStyle aStyle)
{
    myStyle = aStyle;
}

void FrameSplineGraphCmp::SetKeyScale(float aScale)
{
    myDisplayData.myKeyScale = aScale;
}

void FrameSplineGraphCmp::SetTangentScale(float aScale)
{
    myDisplayData.myTangentScale = aScale;
}

void FrameSplineGraphCmp::SetKeys(const Array<Key>& someKeys)
{
    myKeys = someKeys;
    UpdateBuffers(myKeys, myKeyBuffers);
}

void FrameSplineGraphCmp::SetControlKeys(const Array<Key>& someKeys)
{
    myControlKeys = someKeys;
    UpdateBuffers(myControlKeys, myControlKeyBuffers);
}

void FrameSplineGraphCmp::GenerateBuffers(KeyFrameBuffers& someBuffersOut)
{
    glGenVertexArrays(1, &someBuffersOut.myVAO);
    glGenBuffers(1, &someBuffersOut.myVBO);
    glGenBuffers(1, &someBuffersOut.myEBO);
}

void FrameSplineGraphCmp::UpdateBuffers(const Array<Key>& someKeys, KeyFrameBuffers& someBuffersOut)
{
    glBindVertexArray(someBuffersOut.myVAO);
    {
        glBindBuffer(GL_ARRAY_BUFFER, someBuffersOut.myVBO);
        glBufferData(GL_ARRAY_BUFFER, someKeys.Count() * sizeof(Key), someKeys.GetBuffer(), GL_DYNAMIC_DRAW);

        Array<unsigned int> indices;
        const unsigned int keyCount {static_cast<unsigned int>(someKeys.Count())};
        for (unsigned int i = 0; i + 1 < keyCount; ++i)
        {
            indices.PushBack(i);
            indices.PushBack(i + 1);
        }
        indices.PushBack(keyCount - 1);
        indices.PushBack(keyCount - 1);

        glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, someBuffersOut.myEBO);
        glBufferData(GL_ELEMENT_ARRAY_BUFFER, indices.Count() * sizeof(unsigned int), indices.GetBuffer(),
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

void FrameSplineGraphCmp::DeleteBuffers(KeyFrameBuffers& someBuffersOut)
{
    glDeleteBuffers(1, &someBuffersOut.myEBO);
    glDeleteBuffers(1, &someBuffersOut.myVBO);
    glDeleteVertexArrays(1, &someBuffersOut.myVAO);
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
