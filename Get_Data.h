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
	/*�û��ƶ�ƽ����*/
	int MA3_3;
	int MA7_5;
	int MA25_5;
	double RandomIndicatorK;//��ѡ���ָ��K
	double RandomIndicatorD;//��ѡ���ָ��D
	int vol;     //�ɽ���
	long terr;   //�ɽ���
	double aem;   //���
	double swing; //�ǵ���
	int rise;  //�ǵ���
	double turn;  //������
};

void urlencode(const char* secid, int beg, int end, const int klt, const int fqt, char url[1024]);
int request(char url[1024], char data[1024 * 1024]);
int date(int date[3]); //����ת��ʱ���ʽ
void Get_Data(class Kline* kLine, int Data_Length, char* data);
int Get_Data_Length(const char* str);
int getCurrentDate();
void PerformDMA(class Kline* kLine, int Data_Length);
void PreferredRandomIndicator(class Kline* kLine, int Data_Length);//������ѡ���ָ��
#endif // !GET_DATA_H
