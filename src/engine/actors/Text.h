#pragma once

#include <libultraship.h>
#include "engine/Actor.h"
#include "src/textures.h"
#include "engine/CoreMath.h"
#include "port/Game.h"

class AText : public AActor {
public:
    enum TextMode : int16_t {
        STATIONARY,
        FOLLOW_PLAYER
    };

    enum FadeMode : int16_t {
        NO_FADE,
        FADE_IN,
        FADE_OUT
    };

    enum DistanceProps : int16_t {
        TOO_CLOSE,
        ACTIVE,
        TOO_FAR
    };

    DistanceProps Dist = ACTIVE;
    DistanceProps PrevState = ACTIVE;
    FadeMode FadeState = NO_FADE;

    struct CharacterList {
        const char* Texture;
        f32 column;
        f32 row;
        u32 width;
        u32 height;
        s32 mode;
        bool end; // end tag
        Vtx vtx[4];
    };

    std::vector<CharacterList> TextureList;

    std::string Text; // The text to be displayed
    TextMode Mode;
    uint32_t PlayerIndex;
    uint32_t HeightOffset = 8; // Place text above player
    float WidthOffset = 0.0f;
    f32 ScaleX = 1.0f;
    f32 ScaleY = 1.0f;
    f32 LetterSpacing = 1.0f;
    f32 Far = 14000.0f;
    f32 Close = 350.0f;

    bool Animate = false;
    bool SingleColour = true;
    // 1 colour for each of the 4 vtx
    // This allows setting each vtx colour individually
    RGBA8 VertColour[4];

    // Constructor
    AText(const SpawnParams& params);
    virtual ~AText() override = default;

    // This is simply a helper function to keep Spawning code clean
    // PlayerIndex is only used if textMode is FOLLOW_PLAYER
    static inline AText* Spawn(FVector pos, FVector scale, AText::TextMode textMode, int16_t playerIndex, COLOR_ID colour) {
        SpawnParams params = {
            .Name = "hm:text",
            .Type = static_cast<int16_t>(textMode),
            .Behaviour = static_cast<int16_t>(playerIndex),
            .Location = pos,
            .Scale = scale,
            .PrimAlpha = static_cast<int16_t>(colour),
        };
        return static_cast<AText*>(gWorldInstance.AddActor(new AText(params)));
    }

    // Virtual functions to be overridden by derived classes
    virtual void Tick() override;
    virtual void Draw(Camera* camera) override;
    virtual void SetSpawnParams(SpawnParams& params) override;
    virtual bool IsMod() override;
    virtual void DrawEditorProperties() override;
      void DrawColourEditor(bool* updated);
    void FollowPlayer();

    //void Print3D(Camera* camera, char* text, s32 tracking, f32 scaleX, f32 scaleY, s32 mode);
    //Gfx* PrintLetter3D(Camera* camera, Gfx* arg0, MenuTexture* glyphTexture, f32 column, f32 row, s32 mode, f32 scaleX, f32 scaleY);
    //Gfx* SetupMatrix(Camera* camera, Gfx* displayListHead, u8* texture, f32 column, f32 row, u32 width, u32 height, f32 scaleX, f32 scaleY); // Based on func_80095BD0
    //Gfx* Colour(Gfx* displayListHead, Vtx* arg1);
    std::string ValidateString(const std::string_view& text);
    void Refresh();
    void Print3D(char* text, s32 tracking, s32 mode);
    void PrintLetter3D(MenuTexture* glyphTexture, f32 column, f32 row, s32 mode);
    void SetupVtx();
    void SetupMatrix(Camera* camera); // Based on func_80095BD0
    void AnimateColour(Vtx* vtx); // Animate the vtx colours
    void FadeIn(Vtx* vtx);
    void FadeOut(Vtx* vtx);

    inline uint8_t FloatToU8(float v) {
        return (uint8_t)(v * 255.0f);
    }

    Vtx myVtx[54] = { // D_02007BB8
        {{{ 0, 16, 0}, 0, {   0, 0}, {0xff, 0xff, 0xff, 0xff}}},
        {{{26, 16, 0}, 0, {1600, 0}, {0xff, 0xff, 0xff, 0xff}}},
        {{{26, 0, 0}, 0, {1600, 960}, {0x00, 0x00, 0xff, 0xff}}},
        {{{ 0, 0, 0}, 0, {   0, 960}, {0x00, 0x00, 0xff, 0xff}}},
        {{{26, 0, 0}, 0, {1600, 960}, {0x00, 0xff, 0x00, 0xff}}},
        {{{ 0, 0, 0}, 0, {   0, 960}, {0x00, 0xff, 0x00, 0xff}}},
        {{{26, 0, 0}, 0, {1600, 960}, {0xff, 0x00, 0x00, 0xff}}},
        {{{ 0, 0, 0}, 0, {   0, 960}, {0xff, 0x00, 0x00, 0xff}}},
        {{{26, 0, 0}, 0, {1600, 960}, {0xff, 0xff, 0x00, 0xff}}},
        {{{ 0, 0, 0}, 0, {   0, 960}, {0xff, 0xff, 0x00, 0xff}}},
        {{{26, 0, 0}, 0, {1600, 960}, {0xff, 0x00, 0x00, 0xff}}},
        {{{ 0, 0, 0}, 0, {   0, 960}, {0xff, 0x00, 0x00, 0xff}}},
        {{{26, 0, 0}, 0, {1600, 960}, {0xff, 0x00, 0x00, 0xff}}},
        {{{ 0, 0, 0}, 0, {   0, 960}, {0xff, 0x00, 0x00, 0xff}}},
        {{{26, 0, 0}, 0, {1600, 960}, {0xff, 0x00, 0x00, 0xff}}},
        {{{ 0, 0, 0}, 0, {   0, 960}, {0xff, 0x00, 0x00, 0xff}}},
        {{{26, 0, 0}, 0, {1600, 960}, {0xff, 0x00, 0x00, 0xff}}},
        {{{ 0, 0, 0}, 0, {   0, 960}, {0xff, 0x00, 0x00, 0xff}}},
        {{{ 0, 16, 0}, 0, {  0, 0}, {0xff, 0xff, 0xff, 0xff}}},
        {{{16, 16, 0}, 0, {960, 0}, {0xff, 0xff, 0xff, 0xff}}},
        {{{16, 0, 0}, 0, {960, 960}, {0x00, 0x00, 0xff, 0xff}}},
        {{{ 0, 0, 0}, 0, {  0, 960}, {0x00, 0x00, 0xff, 0xff}}},
        {{{16, 0, 0}, 0, {960, 960}, {0x00, 0xff, 0x00, 0xff}}},
        {{{ 0, 0, 0}, 0, {  0, 960}, {0x00, 0xff, 0x00, 0xff}}},
        {{{16, 0, 0}, 0, {960, 960}, {0xff, 0x00, 0x00, 0xff}}},
        {{{ 0, 0, 0}, 0, {  0, 960}, {0xff, 0x00, 0x00, 0xff}}},
        {{{16, 0, 0}, 0, {960, 960}, {0xff, 0xff, 0x00, 0xff}}},
        {{{ 0, 0, 0}, 0, {  0, 960}, {0xff, 0xff, 0x00, 0xff}}},
        {{{16, 0, 0}, 0, {960, 960}, {0xff, 0x00, 0x00, 0xff}}},
        {{{ 0, 0, 0}, 0, {  0, 960}, {0xff, 0x00, 0x00, 0xff}}},
        {{{16, 0, 0}, 0, {960, 960}, {0xff, 0x00, 0x00, 0xff}}},
        {{{ 0, 0, 0}, 0, {  0, 960}, {0xff, 0x00, 0x00, 0xff}}},
        {{{16, 0, 0}, 0, {960, 960}, {0xff, 0x00, 0x00, 0xff}}},
        {{{ 0, 0, 0}, 0, {  0, 960}, {0xff, 0x00, 0x00, 0xff}}},
        {{{16, 0, 0}, 0, {960, 960}, {0xff, 0x00, 0x00, 0xff}}},
        {{{ 0, 0, 0}, 0, {  0, 960}, {0xff, 0x00, 0x00, 0xff}}},
        {{{ 0, 32, 0}, 0, {   0, 0}, {0xff, 0xff, 0xff, 0xff}}},
        {{{30, 32, 0}, 0, {1856, 0}, {0xff, 0xff, 0xff, 0xff}}},
        {{{30, 0, 0}, 0, {1856, 1984}, {0x00, 0x00, 0xff, 0xff}}},
        {{{ 0, 0, 0}, 0, {   0, 1984}, {0x00, 0x00, 0xff, 0xff}}},
        {{{30, 0, 0}, 0, {1856, 1984}, {0x00, 0xff, 0x00, 0xff}}},
        {{{ 0, 0, 0}, 0, {   0, 1984}, {0x00, 0xff, 0x00, 0xff}}},
        {{{30, 0, 0}, 0, {1856, 1984}, {0xff, 0x00, 0x00, 0xff}}},
        {{{ 0, 0, 0}, 0, {   0, 1984}, {0xff, 0x00, 0x00, 0xff}}},
        {{{30, 0, 0}, 0, {1856, 1984}, {0xff, 0xff, 0x00, 0xff}}},
        {{{ 0, 0, 0}, 0, {   0, 1984}, {0xff, 0xff, 0x00, 0xff}}},
        {{{30, 0, 0}, 0, {1856, 1984}, {0xff, 0x00, 0x00, 0xff}}},
        {{{ 0, 0, 0}, 0, {   0, 1984}, {0xff, 0x00, 0x00, 0xff}}},
        {{{30, 0, 0}, 0, {1856, 1984}, {0xff, 0x00, 0x00, 0xff}}},
        {{{ 0, 0, 0}, 0, {   0, 1984}, {0xff, 0x00, 0x00, 0xff}}},
        {{{30, 0, 0}, 0, {1856, 1984}, {0xff, 0x00, 0x00, 0xff}}},
        {{{ 0, 0, 0}, 0, {   0, 1984}, {0xff, 0x00, 0x00, 0xff}}},
        {{{30, 0, 0}, 0, {1856, 1984}, {0xff, 0x00, 0x00, 0xff}}},
        {{{ 0, 0, 0}, 0, {   0, 1984}, {0xff, 0x00, 0x00, 0xff}}},
    };
};
