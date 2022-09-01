#ifndef VIBRATION_DETECTOR_H
#define VIBRATION_DETECTOR_H

// VREMENNO
#define MAIN_WINDOW_NAME "Video"
#define V_MONITOR_WINDOW_NAME "Vibration Monitor"

// standart headers
#include <iostream>
#include <vector>

// OpenCV headers
#include <opencv2/video/tracking.hpp>

// my headers
#include "VVT-V2/lonely_point_handler.h"
#include "VVT-V2/colored_point_handler.h"
#include "VVT-V2/frame_handler.h"
#include "VVT-V2/helper.h"

class VibrationDetector 
{
	enum main_mode
	{
		DEFAULT,
		SELECTINGROI,
		PAUSE
	};

	enum colored_point_mode
	{
		COLORING_DEFAULT,
		COLORING_BASED_ON_FREQUENCY,
		COLORING_BASED_ON_AMPLITUDE
	};

public:
	VibrationDetector(std::string input_file_name, std::string output_file_name, std::string window_name);
	~VibrationDetector();
	// ��������� �������� ��������
	void ExecuteVibrationDetection();

private:
	// ����������� ������� �� �������� � ��������. ���������� ��� ����, ����� �� �������� ��������� ����� �� ����� ���������� ��������� �����
	void ServeTheQueues();
	// ������� "�������" �����
	void CreateNewPoint(Point2f mouse_coordinates);
	// ���������� ����� ������
	void LeftClickHandler(Point2f mouse_coordinates);
	// ���������� "�������" �����
	void CreateNewColoredPoint(Point2f mouse_coordinates);
	// ������� ������ "�������" �����
	void DeletePoints(Point2i mouse_coordinates);
	// ������� ������� "�������" �����
	void DeleteColoredPoints();
	// ������� "�������" � "�������" �����, ���� ������� ������� � ��� ������� ������
	void FindAndDeleteUncofidentPoints();
	// callback functions for detecting the click
	static void OnMouse(int event, int x, int y, int flags, void* userdata);
	void DetectEvent(int event, int x, int y, int flags);
	// ������� �����
	void DefaultModeHandler(int event, int x, int y);
	// ����� ��������� ������� ��������
	void RoiSelectionModeHandler(int event, int x, int y);
	// ������� ������������� ���������
	Mat MakeWarpedFrame(Mat frame, std::vector<Point2i> warping_figure);
	// ������� ����� ���������� ������-������ � �������� ���
	void TrackAndCalc();
	// ������� ���������� ��� �������� ����� �� �����������
	std::vector<Point2f> FindGoodFeatures(Mat frame, Rect roi);
	// ������ ����� � ������� ������
	void DrawDebugLkWinRectangle(Mat& frame);
	// ����������� ���������� ������������ ���� � ������������ ��������� �������������� ����
	Point2f TranslateCoordinates(Point2f point);
	// ������������, ������ � ������� �� ����� ������������ ���� � �������
	void DrawAndOutput(Mat& frame);

private:
	// ��������������
	FrameHandler* frame_handler;
	// ���� ���������� ���������
	bool running_;
	// ����������� ���������� �������� ���� � ����������� �� ���������� ��������� �����
	float res_mp_;
	// ������-id (����������� ����� �����)
	int point_id_;
	// ��������� ������� ������� � ������� ����
	Point last_mouse_position_;
	// �������� ����� ������ (enum main_mode)
	int current_mode_;
	// ���� ��� �������������� ������
	bool fullscreen_;

	// NORMAL MODE
	// 
	// ������� �� �������� "�������" �����
	std::vector<Point2i> l_click_queue_;
	// ������� �� �������� "�������" �����
	std::vector<Point2f> c_point_queue_;
	// ������� �� �������� �����
	std::vector<Point2i> delete_queue_;
	// ������� �� ��������� ���������
	std::vector<Point2i> histograms_queue_;
	// ���������� ������� �����
	std::vector<Point2f> previous_points_coordinates_;
	// Single points handling
	std::vector<LonelyPointHandler*> vec_lonely_point_handlers_;
	// Colored points handling
	std::vector<ColoredPointHandler*> vec_colored_point_handlers_;
	// ������������ ��������� ����� �� ������
	Point3f current_amplitude_;
	double max_amplitude_colored_points_value_;

	// ADJUST SENSIVITY MODE
	int sensivity_in_percents_;

	// �� ����������� �����
	Mat current_tracking_frame_;
	// ����������� �����
	Mat frame_to_be_shown_;
	// ����� � ����������
	Mat grad_scale_;
	// ����� ��������� ������� �����
	int colored_point_mode_;

	// R MODE
	// 
	// ���� ������������� ��������� ROI
	bool roi_selecting_;
	// ���� �������������� ��������� ROI
	bool roi_selected_;
	// 
	// ���������� ������ ��������������� ����� �� ���� ��������� �������������� (top left, bottom left, top right, bottom right - ������ � ����� ������� ��������� ROI)
	Point2i tl_click_coords_;
	

	int point_offset_;
	Point point_to_be_deleted_;
	bool intersection_;
	std::vector<bool> vec_of_intersections_;
	/*int point_id_;*/
	bool interaction_;
	std::vector<bool> vec_of_interacts_;
	std::vector<Point> text_coords_;

	std::string input_file_name_;
	std::string output_file_name_;
	std::string output_csv_file_name_;
	std::string window_name_;

	String main_window_name_;
	String v_monitor_window_name_;
	
	// for detecting the click
	bool point_selected_;
	Point2i click_coords_;

	// for rectangle
	Mat unchanged_frame_;
	Point2i br_click_coords_;
	Point2i mouse_move_coords_;
	bool right_button_down_;
	bool colors_inited_;
	std::vector<Point2f> prev_vibrating_pts_;
	std::vector<Point2f> next_vibrating_pts_;

	// for warping rectangle
	bool warping_figure_selecting_;
	std::vector<Point> warping_figure_;

	// for Lucas-Kanade tracking
	Mat prev_img_gray_;
	Mat next_img_gray_;
	std::vector<Point2f> prev_pts_;
	std::vector<Point2f> next_pts_;
	std::vector<Point2f> contour_prev_pts_;
	std::vector<Point2f> contour_next_pts_;
	std::vector<uchar> status_; // 1 if features found, 0 if not
	std::vector<uchar> rect_status_;
	// ������ ���� ��� ��������� ������-������
	int lk_win_size_;
	// ���������� ����� �������� ����������� � ��������� �-�
	int level_amount_;

	// useful parameters
	int number_of_points_;
	int number_of_vibrating_pts_;
	bool vibration_inited_;

	// Fast Fourier Transform
	int fps_;
	int update_rate_;
	double frame_time_;
};

#endif