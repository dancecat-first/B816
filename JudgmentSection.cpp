

int Judging_Trends(class Kline *kLine,int Data_Length)
{
	int count = 0;
	float a, b;
	float sum = 0;
	float average = 0;
	float variance = 0;
	for (int i = 0; i < Data_Length-1; i++)
	{
		if (kLine[i + 1].max - kLine[i].max < 0)
		{
			for (int j = 0; j < i; j++)
			{
				sum += kLine[j].max;
			}
			average = sum / i;
			for (int x = 0; x < i; x++)
			{
				variance += kLine[x].max - average;
			}
			variance = variance / i;
			leastSquareLinearFit(&(kLine[count].day), &(kLine[count].max), i, a, b);
			count = i;
		}
	}
}

//-------------------------------------------------------------
//���� : ��С���˷�ֱ����� y = a��x + b�� ����ϵ��a �� b
//���� : x -- ���նȵ�����
//       y --  ���ʵ�����
//       num �����������Ԫ�ظ�����x[]��y[]��Ԫ�ظ����������
//       a,b ���Ƿ���ֵ
//���� : ��ϼ���ɹ�����true, ��ϼ���ʧ�ܷ���false
//���� ��Long
//-------------------------------------------------------------
bool leastSquareLinearFit(int x[], int y[], const int num, float& a, float& b)
{
	float sum_x2 = 0.0;
	float sum_y = 0.0;
	float sum_x = 0.0;
	float sum_xy = 0.0;


	try {
		for (int i = 0; i < num; ++i) {
			sum_x2 += x[i] * x[i];
			sum_y += y[i];
			sum_x += x[i];
			sum_xy += x[i] * y[i];
		}
	}
	catch (...) {
		return false;
	}


	a = (num * sum_xy - sum_x * sum_y) / (num * sum_x2 - sum_x * sum_x);
	b = (sum_x2 * sum_y - sum_x * sum_xy) / (num * sum_x2 - sum_x * sum_x);


	return true;
}