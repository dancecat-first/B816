
#define  _CRT_SECURE_NO_WARNINGS
#include <stdio.h>
#include <math.h>
#include <stdlib.h>
#include <Windows.h>
#include <time.h>

int Get_Data();
int Get_Data_Length(const char* FileName);
int conversion();
int SquareTen(int x); //进行10的平方
int date(int a[]); //用于转换时间格式
int Judging_Trends(class Kline* kLine, int Data_Length);
int counter_first = 0;
char X[20] = { 0 };
class Kline
{
public:
	int day;
	int first;   //开盘
	int end;     //收盘
	int max;     //最高
	int min;     //最低
	int vol;     //成交量
	long terr;   //成交额
	float aem;   //振幅
	float swing; //涨跌幅
	float rise;  //涨跌额
	float turn;  //换手率
	
	Kline()
	{
		day = 0;
		first = 0;
		end = 0;
		max = 0;
		min = 0;
		vol = 0;
		terr = 0;
		aem = 0;
		swing = 0;
		rise = 0;
		turn = 0;
	}
};

int main()
{
	system("python 每日.py");
	Get_Data();
	
	double a, b, c;
	double First_Departure_Target, Second_Departure_Target, Third_Departure_Target;
	puts("请输入b点，a点,c点");
	scanf_s("%lf%lf%lf", &b, &a, &c);
	First_Departure_Target = fabs(b - a) + fabs(b - c) * 0.618 + c;
	Second_Departure_Target = fabs(b - a) + fabs(b - c) * 1 + c;
	Third_Departure_Target = fabs(b - a) + fabs(b - c) * 1.382 + c;
	printf("第一离场目标为：%f\n", First_Departure_Target);
	printf("第二离场目标为：%f\n", Second_Departure_Target);
	printf("第三离场目标为：%f\n", Third_Departure_Target);
	return 0;
}

int Get_Data()
{
	
	int Data_Length = Get_Data_Length("today_s.csv") - 1;
	int days[3];

	FILE* fp=fopen("today_s.csv","r");
	class Kline* kLine = (class Kline*)malloc(sizeof(class Kline) * Data_Length);
	if (kLine == NULL)
		return 0;

	for (int i = 0; i < 91; i++)
	{
		fgetc(fp);
	}
	for (int H = 0; H < Data_Length; H++)
	{
		for (int i = 0; i < 3; i++)
		{
			fscanf(fp, "%d", &days[i]);
		}
		kLine[H].day = date(days);
		fscanf(fp, "%c", &X[1]);

		for (int j = 0; j < 6; j++)
		{
			for (int i = 0; i < 20; i++)
			{
				fscanf(fp, "%c", &X[i]);
				if (X[i] == ',')
				{
					counter_first = i;
					X[i] = '\0';
					break;
				}
			}

			switch (j)
			{
			case 0:
				kLine[H].first = conversion();
				break;
			case 1:
				kLine[H].end = conversion();
				break;
			case 2:
				kLine[H].max = conversion();
				break;
			case 3:
				kLine[H].min = conversion();
				break;
			case 4:
				kLine[H].vol = conversion();
				break;
			case 5:
				for (int i = 0; i < 20; i++)
				{
					fscanf(fp, "%c", &X[i]);
					if (X[i] == ',')
					{
						X[i] = '\0';
						break;
					}
				}
				//kLine[H].terr = S();
				break;
			}
		}

		for (int i = 0; i < 20; i++)
		{
			fscanf(fp, "%c", &X[i]);
			if (X[i] == '\n')
			{
				X[i] = '\0';
				break;
			}
		}
	}
	Judging_Trends(kLine, Data_Length);
	fclose(fp);
	return 1;
}
int Get_Data_Length(const char* FileName)
{
	FILE* fp = fopen(FileName, "r");
	int Length = 0;
	char* c = (char*)malloc(sizeof(char));
	if (c != NULL)
	{
		while (*c != EOF)
		{
			*c = fgetc(fp);
			if (*c == '\n')
			{
				Length++;
			}
		}
	}
	free(c);
	fclose(fp);
	return Length;
}
int conversion()
{
	int i2 = 0;
	int ret = 0;
	int N = 0;
	int i3 = 0;
	for (int i = 0; i < counter_first; i++)
	{

		i2 = counter_first - i3 - 1;
		switch (X[i])
		{
		case 48:
			N = 0;
			break;
		case 49:
			N = 1;
			break;
		case 50:
			N = 2;
			break;
		case 51:
			N = 3;
			break;
		case 52:
			N = 4;
			break;
		case 53:
			N = 5;
			break;
		case 54:
			N = 6;
			break;
		case 55:
			N = 7;
			break;
		case 56:
			N = 8;
			break;
		case 57:
			N = 9;
			break;
		default:
			break;
		}

		if (i == 0)
		{
			ret = SquareTen(i2) * N;
		}
		else
		{
			ret += SquareTen(i2) * N;
		}

		i3++;
	}
	ret = ret + N;
	return ret;
}
int SquareTen(int x) //进行10的平方
{
	int y = 10;
	if (x != 0)
	{
		for (int i = 0; i < x - 1; i++)
		{
			y = y * 10;
		}
	}
	else
	{
		y = 0;
	}
	return y;
}
int date(int a[]) //用于转换时间格式
{
	int a_1, a_2, a_3;
	int day;
	a_1 = *a;
	a_2 = *(a + 1);
	a_3 = *(a + 2);
	day = a_1 * 10000 + abs(a_2) * 100 + abs(a_3);
	return day;
}


int Slope(int x1, int y1, int x2, int y2)
{
	return (y1 - y2) / (x2 - x1);
}


int findMax(class Kline arr[], int size) {
	int max = arr[0].max ; // 假设数组的第一个元素是最大值
	int MaxOrdinalNumber = 0;
	for (int i = 1; i < size; i++) {
		if (arr[i].max  > max) {
			max = arr[i].max ; // 如果当前元素大于当前最大值，则更新最大值
			MaxOrdinalNumber = i;
		}
	}

	return MaxOrdinalNumber;
}

int findMin(class Kline arr[], int size) {
	int min = arr[0].max ; // 假设数组的第一个元素是最小值
	int MinOrdinalNumber = 0;
	for (int i = 1; i < size; i++) {
		if (arr[i].max  < min) {
			min = arr[i].max ; // 如果当前元素小于当前最小值，则更新最小值
			MinOrdinalNumber = i;
		}
	}

	return MinOrdinalNumber;
}

int getCurrentDate() 
{
	int year = 0;
	int month = 0;
	int day = 0;
	time_t currentTime;
	time(&currentTime);

	struct tm* localTime = localtime(&currentTime);

	year = localTime->tm_year + 1900;
	month = localTime->tm_mon + 1;
	day = localTime->tm_mday;

	return (year * 10000 + month * 100 + day);
}

int Judging_Trends(class Kline* kLine, int Data_Length)
{
	int count = 0;
	int downFx, downFy;
	int SlopeSum = 0;
	int max = 0;
	int min = 0;
	int time = getCurrentDate();
	for (int i = 0; i < Data_Length ; i++)
	{
		if (kLine[i].day>time-100)
		{
			max = findMax(&kLine[i], Data_Length - i) + i;
			break;
		}
	}
	for (int i = 0; i < Data_Length; i++)
	{
		if (kLine[i].day > kLine[max].day - 600)
		{
			min = findMin(&kLine[i], Data_Length - i) + i;
			break;
		}
	}
	for (int i = min; i < max-1; i++ )
	{

		if (kLine[i + 1].max - kLine[i].max < 0)
		{
			count++;
			if (count=1)
			{
				downFx = kLine[i].day;
				downFy = kLine[i].max;
			}
			if (count==5&&Slope(downFx,downFy, kLine[i].day, kLine[i].max)<-50)
			{
				break;
			}
		}
		else
		{
			count = 0;
		}
		
	}				

	return 0;
}

