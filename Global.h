// Global.h
#pragma once
#include <vector>
#include "Atlas.h"
#include <graphics.h>
#include "Animation.h"
#include "GraphicsUtil.h"

// 游戏窗口和地图尺寸
const int WINDOW_WIDTH = 1280;
const int WINDOW_HEIGHT = 720;
const int BUTTON_WIDTH = 155;
const int BUTTON_HEIGHT = 44;

const int EXP_BAR_WIDTH = 400; // 进度条宽度
const int EXP_BAR_HEIGHT = 20; // 进度条高度
const int EXP_BAR_Y = WINDOW_HEIGHT - 50; // 进度条距离窗口底部的距离

struct MapAreaStand {
    int x1, y1, x2, y2;
    MapAreaStand(int x1, int y1, int x2, int y2) : x1(x1), y1(y1), x2(x2), y2(y2) {}
};
extern MapAreaStand MAP_AREA;

// 全局Atlas资源
extern Atlas* atlas_player_left;
extern Atlas* atlas_player_right;
extern Atlas* atlas_enemy_left;
extern Atlas* atlas_enemy_right;
extern Atlas* atlas_enemy1_left;
extern Atlas* atlas_enemy1_right;
extern Atlas* atlas_enemy2_left;
extern Atlas* atlas_enemy2_right;
extern Atlas* atlas_enemy3_left;
extern Atlas* atlas_enemy3_right;
extern Atlas* atlas_enemy4_left;
extern Atlas* atlas_enemy4_right;
extern Atlas* atlas_enemy5_left;
extern Atlas* atlas_enemy5_right;
extern Atlas* atlas_enemy6_left;
extern Atlas* atlas_enemy6_right;

extern Atlas* atlas_main;
extern Animation* main_anim;

extern IMAGE img_bullet;
extern IMAGE img_loading;
extern IMAGE img_bg;
extern IMAGE img_title;

void InitGameResources();
void CleanupGameResources();

namespace UIHelper {
    // 按钮常量配置
    constexpr int BUTTON_WIDTH = 200;
    constexpr int BUTTON_HEIGHT = 60;
    constexpr int BUTTON_SPACING = 30;  // 按钮间垂直间距
    constexpr int FIRST_BUTTON_TOP = 400;  // 第一个按钮的Y坐标

    // 计算按钮矩形区域
    inline RECT CalculateButtonRect(int buttonIndex) {
        return {
            (WINDOW_WIDTH - BUTTON_WIDTH) / 2,                     // left
            FIRST_BUTTON_TOP + buttonIndex * (BUTTON_HEIGHT + BUTTON_SPACING), // top
            (WINDOW_WIDTH + BUTTON_WIDTH) / 2,                      // right
            FIRST_BUTTON_TOP + buttonIndex * (BUTTON_HEIGHT + BUTTON_SPACING) + BUTTON_HEIGHT // bottom
        };
    }
}