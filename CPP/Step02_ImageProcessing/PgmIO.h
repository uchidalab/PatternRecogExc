/*! @file		PgmIO.h
    @brief		PGM�`���̉摜����o�͂��邽�߂̃w�b�_�t�@�C��
	@author		Ken'ichi Morooka
	@date		2012.05.14
	@version	2.0.0
*/

#ifndef __PGMIO__
#define __PGMIO__

#include <stdio.h>

/*!
  @struct		PgmImage
  @brief		PGM�摜���i�[���邽�߂̍\����
*/
typedef struct PgmImage {
	int width;				/*!< �摜�̕� */
	int height;				/*!< �摜�̍��� */
	int label;				/*!< �摜�̃N���X���x�� */
	unsigned char **pixel;	/*!< �摜���i�[���郁�����ւ̃|�C���^ */

	/*!
	  @fn		PgmImage(int _width, int _height)
	  @brief	�R���X�g���N�^
	  @param	[in] _width		�摜�̕�
	  @param	[in] _height	�摜�̍���
	*/
	PgmImage(int _width, int _height)
	{
		int y;
		width = _width;
		height = _height;
		label = -1;
		pixel = new unsigned char*[height];
		for (y=0; y<height; y++){
			pixel[y] = new unsigned char[width];
		}
	}

	/*!
	  @fn		~PgmImage()
	  @brief	�f�X�g���N�^
	*/
	~PgmImage()
	{
		int y;
		for (y=0; y<height; y++){
			delete [] pixel[y];
		}		
		delete [] pixel;
	}

} PgmImage;

/*!
  @fn		int LoadPgmImage(char *filename, PgmImage *image, int label)
  @brief	PGM�摜��ǂݍ���
  @param	[in] *filename	�t�@�C�����ւ̃|�C���^
  @param	[in] *image		�ǂݍ��񂾉摜���i�[���邽�߂�PgmImage�\���̂ւ̃|�C���^
  @param	[in] label		�ǂݍ��މ摜�̃N���X���x���i�f�t�H���g�ł̓��x������(label=-1)�j
  @return	1:�摜�̓ǂݍ��݂ɐ���
  @n		0:�摜�̓ǂݍ��݂Ɏ��s
*/
int LoadPgmImage(char *filename, PgmImage *image, int label = -1)
{
	FILE *fp;
	char buf[128];
	int y;

	if(fopen_s( &fp, filename, "rb" ) != 0){
		printf("The file \"%s\" was not opened.", filename);
		return 0;
	}

	fgets(buf,128,fp);		//�}�W�b�N�i���o�[�̓ǂݎ��
	fgets(buf,128,fp);		//�摜�T�C�Y�̓ǂݎ��
	fgets(buf,128,fp);		//�ő�P�x�l�̓ǂݎ��

	for(y=0; y<image->height; y++){
		fread(image->pixel[y], sizeof(unsigned char), image->width, fp);
	}

	image->label = label;

	fclose(fp);

	return 1;
}

/*!
  @fn		int SavePgmImage(char *filename, PgmImage *image)
  @brief	PGM�摜�Ƃ��ĕۑ�����
  @param	[in] *filename	�t�@�C�����ւ̃|�C���^
  @param	[in] *image		�ۑ�����摜���i�[����
  @return	1:�摜�̕ۑ��ɐ���
  @n		0:�摜�̕ۑ��Ɏ��s
*/
int SavePgmImage(char *filename, PgmImage *image)
{
	FILE *fp;
	int y;

	if(fopen_s( &fp, filename, "wb" ) != 0){
		printf("The file \"%s\" was not opened.", filename);
		return 0;
	}

	fprintf(fp,"P5\n");									//�}�W�b�N�i���o�[�̏�������
	fprintf(fp,"%d %d\n", image->width, image->height);	//�摜�T�C�Y�̏�������
	fprintf(fp,"255\n");								//�ő�P�x�l�̏�������
  
	for(y=0; y<image->height; y++){
		fwrite(image->pixel[y], sizeof(unsigned char), image->width, fp);
	}

	fclose(fp);

	return 1;
}

#endif//__PGMIO__