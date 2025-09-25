#include "Starship.h"

#include <libultra/gbi.h>
#include "Matrix.h"
#include "port/Game.h"

extern "C" {
#include "common_structs.h"
#include "math_util.h"
#include "main.h"
#include "courses/harbour/starship_model.h"
}

AStarship::AStarship(const SpawnParams& params) : AActor(params) {
    Name = "Starship";
    ResourceName = "hm:starship";
    FVector pos = _spawnParams.Location.value_or(FVector(0, 0, 0));
    SetLocation(pos);
    IRotator rot = _spawnParams.Rotation.value_or(IRotator(0, 0, 0));
    Rot[0] = rot.pitch;
    Rot[1] = rot.yaw;
    Rot[2] = rot.roll;
    Scale = _spawnParams.Scale.value_or(FVector(0, 0, 0));
    Model = (const char*)starship_Cube_mesh;
}

void AStarship::Tick() {
    static float angle = 0.0f;

    angle += _spawnParams.Speed.value_or(0.01f);

    // Move relative to the initial position

    FVector pos = GetLocation();
    FVector spawn = _spawnParams.Location.value_or(FVector(0, 0, 0));
    pos.x = spawn.x + _spawnParams.SpeedB.value_or(150.0f) * cosf(angle);
    pos.z = spawn.z + _spawnParams.SpeedB.value_or(150.0f) * sinf(angle);
    SetLocation(pos);

    // Keep y from changing (or adjust it if necessary)
    //Pos.y = Spawn.y;

    // Rotate to face forward along the circle
    Rot[1] = angle * (180.0f / M_PI) + 90.0f;
}

bool AStarship::IsMod() { return true; }

void AStarship::DrawEditorProperties() {
    auto& params = _spawnParams;

    if (params.Location.has_value()) {
        ImGui::Text("Location");
        ImGui::SameLine();
        FVector location = FVector(Pos[0], Pos[1], Pos[2]);
        if (ImGui::DragFloat3("##Location", (float*)&location)) {
            Translate(location);
            *params.Location = location;
            gEditor.eObjectPicker.eGizmo.Pos = location;
        }

        ImGui::SameLine();
        if (ImGui::Button(ICON_FA_UNDO "##ResetPos")) {
            FVector location = FVector(0, 0, 0);
            Translate(location);
            *params.Location = location;
            gEditor.eObjectPicker.eGizmo.Pos = location;
        }
    }

    if (params.Rotation.has_value()) {
        ImGui::Text("Rotation");
        ImGui::SameLine();

        IRotator objRot = GetRotation();

        // Convert to temporary int values (to prevent writing 32bit values to 16bit variables)
        int rot[3] = {
            objRot.pitch,
            objRot.yaw,
            objRot.roll
        };

        if (ImGui::DragInt3("##Rotation", rot, 5.0f)) {
            for (size_t i = 0; i < 3; i++) {
                // Wrap around 0–65535
                rot[i] = (rot[i] % 65536 + 65536) % 65536;
            }
            IRotator newRot;
            newRot.Set(
                static_cast<uint16_t>(rot[0]),
                static_cast<uint16_t>(rot[1]),
                static_cast<uint16_t>(rot[2])
            );
            Rotate(newRot);
            params.Rotation = newRot;
        }

        ImGui::SameLine();
        if (ImGui::Button(ICON_FA_UNDO "##ResetRot")) {
            IRotator rot = IRotator(0, 0, 0);
            Rotate(rot);
            params.Rotation = rot;
        }
    }

    if (params.Scale.has_value()) {
        FVector scale = GetScale();
        ImGui::Text("Scale   ");
        ImGui::SameLine();

        ImGui::DragFloat3("##Scale", (float*)&scale, 0.1f);
        SetScale(scale);
        params.Scale = scale;
        ImGui::SameLine();
        if (ImGui::Button(ICON_FA_UNDO "##ResetScale")) {
            FVector scale = FVector(0.4f, 0.4f, 0.4f);
            SetScale(scale);
            params.Scale = scale;
        }
    }

    if (params.Speed.has_value()) {
        ImGui::Text("Speed");
        ImGui::SameLine();

        float speed = params.Speed.value();
        if (ImGui::DragFloat("##Speed", &speed, 0.01f)) {
            *params.Speed = speed;
        }
        ImGui::SameLine();
        if (ImGui::Button(ICON_FA_UNDO "##ResetSpeed")) {
            *params.Speed = 0.0f;
        }
    }

    if (params.SpeedB.has_value()) {
        ImGui::Text("Radius");
        ImGui::SameLine();

        float speed = params.SpeedB.value();
        if (ImGui::DragFloat("##SpeedB", &speed, 5.0f)) {
            *params.SpeedB = speed;
        }
        ImGui::SameLine();
        if (ImGui::Button(ICON_FA_UNDO "##ResetSpeedB")) {
            *params.SpeedB = 0.0f;
        }
    }
}