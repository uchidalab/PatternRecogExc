/*! @file		PgmIO.h
    @brief		PGM形式の画像を入出力するためのヘッダファイル
	@author		Ken'ichi Morooka
	@date		2012.05.14
	@version	2.0.0
*/

#ifndef __PGMIO__
#define __PGMIO__

#include <stdio.h>

/*!
  @struct		PgmImage
  @brief		PGM画像を格納するための構造体
*/
typedef struct PgmImage {
	int width;				/*!< 画像の幅 */
	int height;				/*!< 画像の高さ */
	int label;				/*!< 画像のクラスラベル */
	unsigned char **pixel;	/*!< 画像を格納するメモリへのポインタ */

	/*!
	  @fn		PgmImage(int _width, int _height)
	  @brief	コンストラクタ
	  @param	[in] _width		画像の幅
	  @param	[in] _height	画像の高さ
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
	  @brief	デストラクタ
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
  @brief	PGM画像を読み込む
  @param	[in] *filename	ファイル名へのポインタ
  @param	[in] *image		読み込んだ画像を格納するためのPgmImage構造体へのポインタ
  @param	[in] label		読み込む画像のクラスラベル（デフォルトではラベル無し(label=-1)）
  @return	1:画像の読み込みに成功
  @n		0:画像の読み込みに失敗
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

	fgets(buf,128,fp);		//マジックナンバーの読み取り
	fgets(buf,128,fp);		//画像サイズの読み取り
	fgets(buf,128,fp);		//最大輝度値の読み取り

	for(y=0; y<image->height; y++){
		fread(image->pixel[y], sizeof(unsigned char), image->width, fp);
	}

	image->label = label;

	fclose(fp);

	return 1;
}

/*!
  @fn		int SavePgmImage(char *filename, PgmImage *image)
  @brief	PGM画像として保存する
  @param	[in] *filename	ファイル名へのポインタ
  @param	[in] *image		保存する画像を格納した
  @return	1:画像の保存に成功
  @n		0:画像の保存に失敗
*/
int SavePgmImage(char *filename, PgmImage *image)
{
	FILE *fp;
	int y;

	if(fopen_s( &fp, filename, "wb" ) != 0){
		printf("The file \"%s\" was not opened.", filename);
		return 0;
	}

	fprintf(fp,"P5\n");									//マジックナンバーの書き込み
	fprintf(fp,"%d %d\n", image->width, image->height);	//画像サイズの書き込み
	fprintf(fp,"255\n");								//最大輝度値の書き込み
  
	for(y=0; y<image->height; y++){
		fwrite(image->pixel[y], sizeof(unsigned char), image->width, fp);
	}

	fclose(fp);

	return 1;
}

#endif//__PGMIO__