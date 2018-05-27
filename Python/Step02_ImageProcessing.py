
# coding: utf-8

# ## Step02_ImageProcessing
# 
# 画像の読み込み→輝度値を変更→保存

# In[ ]:


from skimage import io
import numpy as np


# In[ ]:


TrainingSampleNum = 2000 # 学習サンプル総数
TestSampleNum = 10000 # テストサンプル総数
ClassNum = 10 # クラス数（今回は10）
ImageSize = 28 # 画像サイズ（今回は縦横ともに28）
TrainingDataFile = './Images/TrainingSamples/{0:1d}-{1:04d}.png'
TestDataFile = './Images/TestSamples/{0:1d}-{1:04d}.png'
OutFile = './Images/OutSamples/gray_{0:1d}-{1:04d}.png'


# In[ ]:


# ImageProcessing ルーチン
def ImageProcessing ( src ):
    '''
    ここでは画素の操作方法がわかるようにあえて２重ループで書いている．
    実際には以下の１行で同じ機能が実現できる．

    dest = src // 2
    '''
    
    dest = np.zeros (src.shape, dtype=np.uint8)
    for y in range (0, src.shape[0] ):
        for x in range ( 0, src.shape[1]):
            dest[y,x] = src[y,x] // 2

    return dest


# In[ ]:


# main ルーチン

for label in range (0, ClassNum):
    for sample in range (0, TrainingSampleNum // ClassNum ):
        filename = TrainingDataFile.format(label,sample)
        print ("Loading the file: " + filename )
        img = io.imread ( filename )
        
        res = ImageProcessing ( img )
            
        filename = OutFile.format(label, sample)
        print ("Saving the file: " + filename )
        io.imsave ( filename, res )
        

