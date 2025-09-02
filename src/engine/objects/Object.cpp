#include <libultraship.h>
#include "Object.h"

#include "World.h"

extern "C" {
    #include "camera.h"
}


    //GameActor()

OObject::OObject() {
}
OObject::OObject(SpawnParams params) {
    _spawnParams = std::move(params);
}

// Virtual functions to be overridden by derived classes
void OObject::Tick() { }
void OObject::Tick60fps() {}
void OObject::Draw(s32 cameraId) { }
void OObject::Expire() { }
void OObject::Destroy() {
    bPendingDestroy = true;
}
void OObject::Reset() { }
void OObject::SetSpawnParams(SpawnParams& params) { }

FVector OObject::GetLocation() const {
    if (_objectIndex != -1) {
        Object* object = &gObjectList[_objectIndex];
        return FVector(object->pos[0], object->pos[1], object->pos[2]);
    }
    printf("Editor tried to get null OObject\n");
    return FVector(0, 0, 0);
};

IRotator OObject::GetRotation() const {
    if (_objectIndex != -1) {
        Object* object = &gObjectList[_objectIndex];
        return IRotator(object->orientation[0], object->orientation[1], object->orientation[2]);
    }
    printf("Editor tried to get null OObject\n");
    return IRotator(0, 0, 0);
}

FVector OObject::GetScale() const {
    if (_objectIndex != -1) {
        Object* object = &gObjectList[_objectIndex];
        return FVector(object->sizeScaling, object->sizeScaling, object->sizeScaling);
    }
    printf("Editor tried to get null OObject\n");
    return FVector(0, 0, 0);
}
void OObject::Translate(FVector pos) {}
void OObject::Rotate(IRotator rot) {}
void OObject::SetScale(FVector scale) {}
