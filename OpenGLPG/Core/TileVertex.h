#pragma once

#include "Dodec.h"
#include "MathDefines.h"
#include "Serializable.h"

#include <functional>
#include <optional>

struct TileVertex : public Serializable
{
    using Predicate = std::function<bool(const TileVertex&)>;

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

        float myColor {1.f};
    };

    Data myData;
    mutable std::optional<Vec2> myCachedPosition {std::nullopt};
};
