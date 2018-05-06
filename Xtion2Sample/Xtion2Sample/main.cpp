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

		//深度情報の初期化を行う
		rc = depthStream.create(device, openni::SENSOR_DEPTH);
		if (rc == openni::STATUS_OK)
		{
			rc = depthStream.start();
			if (rc != openni::STATUS_OK)
			{
				printf("Viewer: Couldn't start depth stream:\n%s\n", openni::OpenNI::getExtendedError());
				depthStream.destroy();
				throw std::runtime_error("Couldn't find depth stream failed");
			}
		}
		else
		{
			printf("Viewer: Couldn't find depth stream:\n%s\n", openni::OpenNI::getExtendedError());
			throw std::runtime_error("Couldn't find depth stream failed");
		}

		//表示用変数
		cv::Mat colorImage, depthImage;

		//本処理
		while (1)
		{
			//フレームの更新処理
			openni::VideoFrameRef colorFrame, depthFrame;

			//更新されたフレームの取得
			colorStream.readFrame(&colorFrame);
			depthStream.readFrame(&depthFrame);

			//フレームデータから画像データへ変換する
			ChangeColorStream(colorFrame, colorImage);
			ChangeDepthStream(depthFrame, depthImage);

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
