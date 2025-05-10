#ifndef COLLISIONDETECTION_H
#define COLLISIONDETECTION_H

#include <array>
#include <utility>

struct Point {
    int x, y;
};

struct ColliderBox {
    int x1, y1, x2, y2; // 左上角和右下角的坐标
    double rotation_angle; // 旋转角度（弧度）
};

bool CheckCollision(const ColliderBox& collider1, const ColliderBox& collider2);

#endif // COLLISIONDETECTION_H
