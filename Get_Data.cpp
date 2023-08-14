#define  _CRT_SECURE_NO_WARNINGS
#define _WINSOCK_DEPRECATED_NO_WARNINGS
#define GOLDEN_CROSS 1
#define DEATH_CROSS 2
#include <WINSOCK2.H> 
#include <openssl/ssl.h>
#include <openssl/err.h>
#include <sstream>
#include<tchar.h>
#include<WS2tcpip.h>
#include<iostream>
#include <windows.h>
#include <stdio.h>
#include <math.h>
#include <stdlib.h>
#include <time.h>
#include"Get_Data.h"
#include"Data.h"

#define BUFF_SIZE 1024
#pragma warning(disable : 4075)
#pragma warning(disable : 4996)
#pragma warning(disable : 6387)
#pragma comment(lib,"ws2_32.lib")
#pragma comment(lib,"libssl.lib")
#pragma comment(lib,"libcrypto.lib")

using namespace std;

void urlencode(const char* secid, int beg, int end, const int klt, const int fqt, char url[1024])
{
	/*
		secid : 根据 get_futures_base_info 函数获取
		获取4个交易所期货数据，取 secid 列来获取 secid
		beg : 开始日期 例如 20200101
		end : 结束日期 例如 20200201
		klt : k线间距 默认为 101 即日k
		klt : 1 1 分钟
		klt : 5 5 分钟
		klt : 101 日
		klt : 102 周
		fqt : 复权方式
		不复权 : 0
		前复权 : 1
		后复权 : 2
	*/

	char fields[] = "fields1=f1%2Cf2%2Cf3%2Cf4%2Cf5%2Cf6%2Cf7%2Cf8%2Cf9%2Cf10%2Cf11%2Cf12%2Cf13&fields2=f51%2Cf52%2Cf53%2Cf54%2Cf55%2Cf56%2Cf57%2Cf58%2Cf59%2Cf60%2Cf61";
	sprintf(url, "push2his.eastmoney.com/api/qt/stock/kline/get?%s&beg=%d&end=%d&rtntype=6&secid=%s&klt=%d&fqt=%d", fields, beg, end, secid, klt, fqt);
	return;
}

int request(char url[1024], char data[1024 * 1024])
{
	//加载SSL错误信息 
	SSL_load_error_strings();
	//添加SSL的加密/HASH算法 
	SSLeay_add_ssl_algorithms();
	//客户端，服务端选择SSLv23_server_method() 
	const SSL_METHOD* meth = SSLv23_client_method();
	//建立新的SSL上下文 
	SSL_CTX* ctx = SSL_CTX_new(meth);
	if (ctx == NULL)
	{
		ERR_print_errors_fp(stderr);
		cout << "SSL_CTX_new error !";
		return -1;
	}

	WSADATA wsaData;
	if (WSAStartup(MAKEWORD(2, 2), &wsaData) != 0)
	{
		return -1;
	}
	SOCKET client = socket(AF_INET, SOCK_STREAM, IPPROTO_TCP);
	if (client == INVALID_SOCKET)
	{
		cout << "socket error !";
		return -1;
	}

	string host = "push2his.eastmoney.com";
	unsigned short port = 443;
	hostent* ip = gethostbyname(host.c_str());
	if (ip == nullptr)
	{
		cout << "get ip error 1";
		return -1;
	}
	sockaddr_in sin;
	sin.sin_family = AF_INET;
	sin.sin_port = htons(port);
	sin.sin_addr = *(in_addr*)ip->h_addr_list[0];

	if (connect(client, (sockaddr*)&sin, sizeof(sin)) == SOCKET_ERROR)
	{
		cout << "connect error 1";
		return -1;
	}

	//建立SSL 
	int ret;
	SSL* ssl = SSL_new(ctx);
	if (ssl == NULL)
	{
		cout << "SSL NEW error";
		return -1;
	}
	//将SSL与TCP SOCKET 连接 
	SSL_set_fd(ssl, (int)client);
	//SSL连接 
	ret = SSL_connect(ssl);
	if (ret == -1)
	{
		cout << "SSL ACCEPT error ";
		return -1;
	}

	stringstream stream;
	stream << "GET https://" << url << " HTTP/1.1\r\n";
	stream << "Accept: */*\r\n";
	//stream << "Accept-Encoding: gzip, deflate, br\r\n";//不要编码，否则还得多一个解码的步骤
	stream << "Accept-Language: zh-Hans-CN, zh-Hans; q=0.8, en-US; q=0.5, en; q=0.3\r\n";
	stream << "Connection: Close\r\n";
	stream << "Host: " << host << "\r\n";
	stream << "User-Agent: Mozilla/5.0 (Windows NT 10.0; Win64; x64) AppleWebKit/537.36 (KHTML, like Gecko) Chrome/64.0.3282.140 Safari/537.36 Edge/17.17134\r\n";
	stream << "\r\n";

	string s = stream.str();
	const char* sendData = s.c_str();
	ret = SSL_write(ssl, sendData, (int)strlen(sendData));
	if (ret == -1)
	{
		cout << "SSL write error !";
		return -1;
	}
	char* rec = (char*)calloc(1024 * 1024, sizeof(char));
	int start = 0;
	if (rec == NULL)
		return 0;
	while ((ret = SSL_read(ssl, rec + start, 1024)) > 0)
	{
		start += ret;
	}
	rec[start] = '\n';
	rec[start + 1] = 0;


	//关闭SSL套接字 
	SSL_shutdown(ssl);
	//释放SSL套接字 
	SSL_free(ssl);
	//释放SSL会话环境 
	SSL_CTX_free(ctx);
	closesocket(client);
	WSACleanup();

	if (strlen(rec) < 360){
		free(rec);//释放内存
		return -1;
	}

	strncpy(data, strstr(rec, "\"klines\":[") + strlen("\"klines\":["), strlen(strstr(rec, "\"klines\":[") + strlen("\"klines\":[")) + 1);
	data[1024 * 1024 - 1] = '\0';//避免出现没有为字符串“data”添加字符串零终止符
	data[strlen(data) - 4] = '\0';

	free(rec);//释放内存
	return 0;
}

int date(int date[3]) //用于转换时间格式
{
	return date[0] * 10000 + date[1] * 100 + date[2];
}

void Get_Data(class Kline* kLine, int Data_Length, char* data)
{
	char* temp = data;
	for (int H = 0; H < Data_Length; H++)
	{
		int days[3] = { 0 };
		sscanf_s(temp, "\" %d-%d-%d,%d,%d,%d,%d,%d,%ld,%lf,%lf,%d,%lf\",", &days[0], &days[1], &days[2], &kLine[H].first, &kLine[H].end, &kLine[H].max, &kLine[H].min, &kLine[H].vol, &kLine[H].terr, &kLine[H].aem, &kLine[H].swing, &kLine[H].rise, &kLine[H].turn);
		temp = strstr(temp, "\",\"") + strlen("\",");
		kLine[H].day = date(days);
	}
	PerformDMA(kLine, Data_Length);
	PreferredRandomIndicator(kLine, Data_Length);
	CalculateMACD(kLine, Data_Length);
	return;
}


void PreferredRandomIndicator(class Kline* kLine, int Data_Length)//计算首选随机指标
{
	const int N = 8;
	double* temp = (double*)calloc(Data_Length, sizeof(double));
	int TEMP = 0;
	if (temp == NULL) {
		printf("temp == NULL");
		return;
	}
	for (int i = N; i < Data_Length; i++)
	{
			int lowestLow = kLine[i - N].min;  // 假设数组的第一个元素是最小值
			int highestHigh = kLine[i - N].max;
			for (int j = (i - N) + 1; j <= i; ++j)
			{
				if (kLine[j].min < lowestLow)
					lowestLow = kLine[j].min;
				if (kLine[j].max > highestHigh)
					highestHigh = kLine[j].max;
			}
			temp[i]= ((double)(kLine[i].end - lowestLow) / (double)(highestHigh - lowestLow)) * 100;
	}

	for (int i = N + 2; i < Data_Length; i++)
	{
		if (i > N + 2)
			kLine[i].KD.RandomIndicatorK = kLine[i - 1].KD.RandomIndicatorK + (temp[i] - kLine[i - 1].KD.RandomIndicatorK) / 3;
		else
			kLine[i].KD.RandomIndicatorK = (temp[i] + temp[i - 1] + temp[i - 2]) / 3;
	}
	for (int i = N + 4; i < Data_Length; i++)
	{
		if (i > N + 4)
			kLine[i].KD.RandomIndicatorD = kLine[i - 1].KD.RandomIndicatorD + (kLine[i].KD.RandomIndicatorK - kLine[i - 1].KD.RandomIndicatorD) / 3;
		else
			kLine[i].KD.RandomIndicatorD = (kLine[i].KD.RandomIndicatorK + kLine[i - 1].KD.RandomIndicatorK + kLine[i - 2].KD.RandomIndicatorK) / 3;
	}
	free(temp);

	for (int i = 0; i < Data_Length; i++)
	{
		if (kLine[i].KD.RandomIndicatorK < kLine[i].KD.RandomIndicatorD)
			TEMP = i;
		if (kLine[i].KD.RandomIndicatorK >= kLine[i].KD.RandomIndicatorD) {
			if (i - 1 == TEMP)
				kLine[i].KD.PenetrateSignal = GOLDEN_CROSS;
		}
	}
	TEMP = 0;
	for (int i = 0; i < Data_Length; i++)
	{
		if (kLine[i].KD.RandomIndicatorK > kLine[i].KD.RandomIndicatorD)
			TEMP = i;
		if (kLine[i].KD.RandomIndicatorK <= kLine[i].KD.RandomIndicatorD) {
			if (i - 1 == TEMP)
				kLine[i].KD.PenetrateSignal = DEATH_CROSS;
		}
	}
	return;
}

void CalculateMACD(class Kline* kLine, int Data_Length)//计算MACD
{
	kLine[0].macd.EMA1 = kLine[0].end;
	kLine[0].macd.EMA2 = kLine[0].end;
	kLine[0].macd.MACD = 0;
	kLine[0].macd.SignalLine = 0;
	for (int i = 1; i < Data_Length; i++)
	{
		kLine[i].macd.EMA1 = kLine[i - 1].macd.EMA1 + 0.213 * (kLine[i].end - kLine[i - 1].macd.EMA1);
		kLine[i].macd.EMA2 = kLine[i - 1].macd.EMA2 + 0.108 * (kLine[i].end - kLine[i - 1].macd.EMA2);
		kLine[i].macd.MACD = kLine[i].macd.EMA1 - kLine[i].macd.EMA2;
		kLine[i].macd.SignalLine = kLine[i - 1].macd.MACD + 0.199 * (kLine[i].macd.MACD - kLine[i - 1].macd.MACD);
	}

	int temp = 0;
	for (int i = 0; i < Data_Length; i++)
	{
		if (kLine[i].macd.MACD < kLine[i].macd.SignalLine)
			temp = i;
		if (kLine[i].macd.MACD >= kLine[i].macd.SignalLine) {
			if (i - 1 == temp)
				kLine[i].macd.PenetrateSignal = GOLDEN_CROSS;
		}
	}
	temp = 0;
	for (int i = 0; i < Data_Length; i++)
	{
		if (kLine[i].macd.MACD > kLine[i].macd.SignalLine)
			temp = i;
		if (kLine[i].macd.MACD <= kLine[i].macd.SignalLine) {
			if (i - 1 == temp)
				kLine[i].macd.PenetrateSignal = DEATH_CROSS;
		}
	}
	/*for (int i = 1; i < Data_Length; i++)
	{
		if(kLine[i].macd.PenetrateSignal!=0)
			printf("%d\t%d\n", kLine[i].day,kLine[i].macd.PenetrateSignal);
	}*/
}
void PerformDMA(class Kline* kLine, int Data_Length)//计算置换移动平均线
{
	/*先进行计算简单移动平均线，再进行置换*/
	for (int H = 0; H < Data_Length; H++)
	{
		int sum = 0;
		if (H >= (3-1)) {
			sum = 0;
			for (int i = 0; i < 3; i++)
			{
				sum += kLine[H - i].end;
			}
			kLine[H].MA3_3 = sum / 3;
		}
		else
			kLine[H].MA3_3 = 0;

		if (H >= (7-1)) {
			sum = 0;
			for (int i = 0; i < 7; i++)
			{
				sum += kLine[H - i].end;
			}
			kLine[H].MA7_5 = sum / 7;
		}
		else
			kLine[H].MA7_5 = 0;

		if (H >= (25-1)) {
			sum = 0;
			for (int i = 0; i < 25; i++)
			{
				sum += kLine[H - i].end;
			}
			kLine[H].MA25_5 = sum / 25;
		}
		else
			kLine[H].MA25_5 = 0;

	}
	for (int i = Data_Length - 1; i > 0; i--)
	{
		if (i > 3)
			kLine[i].MA3_3 = kLine[i - 3].MA3_3;
		else
			kLine[i].MA3_3 = 0;

		if (i > 5)
			kLine[i].MA7_5 = kLine[i - 5].MA7_5;
		else
			kLine[i].MA7_5 = 0;

		if (i > 5)
			kLine[i].MA25_5 = kLine[i - 5].MA25_5;
		else
			kLine[i].MA25_5 = 0;
	}

}
int Get_Data_Length(const char* str)
{
	if (str == NULL) {
		printf("str == NULL");
		return -1;
	}

	int count = 0;
	while (*str) {
		if (*str == '\"') {
			count++;
		}
		str++;
	}

	return count / 2;
}

int getCurrentDate()
{
	int year = 0;
	int month = 0;
	int day = 0;
	time_t currentTime;
	time(&currentTime);

	struct tm* localTime = localtime(&currentTime);

	// 提取年份、月份和日期
    // localtime 函数返回的年份的基准是 1900，所以需要加上 1900
    // 月份从 0 开始计数，所以需要加上 1
    year = localTime->tm_year + 1900;
    month = localTime->tm_mon + 1;
    day = localTime->tm_mday;

    // 返回格式化后的日期值
    return (year * 10000 + month * 100 + day);
}
