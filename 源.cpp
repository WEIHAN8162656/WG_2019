

#include <iostream>
#include <opencv.hpp>
#include <vector>
#include <math.h>
#include<chrono>
#include <string>

using namespace std;
using namespace cv;
//void Armorbright(Mat src, Mat dst, double ArmorContrast, double ArmorBright);
//void Armorpreprocess(Mat src, Mat dst, Mat preprocess);
void RectArmor(Mat dst, vector<vector<Point> >Armorcoutour);
void Armorpt(vector<RotatedRect> ArmorRect);
void ArmorDrawarmor(Mat &img, Point center, int size, Scalar color, int thickness);
float light_max_sizes = 1.0; 
float light_min_areas = 0.5;




bool Armorflags = false;
vector<Point>ArmorCenter;
vector<vector<Point>> Armorcoutours;
vector<vector<Point> >Armorcoutour;
vector<RotatedRect> ArmorRect;
vector<RotatedRect> ArmorRects(Mat src, vector<vector<Point>> Armorcontour);
int main()
{
	//vector<RotatedRect> ArmorRect;
	//Mat armor_wg = imread("E:\\the open\\picutre_WG\\NO338.jpg", 1);
	//Mat armor_wg = imread("E:\\the open\\picutre_WG\\NO202.jpg", 1);
     //Mat armor_wg = imread("E:\\the open\\picutre_WG\\2233.jpg", 1);
	//Mat armor_wg = imread("E:\\the open\\picutre_WG\\1.bmp", 1);
     Mat armor_wg = imread("E:\\the open\\picutre_WG\\4.png", 1);
	Mat delate[10] ;
	
	namedWindow("picutre_text1", 1);
	namedWindow("picutre_text2", 1);
	namedWindow("picutre_text3", 1);
	namedWindow("picutre_text4", 1);
	namedWindow("picutre_text5", 1);
	resize(armor_wg, armor_wg, Size(640, 480));

	cvtColor(armor_wg, delate[1], CV_RGB2HSV);
	inRange(delate[1], Scalar(0, 120, 140), Scalar(10, 255, 255), delate[2]);
	morphologyEx(delate[2], delate[2], 1,(5, 5));
	findContours(delate[2], Armorcoutours, RETR_CCOMP, CHAIN_APPROX_SIMPLE);
	ArmorRects(delate[1], Armorcoutours);
	Armorpt(ArmorRect);
	RectArmor(armor_wg, Armorcoutour);
	//ArmorDrawarmor(armor_wg, ArmorCenter[0],100, Scalar(0, 0, 255), LINE_AA);
	//imwrite("E:\\the open\\picutre_wg\\hsv_blue1.jpg", delate[2]);
	imshow("picutre_text1", armor_wg);
	imshow("picutre_text3", delate[1]);
	imshow("picutre_text4", delate[2]);
	waitKey();
}









/*void Armorbright(Mat src, Mat dst, double ArmorContrast, double ArmorBright)
{
	uchar nvals;
	for(size_t NI = 0; NI < src.rows; NI++)
	{
		Vec3b* src_ptr = src.ptr<Vec3b>(NI);
		Vec3b* dst_ptr = dst.ptr<Vec3b>(NI);
		for (size_t NL = 0; NL < src.cols; NL++)
		{
			for (int channels = 0; channels < 3; channels++)
			{
				nvals = saturate_cast<uchar>(ArmorContrast * dst_ptr[NL][channels] + ArmorBright);

				dst_ptr[NL][channels] = nvals;
			}
		}
	}
}

/*
* @brief preprocessing  image
* @return none
*/
/*void Armorpreprocess(Mat src, Mat dst, Mat preprocess)
{
	Mat  bgr[3];
	split(src, bgr);
	Mat b_image = bgr[0];
	Mat g_image = bgr[1];
	Mat r_image = bgr[2];
	for (size_t NI = 0; NI < src.rows; NI++)
	{
		uchar* p1 = b_image.ptr<uchar>(NI);
		uchar* p2 = g_image.ptr<uchar>(NI);
		uchar* p3 = r_image.ptr<uchar>(NI);
		uchar* p4 = dst.ptr<uchar>(NI);
		for (size_t NL = 0; NL < src.cols; NL++)
		{
			if ((p1[NL] - p2[NL]) >10 || (p3[NL] - p2[NL]) > 10)
			{
				p4[NL] = 0;
			}
			else
			{
				p4[NL] = 255;
			}
		}
	}
	morphologyEx(dst, preprocess, MORPH_CLOSE,(5,5), Point(-1, -1), 1, 0);
}
*/
/*
* @brief count ArmorRect of contour
* @param
* @return none
*/
vector<RotatedRect> ArmorRects(Mat src,  vector<vector<Point>> Armorcoutours)
{
	//RotatedRect lightrect;//Ðý×ª¾ØÕó
    CvScalar armorcolor;//gray 
	//const* q = (*arr)(src);
	for (auto &contour : Armorcoutours)
	{
		Armorflags = true;
		if (contour.size() > 10)
		{
		
				RotatedRect lightrect = fitEllipse(contour);//ÍÖÔ²
				float lightrectarea = contourArea(contour);//Ðý×ª¾ØÕóÃæ»ý
				if (lightrect.size.width / lightrect.size.height< light_max_sizes || lightrectarea / lightrect.size.area() > light_min_areas)//area and size
				{
					
					if (Armorflags)
					{
						ArmorRect.push_back(lightrect);
						Armorcoutour.push_back(contour);
					}
				}			
		}
	}
	return ArmorRect;
}
/*
* @brief find the position of armor
* @param
* @return
*/
void Armorpt(vector<RotatedRect> ArmorRect)
{
	float Aangle;
	Point armorsize;
	Point center_armor;
	size_t size = ArmorRect.size();
	if (size < 2)
	{
		return;
	}
	for (unsigned int NI = 0; NI < size; NI++)
	{
		for (unsigned int NL = 0; NL < size; NL++)
		{
			Aangle = ArmorRect[NI].angle - ArmorRect[NL].angle;
			if (Aangle > 180)
			{
				Aangle -= 180;
			}
			if (abs(Aangle) < 5 || abs(Aangle - 180) < 5)
			{
				if (abs(ArmorRect[NI].size.height - ArmorRect[NL].size.height) < 5 && abs(ArmorRect[NI].size.width - ArmorRect[NL].size.width) < 5)
				{
					center_armor.y = (ArmorRect[NI].center.y + ArmorRect[NL].center.y)*0.5;
					center_armor.x = (ArmorRect[NI].center.x + ArmorRect[NL].center.x)*0.5;
					ArmorCenter.push_back(center_armor);
				/*	armorsize.y = (ArmorRect[NI].size.height + ArmorRect[NL].size.height)*0.5;
					armorsize.x = sqrt(pow(ArmorRect[NI].size.height - ArmorRect[NL].size.height, 2) + pow(ArmorRect[NI].size.width - ArmorRect[NL].size.width, 2));
					if (armorsize.x > armorsize.y)
					{
						swap(armorsize.x, armorsize.y);
					}
					ArmorSIZE.push_back(armorsize);*/
				}
			}
		}
	}
}
/*
* @brief draw ArmorRect in image
* @param
* @return
*/
void ArmorDrawarmor(Mat &img, Point center, int size, Scalar color, int thickness)
{
	int XI, XL, YI, YL;
	Point p1, p2, p3, p4;
	XI = center.x - size ? center.x - size : 0;
	XL = center.x + size;
	YI = center.y - size ? center.y - size : 0;
	YL = center.y + size;
	p1 = Point(XI, center.y);
	p2 = Point(XL, center.y);
	p3 = Point(center.x, XI);
	p4 = Point(center.x, XL);
	line(img, p3, p4, color, thickness, 8, 0);
	line(img, p1, p2, color, thickness, 8, 0);

}
/*
* @brief buff space of image
* @
*/
void RectArmor(Mat dst,vector<vector<Point>>Armorcoutour)
{
	for (auto &i : Armorcoutour)
	{

		RotatedRect box = minAreaRect(i);
		Point2f vertex[4];
		box.points(vertex);
		for (int j = 0; j < 4; j++)
		{
			line(dst, vertex[j], vertex[(j + 1) % 4], Scalar(124, 255, 124), 2, LINE_AA);
		}
	}

}
/*
* @brief rect the armor
* @
*/


