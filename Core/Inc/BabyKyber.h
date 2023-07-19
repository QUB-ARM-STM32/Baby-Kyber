/*
 * BabyKyber.h
 *
 */

#ifndef INC_BABYKYBER_H_
#define INC_BABYKYBER_H_

int* AddVectors(int* a, int* b, int length);
void PolyDiv(int a[7]);
int GetDegree(int* a, int length);
int CheckZero(int* a, int length);
void ShiftRight(int* a, int* res, int length, int shift);
void MultiplyConstant(int* a, int length, int num);
void Sub(int* a, int* b, int* res, int length);
int* multiply(int a[4], int b[4]);
void GenerateT(int A[4][4], int s[2][4], int e[2][4], int t[2][4]);
void PrintPoly(int* a, int length);
void Encrypt(int A[2][4], int t[2][4], int r[2][4], int e1[2][4], int e2[4], int length, int u[2][4], int v[4], int* data);
void Decrypt(int s[][4], int u[][4], int v[4], int* res);
void Modulo17(int vector[], int* res, int length);

#endif /* INC_BABYKYBER_H_ */
