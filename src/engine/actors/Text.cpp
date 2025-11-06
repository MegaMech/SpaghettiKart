#include <libultraship.h>
#include <libultra/gbi.h>

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

void AText::Init() {
    AText::Print3D((char*)Text, 0, 1);
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
    Pos[0] = gPlayers[PlayerIndex].pos[0] - ((s16) (WidthOffset / 4)); // Center above player
    Pos[1] = gPlayers[PlayerIndex].pos[1] + HeightOffset;
    Pos[2] = gPlayers[PlayerIndex].pos[2];
}

void AText::Draw(Camera* camera) {
    f32 distance = is_within_render_distance(camera->pos, (float*)&Pos[0], camera->rot[1], 800.0f,
                                             gCameraZoom[camera - camera1], 14000.0f);

    if (PlayerIndex == camera->playerId) {
        return; // Do not draw the local players own name
    }

    if (distance < -1.0f) {
        //printf("list %d\n", TextureList.size());
        //AText::Print3D(camera, (char*)Text, 0, 0.55f, 0.55f, 1);
        AText::SetupMatrix(camera);
    }
}

void AText::Print3D(char* text, s32 tracking, s32 mode) {
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

    while (*temp_string != '\0') {
        glyphIndex = char_to_glyph_index(temp_string);
        if (glyphIndex >= 0) {
            stringWidth += ((gGlyphDisplayWidth[glyphIndex] + tracking) * ScaleX);
        } else if ((glyphIndex != -2) && (glyphIndex == -1)) {
            stringWidth += ((tracking + 7) * ScaleX);
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

    //gSPDisplayList(gDisplayListHead++, (Gfx*)D_020077A8);
    while (*text != '\0') {
        glyphIndex = char_to_glyph_index(text);
        if (glyphIndex >= 0) {
            // load_menu_img(gGlyphTextureLUT[glyphIndex]);
            AText::PrintLetter3D(gGlyphTextureLUT[glyphIndex],
                                            column, row, sp60);
            column = column + (s32) ((gGlyphDisplayWidth[glyphIndex] + tracking) * ScaleX);
        } else if ((glyphIndex != -2) && (glyphIndex == -1)) {
            column = column + (s32) ((tracking + 7) * ScaleX);
        } else {
            //TextureList.push_back({nullptr, 0, 0, 0, 0, 0, true});
            //gSPDisplayList(gDisplayListHead++, (Gfx*)D_020077D8);
            return;
        }
        if (glyphIndex >= 0x30) {
            text += 2;
        } else {
            text += 1;
        }
    }
    //gSPDisplayList(gDisplayListHead++, (Gfx*)D_020077D8);

    // @port Resume Interpolation, if interpolated later remove this tag
    FrameInterpolation_ShouldInterpolateFrame(true);
}

void AText::PrintLetter3D(MenuTexture* glyphTexture, f32 column, f32 row, s32 mode) {
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
                        //gSPDisplayList(arg0++, (Gfx*)D_020077F8);
                        break;
                    case 2:
                        //gSPDisplayList(arg0++, (Gfx*)D_02007818);
                        break;
                }

                TextureList.push_back({
                    (const char*)texture->textureData,
                    texture->dX + column,
                    texture->dY + row,
                    texture->width,
                    texture->height,
                    mode,
                    false
                });
                WidthOffset += texture->dX + column;
                //arg0 = AText::SetupMatrix(camera, arg0, (u8*)texture->textureData, texture->dX + column, texture->dY + row,
                //                    texture->width, texture->height, scaleX, scaleY);
            }
            texture++;
        //}
    }
}

// Vtx myVtx[] = {
//     {{{ 0, 65520, 0}, 0, {  0, 0}, {0xff, 0xff, 0xff, 0xff}}},
//     {{{16, 65520, 0}, 0, {960, 0}, {0xff, 0xff, 0xff, 0xff}}},
//     {{{16, 0, 0}, 0, {960, 960}, {0x00, 0x00, 0xff, 0xff}}},
//     {{{ 0, 0, 0}, 0, {  0, 960}, {0x00, 0x00, 0xff, 0xff}}},
//     {{{16, 0, 0}, 0, {960, 960}, {0x00, 0xff, 0x00, 0xff}}},
//     {{{ 0, 0, 0}, 0, {  0, 960}, {0x00, 0xff, 0x00, 0xff}}},
//     {{{16, 0, 0}, 0, {960, 960}, {0xff, 0x00, 0x00, 0xff}}},
//     {{{ 0, 0, 0}, 0, {  0, 960}, {0xff, 0x00, 0x00, 0xff}}},
//     {{{16, 0, 0}, 0, {960, 960}, {0xff, 0xff, 0x00, 0xff}}},
//     {{{ 0, 0, 0}, 0, {  0, 960}, {0xff, 0xff, 0x00, 0xff}}},
//     {{{16, 0, 0}, 0, {960, 960}, {0xff, 0x00, 0x00, 0xff}}},
//     {{{ 0, 0, 0}, 0, {  0, 960}, {0xff, 0x00, 0x00, 0xff}}},
//     {{{16, 0, 0}, 0, {960, 960}, {0xff, 0x00, 0x00, 0xff}}},
//     {{{ 0, 0, 0}, 0, {  0, 960}, {0xff, 0x00, 0x00, 0xff}}},
//     {{{16, 0, 0}, 0, {960, 960}, {0xff, 0x00, 0x00, 0xff}}},
//     {{{ 0, 0, 0}, 0, {  0, 960}, {0xff, 0x00, 0x00, 0xff}}},
// };

Vtx letter_vtx[] = {
    {{{ 0, 65520, 0}, 0, {  0, 0}, {0xff, 0xff, 0xff, 0xff}}},
    {{{26, 65520, 0}, 0, {960, 0}, {0xff, 0xff, 0xff, 0xff}}},
    {{{26, 0, 0}, 0, {960, 960}, {0, 0, 0xff, 0xff}}},
    {{{0, 0, 0}, 0, {0, 960}, {0, 0, 0xff, 0xff}}},
};

Vtx letter_vtx2[] = {
    {{{ 26, 65520, 0}, 0, {  0, 0}, {0xff, 0xff, 0xff, 0xff}}},
    {{{52, 65520, 0}, 0, {960, 0}, {0xff, 0xff, 0xff, 0xff}}},
    {{{52, 0, 0}, 0, {960, 960}, {0, 0, 0xff, 0xff}}},
    {{{26, 0, 0}, 0, {0, 960}, {0, 0, 0xff, 0xff}}},
};

void AText::SetupMatrix(Camera* camera) { // Based on func_80095BD0
    Mat4 mf;
    Vtx* vtxPtr;
    Vec3su rot = {0, 0, 0};

    f32 sp48 = sins((camera->rot[1]) - 0x8000);
    f32 temp_f0 = coss(camera->rot[1] - 0x8000);

    mf[0][0] = temp_f0;
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

    mf[3][0] = Pos[0]; // + tex.column;
    mf[3][1] = Pos[1]; // + row
    mf[3][2] = Pos[2];

    // Apply scaling
    mf[0][0] *= ScaleX;
    mf[1][1] = -ScaleY;

//    FrameInterpolation_RecordOpenChild("actor_text", TAG_LETTER((uintptr_t) this << 8));
    AddObjectMatrix(mf, G_MTX_NOPUSH | G_MTX_LOAD | G_MTX_MODELVIEW);
    gSPDisplayList(gDisplayListHead++, (Gfx*)D_020077A8);
        switch (1) {
            case 1:
                gSPDisplayList(gDisplayListHead++, (Gfx*)D_020077F8);
                break;
            case 2:
                gSPDisplayList(gDisplayListHead++, (Gfx*)D_02007818);
                break;
        }

    for (CharacterList& tex : TextureList) {
        printf("tex texture %p width %d height %d mode %d col %f\n", tex.Texture, tex.width, tex.height, tex.mode, tex.column);
        gDPLoadTextureTile_4b(gDisplayListHead++, (Gfx*)tex.Texture, G_IM_FMT_I, tex.width, 0, 0, 0, tex.width, tex.height + 2, 0,
                            G_TX_NOMIRROR | G_TX_CLAMP, G_TX_NOMIRROR | G_TX_CLAMP, G_TX_NOMASK, G_TX_NOMASK, G_TX_NOLOD,
                            G_TX_NOLOD);

        switch (tex.width) {
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

        s32 index;
        gSPClearGeometryMode(gDisplayListHead++, G_ZBUFFER);
        if ((s32) TextColour < TEXT_BLUE_GREEN_RED_CYCLE_1) {
            index = TextColour;
        } else {
            index = ((TextColour * 2) + ((s32) gGlobalTimer % 2)) - 4;
        }

        Vtx* vtxSrc = (Vtx*)LOAD_ASSET_RAW(vtxPtr); // important copy
        memcpy(tex.vtx, vtxSrc, sizeof(Vtx) * 2);

        memcpy(&tex.vtx[2], &vtxSrc[(index + 1) * 2], sizeof(Vtx) * 2);

        tex.vtx[0].v.ob[0] += (s16)(tex.column * 2.1f);
        tex.vtx[1].v.ob[0] += (s16)(tex.column * 2.1f);
        tex.vtx[2].v.ob[0] += (s16)(tex.column * 2.1f);
        tex.vtx[3].v.ob[0] += (s16)(tex.column * 2.1f);

        gSPVertex(gDisplayListHead++, (uintptr_t)tex.vtx, 4, 0);
        gSPDisplayList(gDisplayListHead++, (Gfx*)common_rectangle_display);

        gSPSetGeometryMode(gDisplayListHead++, G_ZBUFFER);
    }

    gSPDisplayList(gDisplayListHead++, (Gfx*)D_020077D8);
  //  FrameInterpolation_RecordCloseChild();
}

// Gfx* AText::SetupVtx(Gfx* displayListHead, Vtx* vtxPtr) { // Based on func_800959F8

//     return displayListHead;
// }
