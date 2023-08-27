#pragma once
#ifndef DOUBLE_REPO_H
#define DOUBLE_REPO_H
int findkLineMax(class Kline arr[], int size);
int findkLineMin(class Kline arr[], int size);
int GetWaveLength(class wave* wave);//��ȡ������
void PrintWave(class Kline* kLine, class wave* wave, bool risingWave);//��ӡ����
void ReleaseLinkedList(class wave* head);//�ͷ�����
void insertAtEndWave(class wave* wave, int MaxLocation, int MinLocation, bool rise);//��������
int Judge_Drop_Double_Repo(class Kline* kLine, int Data_Length, class wave* wave);//�ж�˫�ش�͸
int Judge_Rise_Double_Repo(class Kline* kLine, int Data_Length, class wave* wave);//�ж�˫�ش�͸
bool Judge_wave(class Kline* kLine, int Data_Length);
bool Judge_Drop_wave(class Kline* kLine, int Data_Length);
bool Judge_Rise_wave(class Kline* kLine, int Data_Length);
#endif // DOUBLE_REPO_H
