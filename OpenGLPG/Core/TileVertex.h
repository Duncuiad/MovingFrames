#pragma once

#include "Dodec.h"
#include "Map.h"
#include "MathDefines.h"
#include "Serializable.h"

#include <functional>
#include <optional>

struct TileVertex : public Serializable
{
    using Predicate = std::function<bool(const std::pair<Dodec, TileVertex>&)>;
    using Evaluation = std::function<float(const std::pair<Dodec, TileVertex>&)>;
    using Map = Map<Dodec, TileVertex>;
    using Pair = Map::ValueType;

    TileVertex() = default;
    explicit TileVertex(int aHeight);
    virtual void Serialize(Serializer& aSerializer) override;

    int myHeight {-1};

    struct Data : public Serializable
    {
        virtual void Serialize(Serializer& aSerializer) override;

        float myColor {1.f};
    };

    Data myData;
};
