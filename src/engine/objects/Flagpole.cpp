#include "Flagpole.h"
#include "World.h"

extern "C" {
#include "code_800029B0.h"
#include "render_objects.h"
#include "update_objects.h"
#include "assets/yoshi_valley_data.h"
#include "assets/common_data.h"
#include "math_util.h"
#include "math_util_2.h"
#include "code_80086E70.h"
#include "code_80057C60.h"
}

size_t OFlagpole::_count = 0;

OFlagpole::OFlagpole(const SpawnParams& params) {
    Name = "Flagpole";
    ResourceName = "mk:flagpole";
    _idx = _count;
    _pos = params.Location.value_or(FVector(0, 0, 0));
    _direction = params.Rotation.value_or(IRotator(0, 0, 0));

    find_unused_obj_index(&_objectIndex);

    init_object(_objectIndex, 0);

    _count++;
}

void OFlagpole::SetSpawnParams(SpawnParams& params) {
    Object* object = &gObjectList[_objectIndex];
    params.Name = "mk:flagpole";
    params.Location = FVector(
        object->pos[0],
        object->pos[1],
        object->pos[2]
    );
    IRotator rot; rot.Set(
        object->orientation[0],
        object->orientation[1],
        object->orientation[2]
    );
    params.Rotation = rot;
}

void OFlagpole::Tick() { // func_80083080
    s32 objectIndex = _objectIndex;

    if (gObjectList[objectIndex].state != 0) {
        OFlagpole::func_80083018(objectIndex);
        OFlagpole::func_80083060(objectIndex);
    }
}

void OFlagpole::Draw(s32 cameraId) { // func_80055228
    s32 objectIndex = _objectIndex;

    func_8008A364(objectIndex, cameraId, 0x4000U, 0x000005DC);
    if (is_obj_flag_status_active(objectIndex, VISIBLE) != 0) {
        OFlagpole::func_80055164(objectIndex);
    }
}

void OFlagpole::func_80055164(s32 objectIndex) { // func_80055164
    if (gObjectList[objectIndex].state >= 2) {
        gSPDisplayList(gDisplayListHead++, (Gfx*)D_0D0077A0);
        rsp_set_matrix_transformation(gObjectList[objectIndex].pos, gObjectList[objectIndex].orientation,
                                      gObjectList[objectIndex].sizeScaling);
        if (gIsGamePaused == 0) {
            gObjectList[objectIndex].unk_0A2 = render_animated_model((Armature*) gObjectList[objectIndex].model,
                                                                     (Animation**) gObjectList[objectIndex].vertex, 0,
                                                                     gObjectList[objectIndex].unk_0A2);
        } else {
            render_animated_model((Armature*) gObjectList[objectIndex].model,
                                  (Animation**) gObjectList[objectIndex].vertex, 0, gObjectList[objectIndex].unk_0A2);
        }
    }
}

void OFlagpole::func_80082F1C(s32 objectIndex) {
    gObjectList[objectIndex].model = (Gfx*) d_course_yoshi_valley_unk5;
    gObjectList[objectIndex].vertex = (Vtx*) d_course_yoshi_valley_unk4;
    gObjectList[objectIndex].sizeScaling = 0.027f;
    object_next_state(objectIndex);
    set_obj_origin_pos(objectIndex, _pos.x * xOrientation, _pos.y, _pos.z);
    set_obj_origin_offset(objectIndex, 0.0f, 0.0f, 0.0f);
    set_obj_orientation(objectIndex, _direction.pitch, _direction.yaw, _direction.roll); // changed from directional_angle to orientation for editor support
}

void OFlagpole::func_80083018(s32 objectIndex) {
    switch (gObjectList[objectIndex].state) {
        case 1:
            OFlagpole::func_80082F1C(objectIndex);
            break;
        case 0:
        default:
            break;
    }
}

void OFlagpole::func_80083060(s32 objectIndex) {
    object_calculate_new_pos_offset(objectIndex);
}
