#include "acsd.h"
#include <omp.h>

void main( void )
{
	                                 //'LFSD' OR 'NJU2K'
	String img_dir = "..\\..\\Dataset\\LFSD\\Image\\";				//color image directory
	String depth_dir = "..\\..\\Dataset\\LFSD\\DepthMap\\";			//depth image directory
	String out_dir = "..\\..\\SaliencyMap\\LFSD\\ACSD\\";				//output image directory

	vector<string> imgfiles;
	list_dir(img_dir+"*.jpg", imgfiles);

#pragma omp parallel for
	for (int nFile=0; nFile<imgfiles.size(); ++nFile)
	{
		Mat img = imread((img_dir+imgfiles[nFile]).c_str());
		Mat depth = imread((depth_dir+imgfiles[nFile]).c_str(),0);
		Mat saliency = getSal(img, depth);							//get saliency map
		imwrite((out_dir+imgfiles[nFile]).c_str(), saliency);
	}
}