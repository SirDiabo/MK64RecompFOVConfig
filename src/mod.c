#include "modding.h"
#include "recompconfig.h"
#include "recomputils.h"

#include "camera.h"
#include "defines.h"

#define UNUSED

#define DEFAULT_FOV recomp_get_config_u32("default_fov")
#define HOLD_BANANA_FOV recomp_get_config_u32("banana_fov")
#define BOOST_FOV recomp_get_config_u32("boost_fov")
#define BOOST_RAMP_FOV recomp_get_config_u32("ramp_fov")
#define FLATTEN_FOV recomp_get_config_u32("flat_fov")
#define SPINOUT_FOV recomp_get_config_u32("spinout_fov")
#define START_FOV recomp_get_config_u32("start_fov")

extern f32 D_80164498[4];
extern s32 gActiveScreenMode = SCREEN_MODE_1P;

extern Player gPlayers[NUM_PLAYERS];
extern Player* gPlayerOne[];
extern Player* gPlayerTwo[];
extern Player* gPlayerThree[];
extern Player* gPlayerFour[];

extern f32 func_80014EE4(f32 arg0, s32 arg1);
extern void move_f32_towards(f32*, f32, f32);

RECOMP_PATCH void func_8001F394(Player* player, f32* arg1) {
    f32 var_f0;
    UNUSED s32 pad;
    s32 playerIndex;
    UNUSED s32 pad2;
    Camera* camera = &cameras[0];

    if (player == gPlayerOne) {
        playerIndex = 0;
    }
    if (player == gPlayerTwo) {
        playerIndex = 1;
    }
    if (player == gPlayerThree) {
        playerIndex = 2;
    }
    if (player == gPlayerFour) {
        playerIndex = 3;
    }

    if (D_80164A08[playerIndex] == 0) {
        if (player->soundEffects & HOLD_BANANA_SOUND_EFFECT) {
            D_80164A08[playerIndex] = 1;
        }
        if ((player->effects & BOOST_EFFECT) == BOOST_EFFECT) {
            D_80164A08[playerIndex] = 2;
        }
        if ((player->effects & BOOST_RAMP_ASPHALT_EFFECT) == BOOST_RAMP_ASPHALT_EFFECT) {
            D_80164A08[playerIndex] = 3;
        }
        if ((player->soundEffects & 0x100) == 0x100) {
            D_80164A08[playerIndex] = 4;
        }
        if (((player->effects & 0x80) == 0x80) || ((player->effects & 0x40) == 0x40)) {
            D_80164A08[playerIndex] = 5;
        }
        D_80164498[playerIndex] = DEFAULT_FOV;
    }
    switch (D_80164A08[playerIndex]) {
    case 1:
        if (player->soundEffects & HOLD_BANANA_SOUND_EFFECT) {
            move_f32_towards(&D_80164498[playerIndex], HOLD_BANANA_FOV, 0.2f);
        }
        else {
            if (D_80164498[playerIndex] > 1.0f) {
                D_80164498[playerIndex] -= 1.0f;
            }
            else {
                D_80164A08[playerIndex] = 0;
                D_80164498[playerIndex] = 0.0f;
            }
        }
        break;
    case 2:
        if ((player->effects & BOOST_EFFECT) == BOOST_EFFECT) {
            if (player->boostTimer != 0) {
                move_f32_towards(&D_80164498[playerIndex], BOOST_FOV, 0.2f);
            }
        }
        else {
            if (D_80164498[playerIndex] > 1.0f) {
                D_80164498[playerIndex] -= 2.0f;
            }
            else {
                D_80164A08[playerIndex] = 0;
                D_80164498[playerIndex] = 0.0f;
            }
        }
        break;
    case 3:
        if (((player->effects & BOOST_RAMP_ASPHALT_EFFECT) == BOOST_RAMP_ASPHALT_EFFECT) &&
            ((player->effects & 8) == 8)) {
            move_f32_towards(&D_80164498[playerIndex], BOOST_RAMP_FOV, 0.1f);
        }
        else {
            if (D_80164498[playerIndex] > 1.0f) {
                D_80164498[playerIndex] -= 1.0f;
            }
            else {
                D_80164A08[playerIndex] = 0;
                D_80164498[playerIndex] = 0.0f;
            }
        }
        break;
    case 4:
        if ((player->soundEffects & 0x100) == 0x100) {
            move_f32_towards(&D_80164498[playerIndex], FLATTEN_FOV, 1.0f);
        }
        else {
            if (D_80164498[playerIndex] > 1.0f) {
                D_80164498[playerIndex] -= 2.0f;
            }
            else {
                D_80164A08[playerIndex] = 0;
                D_80164498[playerIndex] = 0.0f;
            }
        }
        break;
    case 5:
        if (((player->effects & 0x80) == 0x80) || ((player->effects & 0x40) == 0x40)) {
            move_f32_towards(&D_80164498[playerIndex], SPINOUT_FOV, 0.2f);
        }
        else {
            if (D_80164498[playerIndex] > 1.0f) {
                D_80164498[playerIndex] -= 2.0f;
            }
            else {
                D_80164A08[playerIndex] = 0;
                D_80164498[playerIndex] = 0.0f;
            }
        }
        break;
    }
    switch (gActiveScreenMode) {
    case SCREEN_MODE_1P:
        if (D_80164A28 == 1) {
            D_80164498[playerIndex] = START_FOV;
        }
        if (D_80164A28 == 2) {
            if (D_80164498[playerIndex] >= 0.0f) {
                D_80164498[playerIndex] -= 0.8;
            }
            if (D_80164498[playerIndex] <= 0.0f) {
                D_80164A28 = 0;
                D_80164498[playerIndex] = 0.0f;
            }
        }
        var_f0 = func_80014EE4(*arg1, playerIndex);
        break;
    case SCREEN_MODE_2P_SPLITSCREEN_HORIZONTAL:
    case SCREEN_MODE_2P_SPLITSCREEN_VERTICAL:
    case SCREEN_MODE_3P_4P_SPLITSCREEN:
        var_f0 = func_80014EE4(*arg1, playerIndex);
        break;
    }
    *arg1 = var_f0;
    camera += playerIndex;
    camera->unk_B4 = var_f0;
}