#define  _CRT_SECURE_NO_WARNINGS
#include <stdio.h>
#include <math.h>
#include <stdlib.h>
#include <Windows.h>
#include <time.h>

void Get_Data(class Kline* kLine, int Data_Length);
int Get_Data_Length(const char* FileName);
int date(int a[]); //����ת��ʱ���ʽ
int GetsNumOfInteger(int num);//��ȡ����λ��
int Judging_Trends(class Kline* kLine, int Data_Length);
int Judge_rising_wave(class Kline* kLine, int Data_Length, class rising_wave* rising);
int counter_first = 0;
char X[20] = { 0 };
class Kline
{
public:
	int day;
	int first;   //����
	int end;     //����
	int max;     //���
	int min;     //���
	int averageValue;//������ƽ��ֵ
	int vol;     //�ɽ���
	int terr;   //�ɽ���
	double aem;   //���
	double swing; //�ǵ���
	int rise;  //�ǵ���
	double turn;  //������
	
	Kline()
	{
		day = 0;
		first = 0;
		end = 0;
		max = 0;
		min = 0;
		averageValue = 0;
		vol = 0;
		terr = 0;
		aem = 0;
		swing = 0;
		rise = 0;
		turn = 0;
	}
};
class rising_wave
{
public:
	int MaxLocation;//�������ֵ��λ��
	int MinLocation ;
	int SubLowLocation;
	rising_wave()
	{
		MaxLocation = 0;
		MinLocation = 0;
		SubLowLocation = 0;
	}
};

int main()
{
	//system("python ÿ��.py");
	int Data_Length = Get_Data_Length("today_s.csv") - 1;
	class Kline* kLine = (class Kline*)malloc(sizeof(class Kline) * Data_Length);
	if (kLine == NULL)
		return 0;
	Get_Data(kLine, Data_Length);
	Judging_Trends(kLine, Data_Length);
	
	free(kLine);
	double a, b, c;
	double First_Departure_Target, Second_Departure_Target, Third_Departure_Target;
	printf("������b�㣬a��,c��");
	scanf_s("%lf%lf%lf", &b, &a, &c);
	First_Departure_Target = fabs(b - a) + fabs(b - c) * 0.618 + c;
	Second_Departure_Target = fabs(b - a) + fabs(b - c) * 1 + c;
	Third_Departure_Target = fabs(b - a) + fabs(b - c) * 1.382 + c;
	printf("��һ�볡Ŀ��Ϊ��%f\n", First_Departure_Target);
	printf("�ڶ��볡Ŀ��Ϊ��%f\n", Second_Departure_Target);
	printf("�����볡Ŀ��Ϊ��%f\n", Third_Departure_Target);
	return 0;
}

void Get_Data(class Kline* kLine, int Data_Length)
{

	FILE* fp=fopen("today_s.csv","r");

	fseek(fp, 91, SEEK_SET);//�����ļ�ǰ91�ֽ�
	for (int H = 0; H < Data_Length; H++)
	{
		int days[3] = { 0 };
		fscanf_s(fp, "%d-%d-%d", &days[0], &days[1], &days[2]);
		kLine[H].day = date(days);

		fscanf_s(fp, ",%d,%d,%d,%d,%d,%d,%lf,%lf,%d,%lf\n", &kLine[H].first, &kLine[H].end, &kLine[H].max, &kLine[H].min, &kLine[H].vol, &kLine[H].terr,&kLine[H].aem, &kLine[H].swing, &kLine[H].rise, &kLine[H].turn);
		kLine[H].averageValue = (kLine[H].max + kLine[H].min) / 2;
	}

	fclose(fp);
	return;
}

int Get_Data_Length(const char* FileName)
{
	FILE* fp = fopen(FileName, "r");
	int Length = 0;
	char* c = (char*)malloc(sizeof(char));
	if (c != NULL)
	{
		while (*c != EOF)
		{
			*c = fgetc(fp);
			if (*c == '\n')
			{
				Length++;
			}
		}
	}
	free(c);
	fclose(fp);
	return Length;
}

int date(int date[3]) //����ת��ʱ���ʽ
{
	return date[0] * 10000 + date[1] * 100 + date[2];
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

int getCurrentDate() 
{
	int year = 0;
	int month = 0;
	int day = 0;
	time_t currentTime;
	time(&currentTime);

	struct tm* localTime = localtime(&currentTime);

	year = localTime->tm_year + 1900;
	month = localTime->tm_mon + 1;
	day = localTime->tm_mday;

	return (year * 10000 + month * 100 + day);
}

int Judging_Trends(class Kline* kLine, int Data_Length)
{
	class rising_wave* rising = (class rising_wave*)malloc(sizeof(class rising_wave));
	if (Judge_rising_wave(kLine, Data_Length, rising) == 1)
	{
		return 0;
	}
	return 0;

}


int Judge_rising_wave(class Kline* kLine, int Data_Length, class rising_wave* rising)
{
	int MaxLocation = 0;//�������ֵ��λ��
	int MinLocation = 0;
	int SubLowLocation = 0;
	int time = getCurrentDate();
	for (int i = 0; i < Data_Length; i++)
	{
		if (kLine[i].day > time - 100)
		{
			MaxLocation = findAverageValueMax(&kLine[i], Data_Length - i) + i;
			break;
		}
		else
		{
			MaxLocation = -1;
		}
	}
	for (int i = 0; i < Data_Length; i++)
	{
		if (kLine[i].day > kLine[MaxLocation].day - 600)
		{
			MinLocation = findAverageValueMin(&kLine[i], Data_Length - i) + i;
			break;
		}
		else
		{
			MinLocation = -1;
		}
	}
	for (int i = 0; i < Data_Length; i++)
	{
		if (kLine[i].day > kLine[MaxLocation].day)
		{
			SubLowLocation = findAverageValueMin(&kLine[i], Data_Length - i) + i;
			break;
		}
		else
		{
			SubLowLocation = -1;
		}
	}

	if (MaxLocation == -1 || MinLocation == -1 || SubLowLocation == -1)
	{
		return 0;
	}

	if (kLine[MaxLocation].day - kLine[MinLocation].day < 7
		|| kLine[SubLowLocation].day <= kLine[MaxLocation].day
		|| kLine[SubLowLocation].averageValue >= kLine[MaxLocation].averageValue
		|| kLine[SubLowLocation].averageValue <= kLine[MinLocation].averageValue)//�ж��Ƿ����������
	{
		return 0;
	}
	else
	{
		int count = 0;
		int downFx = 0, downFy = 0;
		for (int i = MinLocation; i < MaxLocation - 1; i++)
		{

			if (kLine[i + 1].averageValue - kLine[i].averageValue < 0)
			{
				count++;
				if (count == 1)
				{
					downFx = kLine[i].day;
					downFy = kLine[i].averageValue;
				}
				if (count == 5 && Slope(downFx, downFy, kLine[i].day, kLine[i].averageValue) < -10)
				{
					return 0;
				}
			}
			else
			{
				count = 0;
			}

		}

		count = 0;
		downFx = 0;
		downFy = 0;

		for (int i = MaxLocation; i < SubLowLocation - 1; i++)
		{
			if (kLine[i + 1].averageValue > kLine[i].averageValue)
			{
				count++;
				if (count == 1)
				{
					downFx = kLine[i].day;
					downFy = kLine[i].averageValue;
				}
				if (count == 5 && Slope(downFx, downFy, kLine[i].day, kLine[i].averageValue) > 10)
				{
					return 0;
				}
			}
			else
			{
				count = 0;
			}

		}
	}
	rising->MaxLocation = MaxLocation;
	rising->MinLocation = MinLocation;
	rising->SubLowLocation = SubLowLocation;
	return 1;
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

