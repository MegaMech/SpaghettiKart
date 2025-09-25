#pragma once

#include <libultraship.h>
#include "engine/Actor.h"
#include "CoreMath.h"
#include "engine/World.h"

extern "C" {
#include "macros.h"
#include "main.h"
#include "camera.h"
#include "common_structs.h"
}

class ACloud : public AActor {
public:


    // Constructor
    ACloud(const SpawnParams& params);

    virtual ~ACloud() override = default;

    // This is simply a helper function to keep Spawning code clean
    static inline ACloud* Spawn(FVector pos, uint16_t time, f32 hop, f32 gravity) {
        SpawnParams params = {
            .Name = "hm:cloud",
            .Type = time, // How long the effect is active
            .Location = pos,
            .Speed = hop, // How high you hop
            .SpeedB = gravity, // How much gravity is effected
        };
        return static_cast<ACloud*>(gWorldInstance.AddActor(new ACloud(params)));
    }

    // Virtual functions to be overridden by derived classes
    virtual void Tick() override;
    virtual void Draw(Camera*) override;
    virtual void DrawEditorProperties() override;
    virtual void Collision(Player* player, AActor* actor) override;
    virtual bool IsMod() override;

    bool PickedUp = false;
    uint32_t Timer = 0;
    
    Player* _player = NULL;

    f32 OldHop = 0;
    f32 OldGravity = 0;

};
