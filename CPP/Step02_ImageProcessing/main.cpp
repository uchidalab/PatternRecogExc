/*! @file		main.cpp
    @brief		Step02_ImageProcessingプロジェクトのメインファイル
	@n			画像を読み込んで輝度値を変更して保存する
	@n			画素へのアクセス方法を習得する
	@author		Ken'ichi Morooka
	@date		2012.05.14
	@version	2.0.0
*/

#include "Config.h"
#include "PgmIO.h"

void ImageProcessing(PgmImage *src, PgmImage *dst);

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

				ImageProcessing(img, res);//画像処理で輝度値を半分にする

				sprintf_s(filename, sizeof(filename), "gray_%d-%04d.pgm", label, sample);
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
  @fn		void ImageProcessing(PgmImage *src, PgmImage *dst)
  @brief	輝度値を半分にする
  @param	[in]	*src	変換元の画像を格納するメモリへのポインタ
  @param	[out]	*dst	変換後の画像を格納するメモリへのポインタ
*/
void ImageProcessing(PgmImage *src, PgmImage *dst)
{
	for (int y=0; y<src->height; y++){
		for (int x=0; x<src->width; x++){
			//変換元画像の画素にアクセスして値を半分にして
			//変換後画像の同じ位置の画素に画素値を格納する
			dst->pixel[y][x] = src->pixel[y][x]/2;
		}
	}
}