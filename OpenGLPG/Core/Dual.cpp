#include "OpenGLPG_Base.h"

#include "Dual.h"

#include <glm/common.hpp>

Dual::Dual(float aNumber)
    : real {aNumber}
    , dual {0.f}
{}

Dual::Dual(float aReal, float aDual)
    : real {aReal}
    , dual {aDual}
{}

Dual::Dual(const Dual& anOther)
{
    *this = anOther;
}

Dual Dual::operator-() const
{
    return {-real, -dual};
}

Dual Dual::operator+(const Dual& anOther) const
{
    return {real + anOther.real, dual + anOther.dual};
}

Dual Dual::operator-(const Dual& anOther) const
{
    return *this + -anOther;
}

Dual Dual::operator*(const Dual& anOther) const
{
    return {real * anOther.real, real * anOther.dual + dual + anOther.real};
}

Dual& Dual::operator=(const Dual& anOther)
{
    real = anOther.real;
    dual = anOther.dual;
    return *this;
}

bool Dual::operator==(const Dual& anOther)
{
    return real == anOther.real && dual && anOther.dual;
}

bool Dual::IsInvertible() const
{
    return {real != 0.f};
}

bool Dual::CloseTo(const Dual& anOther, float anEpsilon) const
{
    return glm::abs(real - anOther.real) <= anEpsilon && glm::abs(dual - anOther.dual) <= anEpsilon;
}

Dual Dual::Conj() const
{
    return {real, -dual};
}
