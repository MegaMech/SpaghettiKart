#include "Properties.h"
#include "port/ui/PortMenu.h"
#include "UIWidgets.h"
#include "libultraship/src/Context.h"

#include <imgui.h>
#include <map>
#include <libultraship/libultraship.h>
#include <spdlog/fmt/fmt.h>
#include "spdlog/formatter.h"
#include <common_structs.h>
#include <defines.h>

#include "engine/editor/Editor.h"
#include "port/Game.h"
#include "src/engine/World.h"

extern "C" {
#include "actors.h"
}

namespace Editor {

    PropertiesWindow::~PropertiesWindow() {
        SPDLOG_TRACE("destruct properties window");
    }

    void PropertiesWindow::DrawElement() {

        std::visit([this](auto* obj) {
            using T = std::decay_t<decltype(*obj)>;  // Get the type the pointer is pointing to
            if (nullptr == obj) {
                return;
            }

            if constexpr (std::is_same_v<T, AActor>) {
                if (obj->IsMod()) {
                    ImGui::Text("Actor: %s", obj->Name);
                } else {
                    ImGui::Text("Actor: %s", get_actor_display_name(obj->Type));
                }
            } else if constexpr (std::is_same_v<T, OObject>) {
                ImGui::Text("Object: %s", obj->Name);
            } else if constexpr (std::is_same_v<T, GameObject>) {
                ImGui::Text("EditorObject: %s", obj->Name);
            } else {
                ImGui::Text("Unknown type");
            }
            auto params = obj->_spawnParams;
            ImGui::Text(obj->_spawnParams.Name.c_str());

            if (params.Type.has_value()) {
                std::string label = GetDisplayLabel<T>("Type");
                ImGui::Text("%s: %d", label.c_str(), *params.Type);
            }

            if (params.Behaviour.has_value()) {
                std::string label = GetDisplayLabel<T>("Behaviour");
                ImGui::Text("%s: %d", label.c_str(), *params.Behaviour);
            }

            if (params.Skin.has_value()) {
                std::string label = GetDisplayLabel<T>("Skin");
                ImGui::Text("%s: %d", label.c_str(), *params.Skin);
            }

            if (params.Location.has_value()) {
                std::string label = GetDisplayLabel<T>("Location");
                ImGui::Text("%s: %d", label.c_str(), *params.Location);
            }

            if (params.Rotation.has_value()) {
                std::string label = GetDisplayLabel<T>("Rotation");
                ImGui::Text("%s: %d", label.c_str(), *params.Rotation);
            }

            if (params.Scale.has_value()) {
                std::string label = GetDisplayLabel<T>("Scale");
                ImGui::Text("%s: %d", label.c_str(), *params.Scale);
            }

            if (params.Velocity.has_value()) {
                std::string label = GetDisplayLabel<T>("Velocity");
                ImGui::Text("%s: %d", label.c_str(), *params.Velocity);
            }

            if (params.PatrolStart.has_value()) {
                std::string label = GetDisplayLabel<T>("Velocity");
                ImGui::Text("%s: %d", label.c_str(), *params.Velocity);
            }

            if (params.PathPoint.has_value()) {
                std::string label = GetDisplayLabel<T>("PathPoint");
                ImGui::Text("%s: %d", label.c_str(), *params.PathPoint);
            }

            if (params.Speed.has_value()) {
                std::string label = GetDisplayLabel<T>("Speed");
                ImGui::Text("%s: %.2f", label.c_str(), *params.Speed);
            }

            ImGui::Begin("Properties");

            ImGui::Text("Location");
            ImGui::SameLine();

            FVector location = obj->GetLocation();
            FVector locEdit = location;
            bool positionChanged = ImGui::DragFloat3("##Location", (float*)&locEdit, 0.1f);

            ImGui::SameLine();
            if (ImGui::Button(ICON_FA_UNDO "##ResetPos")) {
                obj->Translate(FVector(0, 0, 0));
                positionChanged = true; // also counts as a change
            }

            if (positionChanged) {
                gEditor.eObjectPicker.eGizmo.Pos = obj->GetLocation();
            }

            ImGui::Text("Rotation");
            ImGui::SameLine();

            IRotator objRot = obj->GetRotation();

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
                obj->Rotate(newRot);
            }

            ImGui::SameLine();
            if (ImGui::Button(ICON_FA_UNDO "##ResetRot")) {
                obj->Rotate(IRotator(0, 0, 0));
            }

            FVector scale = obj->GetScale();
            ImGui::Text("Scale   ");
            ImGui::SameLine();

            ImGui::DragFloat3("##Scale", (float*)&scale, 0.1f);
            obj->SetScale(scale);
            ImGui::SameLine();
            if (ImGui::Button(ICON_FA_UNDO "##ResetScale")) {
                obj->SetScale(FVector(1, 1, 1));
            }

            // This allowed the user to alter the bounding box of the editor selection if it was too small.
            // if (selected->Collision == GameObject::CollisionType::BOUNDING_BOX) {
            //     ImGui::Separator();
            //     ImGui::Text("Editor Bounding Box Size:");
            //     ImGui::PushID("BoundingBoxSize");
            //     ImGui::DragFloat("##BoundingBoxSize", &selected->BoundingBoxSize, 0.1f);
            //     ImGui::SameLine();
            //     if (ImGui::Button(ICON_FA_UNDO)) { selected->BoundingBoxSize = 2.0f; }
            //     ImGui::PopID();
            // }

            ImGui::End();

        }, gEditor.eObjectPicker.eGizmo._selected);
    }

    template <typename T>
    std::string GetDisplayLabel(const std::string& fieldName) {
        if constexpr (requires { T::PropertyLabels(); }) {
            const auto& labels = T::PropertyLabels();
            if (auto it = labels.find(fieldName); it != labels.end()) {
                return it->second;
            }
        }
        return fieldName; // Fallback to field name
    }
}