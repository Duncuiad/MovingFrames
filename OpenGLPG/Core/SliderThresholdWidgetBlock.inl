template <typename T, typename Data>
SliderThresholdWidgetBlock<T, Data>::SliderThresholdWidgetBlock(const std::string& aName,
                                                                const Base::Evaluation& anEvaluation,
                                                                const Base::Action& anAction,
                                                                const BoundsCallback& aMinBound,
                                                                const BoundsCallback& aMaxBound)
    : Base {aName, anEvaluation, anAction}
    , myMinBoundCallback {aMinBound}
    , myMaxBoundCallback {aMaxBound}
{}

template <typename T>
IntSliderThresholdWidgetBlock<T>::IntSliderThresholdWidgetBlock(const std::string& aName,
                                                                const typename Base::Evaluation& anEvaluation,
                                                                const typename Base::Action& anAction,
                                                                const typename Base::BoundsCallback& aMinBound,
                                                                const typename Base::BoundsCallback& aMaxBound)
    : Base {aName, anEvaluation, anAction, aMinBound, aMaxBound}
{
    Base::myThreshold = 0;
}

template <typename T>
float IntSliderThresholdWidgetBlock<T>::ComputeThreshold() const
{
    return static_cast<float>(Base::myThreshold);
}

template <typename T>
bool IntSliderThresholdWidgetBlock<T>::UpdateData()
{
    return ImGui::SliderInt("##Slider", &(Base::myThreshold), Base::myMinBoundCallback(), Base::myMaxBoundCallback());
}

template <typename T>
Int2SliderThresholdWidgetBlock<T>::Int2SliderThresholdWidgetBlock(const std::string& aName,
                                                                  const typename Base::Evaluation& anEvaluation,
                                                                  const typename Base::Action& anAction,
                                                                  const typename Base::BoundsCallback& aMinBound,
                                                                  const typename Base::BoundsCallback& aMaxBound)
    : Base {aName, anEvaluation, anAction, aMinBound, aMaxBound}
{
    Base::myThreshold[0] = 0;
    Base::myThreshold[1] = 0;
}

template <typename T>
float Int2SliderThresholdWidgetBlock<T>::ComputeThreshold() const
{
    return static_cast<float>(Base::myThreshold[0] * Base::myThreshold[0] +
                              Base::myThreshold[1] * Base::myThreshold[1]);
}

template <typename T>
bool Int2SliderThresholdWidgetBlock<T>::UpdateData()
{
    return ImGui::SliderInt2("##Slider", &(Base::myThreshold[0]), Base::myMinBoundCallback()[0],
                             Base::myMaxBoundCallback()[0]);
}

template <typename T>
FloatSliderThresholdWidgetBlock<T>::FloatSliderThresholdWidgetBlock(const std::string& aName,
                                                                    const typename Base::Evaluation& anEvaluation,
                                                                    const typename Base::Action& anAction,
                                                                    const typename Base::BoundsCallback& aMinBound,
                                                                    const typename Base::BoundsCallback& aMaxBound)
    : Base {aName, anEvaluation, anAction, aMinBound, aMaxBound}
{
    Base::myThreshold = 0.f;
}

template <typename T>
float FloatSliderThresholdWidgetBlock<T>::ComputeThreshold() const
{
    return Base::myThreshold;
}

template <typename T>
bool FloatSliderThresholdWidgetBlock<T>::UpdateData()
{
    return ImGui::SliderFloat("##Slider", &(Base::myThreshold), Base::myMinBoundCallback(), Base::myMaxBoundCallback());
}
