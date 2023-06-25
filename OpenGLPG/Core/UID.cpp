#include "OpenGLPG_Base.h"

#include "UID.h"

unsigned long int UID::ourCurrent = 0;
const UID UID::Empty = UID();

UID UID::CreateUnique()
{
    ++ourCurrent;
    UID newUID {};
    newUID.myInternalID = ourCurrent;
    return newUID;
}

bool UID::IsValid()
{
    return *this != UID::Empty;
}

bool UID::operator==(const UID& anOther) const
{
    return myInternalID == anOther.myInternalID;
}

std::string UID::GetString() const
{
    return std::to_string(myInternalID);
}
