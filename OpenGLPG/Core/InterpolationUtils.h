#pragma once

#include "MathDefines.h"

DualQuat Sclerp(const DualQuat& aPose0, const DualQuat& aPose1, float aT);
DualQuat Sclerp(const DualQuat& aPose0, const DualQuat& aPose1, float aT, const DualQuat& aMonodromyReference);
