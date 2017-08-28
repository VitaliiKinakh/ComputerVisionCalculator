#include"Calculator.h"

void SVMtrain(cv::Mat &trainMat, std::vector<int> &trainLabels)
{
	cv::Ptr<cv::ml::SVM> svm = cv::ml::SVM::create();
	svm->setType(cv::ml::SVM::C_SVC);
	svm->setKernel(cv::ml::SVM::LINEAR);
	svm->setTermCriteria(cv::TermCriteria(cv::TermCriteria::MAX_ITER, 100, 1e-6));
	svm->setGamma(3);
	svm->setDegree(3);
	cv::Ptr<cv::ml::TrainData> td = cv::ml::TrainData::create(trainMat, cv::ml::ROW_SAMPLE, trainLabels);
	svm->train(td);
	//svm->trainAuto(td);
	svm->save("model4.yml");
}


void generateData(cv::Mat& images, std::vector<int>& labels)
{
	std::cout << "Started loading data" << std::endl;

	std::vector<std::string> names = { "0", "1","2", "3", "4", "5", "6", "7", "8", "9", "minus", "plus", "mult", "divide"};
	for (int i = 0; i < names.size(); ++i) {

		cv::Mat image = cv::imread(names[i] + ".bmp", 0);
		cv::Mat thresh;
		cv::threshold(image, thresh, 0, 255, CV_THRESH_BINARY);
		// Find contours
		std::vector<std::vector<cv::Point>> contours;

		cv::findContours(thresh, contours, cv::RETR_EXTERNAL, cv::CHAIN_APPROX_SIMPLE);

		std::vector<cv::Rect> boundRect(contours.size());
		std::vector<std::vector<cv::Point> > contours_poly(contours.size());

		for (int j = 0; j < contours.size(); ++j)
		{
			if (cv::contourArea(contours[i]) > 15) {
				approxPolyDP(cv::Mat(contours[j]), contours_poly[j], 3, true);
				boundRect[j] = boundingRect(cv::Mat(contours_poly[j]));
				if (i == 0)
				{
					labels.push_back((int)'0');
				}
				if (i == 1)
				{
					labels.push_back((int)'1');
				}
				if (i == 2)
				{
					labels.push_back((int)'2');
				}
				if (i == 3)
				{
					labels.push_back((int)'3');
				}
				if (i == 4)
				{
					labels.push_back((int)'4');
				}
				if (i == 5)
				{
					labels.push_back((int)'5');
				}
				if (i == 6)
				{
					labels.push_back((int)'6');
				}
				if (i == 7)
				{
					labels.push_back((int)'7');
				}
				if (i == 8)
				{
					labels.push_back((int)'8');
				}
				if (i == 9)
				{
					labels.push_back((int)'9');
				}
				if (i == 10)
				{
					labels.push_back((int)'-');
				}
				if (i == 11)
				{
					labels.push_back((int)'+');
				}
				if (i == 12)
				{
					labels.push_back((int)'*');
				}
				if (i == 13)
				{
					labels.push_back((int)'/');
				}
				cv::Mat character = thresh(boundRect[j]);

				cv::Mat resized;
				cv::resize(character, resized, cv::Size(20, 20));

				cv::Mat digitFloat;
				resized.convertTo(digitFloat, CV_32F);

				cv::Mat reshaped = digitFloat.reshape(1, 1);

				images.push_back(reshaped);
			}
		}

	}

	std::cout << "Started loading mnist data" << std::endl;
	// Mnist data
	cv::Mat mnist = cv::imread("digits.png", 0);
	cv::Mat thresh;
	cv::threshold(mnist, thresh, 0, 255, CV_THRESH_BINARY);

	cv::Mat zeros = mnist(cv::Rect(cv::Point(0, 0), cv::Point(mnist.cols - 1, 99)));
	cv::Mat ones = mnist(cv::Rect(cv::Point(0, 100), cv::Point(mnist.cols - 1, 199)));
	cv::Mat twos = mnist(cv::Rect(cv::Point(0, 200), cv::Point(mnist.cols - 1, 299)));
	cv::Mat threes = mnist(cv::Rect(cv::Point(0, 300), cv::Point(mnist.cols - 1, 399)));
	cv::Mat fours = mnist(cv::Rect(cv::Point(0, 400), cv::Point(mnist.cols - 1, 499)));
	cv::Mat fives = mnist(cv::Rect(cv::Point(0, 500), cv::Point(mnist.cols - 1, 599)));
	cv::Mat sixes = mnist(cv::Rect(cv::Point(0, 600), cv::Point(mnist.cols - 1, 699)));
	cv::Mat sevens = mnist(cv::Rect(cv::Point(0, 700), cv::Point(mnist.cols - 1, 799)));
	cv::Mat eights = mnist(cv::Rect(cv::Point(0, 800), cv::Point(mnist.cols - 1, 899)));
	cv::Mat nines = mnist(cv::Rect(cv::Point(0, 900), cv::Point(mnist.cols - 1, 999)));

	cv::Mat digitsArr[10] = { zeros, ones, twos, threes, fours, fives, sixes, sevens, eights, nines };

	for (int i = 0; i < 10; ++i)
	{
		std::vector<std::vector<cv::Point>> contours;

		cv::findContours(digitsArr[i], contours, cv::RETR_EXTERNAL, cv::CHAIN_APPROX_SIMPLE);

		std::vector<cv::Rect> boundRect(contours.size());
		std::vector<std::vector<cv::Point> > contours_poly(contours.size());

		for (int j = 0; j < contours.size(); ++j)
		{
			if (cv::contourArea(contours[i]) > 11) {
				approxPolyDP(cv::Mat(contours[j]), contours_poly[j], 3, true);
				boundRect[j] = boundingRect(cv::Mat(contours_poly[j]));

				if (i == 0)
				{
					labels.push_back((int)'0');
				}
				if (i == 1)
				{
					labels.push_back((int)'1');
				}
				if (i == 2)
				{
					labels.push_back((int)'2');
				}
				if (i == 3)
				{
					labels.push_back((int)'3');
				}
				if (i == 4)
				{
					labels.push_back((int)'4');
				}
				if (i == 5)
				{
					labels.push_back((int)'5');
				}
				if (i == 6)
				{
					labels.push_back((int)'6');
				}
				if (i == 7)
				{
					labels.push_back((int)'7');
				}
				if (i == 8)
				{
					labels.push_back((int)'8');
				}
				if (i == 9)
				{
					labels.push_back((int)'9');
				}

				cv::Mat digit = digitsArr[i](boundRect[j]);
				cv::Mat resized;
				cv::resize(digit, resized, cv::Size(20, 20));
				cv::Mat digitFloat;
				resized.convertTo(digitFloat, CV_32F);
				cv::Mat reshaped = digitFloat.reshape(1, 1);
				images.push_back(reshaped);
			}
		}
	}

	std::cout << "End" << std::endl;
}




int main()
{
	/*
	// Load train data and labels
	cv::Mat data;
	std::vector<int> labels;
	generateData(data, labels);

	// Train SVM classifier
	SVMtrain(data, labels);
	*/

	// Load image
	cv::Mat image = cv::imread("testDB\\test16.jpg");

	// Preprocess
	cv::Mat gray, thresh;
	preprocess(image, gray, thresh);

	// Get rectangles
	std::vector<cv::Rect> rectangles = getRectangles(thresh);

	std::string result = getResultString(thresh, rectangles);
	if (checkString(result)) {

		std::cout << result << " = " << parse(result) << std::endl;
	}
	else {
		std::cout << "Invalid image! Can`t calculate result!" << std::endl;
	}
	
    return 0;
 }