#ifndef CUBEFUNCTIONS_H
#define CUBEFUNCTIONS_H

#include <vector>

//翻转魔方
void frontL(std::vector<int> &cube);
void frontR(std::vector<int> &cube);
void rightL(std::vector<int> &cube);
void rightR(std::vector<int> &cube);
void topL(std::vector<int> &cube);
void topR(std::vector<int> &cube);

//表征二阶魔方状态的向量最小长度
extern int g_cubeVecLen;

#endif // CUBEFUNCTIONS_H
