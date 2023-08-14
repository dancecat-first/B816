#include <stdio.h>
#include <time.h>
#include <stdlib.h>
#include<Windows.h>
#include"Get_Data.h"
#include"Double_Repo.h"
#include"Data.h"

int GetsNumOfInteger(int num);//获取整数位数
int findkLineMax(class Kline arr[], int size);
int findkLineMin(class Kline arr[], int size);
int Judging_Trends(class Kline* kLine, int Data_Length);
int Judge_wave(class Kline* kLine, int Data_Length);
void ReleaseLinkedList(class wave* head);
#pragma warning(disable:4996)

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
	urlencode("114.y2309", 20000101, getCurrentDate(), 101, 1, url);
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

int Judging_Trends(class Kline* kLine, int Data_Length)
{
	if (Judge_wave(kLine, Data_Length) == 1)
	{
		return 1;
	}
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