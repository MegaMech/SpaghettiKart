#pragma once

#include <utility>
#include <iostream>

#include "hmui/Navigator.h"
#include "hmui/widgets/Column.h"
#include "hmui/widgets/Container.h"
#include "hmui/widgets/GestureDetector.h"
#include "hmui/widgets/Scrollable.h"
#include "hmui/widgets/Drawable.h"
#include "hmui/widgets/Text.h"
#include "hmui/graphics/GraphicsContext.h"

extern "C" {
#include "main.h"
}

class GamemodeViewElements : public Drawable {
public:
    // std::vector<Color2D> entries;
    std::vector<std::shared_ptr<InternalDrawable>> entries;

    void init() override {

        std::string multiplayer_modes[] = {"Grand Prix", "Time Trials", "Versus", "Battle", "Carriage Return"};
        std::string single_player_modes[] = {"Grand Prix", "Time Trials", "Carriage Return"};

        // This is really bad code... Too bad!
        std::string* modes = &multiplayer_modes[0];
        size_t numModes = 5;
        if (gPlayerCountSelection1 == 1) {
            numModes = 3;
            modes = &single_player_modes[0];
        }

        for(size_t i = 0; i < numModes; ++i) {
            entries.push_back(GestureDetector(
                .onTap = [i, numModes](std::shared_ptr<InternalDrawable>& child, float x, float y) {
                    // Handle tap event
                    std::shared_ptr<D_Container> c = std::dynamic_pointer_cast<D_Container>(child);
                    c->properties.color = Color2D(0.0f, 0.0f, 0.0f, 0.60f);
                    std::cout << "Tapped on child at (" << x << ", " << y << ")\n";

                    if (i == numModes - 1) {
                        Navigator::pop();
                    }
                },
                .onTapRelease = [](std::shared_ptr<InternalDrawable>& child, float x, float y) {
                    // Handle tap event
                    std::shared_ptr<D_Container> c = std::dynamic_pointer_cast<D_Container>(child);
                    c->properties.color = Color2D(0.0f, 0.0f, 0.0f, 0.50f);
                    std::cout << "Tapped on child at (" << x << ", " << y << ")\n";
                },
                .onHover = [](std::shared_ptr<InternalDrawable>& child, float x, float y) {
                    // Handle hover event
                    std::cout << "Hovered over child at (" << x << ", " << y << ")\n";
                    std::shared_ptr<D_Container> c = std::dynamic_pointer_cast<D_Container>(child);
                    c->properties.color = Color2D(0.0f, 0.0f, 0.0f, 0.40f);
                },
                .onHoverEnd = [](std::shared_ptr<InternalDrawable>& child, float x, float y) {
                    std::shared_ptr<D_Container> c = std::dynamic_pointer_cast<D_Container>(child);
                    c->properties.color = Color2D(0.0f, 0.0f, 0.0f, 0.5f);
                },
                .child = Container(
                    .width = 300.0f,
                    .height = 50.0f,
                    .alignment = Alignment::Center,
                    .color = Color2D(0.0f, 0.0f, 0.0f, 0.5f),
                    .child = TextW(
                        .text = modes[i],
                        .scale = 1.5f,
                        .color = Color2D(1.0f, 1.0f, 1.0f, 1.0f)
                    ),
                )
            ));
        }
        Drawable::init();
    }

    std::shared_ptr<InternalDrawable> build() override {
        return Container(
            .child = Container(
                .width = (f32)OTRGetGameRenderWidth(),
                .height = (f32)OTRGetGameRenderHeight(),
                .padding = EdgeInsets::all(5.0f),
                .alignment = Alignment::CenterLeft,
                .clipToBounds = true,
                .color = Color2D(0.0f, 0.0f, 0.0f, 0.3f),
                .child = Scrollable(
                    .direction = Direction::Vertical,
                    .child = Column(
                        .children = entries
                    )
                )
            )
        );
    }

    ~GamemodeViewElements() override = default;
};

class GamemodeView : public Drawable {
public:
    std::shared_ptr<InternalDrawable> build() override {
        // Render GamemodeView
        return std::make_shared<GamemodeViewElements>();
    }

    ~GamemodeView() override = default;
};

