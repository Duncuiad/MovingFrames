#pragma once

#include "Dodec.h"
#include "MathDefines.h"
#include "Serializable.h"

#include <optional>

struct TileVertex : public Serializable
{
    TileVertex() = default;
    TileVertex(int anIndex, int aHeight, const Dodec& aDodec);
    virtual void Serialize(Serializer& aSerializer) override;

    const Vec2& GetPosition() const;

    int myIndex {-1};
    int myHeight {-1};
    Dodec myCoordinates;

    struct Data : public Serializable
    {
        virtual void Serialize(Serializer& aSerializer) override;

        bool myColor {false};
    };

    Data myData;
    mutable std::optional<Vec2> myCachedPosition {std::nullopt};
};
