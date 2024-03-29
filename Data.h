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
	double RandomIndicatorK;//首选随机指标K
	double RandomIndicatorD;//首选随机指标D
	unsigned char PenetrateSignal;//穿透信号
};
class Kline
{
public:
	int day;
	int first;   //开盘
	int end;     //收盘
	int max;     //最高
	int min;     //最低
	/*置换移动平均线*/
	int MA3_3;
	int MA7_5;
	int MA25_5;
	struct RandomIndicator KD;
	struct MACD macd;
	int vol;     //成交量
	long terr;   //成交额
	double aem;   //振幅
	double swing; //涨跌幅
	int rise;  //涨跌额
	double turn;  //换手率
};
class wave
{
public:
	int MaxLocation;//储存最大值的位置
	int MinLocation;
	class wave* next;
	wave()
	{
		MaxLocation = 0;
		MinLocation = 0;
		next = NULL;
	}
};
#endif // DATA_H