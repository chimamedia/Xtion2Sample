#pragma once

#include <OpenNI.h> //OpenNI2���g�p����(RGB-D)
//#include <NiTE.h> //NITE2���g�p����(Skeleton)

#include "opencv.h"

#pragma comment(lib, "C:\\Program Files\\OpenNI2\\Lib\\OpenNI2.lib")
//#pragma comment(lib, "C:\\Program Files\\PrimeSense\\NiTE2\\Lib\\NiTE2.lib")


/* -------------------------------------------
* �J���[�X�g���[���f�[�^��OpenCV�^�֕ϊ�����
* [in] �J���[�X�g���[��
* [out]�@cv::Mat
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
* �[�x�X�g���[���f�[�^��OpenCV�^�֕ϊ�����
* 0�`10000�~�����[�g����8�r�b�g�ɕϊ�����
* [in] �[�x�X�g���[��
* [out]�@cv::Mat
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