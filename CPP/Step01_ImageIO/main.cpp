/*! @file		main.cpp
    @brief		Step01_ImageIO�v���W�F�N�g�̃��C���t�@�C��
	@n			Config.h���Ŏw�肵�Ă���PGM�摜��ǂݍ���ŁC�J�����g�t�H���_�ɕۑ�����
	@n			�摜�̓ǂݍ��ݕ��@�ƕۑ����@���K������
	@author		Ken'ichi Morooka
	@date		2012.05.14
	@version	2.0.0
*/

#include "Config.h"
#include "PgmIO.h"

void main()
{
	char filename[128];
	
	//PGM�摜���i�[���郁�����̊m��
	PgmImage *img = new PgmImage(ImageSize, ImageSize);

	//�e�N���X�̉摜�������v�Z����
	int each_class_num = TrainingSampleNum/ClassNum;
	for (int label=0; label<ClassNum; label++){
		for (int sample=0; sample<each_class_num; sample++){
			sprintf_s(filename, sizeof(filename), TrainingDataFile, label, sample);
			printf("Loading the file %s\n", filename);
			if (!LoadPgmImage(filename, img, label)){ //�摜�̓ǂݍ���
				return;
			} else {
				sprintf_s(filename, sizeof(filename), "save_test_%d-%04d.pgm", label, sample);
				if (!SavePgmImage(filename, img)){ //�摜�̕ۑ�
					return;
				}
			}
		}
	}

	delete img;
}

