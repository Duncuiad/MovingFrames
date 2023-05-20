#include "OpenGLPG_Base.h"

#include "UID.h"

unsigned long int UID::ourCurrent = 0;
const UID UID::Empty = UID();

UID UID::CreateUnique()
{
    // @twothreadsgoboom
    ++ourCurrent;
    UID newUID {};
    newUID.myInternalID = ourCurrent;
    return newUID;
}

UID::UID(const UID& anOther)
    : myInternalID {anOther.myInternalID}
{}

bool UID::IsValid()
{
    return *this != UID::Empty;
}

bool UID::operator==(const UID& anOther)
{
    return myInternalID == anOther.myInternalID;
}
