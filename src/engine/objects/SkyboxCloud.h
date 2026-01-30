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
 *
 * @cloudVariant The cloud texture to use
 */
class OSkyboxCloud : public OObject {
public:
    OSkyboxCloud() {}
    OSkyboxCloud(u16 cloudVariant, u16 posY, u16 rotY, u16 scalePercent);

    ~OSkyboxCloud() {
        _count--;
    }

    static size_t GetCount() {
        return _count;
    }

    virtual void Tick() override;
    virtual void Draw(s32 cameraId) override;
    virtual void Draw2(s32 arg0);
    virtual void Tick2(Camera* camera);
protected:
    static size_t _count;
    size_t _idx;
    int32_t mRotY;

    int32_t mX;
    int32_t mOldX;
    int32_t mOldY;
};
