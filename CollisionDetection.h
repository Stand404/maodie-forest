#ifndef COLLISIONDETECTION_H
#define COLLISIONDETECTION_H

#include <array>
#include <utility>

struct Point {
    int x, y;
};

struct ColliderBox {
    int x1, y1, x2, y2; // ���ϽǺ����½ǵ�����
    double rotation_angle; // ��ת�Ƕȣ����ȣ�
};

bool CheckCollision(const ColliderBox& collider1, const ColliderBox& collider2);

#endif // COLLISIONDETECTION_H
