#pragma once

#include <vector>
#include <optional>
#include <string>
#include <nlohmann/json.hpp>
#include "CoreMath.h"
#include <unordered_map>

// Used to save and load all game actors to the scene file
struct SpawnParams {
    std::string Name; // Must use format mk:actor_name for stock game, mymodname:myactorname for mods
    std::optional<int16_t> Type; // OObject type (ex. Emperor penguin, sliding penguin) or literal actor type for AActors
    std::optional<int16_t> Behaviour;
    std::optional<std::string> Skin;

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
    std::optional<uint32_t> Count; // vehicles
    std::optional<IPathSpan> LeftExitSpan;  // Disable boo
    std::optional<IPathSpan> TriggerSpan;   // Activate boos
    std::optional<IPathSpan> RightExitSpan; // Disable boo


    // Vehicles
    std::optional<uint32_t> PathIndex; // 0-3 Place vehicle this path
    std::optional<uint32_t> PathPoint; // Path point index
    std::optional<bool> Bool; // train tender
    std::optional<float> Speed; // Train
    std::optional<float> SpeedB; // cars, trucks, buses, etc.

    NLOHMANN_DEFINE_TYPE_INTRUSIVE(
        SpawnParams,
        Name,
        Type,
        Behaviour,
        Skin,
        Location,
        Rotation,
        Scale,
        Velocity,
        PatrolStart,
        PatrolEnd,
        PathSpan,
        PrimAlpha,
        BoundingBoxSize,
        Count,
        LeftExitSpan,
        TriggerSpan,
        RightExitSpan,
        PathIndex,
        PathPoint,
        Bool,
        Speed,
        SpeedB
    )
};

std::unordered_map<std::string, std::string> SpawnLabels {
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