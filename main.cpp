#include <iostream>
#include <cstdlib>
#include<opencv/cv.h>
#include<opencv/highgui.h>
using namespace cv;
cv::Mat getGramMat(std::vector<cv::Mat>& betas){
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
cv::Mat getGramMat(std::vector<cv::Mat>& betas,cv::Mat& beta){
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
cv::Mat getPlane(std::vector<cv::Point3d>& points){
	size_t n=points.size();
	std::vector<cv::Mat> betas;
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
cv::Mat getPlane(std::vector<cv::Point3d>& points,int index){
	assert(index>=0&&index<=2);
	size_t n=points.size();
	std::vector<cv::Mat> betas;
	cv::Mat beta1=cv::Mat::zeros(n,1,CV_64F);//X
	cv::Mat beta2=cv::Mat::zeros(n,1,CV_64F);//Y
	cv::Mat beta3=cv::Mat::ones(n,1,CV_64F);//1
	cv::Mat beta=cv::Mat::zeros(n,1,CV_64F);//Z
	for(size_t i=0;i<n;i++){
		switch(index){
		case 0:
			beta1.at<double>(i,0)=points[i].y;
			beta2.at<double>(i,0)=points[i].z;
			beta.at<double>(i,0)=points[i].x;
			break;
		case 1:
			beta1.at<double>(i,0)=points[i].x;
			beta2.at<double>(i,0)=points[i].z;
			beta.at<double>(i,0)=points[i].y;
			break;
		case 2:
			beta1.at<double>(i,0)=points[i].x;
			beta2.at<double>(i,0)=points[i].y;
			beta.at<double>(i,0)=points[i].z;
		}
	}
	betas.push_back(beta1);
	betas.push_back(beta2);
	betas.push_back(beta3);
	cv::Mat g=getGramMat(betas);
	cv::Mat gb=getGramMat(betas,beta);
	return g.inv()*gb;
}
/**
 * 假设平面方程为Ax+By+Cz+D=0;
 * 返回A,B,C,D
 */
std::array<double,4> getStandrdPlane(vector<cv::Point3d>& points){
	assert(points.size()>=3);
	cv::Point3d p1=points[0];
	cv::Point3d p2=points[1];
	cv::Point3d p3=points[2];
	std::vector<cv::Mat> betas;
	cv::Vec3d v1(p2-p1);
	cv::Vec3d v2(p3-p1);
	cv::Vec3d v3=v1.cross(v2);
	std::array<double,4> result={0};
	if(v3[2]!=0){
		cv::Mat m=getPlane(points,2);
		result[0]=m.at<double>(0,0);
		result[1]=m.at<double>(1,0);
		result[2]=-1;
		result[3]=m.at<double>(2,0);
	}else if(v3[1]!=0){
		cv::Mat m=getPlane(points,2);
		result[0]=m.at<double>(0,0);
		result[1]=-1;
		result[2]=m.at<double>(1,0);
		result[3]=m.at<double>(2,0);
	}else if(v3[0]!=0){
		cv::Mat m=getPlane(points,2);
		result[0]=-1;
		result[1]=m.at<double>(0,0);
		result[2]=m.at<double>(1,0);
		result[3]=m.at<double>(2,0);
	}
	return result;
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

