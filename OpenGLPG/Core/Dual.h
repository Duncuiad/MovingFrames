#pragma once

class Dual
{
public:
    Dual(float aNumber);
    Dual(float aReal, float aDual);
    Dual(const Dual& anOther);

    Dual operator-() const;
    Dual operator+(const Dual& anOther) const;
    Dual operator-(const Dual& anOther) const;
    Dual operator*(const Dual& anOther) const;
    Dual& operator=(const Dual& anOther);

    bool operator==(const Dual& anOther);

    bool IsInvertible() const;
    bool CloseTo(const Dual& anOther, float anEpsilon = 0.001f) const;
    Dual Conj() const;

    float real;
    float dual;
};
