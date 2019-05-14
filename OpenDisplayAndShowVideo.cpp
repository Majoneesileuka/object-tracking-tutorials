#include "opencv2\opencv.hpp"
#include "opencv2\highgui.hpp"

int main() {

	// Create video capturing object
	// 0 opens default camera, otherwise filename as argument
	cv::VideoCapture video(0);

	// Check that video is opened
	if (!video.isOpened()) return -1;

	// For saving the frame
	cv::Mat frame;

	// Get video resolution
	int frameWidth = video.get(cv::CAP_PROP_FRAME_WIDTH);
	int frameHeigth = video.get(cv::CAP_PROP_FRAME_HEIGHT);

	// Create video writer object
	cv::VideoWriter output("output.avi", cv::VideoWriter::fourcc('M', 'J', 'P', 'G'), 30, cv::Size(frameWidth, frameHeigth));

	// Loop through available frames
	while (video.read(frame)) {

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
