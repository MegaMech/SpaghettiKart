#include <libultraship.h>
#include <libultra/gbi.h>
#include "SkyboxCloud.h"
#include <vector>
#include "engine/tracks/Track.h"
#include "engine/World.h"

#include "port/Engine.h"
#include "port/Game.h"
#include "port/interpolation/FrameInterpolation.h"

extern "C" {
#include "update_objects.h"
#include "code_80057C60.h"
#include "code_8006E9C0.h"
#include "assets/models/common_data.h"
#include "math_util_2.h"
#include "render_objects.h"
}

size_t SkyboxCloud::_count = 0;

SkyboxCloud::SkyboxCloud(u16 cloudVariant, u16 posY, u16 rotY, u16 scalePercent) {
    _idx = _count;
    mY = posY;
    mRotY = rotY;
    mCloudVariant = cloudVariant;

   mScale = (f32) scalePercent / 100.0;
    if (GameEngine_ResourceGetTexTypeByName((const char*)CM_GetProps()->CloudTexture) != 1) {
        mTexture = ((u8*) LOAD_ASSET_RAW(CM_GetProps()->CloudTexture)) + (cloudVariant * 1024);
        mTextureWidth = 64;
        mTextureHeight = 32;
        mVtx = (Vtx*)D_0D005FB0;
    } else {
        mTexture = CM_GetProps()->CloudTexture;
        if (strcmp((const char*)CM_GetProps()->CloudTexture, gTextureExhaust0) == 0 ||
            strcmp((const char*)CM_GetProps()->CloudTexture, gTextureExhaust1) == 0 ||
            strcmp((const char*)CM_GetProps()->CloudTexture, gTextureExhaust2) == 0) {
            mTextureWidth = 64;
            mTextureHeight = 32;
            mVtx = cloudvtx2[cloudVariant];
        } else {
            mTextureWidth = 64;
            mTextureHeight = 32;
            mVtx = cloudvtx2[cloudVariant];
        }
    }

    _count += 1;
}

void SkyboxCloud::Tick(Camera* camera) { // func_800788F8
    s16 cameraRot;
    // Adjustable culling factor
    const float cullingFactor = OTRGetAspectRatio();

    // Calculate the cloud's rotation relative to the camera
    cameraRot = (u16)camera->rot[1] + (u16)mRotY;
    // Adjust bounds based on the culling factor
    s16 adjustedLowerBound = (s16) (D_8018D210 * cullingFactor);
    s16 adjustedUpperBound = (s16) (D_8018D208 * cullingFactor);

    // Check if the object is within the adjusted bounds
    if ((cameraRot >= adjustedLowerBound) && (adjustedUpperBound >= cameraRot)) {
        // Calculate and update the object's X position
        // 160 (SCREEN_WIDTH / 2) + (D_8018D1E8 * cameraRot);
        // Grab center of screen, scale by fov factor, offset based on camera rotation
        mX = D_8018D218 + (D_8018D1E8 * cameraRot);

        // Mark the object as visible
        mVisible = true;
    } else {
        // If outside the bounds, mark the object as not visible
        mVisible = false;
    }
}

void SkyboxCloud::Draw(ScreenContext* screen, s32 arg0) { // render_clouds
   // Object* object = &gObjectList[_objectIndex];
    s32 posY = arg0 - mY;
    func_8004B6C4(255, 255, 255);
    // Skip drawing the object this frame if it warped to the other side of the screen
    if ((fabs(mX - mOldX) > SCREEN_WIDTH / 2) || (fabs(posY - mOldY) > SCREEN_HEIGHT / 2)) {
        mOldX = mX;
        mOldY = posY;
        return;
    }
    if (mVisible) {
        FrameInterpolation_RecordOpenChild("render_clouds", TAG_CLOUDS((_idx << 4) | (screen - gScreenOneCtx)));

        if (D_8018D228 != mCloudVariant) {
            D_8018D228 = mCloudVariant;
            func_80044DA0(mTexture, mTextureWidth, mTextureHeight);
        }
        func_80042330_unchanged(mX, posY, 0, mScale);
        gSPVertex(gDisplayListHead++, (uintptr_t)mVtx, 4, 0);
        gSPDisplayList(gDisplayListHead++, (Gfx*)common_rectangle_display);

        FrameInterpolation_RecordCloseChild();
    }
    mOldX = mX;
    mOldY = posY;
}
