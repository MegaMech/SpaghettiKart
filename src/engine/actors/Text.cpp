#include <libultraship.h>

#include "Text.h"

#include "port/interpolation/FrameInterpolation.h"
#include "engine/Matrix.h"

extern "C" {
#include "defines.h"
#include "main.h"
#include "menu_items.h"
#include "assets/data_segment2.h"
#include "render_player.h"
#include "math_util.h"
#include "assets/texture_data_2.h"
#include "render_objects.h"
#include "common_structs.h"
}

AText::AText(const SpawnParams& params) : AActor(params) {
    Name = "Text";
    ResourceName = "hm:text";
    
    FVector pos = params.Location.value_or(FVector(0.0f, 100.0f, 0.0f));
    SpawnPos = pos;
    Pos[0] = pos.x; Pos[1] = pos.y; Pos[2] = pos.z;

    FVector scale = params.Scale.value_or(FVector(1.0f, 1.0f, 1.0f));
    SpawnScale = Scale = scale;

    Mode = static_cast<TextMode>(params.Type.value_or(0)); // STATIONARY
    TextColour = static_cast<COLOR_ID>(params.PrimAlpha.value_or(TEXT_BLUE));

    switch(Mode) {
        case FOLLOW_PLAYER:
            PlayerIndex = static_cast<uint32_t>(params.Behaviour.value_or(0));
            if (PlayerIndex < 0 || PlayerIndex >= NUM_PLAYERS) {
                PlayerIndex = 0;
            }
            break;
    }

    Text = "My Text";
}

bool AText::IsMod() { return true; }

void AText::SetSpawnParams(SpawnParams& params) {
    AActor::SetSpawnParams(params);
    params.Name = ResourceName;
    params.Location = SpawnPos;
    params.Scale = SpawnScale;
    params.Type = static_cast<int16_t>(Mode);
    params.Behaviour = PlayerIndex;
    params.PrimAlpha = static_cast<int16_t>(TextColour);
}

void AText::Tick() {
    switch(Mode) {
        case STATIONARY:
            break; // Do nothing
        case FOLLOW_PLAYER:
            AText::FollowPlayer();
            break;
    }
}

void AText::FollowPlayer() {
    Pos[0] = gPlayers[PlayerIndex].pos[0];
    Pos[1] = gPlayers[PlayerIndex].pos[1] + HeightOffset;
    Pos[2] = gPlayers[PlayerIndex].pos[2];
}

void AText::Draw(Camera* camera) {
    f32 distance = is_within_render_distance(camera->pos, Pos, camera->rot[1], 2500.0f,
                                             gCameraZoom[camera - camera1], 9000000.0f);

    // if (distance < 122500.0f) {
        AText::Print3D(camera, (char*)Text, 0, 0.55f, 0.55f, 1);
    //}
}

void AText::Print3D(Camera* camera, char* text, s32 tracking, f32 scaleX, f32 scaleY, s32 mode) {
    char* temp_string = text;
    s32 stringWidth = 0;
    s32 glyphIndex;
    s32 sp60;

    s32 column = 0;
    s32 row = 0;

    if (text == NULL) {
        // @port if invalid text is loaded it will skip rendering it.
        return;
    }

    // @port Skip Interpolation, if interpolated later remove this tag
    FrameInterpolation_ShouldInterpolateFrame(false);

    while (*temp_string != 0) {
        glyphIndex = char_to_glyph_index(temp_string);
        if (glyphIndex >= 0) {
            stringWidth += ((gGlyphDisplayWidth[glyphIndex] + tracking) * scaleX);
        } else if ((glyphIndex != -2) && (glyphIndex == -1)) {
            stringWidth += ((tracking + 7) * scaleX);
        } else {
            return;
        }
        if (glyphIndex >= 0x30) {
            temp_string += 2;
        } else {
            temp_string += 1;
        }
    }

    switch (mode) {
        case LEFT_TEXT:
            //! FAKE:
            do {
            } while (0);
        case RIGHT_TEXT:
            column -= stringWidth;
            break;
        case CENTER_TEXT_MODE_1:
        case CENTER_TEXT_MODE_2:
            column -= stringWidth / 2;
            break;
        default:
            break;
    }

    if (mode < 3) {
        sp60 = 1;
    } else {
        sp60 = 2;
    }

    gSPDisplayList(gDisplayListHead++, (Gfx*)D_020077A8);
    while (*text != 0) {
        glyphIndex = char_to_glyph_index(text);
        if (glyphIndex >= 0) {
            // load_menu_img(gGlyphTextureLUT[glyphIndex]);
            gDisplayListHead = AText::PrintLetter3D(camera, gDisplayListHead, gGlyphTextureLUT[glyphIndex],
                                            column, row, sp60, scaleX, scaleY);
            column = column + (s32) ((gGlyphDisplayWidth[glyphIndex] + tracking) * scaleX);
        } else if ((glyphIndex != -2) && (glyphIndex == -1)) {
            column = column + (s32) ((tracking + 7) * scaleX);
        } else {
            gSPDisplayList(gDisplayListHead++, (Gfx*)D_020077D8);
            return;
        }
        if (glyphIndex >= 0x30) {
            text += 2;
        } else {
            text += 1;
        }
    }
    gSPDisplayList(gDisplayListHead++, (Gfx*)D_020077D8);

    // @port Resume Interpolation, if interpolated later remove this tag
    FrameInterpolation_ShouldInterpolateFrame(true);
}

Gfx* AText::PrintLetter3D(Camera* camera, Gfx* arg0, MenuTexture* glyphTexture, f32 column, f32 row, s32 mode, f32 scaleX, f32 scaleY) {
    s32 var_v0;
    u8* temp_v0_2;
    f32 thing0;
    f32 thing1;
    MenuTexture* texture;

    texture = glyphTexture;
    while (texture->textureData != NULL) {
        // var_v0 = 0;

        // thing0 = texture->dX + column;
        // if (thing0 > 320.0f) {
        //     var_v0 = 1;
        // }
        // thing0 += texture->width * scaleX;
        // if (thing0 < 0.0f) {
        //     var_v0 += 1;
        // }
        // thing1 = texture->dY + row;
        // if (thing1 < 0.0f) {
        //     var_v0 += 1;
        // }
        // thing1 -= texture->height * scaleY;
        // if (thing1 > 240.0f) {
        //     var_v0 += 1;
        // }

        //if (var_v0 != 0) {
        //    texture++;
       // } else {
            if (texture->textureData != 0) {
                switch (mode) {
                    case 1:
                        gSPDisplayList(arg0++, (Gfx*)D_020077F8);
                        break;
                    case 2:
                        gSPDisplayList(arg0++, (Gfx*)D_02007818);
                        break;
                }
                TextureList.push_back({
                    (const char*)texture->textureData,
                    texture->dX + column,
                    texture->dY + row,
                    texture->width,
                    texture->height,
                    scaleX,
                    scaleY,
                    mode
                });
                arg0 = AText::SetupMatrix(camera, arg0, (u8*)texture->textureData, texture->dX + column, texture->dY + row,
                                    texture->width, texture->height, scaleX, scaleY);
            }
            texture++;
        //}
    }
    return arg0;
}

Gfx* AText::SetupMatrix(Camera* camera, Gfx* displayListHead, u8* texture, f32 column, f32 row, u32 width, u32 height, f32 scaleX, f32 scaleY) { // Based on func_80095BD0
    Mat4 mf;
    Vtx* vtxPtr;
    Vec3su rot = {0, 0, 0};

    f32 sp48 = sins(-camera->rot[1] - 0x8000);
    f32 temp_f0 = coss(camera->rot[1] - 0x8000);

    mf[0][0] = temp_f0;
    mf[0][0] = scaleX;
    
    mf[0][2] = -sp48;
    mf[2][2] = temp_f0;
    mf[1][0] = 0.0f;
    mf[0][1] = 0.0f;
    mf[2][1] = 0.0f;
    mf[1][2] = 0.0f;
    mf[0][3] = 0.0f;
    mf[1][3] = 0.0f;
    mf[2][3] = 0.0f; // 2c
    mf[2][0] = sp48;
    //mf[1][1] = 1.0f;
    mf[3][3] = 1.0f; // unk3c

    mf[3][0] = Pos[0] + column;
    mf[3][1] = Pos[1]; // + row
    mf[3][2] = Pos[2];

    mf[1][1] = scaleY;
    FrameInterpolation_RecordOpenChild("actor_text", TAG_LETTER((uintptr_t) &texture << 8) + (width + height));
    //ApplyMatrixTransformations(mf, FVector(Pos[0] + column, Pos[1] + row, Pos[2]), *(IRotator*)&Rot, FVector(1.0f, 1.0f, 1.0f));
    //mf[0][2] = 0.0f;
    //mf[2][0] = 0.0f;
   //    mf[2][2] = 1.0f;
    //mf[3][2] = 100.0f;
    displayListHead = AddTextMatrix(displayListHead, mf);

    gDPLoadTextureTile_4b(displayListHead++, (Gfx*)LOAD_ASSET_RAW(texture), G_IM_FMT_I, width, 0, 0, 0, width, height + 2, 0,
                          G_TX_NOMIRROR | G_TX_WRAP, G_TX_NOMIRROR | G_TX_WRAP, G_TX_NOMASK, G_TX_NOMASK, G_TX_NOLOD,
                          G_TX_NOLOD);
    switch (width) {
        default:
            vtxPtr = (Vtx*)D_02007CD8;
            break;
        case 16:
            vtxPtr = (Vtx*)D_02007CD8;
            break;
        case 26:
            vtxPtr = (Vtx*)D_02007BB8;
            break;
        case 30:
            vtxPtr = (Vtx*)D_02007DF8;
            break;
    }
    displayListHead = Colour(displayListHead, vtxPtr);
    FrameInterpolation_RecordCloseChild();
    return displayListHead;
}

Gfx* AText::Colour(Gfx* displayListHead, Vtx* vtxPtr) { // Based on func_800959F8
    s32 index;
    gSPClearGeometryMode(displayListHead++, G_ZBUFFER);
    if ((s32) TextColour < TEXT_BLUE_GREEN_RED_CYCLE_1) {
        index = TextColour;
    } else {
        index = ((TextColour * 2) + ((s32) gGlobalTimer % 2)) - 4;
    }

    Vtx* vtx = (Vtx*)LOAD_ASSET_RAW(vtxPtr);
    gSPVertex(displayListHead++, (uintptr_t)vtx, 2, 0);
    gSPVertex(displayListHead++, (uintptr_t)&vtx[(index + 1) * 2], 2, 2);
    gSPDisplayList(displayListHead++, (Gfx*)common_rectangle_display);

    gSPSetGeometryMode(displayListHead++, G_ZBUFFER);
    return displayListHead;
}