/*! @file		main.cpp
    @brief		Step05_kNearestNeighborプロジェクトのメインファイル
	@n			k近傍法を使って学習サンプル画像とテストサンプル画像間のL1距離を計算して文字認識をする
	@n			基本的なパターン認識方法を習得する
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

				// k+1番目～K_MAX-1番目までの要素の更新
				for (int n=K_MAX-1; n>k; n--) {
					k_label[n] = k_label[n-1];
					k_dist[n] = k_dist[n-1]; 
				}
				// k番目の要素の更新
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

	printf("Mrk\n");
	delete result;
	delete img;

	return 1;
}

