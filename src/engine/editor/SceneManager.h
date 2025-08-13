#include <libultraship/libultraship.h>
#include "engine/courses/Course.h"
#include "CoreMath.h"

namespace Editor {

    // Used to save and load all game actors to the scene file
    struct SpawnParams {
        std::optional<int16_t> Type; // OObject type (ex. Emperor penguin, sliding penguin) or literal actor type for AActors
        std::optional<int16_t> Behaviour;

        std::optional<FVector> Location;
        std::optional<IRotator> Rotation;   // int16_t
        std::optional<FVector> Scale;
        std::optional<FVector> Velocity; // Used by some AActors
        std::optional<FVector2D> PatrolStart; // OCrab
        std::optional<FVector2D> PatrolEnd;   // OCrab & Hedgehog
        std::optional<IPathSpan> PathSpan; // Cheep Cheep

        // Thwomps
        std::optional<int16_t> PrimAlpha; // Thwomp
        std::optional<uint16_t> BoundingBoxSize;

        // Boos
        std::optional<size_t> Count; // vehicles
        std::optional<IPathSpan> LeftExitSpan;  // Disable boo
        std::optional<IPathSpan> TriggerSpan;   // Activate boos
        std::optional<IPathSpan> RightExitSpan; // Disable boo


        // Vehicles
        std::optional<uint32_t> PathPoint; // Path index
        std::optional<bool> Bool; // train tender
        std::optional<float> Speed; // Train
        std::optional<float> SpeedB; // cars, trucks, buses, etc.

        inline nlohmann::json to_json() const {
            nlohmann::json j;
            if (Type) j["type"] = *Type;
            if (Behaviour) j["behaviour"] = *Behaviour;

            if (Location) j["location"] = *Location;
            if (Rotation) j["rotation"] = *Rotation;
            if (Scale) j["scale"] = *Scale;
            if (Velocity) j["velocity"] = *Velocity;

            if (PatrolStart) j["patrol_start"] = *PatrolStart;
            if (PatrolEnd) j["patrol_end"] = *PatrolEnd;
            if (PathSpan) j["path_span"] = *PathSpan;

            if (PrimAlpha) j["prim_alpha"] = *PrimAlpha;
            if (BoundingBoxSize) j["bounding_box_size"] = *BoundingBoxSize;

            if (Count) j["count"] = *Count;
            if (LeftExitSpan) j["left_exit_span"] = *LeftExitSpan;
            if (TriggerSpan) j["trigger_span"] = *TriggerSpan;
            if (RightExitSpan) j["right_exit_span"] = *RightExitSpan;

            if (PathPoint) j["path_point"] = *PathPoint;
            if (Bool) j["bool"] = *Bool;
            if (Speed) j["speed"] = *Speed;
            if (SpeedB) j["speed_b"] = *SpeedB;

            return j;
        }

        inline void from_json(const nlohmann::json& j) {
            if (j.contains("type")) Type = j.at("type").get<int16_t>();
            if (j.contains("behaviour")) Behaviour = j.at("behaviour").get<int16_t>();

            if (j.contains("location")) Location = j.at("location").get<FVector>();
            if (j.contains("rotation")) Rotation = j.at("rotation").get<IRotator>();
            if (j.contains("scale")) Scale = j.at("scale").get<FVector>();
            if (j.contains("velocity")) Velocity = j.at("velocity").get<FVector>();

            if (j.contains("patrol_start")) PatrolStart = j.at("patrol_start").get<FVector2D>();
            if (j.contains("patrol_end")) PatrolEnd = j.at("patrol_end").get<FVector2D>();
            if (j.contains("path_span")) PathSpan = j.at("path_span").get<IPathSpan>();

            if (j.contains("prim_alpha")) PrimAlpha = j.at("prim_alpha").get<int16_t>();
            if (j.contains("bounding_box_size")) BoundingBoxSize = j.at("bounding_box_size").get<uint16_t>();

            if (j.contains("count")) Count = j.at("count").get<size_t>();
            if (j.contains("left_exit_span")) LeftExitSpan = j.at("left_exit_span").get<IPathSpan>();
            if (j.contains("trigger_span")) TriggerSpan = j.at("trigger_span").get<IPathSpan>();
            if (j.contains("right_exit_span")) RightExitSpan = j.at("right_exit_span").get<IPathSpan>();

            if (j.contains("path_point")) PathPoint = j.at("path_point").get<uint32_t>();
            if (j.contains("bool")) Bool = j.at("bool").get<bool>();
            if (j.contains("speed")) Speed = j.at("speed").get<float>();
            if (j.contains("speed_b")) SpeedB = j.at("speed_b").get<float>();
        }
    };

    void SaveLevel();
    void LoadLevel(std::shared_ptr<Ship::Archive> archive, Course* course, std::string sceneFile);
    void Load_AddStaticMeshActor(const nlohmann::json& actorJson);
    void SetSceneFile(std::shared_ptr<Ship::Archive> archive, std::string sceneFile);
    void LoadMinimap(std::shared_ptr<Ship::Archive> archive, Course* course, std::string filePath);

    extern std::shared_ptr<Ship::Archive> CurrentArchive; // This is used to retrieve and write the scene data file
    extern std::string SceneFile;
}
