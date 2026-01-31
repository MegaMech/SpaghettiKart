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

size_t OSkyboxCloud::_count = 0;

OSkyboxCloud::OSkyboxCloud(u16 cloudVariant, u16 posY, u16 rotY, u16 scalePercent) {
    _idx = _count;
    ItemWindowObjects* temp_v0;
    find_unused_obj_index(&_objectIndex);
    init_object(_objectIndex, 1);
    temp_v0 = (ItemWindowObjects*) &gObjectList[_objectIndex];
    temp_v0->unk_0D5 = cloudVariant;
    temp_v0->currentItem = ITEM_NONE;
    temp_v0->direction_angle[1] = rotY;
    temp_v0->unk_09E = posY;
    temp_v0->sizeScaling = (f32) scalePercent / 100.0;
    if (GameEngine_ResourceGetTexTypeByName((const char*)CM_GetProps()->CloudTexture) != 1) {
        temp_v0->activeTexture = ((u8*) LOAD_ASSET_RAW(CM_GetProps()->CloudTexture)) + (cloudVariant * 1024);
        func_80073404(_objectIndex, 0x40U, 0x20U, (Vtx*)D_0D005FB0);
    } else {
        temp_v0->activeTexture = CM_GetProps()->CloudTexture;
        if (strcmp((const char*)CM_GetProps()->CloudTexture, gTextureExhaust0) == 0 ||
            strcmp((const char*)CM_GetProps()->CloudTexture, gTextureExhaust1) == 0 ||
            strcmp((const char*)CM_GetProps()->CloudTexture, gTextureExhaust2) == 0) {
            func_80073404(_objectIndex, 0x40U, 0x20U, cloudvtx2[cloudVariant]);
        } else {
            func_80073404(_objectIndex, 0x40U, 0x20U, cloudvtx[cloudVariant]);
        }
    }
    temp_v0->primAlpha = 0x00FF;

    mRotY = rotY;

    _count += 1;
}

void OSkyboxCloud::Tick() {
}

void OSkyboxCloud::Tick2(Camera* camera) { // func_800788F8
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
        gObjectList[_objectIndex].unk_09C = D_8018D218 + (D_8018D1E8 * cameraRot);

        // Mark the object as visible
        set_object_flag(_objectIndex, 0x10);
    } else {
        // If outside the bounds, mark the object as not visible
        clear_object_flag(_objectIndex, 0x10);
    }
}


void OSkyboxCloud::Draw(s32 cameraId) {
}

void OSkyboxCloud::Draw2(s32 arg0) { // render_clouds
    Object* object = &gObjectList[_objectIndex];
    s32 posY = arg0 - object->unk_09E;
    func_8004B6C4(255, 255, 255);
    // Skip drawing the object this frame if it warped to the other side of the screen
    if ((fabs(gObjectList[_objectIndex].unk_09C - mOldX) > SCREEN_WIDTH / 2) || (fabs(posY - mOldY) > SCREEN_HEIGHT / 2)) {
        mOldX = gObjectList[_objectIndex].unk_09C;
        mOldY = posY;
        return;
    }
    if (object->status & 0x10) {

        // @port: Tag the transform.
        FrameInterpolation_RecordOpenChild("render_clouds", TAG_CLOUDS(_objectIndex));

        if (D_8018D228 != object->unk_0D5) {
            D_8018D228 = object->unk_0D5;
            func_80044DA0((u8*)object->activeTexture, object->textureWidth,
                          object->textureHeight);
        }
        func_80042330_unchanged(gObjectList[_objectIndex].unk_09C, posY, 0, object->sizeScaling);
        gSPVertex(gDisplayListHead++, (uintptr_t)object->vertex, 4, 0);
        gSPDisplayList(gDisplayListHead++, (Gfx*)common_rectangle_display);

        // @port Pop the transform id.
        FrameInterpolation_RecordCloseChild();
        
    }
    mOldX = gObjectList[_objectIndex].unk_09C;
    mOldY = posY;
}
