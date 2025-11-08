#include <libultraship.h>
#include <libultra/gbi.h>
#include <vector>

#include "Text.h"

#include "port/interpolation/FrameInterpolation.h"
#include "engine/Matrix.h"
#include "engine/editor/EditorMath.h"

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
#include "code_80005FD0.h"
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
    //TextColour = static_cast<COLOR_ID>(params.PrimAlpha.value_or(TEXT_BLUE));

    switch(Mode) {
        case FOLLOW_PLAYER:
            PlayerIndex = static_cast<uint32_t>(params.Behaviour.value_or(0));
            if (PlayerIndex < 0 || PlayerIndex >= NUM_PLAYERS) {
                PlayerIndex = 0;
            }
            break;
    }

    VertColour[0] = {255, 255, 255, 0xFF};
    VertColour[1] = {255, 255, 255, 0xFF};
    VertColour[2] = {255, 255, 255, 0xFF};
    VertColour[3] = {255, 255, 255, 0xFF};

    Scale.x = 0.15f;
    Scale.y = 0.15f;
    Scale.z = 0.15f;


    Text = ValidateString(params.Skin.value_or("My Text"));
    AText::Print3D((char*)Text.c_str(), 0, 1);
}

/**
 * Filters out bad characters (allows a-z, A-Z, 0-9, space)
 * Returns "Blank Text" for blank input
 * Returns "Invalid" if no valid input found
 * Limits str to 20 characters
 */
std::string AText::ValidateString(const std::string_view& s) {
    if (s.empty()) { return "Blank Text"; }

    Text.clear();

    for (char c : s) {
        if (std::isalpha(static_cast<unsigned char>(c)) || 
            std::isdigit(static_cast<unsigned char>(c)) || 
            c == ' ') 
        {
            Text += c;
            if (Text.size() >= 20) {
                break;
            };
        }
    }

    // No valid characters found
    if (Text.empty()) {
        return "Invalid";
    }
    return Text;
}

void AText::Refresh() {
    AText::TextureList.clear();
    AText::Print3D((char*)Text.c_str(), 0, 1);
}

bool AText::IsMod() { return true; }

void AText::SetSpawnParams(SpawnParams& params) {
    AActor::SetSpawnParams(params);
    params.Name = ResourceName;
    params.Location = SpawnPos;
    params.Scale = SpawnScale;
    params.Type = static_cast<int16_t>(Mode);
    params.Behaviour = PlayerIndex;
    //params.PrimAlpha = static_cast<int16_t>(TextColour);
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

    if (gGPCurrentRaceRankByPlayerId[PlayerIndex] == 0) {
        Animate = true;
    } else {
        Animate = false;
    }
}

void AText::Draw(Camera* camera) {
    f32 distance = is_within_render_distance(camera->pos, (float*)&Pos[0], camera->rot[1], Close,
                                             gCameraZoom[camera - camera1], Far);

    if (PlayerIndex == camera->playerId) {
        return; // Do not draw the local players own name
    }

    if (distance == -1.0f) {
        Dist = DistanceProps::TOO_FAR;
    } else if (distance < Close) {
        Dist = DistanceProps::TOO_CLOSE;
    } else {
        Dist = DistanceProps::ACTIVE;
    }

    if (Dist != PrevState) {
        PrevState = Dist;
        if ((distance == -1.0f) || (distance < Close)) {
            FadeState = FADE_OUT;
        } else {
            FadeState = FADE_IN;
        }
    }

    if ((gPlayers[PlayerIndex].effects & BOO_EFFECT) == BOO_EFFECT) {
        FadeState = FADE_OUT;
    }

    AText::SetupMatrix(camera);
}

/**
 * These have been refactored for efficiency purposes.
 * The new method uses 1 matrix to display the whole string
 * And then setting vertex data is done during the setup/constructor phase,
 * instead of during rendering
 * This requires a refresh if the data ever changes
 */
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

    while (*text != '\0') {
        glyphIndex = char_to_glyph_index(text);
        if (glyphIndex >= 0) {
            AText::PrintLetter3D(gGlyphTextureLUT[glyphIndex],
                                            column, row, sp60);
            column = column + (s32) ((gGlyphDisplayWidth[glyphIndex] + tracking) * ScaleX);
        } else if ((glyphIndex != -2) && (glyphIndex == -1)) {
            column = column + (s32) ((tracking + 7) * ScaleX);
        } else {
            return;
        }
        if (glyphIndex >= 0x30) {
            text += 2;
        } else {
            text += 1;
        }
    }
}

void AText::PrintLetter3D(MenuTexture* glyphTexture, f32 column, f32 row, s32 mode) {
    s32 var_v0;
    u8* temp_v0_2;
    f32 thing0;
    f32 thing1;
    MenuTexture* texture;

    texture = glyphTexture;
    while (texture->textureData != NULL) {
        if (texture->textureData != 0) {
            f32 col = texture->dX + column;

            TextureList.emplace_back(CharacterList{
                (const char*) texture->textureData,
                col,
                texture->dY + row,
                texture->width,
                texture->height,
                mode,
                false
            });
        }
        texture++;
    }
    SetupVtx(); // position each letter
}

void AText::SetupVtx() {
    size_t totalWidth = 0;
    for (CharacterList& character : TextureList) {
        totalWidth += character.width;
    }

    for (CharacterList& character : TextureList) {

        Vtx* vtxPtr;
        switch (character.width) {
            default:
                vtxPtr = (Vtx*)&AText::myVtx[18];
                break;
            case 16:
                vtxPtr = (Vtx*)&AText::myVtx[18];
                break;
            case 26:
                vtxPtr = (Vtx*)&AText::myVtx[0];
                break;
            case 30:
                vtxPtr = (Vtx*)&AText::myVtx[36];
                break;
        }

        // memcpy the vtx data into the unique vtx data for this name
        Vtx* vtxSrc = (Vtx*)vtxPtr;
        memcpy(&character.vtx, vtxSrc, sizeof(Vtx) * 4);

        // Set the location for this letter (beside the previous letter)

        // Update the positions for each vtx
        for (size_t i = 0; i < 4; i++) {
            character.vtx[i].v.ob[0] += (s16)(character.column * LetterSpacing) + (character.width / 2);

            // Set the colour for this letter
            character.vtx[i].v.cn[0] = VertColour[i].r;
            character.vtx[i].v.cn[1] = VertColour[i].g;
            character.vtx[i].v.cn[2] = VertColour[i].b;
            character.vtx[i].v.cn[3] = VertColour[i].a;
        }

        // for (size_t i = 0; i < 4; i++) {
        //     character.vtx[i].v.ob[0] += (s16)(totalWidth/2);
        // }

        // This is for centering the text over the players head
        WidthOffset += character.column * LetterSpacing;
    }
}

void AText::SetupMatrix(Camera* camera) { // Based on func_80095BD0
    Mat4 mtx;

    ApplySphericalBIllBoard(mtx, camera->cameraId);

    // Set position
    mtx[3][0] = Pos[0];
    mtx[3][1] = Pos[1];
    mtx[3][2] = Pos[2];

    mtx[0][0] *= Scale.x;
    mtx[1][0] *= Scale.x;
    mtx[2][0] *= Scale.x;
    mtx[0][1] *= Scale.y;
    mtx[1][1] *= Scale.y;
    mtx[2][1] *= Scale.y;
    mtx[0][2] *= Scale.z;
    mtx[1][2] *= Scale.z;
    mtx[2][2] *= Scale.z;

    AddObjectMatrix(mtx, G_MTX_NOPUSH | G_MTX_LOAD | G_MTX_MODELVIEW);

//    FrameInterpolation_RecordOpenChild("actor_text", TAG_LETTER((uintptr_t) this << 8));
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
        //printf("tex texture %p width %d height %d mode %d col %f\n", tex.Texture, tex.width, tex.height, tex.mode, tex.column);
        gDPLoadTextureTile_4b(gDisplayListHead++, (Gfx*)tex.Texture, G_IM_FMT_I, tex.width, 0, 0, 0, tex.width, tex.height + 2, 0,
                            G_TX_NOMIRROR | G_TX_CLAMP, G_TX_NOMIRROR | G_TX_CLAMP, G_TX_NOMASK, G_TX_NOMASK, G_TX_NOLOD,
                            G_TX_NOLOD);

        gSPClearGeometryMode(gDisplayListHead++, G_ZBUFFER);

        if (Animate) {
            AnimateColour(tex.vtx);
        }

        switch(FadeState) {
            case FADE_IN:
                AText::FadeIn(tex.vtx);
                break;
            case FADE_OUT:
                AText::FadeOut(tex.vtx);
                break;
        }

        gSPVertex(gDisplayListHead++, (uintptr_t)tex.vtx, 4, 0);
        gSP2Triangles(gDisplayListHead++, 0, 2, 1, 0, 0, 3, 2, 0);
//        gSPDisplayList(gDisplayListHead++, (Gfx*)common_rectangle_display);

        gSPSetGeometryMode(gDisplayListHead++, G_ZBUFFER);
    }

    gSPDisplayList(gDisplayListHead++, (Gfx*)D_020077D8);
  //  FrameInterpolation_RecordCloseChild();
}

void AText::AnimateColour(Vtx* vtx) {
    u8 r = (sin(gGlobalTimer * 0.1f) * 0.5f + 0.5f) * 200;
    u8 g = (sin(gGlobalTimer * 0.1f + 2.0f) * 0.5f + 0.5f) * 200;
    u8 b = (sin(gGlobalTimer * 0.1f + 4.0f) * 0.5f + 0.5f) * 200;

    for (size_t i = 0; i < 4; i++) {
        vtx[i].v.cn[0] = r;
        vtx[i].v.cn[1] = g;
        vtx[i].v.cn[2] = b;
    }
}

#define fadeSpeed 16
void AText::FadeIn(Vtx* vtx) {
    uint8_t alpha = vtx[0].v.cn[3];
    if (alpha + fadeSpeed > 255) {
        alpha = 255;
        FadeState = NO_FADE;
    } else {
        alpha += fadeSpeed;
    }

    // Apply alpha to all 4 vertices
    for (size_t i = 0; i < 4; i++) {
        vtx[i].v.cn[3] = alpha;
    }
}

void AText::FadeOut(Vtx* vtx) {
    uint8_t alpha = vtx[0].v.cn[3];
    if (alpha < fadeSpeed) {
        alpha = 0;
        FadeState = NO_FADE;
    } else {
        alpha -= fadeSpeed;
    }

    // Apply alpha to all 4 vertices
    for (size_t i = 0; i < 4; i++) {
        vtx[i].v.cn[3] = alpha;
    }
}
#undef fadeSpeed

void AText::DrawEditorProperties() {
    bool updated = false;
    ImGui::Text("Text");
    ImGui::SameLine();

    char text[21] = "";
    strncpy(text, Text.c_str(), sizeof(text));
    ImGui::InputText("Enter text", text, IM_ARRAYSIZE(text));

    Text = std::string(text);

    ImGui::Text("Mode");
    ImGui::SameLine();

    int32_t mode = static_cast<int32_t>(Mode);
    const char* items[] = { "STATIONARY", "FOLLOW PLAYER" };

    if (ImGui::Combo("##Type", &mode, items, IM_ARRAYSIZE(items))) {
        Mode = static_cast<TextMode>(mode);
        updated = true;
    }

    switch(mode) {
        case STATIONARY: {
            ImGui::Text("Location");
            ImGui::SameLine();
            FVector location = GetLocation();
            if (ImGui::DragFloat3("##Location", (float*)&location)) {
                Translate(location);
                gEditor.eObjectPicker.eGizmo.Pos = location;
            }
            break;
        }
        case FOLLOW_PLAYER:
            // Allow setting PlayerIndex
            int32_t playerIdx = PlayerIndex + 1;

            // Draw the input box (ImGui input limited between 1 and 8)
            ImGui::SetNextItemWidth(100);
            if (ImGui::InputInt("Follow Player", &playerIdx)) {
                // Clamp display value to [1, 8]
                if (playerIdx < 1) playerIdx = 1;
                if (playerIdx > 8) playerIdx = 8;

                // Update the internal value (0–7)
                PlayerIndex = playerIdx - 1;
            }

            int32_t height = HeightOffset;
            if (ImGui::InputInt("Height Offset", &height)) {
                HeightOffset = static_cast<uint32_t>(height);
            }
            break;
    }

    DrawColourEditor(&updated);

    ImGui::Text("Transform Settings");
    ImGui::Separator();

    ImGui::SetNextItemWidth(100);
    ImGui::DragFloat("Scale X", &ScaleX, 0.1f, -5.0f, 5.0f, "%.2f");

    ImGui::SetNextItemWidth(100);
    ImGui::DragFloat("Scale Y", &ScaleY, 0.1f, -5.0f, 5.0f, "%.2f");

    ImGui::SetNextItemWidth(100);
    if (ImGui::DragFloat("Letter Spacing", &LetterSpacing, 0.1f, 0.0f, 5.0f, "%.2f")) {
        updated = true;
    }

    if (updated) {
        Refresh();
    }
}

void AText::DrawColourEditor(bool* updated) {
    ImGui::Checkbox("Use single colour", &SingleColour);

    if (SingleColour)
    {
        // Convert 8bit colours to float
        ImVec4 colour(
            VertColour[0].r / 255.0f,
            VertColour[0].g / 255.0f,
            VertColour[0].b / 255.0f,
            VertColour[0].a / 255.0f
        );

        // Single color input
        ImGui::ColorEdit4("Colour", (float*)&colour);
            // Apply same color to all vertices
            for (int i = 0; i < 4; i++) {
                VertColour[i].r = FloatToU8(colour.x);
                VertColour[i].g = FloatToU8(colour.y);
                VertColour[i].b = FloatToU8(colour.z);
                VertColour[i].a = FloatToU8(colour.w);
            }
            *updated = true;
        
    } else {
        // Separate color pickers for each vertex
        for (int i = 0; i < 4; i++)
        {
            ImVec4 colour2(
                VertColour[i].r / 255.0f,
                VertColour[i].g / 255.0f,
                VertColour[i].b / 255.0f,
                VertColour[i].a / 255.0f
            );
            char label[32];
            snprintf(label, sizeof(label), "Vtx %d Colour", i);
            if (ImGui::ColorEdit4(label, (float*)&colour2)) {
                VertColour[i].r = FloatToU8(colour2.x);
                VertColour[i].g = FloatToU8(colour2.y);
                VertColour[i].b = FloatToU8(colour2.z);
                VertColour[i].a = FloatToU8(colour2.w);
                *updated = true;
            }
        }
    }
}