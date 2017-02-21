#include <math.h>
#include <cv.h>
#include <highgui.h>
#include <stdlib.h>

using namespace std;
using namespace cv;

struct Data
{
	Mat image;
	int classify;
};

struct train_Data
{
	Mat image;
	double belong[5];
};

struct Model
{
	Mat Mean;
	Mat Var;
	double Hooli;
};

//Model EM(vector<Mat> img_vec)
//{
//	Model temp;
//	temp.Mean.create(1,12,CV_64FC1);
//	temp.Var.create(12,12,CV_64FC1);
//	Mat mean_sum;
//	mean_sum=Mat::zeros(1,12,CV_64F);
//	Mat var_sum;
//	var_sum=Mat::zeros(12,12,CV_64F);
//
//	double counter=(double)(img_vec.size());
//
//	for(int i=0;i<img_vec.size();i++)
//		mean_sum=mean_sum+img_vec[i];
//	temp.Mean=mean_sum*(1/counter);
//	for(int i=0;i<img_vec.size();i++)
//		var_sum=var_sum+((img_vec[i]-temp.Mean).t()*(img_vec[i]-temp.Mean));
//	temp.Var=var_sum*(1/counter);
//
//	return temp;
//}



Mat EM(vector<train_Data> vec,vector<Model> cat)  // 返回值类型//
{
	Mat hooli_temp;
	hooli_temp.create(1,5,CV_64FC1);
	double thres1=0;
	double thres2=0;
	double arry[5];
	Mat casual;
	do
	{
		for(int i=0;i<vec.size();i++)
		{	
			double temp_sum=0;
			double temp_=0;
			double temp_1=0;
			for(int j=0;j<5;j++)
				arry[j]=0;
			for(int j=0;j<cat.size();j++)
			{
				casual=(vec[i].image-cat[j].Mean)*(cat[j].Var.inv(DECOMP_CHOLESKY)*(vec[i].image-cat[j].Mean).t());
				temp_1=sqrt(determinant(cat[j].Var));
				temp_=(exp((-0.5)*(casual.at<double>(0,0)))*(cat[j].Hooli));
				temp_sum+=(1/temp_1)*temp_;
				arry[j]=(1/temp_1)*temp_;
			}
			for(int j=0;j<5;j++)
				vec[i].belong[j]=(arry[j]/temp_sum);
		}
		for(int i=0;i<cat.size();i++)
		{
			double hooli_sum=0;
			double hooli_new=0;
			Mat mean_sum=Mat::zeros(1,12,CV_64F);
			Mat mean_new=Mat::zeros(1,12,CV_64F);
			Mat var_sum=Mat::zeros(12,12,CV_64F);
			Mat var_new=Mat::zeros(12,12,CV_64F);

			for(int j=0;j<vec.size();j++)
			{
				hooli_sum+=vec[j].belong[i];
				mean_sum=mean_sum+(vec[j].belong[i]*vec[j].image);
				var_sum=var_sum+vec[j].belong[i]*(vec[j].image.t()*vec[j].image);
			}
			double record=(double)(vec.size());
			hooli_new=hooli_sum/record;
			mean_new=(mean_sum*(1/hooli_sum));
			var_new=(var_sum*(1/hooli_sum));
			cat[i].Hooli=hooli_new;
			for(int j=0;j<12;j++)
			{
				thres1+=abs(cat[i].Mean.at<double>(0,j)-mean_new.at<double>(0,j));
				cat[i].Mean.at<double>(0,j)=mean_new.at<double>(0,j);
				for(int m=0;m<12;m++)
				{
					cat[i].Var.at<double>(m,j)=var_new.at<double>(m,j);
					thres2+=abs(cat[i].Var.at<double>(m,j)-var_new.at<double>(m,j));
				}
			}
		}
	}while(thres1<0.00001&&thres2<0.00001);
	for(int i=0;i<5;i++)
		hooli_temp.at<double>(0,i)=cat[i].Hooli;
	return hooli_temp;
}

int Classify(vector<Model> (&cat)[10],Mat image)
{
	double belong[10]={0};
	double possi[5];
	Mat casual;
	double temp_1,temp_2;
	double be_long=0;
	int o_o;

	for(int i=0;i<10;i++)
	{
		for(int j=0;j<cat[i].size();j++)
		{
			casual=(image-cat[i][j].Mean)*cat[i][j].Var.inv(DECOMP_CHOLESKY)*((image-cat[i][j].Mean).t());
			temp_1=sqrt(determinant(cat[i][j].Var));
			temp_2=exp((-0.5)*(casual.at<double>(0,0)));
			possi[j]=(temp_2)/(temp_1)*cat[i][j].Hooli;
			belong[i]=belong[i]+possi[j];
		}
	}
	for(int i=0;i<10;i++)
	{
		if (belong[i]>be_long)
		{
			be_long=belong[i];
			o_o=i;
		}
	}

	
	return o_o;
}

void test_data(vector<Data> (&test_vec)[10],vector<Model> (&cat_vec)[10])
{
	int result[10][10]={0};

	for(int i=0;i<10;i++)
	{
		for(int j=0;j<test_vec[i].size();j++)
		{
			test_vec[i][j].classify=Classify(cat_vec,test_vec[i][j].image);
			result[i][test_vec[i][j].classify]++;
		}
	}

	for(int i=0;i<10;i++)
	{
		double couter_presice=0;
		double recall=0;
		double presice=0;
		for(int j=0;j<10;j++)
			couter_presice+=result[j][i];
		cout<<result[i][i]<<"   "<<test_vec[i].size()<<endl;
		recall=result[i][i]/(double)test_vec[i].size();
		presice=result[i][i]/couter_presice;
		cout<<i<<"的召回率为："<<recall*100<<"% ,准确率为"<<presice*100<<"%  "<<endl;;
	}
}