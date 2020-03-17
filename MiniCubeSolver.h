#ifndef MINICUBESOLVER_H
#define MINICUBESOLVER_H

#include <vector>
#include <map>
#include <mutex>

class MiniCubeSolver
{
public:
    MiniCubeSolver();
    
    //深度优先搜索，递归版，限制深度
    bool depthFirstSolve_recursion(const std::vector<int> &cubeState1, 
                                   const std::vector<int> &cubeState2,
                                   int maxDepth);
    //深度搜索，为避免栈溢出实现的非递归方案，不限制深度，一秒求解任意状态
    bool singlePathSolve(const std::vector<int> &cubeState1, 
                         const std::vector<int> &cubeState2);
    
    //限制深度的深度优先搜索的多线程加速版
    bool depthFirstSolve_multiThread(const std::vector<int> &cubeState1, 
                                     const std::vector<int> &cubeState2,
                                     int maxDepth);
    
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
    std::map<std::string,bool> m_blackListStates;
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
    
    //优化不限制深度搜索的结果(未完成)
    void optimizePath(std::vector<std::vector<int> > &statePath,
                      std::vector<int> &cmdPath);
    
    //多线程深度优先搜索用到的变量和函数
    std::mutex m_mutexLock;
    bool m_isSolved_multiThread;
    bool depthFirstSolve_thread(const std::vector<int> &cubeState1, 
                                const std::vector<int> &cubeState2, 
                                int maxDepth,
                                std::map<std::string,bool> &blackList,
                                std::vector<int> &cmdPath);
};

#endif // MINICUBESOLVER_H
