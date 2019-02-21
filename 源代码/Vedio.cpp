#include "Vedio.h"

Mat animotion_last;

bool isImgae(char *filename) {
	// get length
	int len = strlen(filename);

	// valid image 
	char types[][5] = {".jpg", ".png", ".tif", ".bmp"};
	if (len > 4) {
		for (int i = 0; i < sizeof(types) / sizeof(types[0]); i++) {
			char temp[256];
			strncpy(temp, filename + len - 4, 4);
			temp[4] = '\0';

			// match
			if (strcmp(temp, types[i]) == 0) {
				return true;
			}
		}
	}
	return false;
}

bool isVedio(char *filename) {
	// get length
	int len = strlen(filename);

	// valid image 
	if (len > 4) {
		char temp[256];
		strncpy(temp, filename + len - 4, 4);
		temp[4] = '\0';

		// match
		if (strcmp(temp, ".avi") == 0) {
			return true;
		}
	}
	return false;
}

// init vedio from path
int Vedio::init_vedio(char * path)
{	
	// init
	elements.clear();
	// save ori size of images
	vector < Mat> temp_images;
	// read
	read_all_image_video(temp_images, path);
	
	// record the argument of vedio
	if (!vedio_cap.isOpened()) {
		printf("[ERROR] Can not find a vedio.\n");
		return -1;
	}
	fps		= (int)vedio_cap.get(CV_CAP_PROP_FPS);
	frameH	= (int)vedio_cap.get(CV_CAP_PROP_FRAME_HEIGHT);
	frameW	= (int)vedio_cap.get(CV_CAP_PROP_FRAME_WIDTH);
	printf("[INFO] Read fps: %d  frame height: %d  frame width: %d\n", fps, frameH, frameW);

	// resize all pictures
	for (vector<Mat>::iterator img = temp_images.begin(); img != temp_images.end(); img++) {
		// resize
		Mat resized_img;
		resize(*img, resized_img, Size(frameW, frameH));	
		images.push_back(resized_img);
	}
	printf("[INFO] Resize all image successfully!\n");

	// create all element
	add_all_element();
	return 0;
}

// generate
void Vedio::generate()
{
	// create avi writer
	VideoWriter writer;
	writer.open("output.avi", CV_FOURCC('M', 'P', '4', '2'), fps, Size(frameW, frameH), true);
	
	// display
	// namedWindow("video", CV_WINDOW_AUTOSIZE);
	printf("[INFO] Generating avi: ");
	int last = -1;
	for (int i = 0; i < frame_num; i++) {
		if (i * 80 / frame_num  > last) {
			printf(">");
			last = (int)i * 80 / frame_num;
		}

		Mat frame(frameH, frameW, CV_8UC3, Scalar(255, 255, 255));
		// draw every element
		for (vector<Element *>::iterator element = elements.begin(); element != elements.end(); element++) {
			(*element)->draw(i, frame);
		}
		writer.write(frame);
	}
	printf("Successfully\n");
	writer.release();
	// end
}

void Vedio::add_all_element()
{
	Ball *ball = new Ball(0, fps);
	elements.push_back(ball);

	Images *image_element = new Images(images, ball->get_end_frame() + fps * 2, fps);
	elements.push_back(image_element);

	Input_Vedio *input_vedio = new Input_Vedio(vedio_cap, image_element->get_end_frame());
	elements.push_back(input_vedio);

	Text *text = new Text(ball->get_end_frame(), fps);
	elements.push_back(text);

	frame_num = input_vedio->get_end_frame();
	text->setEnd(frame_num);

}

void Vedio::read_all_image_video(vector < Mat>& temp_images, char *& path)
{
	// find all picture and a vedio
	HANDLE handle;
	WIN32_FIND_DATA find_data;

	char temp_path[256];
	strcpy(temp_path, path);
	strcat(temp_path, "\\*.*");    // the path to be travel

	// begin traverse
	handle = FindFirstFileA(temp_path, &find_data);
	do {
		if (!(find_data.dwFileAttributes & FILE_ATTRIBUTE_DIRECTORY)) {
			// is a file
			char full_path[256];
			if (isImgae(find_data.cFileName)) {
				printf("[INFO] Find a image:\t%s\n", find_data.cFileName);
				strcpy(full_path, path);
				strcat(full_path, "\\");
				strcat(full_path, find_data.cFileName);
				Mat img = imread(full_path);
				// open fail
				if (img.empty()) {
					printf("[ERROR] Fail to load image:\t%s\n", find_data.cFileName);
				}
				else {
					temp_images.push_back(img);
				}
			}

			// is a vedio
			if (isVedio(find_data.cFileName) && !vedio_cap.isOpened()) {
				printf("[INFO] Find a vedio:\t%s\n", find_data.cFileName);
				strcpy(full_path, path);
				strcat(full_path, "\\");
				strcat(full_path, find_data.cFileName);
				vedio_cap.open(full_path);
				if (!vedio_cap.isOpened()) {
					printf("[ERROR] Fail to load vedio:\t%s\n", find_data.cFileName);
				}
			}
		}
	} while (FindNextFileA(handle, &find_data));
}

Element::Element(){}

void Element::draw(int frame_id, Mat& frame){

}

int Element::get_end_frame()
{
	return end_frame;
}

Images::Images(vector <Mat> images, int begin_frame, int fps)
{
	// init
	this->images = images;
	this->fps = fps;
	this->begin_frame = begin_frame;

	// calculate time to end
	this->pic_show_time = fps * 5 / 2;  // 2 seconds
	this->pic_switch_time = fps / 2;
	this->end_frame = this->begin_frame + pic_show_time * images.size();

}

void Images::draw(int frame_id, Mat& frame)
{
	if (frame_id >= begin_frame && frame_id < end_frame) {
		// get the pic id
		int pic_id = (frame_id - begin_frame) / pic_show_time;
		images[pic_id].copyTo(frame);

		// calculate the switch
		int pic_over = (frame_id - begin_frame) % pic_show_time;
		if (pic_show_time - pic_over <= pic_switch_time) {
			// calculate rate
			float p = (pic_over + pic_switch_time - pic_show_time) / (float)pic_switch_time;

			// get next img
			Mat next_img;
			if (pic_id != images.size() - 1) {
				next_img = images[pic_id + 1];
			}

			//
			for (int nrow = 0; nrow < frame.rows; nrow ++)
			{
				for (int ncol = 0; ncol < frame.cols; ncol++)
				{
					Vec3b bgr = frame.at<Vec3b>(nrow, ncol);
					
					// has next pic 
					if (next_img.empty()) {
						frame.at<Vec3b>(nrow, ncol) = Vec3b((int)(bgr.val[0] * (1 - p)), (int)(bgr.val[1] * (1 - p)), (int)(bgr.val[2] * (1 - p)));
					}else
					{
						// combine
						Vec3b next_bgr = next_img.at<Vec3b>(nrow, ncol);
						int b = (int)(bgr.val[0] * (1 - p) + next_bgr.val[0] * p);
						int g = (int)(bgr.val[1] * (1 - p) + next_bgr.val[1] * p);
						int r = (int)(bgr.val[2] * (1 - p) + next_bgr.val[2] * p);
						frame.at<Vec3b>(nrow, ncol) = Vec3b( b, g, r);
					}
					
				}
			}
		}
		else if(pic_id == 0 && pic_over < pic_switch_time){
			// first picture
			float p = pic_over / (float)pic_switch_time;
			for (int nrow = 0; nrow < frame.rows; nrow++)
			{
				for (int ncol = 0; ncol < frame.cols; ncol++)
				{
					Vec3b bgr = frame.at<Vec3b>(nrow, ncol);
					frame.at<Vec3b>(nrow, ncol) = Vec3b((int)(bgr.val[0] * p), (int)(bgr.val[1] * p), (int)(bgr.val[2] * p));
				}
			}
		}
		
	}
}

Input_Vedio::Input_Vedio(VideoCapture vedio_cap, int begin_frame)
{
	this->vedio_cap = vedio_cap;
	this->begin_frame = begin_frame;
	fps = (int)vedio_cap.get(CV_CAP_PROP_FPS);
	// end time
	this->end_frame = vedio_cap.get(CV_CAP_PROP_FRAME_COUNT) + begin_frame - 1;
	begin_switchtimes = fps / 2;  // 0.5 second to begin
}

void Input_Vedio::draw(int frame_id, Mat& frame)
{
	// for this to draw
	if (frame_id >= begin_frame && frame_id < end_frame) {
		Mat img;
		vedio_cap.read(img);
		img.copyTo(frame);
		// if begin 
		if (frame_id - begin_frame < begin_switchtimes) {
			// just begin  
			float p = (frame_id - begin_frame) / (float)begin_switchtimes;
			for (int nrow = 0; nrow < frame.rows; nrow++)
			{
				for (int ncol = 0; ncol < frame.cols; ncol++)
				{
					Vec3b bgr = frame.at<Vec3b>(nrow, ncol);
					frame.at<Vec3b>(nrow, ncol) = Vec3b((int)(bgr.val[0] * p), (int)(bgr.val[1] * p), (int)(bgr.val[2] * p));
				}
			}
		}
	}
}

BackGround::BackGround(int begin_frame, int fps)
{
	this->begin_frame = begin_frame;
}

void BackGround::draw(int frame_id, Mat & frame)
{
	
}

void BackGround::setEndFrame(int end_frame)
{
	this->end_frame = end_frame;
}

Ball::Ball(int begin_frame, int fps)
{
	// init
	this->begin_frame = begin_frame;
	this->fps = fps;
	change_shape_times = fps * 3 / 25;
	lowest_frame_id = begin_frame + fps;				// 1 s
	middle_frame_down = lowest_frame_id + change_shape_times;
	middle_frame_up = middle_frame_down + change_shape_times;
	middle_frame_id = middle_frame_up + fps;			// 1 s
	end_frame = middle_frame_id + fps * 3 / 2 + fps;
}

void Ball::draw(int frame_id, Mat & frame)
{
	// get the rows and cols
	int h = frame.rows;
	int w = frame.cols;

	// calculate
	int ball_a = -1, ball_b = -1, wave_a = -1, wave_b = -1;
	int pos_w = w / 2, pos_h = -1;
	
	if (frame_id >= begin_frame && frame_id < lowest_frame_id) {
		// get the a
		int t = lowest_frame_id - begin_frame;
		// 1/2 a*t*t = 3/4h
		float a = 3 * h / (2.0f * t * t);
		// get h
		int current_t = frame_id - begin_frame;
		pos_h = int(0.5 * a * current_t * current_t);

		// get ball_a, ball_b
		ball_a = h / 20;
		ball_b = ball_a / 2;
		// draw
		ellipse(frame, Point(pos_w, pos_h), Size(ball_b, ball_a), 0, 0, 360, Scalar(255, 0, 0), CV_FILLED);
		if (frame_id == lowest_frame_id - 1) {
			low_pos_h = pos_h;
			low_pos_w = pos_w;
		}
	}
	else if (frame_id >= lowest_frame_id && frame_id < middle_frame_down) {
		int offset = h / 40 * (frame_id - lowest_frame_id) / (middle_frame_down - lowest_frame_id);
		ball_a = h / 20 - offset;
		ball_b = h / 40 + offset;
		pos_h = low_pos_h + 2 * offset;
		// draw
		ellipse(frame, Point(pos_w, pos_h), Size(ball_b, ball_a), 0, 0, 360, Scalar(255, 0, 0), CV_FILLED);
	}
	else if (frame_id >= middle_frame_down && frame_id < middle_frame_up) {
		int offset = h / 40 * (frame_id - middle_frame_down) / (middle_frame_up - middle_frame_down);
		ball_a = h / 40 + offset;
		ball_b = h / 20 - offset;
		pos_h = low_pos_h + h / 20 - 2 * offset;
		// draw
		ellipse(frame, Point(pos_w, pos_h), Size(ball_b, ball_a), 0, 0, 360, Scalar(255, 0, 0), CV_FILLED);
	}
	else if (frame_id >= middle_frame_up && frame_id < middle_frame_id) {
		// get the a
		int t = middle_frame_id - middle_frame_up;
		// 1/2 a*t*t = 3/8 h
		float a = 3 * h / (4.0f * t * t);
		// get h
		int current_t = middle_frame_id - frame_id;
		pos_h = low_pos_h - (h * 3 / 8 - int(0.5 * a * current_t * current_t));

		// get ball_a, ball_b
		ball_a = h / 20;
		ball_b = ball_a / 2;
		if (frame_id >= middle_frame_id - change_shape_times) {
			int over = frame_id - middle_frame_id + change_shape_times;
			int offset = h * over / 40 / change_shape_times;
			ball_a -= offset;
		}
		// record last pos
		if (frame_id == middle_frame_id - 1) {
			last_pos_h = pos_h;
			last_pos_w = pos_w;
		}
		// draw
		ellipse(frame, Point(pos_w, pos_h), Size(ball_b, ball_a), 0, 0, 360, Scalar(255, 0, 0), CV_FILLED);
	}
	else if (frame_id >= middle_frame_id && frame_id < end_frame) {
		ball_a =  h / 40;
		ball_b = ball_a;
		pos_w = last_pos_w;
		pos_h = last_pos_h;
		//ellipse(frame, Point(pos_w, pos_h), Size(ball_b, ball_a), 0, 0, 360, Scalar(255, 0, 0), CV_FILLED);

		int total_time = end_frame - fps - middle_frame_id;
		int draw_angle = (frame_id - middle_frame_id + 1) * 360 / total_time;
		ellipse(frame, Point(pos_w, pos_h + h/5), Size(h/5, h/5), 270, 0, draw_angle, Scalar(255, 0, 0), ball_a * 5/4, CV_AA);

		int r = h / 5;
		int x = pos_w;
		int y = pos_h + h / 5;

		Point p1(x - r / 2, y - 1.732 * r / 2);
		Point p2(x + r, y);
		Point p3(x - r / 2, y + 1.732 * r / 2);

		if (0 <= frame_id - middle_frame_id && frame_id - middle_frame_id < total_time / 3) {
			Point begin = p1;
			Point end = p2;
			float off = (frame_id - middle_frame_id) * 3.0l / total_time;
			Point endP = begin + off * (end - begin);
			line(frame, begin, endP, Scalar(255, 0, 0), ball_a * 6 / 4, CV_AA);
		}else if (total_time / 3 <= frame_id - middle_frame_id && frame_id - middle_frame_id < total_time * 2 / 3) {
			line(frame, p1, p2, Scalar(255, 0, 0), ball_a * 6 / 4, CV_AA);

			Point begin = p2;
			Point end = p3;
			float off = (frame_id - middle_frame_id) * 3.0l / total_time - 1;
			Point endP = begin + off * (end - begin);
			line(frame, begin, endP, Scalar(255, 0, 0), ball_a * 6 / 4, CV_AA);
		}
		else if (total_time * 2 / 3 <= frame_id - middle_frame_id && frame_id - middle_frame_id < total_time) {
			line(frame, p1, p2, Scalar(255, 0, 0), ball_a * 6 / 4, CV_AA);
			line(frame, p2, p3, Scalar(255, 0, 0), ball_a * 6 / 4, CV_AA);

			Point begin = p3;
			Point end = p1;
			float off = (frame_id - middle_frame_id + 1) * 3.0l / total_time - 2;
			Point endP = begin + off * (end - begin);
			line(frame, begin, endP, Scalar(255, 0, 0), ball_a * 6 / 4, CV_AA);
		}
		else {
			line(frame, p1, p2, Scalar(255, 0, 0), ball_a * 6 / 4, CV_AA);
			line(frame, p2, p3, Scalar(255, 0, 0), ball_a * 6 / 4, CV_AA);
			line(frame, p3, p1, Scalar(255, 0, 0), ball_a * 6 / 4, CV_AA);
		}
		if (frame_id == end_frame - fps - 1) {
			frame.copyTo(animotion_last);
		}
	}
}

Text::Text(int begin_frame, int fps)
{
	this->fps = fps;
	this->begin_frame = begin_frame;
	begin_switchtimes = fps / 2;
}

void Text::draw(int frame_id, Mat & frame)
{
	// for this to draw
	if (frame_id >= end_frame) {
		return;
	}
	if (frame_id < begin_frame) {
		return;
	}
	// get the rows and cols
	int h = frame.rows;
	int w = frame.cols;
	if (frame_id >= begin_frame && frame_id < begin_frame + begin_switchtimes) {
		putText(frame, "Zhengyuhang 3160102332 @ Compution Vision", Point(w / 10, h / 2), FONT_HERSHEY_SIMPLEX,
			0.9, Scalar(220, 0, 0), h / 200, CV_AA);
		// if begin 
		float p = (frame_id - begin_frame) / (float)begin_switchtimes;
		for (int nrow = 0; nrow < frame.rows; nrow++)
		{
			for (int ncol = 0; ncol < frame.cols; ncol++)
			{
				Vec3b bgr = animotion_last.at<Vec3b>(nrow, ncol);
				Vec3b next_bgr = frame.at<Vec3b>(nrow, ncol);
				int b = (int)(bgr.val[0] * (1 - p) + next_bgr.val[0] * p);
				int g = (int)(bgr.val[1] * (1 - p) + next_bgr.val[1] * p);
				int r = (int)(bgr.val[2] * (1 - p) + next_bgr.val[2] * p);
				frame.at<Vec3b>(nrow, ncol) = Vec3b(b, g, r);
			}
		}
	}
	else if (frame_id >= begin_frame + begin_switchtimes && frame_id < begin_frame + begin_switchtimes + fps) {
		putText(frame, "Zhengyuhang 3160102332 @ Compution Vision", Point(w / 10, h / 2), FONT_HERSHEY_SIMPLEX,
			0.9, Scalar(220, 0, 0), h / 200, CV_AA);
	}
	else if (frame_id < begin_frame + begin_switchtimes * 2 + fps && frame_id >= begin_frame + begin_switchtimes + fps) {
		float p = (frame_id - (begin_frame + begin_switchtimes + fps)) / (float)begin_switchtimes;
		int tarh = 19 * h / 20;
		
		int current_h = h / 2 + p * (tarh - h / 2);
		int r = 220 * p;
		putText(frame, "Zhengyuhang 3160102332 @ Compution Vision", Point(w / 10, current_h), FONT_HERSHEY_SIMPLEX,
			0.9, Scalar(220, r, r), h / 200, CV_AA);
	}
	else if (frame_id >= begin_frame + begin_switchtimes && frame_id < end_frame - begin_switchtimes) {
		putText(frame, "Zhengyuhang 3160102332 @ Compution Vision", Point(w / 10, 19 * h / 20), FONT_HERSHEY_SIMPLEX,
			0.9, Scalar(220, 220, 220), h / 200, CV_AA);
	}
}

void Text::setEnd(int end_frame)
{
	this->end_frame = end_frame;
}
