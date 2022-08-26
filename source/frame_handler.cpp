#include "VVT-V2\frame_handler.h"

FrameHandler::FrameHandler(const std::string input_file_name, const std::string output_file_name, const std::string window_name) :
	input_path_{ input_file_name },
	input_cap_{ new VideoCapture(input_file_name) },
	input_cap_status_{ false },
	output_path_{ output_file_name },
	output_cap_{ new VideoWriter(output_file_name, VideoWriter::fourcc('D', 'I', 'V', 'X'), input_cap_->get(CAP_PROP_FPS), Size(input_cap_->get(CAP_PROP_FRAME_WIDTH), input_cap_->get(CAP_PROP_FRAME_HEIGHT))) },
	window_name_{ window_name },
	current_time_of_frame_{ 0 },
	current_pos_of_frame_{ 0 },
	input_fps_{ input_cap_->get(CAP_PROP_FPS) },
	input_frame_height_{ input_cap_->get(CAP_PROP_FRAME_HEIGHT) },
	input_frame_width_{ input_cap_->get(CAP_PROP_FRAME_WIDTH) },
	resizing_coefficient_{ 1.0f }
{
	std::cout << "Input fps is: " << input_fps_ << std::endl;
	// ������ ����
	namedWindow(window_name_);
	// ������� ����������� ��������������� � ������ ������������������� �����
	ResizeResolution();
	// �������������� ����� ���������
	tip_text_.push_back("Default Mode");
	tip_text_.push_back("ROI selecting mode (Press 'R')");
	tip_text_.push_back("Pause (Press 'Space')");
}

FrameHandler::~FrameHandler()
{
	destroyWindow(window_name_);
	this->input_cap_->release();
	delete this->input_cap_;
	this->output_cap_->release();
	delete this->output_cap_;
}
void FrameHandler::ReadNextFrame()
{
	input_cap_status_ = input_cap_->read(input_frame_);
	current_time_of_frame_ = input_cap_->get(CAP_PROP_POS_MSEC);
	current_pos_of_frame_ = input_cap_->get(CAP_PROP_POS_FRAMES);
	input_frame_.copyTo(current_frame_);
}

void FrameHandler::ShowFrame(Mat frame)
{
	imshow(window_name_, frame);
}

void FrameHandler::WriteFrame(Mat frame)
{
	output_cap_->write(frame);
}

Mat FrameHandler::AddTips(Mat frame, int mode)
{
	// ��������� ������
	int font = FONT_HERSHEY_PLAIN;
	double font_scale = 1;
	int thickness = 2;
	int baseline = 0;
	int line_spacing = 5;
	Scalar default_color(150, 150, 150);
	Scalar mode_color(100, 255, 100);
	for (int i = 0; i < tip_text_.size(); i++)
	{
		Scalar font_color = default_color;
		if (i == mode)
			font_color = mode_color;
		putText(
			frame,
			tip_text_[i],
			Point2i(15 * (1 / resizing_coefficient_), 15 * (1 / resizing_coefficient_) + i * (getTextSize(tip_text_[i], font, font_scale, thickness, &baseline).height + line_spacing)),
			font,
			font_scale,
			font_color,
			thickness,
			4
		);
	}
	return frame;
}

Mat FrameHandler::GetGrayFrame(Mat frame_to_be_grayed)
{
	Mat grayed_frame;

	cvtColor(frame_to_be_grayed, grayed_frame, COLOR_BGR2GRAY);
	
	if (!frame_to_be_grayed.empty())
	{		
		GaussianBlur(grayed_frame, grayed_frame, Size(3, 3), 0);
	}
	
	return grayed_frame;
}

Mat FrameHandler::ResizeFrame(Mat frame)
{
	if (!resized_resolution_.empty())
		resize(frame, frame, resized_resolution_);
	return frame;
}

Mat FrameHandler::ConcatenateFrames(Mat left_frame, Mat right_frame)
{
	Mat dst;
	std::vector<Mat> vec_of_frames;
	vec_of_frames.push_back(left_frame);
	vec_of_frames.push_back(right_frame);
	hconcat(vec_of_frames, dst);
	return dst;
}

bool FrameHandler::GetInputCapStatus()
{
	return input_cap_status_;
}

Mat FrameHandler::GetCurrentFrame()
{
	return current_frame_;
}

String FrameHandler::GetWindowName()
{
	return window_name_;
}

double FrameHandler::GetCurrentTimeOfFrame()
{
	return current_time_of_frame_;
}

int FrameHandler::GetCurrentPosOfFrame()
{
	return current_pos_of_frame_;
}

int FrameHandler::GetAmountOfFrames()
{
	return input_cap_->get(CAP_PROP_FRAME_COUNT);
}

int FrameHandler::GetInputFps()
{
	return input_fps_;
}

int FrameHandler::GetFrameWidth()
{
	return input_frame_width_;
}

int FrameHandler::GetFrameHeight()
{
	return input_frame_height_;
}

float FrameHandler::GetResizingCoefficient()
{
	return resizing_coefficient_;
}

void FrameHandler::ResizeResolution()
{
	std::cout << "frame width" << input_frame_width_ << std::endl;
	std::cout << "frame height" << input_frame_height_ << std::endl;
	if (input_frame_width_ > input_frame_height_)
	{
		switch (static_cast<int>(input_frame_height_))
		{
		case (480):
		{
			resizing_coefficient_ = 2.0;
			resized_resolution_ = Size(input_frame_width_ * resizing_coefficient_, input_frame_height_ * resizing_coefficient_);
			break;
		}
		case (720):
		{
			resizing_coefficient_ = 1.0;
			resized_resolution_ = Size(input_frame_width_, input_frame_height_);
			break;
		}
		case (1080):
		{
			resizing_coefficient_ = 0.67;
			resized_resolution_ = Size(input_frame_width_ * resizing_coefficient_, input_frame_height_ * resizing_coefficient_);
			break;
		}
		case (1520):
		{
			resizing_coefficient_ = 0.25;
			resized_resolution_ = Size(input_frame_width_ * resizing_coefficient_, input_frame_height_ * resizing_coefficient_);
			break;
		}
		case (2160):
		{
			resizing_coefficient_ = 0.25;
			resized_resolution_ = Size(input_frame_width_ * resizing_coefficient_, input_frame_height_ * resizing_coefficient_);
			break;
		}
		}
	}
	else
	{
		switch (static_cast<int>(input_frame_width_))
		{
		case (480):
		{
			resizing_coefficient_ = 2.0;
			resized_resolution_ = Size(static_cast<int>(input_frame_width_ * resizing_coefficient_), static_cast<int>(input_frame_height_ * resizing_coefficient_));
			break;
		}
		case (720):
		{
			resizing_coefficient_ = 1.0;
			resized_resolution_ = Size(static_cast<int>(input_frame_width_), static_cast<int>(input_frame_height_));
			break;
		}
		case (1080):
		{
			resizing_coefficient_ = 0.5;
			resized_resolution_ = Size(input_frame_width_ * resizing_coefficient_, input_frame_height_ * resizing_coefficient_);
			break;
		}
		case (1520):
		{
			resizing_coefficient_ = 0.25;
			resized_resolution_ = Size(input_frame_width_ * resizing_coefficient_, input_frame_height_ * resizing_coefficient_);
			break;
		}
		case (2160):
		{
			resizing_coefficient_ = 0.25;
			resized_resolution_ = Size(input_frame_width_ * resizing_coefficient_, input_frame_height_ * resizing_coefficient_);
			break;
		}
		}
	}
}
