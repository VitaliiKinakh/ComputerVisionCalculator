#pragma once
// standart C++ libraries
#include<iostream>
#include<string>
#include<vector>

// OpenCV includes
#include<opencv2\opencv.hpp>
#include<opencv2\core.hpp>
#include<opencv2\ml.hpp>
#include<opencv2\highgui.hpp>

// Include project directories
//#include"Configs.h"


/* Function for training SVM classifier
*  Gets: cv::Mat data, vector<int> labels
*  Algo:
*  - sets SVM params
*  - creates train data
*  - train SVM classifier
*/
void SVMtrain(const cv::Mat &trainMat, const std::vector<int> &trainLabels);

 
/* Function for generating training data
*  Gets: cv::Mat images for loaded images
*        vector<int> labels 
*  Algo:
*  - loads self-created train data images
*  - loads mnist data
*/
void generateData(cv::Mat& images, std::vector<int>& labels);


