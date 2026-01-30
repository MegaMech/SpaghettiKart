#include <libultraship.h>
#include <libultra/gbi.h>
#include "SkyboxSnow.h"
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

size_t OSkyboxSnow::_count = 0;

OSkyboxSnow::OSkyboxSnow() : OSkyboxCloud() {
    _idx = _count;
    find_unused_obj_index(&_objectIndex);
    init_object(_objectIndex, 1);

    _count += 1;
}

void OSkyboxSnow::Tick() {
}

void OSkyboxSnow::Tick2(Camera* camera) {
    if (D_8016559C == 0) {
        D_8018D17C += 1;
        if (D_8018D17C >= D_8018D1F0) {
            D_8018D17C = 0;
        }
        if (gObjectList[_objectIndex].state == 0) {
            init_object(_objectIndex, 1);
        }
    }

    if (gObjectList[_objectIndex].state != 0) {
        switch (gObjectList[_objectIndex].state) {
            case 1:
                func_80077E20(_objectIndex);
                break;
            case 2:
                func_80077F64(_objectIndex, camera);
                if (gObjectList[_objectIndex].unk_0AE == 0) {
                    object_next_state(_objectIndex);
                }
                break;
            case 0:
                break;
            case 3:
                func_80072428(_objectIndex);
                break;
        }
    }
}


void OSkyboxSnow::Draw(s32 cameraId) {
}

void OSkyboxSnow::Draw2(s32 arg0) { // render_clouds
    Object* object = &gObjectList[_objectIndex];
    s32 posY = arg0 - object->unk_09E;
    func_8004B6C4(255, 255, 255);
    // Skip drawing the object this frame if it warped to the other side of the screen
    if ((fabs(object->unk_09C - mOldX) > SCREEN_WIDTH / 2) || (fabs(posY - mOldY) > SCREEN_HEIGHT / 2)) {
        mOldX = object->unk_09C;
        mOldY = posY;
        return;
    }
    
    if (object->status & 0x10) {
        // @port: Tag the transform.
        FrameInterpolation_RecordOpenChild("render_snow", TAG_CLOUDS(_objectIndex));

        if (D_8018D228 != object->unk_0D5) {
            D_8018D228 = object->unk_0D5;
            func_80044DA0((u8*)object->activeTexture, object->textureWidth,
                          object->textureHeight);
        }
        func_80042330_unchanged(object->unk_09C, posY, 0, object->sizeScaling);
        gSPVertex(gDisplayListHead++, (uintptr_t)object->vertex, 4, 0);
        gSPDisplayList(gDisplayListHead++, (Gfx*)common_rectangle_display);

        // @port Pop the transform id.
        FrameInterpolation_RecordCloseChild();
        
        mOldX = object->unk_09C;
        mOldY = posY;
    }
}
