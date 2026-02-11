#pragma once

#include <utility>
#include <iostream>

#include "hmui/Navigator.h"
#include "hmui/widgets/InternalDrawable.h"
#include "hmui/widgets/GestureDetector.h"
#include "hmui/widgets/Column.h"
#include "hmui/widgets/Container.h"
#include "hmui/widgets/Scrollable.h"
#include "hmui/widgets/Drawable.h"
#include "hmui/widgets/Text.h"
#include "hmui/widgets/Image.h"
#include "hmui/graphics/GraphicsContext.h"

// singleplayer, split-screen, online
class CCViewElements : public Drawable {
public:
    // std::vector<Color2D> entries;
    std::vector<std::shared_ptr<InternalDrawable>> entries;

    void init() override {

        std::string modes[] = {"50CC", "100CC", "150CC", "Extra", "Carriage Return"};

        for(int i = 0; i < 5; ++i) {
            entries.push_back(GestureDetector(
                .onTap = [i](std::shared_ptr<InternalDrawable> child, float x, float y) {
                    // Handle tap event
                    std::shared_ptr<D_Container> c = std::dynamic_pointer_cast<D_Container>(child);
                    c->properties.color = Color2D(0.0f, 0.0f, 0.0f, 0.60f);
                    std::cout << "Tapped on child at (" << x << ", " << y << ")\n";
                    switch(i) {
                        case 4:
                            Navigator::pop();
                            break;
                    }
                },
                .onTapRelease = [](std::shared_ptr<InternalDrawable> child, float x, float y) {
                    // Handle tap event
                    std::shared_ptr<D_Container> c = std::dynamic_pointer_cast<D_Container>(child);
                    c->properties.color = Color2D(0.0f, 0.0f, 0.0f, 0.50f);
                    std::cout << "Tapped on child at (" << x << ", " << y << ")\n";
                },
                .onHover = [](std::shared_ptr<InternalDrawable> child, float x, float y) {
                    // Handle hover event
                    std::cout << "Hovered over child at (" << x << ", " << y << ")\n";
                    std::shared_ptr<D_Container> c = std::dynamic_pointer_cast<D_Container>(child);
                    c->properties.color = Color2D(0.0f, 0.0f, 0.0f, 0.40f);
                },
                .onHoverEnd = [](std::shared_ptr<InternalDrawable> child, float x, float y) {
                    std::shared_ptr<D_Container> c = std::dynamic_pointer_cast<D_Container>(child);
                    c->properties.color = Color2D(0.0f, 0.0f, 0.0f, 0.5f);
                },
                .child = Container(
                    .width = 300.0f,
                    .height = 50.0f,
                    .alignment = Alignment::Center(),
                    .color = Color2D(0.0f, 0.0f, 0.0f, 0.5f),
                    .child = Text(
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
            //.child = Image(
                //.texture = "seg2_blue_sky_backgrounD_Imageure",
                .child = Container(
                    .padding = EdgeInsets::all(5.0f),
                    .alignment = Alignment::CenterLeft(),
                    .clipToBounds = true,
                    .color = Color2D(0.0f, 0.0f, 0.0f, 0.3f),
                    .child = Scrollable(
                        .direction = Direction::Vertical,
                        .child = Column(
                            .children = entries
                        )
                    )
                )
           // )
        );
    }

    ~CCViewElements() override = default;
};

class CCView : public Drawable {
public:
    std::shared_ptr<InternalDrawable> build() override {
        // Render CCView
        return std::make_shared<CCViewElements>();
    }

    ~CCView() override = default;
};
