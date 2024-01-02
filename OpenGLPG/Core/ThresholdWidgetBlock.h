#pragma once

#include "ImGuiWidgets.h"

#include <functional>
#include <string>

template <typename T>
class ThresholdWidgetBlock
{
public:
    using Evaluation = std::function<float(const T&)>;
    using Predicate = std::function<bool(const T&)>;
    using Action = std::function<void(const Predicate&)>;

    ThresholdWidgetBlock(const std::string& aName, const Evaluation& anEvaluation, const Action& anAction);
    bool Draw();

protected:
    virtual bool UpdateData() = 0;
    virtual float ComputeThreshold() const = 0;

private:
    enum class ComparisonType {
        Less,
        Equal,
        Greater
    };

    std::string myName;
    const Evaluation myEvaluationCallback;
    const Action myActionCallback;
    ComparisonType myComparisonType {ComparisonType::Less};
};

template <typename T>
ThresholdWidgetBlock<T>::ThresholdWidgetBlock(const std::string& aName, const Evaluation& anEvaluation,
                                              const Action& anAction)
    : myName {aName}
    , myEvaluationCallback {anEvaluation}
    , myActionCallback {anAction}
{}

template <typename T>
bool ThresholdWidgetBlock<T>::Draw()
{
    ImGui::PushID(myName.c_str());
    std::string comparison = "";
    switch (myComparisonType)
    {
    case ComparisonType::Less:
        comparison = "<";
        break;
    case ComparisonType::Equal:
        comparison = "=";
        break;
    case ComparisonType::Greater:
        comparison = ">";
        break;
    default:
        break;
    }
    ImGui::Text("%s %s %.3f", myName.c_str(), comparison.c_str(), ComputeThreshold());

    bool edited {false};
    ImGui::SameLine();
    if (ImGui::Button("<"))
    {
        myComparisonType = ComparisonType::Less;
        edited = true;
    }
    ImGui::SameLine();
    if (ImGui::Button("="))
    {
        myComparisonType = ComparisonType::Equal;
        edited = true;
    }
    ImGui::SameLine();
    if (ImGui::Button(">"))
    {
        myComparisonType = ComparisonType::Greater;
        edited = true;
    }
    edited |= UpdateData();
    ImGui::PopID();

    if (edited)
    {
        switch (myComparisonType)
        {
        case ComparisonType::Less: {
            myActionCallback([this](const T& anObject) { return myEvaluationCallback(anObject) < ComputeThreshold(); });
            break;
        }
        case ComparisonType::Equal: {
            myActionCallback(
                [this](const T& anObject) { return myEvaluationCallback(anObject) == ComputeThreshold(); });
            break;
        }
        case ComparisonType::Greater: {
            myActionCallback([this](const T& anObject) { return myEvaluationCallback(anObject) > ComputeThreshold(); });
            break;
        }
        default:
            break;
        }
    }

    return edited;
}
