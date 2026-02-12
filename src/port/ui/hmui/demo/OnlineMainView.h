#pragma once

#include <utility>
#include <iostream>
#include "LayoutSettings.h"

#include "hmui/Navigator.h"

// singleplayer, split-screen, online
class OnlineMainViewElements : public Drawable {
public:
    // std::vector<Color2D> entries;
    std::vector<std::shared_ptr<InternalDrawable>> entries;

    void init() override {

        std::string modes[] = {"Race!", "Battle!", "Carriage Return"};

        for(int i = 0; i < 3; ++i) {
            entries.push_back(GestureDetector(
                .onTap = [i](std::shared_ptr<InternalDrawable> child, float x, float y) {
                    // Handle tap event
                    std::shared_ptr<D_Container> c = std::dynamic_pointer_cast<D_Container>(child);
                    c->properties.color = BUTTON_ON_TAP_COLOUR;
                    std::cout << "Tapped on child at (" << x << ", " << y << ")\n";
                    switch(i) {
                        case 0:
                            Navigator::push("/online_race");
                            break;
                        case 1:
                            Navigator::push("/online_battle");
                            break;
                        case 2:
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
                        .text = modes[i],
                        .scale = MENU_BUTTON_TEXT_SCALE,
                        .color = MENU_BUTTON_TEXT_COLOUR
                    ),
                )
            ));
        }
        Drawable::init();
    }

    std::shared_ptr<InternalDrawable> build() override {
        return BuildMainMenuLayout(entries);
    }

    ~OnlineMainViewElements() override = default;
};

class OnlineMainView : public Drawable {
public:
    std::shared_ptr<InternalDrawable> build() override {
        // Render OnlineMainView
        return std::make_shared<OnlineMainViewElements>();
    }

    ~OnlineMainView() override = default;
};

