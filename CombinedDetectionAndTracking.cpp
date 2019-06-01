#include "opencv2\opencv.hpp"
#include "opencv2\highgui.hpp"
#include "opencv2\tracking.hpp"

void ResizeBoxes(cv::Rect& box) {
	box.x += cvRound(box.width*0.1);
	box.width = cvRound(box.width*0.8);
	box.y += cvRound(box.height*0.06);
	box.height = cvRound(box.height*0.8);
}

int main() {

	// Create video capturing object
	// 0 opens default camera, otherwise filename as argument
	cv::VideoCapture video("video.mp4");

	// Check that video is opened
	if (!video.isOpened()) return -1;

	// For saving the frame
	cv::Mat frame;

	// Get video resolution
	int frameWidth = video.get(cv::CAP_PROP_FRAME_WIDTH);
	int frameHeigth = video.get(cv::CAP_PROP_FRAME_HEIGHT);

	// Create video writer object
	cv::VideoWriter output("output.avi",
		cv::VideoWriter::fourcc('M', 'J', 'P', 'G'),
		30,
		cv::Size(frameWidth, frameHeigth));

	// Create multi tracker
	video.read(frame);
	cv::Ptr<cv::MultiTracker> multiTracker = cv::MultiTracker::create();

	// Initialize HOG descriptor and use human detection classifier coefficients
	cv::HOGDescriptor hog;
	hog.setSVMDetector(cv::HOGDescriptor::getDefaultPeopleDetector());

	// Detect people and save them to detections and initialize multi tracker based on these
	std::vector<cv::Rect> detections;
	hog.detectMultiScale(frame, detections, 0, cv::Size(8, 8), cv::Size(32, 32), 1.2, 2);

	for (auto& detection : detections) {
		ResizeBoxes(detection);
		multiTracker->add(cv::TrackerKCF::create(), frame, detection);
	}

	int frameNumber = 1;
	// Loop through available frames
	while (video.read(frame)) {
		frameNumber++;

		// Every 15 frames reinitialize the multitracker with new set of detections
		if (frameNumber % 15 == 0) {
			detections.clear();
			hog.detectMultiScale(frame, detections, 0, cv::Size(8, 8), cv::Size(32, 32), 1.2, 2);
			cv::Ptr<cv::MultiTracker> multiTrackerTemp = cv::MultiTracker::create();
			multiTracker = multiTrackerTemp;
			for (auto& detection : detections) {
				ResizeBoxes(detection);
				multiTracker->add(cv::TrackerKCF::create(), frame, detection);
			}
		}
		else {
			multiTracker->update(frame);
		}

		// Update the multi tracker and draw rectangles around objects
		for (const auto& object : multiTracker->getObjects()) {
			cv::rectangle(frame, object, cv::Scalar(255, 0, 0), 2, 8);
		}

		// Display the frame
		cv::imshow("Video feed", frame);

		// Write video frame to output
		output.write(frame);

		// For breaking the loop
		if (cv::waitKey(25) >= 0) break;

	} // end while (video.read(frame))

	  // Release video capture and writer
	output.release();
	video.release();

	// Destroy all windows
	cv::destroyAllWindows();

	return 0;

}