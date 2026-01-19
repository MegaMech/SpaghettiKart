#include <libultraship.h>
#include <libultra/gbi.h>
#include "Cloud.h"
#include <vector>
#include "engine/tracks/Track.h"
#include "engine/World.h"

#include "port/Game.h"
#include "port/interpolation/FrameInterpolation.h"

extern "C" {
#include "update_objects.h"
}

size_t OCloud::_count = 0;

OCloud::OCloud() {
    _idx = _count;

    find_unused_obj_index(&_objectIndex);
    init_object(_objectIndex, 0);

    if (IsKoopaTroopaBeach()) {
        mHeight = 48;
    } else if (IsMooMooFarm()) {
        mHeight = 64;
    } else if (IsYoshiValley()) {
        mHeight = 64;
    } else {
        mHeight = 48;
    }

    if (nullptr != World::Instance->GetTrack()->Props.CloudList) {
        mRotY = World::Instance->GetTrack()->Props.CloudList[_idx].RotY;
    } else {
        mRotY = 0;
    }

    _count += 1;
}

void OCloud::Tick() {
    s16 cameraRot;
    // Adjustable culling factor
    const float cullingFactor = OTRGetAspectRatio();

    // Calculate the cloud's rotation relative to the camera
    cameraRot = camera->rot[1] + mRotY;

    // Adjust bounds based on the culling factor
    s16 adjustedLowerBound = (s16) (D_8018D210 * cullingFactor);
    s16 adjustedUpperBound = (s16) (D_8018D208 * cullingFactor);

    // Check if the object is within the adjusted bounds
    if ((cameraRot >= adjustedLowerBound) && (adjustedUpperBound >= cameraRot)) {
        // Calculate and update the object's position
        gObjectList[objectIndex].unk_09C = (D_8018D218 + (D_8018D1E8 * cameraRot));

        // Mark the object as visible
        set_object_flag(objectIndex, 0x10);
    } else {
        // If outside the bounds, mark the object as not visible
        set_object_flag(objectIndex, 0x10);
    }
}

void OCloud::Draw(s32 cameraId) {
    // (240 - get_screen_context()->cameraHeight ) - mHeight;
    Object* object = gObjectList[_objectIndex];
    s32 posY = mHeight - object->unk_09E;

    // Skip drawing the object this frame if it warped to the other side of the screen
    if ((fabs(object->unk_09C - mOldX) > SCREEN_WIDTH / 2) || (fabs(posY - mOldY) > SCREEN_HEIGHT / 2)) {
        mOldX = object->unk_09C;
        mOldY = posY;
        return;
    }

    if (object->status & 0x10) {

        // @port: Tag the transform.
        FrameInterpolation_RecordOpenChild("render_clouds", TAG_CLOUDS(_objectIndex));

        if (D_8018D228 != object->unk_0D5) {
            D_8018D228 = object->unk_0D5;
            func_80044DA0(object->activeTexture, object->textureWidth,
                          object->textureHeight);
        }
        func_80042330_unchanged(x, y, 0, object->sizeScaling);
        gSPVertex(gDisplayListHead++, object->vertex, 4, 0);
        gSPDisplayList(gDisplayListHead++, common_rectangle_display);

        // @port Pop the transform id.
        FrameInterpolation_RecordCloseChild();
    }

    mOldX = object->unk_09C;
    mOldY = posY;
}
