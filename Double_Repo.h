#pragma once
#ifndef DOUBLE_REPO_H
#define DOUBLE_REPO_H
int findkLineMax(class Kline arr[], int size);
int findkLineMin(class Kline arr[], int size);
int GetWaveLength(class wave* wave);//获取链表长度
void PrintWave(class Kline* kLine, class wave* wave, bool risingWave);//打印链表
void ReleaseLinkedList(class wave* head);//释放链表
void insertAtEndWave(class wave* wave, int MaxLocation, int MinLocation, bool rise);//创建链表
int Judge_Drop_Double_Repo(class Kline* kLine, int Data_Length, class wave* wave);//判断双重穿透
int Judge_Rise_Double_Repo(class Kline* kLine, int Data_Length, class wave* wave);//判断双重穿透
bool Judge_wave(class Kline* kLine, int Data_Length);
bool Judge_Drop_wave(class Kline* kLine, int Data_Length);
bool Judge_Rise_wave(class Kline* kLine, int Data_Length);
#endif // DOUBLE_REPO_H
