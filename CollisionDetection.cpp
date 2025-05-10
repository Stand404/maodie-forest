#include "CollisionDetection.h"
#include <cmath>
#include <algorithm>

// 计算点绕某点旋转后的坐标
Point RotatePoint(const Point& point, const Point& center, double angle) {
    double cos_angle = cos(angle);
    double sin_angle = sin(angle);
    Point rotated;
    rotated.x = center.x + (point.x - center.x) * cos_angle - (point.y - center.y) * sin_angle;
    rotated.y = center.y + (point.x - center.x) * sin_angle + (point.y - center.y) * cos_angle;
    return rotated;
}

// 获取碰撞体的四个顶点
std::array<Point, 4> GetColliderVertices(const ColliderBox& collider) {
    Point center = { (collider.x1 + collider.x2) / 2, (collider.y1 + collider.y2) / 2 };
    int width = collider.x2 - collider.x1;
    int height = collider.y2 - collider.y1;

    std::array<Point, 4> vertices = {
        Point{center.x - width / 2, center.y - height / 2}, // 左上角
        Point{center.x + width / 2, center.y - height / 2}, // 右上角
        Point{center.x + width / 2, center.y + height / 2}, // 右下角
        Point{center.x - width / 2, center.y + height / 2}  // 左下角
    };

    for (auto& vertex : vertices) {
        vertex = RotatePoint(vertex, center, collider.rotation_angle);
    }
    return vertices;
}

// 计算投影的最小值和最大值
std::pair<double, double> ProjectCollider(const std::array<Point, 4>& vertices, const Point& axis) {
    double min = vertices[0].x * axis.x + vertices[0].y * axis.y;
    double max = min;
    for (const auto& vertex : vertices) {
        double projection = vertex.x * axis.x + vertex.y * axis.y;
        min = std::min(min, projection);
        max = std::max(max, projection);
    }
    return { min, max };
}

bool CheckCollision(const ColliderBox& collider1, const ColliderBox& collider2) {
    auto vertices1 = GetColliderVertices(collider1);
    auto vertices2 = GetColliderVertices(collider2);

    std::array<Point, 4> axes = {
        Point{1, 0}, Point{0, 1},
        Point{static_cast<int>(cos(collider1.rotation_angle)), static_cast<int>(sin(collider1.rotation_angle))},
        Point{static_cast<int>(-sin(collider1.rotation_angle)), static_cast<int>(cos(collider1.rotation_angle))}
    };

    for (const auto& axis : axes) {
        std::pair<double, double> projection1 = ProjectCollider(vertices1, axis);
        std::pair<double, double> projection2 = ProjectCollider(vertices2, axis);

        double min1 = projection1.first;
        double max1 = projection1.second;
        double min2 = projection2.first;
        double max2 = projection2.second;

        if (max1 < min2 || max2 < min1) {
            return false; // 找到分离轴，不相交
        }
    }

    // 交换碰撞体，检查碰撞体2的法线方向
    axes = {
        Point{1, 0}, Point{0, 1}, // 碰撞体2的法线方向
        Point{static_cast<int>(cos(collider2.rotation_angle)), static_cast<int>(sin(collider2.rotation_angle))}, // 碰撞体2的旋转边方向
        Point{static_cast<int>(-sin(collider2.rotation_angle)), static_cast<int>(cos(collider2.rotation_angle))}
    };

    for (const auto& axis : axes) {
        std::pair<double, double> projection1 = ProjectCollider(vertices1, axis);
        std::pair<double, double> projection2 = ProjectCollider(vertices2, axis);

        double min1 = projection1.first;
        double max1 = projection1.second;
        double min2 = projection2.first;
        double max2 = projection2.second;

        if (max1 < min2 || max2 < min1) {
            return false; // 找到分离轴，不相交
        }
    }

    return true; // 没有找到分离轴，相交
}