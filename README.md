# Camera_Calibration

## Table Of Contents
1. [Description](#description)
2. [Working](#working)
3. [Scope](#scope)

   

## Description
To learn 'What is Camera Calibration' please refer [here](http://ais.informatik.uni-freiburg.de/teaching/ws10/robotics2/pdfs/rob2-10-camera-calibration.pdf)
To understand the theory behind the process please refer to the following links [1](https://in.mathworks.com/help/vision/ug/camera-calibration.html)
and [2](https://docs.opencv.org/2.4/doc/tutorials/calib3d/camera_calibration/camera_calibration.html).

The specified software will compute Intrinsic Camera coefficients, Extrinsic coefficients(specifically Rotation and Translation Matrix)
and Distortion coefficients using Zhang's method of Camera Calibration which requires you to capture multiple images of a calibration 
pattern,such as a checkerboard from various angles.
  
  ## Working
  ### User Inputs
 User has to input the dimensions of Checker-board pattern which contains dimension of square in meteres and number of squares along
 width and height of the board. User also has to specify number of different views or frames he/she desires to capture for Calibration.
 Greater the number of views/frames/image of the pattern, better is the calibration.   
 
  ### Capture Images from Web-Cam
  The program will give user the choice of selecting or rejecting the frame on which a checker-board pattern has been found, pressing 
  'Enter Key' will select the image where as on pressing any other key frame/image will be rejected.
  
  <img src="https://github.com/naman4u13/Camera_Calibration/blob/master/Camera_Calibration/Image17.jpg" alt="image" height="500px" width="700px" >

  ### Finding the checker-board pattern on the image
  OpenCV inbuilt function 'findChessboardCorners' finds the positions of internal corners of the chessboard, similarly function
 'cornerSubPix' refines the corner locations and function 'drawChessboardCorners' draws the ndividual chessboard corners detected 
  either as red circles if the board was not found, or as colored corners connected with lines if the board was found.
  
   <img src="https://github.com/naman4u13/Camera_Calibration/blob/master/Camera_Calibration/softwareVisual%20Studio%2017VS_projectsCamera_CalibrationCamera_Calibrationpattern16.jpg" alt="image" height="500px" width="700px">

  ### Computing Real-World Coordinates and Image Plane Coordinates
  To estimate the camera parameters, we need to have 3-D world points and their corresponding 2-D image points. We set world plane in such
  a manner that Z-axis values is always zero for all points detected and there x-y coordinate value are simply obtained by multiplying
  board's square dimensions to the vertices matrix of the board.
  
  Image Plane Coordinates are obtained from 'findChessboardCorners' function
  
  ### Compute Camera Calibration
  OpenCV inbuilt function 'calibrateCamera' Finds the camera intrinsic and extrinsic parameters from several views of a calibration pattern.
  It stores intrinsic parameteres in 'cameraMatrix'(Output 3x3 floating-point camera matrix), extrinsic parameters in 'rvecs'(Output vector of rotation vectors estimated for each 
  pattern view) and 'tvecs'(Output vector of translation vectors estimated for each pattern view) , and distortion coefficients in 
  'distCoeffs'(Output vector of distortion coefficients  (k1, k2, p1, p2[, k3[, k4, k5, k6]]) of 4, 5, or 8 elements).
  It also computes the averag reprojection errors which are the distances, in pixels, between the detected and the reprojected points.
  This number gives a good estimation of precision of the found parameters. This should be as close to zero as possible.
  
  [View Results](https://github.com/naman4u13/Camera_Calibration/blob/master/Camera_Calibration/CamCal.yml)
  
  <img src="https://github.com/naman4u13/Camera_Calibration/blob/master/Camera_Calibration/Screenshot%20(23).png" alt="image" height="500px" width="700px">
  
  
  
  
  ## Important
  
  1.) The following program works only for Checker-Board Pattern
  
  
  2.) Function 'cornerSubPix()' which is used to refine detected points position, sometimes lead to over compensation and produces bad 
      results, specially for those images in which board is at further away from camera, therefore its use must be done with caution.
      I have commented it out in my code.Do remember that this function works only on 'Grayscale' image.
      
      
  3.) The checkerboard pattern you use, must not be square. One side must contain an even number of squares and the other side must 
      contain an odd number of squares. 
      
      
  4.) Keep the pattern in focus, but do not use autofocus. If you change zoom settings between images, the focal length changes which
      will eventually produces wrong results
      
      
  5.) Capture a variety of images of the pattern so that you have accounted for as much of the image frame as possible. Lens distortion
      increases radially from the center of the image and sometimes is not uniform across the image frame. To capture this lens 
      distortion, the pattern must appear close to the edges of the captured images. 
      
      
  6.) It is important is to provide a bigger set of paramters, more angles and positions, rather than equivalent data.
  
  
  7.) Remember that even after disabling autofocus, or keeping focal length of the camera same throught the process, camera matrix is 
      still dependent upon the resolution/PPI of the image at which camera is capturing the frames.For every different resolution camera 
      matrix is similarly gets scaled.In this program web camera resolution is set to 640 X 480. 
      
    
  
  
  ## Scope
  
  1.) After calculating the camera matrix, we can remove distortion from any image captured using function 'undistort'.
  
 
  <img src="https://github.com/naman4u13/Camera_Calibration/blob/master/Camera_Calibration/undistort.jpg" alt="image" height="300px" width="400px" align = "left" title = "Undistorted">
  <img src="https://github.com/naman4u13/Camera_Calibration/blob/master/Camera_Calibration/Image1.jpg" alt="image" height="300px" width="400px" title = "Distorted">
  
  
  
  
  
  2.) We can calculate Field of View and Focal Length of camera in metric units using function 'calibrationMatrixValues', this function 
      requires you to input dimensions of camera's image sensor, unfortunately we cannot access computer's integrated webcamera details
      especially its sensor size programmatically in OpenCV C++.
      
  
  3.) We can integrate this program with Android using OpenCV's native interface for development with OpenCv4Android.
  
  
  4.) Android also provides a class 'CameraManager', A system service manager for detecting, characterizing, and connecting to CameraDevices.
      Using it we can obtain Mobile Camera's sensor physical size information, we can also call a public method 'getCameraCharacteristics(String cameraId)',
      to query the capabilities of a camera device eg - horizontal and vertical angle of views and focal length of camera.
        
  
  
  
