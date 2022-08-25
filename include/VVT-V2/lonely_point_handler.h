#ifndef LONELY_POINT_HANDLER_H
#define LONELY_POINT_HANDLER_H

// standart headers
#include <iostream>

// OpenCV headers
#include <opencv2/opencv.hpp>

// my headers
#include "VVT-V2/vibrating_point.h"
#include "VVT-V2/histogram.h"

using namespace cv;

// ����� ��������� ������-������� ����� �� �����, ��������� ������� ������������� � �������� ������� ��� ���������� ������� �������� ���� �����
class LonelyPointHandler : public Histogram, public VibratingPoint
{
public:
	LonelyPointHandler(Point2f init_coordinates, int update_rate, double sampling_rate, int point_id, float resizing_coefficient);
	~LonelyPointHandler();
	// ��������� ������ ��� ������ ����� ���������
	void Filter(std::vector<float>& magnitudes);
	// ������������ ����� � ������, ��������� � ���
	void Draw(Mat& frame) override;

private:
	// ������������ �����������
	void DrawHistogram() override;
	// ������������ ����� �� �����
	void DrawPoint(Mat& frame);
	// ������������ ����� ����������� �����
	void DrawPointTrack(Mat& frame);
	// ������������ ������������� ��������������
	void DrawInteractionRectangle(Mat& frame);
	// ������������ ��������� ������ ����� � ������
	void DrawTextData(Mat& frame);


// �������
public:
	// ���������� �����, ����������� ��� ����������� ���������
	Point2f GetTextCoordinates();
	// ���������� ���������� �������������� ��������������
	Rect GetInteractionRect();

private:
	// ��������� ������� ���� �����
	void UpdatePointColor() override;
private:
	// ����������� �������� ��� ��������� ������
	float resizing_coefficient_;
	// ID ����� (��� ���� �����)
	int point_id_;
	// ������� ���������� �������, ����� � ��
	int update_rate_;
	// ���� �����
	Scalar point_color_;

};

#endif