/*! @file		main.cpp
    @brief		Step04_Binarizationプロジェクトのメインファイル
	@n			画像を読み込んで2値化し保存する
	@n			画素へのアクセス方法を習得する
	@author		Ken'ichi Morooka
	@date		2012.05.29
	@version	2.0.0
*/

#include "Config.h"
#include "PgmIO.h"

void Binarization(PgmImage *src, PgmImage *dst);

void main()
{
	char filename[128];
	
	//PGM画像を格納するメモリの確保
	PgmImage *img = new PgmImage(ImageSize, ImageSize); //入力画像
	PgmImage *res = new PgmImage(ImageSize, ImageSize); //結果画像

	//各クラスの画像枚数を計算する
	int each_class_num = TrainingSampleNum/ClassNum;
	for (int label=0; label<ClassNum; label++){
		for (int sample=0; sample<each_class_num; sample++){
			sprintf_s(filename, sizeof(filename), TrainingDataFile, label, sample);
			printf("Loading the file %s\n", filename);
			if (!LoadPgmImage(filename, img, label)){ //画像の読み込み
				return;
			} else {

				Binarization(img, res);//２値化処理を行う

				sprintf_s(filename, sizeof(filename), "binary_%d-%04d.pgm", label, sample);
				if (!SavePgmImage(filename, res)){ //結果画像の保存
					return;
				}
			}
		}
	}

	delete img;
	delete res;
}

/*!
  @fn		void Binarization(PgmImage *src, PgmImage *dst)
  @brief	輝度値を半分にする
  @param	[in]	*src	変換元の画像を格納するメモリへのポインタ
  @param	[out]	*dst	変換後の画像を格納するメモリへのポインタ
*/
void Binarization(PgmImage *src, PgmImage *dst)
{
	int th_val = 200;

	for (int y=0; y<src->height; y++){
		for (int x=0; x<src->width; x++){

			if (src->pixel[y][x] < th_val)
				dst->pixel[y][x] = 255;
			else
				dst->pixel[y][x] = 0;				
		}
	}
}