#include "OpenGLPG_Base.h"

#include "Serializable.h"

SerializationFactory* SerializationFactory::ourInstance {};

std::unordered_map<std::string, SerializationFactory::Builder>& SerializationFactory::GetBuilders()
{
    if (ourInstance == nullptr)
    {
        ourInstance = new SerializationFactory {};
    }
    return ourInstance->myBuilders;
}
