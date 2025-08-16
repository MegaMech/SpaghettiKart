#include <functional>
#include <unordered_map>
#include <string>

#include "Registry.h"
#include "engine/CoreMath.h"

#include "engine/objects/Thwomp.h"
#include "engine/objects/Snowman.h"

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
        it->second.spawnFunc(params);
    }
}

void RegisterGameActors() {
    RegisterActor("mk:item_box",
        [](const SpawnParams& actor) {
            FVector loc = actor.Location.value_or(FVector{0, 0, 0});
            Vec3f pos = { loc.x, loc.y, loc.z };
            spawn_item_box(pos);
            printf("SPAWNED ITEMBOX\n");
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
}
