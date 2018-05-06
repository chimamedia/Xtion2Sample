// Windows�̏ꍇ��Release�R���p�C�������
// �����I�ȑ��x�œ��삵�܂��B

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
		//�h���C�o�̏��
		openni::Status rc = openni::STATUS_OK;

		//�f�o�C�X���
		openni::Device device;

		//�Z���T���瓾��ꂽRGB-D�f�[�^
		openni::VideoStream depthStream, colorStream;

		//ANY_DEVICE�Ȃ�PC�ɐڑ�����Ă���f�o�C�X��ǂݍ���
		const char* deviceURI = openni::ANY_DEVICE;

		//OpenNI�̏��������s��
		rc = openni::OpenNI::initialize();
		printf("After initialization:\n%s\n", openni::OpenNI::getExtendedError());

		//�f�o�C�X��ǂݍ��߂����ǂ���
		rc = device.open(deviceURI);
		if (rc != openni::DEVICE_STATE_OK)
		{
			printf("Viewer: Device open failed:\n%s\n", openni::OpenNI::getExtendedError());
			openni::OpenNI::shutdown();
			throw std::runtime_error("openni::Device::open() failed");
		}

		//�J���[�摜�̏��������s��
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

		//�[�x���̏��������s��
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

		//�\���p�ϐ�
		cv::Mat colorImage, depthImage;

		//�{����
		while (1)
		{
			//�t���[���̍X�V����
			openni::VideoFrameRef colorFrame, depthFrame;

			//�X�V���ꂽ�t���[���̎擾
			colorStream.readFrame(&colorFrame);
			depthStream.readFrame(&depthFrame);

			//�t���[���f�[�^����摜�f�[�^�֕ϊ�����
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
