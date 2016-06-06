#include <windows.h>
#include <time.h>			//pentru random
#include <stdio.h>
#include "resource.h"

#define M 140
#define BUTTON1_ID 12500
#define BUTTON2_ID 12501
#define BUTTON3_ID 12502
#define EDIT1_ID 12503
#define EDIT2_ID 12504
#define BUTTON4_ID 12505
#define BUTTON5_ID 12506

typedef struct {
	int delai;
	int lai;
}BestStructEver;

BestStructEver mutari[M];
int dim = 0;

const int offsetX = 30;
const int offsetY = 100;

const wchar_t g_szClassName[] = L"myWindowClass";
int matrice[M];
int i = 0;
HWND hwnd_global;
void init() {
	for (int i = 0; i < M; i++)
	{
		if (rand() % 3 == 0) {
			matrice[i] = -1; //pun -1 unde am memoria ocupata
		}
		else {
			matrice[i] = 0;
		}
	}
}
void interschimba(int i1, int i2) { // functie folosita pt sortare
	int aux;
	wchar_t buff1[30];
	wchar_t buff2[30];
	GetDlgItemText(hwnd_global, 1000 * (i1 % 10) + (i1 / 10), buff1, 30);
	GetDlgItemText(hwnd_global, 1000 * (i2 % 10) + (i2 / 10), buff2, 30);
	SetDlgItemText(hwnd_global, 1000 * (i1 % 10) + (i1 / 10), buff2);
	SetDlgItemText(hwnd_global, 1000 * (i2 % 10) + (i2 / 10), buff1);
	aux = matrice[i1];
	matrice[i1] = matrice[i2];
	matrice[i2] = aux;
}

void increment(HWND hwnd) {
	i++;
	if (i == M) { //cand ajunge cursorul pe pozitia dimensiunii maxime 
		MessageBox(hwnd, L"Out of memory!!!", L"ERROR", MB_ICONERROR); //afisez mesajul
		SendMessage(hwnd, WM_DESTROY, NULL, NULL); // inchid programul ?? 
	}
}