#include <stdio.h>
#include <time.h>
#include <stdlib.h>
#include<Windows.h>
#include"Data.h"
#include"Double_Repo.h"

int findkLineMax(class Kline arr[], int size) 
{
	int max = arr[0].max; // 假设数组的第一个元素是最大值
	int MaxNumber = 0;
	for (int i = 1; i < size; i++) {
		if (arr[i].max > max) {
			max = arr[i].max; // 如果当前元素大于当前最大值，则更新最大值
			MaxNumber = i;
		}
	}
	return MaxNumber;
}

int findkLineMin(class Kline arr[], int size)
{
	int min = arr[0].min; // 假设数组的第一个元素是最小值
	int MinNumber = 0;
	for (int i = 1; i < size; i++) {
		if (arr[i].min < min) {
			min = arr[i].min; // 如果当前元素小于当前最小值，则更新最小值
			MinNumber = i;
		}
	}
	return MinNumber;
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
void PrintWave(class Kline* kLine, class wave* wave, bool risingWave)
{
	class wave* current = wave;
	if (risingWave == true)
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
void insertAtEndWave(class wave* wave, int MaxLocation, int MinLocation,bool rise) 
{
	if (wave == NULL) {
		printf("wave == NULL");
		return;
	}
	if (wave->MinLocation == 0&& wave->MaxLocation==0)
	{
		wave->MaxLocation = MaxLocation;
		wave->MinLocation = MinLocation;
	}
	class wave* current = wave;
	while (current->next != NULL)
	{
		current = current->next;
		if (current->MinLocation == MinLocation&&rise==true) {
			current->MaxLocation = MaxLocation;
			return;
		}
		else if (current->MaxLocation == MaxLocation && rise == false)
		{
			current->MinLocation = MinLocation;
			return;
		}
	}
	if (current->MinLocation == MinLocation) {
		current->MaxLocation = MaxLocation;
		return;
	}
	else if (current->MaxLocation == MaxLocation && rise == false)
	{
		current->MinLocation = MinLocation;
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
int Judge_Drop_Double_Repo(class Kline* kLine, int Data_Length, class wave* wave)
{
	class wave* current = wave;
	int WaveLength = GetWaveLength(wave);
	int FibonacciLevels = 0;
	puts("drop:");
	for (int i = 0; i < WaveLength; i++)
	{
		int num = 0;
		char sign = 0;
		int temp = 0;
		int MinLocation = findkLineMin(&kLine[current->MaxLocation], current->MinLocation - current->MaxLocation) + current->MaxLocation;
		for (int j = current->MinLocation; j < current->MinLocation + 13 && j < Data_Length; j++)
		{
			if (kLine[j].end > kLine[j].MA3_3 && sign == 0) {
				num = j;
				sign = 1;
			}

			if (kLine[j].end < kLine[j].MA3_3 && sign == 1) {
				int MaxLocation = findkLineMin(&kLine[num], j - num) + num;
				FibonacciLevels = (int)((kLine[MaxLocation].max - kLine[MinLocation].min) * 0.618 + kLine[MinLocation].min);
				sign = 2;
			}

			if (kLine[j].end > kLine[j].MA3_3 && sign == 2) {
				sign = 3;
				temp = j;
				break;
			}
		}
		if (sign == 3)
		{
			for (int k = temp; k < temp + 10; k++)
			{
				if (kLine[k].min < FibonacciLevels) {
					sign = -1;
					break;
				}
			}
		}
		if (sign == 3)
			printf("%d-%d\t%d\n", kLine[current->MaxLocation].day, kLine[current->MinLocation].day, sign);
		else
			printf("%d-%d\t%d\tnone\n", kLine[current->MaxLocation].day, kLine[current->MinLocation].day, sign);

		current = current->next;
	}
	return 0;
}
int Judge_Rise_Double_Repo(class Kline* kLine, int Data_Length, class wave* wave)
{
	class wave* current = wave;
	int WaveLength = GetWaveLength(wave);
	int FibonacciLevels = 0;
	puts("rise:");
	for (int i = 0; i < WaveLength; i++)
	{
		int num = 0;
		char sign = 0;
		int temp = 0;
		int MaxLocation = findkLineMax(&kLine[current->MinLocation], current->MaxLocation - current->MinLocation) + current->MinLocation;
		for (int j = current->MaxLocation; j < current->MaxLocation + 13 && j < Data_Length; j++)
		{
			if (kLine[j].end < kLine[j].MA3_3 && sign == 0) {
				num = j;
				sign = 1;
			}

			if (kLine[j].end > kLine[j].MA3_3 && sign == 1) {
				int MinLocation = findkLineMin(&kLine[num], j - num) + num;
				FibonacciLevels = (int)((kLine[MaxLocation].max - kLine[MinLocation].min) * 0.618 + kLine[MinLocation].min);
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
			printf("%d-%d\t%d\tnone\n", kLine[current->MinLocation].day, kLine[current->MaxLocation].day, sign);

		current = current->next;
	}
	return 0;
}
BOOL Judge_wave(class Kline* kLine, int Data_Length)
{
	int count = 0;
	int drop = 0;
	BOOL returnValue = FALSE;
	wave* rising_wave = (wave*)calloc(1, sizeof(wave));
	wave* drop_wave = (wave*)calloc(1, sizeof(wave));
	for (int i = 7; i < Data_Length; i++)
	{
		if (kLine[i].end >= kLine[i].MA3_3)
			count++;
		else {
			//在迅猛推进期间只允许1次收盘价低于MA3_3
			if (count > 0)
				drop++;

			if (drop == 2) {
				drop = 0;
				count = 0;
			}
		}
		if (count > 13)
			insertAtEndWave(rising_wave, i, i - (count - 1) - drop, true);
	}
	//PrintWave(kLine, rising_wave, true);
	returnValue = Judge_Rise_Double_Repo(kLine, Data_Length, rising_wave);
	ReleaseLinkedList(rising_wave);
	for (int i = 7; i < Data_Length; i++)
	{
		if (kLine[i].end <= kLine[i].MA3_3)
			count++;
		else {
			//在迅猛推进期间只允许1次收盘价高于MA3_3
			if (count > 0)
				drop++;

			if (drop == 2) {
				drop = 0;
				count = 0;
			}
		}
		if (count > 13)
			insertAtEndWave(drop_wave, i - (count - 1) - drop, i, false);
	}
	//PrintWave(kLine, drop_wave, false);
	if (Judge_Drop_Double_Repo(kLine, Data_Length, drop_wave) == 1)
	{
		ReleaseLinkedList(drop_wave);
		return (returnValue || TRUE);
	}
	else {
		ReleaseLinkedList(drop_wave);
		return (returnValue || FALSE);
	}
	return -1;
}