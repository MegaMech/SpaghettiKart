
#define JSON_USE_IMPLICIT_CONVERSIONS 1
#define JSON_HAS_CPP_20 1

#include "SceneManager.h"
#include "port/Game.h"
#include "engine/CoreMath.h"
#include "World.h"
#include "GameObject.h"

#include <iostream>
#include <fstream>
#include <optional> // Must be before json.hpp
#include <nlohmann/json.hpp>
#include "port/Engine.h"
#include <libultraship/src/resource/type/Json.h>
#include "port/resource/type/Minimap.h"
#include <libultraship/src/resource/File.h>
#include "port/resource/type/ResourceType.h"

#include "engine/objects/Thwomp.h"
#include "engine/objects/Snowman.h"
#include <iostream>

extern "C" {
#include "common_structs.h"
#include "actors.h"
}

namespace Editor {

    std::shared_ptr<Ship::Archive> CurrentArchive;
    std::string SceneFile = "";

    void SaveLevel() {
        auto props = gWorldInstance.CurrentCourse->Props;

        if ((CurrentArchive) && (!SceneFile.empty())) {
            nlohmann::json data;
    
            data["Props"] = props.to_json();

            nlohmann::json staticMesh;

            for (const auto& mesh : gWorldInstance.StaticMeshActors) {
                staticMesh.push_back(mesh->to_json());
            }
            data["StaticMeshActors"] = staticMesh;

            // nlohmann::json actors;

            // for (const auto& actor : gWorldInstance.Actors) {
            //     actors.push_back(actor->to_json());
            // }
            // data["Actors"] = actors;

            // nlohmann::json objects;

            // for (const auto& object : gWorldInstance.Objects) {
            //     objects.push_back(object->to_json());
            // }
            // data["Objects"] = objects;

            try {
                auto dat = data.dump();
                std::vector<uint8_t> stringify;
                stringify.assign(dat.begin(), dat.end());

                bool wrote = GameEngine::Instance->context->GetResourceManager()->GetArchiveManager()->WriteFile(CurrentArchive, SceneFile, stringify);
                if (wrote) {
                    printf("Successfully wrote scene file!\n  Wrote: %s\n", SceneFile.c_str());
                } else {
                    printf("Failed to write scene file!\n");
                }
            } catch (const nlohmann::json::exception& e) {
                printf("SceneManager::SaveLevel():\n  JSON error during dump: %s\n", e.what());
            }
        } else {
            printf("Could not save scene file, SceneFile or CurrentArchive not set\n");
        }
    }

    void LoadLevel(std::shared_ptr<Ship::Archive> archive, Course* course, std::string sceneFile) {
        SceneFile = sceneFile;
    std::cout << "C++ version: " << __cplusplus << std::endl;

        if (archive && (course != nullptr)) {
            auto initData = std::make_shared<Ship::ResourceInitData>();
            initData->Parent = archive;
            initData->Format = RESOURCE_FORMAT_BINARY;
            initData->ByteOrder = Ship::Endianness::Little;
            initData->Type = static_cast<uint32_t>(Ship::ResourceType::Json);
            initData->ResourceVersion = 0;

            nlohmann::json data = std::static_pointer_cast<Ship::Json>(
                GameEngine::Instance->context->GetResourceManager()->LoadResource(sceneFile, true, initData))->Data;

            if (data.is_null() || data.empty()) {
                return;
            }

            // Load the Props (deserialize it)
            if (data.contains("Props")) {
                auto& propsJson = data["Props"];
                try {
                    course->Props.from_json(propsJson);
                } catch(const std::exception& e) {
                    std::cerr << "SceneManager::LoadLevel() Error parsing track properties: " << e.what() << std::endl;
                    std::cerr << "  Is your scene.json file out of date?" << std::endl;
                }
            } else {
                std::cerr << "Props data not found in the JSON file!" << std::endl;
            }

            if (data.contains("Actors")) {
                auto & actorsJson = data["Actors"];
                LoadActors(actorsJson);
            }

            // Load the Actors (deserialize them)
            if (data.contains("StaticMeshActors")) {
                auto& actorsJson = data["StaticMeshActors"];
                gWorldInstance.StaticMeshActors.clear();  // Clear existing actors, if any
                
                for (const auto& actorJson : actorsJson) {
                    Load_AddStaticMeshActor(actorJson);
                }
            } else {
                std::cerr << "Actors data not found in the JSON file!" << std::endl;
            }
        }
    }

    void Load_AddStaticMeshActor(const nlohmann::json& actorJson) {
        gWorldInstance.StaticMeshActors.push_back(new StaticMeshActor("", FVector(0, 0, 0), IRotator(0, 0, 0), FVector(1, 1, 1), "", nullptr));
        auto actor = gWorldInstance.StaticMeshActors.back();
        actor->from_json(actorJson);

        printf("After from_json: Pos(%f, %f, %f), Name: %s, Model: %s\n", 
        actor->Pos.x, actor->Pos.y, actor->Pos.z, actor->Name.c_str(), actor->Model.c_str());
        gEditor.AddObject(actor->Name.c_str(), &actor->Pos, &actor->Rot, &actor->Scale, (Gfx*) nullptr, 1.0f,
                        GameObject::CollisionType::BOUNDING_BOX, 20.0f, (int32_t*) &actor->bPendingDestroy, (int32_t) 1);
    }

    void SetSceneFile(std::shared_ptr<Ship::Archive> archive, std::string sceneFile) {
        CurrentArchive = archive;
        SceneFile = sceneFile;
    }

    void LoadMinimap(std::shared_ptr<Ship::Archive> archive, Course* course, std::string filePath) {
        printf("LOADING MINIMAP %s\n", filePath.c_str());
        if (archive) {
            auto initData = std::make_shared<Ship::ResourceInitData>();
            initData->Parent = archive;
            initData->Format = RESOURCE_FORMAT_BINARY;
            initData->ByteOrder = Ship::Endianness::Little;
            initData->Type = static_cast<uint32_t>(MK64::ResourceType::Minimap);
            initData->ResourceVersion = 0;

            std::shared_ptr<MK64::Minimap> ptr = std::static_pointer_cast<MK64::Minimap>(
                GameEngine::Instance->context->GetResourceManager()->LoadResource(filePath, true, initData));

            if (ptr) {
                printf("LOADED MINIMAP!\n");
                MK64::MinimapTexture texture = ptr->Texture;
                course->Props.Minimap.Texture = (const char*)texture.Data;
                course->Props.Minimap.Width = texture.Width;
                course->Props.Minimap.Height = texture.Height;
            } else { // Fallback
                course->Props.Minimap.Texture = minimap_mario_raceway;
                course->Props.Minimap.Width = ResourceGetTexWidthByName(course->Props.Minimap.Texture);
                course->Props.Minimap.Height = ResourceGetTexHeightByName(course->Props.Minimap.Texture);
            }
        }
    }

    // Used to save and load all game actors to the scene file
    struct SpawnParams {
        std::optional<int16_t> Type = std::nullopt; // OObject type (ex. Emperor penguin, sliding penguin) or literal actor type for AActors
        std::optional<int16_t> Behaviour = std::nullopt;
        std::optional<int32_t> Skin = std::nullopt;

        // std::optional<FVector> Location;
        // std::optional<IRotator> Rotation;   // int16_t
        // std::optional<FVector> Scale;
        // std::optional<FVector> Velocity; // Used by some AActors
        // std::optional<FVector2D> PatrolStart; // OCrab
        // std::optional<FVector2D> PatrolEnd;   // OCrab & Hedgehog
        // std::optional<IPathSpan> PathSpan; // Cheep Cheep

        // // Thwomps
        // std::optional<int16_t> PrimAlpha; // Thwomp
        // std::optional<uint16_t> BoundingBoxSize;

        // // Boos
        // std::optional<uint32_t> Count; // vehicles
        // std::optional<IPathSpan> LeftExitSpan;  // Disable boo
        // std::optional<IPathSpan> TriggerSpan;   // Activate boos
        // std::optional<IPathSpan> RightExitSpan; // Disable boo


        // // Vehicles
        // std::optional<uint32_t> PathIndex; // 0-3 Place vehicle this path
        // std::optional<uint32_t> PathPoint; // Path point index
        // std::optional<bool> Bool; // train tender
        // std::optional<float> Speed; // Train
        // std::optional<float> SpeedB; // cars, trucks, buses, etc.

        NLOHMANN_DEFINE_TYPE_INTRUSIVE(
            SpawnParams,
            Type,
            Behaviour,
            Skin
            // Location,
            // Rotation,And you 
            // Scale,
            // Velocity,
            // PatrolStart,
            // PatrolEnd,
            // PathSpan,
            // PrimAlpha,
            // BoundingBoxSize,
            // Count,
            // LeftExitSpan,
            // TriggerSpan,
            // RightExitSpan,
            // PathIndex,
            // PathPoint,
            // Bool,
            // Speed,
            // SpeedB
        )
    };

    void LoadActors(const nlohmann::json& actorList) {

        for (const auto& actor : actorList) {
            std::string name = actor.at("name").get<std::string>();
            SpawnParams params = actor.at("params").get<SpawnParams>();
            // SpawnParams params = params.test_to_json(actor)

            if (name == "mk:thwomp") {
               // gWorldInstance.AddObject(new OThwomp(params));
            } else if (name == "mk:snowman") {
                //gWorldInstance.AddObject(new OSnowman(params));
            } else if (name == "mk:itembox") {
                // FVector loc = params.Location.value_or(FVector{0, 0, 0});
                // Vec3f pos = { loc.x, loc.y, loc.z };
                // Vec3s rot = {0, 0, 0};
                // Vec3f vel = {0, 0, 0};
                // add_actor_to_empty_slot(pos, rot, vel, 12); // ACTOR_ITEM_BOX
            }
        }
    }
}

/* 
1) Place json export/ import in class
2) make function

*/