#pragma once

#include "port/ui/hmui/provider/LUSImageProvider.h"
#include "hmui/graphics/GraphicsContext.h"
#include "hmui/widgets/InternalDrawable.h"
#include "hmui/widgets/GestureDetector.h"
#include "hmui/widgets/Column.h"
#include "hmui/widgets/Container.h"
#include "hmui/widgets/Scrollable.h"
#include "hmui/widgets/Drawable.h"
#include "hmui/widgets/Image.h"
#include "hmui/widgets/Text.h"
#include "hmui/widgets/Stack.h"
#include "hmui/widgets/Image.h"

//#define DEBUG_COMPONENTS

/** Buttons **/

#define MENU_BUTTON_WIDTH 325.0f
#define MENU_BUTTON_HEIGHT 75.0f
#define MENU_BUTTON_COLOUR Color2D(0.0f, 0.0f, 0.0f, 0.0f)

#define MENU_BUTTON_TEXT_COLOUR Color2D(1.0f, 1.0f, 1.0f, 1.0f)
#define MENU_BUTTON_TEXT_SCALE 1.8f

/** General Container Layout **/

#define BACKGROUND_COLOUR Color2D(0.0f, 0.0f, 0.0f, 0.8f)
#define MARGIN_LEFT_SPACING EdgeInsets::only(580.0f, 360.0f, 0.0f, 0.0f)
#define CONTENT_HEIGHT entries.size() * MENU_BUTTON_HEIGHT

/** Events **/

#define BUTTON_ON_TAP_COLOUR Color2D(0.0f, 0.0f, 0.0f, 0.60f)
#define BUTTON_ON_HOVER_COLOUR Color2D(0.0f, 0.0f, 0.0f, 0.40f)

// Shared Background
inline std::shared_ptr<InternalDrawable> BuildMainMenuLayout(
    const std::vector<std::shared_ptr<InternalDrawable>>& entries
) {
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
