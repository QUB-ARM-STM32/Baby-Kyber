/*
 * BabyKyber.c
 *
 */

#include "BabyKyber.h"
#include <stdlib.h>
#include <string.h>
#include <stdio.h>

int* multiply(int a[4], int b[4])
{
	// takes A and the secret and returns the correct product
	int* res = (int*)malloc(sizeof(int) * 7);
	for (int i = 0; i < 7; i++)
		res[i] = 0;

	// temp to store values of natrix multiplication
	for (int i = 0; i < 4; i++){
		for (int j = 0; j < 4; j++){
			printf("%d * %d = %d\r\n", a[i], b[j], a[i] * b[j]);
			res[i + j] += a[i] * b[j];
		}
	}

	// return the MOD
	for (int i = 0; i < 7; i++){
		//printf("%dx^%d ", res[i], i);
		res[i] = (res[i] % 17 + 17) % 17;
	}

	return res;

}

int* AddVectors(int* a, int* b, int length)
{
	int* res = (int*)malloc(sizeof(int) * length);
	for (int i = 0; i < length; i++)
		res[i] = 0;

	for (int i = 0; i < length; i++)
	{
		res[i] = a[i] + b[i];
		res[i] = (res[i] % 17 + 17) % 17;
	}
	return res;
}

void PolyDiv(int r[7])
{
	int q[7] = {0};
	int D[7] = {1, 0, 0, 0, 1, 0, 0};
	while (GetDegree(r, 7) >= 4)
	{
		int* d = (int*)malloc(sizeof(int) *  7);
		for (int i = 0; i < 7; i++)
			d[i] = 0;

		ShiftRight(D, d, 7, GetDegree(r, 7) - GetDegree(D, 7));
		q[GetDegree(r, 7) - GetDegree(D, 7)] = r[GetDegree(r, 7) / d[GetDegree(d, 7)]];

		MultiplyConstant(d, 7, q[GetDegree(r, 7) - GetDegree(D, 7)]);
		Sub(r, d, r, 7);
		free(d);
	}
}

int GetDegree(int* a, int length)
{
	for (int i = length - 1; i >= 0; i--)
	{
		if (a[i] != 0)
			return i;
	}
	return 0;
}

int CheckZero(int* a, int length)
{
	for (int i = 0; i < length; i++)
	{
		if (a[i] != 0)
		{
			return 0;
		}
	}
	return 1;
}

void ShiftRight(int* a, int* res, int length, int shift)
{
	for (int i = shift; i < length; i++)
	{
		res[i] = a[i - shift];
	}
}

void MultiplyConstant(int* a, int length, int num)
{
	// multiply by the num
	for (int i = 0; i < length; i++)
	{
		a[i] *= num;
	}
}

void Sub(int* a, int* b, int* res, int length)
{
	for (int i = 0; i < length; i++)
	{
		res[i] = a[i] - b[i];
	}
}

void GenerateT(int A[4][4], int s[2][4], int e[2][4], int t[2][4])
{

	for (int i = 0; i < 4; i+=2)
	{
		//t[i] = (int*)malloc(sizeof(int) * 4);

		// A * s
		int* vector1 = multiply(A[0 + i], s[0]);
		int* vector2 = multiply(A[1 + i], s[1]);

		printf("Vector1: \r\n");
		PrintPoly(vector1, 7);


		int* addedRes = AddVectors(vector1, vector2, 7);
		free(vector1);
		free(vector2);

		printf("After Add: \r\n");
		PrintPoly(addedRes, 7);

		// poly div
		PolyDiv(addedRes);

		printf("After Poly: \r\n");
		PrintPoly(addedRes, 4);

		// copy values from addedRes to t

		int* temp = AddVectors(addedRes, e[i / 2], 4);
		free(addedRes);
		memcpy(t[i / 2], temp, sizeof(int) * 4);
	}
}

void PrintPoly(int* a, int length)
{
	for (int i = length - 1; i >= 0; i--)
	{
		printf("%dx^%d ", a[i], i);
	}
	printf("\r\n");
}

void Encrypt(int A[2][4], int t[2][4], int r[2][4], int e1[2][4], int e2[4], int length, int u[2][4], int v[4], int* data)
{
	// Transform the M value
	int q = 9;
	int maxLengthPoly = length * 2 - 1;

	for (int i = 0; i < length; i++){
		data[i] *= -q;
	}

	printf("Checking upslacing: \r\n");
	PrintPoly(data, 4);

	// Calculate u
	for (int i = 0; i < length / 2; i++){
		int* res1 = multiply(A[i], r[0]);
		int* res2 = multiply(A[i + 2], r[1]);

		printf("Res1: \r\n");
		PrintPoly(res1, 7);

		printf("Res2: \r\n");
		PrintPoly(res2, 7);

		int* addRes = AddVectors(res1, res2, maxLengthPoly);

		printf("Added Res: \r\n");
		PrintPoly(addRes, 7);

		PolyDiv(addRes);

		printf("After Reduction: \r\n");
		PrintPoly(addRes, 7);

		for (int j = 0; j < length; ++j){
			u[i][j] = addRes[j];
		}
		memcpy(u[i], addRes, sizeof(int) * sizeof(addRes));
		free(addRes);
		//u[i / 2][0] = addRes;

		free(res1);
		free(res2);
	}

	// Add to u
	int* temp1 = AddVectors(u[0], e1[0], length);
	printf("After error Add: \r\n");
	PrintPoly(temp1, length);

	int* temp2 = AddVectors(u[1], e1[1], length);

	for (int i = 0; i < 4; i++){
		u[0][i] = temp1[i];
		u[1][i] = temp2[i];
	}

	free(temp1);
	free(temp2);

	// print u
	printf("U values:\r\n");
	PrintPoly(u[0], length);
	PrintPoly(u[1], length);


	// generate v now
	int* vTemp1 = multiply(t[0], r[0]);
	int* vTemp2 = multiply(t[1], r[1]);

	int* vAdded = AddVectors(vTemp1, vTemp2, 7);
	free(vTemp1);
	free(vTemp2);

	PolyDiv(vAdded);

	int* addE2 = AddVectors(vAdded, e2, 4);
	free(vAdded);

	printf("v:\r\n");
	PrintPoly(addE2, 4);

	int* vFinal = AddVectors(addE2, data, 4);
	free(addE2);

	memcpy(v, vFinal, sizeof(int) * 4);
	free(vFinal);

	printf("v:\r\n");
	PrintPoly(v, 4);

}

// decrypts the polynomial with u and v
void Decrypt(int s[][4], int u[][4], int v[4], int* res)
{
	int* temp1 = multiply(s[0], u[0]);
	int* temp2 = multiply(s[1], u[1]);

	int* s_by_u = AddVectors(temp1, temp2, 7);
	PolyDiv(s_by_u);
	Modulo17(s_by_u, s_by_u, 4);

	free(temp1);
	free(temp2);
	free(s_by_u);

	Sub(v, s_by_u, res, 4);
	free(s_by_u);

	PrintPoly(res, 4);

	// check the coefficiants
	for (int i = 0; i < 4; ++i){
		if (res[i] < 5){
			res[i] = 0;
		}
		else if (res[i] < 14){
			res[i] = 1;
		}
		else{
			res[i] = 0;
		}
	}
}

void Modulo17(int vector[], int* res, int length)
{
	for (int i = 0; i < length; i++){
		res[i] = (vector[i] % 17 + 17) % 17;
	}
}


