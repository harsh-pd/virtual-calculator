//virtual calculator based on color difference

#include <iostream>
#include <ctime>
#include "opencv2/highgui/highgui.hpp"
#include "opencv2/imgproc/imgproc.hpp"
#include "windows.h"
#include "stdio.h"


using namespace std;
using namespace cv;

char press(VideoCapture);

void main()
{
	int num1 = 0;
	int num2 = 0;
	int result;
	char key;
	char command = '@';
	bool SecondNumPressed = false;


	VideoCapture cap;
	cap.open(0);
	if (!cap.isOpened())
	{
		system("CLS");
		printf("\n\n\t\t\tcamera disconnected");
		system("PAUSE");
		exit;
	}
	cap.set(CV_CAP_PROP_FRAME_WIDTH, 640);
	cap.set(CV_CAP_PROP_FRAME_HEIGHT, 480);


	system("CLS");
	printf("\n\n\t\t\t0");
	while (true)
	{
		key = press(cap);
		if ((SecondNumPressed == true) && ((key != '=') && (((key - '0')<0) || ((key - '0')>9))))
		{
			//cout << SecondNumPressed << " " << key << " " << key - '0' << "\n";
			continue;
		}
		if (key == '=')
		{
			system("CLS");

			if ((num2 == 0) && (command == '/'))
				printf("\n\n\t\t\tcan not devide with zero");
			else if (SecondNumPressed == true)
				printf("\n\n\t\t\t%d", result);
			else
				printf("\n\n\t\t\t%d", num1);
			num1 = 0;
			num2 = 0;
			command = '@';
			SecondNumPressed = false;
		}
		else if (key == 'c')
		{
			system("CLS");
			num1 = 0;
			num2 = 0;
			command = '@';
			printf("\n\n\t\t\t0");
			SecondNumPressed = false;
		}
		else if ((key == '+') || (key == '*') || (key == '-') || (key == '/'))
		{
			if (command != '@')
			{
				system("CLS");
				printf("\n\n\t\t\t%d", num1);
			}
			printf(" %c ", key);
			command = key;
		}
		else if (command == '@')
		{
			system("CLS");
			if (key == '<')
				num1 = num1 - num1 % 10;
			else
				num1 = num1 * 10 + (key - '0');
			printf("\n\n\t\t\t%d", num1);
		}
		else
		{
			system("CLS");
			if (key == '<')
				num2 = num2 - num2 % 10;
			else
			{
				num2 = num2 * 10 + (key - '0');
				SecondNumPressed = true;
			}
			printf("\n\n\t\t\t%d %c %d", num1, command, num2);
			if (command == '+')
				result = num1 + num2;
			else if (command == '-')
				result = num1 - num2;
			else if (command == '*')
				result = num1 * num2;
			else if (command == '/')
			if (num2 != 0)
				result = num1 / num2;
		}
	}
}

char press(VideoCapture cam)
{
	Mat im, ROI;
	int i, j, count;
	time_t KeyOnTime[5][5] = { 0 };
	Rect rect;
	namedWindow("Video");
	time_t newtime;
	int period;
	Scalar avg;


	rect.width = 128;
	rect.height = 96;
	while (true)
	{
		rect.x = 0;
		rect.y = 0;
		cam >> im;
		flip(im, im, -1);

		count = 0;
		for (i = 0; i < 5; i++)
		for (j = 0; j < 5; j++)
		{
			rect.x = j * 128;
			rect.y = i * 96;
			ROI = im(rect);
			avg = mean(ROI);
			if (((avg.val[1] / avg.val[0])>1.5) || ((avg.val[2] / avg.val[0])>1.5))//will detect any reddish, greenish or yellowish stuff
			{

				if (KeyOnTime[i][j] == 0)
					KeyOnTime[i][j] = time(0);
				else
				{
					newtime = time(0);
					period = difftime(newtime, KeyOnTime[i][j]);

					im(rect) = 100;//block darkened if something detected there;

					if (period >= 1)
					{
						if ((i == 0) && (j == 0))
							return('<');
						if ((i == 0) && (j == 2))
							return('c');
						if ((i == 1) && (j == 0))
							return('7');
						if ((i == 1) && (j == 1))
							return('8');
						if ((i == 1) && (j == 2))
							return('9');
						if ((i == 1) && (j == 3))
							return('/');
						if ((i == 2) && (j == 0))
							return('4');
						if ((i == 2) && (j == 1))
							return('5');
						if ((i == 2) && (j == 2))
							return('6');
						if ((i == 2) && (j == 3))
							return('*');
						if ((i == 3) && (j == 0))
							return('1');
						if ((i == 3) && (j == 1))
							return('2');
						if ((i == 3) && (j == 2))
							return('3');
						if ((i == 3) && (j == 3))
							return('-');
						if ((i == 3) && (j == 4))
							return('=');
						if ((i == 4) && (j == 0))
							return('0');
						if ((i == 4) && (j == 1))
							return('0');
						if ((i == 4) && (j == 3))
							return('+');
						if ((i == 4) && (j == 4))
							return('=');
					}
				}
			}
			else
				KeyOnTime[i][j] = 0;
		}

		imshow("Video", im);
		cvWaitKey(10);
	}
}
