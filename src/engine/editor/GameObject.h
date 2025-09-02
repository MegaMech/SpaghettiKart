#pragma once

#include <libultraship/libultraship.h>
#include <libultra/gbi.h>
#include <libultra/types.h>
#include "../CoreMath.h"
#include "EditorMath.h"
#include <vector>
#include "engine/SpawnParams.h"

extern "C" {
#include "common_structs.h"
}

struct Triangle;

namespace Editor {
    class GameObject {
public:
        enum class CollisionType {
            VTX_INTERSECT,
            BOUNDING_BOX,
            BOUNDING_SPHERE
        };

        GameObject(FVector pos, IRotator rot, FVector scale, const char* model, std::vector<Triangle> triangles, CollisionType collision, float boundingBoxSize);
        GameObject();
        virtual void Tick();
        virtual void Draw();
        virtual void Load() {};
        FVector GetLocation() const;
        IRotator GetRotation() const;
        FVector GetScale() const;
        void Translate(FVector pos);
        void Rotate(IRotator rot);
        void SetScale(FVector scale);

        const char* Name;
        const char* ResourceName;
        SpawnParams _spawnParams;
        FVector Pos;
        IRotator Rot;
        FVector Scale;
        const char* Model = "";
        std::vector<Triangle> Triangles;
        CollisionType Collision;
        float BoundingBoxSize;
    };
}
