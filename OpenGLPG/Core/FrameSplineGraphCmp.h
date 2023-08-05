#pragma once

#include "Array.h"
#include "GraphCmp.h"
#include "MathDefines.h"
#include "Serializable.h"
#include "Transform.h"

class FrameSplineGraphCmp : public GraphCmp
{
    DECLARE_SUBTYPE(FrameSplineGraphCmp)
    using Base = GraphCmp;

public:
    enum class DisplayStyle {
        Segments
    };
    struct Key : public Serializable
    {
        Key() = default;
        Key(const Vec3&, const Vec3&, const Vec3&, const Vec3&, const Vec3&, const Vec3&);

        Vec3 myPosition {};
        Vec3 myRight {};
        Vec3 myUp {};
        Vec3 myForward {};
        Vec3 myVelocity {};
        Vec3 myAngularVelocity {};

        void Serialize(Serializer& aSerializer) override;
    };

    ~FrameSplineGraphCmp() override;

    void Draw(const DrawParams& someParams) const override;
    void Serialize(Serializer& aSerializer) override;

    void SetDisplayStyle(DisplayStyle aStyle);
    void SetKeys(const Array<Key>& someKeys);

private:
    void UpdateBuffers();

    Array<Key> myKeys;
    DisplayStyle myStyle {DisplayStyle::Segments};

    GLUID myVAO {0};
    GLUID myVBO {0};
    GLUID myEBO {0};
};
