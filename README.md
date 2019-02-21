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

