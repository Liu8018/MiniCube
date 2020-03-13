#include "MiniCubeVisualizer.h"
#include "MiniCubeFunctions.h"
#include <opencv2/imgproc.hpp>

MiniCubeVisualizer::MiniCubeVisualizer()
{
    for(int i=0;i<8;i++){
        m_cube[i] = i;
    }
    
    for(int i=0;i<6;i++){
        for(int j=0;j<4;j++){
            m_cubeFace[i][j] = i;
        }
    }
}

void halfBlockL(int cubeFace[6][4], int sideIdList[5], int idList[12])
{
    int s0=sideIdList[0], 
        s1=sideIdList[1], s2=sideIdList[2],
        s3=sideIdList[3], s4=sideIdList[4];
    
    int i0=idList[0], i1=idList[1], i2=idList[2],
        i3=idList[3], i4=idList[4], i5=idList[5],
        i6=idList[6], i7=idList[7], i8=idList[8],
        i9=idList[9], i10=idList[10], i11=idList[11];
    
    int tmpv1 = cubeFace[s0][i3];
    cubeFace[s0][i3] = cubeFace[s0][i4];
    cubeFace[s0][i4] = cubeFace[s0][i8];
    cubeFace[s0][i8] = cubeFace[s0][i7];
    cubeFace[s0][i7] = tmpv1;
    
    tmpv1 = cubeFace[s1][i0];
    int tmpv2 = cubeFace[s1][i1];
    cubeFace[s1][i0] = cubeFace[s4][i5];
    cubeFace[s1][i1] = cubeFace[s4][i9];
    cubeFace[s4][i5] = cubeFace[s3][i11];
    cubeFace[s4][i9] = cubeFace[s3][i10];
    cubeFace[s3][i11] = cubeFace[s2][i6];
    cubeFace[s3][i10] = cubeFace[s2][i2];
    cubeFace[s2][i6] = tmpv1;
    cubeFace[s2][i2] = tmpv2;
}
void halfBlockR(int cubeFace[6][4], int sideIdList[5], int idList[12])
{
    int s0=sideIdList[0], 
        s1=sideIdList[1], s2=sideIdList[2],
        s3=sideIdList[3], s4=sideIdList[4];
    
    int i0=idList[0], i1=idList[1], i2=idList[2],
        i3=idList[3], i4=idList[4], i5=idList[5],
        i6=idList[6], i7=idList[7], i8=idList[8],
        i9=idList[9], i10=idList[10], i11=idList[11];
    
    int tmpv1 = cubeFace[s0][i3];
    cubeFace[s0][i3] = cubeFace[s0][i7];
    cubeFace[s0][i7] = cubeFace[s0][i8];
    cubeFace[s0][i8] = cubeFace[s0][i4];
    cubeFace[s0][i4] = tmpv1;
    
    tmpv1 = cubeFace[s1][i0];
    int tmpv2 = cubeFace[s1][i1];
    cubeFace[s1][i0] = cubeFace[s2][i6];
    cubeFace[s1][i1] = cubeFace[s2][i2];
    cubeFace[s2][i6] = cubeFace[s3][i11];
    cubeFace[s2][i2] = cubeFace[s3][i10];
    cubeFace[s3][i11] = cubeFace[s4][i5];
    cubeFace[s3][i10] = cubeFace[s4][i9];
    cubeFace[s4][i5] = tmpv1;
    cubeFace[s4][i9] = tmpv2;
}

void MiniCubeVisualizer::vizFrontL()
{
    frontL(m_cube);
    
    int sideIdList[5] = {0,5,3,4,1};
    int idList[12] = {2,3,0,0,1,0,2,2,3,2,2,3};
    halfBlockL(m_cubeFace,sideIdList,idList);
}
void MiniCubeVisualizer::vizFrontR()
{
    frontR(m_cube);
    
    int sideIdList[5] = {0,5,3,4,1};
    int idList[12] = {2,3,0,0,1,0,2,2,3,2,2,3};
    halfBlockR(m_cubeFace,sideIdList,idList);
}
void MiniCubeVisualizer::vizRightL()
{
    rightL(m_cube);
    
    int sideIdList[5] = {1,5,0,4,2};
    int idList[12] = {3,1,1,0,1,1,3,2,3,3,3,1};
    halfBlockL(m_cubeFace,sideIdList,idList);
}
void MiniCubeVisualizer::vizRightR()
{
    rightR(m_cube);
    
    int sideIdList[5] = {1,5,0,4,2};
    int idList[12] = {3,1,1,0,1,1,3,2,3,3,3,1};
    halfBlockR(m_cubeFace,sideIdList,idList);
}
void MiniCubeVisualizer::vizTopL()
{
    topL(m_cube);
    
    int sideIdList[5] = {5,2,3,0,1};
    int idList[12] = {0,1,1,0,1,1,0,2,3,0,0,1};
    halfBlockL(m_cubeFace,sideIdList,idList);
}
void MiniCubeVisualizer::vizTopR()
{
    topR(m_cube);
    
    int sideIdList[5] = {5,2,3,0,1};
    int idList[12] = {0,1,1,0,1,1,0,2,3,0,0,1};
    halfBlockR(m_cubeFace,sideIdList,idList);
}

cv::Mat MiniCubeVisualizer::getImage(int isFront)
{
    //6种颜色
    cv::Scalar clrList[6];
    clrList[0] = cv::Scalar(255,0,0);
    clrList[1] = cv::Scalar(0,255,0);
    clrList[2] = cv::Scalar(255,0,255);
    clrList[3] = cv::Scalar(255,255,0);
    clrList[4] = cv::Scalar(0,255,255);
    clrList[5] = cv::Scalar(0,0,255);
    
    //19个角点
    cv::Point A(100,200);
    cv::Point B(100,400);
    cv::Point C(300,400);
    cv::Point D(300,200);
    cv::Point E(400,300);
    cv::Point F(400,100);
    cv::Point G(200,100);
    cv::Point a = (A+B)/2;
    cv::Point b = (B+C)/2;
    cv::Point f = (C+E)/2;
    cv::Point e = (C+D)/2;
    cv::Point d = (A+D)/2;
    cv::Point h = (D+F)/2;
    cv::Point g = (E+F)/2;
    cv::Point i = (A+G)/2;
    cv::Point k = (G+F)/2;
    cv::Point c = (a+e)/2;
    cv::Point m = (e+g)/2;
    cv::Point j = (d+k)/2;
    cv::Point n = (a+d)/2;
    cv::Point p = (b+e)/2;
    cv::Point q = (B+c)/2;
    
    
    //画布
    cv::Mat canvas(500,500,CV_8UC3);
    canvas = cv::Scalar(255,255,255);
    
    if(isFront){
        //12个多边形
        std::vector<std::vector<cv::Point>> plgs(12);
        plgs[0] = {A,a,c,d};
        plgs[1] = {d,c,e,D};
        plgs[2] = {a,B,b,c};
        plgs[3] = {c,b,C,e};
        plgs[4] = {G,i,j,k};
        plgs[5] = {k,j,h,F};
        plgs[6] = {i,A,d,j};
        plgs[7] = {j,d,D,h};
        plgs[8] = {D,e,m,h};
        plgs[9] = {h,m,g,F};
        plgs[10] = {e,C,f,m};
        plgs[11] = {m,f,E,g};
        
        //绘制
        for(int i=0;i<12;i++){
            int sideId = i/4;
            int vizSideId;
            if(sideId == 0) vizSideId = 0;
            else if(sideId == 1) vizSideId = 5;
            else vizSideId = 1;
            
            int id = i%4;
            int clrId = m_cubeFace[vizSideId][id];
            cv::Scalar color = clrList[clrId];
            cv::drawContours(canvas,plgs,i,color,-1);
        }
        
        //画线
        cv::Scalar lineClr(0,0,0);
        int lineThick = 2;
        cv::line(canvas,A,B,lineClr,lineThick);
        cv::line(canvas,A,D,lineClr,lineThick);
        cv::line(canvas,C,D,lineClr,lineThick);
        cv::line(canvas,B,C,lineClr,lineThick);
        cv::line(canvas,D,F,lineClr,lineThick);
        cv::line(canvas,C,E,lineClr,lineThick);
        cv::line(canvas,E,F,lineClr,lineThick);
        cv::line(canvas,A,G,lineClr,lineThick);
        cv::line(canvas,G,F,lineClr,lineThick);
        cv::line(canvas,a,e,lineClr,lineThick);
        cv::line(canvas,b,d,lineClr,lineThick);
        cv::line(canvas,i,h,lineClr,lineThick);
        cv::line(canvas,d,k,lineClr,lineThick);
        cv::line(canvas,e,g,lineClr,lineThick);
        cv::line(canvas,h,f,lineClr,lineThick);
    }
    else{
        //12个多边形
        std::vector<std::vector<cv::Point>> plgs(12);
        plgs[0] = {A,a,n,i};
        plgs[1] = {i,n,d,G};
        plgs[2] = {a,B,q,n};
        plgs[3] = {n,q,c,d};
        plgs[4] = {G,d,D,k};
        plgs[5] = {k,D,g,F};
        plgs[6] = {d,c,e,D};
        plgs[7] = {D,e,E,g};
        plgs[8] = {c,q,p,e};
        plgs[9] = {e,p,f,E};
        plgs[10] = {q,B,b,p};
        plgs[11] = {p,b,C,f};
        
        //绘制
        for(int i=0;i<12;i++){
            int sideId = i/4;
            int vizSideId;
            if(sideId == 0) vizSideId = 3;
            else if(sideId == 1) vizSideId = 2;
            else vizSideId = 4;
            
            int id = i%4;
            int clrId = m_cubeFace[vizSideId][id];
            cv::Scalar color = clrList[clrId];
            cv::drawContours(canvas,plgs,i,color,-1);
        }
        
        //画线
        cv::Scalar lineClr(0,0,0);
        int lineThick = 2;
        cv::line(canvas,A,B,lineClr,lineThick);
        cv::line(canvas,A,G,lineClr,lineThick);
        cv::line(canvas,G,c,lineClr,lineThick);
        cv::line(canvas,B,c,lineClr,lineThick);
        cv::line(canvas,G,F,lineClr,lineThick);
        cv::line(canvas,F,E,lineClr,lineThick);
        cv::line(canvas,c,E,lineClr,lineThick);
        cv::line(canvas,C,E,lineClr,lineThick);
        cv::line(canvas,B,C,lineClr,lineThick);
        cv::line(canvas,a,d,lineClr,lineThick);
        cv::line(canvas,i,q,lineClr,lineThick);
        cv::line(canvas,k,e,lineClr,lineThick);
        cv::line(canvas,d,g,lineClr,lineThick);
        cv::line(canvas,q,f,lineClr,lineThick);
        cv::line(canvas,e,b,lineClr,lineThick);
    }
    
    return canvas;
}
