#ifndef MINICUBESOLVER_H
#define MINICUBESOLVER_H

#include <vector>
#include <map>

class MiniCubeSolver
{
public:
    MiniCubeSolver();
    
    //设置最大搜索深度
    void setMaxDepth(int maxDepth);
    
    //广度优先搜索，无法确定搜索层数，且难以解决状态重复问题，
    //bool breadthFirstSolve(const std::vector<int> &cubeState1,
    //                       const std::vector<int> &cubeState2,
    //                       int maxDepth, int maxNodesPerLayer,
    //                       std::vector<int> &cmdPath);
    //深度搜索，递归版
    bool singlePathSolve_recursion(const std::vector<int> &cubeState1, 
                                   const std::vector<int> &cubeState2);
    //深度搜索，为避免栈溢出实现的非递归方案
    bool singlePathSolve(const std::vector<int> &cubeState1, 
                         const std::vector<int> &cubeState2);
    
    //得到指令序列
    std::vector<int> getCmdList();
    
private:
    //魔方状态向量长度
    int m_cubeVecLen;
    
    //求解出的操作指令序列
    std::vector<int> m_cmdList;
    
    //状态向量相似度
    float calcOverlapRatio(const std::vector<int> &cubeState1, 
                           const std::vector<int> &cubeState2);
    
    //从某状态生成下一级的6个状态
    void genNext6States(const std::vector<int> &cube, 
                        std::vector<std::vector<int>> &nextStates);
    
    //按与目标状态的重合率排序，最大重合率为1时返回true
    bool sortStatesByOverlapRatio(const std::vector<int> &targetState, 
                                  std::vector<std::vector<int>> &states, 
                                  std::vector<int> &cmdIds);
    
    //为防止状态重复用了map
    std::map<std::string,int> m_blackListStates;
    int m_hashCode;
    std::string getCubeStateStr(const std::vector<int> &cube);
    
    //深度搜索的非递归方案用到的函数和变量
    std::vector<std::vector<int>> m_curStates;
    std::vector<int> m_curCmdIds;
    int m_curStateId;
    void backStack();
    void pushStack();
    std::vector<std::vector<std::vector<int>>> m_stateStack;
    std::vector<std::vector<int>> m_cmdIdsStack;
    std::vector<int> m_stateIdStack;
    
    //优化深度搜索的结果
    void optimizePath(std::vector<std::vector<int> > &statePath,
                      std::vector<int> &cmdPath);
};

#endif // MINICUBESOLVER_H
