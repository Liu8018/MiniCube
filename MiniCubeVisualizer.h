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
    
private:
    //魔方的各个角块
    int m_cube[8];
    
    //用于显示
    int m_cubeFace[6][4];
};

#endif // MINICUBEVISUALIZER_H
