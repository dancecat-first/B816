#include <stdio.h>
#include <time.h>
#include <stdlib.h>
#include<Windows.h>
#include"Get_Data.h"

int GetsNumOfInteger(int num);//获取整数位数
int findAverageValueMax(class Kline arr[], int size);
int findAverageValueMin(class Kline arr[], int size);
int Judging_Trends(class Kline* kLine, int Data_Length);
int Judge_wave(class Kline* kLine, int Data_Length, class wave* wave);
void ReleaseLinkedList(class wave* head);
#pragma warning(disable:4996)

class wave
{
public:
	int MaxLocation;//储存最大值的位置
	int MinLocation;
	int SubLowLocation;
	class wave* next;
	wave()
	{
		MaxLocation = 0;
		MinLocation = 0;
		SubLowLocation = 0;
		next=NULL;
	}
};

int main()
{
	clock_t start_time = clock();  // 记录初始时间
	char* url = (char*)calloc(1024, sizeof(char));
	char* data = (char*)calloc(1024 * 1024, sizeof(char));
	if (url == NULL || data == NULL)
	{
		printf("内存分配失败");
		return -1;
	}
	urlencode("114.m2309", 20220101, getCurrentDate(), 101, 1, url);
	if (request(url, data) == -1) {
		printf("request err\n");
		free(data);
		free(url);
		return -1;
	}
	free(url);

	int Data_Length = Get_Data_Length(data);
	if (Data_Length == -1)
		return 0;
	class Kline* kLine = (class Kline*)calloc(Data_Length, sizeof(class Kline));
	if (kLine == NULL) {
		printf("内存分配失败");
		return -1;
	}

	Get_Data(kLine, Data_Length, data);
	Judging_Trends(kLine, Data_Length);

	free(data);
	free(kLine);

	clock_t end_time = clock();  // 记录结束时间

	double total_time = (double)(end_time - start_time) / CLOCKS_PER_SEC;  // 计算运行时长，单位为秒

	printf("程序运行时长: %.2f 秒\n", total_time);

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
	class wave* wave = (class wave*)calloc(1, sizeof(class wave));
	if (Judge_wave(kLine, Data_Length, wave) == 1)
	{
		ReleaseLinkedList(wave);
		return 1;
	}
	return 0;

}

int GetWaveLength(class wave* wave)
{
	int count = 1;
	class wave* current = wave;
	while (current->next != NULL)
	{
		current = current->next;
		count++;
	}
	return count;
}
void PrintWave(class Kline* kLine,class wave* wave,  bool risingWave)
{
	class wave* current = wave;
	if(risingWave==true)
		printf("%d-%d\n", kLine[current->MinLocation].day, kLine[current->MaxLocation].day);
	else
		printf("%d-%d\n", kLine[current->MaxLocation].day, kLine[current->MinLocation].day);
	while (current->next != NULL)
	{
		current = current->next;
		if (risingWave == true)
			printf("%d-%d\n", kLine[current->MinLocation].day, kLine[current->MaxLocation].day);
		else
			printf("%d-%d\n", kLine[current->MaxLocation].day, kLine[current->MinLocation].day);
	}
	return;
}
void ReleaseLinkedList(class wave* head)
{
	wave* current = head;
	wave* nextNode;

	while (current != NULL)
	{
		nextNode = current->next;
		free(current);
		current = nextNode;
	}
}
void insertAtEndWave(class wave* wave, int MaxLocation, int MinLocation) {
	if (wave == NULL) {
		printf("wave == NULL");
		return;
	}
	if (wave->MinLocation == 0)
	{
		wave->MaxLocation = MaxLocation;
		wave->MinLocation = MinLocation;
	}
	class wave* current = wave;
	while (current->next != NULL)
	{
		current = current->next;
		if (current->MinLocation == MinLocation) {
			current->MaxLocation = MaxLocation;
			return;
		}
	}
	if (current->MinLocation == MinLocation) {
		current->MaxLocation = MaxLocation;
		return;
	}
	current->next = (class wave*)calloc(1, sizeof(class wave));
	if (current->next == NULL)
	{
		printf("current->next == NULL");
		return;
	}
	current->next->MaxLocation = MaxLocation;
	current->next->MinLocation = MinLocation;
}
int Judge_Double_Repo(class Kline* kLine, int Data_Length, class wave* wave,bool RisingWave)
{
	class wave* current = wave;
	int sign = 0;
	int WaveLength = GetWaveLength(wave);

	for (int i = 0; i < WaveLength; i++)
	{
		sign = 0;
		if (RisingWave == true)
		{
			int temp = kLine[current->MaxLocation].end;
			for (int j = current->MaxLocation; j < current->MaxLocation + 30 && j < Data_Length; j++)
			{
				if (kLine[j].end < kLine[j].MA3_3 && sign == 0 && kLine[j].end > temp) {
					temp = kLine[j].end;
					sign = 1;
				}
				else if (kLine[j].end < kLine[j].MA3_3 && sign == 0 && kLine[j].end < temp)
					temp = kLine[j].end;
				if (kLine[j].end > kLine[j].MA3_3 && sign == 1 && kLine[j].end < temp) {
					temp = kLine[j].end;
					sign = 2;
				}
				else if (kLine[j].end > kLine[j].MA3_3 && sign == 1 && kLine[j].end > temp)
					temp = kLine[j].end;
				if (kLine[j].end < kLine[j].MA3_3 && sign >= 2) {
					sign++;
				}
			}
			if (sign <= 3) {
				printf("%d-%d\t%d\n", kLine[current->MinLocation].day, kLine[current->MaxLocation].day,sign);
				
			}
			else
				printf("none\n");
		}
		current = current->next;
	}
	return 0;
}
int Judge_wave(class Kline* kLine, int Data_Length, class wave* wave)
{
	int count = 0;
	int temp = 0;
	int temp1 = 0;
	int drop = 0;
	int drop_sum = 0;
	for (int i = 0; i < Data_Length; i++)
	{
		printf("%d\t%lf\t%lf\n", kLine[i].day, kLine[i].macd.MACD, kLine[i].macd.SignalLine);
	}
	for (int i = 0; i < Data_Length; i++)
	{
		if (kLine[i].macd.MACD < kLine[i].macd.SignalLine)
			temp = i;
		if (kLine[i].macd.MACD >= kLine[i].macd.SignalLine) {
			if(i-1==temp)
				printf("%d\n", kLine[i].day);
		}

		if (kLine[i].macd.MACD > kLine[i].macd.SignalLine)
			temp1 = i;
		if (kLine[i].macd.MACD <= kLine[i].macd.SignalLine) {
			if (i - 1 == temp1)
				printf("\t%d\n", kLine[i].day);
		}
	}
	for (int i = 7; i < Data_Length; i++)
	{
		if (kLine[i].end >= kLine[i].MA3_3)
			count++;
		else {
			//在迅猛推进期间只允许1次收盘价低于MA3_3
			if (count > 0) {
				drop++;
			}
			if (drop == 2)
			{
				drop = 0;
				count = 0;
			}
		}
		if (count > 13)
			insertAtEndWave(wave, i, i - (count - 1)-drop);
	}
	PrintWave(kLine, wave,true);
	if (Judge_Double_Repo(kLine, Data_Length, wave,true) == 1)
		return 1;
	else
		return 0;
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