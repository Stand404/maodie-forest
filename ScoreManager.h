#pragma once
#include <windows.h>
#include <tchar.h>
#include <cstdio>

class ScoreManager
{
public:
    static int ReadScore();
    static void WriteScore(int score);

private:
    static void Encrypt(int& score);  // 加密函数
    static void Decrypt(int& score);  // 解密函数（与加密不同）
    static const TCHAR* FILE_NAME;
};