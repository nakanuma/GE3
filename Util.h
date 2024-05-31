#pragma once
#include <cstdlib>
#include <ctime>

// 整数のランダム数を生成する関数
int RandomNumber(int min, int max) {

    // 指定された範囲内の乱数を生成して返す
    return min + std::rand() % (max - min + 1);
}

// 浮動小数点数のランダム数を生成する関数
float RandomNumber(float min, float max) {

    // 0から1の範囲の乱数を生成して正規化し、指定された範囲内にスケーリングして返す
    return min + static_cast<float>(std::rand()) / static_cast<float>(RAND_MAX) * (max - min);
}

