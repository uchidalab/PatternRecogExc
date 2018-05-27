
# coding: utf-8

# ## Step01_ImageIO
# 
# 画像の読み込み→表示→保存

# In[ ]:


from skimage import io


# In[ ]:


TrainingSampleNum = 2000 # 学習サンプル総数
TestSampleNum = 10000 # テストサンプル総数
ClassNum = 10 # クラス数（今回は10）
ImageSize = 28 # 画像サイズ（今回は縦横ともに28）
TrainingDataFile = './Images/TrainingSamples/{0:1d}-{1:04d}.png'
TestDataFile = './Images/TestSamples/{0:1d}-{1:04d}.png'
OutFile = './Images/OutSamples/save_{0:1d}-{1:04d}.png'


# In[ ]:


# main ルーチン

for label in range (0, ClassNum):
    for sample in range (0, TrainingSampleNum // ClassNum ):
        filename = TrainingDataFile.format(label,sample)
        print ("Loading the file: " + filename )
        img = io.imread ( filename )

        filename = OutFile.format(label, sample)
        print ("Saving the file: " + filename )
        io.imsave ( filename, img )
        

