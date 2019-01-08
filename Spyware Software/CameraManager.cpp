#include "CameraManager.h"

CameraManager::CameraManager()
{
	this->capture = false;
	this->udp_soc = INVALID_SOCKET;
}

CameraManager::~CameraManager()
{
	this->TurnOff();
}

void CameraManager::TurnOn()
{
	this->udp_soc = Communicator::UDP::createSocket();
	cv::VideoCapture cap;
	cap.open(0);

	if (!cap.isOpened())
	{
		std::cerr << "***Could not initialize capturing...***\n";
		return;
	}
	cv::Mat frame;
	cap >> frame;

	std::string frameSize = std::to_string(frame.total() * frame.elemSize());
	Communicator::UDP::Send(this->udp_soc, CAMERA_LIVEFEED_PORT, (const void *)frameSize.c_str(), (int)frameSize.length());

	this->capture = true;
	this->cap_thread = thread(&CameraManager::Stream, this, cap);

	return;
}

void CameraManager::TurnOff()
{
	this->capture = false;
	if (this->cap_thread.joinable())
	{
		this->cap_thread.join();
	}
	this->udp_soc = Communicator::UDP::destroySocket(this->udp_soc);
}

void CameraManager::Stream(cv::VideoCapture cap)
{
	cv::Mat frame;
	while (this->capture)
	{
		cap >> frame;

		if (!(frame.empty())) {
			this->SendFeed(frame);

		}

		cv::waitKey(100);
	}
}

void CameraManager::SendFeed(cv::Mat frame)
{
	int frameSize = (int)(frame.total() * frame.elemSize());//frame size
	int count = 0;
	while (count + UDP_PACKET_BUFFER_SIZE  < frameSize)
	{
		Communicator::UDP::Send(this->udp_soc, CAMERA_LIVEFEED_PORT, frame.data + count, UDP_PACKET_BUFFER_SIZE);
		count += UDP_PACKET_BUFFER_SIZE;
	}
	Communicator::UDP::Send(this->udp_soc, CAMERA_LIVEFEED_PORT, frame.data + count, frameSize - count);
}
