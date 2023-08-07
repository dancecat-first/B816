#include <stdio.h>
#include <time.h>
#include <stdlib.h>
#include<Windows.h>
#include"Get_Data.h"

int GetsNumOfInteger(int num);//��ȡ����λ��
int findkLineMax(class Kline arr[], int size);
int findkLineMin(class Kline arr[], int size);
int Judging_Trends(class Kline* kLine, int Data_Length);
int Judge_wave(class Kline* kLine, int Data_Length, class wave* wave);
void ReleaseLinkedList(class wave* head);
#pragma warning(disable:4996)

class wave
{
public:
	int MaxLocation;//�������ֵ��λ��
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
	clock_t start_time = clock();  // ��¼��ʼʱ��
	char* url = (char*)calloc(1024, sizeof(char));
	char* data = (char*)calloc(1024 * 1024, sizeof(char));
	if (url == NULL || data == NULL)
	{
		printf("�ڴ����ʧ��");
		return -1;
	}
	urlencode("115.SR309", 20000101, getCurrentDate(), 101, 1, url);
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

int findkLineMax(class Kline arr[], int size) {
	int max = arr[0].max; // ��������ĵ�һ��Ԫ�������ֵ
	int MaxNumber = 0;
	for (int i = 1; i < size; i++) {
		if (arr[i].max> max) {
			max = arr[i].max; // �����ǰԪ�ش��ڵ�ǰ���ֵ����������ֵ
			MaxNumber = i;
		}
	}

	return MaxNumber;
}

int findkLineMin(class Kline arr[], int size) {
	int min = arr[0].min; // ��������ĵ�һ��Ԫ������Сֵ
	int MinNumber = 0;
	for (int i = 1; i < size; i++) {
		if (arr[i].min < min) {
			min = arr[i].min; // �����ǰԪ��С�ڵ�ǰ��Сֵ���������Сֵ
			MinNumber = i;
		}
	}

	return MinNumber;
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
	char sign = 0;
	int WaveLength = GetWaveLength(wave);
	int FibonacciLevels = 0;
	
	for (int i = 0; i < WaveLength; i++)
	{
		int num = 0;
		sign = 0;
		int temp = 0;
		int MaxLocation = findkLineMax(&kLine[current->MinLocation], current->MaxLocation - current->MinLocation);
		for (int j = current->MaxLocation; j < current->MaxLocation + 13 && j < Data_Length; j++)
		{
			if (kLine[j].end < kLine[j].MA3_3 && sign == 0) {
				num = j;
				sign = 1;
			}

			if (kLine[j].end > kLine[j].MA3_3 && sign == 1) {
				int MinLocation = findkLineMin(&kLine[num], j - num);
				FibonacciLevels = (int)((kLine[MaxLocation + current->MinLocation].max - kLine[MinLocation + num].min) * 0.618 + kLine[MinLocation + num].min);
				sign = 2;
			}

			if (kLine[j].end < kLine[j].MA3_3 && sign == 2) {
				sign = 3;
				temp = j;
				break;
			}
		}
		if (sign == 3)
		{
			for (int k = temp; k < temp + 10; k++)
			{
				if (kLine[k].max > FibonacciLevels) {
					sign = -1;
					break;
				}
			}
		}
		if (sign == 3)
			printf("%d-%d\t%d\n", kLine[current->MinLocation].day, kLine[current->MaxLocation].day, sign);
		else
			printf("%d-%d\tnone\n", kLine[current->MinLocation].day, kLine[current->MaxLocation].day);

		current = current->next;
	}
	return 0;
}
int Judge_wave(class Kline* kLine, int Data_Length, class wave* wave)
{
	int count = 0;
	int drop = 0;
	
	for (int i = 7; i < Data_Length; i++)
	{
		if (kLine[i].end >= kLine[i].MA3_3)
			count++;
		else {
			//��Ѹ���ƽ��ڼ�ֻ����1�����̼۵���MA3_3
			if (count > 0) 
				drop++;

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