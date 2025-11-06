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

    const char* Text;
    TextMode Mode;
    uint32_t PlayerIndex;
    uint32_t HeightOffset = 10; // Place text above player
    float WidthOffset = 0.0f;
    COLOR_ID TextColour;
    f32 ScaleX = 0.55f;
    f32 ScaleY = 0.55f;
    bool Build = true;
    Vtx myVerts[16] = {
    {{{ 0, (s16)65520, 0}, 0, {  0, 0}, {0xff, 0xff, 0xff, 0xff}}},
    {{{16, (s16)65520, 0}, 0, {960, 0}, {0xff, 0xff, 0xff, 0xff}}},
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
};

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
    void FollowPlayer();

    //void Print3D(Camera* camera, char* text, s32 tracking, f32 scaleX, f32 scaleY, s32 mode);
    //Gfx* PrintLetter3D(Camera* camera, Gfx* arg0, MenuTexture* glyphTexture, f32 column, f32 row, s32 mode, f32 scaleX, f32 scaleY);
    //Gfx* SetupMatrix(Camera* camera, Gfx* displayListHead, u8* texture, f32 column, f32 row, u32 width, u32 height, f32 scaleX, f32 scaleY); // Based on func_80095BD0
    //Gfx* Colour(Gfx* displayListHead, Vtx* arg1);
    void Init();
    void Print3D(char* text, s32 tracking, s32 mode);
    void PrintLetter3D(MenuTexture* glyphTexture, f32 column, f32 row, s32 mode);
    void SetupMatrix(Camera* camera); // Based on func_80095BD0
    Gfx* SetupVtx(Gfx* displayListHead, Vtx* arg1);
};
