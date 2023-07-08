#pragma once
#ifndef GET_DATA_H
#define	GET_DATA_H
class Kline
{
public:
	int day;
	int first;   //����
	int end;     //����
	int max;     //���
	int min;     //���
	int averageValue;//������ƽ��ֵ
	int MA3_3;
	int MA7_5;
	int MA25_5;
	int vol;     //�ɽ���
	long terr;   //�ɽ���
	double aem;   //���
	double swing; //�ǵ���
	int rise;  //�ǵ���
	double turn;  //������

	void initKline()
	{
		day = 0;
		first = 0;
		end = 0;
		max = 0;
		min = 0;
		averageValue = 0;
		MA3_3 = 0;
		MA7_5 = 0;
		MA25_5 = 0;
		vol = 0;
		terr = 0;
		aem = 0;
		swing = 0;
		rise = 0;
		turn = 0;
	}
};

void urlencode(const char* secid, int beg, int end, const int klt, const int fqt, char url[1024]);
int request(char url[1024], char data[1024 * 1024]);
int date(int date[3]); //����ת��ʱ���ʽ
void Get_Data(class Kline* kLine, int Data_Length, char* data);
int Get_Data_Length(const char* str);
int getCurrentDate();
void PerformDMA(class Kline* kLine, int Data_Length);
#endif // !GET_DATA_H
