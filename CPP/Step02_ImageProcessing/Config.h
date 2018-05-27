/*! @file		Config.h
    @brief		�p�����[�^�Ȃǂ̐ݒ�������t�@�C��
	@author		Ken'ichi Morooka
	@date		2012.05.14
	@version	2.0.0
*/

#ifndef __CONFIG__
#define __CONFIG__

#define		TrainingSampleNum	2000		/*!< �w�K�T���v���̑��� */
#define		TestSampleNum		10000	/*!< �e�X�g�T���v���̑��� */
#define		ClassNum			10		/*!< �N���X���i����̉��K�ł�10�j */
#define		ImageSize			28		/*!< �摜�T�C�Y�i����̉��K�ł͕��C�����Ƃ���28�j*/

/*!
	@brief	�ǂݍ��ފw�K�T���v���摜�ւ̃p�X�ƃt�@�C�����e���v���[�g
	@n		%d-%04d�̑O�����N���X���x���C�㔼���摜�ԍ��ɑΉ�����
	@n		��F�N���X���x��5��10�Ԗڂ̉摜��"5-0010.pgm"�ƂȂ�  
*/
const char TrainingDataFile[] = "../../Images/TrainingSamples/%d-%04d.pgm";

/*!
	@brief	�ǂݍ��ރe�X�g�T���v���摜�ւ̃p�X�ƃt�@�C�����e���v���[�g
	@n		%d-%04d�̑O�����N���X���x���C�㔼���摜�ԍ��ɑΉ�����
	@n		��F�N���X���x��5��10�Ԗڂ̉摜��"5-0010.pgm"�ƂȂ�  
*/
const char TestDataFile[] = "../Images/TestSamples/%d-%04d.pgm";

#endif//__CONFIG__
