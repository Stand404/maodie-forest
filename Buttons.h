#pragma once
#include <graphics.h>
#include <functional>
#include <memory>

class Button {
public:
    using ClickCallback = std::function<void()>;

    Button(RECT rect, LPCTSTR path_folder, ClickCallback onClick = nullptr);

    virtual ~Button() = default;

    void Draw();
    void ProcessEvent(const ExMessage& msg);
    void SetCallback(ClickCallback callback);

private:
    enum class Status {
        Idle = 0, Hovered, Pushed
    };

    bool CheckCursorHit(int x, int y) const;

    RECT region;
    IMAGE img_idle;
    IMAGE img_hovered;
    IMAGE img_pushed;
    Status status = Status::Idle;
    ClickCallback onClick;
};

// 按钮创建辅助函数
Button* CreateStartButton(RECT rect, std::function<void()> onClick);
Button* CreateQuitButton(RECT rect, std::function<void()> onClick);