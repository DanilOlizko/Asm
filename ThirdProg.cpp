// ThirdProg.cpp: ���������� ����� ����� ��� ����������� ����������.
//���������� ������������ ���������� ������� ����� ���������� �� ������ ������. 
#include "stdafx.h"
#include <stdio.h>
#include <stdlib.h>
#include <time.h>
#include <iostream>
#include <math.h>
#include <iomanip>
using namespace std; 
bool testj(int *mas, int v, int m)
{
	int i;
	_asm {
		//	     for(int i = 0; i < m; i++)
		mov	DWORD PTR i, 0          // ������������� ���������� ����� i
		jmp	SHORT LN4testj
		LN3testj :
		mov	eax, DWORD PTR i  // ��������� ���������� �����
			add	eax, 1
			mov	DWORD PTR i, eax
			LN4testj :
		mov	eax, DWORD PTR i  // ����� �� �� ����� �����?
			cmp	eax, DWORD PTR m
			jge	SHORT LN2testj  // �� - ����� �� �����

								//         if(v == mas[i]) return false;
								//   ��������, ������ �� v � ������ mas (���� ������ - ���������� 0)

			mov	eax, DWORD PTR i
			mov	ecx, DWORD PTR mas
			mov	edx, DWORD PTR v
			cmp	edx, DWORD PTR[ecx + eax * 4] // ecx+eax*4 = ����� mas[i]   
			jne	SHORT LN1testj
			xor	al, al
			jmp	SHORT LN5testj
			LN1testj :

		//     return true;

		jmp	SHORT LN3testj
			LN2testj :
		mov	al, 1                  // ����� ���������� TRUE 
			LN5testj :
	}
}

int det(int **&mtr, int sizze, int *mas, int m)
{
	int r = 0; int j;
	int s = 1;
	__asm {

		//     for(int j = 0; j < size; j++){

		mov	DWORD PTR j, 0   // ������������� ���������� ����� j
		jmp	SHORT LN6det
		LN5det :
		mov	eax, DWORD PTR j // ��������� ���������� ����� j
			add	eax, 1
			mov	DWORD PTR j, eax
			LN6det :
		mov	eax, DWORD PTR j  // ����� �� ����� �����?
			cmp	eax, DWORD PTR sizze
			jge	LN4det			// ��

								//         if (testj(mas, j, m)){        ���

			mov	eax, DWORD PTR m   // ���������� ������ ������� testj
			push	eax		  // ��� 3 ��������� ���������� ����� ����
			mov	ecx, DWORD PTR j
			push	ecx
			mov	edx, DWORD PTR mas
			push	edx
			call	testj		// ����� testj	
			add	esp, 12		// ������������ �����			
			movzx	eax, al		// ��������� � eax
			test	eax, eax        // ��������� =0 ?
			je	SHORT LN3det    // ��

								//             if(m == size - 1) return mtr[m][j];     ���
								//      ���� ������ ������� = 1, �� ���������� ����� ������� �������
			mov	eax, DWORD PTR sizze
			sub eax, 1
			cmp	DWORD PTR m, eax
			jne	SHORT LN2det          // ���� if �� �����������
			mov	eax, DWORD PTR mtr // ����� 
			mov	ecx, DWORD PTR[eax]
			mov	edx, DWORD PTR m
			mov	eax, DWORD PTR[ecx + edx * 4]
			mov	ecx, DWORD PTR j
			mov	eax, DWORD PTR[eax + ecx * 4] // eax = mtr[m][j] (������� �������)
			jmp	SHORT LN7det               // �� ������� �� �������

										   //             else{       ����� ���������� ���� for 

			jmp	SHORT LN3det
			LN2det :

		//       mas[m] = j; m-������� ���. ������� �������� mas[m]=j

		mov	eax, DWORD PTR m
			mov	ecx, DWORD PTR mas
			mov	edx, DWORD PTR j
			mov	DWORD PTR[ecx + eax * 4], edx   // mas[m] = j
												//  ����������� ������������ ������� �� ��������� ������ ������
												//                 r = r + s * mtr[m][j] * det(mtr, size, mas, m + 1);

			mov	eax, DWORD PTR mtr
			mov	ecx, DWORD PTR[eax]
			mov	edx, DWORD PTR m
			mov	eax, DWORD PTR[ecx + edx * 4]
			mov	ecx, DWORD PTR j    // ������ ������������ ��-�� mtr[m][j]   
			mov	esi, DWORD PTR s
			imul	esi, DWORD PTR[eax + ecx * 4]  // esi=s * mtr[m][j]
			mov	edx, DWORD PTR m // ���������� 4-� ���������� ��� ������ det 
			add	edx, 1
			push	edx
			mov	eax, DWORD PTR mas
			push	eax
			mov	ecx, DWORD PTR sizze
			push	ecx
			mov	edx, DWORD PTR mtr
			push	edx
			call	det		// ����� ������� det
			add	esp, 16		 // ������������ �����			
			imul	esi, eax    // esi = s * mtr[m][j] * det(mtr, size, mas, m + 1)    
			add	esi, DWORD PTR r // esi=r
			mov	DWORD PTR r, esi

			mov	eax, DWORD PTR s
			neg	eax
			mov	DWORD PTR s, eax
			LN3det :

		
		jmp	LN5det
			LN4det :

		
		mov	eax, DWORD PTR r   // ������� ���������� � eax 
			LN7det :
	}
}

int _tmain(int argc, _TCHAR* argv[])
{
	int i, j;
	struct strum { int matsize; int **uk; } stru;
	// ��������� stru, ������������ ������ ������� � �� �����
	int **mtr = NULL /* ����� ������ ������� */, *mas = NULL /*���. ������ ��������*/;
	int size;  /* ����������� ������� */
	srand((unsigned)time(NULL));    /* ��� ��������� ����. ����� */
	cout << "Enter matrix size: ";
	cin >> size;
	stru.matsize = size;  /* ������ ������� � ��������� */
	mas = new int[size];  /* ������ ��� ���. ������� �������� */
	mtr = new int*[size]; /* ��������� ������ ��� ������� */
	for (j = 0; j < size; j++)
		mtr[j] = new int[size];
	stru.uk = mtr;   /* ����� ������ ������� � ��������� */
	char sw;
	cout << "\n1 - Typing automatically\n2 - Typing manually\n";
	cin >> sw;
	switch (sw)   /* ���� ������� ���������� ������� ����. ������� */
	{
	case '1':
		for (i = 0; i < size; i++)
		{
			for (j = 0; j < size; j++)
				mtr[i][j] = rand() % 20; /* ���������  ������� ���� ������� */
		}
		break;

	case '2':     /* ���������� ������� ������� � ����������  */
		for (i = 0; i < size; i++)
		{
			for (j = 0; j < size; j++)
				cin >> mtr[i][j];
		}
		break;
	default:
		cout << "\n";
		system("pause");
		return 0;
	}
	/* ����� ������� �� ����� */
	printf("Vasha matrica:\n");
	for (i = 0; i < size; i++) {
		for (j = 0; j < size; j++) printf("%2d ", mtr[i][j]);
		cout << "\n";
	}
	/* ������ ������������ ( ����������) � ��� ����� �� ����� */
	printf("determinant: %d\n", det(mtr, size, mas, 0));
	/* ������������ ������ */
	for (j = 0; j < size; j++) delete[]mtr[j];
	delete[]mtr;
	delete[]mas;
	/* ����� �� ��������� */
	system("pause");
	return 0;
}

