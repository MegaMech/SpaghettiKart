#pragma once

#include <utility>
#include <iostream>
#include "LayoutSettings.h"

#include "hmui/Navigator.h"
#include "hmui/widgets/InternalDrawable.h"
#include "hmui/widgets/Wrap.h"

extern "C" {
#include "menu_items.h"
}

// singleplayer, split-screen, online
class PlayerSelectViewElements : public Drawable {
public:
    // std::vector<Color2D> entries;
    std::vector<std::shared_ptr<InternalDrawable>> entries;

    void init() override {

        for(int i = 0; i < 8; ++i) {
            entries.push_back(GestureDetector(
                .onTap = [i](std::shared_ptr<InternalDrawable> child, float x, float y) {
                    // Handle tap event
                    std::shared_ptr<D_Container> c = std::dynamic_pointer_cast<D_Container>(child);
                    c->properties.color = BUTTON_ON_TAP_COLOUR;
                    std::cout << "Tapped on child at (" << x << ", " << y << ")\n";
                    Navigator::push("/track_select");
                    switch(i) {
                        case 4:
                            Navigator::pop();
                            break;
                    }

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
                        .text = D_800E76A8[i], // names
                        .scale = MENU_BUTTON_TEXT_SCALE,
                        .color = MENU_BUTTON_TEXT_COLOUR
                    ),
                )
            ));
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
            (MENU_BUTTON_WIDTH + 10.0f) * PLAYER_SELECT_ROWS,
            entries.size() * MENU_BUTTON_HEIGHT
        );
    }

    ~PlayerSelectViewElements() override = default;
};

class PlayerSelectView : public Drawable {
public:
    std::shared_ptr<InternalDrawable> build() override {
        // Render PlayerSelectView
        return std::make_shared<PlayerSelectViewElements>();
    }

    ~PlayerSelectView() override = default;
};
