#pragma once
#include <opencv2/opencv.hpp>
#include <opencv2/video/tracking.hpp>
#include <thread>
#include "Communicator.h"

using namespace std;

class CameraManager
{
public:
	CameraManager();
	~CameraManager();
	void TurnOn();
	void TurnOff();

private:
	void Stream(cv::VideoCapture cap);
	void SendFeed(cv::Mat frame);

	bool capture;
	thread cap_thread;
	SOCKET udp_soc;
};
