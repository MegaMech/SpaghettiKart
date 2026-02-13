#pragma once

#include <utility>
#include <iostream>
#include "LayoutSettings.h"

#include "hmui/Navigator.h"
#include "hmui/widgets/InternalDrawable.h"
#include "hmui/widgets/Wrap.h"

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

        std::vector<const TrackInfo*> infos = gTrackRegistry.GetAllInfo();

        for(const TrackInfo* info : infos) {
            auto entry = GestureDetector(
                .focusable = true,
                .focusDecorator = FocusDecorator {
                    .color = Color2D(1.0f, 1.0f, 1.0f, 0.8f),
                    .thickness = 2.0f
                },
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

            entries.push_back(entry);
        }
        Drawable::init();
    }

    std::shared_ptr<InternalDrawable> build() override {
        return BuildMainMenuLayout2(
            static_cast<std::shared_ptr<InternalDrawable>>(Wrap(
                    .direction = Direction::Horizontal,
                    .spacing = 10.0f,
                    .runSpacing = 10.0f,
                    .alignment = MainAxisAlignment::CENTER,
                    .children = entries
                )
            ),
            (MENU_BUTTON_WIDTH + 10.0f) * 3, // width
            350 // height
        );
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
