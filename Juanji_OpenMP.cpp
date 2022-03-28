// Juanji_OpenMP.cpp : 此文件包含 "main" 函数。程序执行将在此处开始并结束。
//

#include<cstdlib>
#include<iostream>
#include<vector>
#include <windows.h>
#include <time.h>
#include <omp.h>
using namespace std;

int const kernel = 3;
int const map = 4000;



	//定义被卷积矩阵
float A[map * map];


//定义卷积核矩阵（其实也是一个数组，数组元素的个数3*3）
float B[kernel * kernel] =
{
1, 1, 1,
1, 1, 1, 
1, 1, 1
};

int const outm = map - kernel + 1; //被卷积矩阵的维数-卷积核的维数+1
	//计算卷积过程中的被卷积矩阵的宽和高（就是把宽拉成和卷积核的高一样，这样才好对应相乘）
int const convAw = kernel * kernel;
int const convAh = map * map;
float A_convert[convAh * convAw] = { 0 };//定义一个卷积过程中的矩阵（也就是被拉长过后的矩阵）

 int ThreadFunc1(void*)
{
	for (int i = 0; i < outm; i++)
	{
		for (int j = 0; j < outm; j++)
		{
			int wh = i * outm * convAw + j * convAw;
			int col1 = i * map + j;
			A_convert[wh] = A[col1]; //第一次循环时把A[0] 的值赋给 A_convert[0]
			A_convert[wh + 1] = A[col1 + 1];//第一次循环时把A[1] 的值赋给 A_convert[1]
			A_convert[wh + 2] = A[col1 + 2];//第一次循环时把A[2] 的值赋给 A_convert[2]
			int col2 = (i + 1) * map + j;
			A_convert[wh + 3] = A[col2]; //第一次循环时把A[8] 的值赋给 A_convert[3]
			A_convert[wh + 4] = A[col2 + 1];//第一次循环时把A[9] 的值赋给 A_convert[4]
			A_convert[wh + 5] = A[col2 + 2];//第一次循环时把A[10] 的值赋给 A_convert[5]
			int col3 = (i + 2) * map + j;
			A_convert[wh + 6] = A[col3]; //第一次循环时把A[16] 的值赋给 A_convert[6]
			A_convert[wh + 7] = A[col3 + 1]; //第一次循环时把A[17] 的值赋给 A_convert[7]
			A_convert[wh + 8] = A[col3 + 2]; //第一次循环时把A[18] 的值赋给 A_convert[8]
		}
	}
	

	vector<int> C(map*map);
	#pragma omp parallel for num_threads(8)
	for (int i = 0; i < outm; i++)
	{
		for (int j = 0; j < outm; j++)
		{
			int a = 0;
			int wh = i * outm * convAw + j * convAw;
			for (int m = 0; m < convAw; m++)
			{
				a += A_convert[wh + m] * B[m];
			}
			C[i*map+j] = a; //在C中添加数据a
		}
	}
	/*
	cout << "被卷积矩阵 :" << endl;
	for (int i = 0; i < map; i++)
	{
		for (int j = 0; j < map; j++)
		{
			cout << A[i * map + j] << " ";
		}
		cout << endl;
	}

	cout << endl;
	//输出卷积核矩阵
	cout << "卷积核矩阵:" << endl;
	for (int i = 0; i < kernel; i++)
	{
		for (int j = 0; j < kernel; j++)
		{
			cout << B[i * kernel + j] << " ";
		}
		cout << endl;
	}
	cout << endl;
	//输出卷积后输出矩阵
	cout << "卷积后输出矩阵：" << endl;
	for (int i = 0; i < outm; i++)
	{
		for (int j = 0; j < outm; j++)
		{
			cout << C[i * outm + j] << " ";
		}
		cout << endl;
	}
	*/
	return 0;

}

int main()
{

	//cout << omp_get_num_procs() << endl;
	for (int i = 0; i < map; i++)
		for (int j = 0; j < map; j++)
		{
			A[i*map+j] = 1.0*j;
		};
	clock_t start, end;
	start = clock();
	ThreadFunc1(0);
	end = clock();
	printf("matrix multiply time: %0.6lf sec\n", ((double)end - start) / CLK_TCK);
}


