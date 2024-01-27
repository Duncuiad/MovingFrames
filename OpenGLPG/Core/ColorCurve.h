#pragma once

#include "Array.h"
#include "MathDefines.h"

class ColorCurve
{
public:
    explicit ColorCurve(const Array<Vec3>& someColorsRGB);
    Vec3 GetColorRGBAt(float aT) const;

private:
    static Vec3 HCLToBall(const Vec3& aColorHCL);
    static Vec3 BallToHCL(const Vec3& aColorBall);
    static Vec3 Interpolate(const Vec3& aPreviousKeyFrom, const Vec3& aKeyFrom, const Vec3& aKeyTo,
                            const Vec3& aNextKeyTo, float aT);
    static Vec3 CubicBezier(const Vec3& aP0, const Vec3& aP1, const Vec3& aP2, const Vec3& aP3, float aT);
    static Vec3 ComputeTangent(const Vec3& aPreviousKey, const Vec3& aKey, const Vec3& aNextKey);

    Array<Vec3> myColorKeys;
};
