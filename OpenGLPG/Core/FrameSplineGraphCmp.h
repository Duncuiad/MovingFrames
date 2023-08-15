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

    FrameSplineGraphCmp();
    ~FrameSplineGraphCmp() override;

    void OnLoad(const LoadParams& someParams) override;
    void Draw(const DrawParams& someParams) const override;
    void Serialize(Serializer& aSerializer) override;

    float GetKeyScale() const;
    float GetTangentScale() const;

    void SetDisplayStyle(DisplayStyle aStyle);
    void SetKeyScale(float aScale);
    void SetTangentScale(float aScale);
    void SetKeys(const Array<Key>& someKeys);
    void SetControlKeys(const Array<Key>& someKeys);

    bool myShowControlKeys {false};

private:
    struct KeyFrameBuffers
    {
        GLUID myVAO {0};
        GLUID myVBO {0};
        GLUID myEBO {0};
    };
    struct DisplayData
    {
        float myKeyScale {0.1f};
        float myTangentScale {1.f};
    };
    static void GenerateBuffers(KeyFrameBuffers& someBuffersOut);
    static void UpdateBuffers(const Array<Key>& someKeys, KeyFrameBuffers& someBuffersOut);
    static void DeleteBuffers(KeyFrameBuffers& someBuffersOut);

    Shader::Ptr myControlKeyShader {nullptr};
    Filepath myControlKeyShaderAsset;

    Array<Key> myKeys;
    Array<Key> myControlKeys;
    DisplayStyle myStyle {DisplayStyle::Segments};
    DisplayData myDisplayData;

    KeyFrameBuffers myKeyBuffers;
    KeyFrameBuffers myControlKeyBuffers;
};
