#ifndef GET_DATA_H
#define	GET_DATA_H
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

void urlencode(const char* secid, int beg, int end, const int klt, const int fqt, char url[1024]);
int request(char url[1024], char data[1024 * 1024]);
int date(int date[3]); //用于转换时间格式
void Get_Data(class Kline* kLine, int Data_Length, char* data);
int Get_Data_Length(const char* str);
int getCurrentDate();
void PerformDMA(class Kline* kLine, int Data_Length);
void PreferredRandomIndicator(class Kline* kLine, int Data_Length);//计算首选随机指标
void CalculateMACD(class Kline* kLine, int Data_Length);//计算MACD
#endif // !GET_DATA_H
