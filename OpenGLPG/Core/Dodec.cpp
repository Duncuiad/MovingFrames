#include "OpenGLPG_Base.h"

#include "Dodec.h"

Dodec::Dodec()
    : Dodec {0, 0, 0, 0}
{}

constexpr Dodec::Dodec(int anO, int anI, int anN, int anIN)
    : myO {anO}
    , myI {anI}
    , myN {anN}
    , myIN {anIN}
{}

Dodec Dodec::operator+(const Dodec& anOther) const
{
    return {myO + anOther.myO, myI + anOther.myI, myN + anOther.myN, myIN + anOther.myIN};
}

Dodec Dodec::operator-(const Dodec& anOther) const
{
    return {myO - anOther.myO, myI - anOther.myI, myN - anOther.myN, myIN - anOther.myIN};
}

Dodec Dodec::operator*(const Dodec& anOther) const
{
    const int o {anOther.myO};
    const int i {anOther.myI};
    const int n {anOther.myN};
    const int in {anOther.myIN};
    return {myO * o - myI * i + myIN * n + myN * in, myI * o + myO * i - myN * n + myIN * in,
            myN * o + myO * n - myIN * i - myI * in + myN * n - myIN * in - 2 * myIN * n - 2 * myN * in,
            myI * n + myN * i + myO * in + myIN * o + 2 * myN * n - 2 * myIN * in + myIN * n + myN * in};
}

Dodec Dodec::operator*(int aScalar) const
{
    return {myO * aScalar, myI * aScalar, myN * aScalar, myIN * aScalar};
}

Vec2 Dodec::GetPos() const
{
    const double o {static_cast<double>(myO)};
    const double i {static_cast<double>(myI)};
    const double n {static_cast<double>(myN)};
    const double in {static_cast<double>(myIN)};
    constexpr double h {0.13397459621556135};
    return {static_cast<float>(o + n * 0.5 - h * in), static_cast<float>(i + in * 0.5 + h * n)};
}

std::tuple<int, int, int, int> Dodec::GetCoords() const
{
    return {myO, myI, myN, myIN};
}

Dodec Dodec::O()
{
    return {1, 0, 0, 0};
}

Dodec Dodec::I()
{
    return {0, 1, 0, 0};
}

Dodec Dodec::N()
{
    return {0, 0, 1, 0};
}

Dodec Dodec::IN()
{
    return {0, 0, 0, 1};
}
