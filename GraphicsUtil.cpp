#include "GraphicsUtil.h"
#include <Windows.h>

#pragma comment(lib, "MSIMG32.LIB")  // AlphaBlend 所需库

// 实现透明绘制
void putimage_alpha(int x, int y, IMAGE* img) {
    int w = img->getwidth();
    int h = img->getheight();
    AlphaBlend(
        GetImageHDC(NULL), x, y, w, h,
        GetImageHDC(img), 0, 0, w, h,
        { AC_SRC_OVER, 0, 255, AC_SRC_ALPHA }
    );
}