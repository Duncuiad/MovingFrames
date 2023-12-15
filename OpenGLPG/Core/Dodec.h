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

    bool operator==(const Dodec& anOther) const = default;

    Dodec Conj() const;
    float Norm2() const;
    Vec2 Pos() const;
    std::tuple<int, int, int, int> GetCoordsININ() const;
    std::tuple<int, int, int, int> GetCoordsIZIZ() const;

    static Dodec O();
    static Dodec I();
    static Dodec N();
    static Dodec M();
    static Dodec Z();
    static Dodec P();

private:
    int myO;
    int myI;
    int myN;
    int myIN;

    friend struct std::hash<Dodec>;
};

template <>
struct std::hash<Dodec>
{
    inline size_t operator()(const Dodec& aDodec) const
    {
        return (size_t(aDodec.myO)) + (size_t(aDodec.myI) << 16) + (size_t(aDodec.myN) << 32) +
               (size_t(aDodec.myIN) << 48);
    }
};
