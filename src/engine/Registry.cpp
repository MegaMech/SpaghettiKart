#include <functional>
#include <unordered_map>
#include <string>

#include "Registry.h"
#include "engine/CoreMath.h"

#include "AllActors.h"

extern "C" {
#include "actors.h"
}

std::unordered_map<std::string, ActorRegistryEntry> gActorRegistry;

void RegisterActor(const std::string& name,
                   std::function<void(const SpawnParams&)> spawnFunc)
{
    gActorRegistry[name] = { spawnFunc };
}

void Registry_SpawnActor(SpawnParams& params) {
    auto it = gActorRegistry.find(params.Name);
    if (it != gActorRegistry.end() && it->second.spawnFunc) {
        printf("[Registry] Spawned %s\n", params.Name.c_str());
        it->second.spawnFunc(params);
    }
}

void RegisterGameActors() {
    RegisterActor("mk:item_box",
        [](const SpawnParams& actor) {
            FVector loc = actor.Location.value_or(FVector{0, 0, 0});
            Vec3f pos = { loc.x, loc.y, loc.z };
            spawn_item_box(pos);
        }
    );

    RegisterActor("mk:thwomp",
        [](const SpawnParams& params) {
            gWorldInstance.AddObject(new OThwomp(params));
        }
    );

    RegisterActor("mk:snowman",
        [](const SpawnParams& params) {
            gWorldInstance.AddObject(new OSnowman(params));
        }
    );

    RegisterActor("mk:hot_air_balloon",
        [](const SpawnParams& params) {
            gWorldInstance.AddObject(new OHotAirBalloon(params));
        }
    );

    RegisterActor("mk:hedgehog",
        [](const SpawnParams& params) {
            gWorldInstance.AddObject(new OHedgehog(params));
        }
    );

    RegisterActor("mk:grand_prix_balloons",
        [](const SpawnParams& params) {
            gWorldInstance.AddObject(new OGrandPrixBalloons(params));
        }
    );

    RegisterActor("mk:flagpole",
        [](const SpawnParams& params) {
            gWorldInstance.AddObject(new OFlagpole(params));
        }
    );

    RegisterActor("mk:crab",
        [](const SpawnParams& params) {
            gWorldInstance.AddObject(new OCrab(params));
        }
    );

    RegisterActor("mk:cheep_cheep",
        [](const SpawnParams& params) {
            gWorldInstance.AddObject(new OCheepCheep(params));
        }
    );

    RegisterActor("mk:bomb_kart",
        [](const SpawnParams& params) {
            gWorldInstance.AddObject(new OBombKart(params));
        }
    );

    RegisterActor("mk:bat",
        [](const SpawnParams& params) {
            gWorldInstance.AddObject(new OBat(params));
        }
    );

    RegisterActor("mk:boos",
        [](const SpawnParams& params) {
            gWorldInstance.AddObject(new OBoos(params));
        }
    );
}
