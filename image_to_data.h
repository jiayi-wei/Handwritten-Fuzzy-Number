#include <math.h>
#include <cv.h>
#include <highgui.h>
#include <iostream>
#include <fstream>
#include <io.h>

using namespace std;
using namespace cv;


Mat GetData( Mat src_img)
{
	Mat tmp_img;
	
	int height = src_img.rows;  
    int width  = src_img.cols;  
  
    int ceil_height_little = height/4;  
    int ceil_width_little  = width/3; 

	int ceil_height_large =ceil_height_little+1;
	int ceil_width_large  =ceil_width_little+1;

	int remain_height = height%4;
	int remain_width  = width%3;

	int ceil_height[4],ceil_width[3];
  
	if(remain_height==0)
	{
		ceil_height[0]=ceil_height_little;
		ceil_height[1]=ceil_height_little;
		ceil_height[2]=ceil_height_little;
		ceil_height[3]=ceil_height_little;
	}
	else if(remain_height==1)
	{
		ceil_height[0]=ceil_height_large;
		ceil_height[1]=ceil_height_little;
		ceil_height[2]=ceil_height_little;
		ceil_height[3]=ceil_height_little;
	}
	else if(remain_height==2)
	{
		ceil_height[0]=ceil_height_large;
		ceil_height[1]=ceil_height_little;
		ceil_height[2]=ceil_height_little;
		ceil_height[3]=ceil_height_large;
	}
	else
	{
		ceil_height[0]=ceil_height_large;
		ceil_height[1]=ceil_height_large;
		ceil_height[2]=ceil_height_little;
		ceil_height[3]=ceil_height_large;
	}

	if(remain_width==0)
	{
		ceil_width[0]=ceil_width_little;
		ceil_width[1]=ceil_width_little;
		ceil_width[2]=ceil_width_little;
	}
	else if(remain_width==1)
	{
		ceil_width[0]=ceil_width_little;
		ceil_width[1]=ceil_width_little;
		ceil_width[2]=ceil_width_little;
	}
	else
	{
		ceil_width[0]=ceil_width_large;
		ceil_width[1]=ceil_width_large;
		ceil_width[2]=ceil_width_little;
	}

	int x=0;
	int y=0;
	int n=0;
	double counter=0;
	
	Mat temp;
	temp.create(1,12,CV_64FC1);

    for(int i = 0;i<4;i++)
	{
        for(int j = 0;j<3;j++)
		{    
            Rect rect(x,y,ceil_width[j],ceil_height[i]);   
            waitKey(0); 
			x+=ceil_width[j];
			src_img(rect).copyTo(tmp_img);
			for(int row=0;row<tmp_img.rows;row++)
			{
				for(int col=0;col<tmp_img.cols;col++)
				{
					if(tmp_img.at<uchar>(row,col)==255)
						counter++;
				}
			}
			double rows_tmp=double(tmp_img.rows);
			double cols_tmp=double(tmp_img.cols);
			
			temp.at<double>(0,n)=counter/(rows_tmp*cols_tmp);
			
			n++;
			counter=0;
        } 
		x=0;
		y+=ceil_height[i];
	}
	return temp;
} 