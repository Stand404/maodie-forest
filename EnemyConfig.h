// EnemyConfig.h
#pragma once
#include "Atlas.h"
#include <map>

enum class EnemyType {
    Normal,
    Type1,  // лЈсу1
    Type2,  // лЈсу2
    Type3,  // лЈсу3
    Type4,
    Type5,
    Type6
};

struct EnemyConfig {
    int speed;
    int width;
    int height;
    int shadow_width;
    Atlas* atlas_left;
    Atlas* atlas_right;
    LPCTSTR shadow_path;
};

extern std::map<EnemyType, EnemyConfig> enemy_configs;

void InitEnemyConfigs();