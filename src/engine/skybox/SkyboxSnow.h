#pragma once

#include <libultraship.h>
#include "SkyboxCloud.h"
#include "engine/registry/RegisterContent.h"
#include "engine/World.h"
#include "engine/SpawnParams.h"
#include "engine/CoreMath.h"

#include "engine/objects/Object.h"

extern "C" {
#include "common_structs.h"
}

/**
 * Skybox Stars
 *
 * Inherits from SkyboxCloud so that stars/clouds can be stored in the same list
 * and called the same way.
 *
 * @cloudVariant unused for stars
 */
class SkyboxSnow : public SkyboxCloud {
public:
    SkyboxSnow(ScreenContext* screen);

    ~SkyboxSnow() {
        _count--;
    }

    static size_t GetCount() {
        return _count;
    }

    virtual void Draw(ScreenContext* ctx, s32 arg0) override;
    virtual void Tick() override;
    void func_80077E20();
    void func_80077F64(Camera* camera);
private:
    static size_t _count;
    size_t _idx;
    s32 mState;
    s32 mState2;
    FVector mOffset;
    FVector mOrigin;
    FVector mVelocity;
    Vec3su mDirectionAngle;
};
