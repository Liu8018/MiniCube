#include <iostream>
#include <opencv2/highgui.hpp>

#include "MiniCubeVisualizer.h"

int main()
{
    MiniCubeVisualizer vizer;
    cv::Mat image(500,1000,CV_8UC3);
    image = cv::Scalar(255,255,255);
    cv::Mat image_b = image.colRange(0,500);
    cv::Mat image_f = image.colRange(500,1000);
    vizer.getImage(0).copyTo(image_b);
    vizer.getImage(1).copyTo(image_f);
    cv::imshow("MiniCube",image);
    
    while(1){
        char key = cv::waitKey();
        if(key == '1') vizer.vizFrontL();
        else if(key == '3') vizer.vizFrontR();
        else if(key == '4') vizer.vizRightL();
        else if(key == '6') vizer.vizRightR();
        else if(key == '9') vizer.vizTopL();
        else if(key == '7') vizer.vizTopR();
        else if(key == 'q') break;
        
        vizer.getImage(0).copyTo(image_b);
        vizer.getImage(1).copyTo(image_f);
        cv::imshow("MiniCube",image);
    }
    
    return 0;
}
