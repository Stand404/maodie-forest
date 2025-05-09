#pragma once
#include "GraphicsUtil.h"  // 依赖透明绘制函数
#include "Atlas.h"         // 依赖 Atlas 类

class Animation {
public:
    Animation(Atlas* atlas, int interval);
    ~Animation() = default;

    void Play(int x, int y, int delta);

private:
    int timer = 0;        // 动画计时器
    int idx_frame = 0;    // 当前帧索引
    int interval_ms = 0;  // 帧间隔（毫秒）
    Atlas* anim_atlas;    // 动画图集
};