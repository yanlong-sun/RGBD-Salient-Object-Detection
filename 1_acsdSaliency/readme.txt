1. This code is an implementation for paper:
Depth Saliency Based on Anisotropic Center-Surround Difference. Ran Ju, Ling Ge, Wenjing Geng, Tongwei Ren and Gangshan Wu. ICIP2014.

2. The source code is for educational and research use only. If you use any part of the source code, please cite related papers.

3. This code has been tested on Windows 7 32/64bit with Visual Studio 2010/2012 environment. To use our code, you need to install openCV. The version used in our code is 2.4.2: http://opencv.org/opencv-v2-4-2-released.html. We have integrated a minimum package into our code. If you want to run our code without setting openCV, add acsdSaliency\acsdSaliency\include to VC++ Include Directories and acsdSaliency\acsdSaliency\lib to VC++ Libarary Directories, and add acsdSaliency\acsdSaliency\bin to the environment variables of PATH.

4. Usage:
a. Open the project acsdSaliency\acsdSaliency.sln using Visual Studio.
b. Copy the color and corresponding depth images to acsdSaliency\acsdSaliency\image and acsdSaliency\acsdSaliency\depth separately. Note the color image file and its corresponding depth image file should be named the same.
c. Compile and Run. The output saliency results are stored in acsdSaliency\acsdSaliency\out.

Changelog:
11.11.2014: Fixed a bug leads to memory leak.

If you find any problems related to the code, please contact me.
juran@smail.nju.edu.cn