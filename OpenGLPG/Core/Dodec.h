#pragma once

#include "MathDefines.h"

#include <functional>
#include <tuple>

class Dodec
{
public:
    Dodec();
    constexpr Dodec(int anO, int anI, int anN, int anIN);
    Dodec(const Dodec& anOther) = default;
    Dodec& operator=(const Dodec& anOther) = default;

    Dodec operator+(const Dodec& anOther) const;
    Dodec operator-(const Dodec& anOther) const;
    Dodec operator*(const Dodec& anOther) const;
    Dodec operator*(int aScalar) const;
    Dodec operator/(int aDivisor) const;

    bool operator==(const Dodec& anOther) const = default;

    Dodec Conj() const;
    Dodec ConjNM() const;
    int Norm() const;
    float Norm2() const;
    Vec2 Pos() const;
    std::tuple<int, int, int, int> GetCoordsININ() const;
    std::tuple<int, int, int, int> GetCoordsIZIZ() const;
    std::tuple<int, int, int, int> GetCoordsPowersP() const;
    int GID() const; // Greatest integer divisor

    static Dodec O();
    static Dodec I();
    static Dodec N();
    static Dodec M();
    static Dodec Z();
    static Dodec P();
    static Dodec P(int aPower);

private:
    int myO;
    int myI;
    int myN;
    int myIN;

    friend struct std::hash<Dodec>;
    friend Dodec operator-(const Dodec& aDodec);
};

Dodec operator-(const Dodec& aDodec);

template <>
struct std::hash<Dodec>
{
    inline size_t operator()(const Dodec& aDodec) const
    {
        return (size_t(aDodec.myO)) + (size_t(aDodec.myI) << 16) + (size_t(aDodec.myN) << 32) +
               (size_t(aDodec.myIN) << 48);
    }
};
