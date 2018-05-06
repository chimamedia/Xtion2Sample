/***
* Revised 2016/08/07 for OpenCV-3
***/

#pragma once

#include <iostream>
#include <opencv2/opencv.hpp>
#include <opencv2/core/core.hpp>              // (C++�p)
#include <opencv2/imgproc/imgproc.hpp>        // (C++�p)
#include <opencv2/highgui/highgui.hpp>        // (C++�p)
#include <opencv2/ml/ml.hpp>                  // �T�|�[�g�x�N�^�}�V���A�u�[�X�e�B���O�Ȃǂ̋@�B�w�K
#include <opencv2/features2d/features2d.hpp>  // SURF�AFAST�Ȃǂ̓������o
#include <opencv2/xfeatures2d.hpp>
#include <opencv2/xfeatures2d/nonfree.hpp>
#include <opencv2/video/background_segm.hpp>  // �O�i/�w�i����
#include <opencv2/video/tracking.hpp>         // �g���b�L���O
#include <opencv2/objdetect/objdetect.hpp>    // Haar�ALBP�AHOG�Ȃǂ̃I�u�W�F�N�g���o��
#include <opencv2/calib3d/calib3d.hpp>        // �J�����L�����u���[�V�����A�X�e���I�J�����Ȃ�
#include <opencv2/flann/flann.hpp>            // �����ŋߖT����(FLANN)�Ȃ�
#include <opencv2/superres/optical_flow.hpp>  //�I�v�e�B�J���t���[
//#include <opencv2/tracking/feature.hpp>

#define CV_VERSION_STR CVAUX_STR(CV_MAJOR_VERSION) CVAUX_STR(CV_MINOR_VERSION) CVAUX_STR(CV_SUBMINOR_VERSION)
#ifdef _DEBUG
#define CV_EXT_STR "d.lib"
#else
#define CV_EXT_STR ".lib"
#endif

#define _X64
//#undef _X64

#ifdef _X64
#pragma comment(lib, "C:\\OpenCV3.1.0\\vs2013\\x64\\lib\\opencv_calib3d"	CV_VERSION_STR	CV_EXT_STR)
#pragma comment(lib, "C:\\OpenCV3.1.0\\vs2013\\x64\\lib\\opencv_core"	CV_VERSION_STR	CV_EXT_STR)
#pragma comment(lib, "C:\\OpenCV3.1.0\\vs2013\\x64\\lib\\opencv_features2d"	CV_VERSION_STR	CV_EXT_STR)
#pragma comment(lib, "C:\\OpenCV3.1.0\\vs2013\\x64\\lib\\opencv_flann"	CV_VERSION_STR	CV_EXT_STR)
#pragma comment(lib, "C:\\OpenCV3.1.0\\vs2013\\x64\\lib\\opencv_highgui"	CV_VERSION_STR	CV_EXT_STR)
#pragma comment(lib, "C:\\OpenCV3.1.0\\vs2013\\x64\\lib\\opencv_imgcodecs"	CV_VERSION_STR	CV_EXT_STR)
#pragma comment(lib, "C:\\OpenCV3.1.0\\vs2013\\x64\\lib\\opencv_imgproc"	CV_VERSION_STR	CV_EXT_STR)
#pragma comment(lib, "C:\\OpenCV3.1.0\\vs2013\\x64\\lib\\opencv_ml"	CV_VERSION_STR	CV_EXT_STR)
#pragma comment(lib, "C:\\OpenCV3.1.0\\vs2013\\x64\\lib\\opencv_objdetect"	CV_VERSION_STR	CV_EXT_STR)
#pragma comment(lib, "C:\\OpenCV3.1.0\\vs2013\\x64\\lib\\opencv_photo"	CV_VERSION_STR	CV_EXT_STR)
#pragma comment(lib, "C:\\OpenCV3.1.0\\vs2013\\x64\\lib\\opencv_shape"	CV_VERSION_STR	CV_EXT_STR)
#pragma comment(lib, "C:\\OpenCV3.1.0\\vs2013\\x64\\lib\\opencv_stitching"	CV_VERSION_STR	CV_EXT_STR)
#pragma comment(lib, "C:\\OpenCV3.1.0\\vs2013\\x64\\lib\\opencv_superres"	CV_VERSION_STR	CV_EXT_STR)
#pragma comment(lib, "C:\\OpenCV3.1.0\\vs2013\\x64\\lib\\opencv_ts"	CV_VERSION_STR	CV_EXT_STR)
#pragma comment(lib, "C:\\OpenCV3.1.0\\vs2013\\x64\\lib\\opencv_video"	CV_VERSION_STR	CV_EXT_STR)
#pragma comment(lib, "C:\\OpenCV3.1.0\\vs2013\\x64\\lib\\opencv_videoio"	CV_VERSION_STR	CV_EXT_STR)
#pragma comment(lib, "C:\\OpenCV3.1.0\\vs2013\\x64\\lib\\opencv_videostab"	CV_VERSION_STR	CV_EXT_STR)
#else
#pragma comment(lib, "I:\\OpenCV3.1.0\\vs2013\\x86\\lib\\opencv_calib3d"	CV_VERSION_STR	CV_EXT_STR)
#pragma comment(lib, "I:\\OpenCV3.1.0\\vs2013\\x86\\lib\\opencv_core"	CV_VERSION_STR	CV_EXT_STR)
#pragma comment(lib, "I:\\OpenCV3.1.0\\vs2013\\x86\\lib\\opencv_features2d"	CV_VERSION_STR	CV_EXT_STR)
#pragma comment(lib, "I:\\OpenCV3.1.0\\vs2013\\x86\\lib\\opencv_flann"	CV_VERSION_STR	CV_EXT_STR)
#pragma comment(lib, "I:\\OpenCV3.1.0\\vs2013\\x86\\lib\\opencv_highgui"	CV_VERSION_STR	CV_EXT_STR)
#pragma comment(lib, "I:\\OpenCV3.1.0\\vs2013\\x86\\lib\\opencv_imgcodecs"	CV_VERSION_STR	CV_EXT_STR)
#pragma comment(lib, "I:\\OpenCV3.1.0\\vs2013\\x86\\lib\\opencv_imgproc"	CV_VERSION_STR	CV_EXT_STR)
#pragma comment(lib, "I:\\OpenCV3.1.0\\vs2013\\x86\\lib\\opencv_ml"	CV_VERSION_STR	CV_EXT_STR)
#pragma comment(lib, "I:\\OpenCV3.1.0\\vs2013\\x86\\lib\\opencv_objdetect"	CV_VERSION_STR	CV_EXT_STR)
#pragma comment(lib, "I:\\OpenCV3.1.0\\vs2013\\x86\\lib\\opencv_photo"	CV_VERSION_STR	CV_EXT_STR)
#pragma comment(lib, "I:\\OpenCV3.1.0\\vs2013\\x86\\lib\\opencv_shape"	CV_VERSION_STR	CV_EXT_STR)
#pragma comment(lib, "I:\\OpenCV3.1.0\\vs2013\\x86\\lib\\opencv_stitching"	CV_VERSION_STR	CV_EXT_STR)
#pragma comment(lib, "I:\\OpenCV3.1.0\\vs2013\\x86\\lib\\opencv_superres"	CV_VERSION_STR	CV_EXT_STR)
//#pragma comment(lib, "I:\\OpenCV3.1.0\\vs2013\\x86\\lib\\opencv_ts"	CV_VERSION_STR	CV_EXT_STR)
#pragma comment(lib, "I:\\OpenCV3.1.0\\vs2013\\x86\\lib\\opencv_video"	CV_VERSION_STR	CV_EXT_STR)
#pragma comment(lib, "I:\\OpenCV3.1.0\\vs2013\\x86\\lib\\opencv_videoio"	CV_VERSION_STR	CV_EXT_STR)
#pragma comment(lib, "I:\\OpenCV3.1.0\\vs2013\\x86\\lib\\opencv_videostab"	CV_VERSION_STR	CV_EXT_STR)
#endif

/***** �摜�̃s�N�Z���̒��ڑ��� OpenCV-2.x (MAT* ) *****/

// �ėp�}�N��
// IMG: IplImage*, TYPE: �^, X,Y: ���W, ID: �C���f�b�N�X
// �^
// IPL_DEPTH_8U: unsigned char
// IPL_DEPTH_16S: short
// IPL_DEPTH_

#define M_VALUE(MAT, TYPE, X, Y, ID) ((TYPE*)((MAT)->data + (MAT)->step*(Y)))[(X)*(MAT)->channels()+(ID)]
// �ėp�}�N��
// IMG: IplImage*, TYPE: �^, X,Y: ���W, ID: �C���f�b�N�X
// M_IMG:Mat ,M_X,M_Y:���W,ID:�C���f�b�N�X
// �^
// IPL_DEPTH_8U: unsigned char
// IPL_DEPTH_16S: short
// IPL_DEPTH_
#define I_VALUE(IMG, TYPE, X, Y, ID) ((TYPE*)((IMG)->imageData + (IMG)->widthStep*(Y)))[(X)*(IMG)->nChannels+(ID)]
#define I_VALUE_FOR_MAT(M_IMG, TYPE,M_X,M_Y,ID) (TYPE)M_IMG.data[(M_X) * M_IMG.channels() + (ID) +  M_IMG.step * (M_Y)]	
// �悭�g���摜�p�}�N��
// 8�r�b�g1�`�����l���摜 (CV_8UC1)
#define MVC(MAT, X, Y) M_VALUE((MAT), unsigned char, X, Y, 0)

// 8�r�b�g3�`�����l���摜 (CV_8UC3)
#define MBC(MAT, X, Y) M_VALUE((MAT), unsigned char, X, Y, 0)
#define MGC(MAT, X, Y) M_VALUE((MAT), unsigned char, X, Y, 1)
#define MRC(MAT, X, Y) M_VALUE((MAT), unsigned char, X, Y, 2)

// 16�r�b�g����1�`�����l���摜 (CV_16U) ��������16�r�b�g����(short) 
#define MVS(IMG, X, Y) M_VALUE(IMG, short, X, Y, 0)

// 16�r�b�g3�`�����l���摜 (CV_16SC3) ��������16�r�b�g����(short) 
#define MBS(MAT, X, Y) M_VALUE((MAT), short, X, Y, 0)
#define MGS(MAT, X, Y) M_VALUE((MAT), short, X, Y, 1)
#define MRS(MAT, X, Y) M_VALUE((MAT), short, X, Y, 2)

// 32�r�b�g���������_1�`�����l���摜 (CV_32FC1)
#define MVF(MAT, X, Y) M_VALUE((MAT), float, X, Y, 0)

// 32�r�b�g���������_3�`�����l���摜 (CV_32FC3)
#define MBF(MAT, X, Y) M_VALUE((MAT), float, X, Y, 0)
#define MGF(MAT, X, Y) M_VALUE((MAT), float, X, Y, 1)
#define MRF(MAT, X, Y) M_VALUE((MAT), float, X, Y, 2)

/***** �摜�̃s�N�Z���̒��ڑ��� OpenCV-1.x (IplImage* ) *****/

// �ėp�}�N��
// IMG: IplImage*, TYPE: �^, X,Y: ���W, ID: �C���f�b�N�X
// �^
// IPL_DEPTH_8U: unsigned char
// IPL_DEPTH_16S: short
// IPL_DEPTH_
#define I_VALUE(IMG, TYPE, X, Y, ID) ((TYPE*)((IMG)->imageData + (IMG)->widthStep*(Y)))[(X)*(IMG)->nChannels+(ID)]

// �悭�g���摜�p�}�N��
// 8�r�b�g1�`�����l���摜 (IPL_DEPTH_8U, 1)
#define IVC(IMG, X, Y) I_VALUE(IMG, unsigned char, X, Y, 0)

// 8�r�b�g3�`�����l���摜 (IPL_DEPTH_8U, 3)
#define IBC(IMG, X, Y) I_VALUE(IMG, unsigned char, X, Y, 0)
#define IGC(IMG, X, Y) I_VALUE(IMG, unsigned char, X, Y, 1)
#define IRC(IMG, X, Y) I_VALUE(IMG, unsigned char, X, Y, 2)

// 16�r�b�g����1�`�����l���摜 (IPL_DEPTH_16S, 1) ��������16�r�b�g����(short) 
#define IVS(IMG, X, Y) I_VALUE(IMG, short, X, Y, 0)

// 16�r�b�g3�`�����l���摜 (IPL_DEPTH_16S, 3) ��������16�r�b�g����(short) 
#define IBS(IMG, X, Y) I_VALUE(IMG, short, X, Y, 0)
#define IGS(IMG, X, Y) I_VALUE(IMG, short, X, Y, 1)
#define IRS(IMG, X, Y) I_VALUE(IMG, short, X, Y, 2)

// 32�r�b�g���������_1�`�����l���摜 (IPL_DEPTH_32F, 1)
#define IVF(IMG, X, Y) I_VALUE(IMG, float, X, Y, 0)

// 32�r�b�g���������_3�`�����l���摜 (IPL_DEPTH_32F, 3)
#define IBF(IMG, X, Y) I_VALUE(IMG, float, X, Y, 0)
#define IGF(IMG, X, Y) I_VALUE(IMG, float, X, Y, 1)
#define IRF(IMG, X, Y) I_VALUE(IMG, float, X, Y, 2)

/***** �s��̗v�f�̒��ڑ��� *****/
// �s:ROW, ��: COL
// 32�r�b�g���������_1�`�����l���s�� (CV_32FC1)
#define M1F1(MAT, ROW, COL) ((float*)((MAT)->data.ptr + (MAT)->step*(ROW)))[(COL)]

// 32�r�b�g���������_2�`�����l���s�� (CV_32FC2)
#define M2F1(MAT, ROW, COL) ((float*)((MAT)->data.ptr + (MAT)->step*(ROW)))[(COL)*2+0]
#define M2F2(MAT, ROW, COL) ((float*)((MAT)->data.ptr + (MAT)->step*(ROW)))[(COL)*2+1] 

// 32�r�b�g���������_3�`�����l���s�� (CV_32FC3)
#define M3F1(MAT, ROW, COL) ((float*)(((MAT)->data).ptr + (MAT)->step*(ROW)))[(COL)*3+0]
#define M3F2(MAT, ROW, COL) ((float*)(((MAT)->data).ptr + (MAT)->step*(ROW)))[(COL)*3+1]
#define M3F3(MAT, ROW, COL) ((float*)(((MAT)->data).ptr + (MAT)->step*(ROW)))[(COL)*3+2]

// 64�r�b�g���������_1�`�����l���s�� (CV_32FC1)
#define M1D1(MAT, ROW, COL) ((double*)((MAT)->data.ptr + (MAT)->step*(ROW)))[(COL)]

// 64�r�b�g���������_2�`�����l���s�� (CV_64FC2)
#define M2D1(MAT, ROW, COL) ((double*)((MAT)->data.ptr + (MAT)->step*(ROW)))[(COL)*2+0]
#define M2D2(MAT, ROW, COL) ((double*)((MAT)->data.ptr + (MAT)->step*(ROW)))[(COL)*2+1] 

// 64�r�b�g���������_3�`�����l���s�� (CV_64FC3)
#define M3D1(MAT, ROW, COL) ((double*)((MAT)->data.ptr + (MAT)->step*(ROW)))[(COL)*3+0]
#define M3D2(MAT, ROW, COL) ((double*)((MAT)->data.ptr + (MAT)->step*(ROW)))[(COL)*3+1]
#define M3D3(MAT, ROW, COL) ((double*)((MAT)->data.ptr + (MAT)->step*(ROW)))[(COL)*3+2]