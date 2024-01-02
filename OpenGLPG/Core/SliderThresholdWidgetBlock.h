#pragma once

#include "ThresholdWidgetBlock.h"

#include <array>
#include <functional>

template <typename T, typename Data>
class SliderThresholdWidgetBlock : public ThresholdWidgetBlock<T>
{
public:
    using BoundsCallback = std::function<Data(void)>;
    using Base = ThresholdWidgetBlock<T>;
    SliderThresholdWidgetBlock(const std::string& aName, const Base::Evaluation& anEvaluation,
                               const Base::Action& anAction, const BoundsCallback& aMinBound,
                               const BoundsCallback& aMaxBound);

protected:
    const BoundsCallback myMinBoundCallback;
    const BoundsCallback myMaxBoundCallback;
    Data myThreshold;
};

template <typename T>
class IntSliderThresholdWidgetBlock : public SliderThresholdWidgetBlock<T, int>
{
public:
    using Base = SliderThresholdWidgetBlock<T, int>;
    IntSliderThresholdWidgetBlock(const std::string& aName, const typename Base::Evaluation& anEvaluation,
                                  const typename Base::Action& anAction, const typename Base::BoundsCallback& aMinBound,
                                  const typename Base::BoundsCallback& aMaxBound);

private:
    float ComputeThreshold() const override;
    bool UpdateData() override;
};

template <typename T>
class Int2SliderThresholdWidgetBlock : public SliderThresholdWidgetBlock<T, std::array<int, 2>>
{
public:
    using Base = SliderThresholdWidgetBlock<T, std::array<int, 2>>;
    Int2SliderThresholdWidgetBlock(const std::string& aName, const typename Base::Evaluation& anEvaluation,
                                   const typename Base::Action& anAction,
                                   const typename Base::BoundsCallback& aMinBound,
                                   const typename Base::BoundsCallback& aMaxBound);

private:
    float ComputeThreshold() const override;
    bool UpdateData() override;
};

template <typename T>
class FloatSliderThresholdWidgetBlock : public SliderThresholdWidgetBlock<T, float>
{
public:
    using Base = SliderThresholdWidgetBlock<T, float>;
    FloatSliderThresholdWidgetBlock(const std::string& aName, const typename Base::Evaluation& anEvaluation,
                                    const typename Base::Action& anAction,
                                    const typename Base::BoundsCallback& aMinBound,
                                    const typename Base::BoundsCallback& aMaxBound);

private:
    float ComputeThreshold() const override;
    bool UpdateData() override;
};

#include "SliderThresholdWidgetBlock.inl"
