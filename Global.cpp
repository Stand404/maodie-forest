// Global.cpp
#include "Global.h"
#include "Atlas.h"

// Atlas��Դָ��
Atlas* atlas_player_left;
Atlas* atlas_player_right;
Atlas* atlas_enemy_left;
Atlas* atlas_enemy_right;
Atlas* atlas_enemy1_left;
Atlas* atlas_enemy1_right;
Atlas* atlas_enemy2_left;
Atlas* atlas_enemy2_right;
Atlas* atlas_enemy3_left;
Atlas* atlas_enemy3_right;
Atlas* atlas_enemy4_left;
Atlas* atlas_enemy4_right;
Atlas* atlas_enemy5_left;
Atlas* atlas_enemy5_right;
Atlas* atlas_enemy6_left;
Atlas* atlas_enemy6_right;

Atlas* atlas_main;
Animation* main_anim;

IMAGE img_bullet;  
IMAGE img_loading;
IMAGE img_bg;
IMAGE img_title;

MapAreaStand MAP_AREA = { 180, 120, 1100, 580 };

void InitGameResources() {
    // ��ʼ��Atlas��Դ
    atlas_player_left = new Atlas(_T("resources/����/left_%d.png"), 13, 60, 60);
    atlas_player_right = new Atlas(_T("resources/����/right_%d.png"), 13, 60, 60);
    atlas_enemy_left = new Atlas(_T("resources/�ﳵ���/left_%d.png"), 4, 50, 50);
    atlas_enemy_right = new Atlas(_T("resources/�ﳵ���/right_%d.png"), 4, 50, 50);
    atlas_enemy1_left = new Atlas(_T("resources/���1/left_%d.png"), 4, 50, 50);
    atlas_enemy1_right = new Atlas(_T("resources/���1/right_%d.png"), 4, 50, 50);
    atlas_enemy2_left = new Atlas(_T("resources/���2/left_%d.png"), 4, 50, 50);
    atlas_enemy2_right = new Atlas(_T("resources/���2/right_%d.png"), 4, 50, 50);
    atlas_enemy3_left = new Atlas(_T("resources/���3/left_%d.png"), 4, 50, 50);
    atlas_enemy3_right = new Atlas(_T("resources/���3/right_%d.png"), 4, 50, 50);
    atlas_enemy4_left = new Atlas(_T("resources/���4/left_%d.png"), 4, 50, 50);
    atlas_enemy4_right = new Atlas(_T("resources/���4/right_%d.png"), 4, 50, 50);
    atlas_enemy5_left = new Atlas(_T("resources/�������/left_%d.png"), 4, 50, 50);
    atlas_enemy5_right = new Atlas(_T("resources/�������/right_%d.png"), 4, 50, 50);
    atlas_enemy6_left = new Atlas(_T("resources/֩�����_��/%d.png"), 14, 100, 50);
    atlas_enemy6_right = new Atlas(_T("resources/֩�����_��/%d.png"), 14, 100, 50);

    loadimage(&img_bullet, _T("resources/צ��.png"), 20, 40);
    loadimage(&img_bg, _T("resources/bg.jpg"));
    loadimage(&img_title, _T("resources/title.png"), 300, 150);

    atlas_main = new Atlas(_T("resources/start/(%d).jpg"), 54, WINDOW_WIDTH, WINDOW_HEIGHT);
    main_anim = new Animation(atlas_main, 45);
}

void CleanupGameResources() {
    // ����Atlas��Դ
    delete atlas_player_left;
    delete atlas_player_right;
    delete atlas_enemy_left;
    delete atlas_enemy_right;
    delete atlas_enemy1_left;
    delete atlas_enemy1_right;
    delete atlas_enemy2_left;
    delete atlas_enemy2_right;
    delete atlas_enemy3_left;
    delete atlas_enemy3_right;
    delete atlas_enemy4_left;
    delete atlas_enemy4_right;
    delete atlas_enemy5_left;
    delete atlas_enemy5_right;
    delete atlas_enemy6_right;
    delete atlas_enemy6_left;
    delete atlas_main;
    delete main_anim;
}