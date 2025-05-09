// Atlas.h
#pragma once
#include <graphics.h>
#include <vector>

class Atlas {
public:
    Atlas(LPCTSTR path, int num, int width, int height);
    ~Atlas();

    std::vector<IMAGE*> frame_list;
}; 
