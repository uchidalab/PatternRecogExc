/*! @file		main.cpp
    @brief		Step05_kNearestNeighbor�v���W�F�N�g�̃��C���t�@�C��
	@n			k�ߖT�@���g���Ċw�K�T���v���摜�ƃe�X�g�T���v���摜�Ԃ�L1�������v�Z���ĕ����F��������
	@n			��{�I�ȃp�^�[���F�����@���K������
	@author		Ken'ichi Morooka
	@date		2012.06.13
	@version	1.0.0
*/

#include <stdlib.h>
#include "Config.h"
#include "PgmIO.h"
#include "RecognitionResult.h"

#define K_MAX 3

int LoadTemplateImages(PgmImage **tmp);
int TemplateMatching(PgmImage **tmp);

void main()
{
	PgmImage **tmp = new PgmImage*[TrainingSampleNum];
	for (int i=0; i<TrainingSampleNum; i++){
		tmp[i] = new PgmImage(ImageSize, ImageSize);
	}

	LoadTemplateImages(tmp);

	TemplateMatching(tmp);

	for (int i=0; i<TrainingSampleNum; i++){
		delete  tmp[i];
	}
	delete[] tmp;
}


/*!
  @fn		int CalcL1Distance(PgmImage *img1, PgmImage *img2)
  @brief	2���̉摜�Ԃ�L1�������v�Z����
  @param	[in]	*img1	1���ڂ̉摜
  @param	[in]	*img2	2���ڂ̉摜
  @return	L1�����l
*/
int CalcL1Distance(PgmImage *img1, PgmImage *img2)
{
	int dis = 0;
	int x, y;

	for  (y=0; y<img1->height; y++){
		for (x=0; x<img1->width; x++){
			dis += abs(img1->pixel[y][x] - img2->pixel[y][x]);
		}
	}

	return dis;
}

/*!
  @fn		int LoadTemplateImages(PgmImage **tmp)
  @brief	�w�K�p�摜��ǂݍ���
  @param	[out]	**tmp	�e���v���[�g�摜���w�K�T���v������ǂݏo���Ċi�[���郁�����ւ̃|�C���^
  @n		�ǂݏo���摜��1���Ƃ͌���Ȃ��̂�2�����|�C���^�ɂ��Ă���
  @return	1:���ׂẴe���v���[�g�̓ǂݏo���ɐ���
  @n		0:�e���v���[�g�̓ǂݏo���Ɏ��s
*/
int LoadTemplateImages(PgmImage **tmp)
{
	char filename[256];

	int img_no = 0;
	int each_class_num = TrainingSampleNum/ClassNum;
	for (int label=0; label<ClassNum; label++){
		for (int sample=0; sample<each_class_num; sample++){
			sprintf_s(filename, sizeof(filename), TrainingDataFile, label, sample);
			printf("\rLoading the file %s\n", filename);
			if (!LoadPgmImage(filename, tmp[img_no++], label)){
				return 0;
			}
		}
	}
	return 1;
}

/*!
  @fn		int ReturnMatchLabel(PgmImage *img, PgmImage **tmp)
  @brief	���͉摜�ɑ΂��čł�L1�������������w�K�T���v���̃N���X���x���𒲂ׂ�
  @param	[in]	*img	���͉摜
  @param	[in]	**tmp	�w�K�T���v����ǂݍ��񂾃e���v���[�g�摜�Q
  @return	L1�������ł������������N���X���x��
  @n		-1:���s
*/
int ReturnMatchLabel(PgmImage *img, PgmImage **tmp)
{
	int* k_label = new int[K_MAX];
	int* k_dist = new int[K_MAX];
	for (int k=0; k<K_MAX; k++) {
		k_label[k] = -1;
		k_dist[k] = INT_MAX;
	}

	for (int i=0; i<TrainingSampleNum; i++){
		int dis = CalcL1Distance(img, tmp[i]);

		for (int k=0; k<K_MAX; k++) {
			if (k_dist[k] > dis) {

				// k+1�Ԗځ`K_MAX-1�Ԗڂ܂ł̗v�f�̍X�V
				for (int n=K_MAX-1; n>k; n--) {
					k_label[n] = k_label[n-1];
					k_dist[n] = k_dist[n-1]; 
				}
				// k�Ԗڂ̗v�f�̍X�V
				k_label[k] = tmp[i]->label;
				k_dist[k] = dis;
				break;
			}
		}
	}

	int* class_cnt = new int[ClassNum];
	for (int i=0; i<ClassNum; i++)
		class_cnt[i] = 0;
	
	for (int i=0; i<K_MAX; i++) 
		class_cnt[k_label[i]]++;

	int match_label = -1;
	int max_count = 0;
	for (int i=0; i<ClassNum; i++) {
		if (max_count < class_cnt[i]) {
			max_count = class_cnt[i];
			match_label = i;
		}
	}

	delete [] k_label;
	delete [] k_dist;
	delete [] class_cnt;
	
	return match_label;
}


/*!
  @fn		int TemplateMatching(PgmImage **tmp)
  @brief	���C���֐�����Ăяo���e���v���[�g�}�b�`���O�p�֐�
  @n		�e�X�g�T���v����1�����ǂݏo���ă}�b�`���O���ʂ𓾂�
  @n		�}�b�`���O���ʂ͌��ʊi�[�p�̍\���̂ɋL�^
  @param	[in]	**tmp	�w�K�T���v����ǂݍ��񂾃e���v���[�g�摜�Q
  @return	1:���ׂẴe�X�g�T���v���̃e���v���[�g�}�b�`���O�ɐ����i�F����100%�Ƃ����Ӗ��ł͂Ȃ��I�j
  @n		0:�e�X�g�T���v���̓ǂݍ��݂Ɏ��s
*/
int TemplateMatching(PgmImage **tmp)
{
	char filename[256];
	PgmImage *img = new PgmImage(ImageSize, ImageSize);
	RecognitionResult *result = new RecognitionResult(ClassNum);

	int each_class_num = TestSampleNum/ClassNum;	//�e�N���X�̃T���v�������Z�o
	int match_label = -1;

	for (int label=0; label<ClassNum; label++){
		for (int sample=0; sample<each_class_num; sample++){
			sprintf_s(filename, 128, TestDataFile, label, sample);
			printf("\rLoading the file %s\n", filename);
			if (!LoadPgmImage(filename, img, label)){	//�e�X�g�T���v���̓ǂݍ���
				return 0;
			}
			match_label = ReturnMatchLabel(img, tmp);	//�e�X�g�T���v���Ƀ}�b�`�����N���X���x���𓾂�
			result->res[img->label][match_label] ++;	//�����Ώƕ\�̊Y���������C���N�������g����
		}
	}

	result->CalcRatio();								//�F�������v�Z
	result->PrintRecognitionResult();					//�F������\��
	sprintf_s(filename, 128, RecognitionResultFile, TrainingSampleNum, TestSampleNum);
	result->SaveRecognitionResult(filename);			//�F������ۑ�

	printf("Mrk\n");
	delete result;
	delete img;

	return 1;
}

