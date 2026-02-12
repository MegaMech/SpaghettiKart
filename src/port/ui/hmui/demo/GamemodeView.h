#pragma once

#include <utility>
#include <iostream>
#include "LayoutSettings.h"

#include "hmui/Navigator.h"
#include "hmui/widgets/InternalDrawable.h"
#include "hmui/widgets/GestureDetector.h"
#include "hmui/widgets/Column.h"
#include "hmui/widgets/Container.h"
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
                .focusable = true,
                .focusDecorator = FocusDecorator {
                    .color = Color2D(1.0f, 1.0f, 1.0f, 0.8f),
                    .thickness = 2.0f
                },
                .onTap = [i, numModes](std::shared_ptr<InternalDrawable> child, float x, float y) {
                    // Handle tap event
                    std::shared_ptr<D_Container> c = std::dynamic_pointer_cast<D_Container>(child);
                    c->properties.color = BUTTON_ON_TAP_COLOUR;
                    std::cout << "Tapped on child at (" << x << ", " << y << ")\n";

                    if (i == numModes - 1) {
                        Navigator::pop();
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
        return Container(
            .child = Container(
                    .width = INFINITY,
                    .height = INFINITY,
                    //.padding = MARGIN_LEFT_SPACING,
                    .alignment = Alignment::TopLeft(),
                    .clipToBounds = true,
                    .color = BACKGROUND_COLOUR,
                    //.child = Stack(
                        // .children = {
                            //  Image(
                            //      .provider = AssetImage("__OTR__seg2_blue_sky_background_texture"),
                            //      .fit = BoxFit::Cover
                            //  ),
                         .child =    Container(
                                .width = MENU_BUTTON_WIDTH,
                                .height = CONTENT_HEIGHT,
                                .margin = MARGIN_LEFT_SPACING,
                                .child = Scrollable(
                                    .direction = Direction::Vertical,
                                    .child = Column(
                                        .children = entries
                                    )
                            )
                        )
                       // }
                   // )
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

