#include "include/opencv/highgui.h"
#include "include/opencv/cv.h"
#include "include/opencv/cxcore.h"
#include <io.h>

using namespace std;
using namespace cv;

#ifdef _DEBUG
#pragma comment(lib, "opencv_core242d.lib")
#pragma comment(lib, "opencv_highgui242d.lib")		
#pragma comment(lib, "opencv_imgproc242d.lib")
#else
#pragma comment(lib, "opencv_core242.lib")
#pragma comment(lib, "opencv_highgui242.lib")		
#pragma comment(lib, "opencv_imgproc242.lib")
#endif

void list_dir(const string & path, vector<string> & files);
Mat getSal(Mat& img, Mat& depth);