#include "OpenGLPG_Base.h"

#include "ColorCurve.h"

namespace ColorCurve_Private
{

// NOTE: Hue is expressend in radians

Vec3 RGBToHCL(const Vec3& aColorRGB)
{
    constexpr float pi {glm::pi<float>()};

    const float r = aColorRGB.x;
    const float g = aColorRGB.y;
    const float b = aColorRGB.z;
    const float M = glm::max(glm::max(r, g), b);
    const float m = glm::min(glm::min(r, g), b);
    const float l = (m + M) * 0.5f;
    const float c = M - m;
    const float hBase = c == 0.f ? 0.f : M == r ? glm::mod(g - b, 6.f * c) : M == g ? b - r + 2.f * c : r - g + 4.f * c;
    const float h = c == 0.f ? 0.f : pi / (3.f * c) * hBase;

    return {h, c, l};
}

Vec3 HCLToRGB(const Vec3& aColorHCL)
{
    constexpr float pi {glm::pi<float>()};

    const float h = aColorHCL.x;
    const float c = aColorHCL.y;
    const float l = aColorHCL.z;
    const float hPrime = glm::mod(h * 3.f / pi, 6.f);
    const float x = c * (1.f - glm::abs(glm::mod(hPrime, 2.f) - 1.f));
    Vec3 rgbBase;
    switch (static_cast<int>(hPrime))
    {
    case 0:
        rgbBase = {c, x, 0.f};
        break;
    case 1:
        rgbBase = {x, c, 0.f};
        break;
    case 2:
        rgbBase = {0.f, c, x};
        break;
    case 3:
        rgbBase = {0.f, x, c};
        break;
    case 4:
        rgbBase = {x, 0.f, c};
        break;
    case 5:
        rgbBase = {c, 0.f, x};
        break;
    default:
        ASSERT(false, "H' has an invalid value");
    }

    return rgbBase + Vec3 {l - c * 0.5f};
}

} // namespace ColorCurve_Private

ColorCurve::ColorCurve(const Array<Vec3>& someColorsRGB)
    : myColorKeys {someColorsRGB}
{
    for (Vec3& color : myColorKeys)
    {
        color = HCLToBall(ColorCurve_Private::RGBToHCL(color));
    }
}

Vec3 ColorCurve::GetColorRGBAt(float aT) const
{
    const int n {myColorKeys.Count()};
    const float scaledT {glm::mod(aT, 1.f) * n};
    const int i {static_cast<int>(scaledT)};
    const float internalT {scaledT - static_cast<float>(i)};
    const Vec3 ballColor {Interpolate(myColorKeys[Mod(i - 1, n)], myColorKeys[i], myColorKeys[Mod(i + 1, n)],
                                      myColorKeys[Mod(i + 2, n)], internalT)};
    return glm::length(ballColor) <= 1.f ? ColorCurve_Private::HCLToRGB(BallToHCL(ballColor)) : Vec3 {1.f, 0.f, 0.f};
}

Vec3 ColorCurve::HCLToBall(const Vec3& aColorHCL)
{
    const float h {aColorHCL.x};
    const float c {aColorHCL.y};
    const float l {aColorHCL.z};
    const float z {2.f * l - 1.f};
    const float absZ {abs(z)};
    const float rBase {absZ == 1.f ? 0.f : c * sqrt((1.f + absZ) / (1.f - absZ))};
    const float r {rBase * rBase};
    return {r * cos(h), r * sin(h), z};
}

Vec3 ColorCurve::BallToHCL(const Vec3& aColorBall)
{
    const float x {aColorBall.x};
    const float y {aColorBall.y};
    const float z {aColorBall.z};
    const float r {sqrt(x * x + y * y)};
    const float rBase {sqrt(r)};
    const float h {glm::mod(atan2(y, x), glm::two_pi<float>())};
    const float c {rBase * sqrt((1.f - abs(z)) / (1.f + abs(z)))};
    const float l {(z + 1.f) * 0.5f};
    return {h, c, l};
}

Vec3 ColorCurve::Interpolate(const Vec3& aPreviousKeyFrom, const Vec3& aKeyFrom, const Vec3& aKeyTo,
                             const Vec3& aNextKeyTo, float aT)
{
    return CubicBezier(aKeyFrom, aKeyFrom + ComputeTangent(aPreviousKeyFrom, aKeyFrom, aKeyTo) / 3.f,
                       aKeyTo - ComputeTangent(aKeyFrom, aKeyTo, aNextKeyTo) / 3.f, aKeyTo, aT);
}

Vec3 ColorCurve::CubicBezier(const Vec3& aP0, const Vec3& aP1, const Vec3& aP2, const Vec3& aP3, float aT)
{
    const float iT {1.f - aT};
    return iT * iT * iT * aP0 + iT * iT * aT * aP1 + iT * aT * aT * aP2 + aT * aT * aT * aP3;
}

Vec3 ColorCurve::ComputeTangent(const Vec3& aPreviousKey, const Vec3& aKey, const Vec3& aNextKey)
{
    if (aKey == Vec3 {0.f})
    {
        return aNextKey - aPreviousKey;
    }
    return aNextKey - aPreviousKey - glm::dot(aNextKey - aPreviousKey, aKey) * glm::normalize(aKey);
}
