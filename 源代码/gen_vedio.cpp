#include <opencv2/opencv.hpp>
#include <iostream>
#include "Vedio.h"
using namespace std;
using namespace cv;

int main(int argc, char **argv)
{
	Vedio vedio;
	
	if (argc != 2) {
		printf("Usage: %s <directory-path>", argv[0]);
		return 0;
	}
	char path[256];
	strcpy(path, argv[1]);
	int res = vedio.init_vedio(path);
	if (res == 0) {
		vedio.generate();
	}
	system("pause");
	return 0;
}
