#include <stdio.h>
#include <time.h>
#include <stdlib.h>
#include<Windows.h>
#include"Get_Data.h"

int GetsNumOfInteger(int num);//��ȡ����λ��
int Judging_Trends(class Kline* kLine, int Data_Length);
int Judge_rising_wave(class Kline* kLine, int Data_Length, class rising_wave* rising);
void ReleaseLinkedList(class rising_wave* head);
#pragma warning(disable:4996)

class rising_wave
{
public:
	int MaxLocation;//�������ֵ��λ��
	int MinLocation;
	int SubLowLocation;
	class rising_wave* next;
	rising_wave()
	{
		MaxLocation = 0;
		MinLocation = 0;
		SubLowLocation = 0;
		next=NULL;
	}
};

int main()
{
	clock_t start_time = clock();  // ��¼��ʼʱ��
	char* url = (char*)calloc(1024, sizeof(char));
	char* data = (char*)calloc(1024 * 1024, sizeof(char));
	if (url == NULL || data == NULL)
	{
		printf("�ڴ����ʧ��");
		return -1;
	}
	urlencode("115.SR401", 20060101, getCurrentDate(), 101, 1, url);
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
		printf("�ڴ����ʧ��");
		return -1;
	}

	Get_Data(kLine, Data_Length, data);
	Judging_Trends(kLine, Data_Length);

	free(data);
	free(kLine);

	clock_t end_time = clock();  // ��¼����ʱ��

	double total_time = (double)(end_time - start_time) / CLOCKS_PER_SEC;  // ��������ʱ������λΪ��

	printf("��������ʱ��: %.2f ��\n", total_time);

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
		ReleaseLinkedList(rising);
		return 1;
	}
	return 0;

}

int GetRising_waveLength(class rising_wave* rising)
{
	int count = 1;
	class rising_wave* current = rising;
	while (current->next != NULL)
	{
		current = current->next;
		count++;
	}
	return count;
}
void PrintRising_wave(class Kline* kLine, class rising_wave* rising)
{
	class rising_wave* current = rising;
	printf("%d-%d\n", kLine[current->MinLocation].day, kLine[current->MaxLocation].day);
	while (current->next != NULL)
	{
		current = current->next;
		printf("%d-%d\n", kLine[current->MinLocation].day, kLine[current->MaxLocation].day);
	}
	return;
}
void ReleaseLinkedList(class rising_wave* head)
{
	rising_wave* current = head;
	rising_wave* nextNode;

	while (current != NULL)
	{
		nextNode = current->next;
		free(current);
		current = nextNode;
	}
}
void insertAtEndRising_wave(class rising_wave* rising, int MaxLocation, int MinLocation) {
	if (rising == NULL) {
		printf("rising == NULL");
		return;
	}
	if (rising->MinLocation==0)
	{
		rising->MaxLocation = MaxLocation;
		rising->MinLocation = MinLocation;
	}
	class rising_wave* current = rising;
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
	current->next = (class rising_wave*)calloc(1, sizeof(class rising_wave));
	if (current->next == NULL)
	{
		printf("current->next == NULL");
		return;
	}
	current->next->MaxLocation = MaxLocation;
	current->next->MinLocation = MinLocation;
}
int Judge_Double_Repo(class Kline* kLine, int Data_Length, class rising_wave* rising)
{
	class rising_wave* current = rising;
	int sign = 0;
	int Rising_waveLength = GetRising_waveLength(rising);

	for (int i = 0; i < Rising_waveLength; i++)
	{
		for (int j = current->MaxLocation; j < current->MaxLocation + 30 && j < Data_Length; j++)
		{
			if (kLine[j].end < kLine[j].MA3_3 && sign == 0)
				sign = 1;
			if (kLine[j].end > kLine[j].MA3_3 && sign == 1)
				sign = 2;
			if (kLine[j].end < kLine[j].MA3_3 && sign >= 2) {
				sign++;
			}
		}
		if (sign <= 3) {
			printf("%d-%d\n", kLine[current->MinLocation].day, kLine[current->MaxLocation].day);
			return 1;
		}
		else
		{
			printf("none\n");
		}
		current = current->next;
	}
	return 0;
}
int Judge_rising_wave(class Kline* kLine, int Data_Length, class rising_wave* rising)
{
	int count = 0;
	int num = 0;
	for (int i = 0; i < Data_Length; i++)
	{
		printf("%d\t%lf\t%lf\n", kLine[i].day, kLine[i].macd.MACD, kLine[i].macd.SignalLine);
	}
	for (int i = 0; i < Data_Length; i++)
	{
		if (kLine[i].macd.MACD < kLine[i].macd.SignalLine)
			num = i;
		if (kLine[i].macd.MACD > kLine[i].macd.SignalLine) {
			if(i-1==num)
				printf("%d\n", kLine[i].day);
		}
	}
	for (int i = 0; i < Data_Length; i++)
	{
		if (kLine[i].macd.MACD > kLine[i].macd.SignalLine)
			num = i;
		if (kLine[i].macd.MACD < kLine[i].macd.SignalLine) {
			if (i - 1 == num)
				printf("50\t%d\n", kLine[i].day);
		}
	}
	for (int i = 7; i < Data_Length; i++)
	{
		if (kLine[i].end >= kLine[i].MA3_3)
			count++;
		else
			count = 0;

		if (count > 10)
			insertAtEndRising_wave(rising, i, i - (count - 1));
	}
	PrintRising_wave(kLine, rising);
	if (Judge_Double_Repo(kLine, Data_Length, rising) == 1)
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