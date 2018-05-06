// Windowsの場合はReleaseコンパイルすると
// 現実的な速度で動作します。

using namespace std;

#include "openni2.h"
#include "opencv.h"
#include <iostream>

void PleaseBottom()
{
	system("pause");
}

int main(int argc, char **argv)
{
	try
	{
		//NiTEを初期化する
		nite::Status rcn = nite::NiTE::initialize();

		//ドライバの状態
		openni::Status rc = openni::STATUS_OK;

		//デバイス情報
		openni::Device device;

		//センサから得られたRGB-Dデータ
		openni::VideoStream depthStream, colorStream;

		//ANY_DEVICEならPCに接続されているデバイスを読み込む
		const char* deviceURI = openni::ANY_DEVICE;

		//OpenNIの初期化を行う
		rc = openni::OpenNI::initialize();
		printf("After initialization:\n%s\n", openni::OpenNI::getExtendedError());

		//デバイスを読み込めたかどうか
		rc = device.open(deviceURI);
		if (rc != openni::DEVICE_STATE_OK)
		{
			printf("Viewer: Device open failed:\n%s\n", openni::OpenNI::getExtendedError());
			openni::OpenNI::shutdown();
			throw std::runtime_error("openni::Device::open() failed");
		}

		//カラー画像の初期化を行う
		rc = colorStream.create(device, openni::SENSOR_COLOR);
		if (rc == openni::STATUS_OK)
		{
			rc = colorStream.start();
			if (rc != openni::STATUS_OK)
			{
				printf("Viewer: Couldn't start color stream:\n%s\n", openni::OpenNI::getExtendedError());
				colorStream.destroy();
				throw std::runtime_error("Couldn't start color stream failed");
			}
		}
		else
		{
			printf("SimpleViewer: Couldn't find color stream:\n%s\n", openni::OpenNI::getExtendedError());
			throw std::runtime_error("Couldn't find color stream failed");
		}

		//ユーザートラッカー変数
		nite::UserTracker userTracker;
		rcn = userTracker.create();
		if (rcn != nite::Status::STATUS_OK)
		{
			printf("Viewer: Device open failed:\n\n");
			throw std::runtime_error("userTracker.create(&device) failed");
		}

		//表示用変数
		cv::Mat colorImage, depthImage;

		//ユーザの座標データ
		nite::Point3f userPositions[15][15];

		//本処理
		while (1)
		{
			//フレームの更新処理
			openni::VideoFrameRef colorFrame, depthFrame;

			//更新されたフレームの取得
			colorStream.readFrame(&colorFrame);
			//depthStream.readFrame(&depthFrame);

			//フレームデータから画像データへ変換する
			ChangeColorStream(colorFrame, colorImage);
			//ChangeDepthStream(depthFrame, depthImage);

			nite::UserTrackerFrameRef userFrame;
			userTracker.readFrame(&userFrame);

			//深度画像を表示できるようにする
			ChangeUserTrackerStream(userFrame, depthImage);

			const nite::Array<nite::UserData> &users = userFrame.getUsers();

			for (int i = 0; i < users.getSize(); ++i)
			{
				const nite::UserData& user = users[i];

				//新しく検出したユーザなら、トラッキング開始
				if (user.isNew())
				{
					userTracker.startSkeletonTracking(user.getId());
				}

				//すでに検出したユーザの処理
				//消失していないならスケルトン表示
				if (!user.isLost())
				{
					DrawSkeleton(colorImage, userTracker, user);
					DrawSkeletonPoint(colorImage, userTracker, user);
				}

				GetUserSkeletionPosition(userFrame.getDepthFrame(), userTracker,
					user, userPositions[i]);

			}

			cerr << "Head = " << userPositions[0][0].x << "," << userPositions[0][0].y << "," << userPositions[0][0].z << endl;
			cerr << "Head = " << userPositions[1][0].x << "," << userPositions[1][0].y << "," << userPositions[1][0].z << endl;

			if (userPositions[0][nite::JOINT_LEFT_HAND].y < userPositions[0][nite::JOINT_HEAD].y)
				cv::putText(colorImage, "Hand Up", cv::Point(0, 5), cv::FONT_HERSHEY_COMPLEX, 0.7, cv::Scalar(0, 255, 255), 2);

			openni::VideoFrameRef vfr = userFrame.getDepthFrame();
			int depthDist;
			cv::Point2i cent(colorImage.cols / 2, colorImage.rows / 2);
			GetDepthData(vfr, cent, depthDist);
			cv::circle(depthImage, cent, 2, cv::Scalar(255), -1);
			cerr << depthDist << "\n";
			if (!colorImage.empty()) cv::imshow("COLOR", colorImage);
			if (!depthImage.empty()) cv::imshow("DEPTH", depthImage);

			int key = cv::waitKey(1);
			if (key == 'Q' || key == 'q')
			{
				break;
			}
		}


		return 0;
	}
	catch (std::exception& e)
	{
		cerr << openni::OpenNI::getExtendedError() << "\n";
		PleaseBottom();
	}
}