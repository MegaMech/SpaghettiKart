#pragma once

#include <utility>
#include <iostream>
#include "LayoutSettings.h"

#include "hmui/Navigator.h"
#include "hmui/widgets/InternalDrawable.h"

#include "port/Game.h"
#include "engine/registry/Registry.h"

extern "C" {
#include "menu_items.h"
}

// singleplayer, split-screen, online
class TrackSelectViewElements : public Drawable {
public:
    // std::vector<Color2D> entries;
    std::vector<std::shared_ptr<InternalDrawable>> entries;

    
    void init() override {
        std::vector<std::shared_ptr<InternalDrawable>> col1;
        std::vector<std::shared_ptr<InternalDrawable>> col2;
        std::vector<std::shared_ptr<InternalDrawable>> col3;

        std::vector<const TrackInfo*> infos = gTrackRegistry.GetAllInfo();

        size_t i = 0;
        for(const TrackInfo* info : infos) {
            auto entry = GestureDetector(
                .onTap = [](std::shared_ptr<InternalDrawable> child, float x, float y) {
                    // Handle tap event
                    std::shared_ptr<D_Container> c = std::dynamic_pointer_cast<D_Container>(child);
                    c->properties.color = BUTTON_ON_TAP_COLOUR;
                    std::cout << "Tapped on child at (" << x << ", " << y << ")\n";
                },
                .onTapRelease = [](std::shared_ptr<InternalDrawable> child, float x, float y) {
                    // Handle tap event
                    std::shared_ptr<D_Container> c = std::dynamic_pointer_cast<D_Container>(child);
                    c->properties.color = MENU_BUTTON_COLOUR;
                    std::cout << "Tapped on child at (" << x << ", " << y << ")\n";
                },
                .onHover = [](std::shared_ptr<InternalDrawable> child, float x, float y) {
                    // Handle hover event
                    std::cout << "Hovered over child at (" << x << ", " << y << ")\n";
                    std::shared_ptr<D_Container> c = std::dynamic_pointer_cast<D_Container>(child);
                    c->properties.color = BUTTON_ON_HOVER_COLOUR;
                },
                .onHoverEnd = [](std::shared_ptr<InternalDrawable> child, float x, float y) {
                    std::shared_ptr<D_Container> c = std::dynamic_pointer_cast<D_Container>(child);
                    c->properties.color = MENU_BUTTON_COLOUR;
                },
                .child = Container(
                    .width = MENU_BUTTON_WIDTH,
                    .height = MENU_BUTTON_HEIGHT,
                    .alignment = Alignment::Center(),
                    .color = MENU_BUTTON_COLOUR,
                    .child = Text(
                        .text = info->Name,
                        .scale = MENU_BUTTON_TEXT_SCALE,
                        .color = MENU_BUTTON_TEXT_COLOUR
                    )
                )
            );

            if (i % 3 == 0)
                col1.push_back(entry);
            else if (i % 3 == 1)
                col2.push_back(entry);
            else
                col3.push_back(entry);

            i += 1;
        }

        entries.push_back(Container(
            .width = 800,
            .height = 1000,
            .child = Scrollable(
                .direction = Direction::Vertical,
                .child = Row(
                    .children = {
                        Column(
                            .children = col1
                        ),
                        Column(
                            .children = col2
                        ),
                        Column(
                            .children = col3
                        )
                    }
                )
            )
        ));
        Drawable::init();
    }

    std::shared_ptr<InternalDrawable> build() override {
        return BuildMainMenuLayout(entries);
    }

    ~TrackSelectViewElements() override = default;
};

class TrackSelectView : public Drawable {
public:
    std::shared_ptr<InternalDrawable> build() override {
        // Render TrackSelectView
        return std::make_shared<TrackSelectViewElements>();
    }

    ~TrackSelectView() override = default;
};
