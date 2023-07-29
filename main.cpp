#include <stdio.h>
#include <time.h>
#include <stdlib.h>
#include"Get_Data.h"

int GetsNumOfInteger(int num);//获取整数位数
int Judging_Trends(class Kline* kLine, int Data_Length);
int Judge_rising_wave(class Kline* kLine, int Data_Length, class rising_wave* rising);
int counter_first = 0;
char X[20] = { 0 };

class rising_wave
{
public:
	int MaxLocation;//储存最大值的位置
	int MinLocation;
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
	clock_t start_time = clock();  // 记录初始时间
	char* url = (char*)calloc(1024, sizeof(char));
	char* data = (char*)calloc(1024 * 1024, sizeof(char));
	urlencode("115.SR401", 20000101, getCurrentDate(), 101, 1, url);
	request(url, data);
	free(url);

	int Data_Length = Get_Data_Length(data);
	if (Data_Length == -1)
		return 0;
	class Kline* kLine = (class Kline*)calloc(Data_Length, sizeof(class Kline));
	if (kLine == NULL)
		return 0;
	
	if (kLine == NULL)
		return 0;
	Get_Data(kLine, Data_Length, data);
	free(data);
	
	Judging_Trends(kLine, Data_Length);

	free(kLine);

	clock_t end_time = clock();  // 记录结束时间

	double total_time = (double)(end_time - start_time) / CLOCKS_PER_SEC;  // 计算运行时长，单位为秒

	printf("程序运行时长: %.2f 秒\n", total_time);

	/*double a, b, c;
	double First_Departure_Target, Second_Departure_Target, Third_Departure_Target;
	printf("请输入b点，a点,c点");
	scanf_s("%lf%lf%lf", &b, &a, &c);
	First_Departure_Target = fabs(b - a) + fabs(b - c) * 0.618 + c;
	Second_Departure_Target = fabs(b - a) + fabs(b - c) * 1 + c;
	Third_Departure_Target = fabs(b - a) + fabs(b - c) * 1.382 + c;
	printf("第一离场目标为：%f\n", First_Departure_Target);
	printf("第二离场目标为：%f\n", Second_Departure_Target);
	printf("第三离场目标为：%f\n", Third_Departure_Target);*/
	return 0;
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


int Judging_Trends(class Kline* kLine, int Data_Length)
{
	class rising_wave* rising = (class rising_wave*)calloc(1, sizeof(class rising_wave));
	if (Judge_rising_wave(kLine, Data_Length, rising) == 1)
	{
		return 0;
	}
	return 1;

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