#pragma once

#include <OpenNI.h> //OpenNI2を使用する(RGB-D)
//#include <NiTE.h> //NITE2を使用する(Skeleton)
#include <NiTE.h>
#include "opencv.h"

#pragma comment(lib, "C:\\Program Files\\OpenNI2\\Lib\\OpenNI2.lib")
#pragma comment(lib, "C:\\Program Files\\PrimeSense\\NiTE2\\Lib\\NiTE2.lib")


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

/* -------------------------------------------
* 深度情報を取得する
* [in] 深度ストリーム
* [out]　深度距離
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


//NiTEの処理
/* -------------------------------------------
* 深度ストリームデータをOpenCV型へ変換する
* 0～10000ミリメートルを8ビットに変換する
* [in] 深度ストリーム
* [out]　cv::Mat
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
* ユーザの骨格情報を取得する
* [in] ユーザ情報
* [out]　ユーザの座標
* ------------------------------------------*/
void GetUserSkeletionPosition(
	const openni::VideoFrameRef &videoFrame,
	nite::UserTracker &userTracker,
	const nite::UserData user,
	nite::Point3f *userPosition)
{
	//スケルトンを取得し、追跡状態を確認
	const nite::Skeleton &skeleton = user.getSkeleton();
	if (skeleton.getState() != nite::SKELETON_TRACKED)
	{
		return;
	}

	//すべての関節を描画
	for (int j = 0; j < 15; ++j)
	{
		//信頼値（70%）以上なら処理する
		const nite::SkeletonJoint& joint = skeleton.getJoint((nite::JointType)j);
		if (joint.getPositionConfidence() < 0.7f)
		{
			continue;
		}

		//3次元を2次元に変換
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
* カラーストリームデータをOpenCV型へ変換する
* [in] カラーストリーム
* [out]　cv::Mat
* ------------------------------------------*/
void DrawSkeletonPoint(
	cv::Mat &image,
	nite::UserTracker &userTracker,
	const nite::UserData user)
{
	//スケルトンを取得し、追跡状態を確認
	const nite::Skeleton &skeleton = user.getSkeleton();
	if (skeleton.getState() != nite::SKELETON_TRACKED)
	{
		return;
	}

	//すべての関節を描画
	for (int j = 0; j < 15; ++j)
	{
		//信頼値（70%）以上なら処理する
		const nite::SkeletonJoint& joint = skeleton.getJoint((nite::JointType)j);
		if (joint.getPositionConfidence() < 0.7f)
		{
			continue;
		}

		//3次元を2次元に変換
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

	//3次元を2次元に変換
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
	//スケルトンを取得し、追跡状態を確認
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

