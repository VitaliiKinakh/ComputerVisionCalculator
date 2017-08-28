#include"Calculator.h"
#include"Training.h"
#include"CommandLineUI.h"
#include"Configs.h"

int main()
{
	showConsoleHeader();
	int userOption = getUserOption();

	if (userOption == 1)
	{
		std::cout << "  >> Start calculator\n";
		// Get filename for image
		std::string filename = getFilename();
		
		// Load image
		cv::Mat image = cv::imread(TEST_DIR + filename);

		// In case image is empty
		while (image.empty())
		{
			std::cout << "  => There is not such image!\n";
			// Load image again
			filename = getFilename();
			image = cv::imread(TEST_DIR + filename);
		}

		// Create image for drawing
		cv::Mat drawing(image.rows, image.cols + 600, image.type(), cv::Scalar::all(255));

		// Copy image to drawing image
		image.copyTo(drawing(cv::Rect(cv::Point(0, 0), cv::Point(image.cols, image.rows))));


		// Get preprocessed image
		cv::Mat gray, thresh;
		preprocess(image, gray, thresh);

		// Get rectangles
		std::vector<cv::Rect> rectangles = getRectangles(thresh);

		std::string result = getResultString(thresh, rectangles);
		if (checkString(result)) {
			double doubleResult = parse(result);
			std::cout << result << " = " << doubleResult << std::endl;
			// Draw result at image
			for (int i = 0; i < rectangles.size(); ++i)
			{
				cv::rectangle(drawing, rectangles[i], cv::Scalar(0, 0, 255), 3);
				std::string text = "";
				text += result[i];
				cv::putText(drawing, text, rectangles[i].tl(), cv::FONT_HERSHEY_SCRIPT_COMPLEX, 3, cv::Scalar(255, 0, 0), 3);
			}

			// Put string result at image
			std::string stringResult = "=" + std::to_string(doubleResult);
			cv::putText(drawing, stringResult, rectangles[rectangles.size() - 1].br() - cv::Point(0, rectangles[rectangles.size() - 1].height / 2), cv::FONT_HERSHEY_SCRIPT_COMPLEX, 9, cv::Scalar(255, 0, 0), 9);

			// Show result image
			cv::namedWindow("Result", CV_WINDOW_FREERATIO);
			cv::imshow("Result", drawing);
			cv::waitKey(0);
		}
		else {
			std::cout << "Invalid image! Can`t calculate result!" << std::endl;
		}
	}
	else if (userOption == 2)
	{
		std::cout << "  >> Start training SVM classifier\n";

		// Generate data for training
		cv::Mat data;
		std::vector<int> labels;
		generateData(data, labels);

		// Train classifier
		SVMtrain(data, labels);
	}
	else if (userOption == 0)
	{
		std::cout << "  Exit\n";
	}
    return 0;
 }