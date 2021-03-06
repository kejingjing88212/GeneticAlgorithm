#include <iostream>
#include <algorithm>
#include <vector>
#include <cstdarg>
// #include <iomanip> //控制cout 输出格式
#include "cvplot.h"
#include "opencv2/opencv.hpp"
#include "GA.h"
#include "demo.h"
//Y=x^2*sin(3X*pi)
float fun1(std::vector<float> argv){
	float x=argv[0];
	return (x*x*std::sin(3*pi*x));
}
void demo1(){
	cv::Mat Popula;
	std::vector<float> data;
	std::vector<std::pair<float, float> > recode;
	GA ga;
	ga.solve(fun1,1);
	//种群越大，越稳定，基因编码越多迭代越稳定
	Popula=ga.crtbp();
	for(int i=0;i<20;i++){
		ga.bs2rv(Popula,0,10);
		std::pair<std::vector<float>, float> best=ga.ranking();
		recode.push_back(std::pair<float,float>((best.first[0]),best.second));
		data.push_back(recode[recode.size()-1].second);
		ga.select(Popula).recombin(Popula).mut(Popula);
	}
	//绘图
	{
		auto name="math";
		cvplot::setWindowTitle(name,"origin curve");
		cvplot::moveWindow(name, 0, 0);
		cvplot::resizeWindow(name, 400, 400);
		auto &figure=cvplot::figure(name);
		std::vector<std::pair<float, float> > data;
		for(float i=0;i<10;i+=0.025)
			data.push_back({i,i*i*std::sin(i*3*pi)});
		figure.series("x^2*sin(3x*pi)").set(data).color(cvplot::Green);
		figure.series("recode").set(recode).type(cvplot::Dots).color(cvplot::Red);
		figure.border(30).show(false);
	}
	{
		auto name="GA";
		cvplot::setWindowTitle(name,"GA");
		cvplot::moveWindow(name, 400, 0);
		cvplot::resizeWindow(name, 400, 400);
		auto &figure=cvplot::figure(name);
		std::sort(data.begin(), data.end(), [](float &a,float &b){if(a<b)return true;else return false;});
		figure.series("count").setValue(data).color(cvplot::Orange);
		figure.border(30).show();
	}
	cv::waitKey(0);

}
//(x*std::cos(2*pi*y)+y*std::sin(2*pi*x))
float fun2(std::vector<float> argv){
	float x=argv[0];
	float y=argv[1];
	return (x*std::cos(2*pi*y)+y*std::sin(2*pi*x));
}
void demo2(){
	cv::Mat Popula;
	std::vector<float> data;
	std::vector<std::pair<float, float> > recodex,recodey;
	GA ga(80,80);
	ga.solve(fun2,2);
	//种群越大，越稳定，基因编码越多迭代越稳定
	Popula=ga.crtbp();
	for(int i=0;i<80;i++){
		ga.bs2rv(Popula,-2,2);
		std::pair<std::vector<float>, float> best=ga.ranking();
		recodex.push_back(std::pair<float,float>((best.first[0]),best.second));
		recodey.push_back(std::pair<float,float>((best.first[1]),best.second));
		data.push_back(recodex[recodex.size()-1].second);
		ga.select(Popula).recombin(Popula).mut(Popula);
	}
	//绘图
	{
		auto name="math";
		cvplot::setWindowTitle(name,"origin curve");
		cvplot::moveWindow(name, 0, 0);
		cvplot::resizeWindow(name, 400, 400);
		auto &figure=cvplot::figure(name);
		figure.series("x-z").set(recodex).type(cvplot::Dots).color(cvplot::Green);
		figure.series("y-z").set(recodey).type(cvplot::Dots).color(cvplot::Red);
		figure.border(30).show(false);
	}
	{
		auto name="GA";
		cvplot::setWindowTitle(name,"GA");
		cvplot::moveWindow(name, 400, 0);
		cvplot::resizeWindow(name, 400, 400);
		auto &figure=cvplot::figure(name);
		std::sort(data.begin(), data.end(), [](float &a,float &b){if(a<b)return true;else return false;});
		figure.series("count").setValue(data).color(cvplot::Orange);
		figure.border(30).show();
	}
	cv::waitKey(0);
}

void demo3(){
	std::vector<float> data;
	float _input[2][3]={{1,2,3},{4,5,6}};
	float _output[2][2]={{1.,2.},{3.,4.}};
	cv::Mat input(cv::Size(3,2),CV_32FC1,_input);
	cv::Mat output(cv::Size(2,2),CV_32FC1,_output);
	GA_BP ga(10,440);
	cv::Mat Popula;
	ga.BPsolve(input, output);
	Popula=ga.crtbp();
	for(int i=0;i<20;i++){
		ga.bs2rv(Popula,0,4);
		std::pair<std::vector<float>, float> best=ga.ranking();
		data.push_back(best.second);
		ga.select(Popula,1).recombin(Popula).mut(Popula);
	}
	{
		auto name="GA";
		cvplot::setWindowTitle(name,"GA");
		cvplot::moveWindow(name, 0, 0);
		cvplot::resizeWindow(name, 400, 400);
		auto &figure=cvplot::figure(name);
		figure.series("count").setValue(data).color(cvplot::Orange);
		figure.border(30).show();
	}
	cv::waitKey(0);
}
void demo4(){
	float _address[14][2]={{16.47,96.10},{16.47,94.44},
						   {22.39,93.37},{25.23,97.24},
						   {20.47,97.02},{17.20,96.29},
						   {14.05,98.12},{22.00,96.05},
						   {16.53,97.38},{21.52,95.59},
						   {19.41,97.13},{20.09,92.55}};
	std::vector<float> data;
	std::vector<float> road_recode;
	float min_distance=-1.0/0.0;
	cv::Mat address(cv::Size(2,12),CV_32FC1,_address);
	GA_TSP ga(100,address.rows);
	cv::Mat Popula;
	ga.TSPsolve(address);
	Popula=ga.crtbp();
	for(int i=0;i<400;i++){
		std::pair<std::vector<float>, float> best=ga.ranking(Popula);
		data.push_back(best.second);
		if(best.second>min_distance){
			road_recode=best.first;
			min_distance=best.second;
		}
		ga.select(Popula,0).recombin(Popula,0.1).mut(Popula,0.5);
	}
	{
		auto name="math";
		cvplot::setWindowTitle(name,"best map");
		cvplot::moveWindow(name, 0, 0);
		cvplot::resizeWindow(name, 400, 400);
		auto &figure=cvplot::figure(name);
		figure.origin(false, false);
		std::vector<std::pair<float, float> > recode;
		for(int i=0;i<road_recode.size();i++)
			recode.push_back(std::pair<float,float>(
				_address[(int)road_recode[i]][0],_address[(int)road_recode[i]][1]));
		recode.push_back(std::pair<float,float>(
				_address[(int)road_recode[0]][0],_address[(int)road_recode[0]][1]));
		figure.series("line").set(recode).color(cvplot::Green);
		figure.series("point").set(recode).type(cvplot::Dots).color(cvplot::Red);
		
		figure.border(30).show(false);
	}
	{
		auto name="GA";
		cvplot::setWindowTitle(name,"GA");
		cvplot::moveWindow(name, 400, 0);
		cvplot::resizeWindow(name, 400, 400);
		auto &figure=cvplot::figure(name);
		std::sort(data.begin(), data.end(), [](float &a,float &b){if(a<b)return true;else return false;});
		figure.series("count").setValue(data).color(cvplot::Orange);
		figure.border(25).show();
	}
	cv::waitKey(0);
}
/**
 * 求解目标是 fun2二元表达式
 */
void demo5(void){
	cv::Mat Popula;
	std::vector<float> data;
	std::vector<std::pair<float, float> > recodex,recodey;
	QGA ga(80,80);
	ga.solve(fun2,2);
	//种群越大，越稳定，基因编码越多迭代越稳定
	Popula=ga.crtbp();
	for(int i=0;i<80;i++){
		ga.bs2rv(Popula,-2,2);
		std::pair<std::vector<float>, float> best=ga.ranking();
		recodex.push_back(std::pair<float,float>((best.first[0]),best.second));
		recodey.push_back(std::pair<float,float>((best.first[1]),best.second));
		data.push_back(recodex[recodex.size()-1].second);
		ga.select(Popula);
	}
	//绘图
	{
		auto name="math";
		cvplot::setWindowTitle(name,"origin curve");
		cvplot::moveWindow(name, 0, 0);
		cvplot::resizeWindow(name, 400, 400);
		auto &figure=cvplot::figure(name);
		figure.series("x-z").set(recodex).type(cvplot::Dots).color(cvplot::Green);
		figure.series("y-z").set(recodey).type(cvplot::Dots).color(cvplot::Red);
		figure.border(30).show(false);
	}
	{
		auto name="GA";
		cvplot::setWindowTitle(name,"GA");
		cvplot::moveWindow(name, 400, 0);
		cvplot::resizeWindow(name, 400, 400);
		auto &figure=cvplot::figure(name);
		std::sort(data.begin(), data.end(), [](float &a,float &b){if(a<b)return true;else return false;});
		figure.series("count").setValue(data).color(cvplot::Orange);
		figure.border(30).show();
	}
	cv::waitKey(0);
}
/**
 * 粒子群算法求解
 */
float fun3(std::vector<float> argv){
	float x=argv[0];
	float y=argv[1];
	return (x*std::cos(2*pi*y)+y*std::sin(2*pi*x));
}
void demo6(){
	cv::Mat Popula;
	std::vector<float> data;
	std::vector<std::pair<float, float> > recodex,recodey;
	PSO pso(50,2,-2,2);
	pso.solve(fun3);
	//种群越大，越稳定，基因编码越多迭代越稳定
	pso.crtbp();
	for(int i=0;i<80;i++){
		std::pair<std::vector<float>, float> best=pso.ranking();
		recodex.push_back(std::pair<float,float>((best.first[0]),best.second));
		recodey.push_back(std::pair<float,float>((best.first[1]),best.second));
		data.push_back(recodex[recodex.size()-1].second);
		pso.update(1);
	}
	//绘图
	{
		auto name="math";
		cvplot::setWindowTitle(name,"origin curve");
		cvplot::moveWindow(name, 0, 0);
		cvplot::resizeWindow(name, 400, 400);
		auto &figure=cvplot::figure(name);
		figure.series("x-z").set(recodex).type(cvplot::Dots).color(cvplot::Green);
		figure.series("y-z").set(recodey).type(cvplot::Dots).color(cvplot::Red);
		figure.border(30).show(false);
	}
	{
		auto name="PSO";
		cvplot::setWindowTitle(name,"PSOq");
		cvplot::moveWindow(name, 400, 0);
		cvplot::resizeWindow(name, 400, 400);
		auto &figure=cvplot::figure(name);
		// std::sort(data.begin(), data.end(), [](float &a,float &b){if(a<b)return true;else return false;});
		figure.series("count").setValue(data).color(cvplot::Orange);
		figure.border(30).show();
	}
	cv::waitKey(0);
}