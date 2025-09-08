#pragma once

#include <libultraship/libultraship.h>
#include "port/Game.h"


namespace Editor {
class PropertiesWindow : public Ship::GuiWindow {
public:
    using Ship::GuiWindow::GuiWindow;
    ~PropertiesWindow();

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

    const std::unordered_map<std::string, std::string> SpawnLabels {
        {"Name", "Name"},
        {"Type", "Type"},
        {"Behaviour", "Behaviour"},
        {"Skin", "Skin"},
        {"Location", "Location"},
        {"Rotation", "Rotation"},
        {"Scale", "Scale"},
        {"Velocity", "Velocity"},
        {"PatrolStart", "Patrol Start"},
        {"PatrolEnd", "Patrol End"},
        {"PathSpan", "Path Span"},
        {"PrimAlpha", "PrimAlpha"},
        {"BoundingBoxSize", "Bounding Box Size"},
        {"Count", "Count"},
        {"LeftExitSpan", "Left Exit Span"},
        {"TriggerSpan", "Trigger Span"},
        {"RightExitSpan", "Right Exit Span"},
        {"PathIndex", "Path Index"},
        {"PathPoint", "Path Point"},
        {"Bool", "Bool"},
        {"Speed", "Speed"},
        {"SpeedB", "SpeedB"},
    };
protected:
    void InitElement() override {};
    void DrawElement() override;
    void UpdateElement() override {};
};
}