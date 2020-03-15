#ifndef MINICUBEVISUALIZER_H
#define MINICUBEVISUALIZER_H

#include <opencv2/core.hpp>

class MiniCubeVisualizer
{
public:
    MiniCubeVisualizer();
    
    //翻转魔方
    void vizFrontL();
    void vizFrontR();
    void vizRightL();
    void vizRightR();
    void vizTopL();
    void vizTopR();
    
    //得到图片
    cv::Mat getImage(int isFront);
    
    //得到魔方当前状态
    std::vector<int> getCurrentState();
    
private:
    //魔方状态
    std::vector<int> m_cube;
    
    //可视化魔方状态
    int m_cubeFace[6][4];
};

#endif // MINICUBEVISUALIZER_H
