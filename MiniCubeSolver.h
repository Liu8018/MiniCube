#ifndef MINICUBESOLVER_H
#define MINICUBESOLVER_H

#include <vector>
#include <unordered_map>

class MiniCubeSolver
{
public:
    MiniCubeSolver();
    ~MiniCubeSolver();
    
    //设置最大搜索深度
    void setMaxDepth(int maxDepth);
    
    //求解任意两个状态之间的转换方法，返回值是操作指令id序列
    bool solve(const int cubeState1[6][4], const int cubeState2[6][4]);
    
    //得到指令序列
    std::vector<int> getCmdList();
    
private:
    //求解出的操作指令序列
    std::vector<int> m_cmdList;
    
    //最大搜索深度
    int m_maxDepth;
    
    //
    std::unordered_map<int,int> m_blackListStates;
    int m_hashCode;
    int m_hashMat[24];
    int getCubeStateHash(int *cube);
};

#endif // MINICUBESOLVER_H
