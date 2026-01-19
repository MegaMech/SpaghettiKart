#pragma once

#include <libultraship.h>
#include "engine/registry/RegisterContent.h"
#include "engine/World.h"
#include "engine/SpawnParams.h"
#include "engine/CoreMath.h"

#include "engine/objects/Object.h"

extern "C" {
#include "common_structs.h"
}

/**
 * Skybox clouds
 */
class OCloud : public OObject {

    OCloud();

    ~OCloud() {
        _count--;
    }

    static size_t GetCount() {
        return _count;
    }

    virtual void Tick() override;
    virtual void Draw(s32 cameraId) override;

    static size_t _count;
    size_t _idx;
    size_t _objectIndex;
    int32_t mHeight;
    int32_t mRotY;

    int32_t mOldX;
    int32_t mOldZ;
};
