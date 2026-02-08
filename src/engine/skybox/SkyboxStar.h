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
class SkyboxStar : public SkyboxCloud {
public:
    SkyboxStar(ScreenContext* screen, u16 cloudVariant, u16 posY, u16 rotY, u16 scalePercent);

    virtual ~SkyboxStar() {
        _count--;
    }

    static size_t GetCount() {
        return _count;
    }

    virtual void Draw(ScreenContext* ctx, s32 arg0) override;
    virtual void Tick() override;
    bool star_func_80073B78(s32 arg0, s32 arg3, s32 arg4, s32 arg5, s32 arg6, s32 arg7);
private:
    static size_t _count;
    size_t _idx;

    s16 mPrimAlpha;
    s8 mUnk_0CF;
    s16 mUnk_0AC;
    s8 mUnk_0D0;
};
