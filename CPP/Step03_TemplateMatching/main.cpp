/*! @file		main.cpp
    @brief		Step03_TemplateMatchingプロジェクトのメインファイル
	@n			学習サンプル画像とテストサンプル画像間のL1距離を計算して文字認識をする
	@n			基本的なパターン認識方法を習得する
	@author		Ken'ichi Morooka
	@date		2012.05.14
	@version	2.0.0
*/

#include <stdlib.h>
#include "Config.h"
#include "PgmIO.h"
#include "RecognitionResult.h"

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
	delete tmp;
}


/*!
  @fn		int CalcL1Distance(PgmImage *img1, PgmImage *img2)
  @brief	2枚の画像間のL1距離を計算する
  @param	[in]	*img1	1枚目の画像
  @param	[in]	*img2	2枚目の画像
  @return	L1距離値
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
  @brief	学習用画像を読み込む
  @param	[out]	**tmp	テンプレート画像を学習サンプルから読み出して格納するメモリへのポインタ
  @n		読み出す画像が1枚とは限らないので2次元ポインタにしている
  @return	1:すべてのテンプレートの読み出しに成功
  @n		0:テンプレートの読み出しに失敗
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
  @brief	入力画像に対して最もL1距離が小さい学習サンプルのクラスラベルを調べる
  @param	[in]	*img	入力画像
  @param	[in]	**tmp	学習サンプルを読み込んだテンプレート画像群
  @return	L1距離が最も小さかったクラスラベル
  @n		-1:失敗
*/
int ReturnMatchLabel(PgmImage *img, PgmImage **tmp)
{
	int match_label = -1;
	int min_dis = INT_MAX;
	for (int i=0; i<TrainingSampleNum; i++){
		int dis = CalcL1Distance(img, tmp[i]);
		if (dis < min_dis){
			min_dis = dis;
			match_label = tmp[i]->label;
		}
	}

	return match_label;
}

/*!
  @fn		int TemplateMatching(PgmImage **tmp)
  @brief	メイン関数から呼び出すテンプレートマッチング用関数
  @n		テストサンプルを1枚ずつ読み出してマッチング結果を得る
  @n		マッチング結果は結果格納用の構造体に記録
  @param	[in]	**tmp	学習サンプルを読み込んだテンプレート画像群
  @return	1:すべてのテストサンプルのテンプレートマッチングに成功（認識率100%という意味ではない！）
  @n		0:テストサンプルの読み込みに失敗
*/
int TemplateMatching(PgmImage **tmp)
{
	char filename[256];
	PgmImage *img = new PgmImage(ImageSize, ImageSize);
	RecognitionResult *result = new RecognitionResult(ClassNum);

	int each_class_num = TestSampleNum/ClassNum;	//各クラスのサンプル数を算出
	int match_label = -1;

	for (int label=0; label<ClassNum; label++){
		for (int sample=0; sample<each_class_num; sample++){
			sprintf_s(filename, 128, TestDataFile, label, sample);
			printf("\rLoading the file %s\n", filename);
			if (!LoadPgmImage(filename, img, label)){	//テストサンプルの読み込み
				return 0;
			}
			match_label = ReturnMatchLabel(img, tmp);	//テストサンプルにマッチしたクラスラベルを得る
			result->res[img->label][match_label] ++;	//混同対照表の該当部分をインクリメントする
		}
	}

	result->CalcRatio();								//認識率を計算
	result->PrintRecognitionResult();					//認識率を表示
	sprintf_s(filename, 128, RecognitionResultFile, TrainingSampleNum, TestSampleNum);
	result->SaveRecognitionResult(filename);			//認識率を保存

	delete result;
	delete img;

	return 1;
}

