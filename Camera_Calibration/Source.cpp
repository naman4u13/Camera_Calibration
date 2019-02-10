#include <opencv2/opencv.hpp>
#include <iostream>

using namespace cv;
using namespace std;

void getWorldPts(Size boardSize, float chessSqDim, vector<Point3f>& worldPts)
{
	for (int i = 0; i < boardSize.height; i++)
	{
		for (int j = 0; j < boardSize.width; j++)
		{
			worldPts.push_back(Point3f(j*chessSqDim, i*chessSqDim, 0.0f));
		}

	}
}
void getImagePts(vector<Mat> images, vector<vector<Point2f>>& imgPts, Size boardSize)
{
	cout << "\n" << images.size() << endl;
	for (int i = 0; i < images.size(); i++)
	{
		vector<Point2f> tempPoint;
		bool found = findChessboardCorners(images.at(i), boardSize, tempPoint,CALIB_CB_ADAPTIVE_THRESH + CALIB_CB_NORMALIZE_IMAGE+ CALIB_CB_FAST_CHECK);
		if (found)
		{
			imgPts.push_back(tempPoint);
		}

	}

}
void cameraCalibration(vector<Mat> images, Size boardSize, float chessSqDim)
{
	Mat cameraMatrix;
	vector<float> distMatrix;

	vector<vector<Point2f>> ImgPts;
	getImagePts(images, ImgPts, boardSize);

	vector<vector<Point3f>> WorldPts(1);
	getWorldPts(boardSize, chessSqDim, WorldPts[0]);
	WorldPts.resize(ImgPts.size(), WorldPts[0]);

	vector<Mat> rvectors, tvectors;


	calibrateCamera(WorldPts, ImgPts, boardSize, cameraMatrix, distMatrix, rvectors, tvectors);

	cout << cameraMatrix << endl;

	cout << "\n\n" ;

	for (int i = 0; i < distMatrix.size(); i++)
	{
		cout << distMatrix.at(i) << endl;
	}

	waitKey(0);

}


int compute(Size boardSize, float chessSqDim)
{
	int count = 0;
	Mat frame;
	vector<Mat> selectedImgs;
	VideoCapture vid(0);
	if (!vid.isOpened())
	{
		return 0;
	}
	int framespersec = 20;
	namedWindow("Webcam", CV_WINDOW_AUTOSIZE);
	while (count!=15)
	{	
		if (!vid.read(frame))
		{
			break;
		}
		vector<Point2f> corners; //this will be filled by the detected corners
		bool patternfound = findChessboardCorners(frame, boardSize, corners,
			CALIB_CB_ADAPTIVE_THRESH + CALIB_CB_NORMALIZE_IMAGE
			+ CALIB_CB_FAST_CHECK);
		Mat pattern_frame;
		frame.copyTo(pattern_frame);
		drawChessboardCorners(pattern_frame, boardSize, Mat(corners), patternfound);
		
		if (patternfound)
		{
			selectedImgs.push_back(frame);
			count++;
			imshow("Webcam", pattern_frame);
		}
		else
		{
			imshow("Webcam", frame);
		}
		char character = waitKey(1000 / framespersec);
	}
	destroyWindow("Webcam");
	cameraCalibration(selectedImgs, boardSize, chessSqDim);
	//CALIB_CB_FAST_CHECK saves a lot of time on images
	//that do not contain any chessboard corners
	

	/*if (patternfound)
		cornerSubPix(image, corners, Size(11, 11), Size(-1, -1),
			TermCriteria(CV_TERMCRIT_EPS + CV_TERMCRIT_ITER, 30, 0.1));*/
	return 0;
}

int main(int argc, char** argv)
{
	float chessSqDim = 0.03f; //meters
	Size boardSize(7, 7); //interior number of corners
	// Read the image file
	Mat image = imread("chessboard.jpg", CV_LOAD_IMAGE_GRAYSCALE);

	// Check for failure
	if (image.empty())
	{
		cout << "Could not open or find the image" << endl;
		cin.get(); //wait for any key press
		return -1;
	}
	String windowName = "Chessboard";
	imshow(windowName, image);
	waitKey(0); // Wait for any keystroke in the window
	destroyWindow(windowName);
	compute(boardSize,chessSqDim);

	return 0;
}