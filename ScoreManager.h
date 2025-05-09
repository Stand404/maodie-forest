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
    static void Encrypt(int& score);  // ���ܺ���
    static void Decrypt(int& score);  // ���ܺ���������ܲ�ͬ��
    static const TCHAR* FILE_NAME;
};