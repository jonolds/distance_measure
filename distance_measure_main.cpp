#include <iostream>
#include <opencv2/core.hpp>
#include <opencv2/highgui.hpp>
#include <opencv2/videostab/ring_buffer.hpp>

using namespace std;
using namespace cv;

void loadCamData();
void getRoi();
Mat outMat, camMat, distCos;

int main() {
	loadCamData();
	namedWindow("mainWin", WINDOW_AUTOSIZE);
	moveWindow("mainWin", 10, 10);
	
	VideoCapture cap(0);
	cap.set(CAP_PROP_FRAME_WIDTH, 1280);
	cap.set(CAP_PROP_FRAME_HEIGHT, 720);

	while(true) {
		cap >> outMat;
		undistort(outMat.clone(), outMat, camMat, distCos);
		resize(outMat , outMat, Size(640, 360));

		imshow("mainWin", outMat);
		char c = waitKey(30);
		if(c == 114)
			getRoi();
		else if(c == 27) //esc
			break;
	}
	return 0;
}

void getRoi() {

	Rect r = selectROI("mainWin", outMat);
	cout << r << "\n";
	cout << r.x << "\n";
	cout << r.y << "\n";
	cout << r.width << "\n";
	cout << r.height << "\n";



	Mat tmpMat = Mat(Size(640, 360), outMat.type(), Scalar::all(0));
	outMat.copyTo(tmpMat(Rect(r.x, r.y, r.width, r.height)));
	
	imshow("roi", tmpMat);
	waitKey();


}

void loadCamData() {
	FileStorage fs("../files/camParms.xml", FileStorage::READ);
	fs["cameraMatrix"] >> camMat;
	fs["dist_coeffs"] >> distCos;
}