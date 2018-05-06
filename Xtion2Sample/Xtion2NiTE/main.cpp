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
		//NiTE������������
		nite::Status rcn = nite::NiTE::initialize();

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

		//���[�U�[�g���b�J�[�ϐ�
		nite::UserTracker userTracker;
		rcn = userTracker.create();
		if (rcn != nite::Status::STATUS_OK)
		{
			printf("Viewer: Device open failed:\n\n");
			throw std::runtime_error("userTracker.create(&device) failed");
		}

		//�\���p�ϐ�
		cv::Mat colorImage, depthImage;

		//���[�U�̍��W�f�[�^
		nite::Point3f userPositions[15][15];

		//�{����
		while (1)
		{
			//�t���[���̍X�V����
			openni::VideoFrameRef colorFrame, depthFrame;

			//�X�V���ꂽ�t���[���̎擾
			colorStream.readFrame(&colorFrame);
			//depthStream.readFrame(&depthFrame);

			//�t���[���f�[�^����摜�f�[�^�֕ϊ�����
			ChangeColorStream(colorFrame, colorImage);
			//ChangeDepthStream(depthFrame, depthImage);

			nite::UserTrackerFrameRef userFrame;
			userTracker.readFrame(&userFrame);

			//�[�x�摜��\���ł���悤�ɂ���
			ChangeUserTrackerStream(userFrame, depthImage);

			const nite::Array<nite::UserData> &users = userFrame.getUsers();

			for (int i = 0; i < users.getSize(); ++i)
			{
				const nite::UserData& user = users[i];

				//�V�������o�������[�U�Ȃ�A�g���b�L���O�J�n
				if (user.isNew())
				{
					userTracker.startSkeletonTracking(user.getId());
				}

				//���łɌ��o�������[�U�̏���
				//�������Ă��Ȃ��Ȃ�X�P���g���\��
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