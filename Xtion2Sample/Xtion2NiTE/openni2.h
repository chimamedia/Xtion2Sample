#pragma once

#include <OpenNI.h> //OpenNI2���g�p����(RGB-D)
//#include <NiTE.h> //NITE2���g�p����(Skeleton)
#include <NiTE.h>
#include "opencv.h"

#pragma comment(lib, "C:\\Program Files\\OpenNI2\\Lib\\OpenNI2.lib")
#pragma comment(lib, "C:\\Program Files\\PrimeSense\\NiTE2\\Lib\\NiTE2.lib")


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

/* -------------------------------------------
* �[�x�����擾����
* [in] �[�x�X�g���[��
* [out]�@�[�x����
* ------------------------------------------*/
template<typename T>
void GetDepthData(const openni::VideoFrameRef &videoFrame,
	const T x, const T y, T &depthDist)
{
	openni::VideoMode videMode = videoFrame.getVideoMode();
	int position = (y * videMode.getResolutionX()) + x;

	unsigned short* depth = (unsigned short*)videoFrame.getData();

	depthDist = depth[position];
}
void GetDepthData(const openni::VideoFrameRef &videoFrame,
	const cv::Point2i &point, int &depthDist)
{
	GetDepthData(videoFrame, point.x, point.y, depthDist);
}


//NiTE�̏���
/* -------------------------------------------
* �[�x�X�g���[���f�[�^��OpenCV�^�֕ϊ�����
* 0�`10000�~�����[�g����8�r�b�g�ɕϊ�����
* [in] �[�x�X�g���[��
* [out]�@cv::Mat
* ------------------------------------------*/
void ChangeUserTrackerStream(nite::UserTrackerFrameRef userTracke,
	cv::Mat &image)
{
	openni::VideoFrameRef depthFrame = userTracke.getDepthFrame();

	if (depthFrame.isValid())
	{
		ChangeDepthStream(depthFrame, image);
	}
}

/* -------------------------------------------
* ���[�U�̍��i�����擾����
* [in] ���[�U���
* [out]�@���[�U�̍��W
* ------------------------------------------*/
void GetUserSkeletionPosition(
	const openni::VideoFrameRef &videoFrame,
	nite::UserTracker &userTracker,
	const nite::UserData user,
	nite::Point3f *userPosition)
{
	//�X�P���g�����擾���A�ǐՏ�Ԃ��m�F
	const nite::Skeleton &skeleton = user.getSkeleton();
	if (skeleton.getState() != nite::SKELETON_TRACKED)
	{
		return;
	}

	//���ׂĂ̊֐߂�`��
	for (int j = 0; j < 15; ++j)
	{
		//�M���l�i70%�j�ȏ�Ȃ珈������
		const nite::SkeletonJoint& joint = skeleton.getJoint((nite::JointType)j);
		if (joint.getPositionConfidence() < 0.7f)
		{
			continue;
		}

		//3������2�����ɕϊ�
		float fx = 0.0f, fy = 0.0f, fz = 0.0f;
		const nite::Point3f& position = joint.getPosition();
		userTracker.convertJointCoordinatesToDepth(
			position.x,
			position.y,
			position.z,
			&fx,
			&fy);

		userPosition[j].x = fx;
		userPosition[j].y = fy;
		GetDepthData(videoFrame, fx, fy, userPosition[j].z);
	}
}

/* -------------------------------------------
* �J���[�X�g���[���f�[�^��OpenCV�^�֕ϊ�����
* [in] �J���[�X�g���[��
* [out]�@cv::Mat
* ------------------------------------------*/
void DrawSkeletonPoint(
	cv::Mat &image,
	nite::UserTracker &userTracker,
	const nite::UserData user)
{
	//�X�P���g�����擾���A�ǐՏ�Ԃ��m�F
	const nite::Skeleton &skeleton = user.getSkeleton();
	if (skeleton.getState() != nite::SKELETON_TRACKED)
	{
		return;
	}

	//���ׂĂ̊֐߂�`��
	for (int j = 0; j < 15; ++j)
	{
		//�M���l�i70%�j�ȏ�Ȃ珈������
		const nite::SkeletonJoint& joint = skeleton.getJoint((nite::JointType)j);
		if (joint.getPositionConfidence() < 0.7f)
		{
			continue;
		}

		//3������2�����ɕϊ�
		float fx = 0.0f, fy = 0.0f;
		const nite::Point3f& position = joint.getPosition();
		userTracker.convertJointCoordinatesToDepth(
			position.x,
			position.y,
			position.z,
			&fx,
			&fy);

		cv::circle(image, cv::Point(fx, fy), 5, cv::Scalar(0, 0, 255), -1);
	}
}

void DrawLine(const nite::SkeletonJoint& joint1, const nite::SkeletonJoint& joint2,
	const nite::UserTracker &userTracker, cv::Mat &image)
{
	if (joint1.getPositionConfidence() < 0.7f ||
		joint2.getPositionConfidence() < 0.7f)
	{
		return;
	}

	//3������2�����ɕϊ�
	cv::Point2f f1, f2;
	const nite::Point3f& position1 = joint1.getPosition();
	userTracker.convertJointCoordinatesToDepth(
		position1.x, position1.y, position1.z,
		&f1.x, &f1.y);

	const nite::Point3f& position2 = joint2.getPosition();
	userTracker.convertJointCoordinatesToDepth(
		position2.x, position2.y, position2.z,
		&f2.x, &f2.y);

	cv::line(image, f1, f2, cv::Scalar(0, 255, 0), 2, CV_AA, 0);
}


void DrawSkeleton(
	cv::Mat &image,
	nite::UserTracker &userTracker,
	const nite::UserData user)
{
	//�X�P���g�����擾���A�ǐՏ�Ԃ��m�F
	const nite::Skeleton &skeleton = user.getSkeleton();
	if (skeleton.getState() != nite::SKELETON_TRACKED)
	{
		return;
	}

	DrawLine(skeleton.getJoint(nite::JOINT_HEAD), skeleton.getJoint(nite::JOINT_NECK), userTracker, image);

	DrawLine(skeleton.getJoint(nite::JOINT_NECK), skeleton.getJoint(nite::JOINT_LEFT_SHOULDER), userTracker, image);
	DrawLine(skeleton.getJoint(nite::JOINT_NECK), skeleton.getJoint(nite::JOINT_RIGHT_SHOULDER), userTracker, image);
	DrawLine(skeleton.getJoint(nite::JOINT_RIGHT_SHOULDER), skeleton.getJoint(nite::JOINT_TORSO), userTracker, image);
	DrawLine(skeleton.getJoint(nite::JOINT_LEFT_SHOULDER), skeleton.getJoint(nite::JOINT_TORSO), userTracker, image);

	DrawLine(skeleton.getJoint(nite::JOINT_LEFT_SHOULDER), skeleton.getJoint(nite::JOINT_LEFT_ELBOW), userTracker, image);
	DrawLine(skeleton.getJoint(nite::JOINT_LEFT_ELBOW), skeleton.getJoint(nite::JOINT_LEFT_HAND), userTracker, image);

	DrawLine(skeleton.getJoint(nite::JOINT_RIGHT_SHOULDER), skeleton.getJoint(nite::JOINT_RIGHT_ELBOW), userTracker, image);
	DrawLine(skeleton.getJoint(nite::JOINT_RIGHT_ELBOW), skeleton.getJoint(nite::JOINT_RIGHT_HAND), userTracker, image);

	DrawLine(skeleton.getJoint(nite::JOINT_TORSO), skeleton.getJoint(nite::JOINT_LEFT_HIP), userTracker, image);
	DrawLine(skeleton.getJoint(nite::JOINT_TORSO), skeleton.getJoint(nite::JOINT_RIGHT_HIP), userTracker, image);

	DrawLine(skeleton.getJoint(nite::JOINT_LEFT_HIP), skeleton.getJoint(nite::JOINT_LEFT_KNEE), userTracker, image);
	DrawLine(skeleton.getJoint(nite::JOINT_LEFT_KNEE), skeleton.getJoint(nite::JOINT_LEFT_FOOT), userTracker, image);

	DrawLine(skeleton.getJoint(nite::JOINT_RIGHT_HIP), skeleton.getJoint(nite::JOINT_RIGHT_KNEE), userTracker, image);
	DrawLine(skeleton.getJoint(nite::JOINT_RIGHT_KNEE), skeleton.getJoint(nite::JOINT_RIGHT_FOOT), userTracker, image);
}

