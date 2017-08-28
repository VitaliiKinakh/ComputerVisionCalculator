#pragma once
// standart C++ includes
#include<vector>
#include<string>

// OpenCV includes
#include<opencv2\opencv.hpp>
#include<opencv2\ml.hpp>
#include<opencv2\highgui.hpp>

// Project directories
//#include"Configs.h"


/* Function for extracting "v" part from image in HSV colorspace
*  Gets: BGR image
*  Return: grayscaled image - "v" part
*/
cv::Mat extractValue(cv::Mat &imgOriginal);


/* Function for maximizing contrast using morphological operations:
*  - BLACKHAT
*  - TOPHAT
*  Gets: grayscaled image
*  Return: grayscaled image
*/
cv::Mat maximizeContrast(cv::Mat &imgGrayscale);


/* Preprocessing function 
*  Algo:
*  - extract value from HSV colorspace image
*  - maximasize contrast using TOPHAT and BLACKHAT
*  - blur image with Gaussian blur
*  - get binary image using adaptive threshold
*  - dilate image for getting bigger parts
*  Gets: BGR image to process
*  Changes: gray image become value from HSV colorspace image
*           thresh binary image processed by adaptive threshold and dilated
*/
void preprocess(cv::Mat& image, cv::Mat& gray, cv::Mat& thresh);


/* Function for verifing rectangle by size
*  Gets: cv::Rect object
*  Return: bool
*/
bool verifyCharacter(const cv::Rect& rect);


/* Predicate for sorting rectangles by x coord
*  Gets: 2 rectangles
*  Return: bool
*/ 
bool sortRectangles(const cv::Rect& r1, const cv::Rect& r2);


/* Function for getting rectangles with possible chars in the image
*  Gets: binary image
*  Return: vector of cv::Rect
*  Algo:
*  - get contours from binary image
*  - find bounding rectangles around each contour
*  - verify rectangles
*/
std::vector<cv::Rect> getRectangles(cv::Mat& thresh);


/* Function for extracting and preprocessing characters from binary image with rectangles 
*  Gets: binary image
*        vector of cv::Rect rectangles
*  Return: vector of extracted and preprocessed images
*  Algo:
*  - go throw all rectangles and extract each possible digit
*  -- resize digit
*  -- convert digit to float
*  -- reshape it to one row
*/
std::vector<cv::Mat> getCharacters(const cv::Mat& thresh, const std::vector<cv::Rect> rectangles);


/* Function for getting result string 
*  Gets: binary image, vector of cv::Rect of possible characters
*  Return: string result detected by SVM classifier
*  Algo:
*  - get characters with getCharacters function
*  - go throw all characters
*  -- predict value by SVM classifier
*  -- add character to result
*/
std::string getResultString(const cv::Mat& thresh, const std::vector<cv::Rect> rectangles);


/* Function for checking if it is possible to parse string
*  Gets: string
*  Return: bool value
*  Algo:
*  - chech if first character is digit
*  - check if last character is digit
*  - check if there is only one operation character
*/
bool checkString(const std::string& str);


/* Function for parsing expression
*  expression should be like: a op b
*  where 'a' and 'b' are numbers and 'op' is operation
*  Gets: string with expression
*  Return: double value computed
*  Algo:
*  - go throw all string and get chars before operation character
*  - get operation character
*  - get last digits
*  - compute result;
*/
double parse(const std::string& str);
