#include <opencv2\opencv.hpp>

void ResizeBoxes(cv::Rect& box) {
	box.x += cvRound(box.width*0.1);
	box.width = cvRound(box.width*0.8);
	box.y += cvRound(box.height*0.06);
	box.height = cvRound(box.height*0.8);
}

int main() {
	// Open image
	cv::Mat img = cv::imread("picture.jpg");
	if (!img.data) return -1;

	// Initialize HOG descriptor and use human detection classifier coefficients
	cv::HOGDescriptor hog;
	hog.setSVMDetector(cv::HOGDescriptor::getDefaultPeopleDetector());

	// Detect people and save them to detections
	std::vector<cv::Rect> detections;
	hog.detectMultiScale(img, detections, 0, cv::Size(8, 8), cv::Size(32, 32), 1.2, 2);

	// Resize detection boxes and draw them
	for (auto& detection : detections) {
		ResizeBoxes(detection);
		cv::rectangle(img, detection.tl(), detection.br(), cv::Scalar(255, 0, 0), 2);
	}

	// Display picture
	cv::namedWindow("Picture");
	cv::imshow("Picture", img);
	cv::waitKey(0);

	return 0;
}