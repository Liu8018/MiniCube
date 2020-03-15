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
    
    //求解任意两个状态之间的转换方法，返回值是操作指令id序列
    bool breadthFirstSolve(const std::vector<int> &cubeState1, const std::vector<int> &cubeState2);
    bool singlePathSolve(const std::vector<int> &cubeState1, const std::vector<int> &cubeState2);
    bool singlePathSolve_recursion(const std::vector<int> &cubeState1, const std::vector<int> &cubeState2);
    
    //得到指令序列
    std::vector<int> getCmdList();
    
private:
    //魔方状态向量长度
    int m_cubeVecLen;
    
    //求解出的操作指令序列
    std::vector<int> m_cmdList;
    
    //最大搜索深度
    int m_maxDepth;
    
    //状态向量相似度
    float calcOverlapRatio(const std::vector<int> &cubeState1, const std::vector<int> &cubeState2);
    
    //从某状态生成下一级的6个状态
    void genNext6States(const std::vector<int> &cube, std::vector<std::vector<int>> &nextStates);
    
    //按与目标状态的重合率排序，最大重合率为1时返回true
    bool sortStatesByOverlapRatio(const std::vector<int> &targetState, 
                                  std::vector<std::vector<int>> &states, std::vector<int> &cmdIds);
    
    //
    std::map<std::string,int> m_blackListStates;
    int m_hashCode;
    std::string getCubeStateStr(const std::vector<int> &cube);
    
    //singlePathSolve用到的
    std::vector<std::vector<int>> m_curStates;
    std::vector<int> m_curCmdIds;
    int m_curStateId;
    void backStack();
    void pushStack();
    std::vector<std::vector<std::vector<int>>> m_stateStack;
    std::vector<std::vector<int>> m_cmdIdsStack;
    std::vector<int> m_stateIdStack;
};

#endif // MINICUBESOLVER_H
