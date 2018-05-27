/*! @file		RecognitionResult.h
    @brief		認識結果を保存する構造体を定義したファイル
	@author		Ken'ichi Morooka
	@date		2012.05.14
	@version	2.0.0
*/

#ifndef __RECOGNITIONRESULT__
#define __RECOGNITIONRESULT__

/*!
	@brief	認識結果を保存するファイル名テンプレート
	@n		%05d-%05dの前半が学習サンプル数，後半がテストサンプル数に対応する
	@n		例：学習サンプル数100，テストサンプル数1000の場合は"result-training_00100-test_01000.txt"となる  
*/
const char RecognitionResultFile[] = "result-training_%05d-test_%05d.txt";

/*!
  @struct		RecognitionResult
  @brief		認識結果を保存するための構造体
*/
typedef struct RecognitionResult {
	int class_num;			/*!< クラス数 */

	/*!
	@brief	混同対照表（Confusion Matrix）
	@n		res[認識してほしいクラスラベル][認識した結果のクラスラベル]
	@n		例：res[4][9]=10の場合，クラス4と認識すべきサンプルをクラス9として認識したものが10個あったということ  
	@n		例：res[4][4]=10の場合，クラス4と認識すべきサンプルをクラス4として認識したものが10個あったということなのでこの場合正しく認識されたサンプルが10個あったということになる
	*/
	int **res;				/*!< 混同対照表（Confusion Matrix） */
	double *ratio;			/*!< 各クラスの認識率 */
	double average_ratio;	/*!< 平均認識率 */

	/*!
	  @fn		RecognitionResult(int _class_num)
	  @brief	コンストラクタ
	  @param	[in] _class_num	クラス数
	*/
	RecognitionResult(int _class_num)
	{
		int i, j;
		class_num = _class_num;
		res = new int*[class_num];
		for (i=0; i<class_num; i++){
			res[i] = new int [class_num];
		}
		for (i=0; i<class_num; i++){
			for (j=0; j<class_num; j++){
				res[i][j] = 0;
			}
		}

		ratio = new double [class_num];
		for (i=0; i<class_num; i++){
			ratio[i] = 0.0;
		}

		average_ratio = 0.0;
	
	}

	/*!
	  @fn		~RecognitionResult()
	  @brief	デストラクタ
	*/
	~RecognitionResult()
	{
		int i;
		for (i=0; i<class_num; i++){
			delete [] res[i];
		}		
		delete [] res;
	}

	/*!
	  @fn		void CalcRatio()
	  @brief	認識率を計算する
	*/
	void CalcRatio()
	{
		int i, j;
		int correct, total;

		for (i=0; i<class_num; i++){
			total = 0;
			for (j=0; j<class_num; j++){
				total += res[i][j];
				if (i==j){
					correct = res[i][j];
				}
			}
			ratio[i] = (double)correct/total;
			average_ratio += ratio[i];
		}

		average_ratio /= class_num;
	}

	/*!
	  @fn		void PrintRecognitionResult()
	  @brief	認識率を標準出力に表示する
	*/
	void PrintRecognitionResult()
	{
		int i, j;

		printf("\n\n");
		for (i=0; i<class_num; i++){
			printf("\tC%d", i);	
		}
		printf("\n");
		for (i=0; i<class_num; i++){
			printf("C%d", i);	
			for (j=0; j<class_num; j++){
				printf("\t%4d", res[i][j]);	
			}
			printf("\n");
		}		
		printf("\nRecognition Ratio\n");
		for (i=0; i<class_num; i++){
			printf("Class %d\t%3.2lf\n", i, ratio[i]*100);	
		}
		printf("Average\t%2.2lf\n",average_ratio*100);	
		
	}

	/*!
	  @fn		void SaveRecognitionResult(char *filename)
	  @brief	認識率をファイルに出力する
	  @param	[in] *filename	ファイル名へのポインタ
	*/
	void SaveRecognitionResult(char *filename)
	{
		int i, j;
		FILE *fp;

		if(fopen_s( &fp, filename, "w" ) != 0){
			printf("The file \"%s\" was not opened.", filename);
			return;
		}

		for (i=0; i<class_num; i++){
			fprintf(fp, "\tC%d", i);	
		}
		fprintf(fp, "\n");

		for (i=0; i<class_num; i++){
			fprintf(fp, "C%d", i);	
			for (j=0; j<class_num; j++){
				fprintf(fp, "\t%4d", res[i][j]);	
			}
			fprintf(fp, "\n");
		}		
		fprintf(fp, "\nRecognition Ratio\n");
		for (i=0; i<class_num; i++){
			fprintf(fp, "Class %d\t%3.2lf\n", i, ratio[i]*100);	
		}
		fprintf(fp, "Average\t%2.2lf\n",average_ratio*100);	

		fclose(fp);		
	}

} RecognitionResult;

#endif//__RECOGNITIONRESULT__