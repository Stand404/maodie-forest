// Atlas.cpp
#include "Atlas.h"

Atlas::Atlas(LPCTSTR path, int num, int width, int height) {
    TCHAR path_file[256];
    for (int i = 0; i < num; i++) {
        _stprintf_s(path_file, path, i + 1);
        IMAGE* frame = new IMAGE();
        loadimage(frame, path_file, width, height);
        frame_list.push_back(frame);
    }
}

Atlas::~Atlas() {
    for (auto frame : frame_list) {
        delete frame;
    }
    frame_list.clear();
}