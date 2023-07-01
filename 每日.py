#!/usr/bin/python3 
import datetime
from lib2to3.refactor import get_all_fix_names
import pandas as pd
import requests
from urllib.parse import urlencode
import time

date01 = datetime.date.today()
year=date01.year
data=year*10000+date01.month*100+date01.day


def get_ftures_k_history(secid: str, beg: str = '20220101', end: str = '20500101', klt: int = 101, fqt: int = 1) -> pd.DataFrame:
    '''
    获取k线数据

    Parameters
    ----------
    secid : 根据 get_futures_base_info 函数获取
    获取4个交易所期货数据，取 secid 列来获取 secid
    beg : 开始日期 例如 20200101
    end : 结束日期 例如 20200201
    klt : k线间距 默认为 101 即日k
            klt : 1 1 分钟
            klt : 5 5 分钟
            klt : 101 日
            klt : 102 周
    fqt: 复权方式
            不复权 : 0
            前复权 : 1
            后复权 : 2 

    Return
    ------
    DateFrame : 包含期货k线数据

    '''
    EastmoneyHeaders = {
        'User-Agent': 'Mozilla/5.0 (Windows NT 6.3; WOW64; Trident/7.0; Touch; rv:11.0) like Gecko',
        'Accept': '*/*',
        'Accept-Language': 'zh-CN,zh;q=0.8,zh-TW;q=0.7,zh-HK;q=0.5,en-US;q=0.3,en;q=0.2',
        'Referer': 'http://quote.eastmoney.com/center/gridlist.html',
    }
    EastmoneyKlines = {
        'f51': '日期',
        'f52': '开盘',
        'f53': '收盘',
        'f54': '最高',
        'f55': '最低',
        'f56': '成交量',
        'f57': '成交额',
        'f58': '振幅',
        'f59': '涨跌幅',
        'f60': '涨跌额',
        'f61': '换手率',


    }
    fields = list(EastmoneyKlines.keys())
    columns = list(EastmoneyKlines.values())
    fields2 = ",".join(fields)

    params = (
        ('fields1', 'f1,f2,f3,f4,f5,f6,f7,f8,f9,f10,f11,f12,f13'),
        ('fields2', fields2),
        ('beg', beg),
        ('end', end),
        ('rtntype', '6'),
        ('secid', secid),
        ('klt', f'{klt}'),
        ('fqt', f'{fqt}'),
    )
    base_url = 'https://push2his.eastmoney.com/api/qt/stock/kline/get'
    url = base_url+'?'+urlencode(params)
    json_response = requests.get(
        url, headers=EastmoneyHeaders).json()

    data = json_response['data']
    if data is None:
        print(secid, '无数据')
        return None
    # code = data['code']
    # name
    # name = data['name']
    klines = data['klines']

    rows = []
    for _kline in klines:

        kline = _kline.split(',')
        rows.append(kline)

    df = pd.DataFrame(rows, columns=columns)

    return df


if "__main__" == __name__:

    
   #以 utf-8 的编码格式打开指定文件

    f = open("mubiao.txt",encoding = "utf-8")
    a=''
    a=f.read()

    f.close()
    secid=a
    futures_name ="today_s"
    #print(f'正在获取期货：{futures_name} 的今日 k 线数据')
    # 获取期货历史 日k 线数据
    # secid 可以在文件 期货信息表.csv 中查看，更改 secid 即可获取不同的期货数据
    df = get_ftures_k_history(secid)
    #df.insert(0, '期货名称', [futures_name for _ in range(len(df))])
    df.to_csv(f'{futures_name}.csv', index=None, encoding='utf-8')
    #print(f'期货 : {futures_name} 的 k 线数据已保存到文件 {futures_name}.csv' )