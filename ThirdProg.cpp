// ThirdProg.cpp: определяет точку входа для консольного приложения.
//Вычисление определителя квадратной матрицы путем разложения по первой строке. 
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
		mov	DWORD PTR i, 0          // инициализация переменной цикла i
		jmp	SHORT LN4testj
		LN3testj :
		mov	eax, DWORD PTR i  // инкремент переменной цикла
			add	eax, 1
			mov	DWORD PTR i, eax
			LN4testj :
		mov	eax, DWORD PTR i  // дошли ли до конца цикла?
			cmp	eax, DWORD PTR m
			jge	SHORT LN2testj  // да - выход из цикла

								//         if(v == mas[i]) return false;
								//   проверка, входит ли v в массив mas (если входит - возвращаем 0)

			mov	eax, DWORD PTR i
			mov	ecx, DWORD PTR mas
			mov	edx, DWORD PTR v
			cmp	edx, DWORD PTR[ecx + eax * 4] // ecx+eax*4 = адрес mas[i]   
			jne	SHORT LN1testj
			xor	al, al
			jmp	SHORT LN5testj
			LN1testj :

		//     return true;

		jmp	SHORT LN3testj
			LN2testj :
		mov	al, 1                  // иначе возвращаем TRUE 
			LN5testj :
	}
}

int det(int **&mtr, int sizze, int *mas, int m)
{
	int r = 0; int j;
	int s = 1;
	__asm {

		//     for(int j = 0; j < size; j++){

		mov	DWORD PTR j, 0   // инициализация переменной цикла j
		jmp	SHORT LN6det
		LN5det :
		mov	eax, DWORD PTR j // инкремент переменной цикла j
			add	eax, 1
			mov	DWORD PTR j, eax
			LN6det :
		mov	eax, DWORD PTR j  // дошли до конца цикла?
			cmp	eax, DWORD PTR sizze
			jge	LN4det			// да

								//         if (testj(mas, j, m)){        нет

			mov	eax, DWORD PTR m   // подготовка вызова функции testj
			push	eax		  // все 3 параметра передаются через стек
			mov	ecx, DWORD PTR j
			push	ecx
			mov	edx, DWORD PTR mas
			push	edx
			call	testj		// вызов testj	
			add	esp, 12		// освобождение стека			
			movzx	eax, al		// результат в eax
			test	eax, eax        // результат =0 ?
			je	SHORT LN3det    // да

								//             if(m == size - 1) return mtr[m][j];     нет
								//      если размер матрицы = 1, то возвращаем левый верхний элемент
			mov	eax, DWORD PTR sizze
			sub eax, 1
			cmp	DWORD PTR m, eax
			jne	SHORT LN2det          // если if не выполняется
			mov	eax, DWORD PTR mtr // иначе 
			mov	ecx, DWORD PTR[eax]
			mov	edx, DWORD PTR m
			mov	eax, DWORD PTR[ecx + edx * 4]
			mov	ecx, DWORD PTR j
			mov	eax, DWORD PTR[eax + ecx * 4] // eax = mtr[m][j] (элемент матрицы)
			jmp	SHORT LN7det               // на возврат из функции

										   //             else{       иначе продолжаем цикл for 

			jmp	SHORT LN3det
			LN2det :

		//       mas[m] = j; m-элемент всп. массива индексов mas[m]=j

		mov	eax, DWORD PTR m
			mov	ecx, DWORD PTR mas
			mov	edx, DWORD PTR j
			mov	DWORD PTR[ecx + eax * 4], edx   // mas[m] = j
												//  рекурсивное накапливание миноров по элементам первой строки
												//                 r = r + s * mtr[m][j] * det(mtr, size, mas, m + 1);

			mov	eax, DWORD PTR mtr
			mov	ecx, DWORD PTR[eax]
			mov	edx, DWORD PTR m
			mov	eax, DWORD PTR[ecx + edx * 4]
			mov	ecx, DWORD PTR j    // расчет расположения эл-та mtr[m][j]   
			mov	esi, DWORD PTR s
			imul	esi, DWORD PTR[eax + ecx * 4]  // esi=s * mtr[m][j]
			mov	edx, DWORD PTR m // подготовка 4-х параметров для вызова det 
			add	edx, 1
			push	edx
			mov	eax, DWORD PTR mas
			push	eax
			mov	ecx, DWORD PTR sizze
			push	ecx
			mov	edx, DWORD PTR mtr
			push	edx
			call	det		// вызов функции det
			add	esp, 16		 // выравнивание стека			
			imul	esi, eax    // esi = s * mtr[m][j] * det(mtr, size, mas, m + 1)    
			add	esi, DWORD PTR r // esi=r
			mov	DWORD PTR r, esi

			mov	eax, DWORD PTR s
			neg	eax
			mov	DWORD PTR s, eax
			LN3det :

		
		jmp	LN5det
			LN4det :

		
		mov	eax, DWORD PTR r   // возврат результата в eax 
			LN7det :
	}
}

int _tmain(int argc, _TCHAR* argv[])
{
	int i, j;
	struct strum { int matsize; int **uk; } stru;
	// структура stru, определяющая размер матрицы и ее адрес
	int **mtr = NULL /* сдесь данные матрицы */, *mas = NULL /*всп. массив индексов*/;
	int size;  /* размерность матрицы */
	srand((unsigned)time(NULL));    /* для генерации случ. чисел */
	cout << "Enter matrix size: ";
	cin >> size;
	stru.matsize = size;  /* размер матрицы в структуру */
	mas = new int[size];  /* память для всп. массива индексов */
	mtr = new int*[size]; /* выделение памяти под матрицу */
	for (j = 0; j < size; j++)
		mtr[j] = new int[size];
	stru.uk = mtr;   /* адрес памяти матрицы в структуру */
	char sw;
	cout << "\n1 - Typing automatically\n2 - Typing manually\n";
	cin >> sw;
	switch (sw)   /* если выбрано заполнение матрицы случ. числами */
	{
	case '1':
		for (i = 0; i < size; i++)
		{
			for (j = 0; j < size; j++)
				mtr[i][j] = rand() % 20; /* заполняем  матрицу случ числами */
		}
		break;

	case '2':     /* заполнение матрицы вручную с клавиатуры  */
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
	/* вывод матрицы на экран */
	printf("Vasha matrica:\n");
	for (i = 0; i < size; i++) {
		for (j = 0; j < size; j++) printf("%2d ", mtr[i][j]);
		cout << "\n";
	}
	/* расчет определителя ( рекурсивно) и его вывод на экран */
	printf("determinant: %d\n", det(mtr, size, mas, 0));
	/* освобождение памяти */
	for (j = 0; j < size; j++) delete[]mtr[j];
	delete[]mtr;
	delete[]mas;
	/* выход из программы */
	system("pause");
	return 0;
}

