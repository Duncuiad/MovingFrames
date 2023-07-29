#pragma once

#include "Filepath.h"
#include "GraphCmp.h"
#include "Model.h"

class ModelGraphCmp : public GraphCmp
{
    DECLARE_SUBTYPE(ModelGraphCmp)
    using Base = GraphCmp;

public:
    ModelGraphCmp() = default;
    ModelGraphCmp(const Filepath& aModelAsset, const Filepath& aShaderAsset);
    void OnLoad(const LoadParams& someParams) override;
    void Update() override {}
    void Draw(const DrawParams& someParams) const override;

    void Serialize(Serializer& aSerializer) override;

private:
    Model::Ptr myModel {nullptr};
    Filepath myModelAsset;
};
