#include "ScoreManager.h"
#include <random>

const TCHAR* ScoreManager::FILE_NAME = _T("score.bin"); // �������ļ���չ��

// ���ܺ�������XOR��
void ScoreManager::Encrypt(int& score) {
    const uint32_t key = 0x55AAFF00;
    uint8_t* bytes = reinterpret_cast<uint8_t*>(&score);

    // XOR���ܣ����ֽڣ�
    for (int i = 0; i < sizeof(int); i++) {
        bytes[i] ^= ((key >> (8 * i)) & 0xFF);
    }
}

// ���ܺ������������ȫ��ͬ����ΪXOR�ǿ���ģ�
void ScoreManager::Decrypt(int& score) {
    Encrypt(score); // XOR���ܺͽ�������ͬ�Ĳ���
}

// д������������Ƽ��ܣ�
void ScoreManager::WriteScore(int score) {
    Encrypt(score); // �ȼ���

    FILE* file = nullptr;
    if (_tfopen_s(&file, FILE_NAME, _T("wb")) == 0 && file) {
        fwrite(&score, sizeof(int), 1, file); // ������д��
        fclose(file);
    }
}

// ��ȡ�����������ƽ��ܣ�
int ScoreManager::ReadScore() {
    FILE* file = nullptr;
    if (_tfopen_s(&file, FILE_NAME, _T("rb")) != 0 || !file) {
        return 0; // Ĭ�Ϸ���
    }

    int encryptedScore = 0;
    if (fread(&encryptedScore, sizeof(int), 1, file) != 1) {
        fclose(file);
        return 0;
    }

    fclose(file);
    Decrypt(encryptedScore); // ����
    return encryptedScore;
}