#pragma once
#include <opencv2/opencv.hpp>
#include <iostream>
#include <vector>
#include <Windows.h>
using namespace std;
using namespace cv;

class Element
{
public:
	Element();
	int get_end_frame();
	virtual void draw(int frame_id, Mat& frame);

protected:
	int begin_frame = -1;
	int end_frame = -1;
};

class Images:public Element {
public:
	Images(vector <Mat> images, int begin_frame, int fps);
	void draw(int frame_id, Mat& frame);
private:
	int pic_show_time = -1; // 3s per image
	int pic_switch_time = -1; // 0.5s for switch

	vector <Mat> images;

	// play begin and end
	int fps;
};

class Input_Vedio:public Element{
public:
	Input_Vedio(VideoCapture vedio_cap, int begin_frame);
	void draw(int frame_id, Mat& frame);
private:
	VideoCapture vedio_cap;

	// play begin and end
	int fps;
	int begin_switchtimes;
};

class BackGround :Element {
public:
	BackGround(int begin_frame, int fps);
	void draw(int frame_id, Mat& frame);
	void setEndFrame(int end_frame);
private:
	int fps;
	Mat background;
};

class Ball : public Element {
public:
	Ball(int begin_frame, int fps);
	void draw(int frame_id, Mat& frame);
private:
	int fps;
	int lowest_frame_id;
	int middle_frame_id;
	int middle_frame_down;
	int middle_frame_up;
	int change_shape_times;

	int low_pos_w = -1;
	int low_pos_h = -1;
	int last_pos_w = -1;
	int last_pos_h = -1;
};

class Text : public Element {
public:
	Text(int begin_frame, int fps);
	void draw(int frame_id, Mat& frame);
	void setEnd(int end_frame);
private:
	int fps;
	int begin_switchtimes;
};

class Vedio
{
public:
	// init all element, get all pic, get avi
	int init_vedio(char *path);
	
	// generate
	void generate();

private:
	// save all element
	vector<Element *> elements;
	void add_all_element();
	void read_all_image_video(vector < Mat>& temp_images, char *& path);

	// save all images
	vector <Mat> images;
	VideoCapture vedio_cap;
	
	// arguments of vedio 
	int fps = -1;
	int frameH = -1;
	int frameW = -1;
	int frame_num = -1;
};

bool isImgae(char *filename);
bool isVedio(char *filename);