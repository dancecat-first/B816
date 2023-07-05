#define  _CRT_SECURE_NO_WARNINGS
#include <stdio.h>
#include <math.h>
#include <stdlib.h>
#include <Windows.h>
#include <time.h>

void Get_Data(class Kline* kLine, int Data_Length);
int Get_Data_Length(const char* FileName);
int date(int a[]); //用于转换时间格式
int GetsNumOfInteger(int num);//获取整数位数
int Judging_Trends(class Kline* kLine, int Data_Length);
int Judge_rising_wave(class Kline* kLine, int Data_Length, class rising_wave* rising);
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
	int averageValue;//最高最低平均值
	int vol;     //成交量
	int terr;   //成交额
	double aem;   //振幅
	double swing; //涨跌幅
	int rise;  //涨跌额
	double turn;  //换手率
	
	Kline()
	{
		day = 0;
		first = 0;
		end = 0;
		max = 0;
		min = 0;
		averageValue = 0;
		vol = 0;
		terr = 0;
		aem = 0;
		swing = 0;
		rise = 0;
		turn = 0;
	}
};
class rising_wave
{
public:
	int MaxLocation;//储存最大值的位置
	int MinLocation ;
	int SubLowLocation;
	rising_wave()
	{
		MaxLocation = 0;
		MinLocation = 0;
		SubLowLocation = 0;
	}
};

int main()
{
	//system("python 每日.py");
	int Data_Length = Get_Data_Length("today_s.csv") - 1;
	class Kline* kLine = (class Kline*)malloc(sizeof(class Kline) * Data_Length);
	if (kLine == NULL)
		return 0;
	Get_Data(kLine, Data_Length);
	Judging_Trends(kLine, Data_Length);
	
	free(kLine);
	double a, b, c;
	double First_Departure_Target, Second_Departure_Target, Third_Departure_Target;
	printf("请输入b点，a点,c点");
	scanf_s("%lf%lf%lf", &b, &a, &c);
	First_Departure_Target = fabs(b - a) + fabs(b - c) * 0.618 + c;
	Second_Departure_Target = fabs(b - a) + fabs(b - c) * 1 + c;
	Third_Departure_Target = fabs(b - a) + fabs(b - c) * 1.382 + c;
	printf("第一离场目标为：%f\n", First_Departure_Target);
	printf("第二离场目标为：%f\n", Second_Departure_Target);
	printf("第三离场目标为：%f\n", Third_Departure_Target);
	return 0;
}

void Get_Data(class Kline* kLine, int Data_Length)
{

	FILE* fp=fopen("today_s.csv","r");

	fseek(fp, 91, SEEK_SET);//跳过文件前91字节
	for (int H = 0; H < Data_Length; H++)
	{
		int days[3] = { 0 };
		fscanf_s(fp, "%d-%d-%d", &days[0], &days[1], &days[2]);
		kLine[H].day = date(days);

		fscanf_s(fp, ",%d,%d,%d,%d,%d,%d,%lf,%lf,%d,%lf\n", &kLine[H].first, &kLine[H].end, &kLine[H].max, &kLine[H].min, &kLine[H].vol, &kLine[H].terr,&kLine[H].aem, &kLine[H].swing, &kLine[H].rise, &kLine[H].turn);
		kLine[H].averageValue = (kLine[H].max + kLine[H].min) / 2;
	}

	fclose(fp);
	return;
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

int date(int date[3]) //用于转换时间格式
{
	return date[0] * 10000 + date[1] * 100 + date[2];
}


int Slope(int x1, int y1, int x2, int y2)
{
	return (y1 - y2) / (x2 - x1);
}


int findAverageValueMax(class Kline arr[], int size) {
	int max = arr[0].averageValue; // 假设数组的第一个元素是最大值
	int MaxOrdinalNumber = 0;
	for (int i = 1; i < size; i++) {
		if (arr[i].averageValue > max) {
			max = arr[i].averageValue; // 如果当前元素大于当前最大值，则更新最大值
			MaxOrdinalNumber = i;
		}
	}

	return MaxOrdinalNumber;
}

int findAverageValueMin(class Kline arr[], int size) {
	int min = arr[0].averageValue; // 假设数组的第一个元素是最小值
	int MinOrdinalNumber = 0;
	for (int i = 1; i < size; i++) {
		if (arr[i].averageValue < min) {
			min = arr[i].averageValue; // 如果当前元素小于当前最小值，则更新最小值
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
	class rising_wave* rising = (class rising_wave*)malloc(sizeof(class rising_wave));
	if (Judge_rising_wave(kLine, Data_Length, rising) == 1)
	{
		return 0;
	}
	return 0;

}


int Judge_rising_wave(class Kline* kLine, int Data_Length, class rising_wave* rising)
{
	int MaxLocation = 0;//储存最大值的位置
	int MinLocation = 0;
	int SubLowLocation = 0;
	int time = getCurrentDate();
	for (int i = 0; i < Data_Length; i++)
	{
		if (kLine[i].day > time - 100)
		{
			MaxLocation = findAverageValueMax(&kLine[i], Data_Length - i) + i;
			break;
		}
		else
		{
			MaxLocation = -1;
		}
	}
	for (int i = 0; i < Data_Length; i++)
	{
		if (kLine[i].day > kLine[MaxLocation].day - 600)
		{
			MinLocation = findAverageValueMin(&kLine[i], Data_Length - i) + i;
			break;
		}
		else
		{
			MinLocation = -1;
		}
	}
	for (int i = 0; i < Data_Length; i++)
	{
		if (kLine[i].day > kLine[MaxLocation].day)
		{
			SubLowLocation = findAverageValueMin(&kLine[i], Data_Length - i) + i;
			break;
		}
		else
		{
			SubLowLocation = -1;
		}
	}

	if (MaxLocation == -1 || MinLocation == -1 || SubLowLocation == -1)
	{
		return 0;
	}

	if (kLine[MaxLocation].day - kLine[MinLocation].day < 7
		|| kLine[SubLowLocation].day <= kLine[MaxLocation].day
		|| kLine[SubLowLocation].averageValue >= kLine[MaxLocation].averageValue
		|| kLine[SubLowLocation].averageValue <= kLine[MinLocation].averageValue)//判断是否符合上涨浪
	{
		return 0;
	}
	else
	{
		int count = 0;
		int downFx = 0, downFy = 0;
		for (int i = MinLocation; i < MaxLocation - 1; i++)
		{

			if (kLine[i + 1].averageValue - kLine[i].averageValue < 0)
			{
				count++;
				if (count == 1)
				{
					downFx = kLine[i].day;
					downFy = kLine[i].averageValue;
				}
				if (count == 5 && Slope(downFx, downFy, kLine[i].day, kLine[i].averageValue) < -10)
				{
					return 0;
				}
			}
			else
			{
				count = 0;
			}

		}

		count = 0;
		downFx = 0;
		downFy = 0;

		for (int i = MaxLocation; i < SubLowLocation - 1; i++)
		{
			if (kLine[i + 1].averageValue > kLine[i].averageValue)
			{
				count++;
				if (count == 1)
				{
					downFx = kLine[i].day;
					downFy = kLine[i].averageValue;
				}
				if (count == 5 && Slope(downFx, downFy, kLine[i].day, kLine[i].averageValue) > 10)
				{
					return 0;
				}
			}
			else
			{
				count = 0;
			}

		}
	}
	rising->MaxLocation = MaxLocation;
	rising->MinLocation = MinLocation;
	rising->SubLowLocation = SubLowLocation;
	return 1;
}
int GetsNumOfInteger(int num)
{
	int temp = num / 10;
	int NumOfInteger = 1;
	while (temp > 0)
	{
		temp /= 10;
		NumOfInteger++;
	}
	return NumOfInteger;
}

