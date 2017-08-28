#include"Calculator.h"


cv::Mat extractValue(cv::Mat &imgOriginal) {
	cv::Mat imgHSV;
	std::vector<cv::Mat> vectorOfHSVImages;
	cv::Mat imgValue;

	cv::cvtColor(imgOriginal, imgHSV, CV_BGR2HSV);

	cv::split(imgHSV, vectorOfHSVImages);

	imgValue = vectorOfHSVImages[2];

	return(imgValue);
}


cv::Mat maximizeContrast(cv::Mat &imgGrayscale) {
	cv::Mat imgTopHat;
	cv::Mat imgBlackHat;
	cv::Mat imgGrayscalePlusTopHat;
	cv::Mat imgGrayscalePlusTopHatMinusBlackHat;

	cv::Mat structuringElement = cv::getStructuringElement(CV_SHAPE_RECT, cv::Size(3, 3));

	cv::morphologyEx(imgGrayscale, imgTopHat, CV_MOP_TOPHAT, structuringElement);
	cv::morphologyEx(imgGrayscale, imgBlackHat, CV_MOP_BLACKHAT, structuringElement);

	imgGrayscalePlusTopHat = imgGrayscale + imgTopHat;
	imgGrayscalePlusTopHatMinusBlackHat = imgGrayscalePlusTopHat - imgBlackHat;

	return(imgGrayscalePlusTopHatMinusBlackHat);
}


void preprocess(cv::Mat& image, cv::Mat& gray, cv::Mat& thresh)
{
	// extract value channel only from original image to get imgGrayscale
	gray = extractValue(image);

	// maximize contrast with top hat and black hat
	cv::Mat imgMaxContrastGrayscale = maximizeContrast(gray);

	cv::Mat imgBlurred;

	// gaussian blur
	cv::GaussianBlur(imgMaxContrastGrayscale, imgBlurred, cv::Size(25, 25), 0);

	// call adaptive threshold to get imgThresh
	cv::adaptiveThreshold(imgBlurred, thresh, 255.0, CV_ADAPTIVE_THRESH_GAUSSIAN_C, CV_THRESH_BINARY_INV, 100 * 2 + 1, 20);

	cv::dilate(thresh, thresh, cv::Mat(31, 31, CV_8U, cv::Scalar::all(1)));
}


bool verifyCharacter(const cv::Rect& rect)
{
	if (rect.width > 70 && rect.height > 100 && rect.width * rect.height > 6500 && rect.height / rect.width < 7)
	{
		return true;
	}
	else {
		return false;
	}
}


bool sortRectangles(const cv::Rect& r1, const cv::Rect& r2)
{
	// Sort by center x 
	return ((r1.br().x + r1.tl().x) / 2) < ((r2.br().x + r2.tl().x) / 2);
}


std::vector<cv::Rect> getRectangles(cv::Mat& thresh)
{
	// Find contours
	std::vector<std::vector<cv::Point>> contours;

	cv::findContours(thresh, contours, CV_RETR_EXTERNAL, CV_CHAIN_APPROX_SIMPLE);

	// Get rectangles around characters
	std::vector<cv::Rect> boundRect(contours.size());
	std::vector<std::vector<cv::Point> > contours_poly(contours.size());
	// Verified rectangles
	std::vector<cv::Rect> verified;
	for (int i = 0; i < contours.size(); ++i)
	{
		approxPolyDP(cv::Mat(contours[i]), contours_poly[i], 3, true);
		boundRect[i] = boundingRect(cv::Mat(contours_poly[i]));
		if (verifyCharacter(boundRect[i]))
		{
			verified.push_back(boundRect[i]);
		}
	}
	std::vector<cv::Rect> rectangles = verified;

	// Sort rectangles
	std::sort(rectangles.begin(), rectangles.end(), sortRectangles);
	return rectangles;
}


std::vector<cv::Mat> getCharacters(const cv::Mat& thresh, const std::vector<cv::Rect> rectangles)
{
	std::vector<cv::Mat> characters(rectangles.size());
	for (int i = 0; i < rectangles.size(); ++i)
	{
		// Extract
		cv::Mat extracted = thresh(rectangles[i]);

		//	Resize
		cv::Mat resized;
		cv::resize(extracted, resized, cv::Size(20, 20));

		// Reshape
		cv::Mat digitFloat;
		resized.convertTo(digitFloat, CV_32F);

		characters[i] = digitFloat.reshape(1, 1);
	}
	return characters;
}


std::string getResultString(const cv::Mat& thresh, const std::vector<cv::Rect> rectangles)
{
	// Get extracted, resized and reshaped characters
	std::vector<cv::Mat> characters = getCharacters(thresh, rectangles);

	// Declare result
	std::string result = "";

	// Load SVM classifier
	cv::Ptr<cv::ml::SVM> svm = cv::ml::StatModel::load<cv::ml::SVM>("model4.yml");

	// Iterate throw all rectangles
	for (int i = 0; i < characters.size(); ++i)
	{
		// Classify
		char Char = svm->predict(characters[i]);

		// Check for minus
		if (Char == '1' && rectangles[i].width >= 3 * rectangles[i].height)
		{
			Char = '-';
		}

		// Result
		std::stringstream ss;
		ss << Char;
		std::string str;
		ss >> str;

		// Add each detected character
		result += str;
	}
	return result;
}


bool checkString(const std::string& str)
{
	// Check if first elem is digit 
	if (isdigit(str[0]))
	{
		// Check if last elem is digit
		if (isdigit(str[str.size() - 1]))
		{
			// Check if there is operation char
			int numOp = 0;
			for (int i = 1; i < str.size() - 1; ++i)
			{
				if (str[i] == '+' || str[i] == '-' || str[i] == '/' || str[i] == '*')
				{
					++numOp;
				}
			}
			if (numOp == 1)
			{
				return true;
			}
			else {
				return true;
			}
		}
		else {
			return false;
		}
	}
	else {
		return false;
	}
}


double parse(const std::string& str)
{
	std::string n1, n2, op;

	// Preprocess string
	int i = 0;
	while (isdigit(str[i]))
	{
		n1 += str[i++];
	}

	op = str[i++];

	while (isdigit(str[i]))
	{
		n2 += str[i++];
	}

	char* n1_c = new char[n1.size()];
	for (int i = 0; i < n1.size(); ++i)
	{
		n1_c[i] = n1[i];
	}
	double a = atof(n1_c);

	char* n2_c = new char[n2.size()];
	for (int i = 0; i < n2.size(); ++i)
	{
		n2_c[i] = n2[i];
	}
	double b = atof(n2_c);


	if (op == "+")
	{
		return a + b;
	}
	if (op == "-")
	{
		return a - b;
	}
	if (op == "/")
	{
		return a / b;
	}
	if (op == "*")
	{
		return a * b;
	}
}