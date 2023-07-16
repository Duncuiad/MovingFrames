#pragma once

#include "Array.h"
#include "CameraData.h"
#include "CameraManager.h"
#include "EditorImGui.h"
#include "Entity.h"
#include "Filepath.h"
#include "UID.h"

class WorldModel
{
public:
    struct ConstructionParams
    {
        const ClientLoader& myClientLoader;
        Filepath aLevelName = "";
    };
    struct UpdateParams
    {
        const GameInputData& myGameInputData;
        float myDeltaTime;
    };

    WorldModel(const ConstructionParams& someParams);
    void Init();
    void Shutdown();
    void Update(const UpdateParams& someParams);
    void SaveWorld(const std::string& aLevelName);

    bool IsAvailable() const;

    const CameraData& GetActiveCameraData() const;
    const Entity::Container& GetEntities() const { return myEntities; }
    const Entity& GetEntity(const UID& anEntityUID) const;
    Entity& GetEntity(const UID& anEntityUID);

#if EDITOR_IMGUI
    void EditorWidgetImGui();
#endif

private:
    Entity::Container myEntities;
    Array<Entity> myEntityQueue;

    CameraManager myCameraManager;
    UID myGameplayCamera {UID::Empty};
    const ClientLoader& myClientLoader;
};
