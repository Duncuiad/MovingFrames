#include "OpenGLPG_Base.h"

#include "FrameSplineGraphCmp.h"

FrameSplineGraphCmp::~FrameSplineGraphCmp()
{
    glDeleteBuffers(1, &myEBO);
    glDeleteBuffers(1, &myVBO);
    glDeleteVertexArrays(1, &myVAO);
}

void FrameSplineGraphCmp::Draw(const DrawParams& someParams) const
{
    glBindVertexArray(myVAO);
    glBindBuffer(GL_ARRAY_BUFFER, myVBO);
    glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, myEBO);

    glDrawElements(GL_LINE, myKeys.Count(), GL_UNSIGNED_INT, 0);
    glBindVertexArray(0);
}

void FrameSplineGraphCmp::Serialize(Serializer& aSerializer)
{
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
    glDeleteBuffers(1, &myEBO);
    glDeleteBuffers(1, &myVBO);
    glDeleteVertexArrays(1, &myVAO);

    glGenVertexArrays(1, &myVAO);
    glGenBuffers(1, &myVBO);
    glGenBuffers(1, &myEBO);

    {
        glBindBuffer(GL_ARRAY_BUFFER, myVBO);
        glBufferData(GL_ARRAY_BUFFER, myKeys.Count() * sizeof(Key), &myKeys[0], GL_STATIC_DRAW);

        const unsigned int keyCount {static_cast<unsigned int>(myKeys.Count())};
        unsigned int* segments {new unsigned int[keyCount * 2u]};
        for (unsigned int i = 0; i < keyCount; ++i)
        {
            segments[i] = i;
            segments[i + 1] = i + 1;
        }
        glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, myEBO);
        glBufferData(GL_ELEMENT_ARRAY_BUFFER, myKeys.Count() * 2 * sizeof(unsigned int), segments, GL_STATIC_DRAW);
        delete[] segments;
    }

    {
        glBindVertexArray(myVAO);

        // key positions
        glEnableVertexAttribArray(0);
        glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, sizeof(Key), (void*)0);
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

        glBindVertexArray(0);
    }
}

void FrameSplineGraphCmp::Key::Serialize(Serializer& aSerializer)
{
    aSerializer.Process("myPosition", myPosition);
    aSerializer.Process("myRight", myRight);
    aSerializer.Process("myUp", myUp);
    aSerializer.Process("myForward", myForward);
    aSerializer.Process("myVelocity", myVelocity);
    aSerializer.Process("myAngularVelocity", myAngularVelocity);
}
