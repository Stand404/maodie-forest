// EnemyConfig.cpp
#include "EnemyConfig.h"
#include "Global.h"  // 包含所有Atlas指针的声明

std::map<EnemyType, EnemyConfig> enemy_configs;

void InitEnemyConfigs() {
    // 初始化敌人配置
    enemy_configs[EnemyType::Normal] = {
        5, 50, 50, 40,
        atlas_enemy_left, atlas_enemy_right,
        _T("resources/shadow.png")
    };

    enemy_configs[EnemyType::Type1] = {
        2, 50, 50, 40,
        atlas_enemy1_left, atlas_enemy1_right,
        _T("resources/shadow.png")
    };

    enemy_configs[EnemyType::Type2] = {
        2, 50, 50, 40,
        atlas_enemy2_left, atlas_enemy2_right,
        _T("resources/shadow.png")
    };

    enemy_configs[EnemyType::Type3] = {
        2, 50, 50, 40,
        atlas_enemy3_left, atlas_enemy3_right,
        _T("resources/shadow.png")
    };

    enemy_configs[EnemyType::Type4] = {
        2, 50, 50, 40,
        atlas_enemy4_left, atlas_enemy4_right,
        _T("resources/shadow.png")
    };

    enemy_configs[EnemyType::Type5] = {
        4, 50, 50, 40,
        atlas_enemy5_left, atlas_enemy5_right,
        _T("resources/shadow.png")
    };

    enemy_configs[EnemyType::Type6] = {
        6, 100, 50, 60,
        atlas_enemy6_left, atlas_enemy6_right,
        _T("resources/shadow.png")
    };
}