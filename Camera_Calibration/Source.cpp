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
			worldPts.push_back(Point3f(j*chessSqDim, i*chessSqDim, 0.0f));//creates world cooridnates, as all points lies on plane, Z is always 0
		}

	}
}
void getImagePts(vector<Mat> images, vector<vector<Point2f>>& imgPts, Size boardSize)
{
	//cout << "\n" << images.size() << endl;
	for (int i = 0; i < images.size(); i++)
	{
		
		vector<Point2f> tempPoint;
		bool found = findChessboardCorners(images.at(i), boardSize, tempPoint,CALIB_CB_ADAPTIVE_THRESH + CALIB_CB_NORMALIZE_IMAGE);//Finds the positions of internal corners of the chessboard and store it in "tempPoint".
		if (found)
		{
		    /*Mat viewGray;//Store greyscale image of view/frame 
			cvtColor(images.at(i), viewGray, CV_BGR2GRAY);//to use function "cornerSubPix()", input image must be grayscale
			cornerSubPix(viewGray, tempPoint, Size(11, 11), Size(-1, -1),TermCriteria(CV_TERMCRIT_EPS + CV_TERMCRIT_ITER, 30, 0.1));// To improve accuracy of computed position of detected coordinates*/
			imgPts.push_back(tempPoint);
		}

	}

}
void cameraCalibration(vector<Mat> images, Size boardSize, float chessSqDim)
{
	Mat cameraMatrix;//Intrinsic Camera Matrix 
	vector<float> distMatrix;//Distortion Matrix

	vector<vector<Point2f>> ImgPts;//Store Image coordinates for every view/frame
	getImagePts(images, ImgPts, boardSize);//Obtain Image coordinates for every view/frame

	vector<vector<Point3f>> WorldPts(1);
	getWorldPts(boardSize, chessSqDim, WorldPts[0]);//Obtain World coordinates for every view/frame
	WorldPts.resize(ImgPts.size(), WorldPts[0]);//World Points remain same for every corresponding Image Points 

	vector<Mat> rvectors, tvectors;//For each view /frame there corresponding Rotation and Translation Matrix (Extrinsic Matrix)

	double error = calibrateCamera(WorldPts, ImgPts, boardSize, cameraMatrix, distMatrix, rvectors, tvectors);//this function computes camera matrix, extrinsic matrix and mean reprojection error assosciated for a set of images using Zhang's Method Of Camera Calibration

	cout << "\n   CAMERA INTRINSIC MATRIX -  \n\n" << cameraMatrix << endl;
	cout << "\n    re-projection error -  " << error << endl;//Print out Mean Reprojection Error, lesser the value better is the calibration.
	cout << "\n\n DISTROTION VALUES -" ;

	for (int i = 0; i < distMatrix.size(); i++)
	{
		cout << distMatrix.at(i) << "   " ;//Prints Distortion coefficients (k1, k2, p1, p2[, k3[, k4, k5, k6]])
	}

	
	Mat undistorted;//Store undistorted Image
	undistort(images.at(0), undistorted, cameraMatrix, distMatrix);//Transforms an image to compensate for lens distortion using camera matrix and distortion matrix
	namedWindow("DISTORTED", CV_WINDOW_AUTOSIZE);
	imshow("DISTORTED", images.at(0));
	namedWindow("UNDISTORTED", CV_WINDOW_AUTOSIZE);
	imshow("UNDISTORTED", undistorted);
	/*FileStorage storage("CamCal.yml", cv::FileStorage::WRITE);
	storage << "Intrinsic_camera_matrix" << cameraMatrix;
	storage << "distortion_matrix" << distMatrix;
	storage << "mean_projection_error" << error;
	storage.release();*/
	waitKey(0);
	


}


int compute(Size boardSize, float chessSqDim,int framesNum)
{
	int count = 0;
	Mat frame;//Store each frame of video
	vector<Mat> selectedImgs;//Store Frames/Views selected for calibration
	VideoCapture vid(0);//open the default camera
	vid.set(CAP_PROP_AUTOFOCUS, 0);//disabling autofocus in the camera
	vid.set(CAP_PROP_FRAME_WIDTH, 640);//setting image width resolution(pixels)
	vid.set(CAP_PROP_FRAME_HEIGHT, 480);//setting image height resolution(pixels)
	if (!vid.isOpened())
	{
		return 0;
	}
	int framespersec = 20;//setting video frames speed 
	namedWindow("Webcam", CV_WINDOW_AUTOSIZE);
	while (count!=framesNum)//Keep recording frames until specified number('framesNum') are not detected and selected for calibration
	{	
		if (!vid.read(frame))//Grabs, decodes and returns the next video frame.
		{
			break;
		}
		vector<Point2f> corners; //this will be filled by the detected corners
		bool patternfound = findChessboardCorners(frame, boardSize, corners,//Finds the positions of internal corners of the chessboard and store it in "corners".
			CALIB_CB_ADAPTIVE_THRESH + CALIB_CB_NORMALIZE_IMAGE
			+ CALIB_CB_FAST_CHECK);//CALIB_CB_FAST_CHECK saves a lot of time on images that do not contain any chessboard corners
		Mat pattern_frame;
		frame.copyTo(pattern_frame);
		drawChessboardCorners(pattern_frame, boardSize, Mat(corners), patternfound);//The function draws individual chessboard corners detected
		if (patternfound)//if checker board pattern is detected show patterned image of frame else show original frame 
		{
			imshow("Webcam", pattern_frame);//shows patterned image
			char ch = waitKey(0);//Based on user choice,if he presses "ENTER" key image/frame will be selected elsewise for any other key its is rejected 
			if (ch == 13)
			{
				printf("  %d  ", count + 1);
				selectedImgs.push_back(frame.clone());//Store frame in 'selectedImgs' vector
				count++;//increment count 
			}
			
		}
		else
		{
			imshow("Webcam", frame);
		}
		waitKey(1000 / framespersec);//wait for specified amount of time before capturing subsequent frames
	}
	vid.release();//close webcam
	destroyWindow("Webcam");
	cameraCalibration(selectedImgs, boardSize, chessSqDim);
	return 0;
}

int main(int argc, char** argv)
{
	float chessSqDim; //in meters
	cout << "\n\n Enter ChessBoard squares dimesions in meteres - ";
	cin >> chessSqDim;
	
	int width, height;
	cout << "\n\n Enter count of CheckerBoard's squares alligned horizontally - ";
	
	cin >> width;
	cout << "\n\n Enter count of CheckerBoard's squares alligned vertically - ";
	
	cin >> height;
	int framesNum;
	cout << "\n\n Enter Count of images/frames/view you want for computing Camera Matrix (NOTE : Greater the number of Frames Count greater the accuracy) - ";
	
	cin >> framesNum;
	cout << "\n\n" << endl;
	Size boardSize(width-1, height-1); //interior number of corners
	compute(boardSize,chessSqDim,framesNum);
	
	return 0;
}