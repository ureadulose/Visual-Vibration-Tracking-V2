#include "VVT-V2\vibration_displayer.h"

VibrationDisplayer::VibrationDisplayer(std::string window_name, int frame_width, int frame_height)
	:window_name_{ window_name },
	frame_width_{ frame_width },
	frame_height_{ frame_height },
	mode_{ -1 }
{
	
}

void VibrationDisplayer::Init()
{
	// making a window
	// uncomment to show in another window
	//namedWindow(window_name_, WINDOW_AUTOSIZE);
	frame_ = Mat(frame_height_, frame_width_, CV_8UC3, Scalar(0, 0, 0));
}

bool VibrationDisplayer::InitColors()
{
	if (points_.empty())
		return false;

	if (!colors_.empty())
		return false;

	for (int i = 0; i < points_.size(); i++)
	{
		colors_.push_back(Scalar(0, 0, 255));
	}
	return true;
}

Mat VibrationDisplayer::AddTipText(Mat frame)
{
	std::vector<std::string> tip_text = { "SPACE pause", "C calib mode for amplitude", "R set ROI", "Q quit" };
	Point local_offset = Point(0, 20);
	Point top_left_offset = Point(5, local_offset.y);
	switch (mode_)
	{
	case -1:
	{
		putText(/*frame_*/frame, tip_text[0], top_left_offset, FONT_HERSHEY_PLAIN, 1, Scalar(255, 100, 100), 1, 8, false);
		putText(/*frame_*/frame, tip_text[1], top_left_offset + local_offset, FONT_HERSHEY_PLAIN, 1, Scalar(255, 100, 100), 1, 8, false);
		putText(/*frame_*/frame, tip_text[2], top_left_offset + local_offset * 2, FONT_HERSHEY_PLAIN, 1, Scalar(255, 100, 100), 1, 8, false);
		putText(/*frame_*/frame, tip_text[3], top_left_offset + local_offset * 3, FONT_HERSHEY_PLAIN, 1, Scalar(255, 100, 100), 1, 8, false);
		break;
	}
	case 0:
	{

		putText(/*frame_*/frame, tip_text[0], top_left_offset, FONT_HERSHEY_PLAIN, 1, Scalar(255, 100, 100), 1, 8, false);
		break;
	}
	case 1:
	{
		putText(/*frame_*/frame, tip_text[1], top_left_offset + local_offset, FONT_HERSHEY_PLAIN, 1, Scalar(255, 100, 100), 1, 8, false);
		break;
	}
	case 2:
	{
		putText(/*frame_*/frame, tip_text[2], top_left_offset + local_offset * 2, FONT_HERSHEY_PLAIN, 1, Scalar(255, 100, 100), 1, 8, false);
		break;
	}
	case 3:
	{
		putText(/*frame_*/frame, tip_text[3], top_left_offset + local_offset * 3, FONT_HERSHEY_PLAIN, 1, Scalar(255, 100, 100), 1, 8, false);
		break;
	}
	}
	return frame;
}

void VibrationDisplayer::ProcessFrame(Mat tmp_frame)
{
	// "le kostyl", going to be fixed later
	tmp_frame_ = tmp_frame;
	//ClearFrame();

	// drawing ROI and updating colors
	//UpdateDisplayingRectangle();
	UpdateColors();
	//std::cout << colors_.size() << std::endl;

	for (int i = 0; i < points_.size(); i++)
	{
		// drawing points with their own colors - frequencies
		circle(/*frame_*/tmp_frame_, points_[i], 1, colors_[i], FILLED);
	}

	AddGradient();
	tmp_frame_ = AddTipText(tmp_frame_);
}

Mat VibrationDisplayer::GetFrame()
{
	return /*frame_*/tmp_frame_;
}

void VibrationDisplayer::SetRoi(Rect roi)
{
	roi_ = roi;
}

void VibrationDisplayer::UpdateFrequencies(std::vector<float> frequencies, double range)
{
	frequencies_ = frequencies;
	range_ = range;
}

void VibrationDisplayer::UpdateDisplayingPoints(std::vector<Point2f> points)
{
	points_ = points;
}

void VibrationDisplayer::SetMode(int mode)
{
	mode_ = mode;
}

void VibrationDisplayer::ClearFrame()
{
	frame_ = Mat(frame_height_, frame_width_, CV_8UC3, Scalar(0, 0, 0));
}

void VibrationDisplayer::UpdateDisplayingRectangle()
{
	rectangle(frame_, roi_.tl(), roi_.br(), Scalar(0, 255, 0), 1);
}

void VibrationDisplayer::UpdateColors()
{
	// translate frequency [0; range_] to [0; 255] int color value;
	if (!frequencies_.empty())
	{
		//std::cout << "UPDATING COLORS" << std::endl;
		//std::cout << frequencies_.size() << std::endl;
		//std::cout << points_.size() << std::endl;
		
		colors_.clear();
		for (int i = 0; i < points_.size(); i++)
		{
			std::vector<int> color = Rgb(frequencies_[i] / range_);
			colors_.push_back(Scalar(color[0], color[1], color[2]));
		}
	}
	else
	{
		for (int i = 0; i < points_.size(); i++)
		{
			colors_.push_back(Scalar(0, 0, 0));
		}
	}
}

void VibrationDisplayer::AddGradient()
{
	// ��� ����� ������������, ���� �� ���� ��������((
	// ����� ��������� ������

	Point top_left = Point(15, frame_.rows - 150);
	Point bottom_right = Point(frame_.cols - 15, frame_.rows - 50);
	Rect gradient_bounds(top_left, bottom_right);
	rectangle(/*frame_*/tmp_frame_, gradient_bounds.tl(), gradient_bounds.br(), Scalar(0, 255, 0), 1);

	Point position;
	position = Point(top_left.x - 2, bottom_right.y + 15);

	for (int i = 0; i <= 6; i++)
	{
		putText(/*frame_*/tmp_frame_, std::to_string(i * 3), position, FONT_HERSHEY_PLAIN, 1, Scalar(255, 255, 255), 1);
		position = Point(position.x + (frame_.cols - 30)/5 - 1, bottom_right.y + 15);
	}

	int rect_width = bottom_right.x - top_left.x;
	int rect_height = top_left.y - bottom_right.y;

	int slices = 32;
	int dx = rect_width / slices;
	Point tl = gradient_bounds.tl();
	Point br = Point(tl.x + dx, bottom_right.y);

	for (int i = 0; i < slices; i++)
	{
		Scalar scalar_color;
		std::vector<int> vec_color = Rgb((double)(i) / (double)(slices));
		//std::cout << vec_color[0] << std::endl;

		scalar_color[0] = vec_color[0];
		scalar_color[1] = vec_color[1];
		scalar_color[2] = vec_color[2];

		rectangle(/*frame_*/tmp_frame_, tl, br, scalar_color, FILLED);

		tl = Point(tl.x + dx, tl.y);
		br = Point(br.x + dx, br.y);

	}
}

// not mine, taken from stackoverflow
std::vector<int> VibrationDisplayer::Rgb(double ratio)
{
	// we want to normalize ratio so that it fits in to 6 regions
	// where each region is 256 units long
	int normalized = int(ratio * 256 * 4);

	// find the region for this position
	int region = normalized / 256;

	// find the distance to the start of the closest region
	int x = normalized % 256;

	int r = 0, g = 0, b = 0;
	switch (region)
	{
	case 0: r = 255; g = 0;   b = 0;   g += x; break;
	case 1: r = 255; g = 255; b = 0;   r -= x; break;
	case 2: r = 0;   g = 255; b = 0;   b += x; break;
	case 3: r = 0;   g = 255; b = 255; g -= x; break;
	//case 4: r = 0;   g = 0;   b = 255; r += x; break;
	//case 5: r = 255; g = 0;   b = 255; b -= x; break;
	}
	return { r, g, b };
}

//void VibrationDisplayer::ContourHandler(std::vector<std::vector<Point>> contour_shapes)
//{
//	std::vector<Point> contour_points;
//	// ������������� ������ ��������� ������
//	for (int i = 0; i < contour_shapes.size(); i++)
//	{
//		
//	}
//	// imshow
//}
//
//void VibrationDisplayer::GetContourHeadPoints(std::vector<std::vector<Point>> contour_shapes, std::vector<Point>& contour_head_points)
//{
//	// ������������� ������ ��������� ������
//	for (int i = 0; i < contour_shapes.size(); i++)
//	{
//		std::cout << "cum" << std::endl;
//		if (!contour_shapes[i].empty())
//		{
//			std::cout << "cum" << std::endl;
//			contour_head_points.push_back(contour_shapes[i][0]);
//		}
//	}
//}


