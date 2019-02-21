# CVGenerateVedio
Apply OpenCv to generate vedio 

### Description ###

This project is to generate a vedio from existed vedio and pictures by followed steps:
+ Add a generated vedio 
+ Add all pictures of `Input Directory`
+ Add an `avi` vedio of `Input Directory`

### Preparement ###

+ OpenCV 3.2.0
+ VS2017

### Install ###

+ Use VS2017 to generate execute file.
+ Run it in terminal:   `gen_vedio.exe  <Directory>`

### Example ###

Example input and output is in fold `Example`

##### Run Exmaple ####
+ `git clone https://github.com/ItemZheng/CVGenerateVedio`
+ Different OS:
    + Windows:
        + `cd CVGenerateVedio/EXE`
        + `gen_vedio.exe ../Example/Input`
    + Mac:
        + generate execute file `gen_vedio`
        + `gen_vedio <project_directory>/Example/Input`

##### Input #####

5 pitures and 1 avi vedio in `Example/Input`

##### Output Vedio #####

<video id="video" controls="" preload="none" poster="http://media.w3.org/2010/05/sintel/poster.png">
  <source id="mp4" src="https://github.com/ItemZheng/CVGenerateVedio/Example/GeneratedVedio/output.avi" type="video/avi">
</video>

##### Gifs #####

片头动画
图片切换：
![avatar](https://github.com/ItemZheng/CVGenerateVedio/blob/master/Example/gif/%E7%89%87%E5%A4%B4%E5%8A%A8%E7%94%BB.gif)

字幕移动渐变，图片淡入
![avatar](https://github.com/ItemZheng/CVGenerateVedio/blob/master/Example/gif/%E5%AD%97%E5%B9%95%E7%A7%BB%E5%8A%A8%E6%B8%90%E5%8F%98_%E5%9B%BE%E7%89%87%E6%B7%A1%E5%85%A5.gif)

图片切换：
![avatar](https://github.com/ItemZheng/CVGenerateVedio/blob/master/Example/gif/%E5%9B%BE%E7%89%87%E5%88%87%E6%8D%A2.gif)

图片淡出，视频淡入
![avatar](https://github.com/ItemZheng/CVGenerateVedio/blob/master/Example/gif/%E5%9B%BE%E7%89%87%E6%B7%A1%E5%87%BA_%E8%A7%86%E5%B1%8F%E6%B7%A1%E5%85%A5.gif)