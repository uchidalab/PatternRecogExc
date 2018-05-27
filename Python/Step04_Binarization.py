
# coding: utf-8

# ## Step04_Binarization
# 画像の読み込み→画像を２値化→保存

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
OutFile = './Images/OutSamples/bin_{0:1d}-{1:04d}.png'


# In[ ]:


# Binarization ルーチン
def Binarization ( src, thres ):
    '''
    ここでは画素の操作方法がわかるようにあえて２重ループで書いている．
    '''
    
    dest = np.zeros (src.shape, dtype=np.uint8)
    for y in range (0, src.shape[0] ):
        for x in range ( 0, src.shape[1]):
            if src[y,x] < thres :
                dest[y,x] = 255
            else :
                dest[y,x] = 0

    return dest


# In[ ]:


# main ルーチン

for label in range (0, ClassNum):
    for sample in range (0, TrainingSampleNum // ClassNum ):
        filename = TrainingDataFile.format(label,sample)
        print ("Loading the file: " + filename )
        img = io.imread ( filename )
        
        res = Binarization (img, 200)
            
        filename = OutFile.format(label, sample)
        print ("Saving the file: " + filename )
        io.imsave ( filename, res )
        

