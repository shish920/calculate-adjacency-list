#include<opencv2/opencv.hpp>
#include<math.h>

using namespace std;
using namespace cv;


vector<vector<int>> adjacent_list;	//adjectcent list is used to record the relationship between dots
int draw_flag = 0;						//the flag is to record mouse moved or not while left bottom is down; 0 not, 1 yes
//int count = 0;							//count numbers, represent the index of dots

class dot
{
public:
	dot() { _point = { 0,0 }; _number = -1; };
	dot(Point2f point, int number) { _point = point; _number = number; }
	Point2f _point;	//coordinate
	int _number;		//the index in the dots list
}A, B;

vector<dot> dots;			//dots list

//calculate Euclidean distance
double GetDistane(Point2f A, Point2f B)
{
	return sqrtf(powf(A.x - B.x, 2) + powf(A.y - B.y, 2));
}

//mouse callback function
void OnMouse(int _event, int _x, int _y, int _flags, void* _param)
{
	Mat &image = *(Mat*)_param;

	if (_event == CV_EVENT_LBUTTONDOWN)
	{
		//鼠标左键,创建节点
		//A._point = Point2f(_x, _y);
		//A._number = dots.size();
		A = dot(Point2f(_x, _y), dots.size());//use dots list's size to count
		int replace_flag = 0;
		if (!dots.empty())
		{	
			for (dot exist_dot : dots)
			{
				//if A is close to the existing dot, replace existing dot to A
				if (GetDistane(exist_dot._point, A._point) < 30)
				{
					A = exist_dot;
					replace_flag = 1;
				}
			}
		}
		if (replace_flag == 0)
		{
			dots.push_back(A);
			vector<int> Avec;
			adjacent_list.push_back(Avec);
			adjacent_list[A._number].push_back(-1);
			circle(image, A._point, 2, Scalar(255, 255, 0), 2);
			putText(image, to_string(A._number), A._point, FONT_HERSHEY_COMPLEX, 1, Scalar(255, 255, 255));
		}
		draw_flag = 0;
		
	}
	else if(_event == CV_EVENT_MOUSEMOVE && _flags == CV_EVENT_FLAG_LBUTTON)
	{		
		draw_flag = 1;
	}
	else if (_event == CV_EVENT_LBUTTONUP)
	{
		B = dot(Point2f(_x, _y), dots.size());
		int replace_flag = 0;
		if (draw_flag == 1)
		{
			for (dot exist_dot : dots)
			{
				//if B is close to the existing dot, replace existing dot to B
				if (GetDistane(exist_dot._point, B._point) < 30)
				{
					B = exist_dot;
					replace_flag = 1;
				}
			}
			if (replace_flag == 0)
			{
				dots.push_back(B);
				vector<int> Bvec;
				adjacent_list.push_back(Bvec);
				adjacent_list[B._number].push_back(-1);
				circle(image, B._point, 2, Scalar(255, 255, 0), 2);
				putText(image, to_string(B._number), B._point, FONT_HERSHEY_COMPLEX, 1, Scalar(255, 255, 255));

			}
			if (GetDistane(A._point, B._point) > 10)
			{
				line(image, A._point, B._point, Scalar(0, 255, 64), 1);
				int is_ab_lined = 0;
				for (int i = 0; i < adjacent_list[A._number].size(); ++i)
				{
					if (adjacent_list[A._number][i] == B._number)is_ab_lined = 1;
				}
				if (is_ab_lined == 0)
				{
					adjacent_list[A._number].push_back(B._number);
					adjacent_list[B._number].push_back(A._number);
				}

			}
			draw_flag = 0;
		}
	}
}

int main()
{
	Mat displayImg = Mat::zeros(600, 600, CV_8UC3);
	namedWindow("display");
	//putText(displayImg, "left mouse down:draw dot", Point(0, 20), FONT_HERSHEY_SCRIPT_SIMPLEX, 1, Scalar(255, 255, 255));
	cout << "left mouse down: draw point or select point " << endl ;
	cout << "lmouse down and move then up: line two points" << endl;	
	cout << "Esc: show adjancent list" << endl << endl << endl;
	setMouseCallback("display", OnMouse, (void*)&displayImg);
	while (waitKey(30) != 27)
	{
		imshow("display", displayImg);
	}
	for (int i = 0; i < adjacent_list.size(); ++i)
	{
		cout << i << ":";
		for (int j = 0; j < adjacent_list[i].size(); ++j)
		{
			if(adjacent_list[i][j] != -1)	cout << adjacent_list[i][j] << "  ";
		}
		cout << endl;
	}
	destroyAllWindows();
	system("pause");
	return 0;
}