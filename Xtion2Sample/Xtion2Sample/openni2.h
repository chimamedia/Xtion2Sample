#pragma once

#include <OpenNI.h> //OpenNI2を使用する(RGB-D)
//#include <NiTE.h> //NITE2を使用する(Skeleton)

#include "opencv.h"

#pragma comment(lib, "C:\\Program Files\\OpenNI2\\Lib\\OpenNI2.lib")
//#pragma comment(lib, "C:\\Program Files\\PrimeSense\\NiTE2\\Lib\\NiTE2.lib")


/* -------------------------------------------
* カラーストリームデータをOpenCV型へ変換する
* [in] カラーストリーム
* [out]　cv::Mat
* ------------------------------------------*/
void ChangeColorStream(const openni::VideoFrameRef &videoFrame,
	cv::Mat &image)
{
	image = cv::Mat(
		videoFrame.getHeight(),
		videoFrame.getWidth(),
		CV_8UC3,
		(unsigned char*)videoFrame.getData());

	cv::cvtColor(image, image, CV_BGR2RGB);
}

/* -------------------------------------------
* 深度ストリームデータをOpenCV型へ変換する
* 0～10000ミリメートルを8ビットに変換する
* [in] 深度ストリーム
* [out]　cv::Mat
* ------------------------------------------*/
void ChangeDepthStream(const openni::VideoFrameRef &videoFrame,
	cv::Mat &image)
{
	image = cv::Mat(
		videoFrame.getHeight(),
		videoFrame.getWidth(),
		CV_16UC1,
		(unsigned char*)videoFrame.getData());

	image.convertTo(image, CV_8U, 255.0 / 10000);
}