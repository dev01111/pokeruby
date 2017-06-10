#include "global.h"
#include "asm.h"
#include "battle.h"
#include "battle_anim.h"
#include "battle_interface.h"
#include "blend_palette.h"
#include "data2.h"
#include "decompress.h"
#include "main.h"
#include "m4a.h"
#include "palette.h"
#include "pokemon.h"
#include "songs.h"
#include "sound.h"
#include "species.h"
#include "sprite.h"
#include "task.h"
#include "gba/m4a_internal.h"

struct Struct2019348
{
    u8 filler0[2];
    u16 unk2;
    u8 filler4[4];
    u32 unk8;
    u32 unkC;
    u32 unk10;
};

extern struct MusicPlayerInfo gMPlay_SE1;
extern struct MusicPlayerInfo gMPlay_SE2;
extern u8 gUnknown_02024A60;
extern u8 gUnknown_02024A68;
extern u16 gUnknown_02024A6A[];
extern u8 gUnknown_02024A72[];
extern u8 gUnknown_02024BE0[];
extern u16 gUnknown_02024DE8;
extern u32 gUnknown_02024E70[];
extern u8 gUnknown_03004340[];
extern u16 gBattleTypeFlags;
extern u8 gBattleMonForms[];
extern u8 gBattleAnimPlayerMonIndex;
extern u8 gBattleAnimEnemyMonIndex;
extern void (*gAnimScriptCallback)(void);
extern u8 gAnimScriptActive;
extern const u8 *const gBattleAnims_Unknown1[];
extern const u8 *const gBattleAnims_Unknown2[];
extern const struct SpriteSheet gTrainerFrontPicTable[];
extern const struct MonCoords gTrainerFrontPicCoords[];
extern const struct SpritePalette gTrainerFrontPicPaletteTable[];
extern const struct SpriteSheet gUnknown_0820A47C;
extern const u8 gSubstituteDollTilemap[];
extern const u8 gSubstituteDollGfx[];
extern const u8 gSubstituteDollPal[];
extern const struct SpriteSheet gUnknown_0820A484;
extern const struct SpriteSheet gUnknown_0820A48C[];
extern const struct SpriteSheet gUnknown_0820A49C[];
extern const struct SpriteSheet gUnknown_0820A4AC;
extern const struct SpriteSheet gUnknown_0820A4B4[];
extern const struct SpritePalette gUnknown_0820A4D4[];
extern const u8 gUnknown_08D09C48[];

#define ewram19348 (*(struct Struct2019348 *)(ewram + 0x19348))

extern u8 sub_8078874(u8);
extern u8 sub_8077F68(u8);
extern u8 sub_8077F7C(u8);
extern void sub_8094958(void);
extern const u16 *pokemon_get_pal(struct Pokemon *);
extern void sub_80105DC(struct Sprite *);
extern void move_anim_start_t2();
extern const u16 *species_and_otid_get_pal();

void sub_80315E8(u8);
u8 sub_803163C(u8);
void sub_80316CC(u8);
void sub_8031F0C(void);
void refresh_graphics_maybe(u8, u8, u8);
void sub_80324E0(u8 a);

void sub_80327CC(void);

void sub_80312F0(struct Sprite *sprite)
{
    u8 spriteId = sprite->data1;

    if (gSprites[spriteId].affineAnimEnded && !gSprites[spriteId].invisible)
    {
        if (gSprites[spriteId].animPaused)
            gSprites[spriteId].animPaused = FALSE;
        else if (gSprites[spriteId].animEnded)
        {
            gSprites[spriteId].callback = sub_80105DC;
            StartSpriteAffineAnim(&gSprites[spriteId], 0);
            sprite->callback = SpriteCallbackDummy;
        }
    }
}

void unref_sub_8031364(struct Sprite *sprite, bool8 stupid)
{
    sprite->animPaused = TRUE;
    sprite->callback = SpriteCallbackDummy;
    if (!stupid)
        StartSpriteAffineAnim(sprite, 1);
    else
        StartSpriteAffineAnim(sprite, 1);
    AnimateSprite(sprite);
}

void sub_80313A0(struct Sprite *sprite)
{
    if (!(gUnknown_02024DE8 & 1))
    {
        sprite->pos2.x += sprite->data0;
        if (sprite->pos2.x == 0)
            sprite->callback = SpriteCallbackDummy;
    }
}

void move_anim_start_t2_for_situation(u8 a, u32 b)
{
    ewram17810[gUnknown_02024A60].unk0_4 = 1;
    if (a == 0)
    {
        if (b == 0x20)
            move_anim_start_t2(gUnknown_02024A60, 6);
        else if (b == 8 || (b & 0x80))
            move_anim_start_t2(gUnknown_02024A60, 0);
        else if (b == 0x10)
            move_anim_start_t2(gUnknown_02024A60, 2);
        else if (b & 7)
            move_anim_start_t2(gUnknown_02024A60, 4);
        else if (b == 0x40)
            move_anim_start_t2(gUnknown_02024A60, 5);
        else
            ewram17810[gUnknown_02024A60].unk0_4 = 0;
    }
    else
    {
        if (b & 0x000F0000)
            move_anim_start_t2(gUnknown_02024A60, 3);
        else if (b & 7)
            move_anim_start_t2(gUnknown_02024A60, 1);
        else if (b & 0x10000000)
            move_anim_start_t2(gUnknown_02024A60, 7);
        else if (b & 0x08000000)
            move_anim_start_t2(gUnknown_02024A60, 8);
        else if (b & 0x0000E000)
            move_anim_start_t2(gUnknown_02024A60, 9);
        else
            ewram17810[gUnknown_02024A60].unk0_4 = 0;
    }
}

bool8 move_anim_start_t3(u8 a, u8 b, u8 c, u8 d, u16 e)
{
    u8 taskId;

    if (d == 0 && (e & 0x80))
    {
        gBattleMonForms[a] = e & 0x7F;
        return TRUE;
    }
    if (ewram17800[a].unk0_2 && sub_803163C(d) == 0)
        return TRUE;
    if (ewram17800[a].unk0_2 && d == 2 && gSprites[gUnknown_02024BE0[a]].invisible)
    {
        refresh_graphics_maybe(a, 1, gUnknown_02024BE0[a]);
        sub_80324E0(a);
        return TRUE;
    }
    gBattleAnimPlayerMonIndex = b;
    gBattleAnimEnemyMonIndex = c;
    ewram17840.unk0 = e;
    DoMoveAnim(gBattleAnims_Unknown1, d, 0);
    taskId = CreateTask(sub_80315E8, 10);
    gTasks[taskId].data[0] = a;
    ewram17810[gTasks[taskId].data[0]].unk0_5 = 1;
    return FALSE;
}

void sub_80315E8(u8 taskId)
{
    gAnimScriptCallback();
    if (!gAnimScriptActive)
    {
        ewram17810[gTasks[taskId].data[0]].unk0_5 = 0;
        DestroyTask(taskId);
    }
}

u8 sub_803163C(u8 a)
{
    switch (a)
    {
    case 2:
    case 10:
    case 11:
    case 12:
    case 13:
    case 17:
        return 1;
    default:
        return 0;
    }
}

void move_anim_start_t4(u8 a, u8 b, u8 c, u8 d)
{
    u8 taskId;

    gBattleAnimPlayerMonIndex = b;
    gBattleAnimEnemyMonIndex = c;
    DoMoveAnim(gBattleAnims_Unknown2, d, 0);
    taskId = CreateTask(sub_80316CC, 10);
    gTasks[taskId].data[0] = a;
    ewram17810[gTasks[taskId].data[0]].unk0_6 = 1;
}

void sub_80316CC(u8 taskId)
{
    gAnimScriptCallback();
    if (!gAnimScriptActive)
    {
        ewram17810[gTasks[taskId].data[0]].unk0_6 = 0;
        DestroyTask(taskId);
    }
}

u8 sub_8031720(int unused1, int unused2)
{
    return 0;
}

bool8 mplay_80342A4(u8 a)
{
    u8 zero = 0;

    if (IsSEPlaying())
    {
        ewram17810[a].unk8++;
        if (ewram17810[gUnknown_02024A60].unk8 < 30)
            return TRUE;
        m4aMPlayStop(&gMPlay_SE1);
        m4aMPlayStop(&gMPlay_SE2);
    }
    if (zero == 0)
    {
        ewram17810[a].unk8 = 0;
        return FALSE;
    }
    return TRUE;
}

void sub_8031794(struct Pokemon *pkmn, u8 b)
{
    u32 personalityValue;
    u16 species;
    u32 r7;
    u32 otId;
    u8 var;
    u16 paletteOffset;
    const u16 *palette;

    personalityValue = GetMonData(pkmn, MON_DATA_PERSONALITY);
    if (ewram17800[b].unk2 == 0)
    {
        species = GetMonData(pkmn, MON_DATA_SPECIES);
        r7 = personalityValue;
    }
    else
    {
        species = ewram17800[b].unk2;
        r7 = gUnknown_02024E70[b];
    }
    otId = GetMonData(pkmn, MON_DATA_OT_ID);
    var = battle_get_per_side_status(b);
    HandleLoadSpecialPokePic(
      &gMonFrontPicTable[species],
      gMonFrontPicCoords[species].coords,
      gMonFrontPicCoords[species].y_offset,
      0x02000000,
      gUnknown_081FAF4C[var],
      species,
      r7);
    paletteOffset = 0x100 + b * 16;
    if (ewram17800[b].unk2 == 0)
        palette = pokemon_get_pal(pkmn);
    else
        palette = species_and_otid_get_pal(species, otId, personalityValue);
    sub_800D238(palette, ewram);
    LoadPalette(ewram, paletteOffset, 0x20);
    LoadPalette(ewram, 0x80 + b * 16, 0x20);
    if (species == SPECIES_CASTFORM)
    {
        paletteOffset = 0x100 + b * 16;
        sub_800D238(palette, ewram + 0x16400);
        LoadPalette(ewram + 0x16400 + gBattleMonForms[b] * 32, paletteOffset, 0x20);
    }
    if (ewram17800[b].unk2 != 0)
    {
        BlendPalette(paletteOffset, 16, 6, 0x7FFF);
        CpuCopy32(gPlttBufferFaded + paletteOffset, gPlttBufferUnfaded + paletteOffset, 32);
    }
}

void sub_80318FC(struct Pokemon *pkmn, u8 b)
{
    u32 personalityValue;
    u16 species;
    u32 r7;
    u32 otId;
    u8 var;
    u16 paletteOffset;
    const u16 *palette;

    personalityValue = GetMonData(pkmn, MON_DATA_PERSONALITY);
    if (ewram17800[b].unk2 == 0)
    {
        species = GetMonData(pkmn, MON_DATA_SPECIES);
        r7 = personalityValue;
    }
    else
    {
        species = ewram17800[b].unk2;
        r7 = gUnknown_02024E70[b];
    }
    otId = GetMonData(pkmn, MON_DATA_OT_ID);
    var = battle_get_per_side_status(b);
    HandleLoadSpecialPokePic(
      &gMonBackPicTable[species],
      gMonBackPicCoords[species].coords,
      gMonBackPicCoords[species].y_offset,
      0x02000000,
      gUnknown_081FAF4C[var],
      species,
      r7);
    paletteOffset = 0x100 + b * 16;
    if (ewram17800[b].unk2 == 0)
        palette = pokemon_get_pal(pkmn);
    else
        palette = species_and_otid_get_pal(species, otId, personalityValue);
    sub_800D238(palette, ewram);
    LoadPalette(ewram, paletteOffset, 0x20);
    LoadPalette(ewram, 0x80 + b * 16, 0x20);
    if (species == SPECIES_CASTFORM)
    {
        paletteOffset = 0x100 + b * 16;
        sub_800D238(palette, ewram + 0x16400);
        LoadPalette(ewram + 0x16400 + gBattleMonForms[b] * 32, paletteOffset, 0x20);
    }
    if (ewram17800[b].unk2 != 0)
    {
        BlendPalette(paletteOffset, 16, 6, 0x7FFF);
        CpuCopy32(gPlttBufferFaded + paletteOffset, gPlttBufferUnfaded + paletteOffset, 32);
    }
}

void unref_sub_8031A64(void)
{
}

void nullsub_9(u16 unused)
{
}

void sub_8031A6C(u16 a, u8 b)
{
    u8 status;
    struct SpriteSheet spriteSheet;

    status = battle_get_per_side_status(b);
    DecompressPicFromTable_2(
      &gTrainerFrontPicTable[a],
      gTrainerFrontPicCoords[a].coords,
      gTrainerFrontPicCoords[a].y_offset,
      (void *)0x02000000,
      gUnknown_081FAF4C[status],
      0);
    spriteSheet.data = gUnknown_081FAF4C[status];
    spriteSheet.size = gTrainerFrontPicTable[a].size;
    spriteSheet.tag = gTrainerFrontPicTable[a].tag;
    LoadCompressedObjectPic(&spriteSheet);
    LoadCompressedObjectPalette(&gTrainerFrontPicPaletteTable[a]);
}

void sub_8031AF4(u16 a, u8 b)
{
    u8 status;

    status = battle_get_per_side_status(b);
    DecompressPicFromTable_2(
      &gTrainerBackPicTable[a],
      gTrainerBackPicCoords[a].coords,
      gTrainerBackPicCoords[a].y_offset,
      (void *)0x02000000,
      gUnknown_081FAF4C[status],
      0);
    LoadCompressedPalette(gTrainerBackPicPaletteTable[a].data, 0x100 + b * 16, 32);
}

void nullsub_10(int unused)
{
}

void sub_8031B74(u16 a)
{
    FreeSpritePaletteByTag(gTrainerFrontPicPaletteTable[a].tag);
    FreeSpriteTilesByTag(gTrainerFrontPicTable[a].tag);
}

void unref_sub_8031BA0(void)
{
    u8 count;
    u8 i;

    LoadSpritePalette(&gUnknown_0820A4D4[0]);
    LoadSpritePalette(&gUnknown_0820A4D4[1]);
    if (!IsDoubleBattle())
    {
        LoadCompressedObjectPic(&gUnknown_0820A47C);
        LoadCompressedObjectPic(&gUnknown_0820A484);
        count = 2;
    }
    else
    {
        LoadCompressedObjectPic(&gUnknown_0820A48C[0]);
        LoadCompressedObjectPic(&gUnknown_0820A48C[1]);
        LoadCompressedObjectPic(&gUnknown_0820A49C[0]);
        LoadCompressedObjectPic(&gUnknown_0820A49C[1]);
        count = 4;
    }
    for (i = 0; i < count; i++)
        LoadCompressedObjectPic(&gUnknown_0820A4B4[gUnknown_02024A72[i]]);
}

bool8 sub_8031C30(u8 a)
{
    bool8 retVal = FALSE;

    if (a != 0)
    {
        if (a == 1)
        {
            LoadSpritePalette(&gUnknown_0820A4D4[0]);
            LoadSpritePalette(&gUnknown_0820A4D4[1]);
        }
        else if (!IsDoubleBattle())
        {
            if (a == 2)
            {
                if (gBattleTypeFlags & 0x80)
                    LoadCompressedObjectPic(&gUnknown_0820A4AC);
                else
                    LoadCompressedObjectPic(&gUnknown_0820A47C);
            }
            else if (a == 3)
                LoadCompressedObjectPic(&gUnknown_0820A484);
            else if (a == 4)
                LoadCompressedObjectPic(&gUnknown_0820A4B4[gUnknown_02024A72[0]]);
            else if (a == 5)
                LoadCompressedObjectPic(&gUnknown_0820A4B4[gUnknown_02024A72[1]]);
            else
                retVal = TRUE;
        }
        else
        {
            if (a == 2)
                LoadCompressedObjectPic(&gUnknown_0820A48C[0]);
            else if (a == 3)
                LoadCompressedObjectPic(&gUnknown_0820A48C[1]);
            else if (a == 4)
                LoadCompressedObjectPic(&gUnknown_0820A49C[0]);
            else if (a == 5)
                LoadCompressedObjectPic(&gUnknown_0820A49C[1]);
            else if (a == 6)
                LoadCompressedObjectPic(&gUnknown_0820A4B4[gUnknown_02024A72[0]]);
            else if (a == 7)
                LoadCompressedObjectPic(&gUnknown_0820A4B4[gUnknown_02024A72[1]]);
            else if (a == 8)
                LoadCompressedObjectPic(&gUnknown_0820A4B4[gUnknown_02024A72[2]]);
            else if (a == 9)
                LoadCompressedObjectPic(&gUnknown_0820A4B4[gUnknown_02024A72[3]]);
            else
                retVal = TRUE;
        }
    }
    return retVal;
}

void load_gfxc_health_bar(void)
{
    sub_800D238(gUnknown_08D09C48, (void *)0x02000000);
}

u8 battle_load_something(u8 *pState, u8 *b)
{
    bool8 retVal = FALSE;

    switch (*pState)
    {
    case 0:
        sub_8031F0C();
        (*pState)++;
        break;
    case 1:
        if (sub_8031C30(*b) == 0)
        {
            (*b)++;
        }
        else
        {
            *b = 0;
            (*pState)++;
        }
        break;
    case 2:
        (*pState)++;
        break;
    case 3:
        if ((gBattleTypeFlags & 0x80) && *b == 0)
            gUnknown_03004340[*b] = battle_make_oam_safari_battle();
        else
            gUnknown_03004340[*b] = battle_make_oam_normal_battle(*b);
        (*b)++;
        if (*b == gUnknown_02024A68)
        {
            *b = 0;
            (*pState)++;
        }
        break;
    case 4:
        sub_8043F44(*b);
        if (gUnknown_02024A72[*b] <= 1)
            nullsub_11(gUnknown_03004340[*b], 0);
        else
            nullsub_11(gUnknown_03004340[*b], 1);
        (*b)++;
        if (*b == gUnknown_02024A68)
        {
            *b = 0;
            (*pState)++;
        }
        break;
    case 5:
        if (battle_side_get_owner(*b) == 0)
        {
            if (!(gBattleTypeFlags & 0x80))
                sub_8045A5C(gUnknown_03004340[*b], &gPlayerParty[gUnknown_02024A6A[*b]], 0);
        }
        else
        {
            sub_8045A5C(gUnknown_03004340[*b], &gEnemyParty[gUnknown_02024A6A[*b]], 0);
        }
        sub_8043DB0(gUnknown_03004340[*b]);
        (*b)++;
        if (*b == gUnknown_02024A68)
        {
            *b = 0;
            (*pState)++;
        }
        break;
    case 6:
        sub_80327CC();
        sub_8094958();
        retVal = TRUE;
        break;
    }
    return retVal;
}

void sub_8031EE8(void)
{
    memset(ewram17810, 0, 0x30);
    memset(&ewram17840, 0, 0x10);
}

void sub_8031F0C(void)
{
    sub_8031EE8();
    memset(ewram17800, 0, 0x10);
}

void sub_8031F24(void)
{
    s32 i;

    for (i = 0; i < gUnknown_02024A68; i++)
        ewram17800[i].unk0_0 = gSprites[gUnknown_02024BE0[i]].invisible;
}

void sub_8031F88(u8 a)
{
    ewram17800[a].unk0_0 = gSprites[gUnknown_02024BE0[a]].invisible;
}

void sub_8031FC4(u8 a, u8 b, bool8 c)
{
    u16 paletteOffset;
    u16 species;
    u32 personalityValue;
    u32 otId;
    u8 r10;
    const u16 *palette;

    if (c)
    {
        StartSpriteAnim(&gSprites[gUnknown_02024BE0[a]], ewram17840.unk0);
        paletteOffset = 0x100 + a * 16;
        LoadPalette(ewram + 0x16400 + ewram17840.unk0 * 32, paletteOffset, 32);
        gBattleMonForms[a] = ewram17840.unk0;
        if (ewram17800[a].unk2 != 0)
        {
            BlendPalette(paletteOffset, 16, 6, 0x7FFF);
            CpuCopy32(gPlttBufferFaded + paletteOffset, gPlttBufferUnfaded + paletteOffset, 32);
        }
        gSprites[gUnknown_02024BE0[a]].pos1.y = sub_8077F68(a);
    }
    else
    {
        const void *src;
        void *dst;

        if (IsContest())
        {
            r10 = 0;
            species = ewram19348.unk2;
            personalityValue = ewram19348.unk8;
            otId = ewram19348.unkC;
            HandleLoadSpecialPokePic(
              &gMonBackPicTable[species],
              gMonBackPicCoords[species].coords,
              gMonBackPicCoords[species].y_offset,
              0x02000000,
              gUnknown_081FAF4C[0],
              species,
              ewram19348.unk10);
        }
        else
        {
            r10 = battle_get_per_side_status(a);
            if (battle_side_get_owner(b) == 1)
                species = GetMonData(&gEnemyParty[gUnknown_02024A6A[b]], MON_DATA_SPECIES);
            else
                species = GetMonData(&gPlayerParty[gUnknown_02024A6A[b]], MON_DATA_SPECIES);
            if (battle_side_get_owner(a) == 0)
            {
                personalityValue = GetMonData(&gPlayerParty[gUnknown_02024A6A[a]], MON_DATA_PERSONALITY);
                otId = GetMonData(&gPlayerParty[gUnknown_02024A6A[a]], MON_DATA_OT_ID);
                HandleLoadSpecialPokePic(
                  &gMonBackPicTable[species],
                  gMonBackPicCoords[species].coords,
                  gMonBackPicCoords[species].y_offset,
                  0x02000000,
                  gUnknown_081FAF4C[r10],
                  species,
                  gUnknown_02024E70[a]);
            }
            else
            {
                personalityValue = GetMonData(&gEnemyParty[gUnknown_02024A6A[a]], MON_DATA_PERSONALITY);
                otId = GetMonData(&gEnemyParty[gUnknown_02024A6A[a]], MON_DATA_OT_ID);
                HandleLoadSpecialPokePic(
                  &gMonFrontPicTable[species],
                  gMonFrontPicCoords[species].coords,
                  gMonFrontPicCoords[species].y_offset,
                  0x02000000,
                  gUnknown_081FAF4C[r10],
                  species,
                  gUnknown_02024E70[a]);
            }
        }
        src = gUnknown_081FAF4C[r10];
        dst = (void *)(VRAM + 0x10000 + gSprites[gUnknown_02024BE0[a]].oam.tileNum * 32);
        DmaCopy32(3, src, dst, 0x800);
        paletteOffset = 0x100 + a * 16;
        palette = species_and_otid_get_pal(species, otId, personalityValue);
        sub_800D238(palette, ewram);
        LoadPalette(ewram, paletteOffset, 32);
        if (species == SPECIES_CASTFORM)
        {
            u16 *paletteSrc = (u16 *)(ewram + 0x16400);

            sub_800D238(palette, paletteSrc);
            LoadPalette(paletteSrc + gBattleMonForms[b] * 16, paletteOffset, 32);
        }
        BlendPalette(paletteOffset, 16, 6, 0x7FFF);
        CpuCopy32(gPlttBufferFaded + paletteOffset, gPlttBufferUnfaded + paletteOffset, 32);
        if (!IsContest())
        {
            ewram17800[a].unk2 = species;
            gBattleMonForms[a] = gBattleMonForms[b];
        }
        gSprites[gUnknown_02024BE0[a]].pos1.y = sub_8077F68(a);
        StartSpriteAnim(&gSprites[gUnknown_02024BE0[a]], gBattleMonForms[a]);
    }
}

void sub_8032350(u8 a, u8 b)
{
    u8 r4;
    u16 foo;
    const u8 *gSubstituteDollPal_;
    void *src;
    s32 i;
    
    if (b == 0)
    {
        if (IsContest())
            r4 = 0;
        else
            r4 = battle_get_per_side_status(a);
        if (IsContest())
            LZDecompressVram(gSubstituteDollTilemap, gUnknown_081FAF4C[r4]);
        else if (battle_side_get_owner(a) != 0)
            LZDecompressVram(gSubstituteDollGfx, gUnknown_081FAF4C[r4]);
        else
            LZDecompressVram(gSubstituteDollTilemap, gUnknown_081FAF4C[r4]);
        // There is probably a way to do this without all the temp variables, but I couldn't figure it out.
        foo = a * 16;
        gSubstituteDollPal_ = gSubstituteDollPal;
        src = gUnknown_081FAF4C[r4];
        for (i = 0; i < 3; i++)
            DmaCopy32(3, src, src + i * 0x800 + 0x800, 0x800);
        LoadCompressedPalette(gSubstituteDollPal_, 0x100 + foo, 32);
    }
    else
    {
        if (!IsContest())
        {
            if (battle_side_get_owner(a) != 0)
                sub_8031794(&gEnemyParty[gUnknown_02024A6A[a]], a);
            else
                sub_80318FC(&gPlayerParty[gUnknown_02024A6A[a]], a);
        }
    }
}

void refresh_graphics_maybe(u8 a, u8 b, u8 spriteId)
{
    sub_8032350(a, b);
    StartSpriteAnim(&gSprites[spriteId], gBattleMonForms[a]);
    if (b == 0)
        gSprites[spriteId].pos1.y = sub_8077F7C(a);
    else
        gSprites[spriteId].pos1.y = sub_8077F68(a);
}

void sub_80324BC(u8 a, u16 b)
{
    if (b == 0xA4)
        ewram17800[a].unk0_2 = 1;
}

void sub_80324E0(u8 a)
{
    ewram17800[a].unk0_2 = 0;
}

void sub_80324F8(struct Pokemon *pkmn, u8 b)
{
    u16 hp = GetMonData(pkmn, MON_DATA_HP);
    u16 maxHP = GetMonData(pkmn, MON_DATA_MAX_HP);
    
    if (GetHPBarLevel(hp, maxHP) == 1)
    {
        if (!ewram17800[b].unk0_1)
        {
            if (!ewram17800[b ^ 2].unk0_1)
                PlaySE(SE_HINSI);
            ewram17800[b].unk0_1 = 1;
        }
    }
    else
    {
        ewram17800[b].unk0_1 = 0;
        if (!IsDoubleBattle())
        {
            m4aSongNumStop(SE_HINSI);
            return;
        }
        if (IsDoubleBattle() && !ewram17800[b ^ 2].unk0_1)
        {
            m4aSongNumStop(SE_HINSI);
            return;
        }
    }
}

void sub_80325B8(void)
{
    u8 r4 = battle_get_side_with_given_state(0);
    
    ewram17800[r4].unk0_1 = 0;
    if (IsDoubleBattle())
        ewram17800[r4 ^ 2].unk0_1 = 0;
    m4aSongNumStop(SE_HINSI);
}

u8 unref_sub_8032604(struct Pokemon *pkmn)
{
    u16 hp = GetMonData(pkmn, MON_DATA_HP);
    u16 maxHP = GetMonData(pkmn, MON_DATA_MAX_HP);
    
    return GetHPBarLevel(hp, maxHP);
}

void sub_8032638(void)
{
    if (gMain.inBattle)
    {
        u8 r8 = battle_get_side_with_given_state(0);
        u8 r9 = battle_get_side_with_given_state(2);
        u8 r4 = pokemon_order_func(gUnknown_02024A6A[r8]);
        u8 r5 = pokemon_order_func(gUnknown_02024A6A[r9]);
        
        if (GetMonData(&gPlayerParty[r4], MON_DATA_HP) != 0)
            sub_80324F8(&gPlayerParty[r4], r8);
        if (IsDoubleBattle())
        {
            if (GetMonData(&gPlayerParty[r5], MON_DATA_HP) != 0)
                sub_80324F8(&gPlayerParty[r5], r9);
        }
    }
}

void sub_80326EC(u8 a)
{
    s32 i;
    
    for (i = 0; i < gUnknown_02024A68; i++)
    {
        if (sub_8078874(i) != 0)
        {
            gSprites[gUnknown_02024BE0[i]].oam.affineMode = a;
            if (a == 0)
            {
                ewram17810[i].unk6 = gSprites[gUnknown_02024BE0[i]].oam.matrixNum;
                gSprites[gUnknown_02024BE0[i]].oam.matrixNum = 0;
            }
            else
            {
                gSprites[gUnknown_02024BE0[i]].oam.matrixNum = ewram17810[i].unk6;
            }
        }
    }
}
