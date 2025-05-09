#include "Buttons.h"
#include <mmsystem.h>
#include "GraphicsUtil.h"
#include <string>

Button::Button(RECT rect, LPCTSTR path_folder, ClickCallback onClick)
    : region(rect), onClick(std::move(onClick))
{
    // 拼接文件路径
    std::wstring path_idle = std::wstring(path_folder) + L"idle.png";
    std::wstring path_hovered = std::wstring(path_folder) + L"hovered.png";
    std::wstring path_pushed = std::wstring(path_folder) + L"pushed.png";

    // 加载图片
    loadimage(&img_idle, path_idle.c_str(), rect.right - rect.left, rect.bottom - rect.top);
    loadimage(&img_hovered, path_hovered.c_str(), rect.right - rect.left, rect.bottom - rect.top);
    loadimage(&img_pushed, path_pushed.c_str(), rect.right - rect.left, rect.bottom - rect.top);
}

void Button::Draw() {
    switch (status) {
    case Status::Idle:
        putimage_alpha(region.left, region.top, &img_idle);
        break;
    case Status::Hovered:
        putimage_alpha(region.left, region.top, &img_hovered);
        break;
    case Status::Pushed:
        putimage_alpha(region.left, region.top, &img_pushed);
        break;
    }
}

void Button::ProcessEvent(const ExMessage& msg) {
    switch (msg.message) {
    case WM_MOUSEMOVE:
        if (CheckCursorHit(msg.x, msg.y)) {
            status = (status == Status::Pushed) ? Status::Pushed : Status::Hovered;
        }
        else {
            status = Status::Idle;
        }
        break;

    case WM_LBUTTONDOWN:
        if (CheckCursorHit(msg.x, msg.y)) {
            status = Status::Pushed;
        }
        break;

    case WM_LBUTTONUP:
        if (status == Status::Pushed && CheckCursorHit(msg.x, msg.y)) {
            status = Status::Hovered;
            if (onClick) {
                onClick();
            }
        }
        break;
    }
}

void Button::SetCallback(ClickCallback callback) {
    onClick = std::move(callback);
}

bool Button::CheckCursorHit(int x, int y) const {
    return x >= region.left && x <= region.right &&
        y >= region.top && y <= region.bottom;
}

// 创建特定功能的按钮
Button* CreateStartButton(RECT rect, std::function<void()> onClick) {
    return new Button(
        rect,
        L"resources/start_btn/",
        std::move(onClick)
    );
}

Button* CreateQuitButton(RECT rect, std::function<void()> onClick) {
    return new Button(
        rect,
        L"resources/quit_btn/",
        std::move(onClick)
    );
}