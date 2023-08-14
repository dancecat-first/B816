#pragma once
#ifndef DATA_H
#define	DATA_H
struct MACD
{
	double EMA1;
	double EMA2;
	double MACD;
	double SignalLine;
	unsigned char PenetrateSignal;
};
struct RandomIndicator
{
	double RandomIndicatorK;//��ѡ���ָ��K
	double RandomIndicatorD;//��ѡ���ָ��D
	unsigned char PenetrateSignal;//��͸�ź�
};
class Kline
{
public:
	int day;
	int first;   //����
	int end;     //����
	int max;     //���
	int min;     //���
	/*�û��ƶ�ƽ����*/
	int MA3_3;
	int MA7_5;
	int MA25_5;
	struct RandomIndicator KD;
	struct MACD macd;
	int vol;     //�ɽ���
	long terr;   //�ɽ���
	double aem;   //���
	double swing; //�ǵ���
	int rise;  //�ǵ���
	double turn;  //������
};
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
		next = NULL;
	}
};
#endif // DATA_H