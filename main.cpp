#include <stdio.h>
#include <time.h>
#include <stdlib.h>
#include"Get_Data.h"

int GetsNumOfInteger(int num);//��ȡ����λ��
int Judging_Trends(class Kline* kLine, int Data_Length);
int Judge_rising_wave(class Kline* kLine, int Data_Length, class rising_wave* rising);
int counter_first = 0;
char X[20] = { 0 };

class rising_wave
{
public:
	int MaxLocation;//�������ֵ��λ��
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
	clock_t start_time = clock();  // ��¼��ʼʱ��
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

	clock_t end_time = clock();  // ��¼����ʱ��

	double total_time = (double)(end_time - start_time) / CLOCKS_PER_SEC;  // ��������ʱ������λΪ��

	printf("��������ʱ��: %.2f ��\n", total_time);

	/*double a, b, c;
	double First_Departure_Target, Second_Departure_Target, Third_Departure_Target;
	printf("������b�㣬a��,c��");
	scanf_s("%lf%lf%lf", &b, &a, &c);
	First_Departure_Target = fabs(b - a) + fabs(b - c) * 0.618 + c;
	Second_Departure_Target = fabs(b - a) + fabs(b - c) * 1 + c;
	Third_Departure_Target = fabs(b - a) + fabs(b - c) * 1.382 + c;
	printf("��һ�볡Ŀ��Ϊ��%f\n", First_Departure_Target);
	printf("�ڶ��볡Ŀ��Ϊ��%f\n", Second_Departure_Target);
	printf("�����볡Ŀ��Ϊ��%f\n", Third_Departure_Target);*/
	return 0;
}


int Slope(int x1, int y1, int x2, int y2)
{
	return (y1 - y2) / (x2 - x1);
}

int findAverageValueMax(class Kline arr[], int size) {
	int max = arr[0].averageValue; // ��������ĵ�һ��Ԫ�������ֵ
	int MaxOrdinalNumber = 0;
	for (int i = 1; i < size; i++) {
		if (arr[i].averageValue > max) {
			max = arr[i].averageValue; // �����ǰԪ�ش��ڵ�ǰ���ֵ����������ֵ
			MaxOrdinalNumber = i;
		}
	}

	return MaxOrdinalNumber;
}

int findAverageValueMin(class Kline arr[], int size) {
	int min = arr[0].averageValue; // ��������ĵ�һ��Ԫ������Сֵ
	int MinOrdinalNumber = 0;
	for (int i = 1; i < size; i++) {
		if (arr[i].averageValue < min) {
			min = arr[i].averageValue; // �����ǰԪ��С�ڵ�ǰ��Сֵ���������Сֵ
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
	int MaxLocation = 0;//�������ֵ��λ��
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
		|| kLine[SubLowLocation].averageValue <= kLine[MinLocation].averageValue)//�ж��Ƿ����������
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