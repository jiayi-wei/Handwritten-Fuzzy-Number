#include <cv.h>
#include <highgui.h>
#include <iostream>
#include <fstream>
#include <io.h>
#include "image_to_data.h"
#include "file_reader.h"
#include "EM.h"
#include <sstream>
#include <math.h>

using namespace std;
using namespace cv;

void main()
{
	string file_path="C:\\Users\\Administrator\\Desktop\\data\\A\\";
	string file_path_B="C:\\Users\\Administrator\\Desktop\\data\\B\\";
	vector<string> file_list;
	string image_path;
	char t[256];
	char t_[256];
	string s,s_;
	
	Mat image,gray_image,final_image;
	Data temp_da;
	Data temp_da_B;
	Mat hooli_temp;
	Model initia;
	train_Data train_temp;
	vector<train_Data> train_vec[10];
	vector<Data> test_vec[10];
	vector<Data> test_vec_B[10];
	vector<Model> cat_vec[10];

	for(int i=0;i<=9;i++)
	{
		sprintf(t,"%d",i);
		s=t;
		string search_path=file_path+s+"\\*.bmp";
		if(!get_filelist(search_path,file_list))
			cout<<"open file error!"<<endl;
		int sum_number=file_list.size();
		int end_number=(sum_number*4)/5;
		for(int j=0;j<file_list.size();j++)   
		{
			image_path=file_path+s+"\\"+file_list[j];
			image=imread(image_path);
			cvtColor(image,gray_image,CV_RGB2GRAY,1);
			adaptiveThreshold(gray_image,final_image,255,CV_ADAPTIVE_THRESH_GAUSSIAN_C,CV_THRESH_BINARY,5,0);
			train_temp.image=GetData(final_image);
			if(j<end_number)
				train_vec[i].push_back(train_temp);
			else 
			{
				temp_da.image=train_temp.image;
				test_vec[i].push_back(temp_da);
			}
		}
		for (int m=0;m<5;m++)
		{
			initia.Hooli=0.2;
			initia.Mean=train_vec[i][m].image;
			initia.Var.create(12,12,CV_64FC1);
			initia.Var=Mat::eye(12,12,CV_64F);
			cat_vec[i].push_back(initia);
		}
		hooli_temp=EM(train_vec[i],cat_vec[i]);
		for( int m=0;m<5;m++)
			cat_vec[i][m].Hooli=hooli_temp.at<double>(0,m);
		file_list.clear();
	}

	Mat image_;
	for(int i=0;i<=9;i++)
	{
		sprintf(t,"%d",i);
		s=t;
		for(int j=0;j<200;j++)
		{
			sprintf(t_,"%d",j);
			s_=t_;
			string search_path=file_path_B+s+"_"+s_+".bmp";
			image_=imread(search_path,CV_LOAD_IMAGE_GRAYSCALE);
			temp_da_B.image=GetData(image_);
			test_vec_B[i].push_back(temp_da_B);
		}
	}
	
	cout<<"A"<<endl;
	test_data(test_vec,cat_vec);
	cout<<"B"<<endl;
	test_data(test_vec_B,cat_vec);

	system("PAUSE");
}
