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