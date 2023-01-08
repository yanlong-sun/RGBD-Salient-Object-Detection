#include "acsd.h"
#include "SLIC.h"

void list_dir(const string & path, vector<string> & files)  
{  
	WIN32_FIND_DATA fd;  

	HANDLE hFind = ::FindFirstFile(path.c_str(), &fd);  
	if (hFind != INVALID_HANDLE_VALUE)  
	{  
		do  
		{  
			// filtering directory and hidden file  
			if (!(fd.dwFileAttributes & FILE_ATTRIBUTE_DIRECTORY) &&  
				!(fd.dwFileAttributes & FILE_ATTRIBUTE_HIDDEN))  
			{  

				files.push_back(fd.cFileName);  
			}  
		} while (::FindNextFile(hFind, &fd));  
	}  
	else  
	{  
		cout << "Error: cannot find the first file! Please copy the related folders (.\\image, .\\depth, .\\out) to the current directory." << endl; 
		system("pause");
	}  
	FindClose(hFind);  
}  

void DepthRefine(Mat& depth, Mat& sal)
{
	int* hist = new int[256];
	memset(hist, 0, sizeof(int)*256);
	int numPixels = depth.rows*depth.cols;
	for (int i=0; i<numPixels; ++i)
		++hist[depth.data[i]];

	int acchist = 0;
	int dTh;
	for (int i=255; i>=0; --i)
	{
		acchist += hist[i];
		if (double(acchist)/numPixels>0.5)			//top 50%
		{
			dTh = i;
			break;
		}
	}

	for (int i=0; i<numPixels; ++i)
	{
		float dp;
		if (depth.data[i]<dTh)
			dp = (float)depth.data[i]/dTh;
		else
			dp = 1.0f;
		sal.data[i] = unsigned char((float)sal.data[i]*dp);
	}

	delete hist;
}

void GaussianRefine(Mat& sal)
{
	int cx=0, cy=0, cnum=0;
	for (int i=0; i<sal.rows; ++i)
		for (int j=0; j<sal.cols; ++j)
		{
			cx += i;
			cy += j;
			++cnum;
		}
		cx /= cnum;
		cy /= cnum;

		int sigmax = sal.rows/2;
		int sigmay = sal.cols/2;

		for (int i=0; i<sal.rows; ++i)
			for (int j=0; j<sal.cols; ++j)
			{
				sal.data[i*sal.cols+j] = uchar(sal.data[i*sal.cols+j] * exp(-((double)i-cx)*(i-cx)/sigmax/sigmax-((double)j-cy)*(j-cy)/sigmay/sigmay));
			}
}

int getMinDepthDiff(Mat& img, Mat& depth, int ix, int iy, double angle, int arm, int step)
{
	double xstep,ystep;
	if (angle<0)			{ cerr<<"scan angle can't be negative!"<<endl; system("pause"); }
	else if (angle<=45)		{ xstep = -step; ystep = step*tan(angle/180*3.14159265359); }
	else if (angle<=90)		{ xstep = -step*tan((90-angle)/180*3.14159265359); ystep = step; }
	else if (angle<=135)	{ xstep = step*tan((angle-90)/180*3.14159265359); ystep = step; }
	else if (angle<=180)	{ xstep = step; ystep = step*tan((180-angle)/180*3.14159265359); }
	else if (angle<=225)	{ xstep = step; ystep = -step*tan((angle-180)/180*3.14159265359); }
	else if (angle<=270)	{ xstep = step*tan((270-angle)/180*3.14159265359); ystep = -step; }
	else if (angle<=315)	{ xstep = -step*tan((angle-270)/180*3.14159265359); ystep = -step; }
	else if (angle<=360)	{ xstep = -step; ystep = -step*tan((360-angle)/180*3.14159265359); }
	else					{ cerr<<"Angle can't be larger than 360 degree!"<<endl; system("pause"); }

	int px = ix;
	int py = iy;
	double accxstep=0,accystep=0;
	int mind = 255;
	while (px<depth.rows&&py<depth.cols&&px>=0&&py>=0&&sqrt(accxstep*accxstep+accystep*accystep)<arm)
	{
		int pd = int(depth.data[px*depth.cols+py]);
		if (pd<mind)
			mind = pd;
		accxstep += xstep;
		accystep += ystep;
		px = ix + (int)accxstep;
		py = iy + (int)accystep;
	}
	int dp = depth.data[ix*depth.cols+iy];
	return dp-mind>0?dp-mind:0;
}

Mat getACSD(Mat& img, Mat& depth, int* labels, int numlabels)
{
	int* cx = new int[numlabels];
	int* cy = new int[numlabels];
	int* numPixels = new int[numlabels];
	memset(cx, 0, sizeof(int)*numlabels);
	memset(cy, 0, sizeof(int)*numlabels);
	memset(numPixels, 0, sizeof(int)*numlabels);
	int nPix = 0;
	for (int i=0; i<depth.rows; ++i)
		for (int j=0; j<depth.cols; ++j)
		{
			++numPixels[labels[nPix]];
			cx[labels[nPix]] += i;
			cy[labels[nPix]] += j;

			++nPix;
		}

	for (int i=0; i<numlabels; ++i)
	{
		cx[i] /= numPixels[i];
		cy[i] /= numPixels[i];
	}

	int* acsd = new int[numlabels];
	memset(acsd, 0, sizeof(int)*numlabels);

	double numDirs = 8;					//number of scan directions, this parameter can be SET manually!!!
	double angstep = 360.0/numDirs;
	int scanLength = int(0.4*sqrt((double)depth.rows*depth.rows+depth.cols*depth.cols));		//set scan length

	for (int i=0; i<numlabels; ++i)
	{
		for (double ang=0; ang<360; ang+=angstep)
			acsd[i] += getMinDepthDiff(img, depth, cx[i], cy[i], ang, scanLength, 5);			//get the difference between centroid and minimum depth along each scan line
	}

	//normalization
	int maxSal = 0;
	for (int i=0; i<numlabels; ++i)
	{
		if (acsd[i]>maxSal)
			maxSal = acsd[i];
	}
	for (int i=0; i<numlabels; ++i)
	{
		acsd[i] = acsd[i]*255/maxSal;
	}

	//store to Mat
	Mat sal = Mat(depth.rows, depth.cols, CV_8UC1);
	nPix = depth.rows*depth.cols;
	for (int i=0; i<nPix; ++i)
		sal.data[i] = acsd[labels[i]];

	delete cx;
	delete cy;
	delete numPixels;
	delete acsd;

	return sal;
}

Mat getSal(Mat& img, Mat& depth)
{
	Mat sal;

	if (img.rows!=depth.rows||img.cols!=depth.cols)
	{
		cerr<<"Inconsistent resolution between color and depth image!"<<endl;
		return sal;
	}

	//super pixel segmentation using SLIC
	SLIC slic;
	int numPixels = img.rows*img.cols;
	int numSegments = int(sqrt((double)img.rows*img.rows+img.cols*img.cols));
	int numlabels(0);
	int* labels = new int[numPixels];
	slic.DoSuperpixelSegmentation_ForGivenNumberOfSuperpixels(img.data, img.cols, img.rows, labels, numlabels, numSegments, 20.0);

	//get saliency and perform refinement
	GaussianBlur(depth, depth, Size(7, 7), 1.75, 1.75);
	sal = getACSD(img, depth, labels, numlabels);
 	DepthRefine(depth, sal);
 	GaussianRefine(sal);

	delete labels;
	return sal;
}