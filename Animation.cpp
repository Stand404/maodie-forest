#include "Animation.h"

Animation::Animation(Atlas* atlas, int interval)
    : anim_atlas(atlas), interval_ms(interval) {}

void Animation::Play(int x, int y, int delta) {
    timer += delta;
    if (timer >= interval_ms) {
        idx_frame = (idx_frame + 1) % anim_atlas->frame_list.size();
        timer = 0;
    }
    putimage_alpha(x, y, anim_atlas->frame_list[idx_frame]);
}