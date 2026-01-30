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
 * Inherits from OSkyboxCloud so that stars/clouds can be stored in the same list
 * and called the same way.
 *
 * @cloudVariant unused for stars
 */
class OSkyboxSnow : public OSkyboxCloud {
public:
    OSkyboxSnow();

    ~OSkyboxSnow() {
        _count--;
    }

    static size_t GetCount() {
        return _count;
    }

    virtual void Tick() override;
    virtual void Draw(s32 cameraId) override;
    virtual void Draw2(s32 arg0) override;
    virtual void Tick2(Camera* camera) override;
protected:
    static size_t _count;
    size_t _idx;
    int32_t mRotY;
};
