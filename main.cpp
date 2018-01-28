#include <iostream>
#include <cstdlib>
#include<opencv/cv.h>
#include<opencv/highgui.h>
using namespace cv;
cv::Mat getGramMat(vector<cv::Mat>& betas){
	size_t n=betas.size();
	cv::Mat m=cv::Mat::zeros(n,n,CV_64F);
	for(size_t i=0;i<n;i++){
		for(size_t j=0;j<n;j++){
			m.at<double>(i,j)=betas[i].dot(betas[j]);
			std::cout<<i<<","<<j<<":"<<betas[i].dot(betas[j])<<std::endl;
		}
	}
	return m;
}
cv::Mat getGramMat(vector<cv::Mat>& betas,cv::Mat& beta){
	size_t n=betas.size();
	cv::Mat m=cv::Mat::zeros(n,1,CV_64F);
	for(size_t i=0;i<n;i++){
		m.at<double>(i,0)=betas[i].dot(beta);
		//std::cout<<i<<","<<j<<":"<<betas[i].dot(betas[j])<<std::endl;

	}
	return m;
}
/**
 * 假设平面方程为z=k1*x+k2*y+k3
 * 返回(k1,k2,k3)
 */
cv::Mat getPlane(vector<cv::Point3d>& points){
	size_t n=points.size();
	vector<cv::Mat> betas;
	cv::Mat beta1=cv::Mat::zeros(n,1,CV_64F);//X
	cv::Mat beta2=cv::Mat::zeros(n,1,CV_64F);//Y
	cv::Mat beta3=cv::Mat::ones(n,1,CV_64F);//1
	cv::Mat beta=cv::Mat::zeros(n,1,CV_64F);//Z
	for(size_t i=0;i<n;i++){
		beta1.at<double>(i,0)=points[i].x;
		beta2.at<double>(i,0)=points[i].y;
		beta.at<double>(i,0)=points[i].z;
	}
	betas.push_back(beta1);
	betas.push_back(beta2);
	betas.push_back(beta3);
	cv::Mat g=getGramMat(betas);
	cv::Mat gb=getGramMat(betas,beta);
	return g.inv()*gb;
}
int main()
{
	cv::Point3d p1(1,0,0);
	cv::Point3d p2(0,1,0);
	cv::Point3d p3((double)1/3,(double)1/3,(double)1/3);
	//cv::Point3d p3(1,1,0);

	vector<cv::Point3d> points;
	points.push_back(p1);
	points.push_back(p2);
	points.push_back(p3);
	std::cout<<getPlane(points)<<std::endl;
}

