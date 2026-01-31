#include <libultraship.h>
#include <libultra/gbi.h>
#include "SkyboxStar.h"
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
#include "assets/textures/common_data.h"
#include "math_util_2.h"
#include "render_objects.h"
}

size_t OSkyboxStar::_count = 0;

OSkyboxStar::OSkyboxStar(u16 cloudVariant, u16 posY, u16 rotY, u16 scalePercent) : OSkyboxCloud() {
    _idx = _count;
    ItemWindowObjects* temp_v0;
    find_unused_obj_index(&_objectIndex);
    init_object(_objectIndex, 1);
    temp_v0 = (ItemWindowObjects*) &gObjectList[_objectIndex];
    temp_v0->unk_0D5 = cloudVariant;
    temp_v0->currentItem = ITEM_BANANA;
    temp_v0->direction_angle[1] = rotY;
    temp_v0->unk_09E = posY;
    temp_v0->sizeScaling = (f32) scalePercent / 100.0;
    temp_v0->activeTexture = (u8*)D_0D0293D8;
    func_80073404(_objectIndex, 0x10U, 0x10U, (Vtx*)common_vtx_rectangle);

    mRotY = rotY;

    _count += 1;
}

void OSkyboxStar::Tick() {
}

void OSkyboxStar::Tick2(Camera* camera) {
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
        // Calculate and update the object's position
        // 160 (SCREEN_WIDTH / 2) + (D_8018D1E8 * cameraRot);
        // Grab center of screen, scale by fov factor, offset based on camera rotation
        gObjectList[_objectIndex].unk_09C = D_8018D218 + (D_8018D1E8 * cameraRot);

        // Mark the object as visible
        set_object_flag(_objectIndex, 0x10);
    } else {
        // If outside the bounds, mark the object as not visible
        set_object_flag(_objectIndex, 0x10);
    }
    
    // Vary the star based on star index
    switch (_idx % 5U) {
        case 0:
            func_80073CB0(_objectIndex, &gObjectList[_objectIndex].primAlpha, 0x00000028, 0x000000B4, 0x000000FF, 0,
                            -1);
            break;
        case 1:
            func_80073CB0(_objectIndex, &gObjectList[_objectIndex].primAlpha, 0x00000080, 0x000000FF, 0x000000FF, 0,
                            -1);
            break;
        case 2:
            func_80073CB0(_objectIndex, &gObjectList[_objectIndex].primAlpha, 0x00000050, 0x000000C8, 0x000000FF, 0,
                            -1);
            break;
        case 3:
            func_80073CB0(_objectIndex, &gObjectList[_objectIndex].primAlpha, 0, 0x0000009B, 0x000000FF, 0, -1);
            break;
        case 4:
            func_80073CB0(_objectIndex, &gObjectList[_objectIndex].primAlpha, 0x0000005A, 0x00000080, 0x000000FF, 0,
                            -1);
            break;
    }
}


void OSkyboxStar::Draw(s32 cameraId) {
}

void OSkyboxStar::Draw2(s32 arg0) { // render_stars
    Object* object = &gObjectList[_objectIndex];
    s32 posY = arg0 - object->unk_09E;
    func_8004B414(255, 255, 255, 255);
    if (gObjectList[_objectIndex].status & 0x10) {
        FrameInterpolation_RecordOpenChild("render_stars", TAG_CLOUDS(_objectIndex));
        if (D_8018D228 != gObjectList[_objectIndex].unk_0D5) {

            D_8018D228 = gObjectList[_objectIndex].unk_0D5;
            func_80044DA0((u8*)gObjectList[_objectIndex].activeTexture, gObjectList[_objectIndex].textureWidth,
                          gObjectList[_objectIndex].textureHeight);
        }
        func_8004B138(0xFF, 0xFF, 0xFF, gObjectList[_objectIndex].primAlpha);
        func_80042330_unchanged(gObjectList[_objectIndex].unk_09C, posY, 0, gObjectList[_objectIndex].sizeScaling);
        gSPVertex(gDisplayListHead++, (uintptr_t)gObjectList[_objectIndex].vertex, 4, 0);
        gSPDisplayList(gDisplayListHead++, (Gfx*)common_rectangle_display);
        FrameInterpolation_RecordCloseChild();
    }
}

