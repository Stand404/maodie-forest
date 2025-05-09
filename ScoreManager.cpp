#include "ScoreManager.h"
#include <random>

const TCHAR* ScoreManager::FILE_NAME = _T("score.bin"); // 二进制文件扩展名

// 加密函数（仅XOR）
void ScoreManager::Encrypt(int& score) {
    const uint32_t key = 0x55AAFF00;
    uint8_t* bytes = reinterpret_cast<uint8_t*>(&score);

    // XOR加密（按字节）
    for (int i = 0; i < sizeof(int); i++) {
        bytes[i] ^= ((key >> (8 * i)) & 0xFF);
    }
}

// 解密函数（与加密完全相同，因为XOR是可逆的）
void ScoreManager::Decrypt(int& score) {
    Encrypt(score); // XOR加密和解密是相同的操作
}

// 写入分数（二进制加密）
void ScoreManager::WriteScore(int score) {
    Encrypt(score); // 先加密

    FILE* file = nullptr;
    if (_tfopen_s(&file, FILE_NAME, _T("wb")) == 0 && file) {
        fwrite(&score, sizeof(int), 1, file); // 二进制写入
        fclose(file);
    }
}

// 读取分数（二进制解密）
int ScoreManager::ReadScore() {
    FILE* file = nullptr;
    if (_tfopen_s(&file, FILE_NAME, _T("rb")) != 0 || !file) {
        return 0; // 默认分数
    }

    int encryptedScore = 0;
    if (fread(&encryptedScore, sizeof(int), 1, file) != 1) {
        fclose(file);
        return 0;
    }

    fclose(file);
    Decrypt(encryptedScore); // 解密
    return encryptedScore;
}