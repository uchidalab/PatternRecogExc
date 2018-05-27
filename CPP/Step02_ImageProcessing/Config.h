/*! @file		Config.h
    @brief		パラメータなどの設定を書くファイル
	@author		Ken'ichi Morooka
	@date		2012.05.14
	@version	2.0.0
*/

#ifndef __CONFIG__
#define __CONFIG__

#define		TrainingSampleNum	2000		/*!< 学習サンプルの総数 */
#define		TestSampleNum		10000	/*!< テストサンプルの総数 */
#define		ClassNum			10		/*!< クラス数（今回の演習では10） */
#define		ImageSize			28		/*!< 画像サイズ（今回の演習では幅，高さともに28）*/

/*!
	@brief	読み込む学習サンプル画像へのパスとファイル名テンプレート
	@n		%d-%04dの前半がクラスラベル，後半が画像番号に対応する
	@n		例：クラスラベル5の10番目の画像は"5-0010.pgm"となる  
*/
const char TrainingDataFile[] = "../../Images/TrainingSamples/%d-%04d.pgm";

/*!
	@brief	読み込むテストサンプル画像へのパスとファイル名テンプレート
	@n		%d-%04dの前半がクラスラベル，後半が画像番号に対応する
	@n		例：クラスラベル5の10番目の画像は"5-0010.pgm"となる  
*/
const char TestDataFile[] = "../Images/TestSamples/%d-%04d.pgm";

#endif//__CONFIG__
