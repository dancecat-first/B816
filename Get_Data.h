#ifndef GET_DATA_H
#define	GET_DATA_H
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
