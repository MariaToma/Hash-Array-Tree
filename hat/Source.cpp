#include <windows.h>
#include <time.h>			//pentru random
#include <stdio.h>
#include "resource.h"

#define M 140
#define BUTTON1_ID 12500 // fiecare buton are un id
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

const int offsetX = 30; //dimensiunile "memoriei" reprezentata prin matrice
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

int adauga_memorie(HWND hwnd) {
	wchar_t numar[10];
	GetDlgItemText(hwnd, EDIT1_ID, numar, 10); // pune valoarea introdusa de la tastarura in "numar"
	int nr = _wtoi(numar);						//transformam din string in int ( <=> atoi() )
	if (matrice[i] == 0) {//verific daca zona e libera
		matrice[i] = nr;	//locatia din matrice ia valoarea numarului
		SetDlgItemText(hwnd, 1000 * (i % 10) + (i / 10), numar);	//scriem numarul
		increment(hwnd);							// incrementam i
	}
	else {
		mutari[dim].delai = i; //de aici pleaca

		do {
			increment(hwnd); // muta cursorul  pana gaseste pozitie libera in matrice
		} while (matrice[i] != 0);

		mutari[dim].lai = i;
		dim++;

		matrice[i] = nr; // //locatia din matrice ia valoarea numarului
		SetDlgItemText(hwnd, 1000 * (i % 10) + (i / 10), numar); //scriem numarul
		increment(hwnd); //incrementam i
	}
	return 0;
}

void sortare() {
	int j = 0, l = 0;
	while (j != i - 1) {
		l = j + 1;
		if (matrice[l] == -1) {
			for (int k = 0; k < dim; k++) {
				if (l == mutari[k].delai) {
					l = mutari[k].lai;
				}
			}
		}

		while (l != i - 0) {
			if (matrice[j] > matrice[l]) {
				interschimba(j, l);
			}

			l++;
			if (matrice[l] == -1) {
				for (int k = 0; k < dim; k++) {
					if (l == mutari[k].delai) {
						l = mutari[k].lai;
					}
				}
			}
		}

		j++;
		if (matrice[j] == -1) {
			for (int k = 0; k < dim; k++) {
				if (j == mutari[k].delai) {
					j = mutari[k].lai;
				}
			}
		}
	}
}

void cautare_stergere(int nr) {
	int i2 = 0;
	int i3;
	int gasit = 0;
	int schimbat = 0;
	while (matrice[i2] != 0 && i2 != M) {
		if (matrice[i2] == nr && !gasit) {
			gasit = 1;
		}
		if (gasit) {
			i3 = i2 + 1;
			if (matrice[i3] == -1) {
				for (int k = 0; k < dim; k++) {
					if (i3 == mutari[k].delai) {
						i3 = mutari[k].lai;
					}
				}
			}
			if (i3 == i) {
				matrice[i2] = 0;
				SetDlgItemText(hwnd_global, 1000 * (i2 % 10) + (i2 / 10), L"");

				int sch = 0;
				for (int k = 0; k < dim; k++) {
					if (i == mutari[k].lai) {
						i = mutari[k].delai - 1;
						sch = 1;
					}
				}
				if (sch == 0) {
					i--;
				}

				break;
			}
			if (matrice[i2] != -1) {
				matrice[i2] = matrice[i3];
				wchar_t buff[30];
				GetDlgItemText(hwnd_global, 1000 * (i3 % 10) + (i3 / 10), buff, 30);
				SetDlgItemText(hwnd_global, 1000 * (i2 % 10) + (i2 / 10), buff);
			}
		}

		if (matrice[i2] == -1) {
			for (int k = 0; k < dim; k++) {
				if (i2 == mutari[k].delai) {
					i2 = mutari[k].lai;
					schimbat = 1;
				}
			}
		}
		if (schimbat) {
			schimbat = 0;
		}
		else {
			i2++;
		}
	}
	if (!gasit) MessageBox(hwnd_global, L"Numarul nu a fost gasit!", L"Ups!", MB_ICONINFORMATION);
}

void cautare(int nr) {
	int i2 = 0;
	int gasit = 0;
	int schimbat = 0;
	while (matrice[i2] != 0 && i2 != M) { //daca este != 0 si daca pozatia sa nu este mai mare decat dimensiunea
		if (matrice[i2] == nr) {
			wchar_t mesaj[100];
			wchar_t numar[10];
			wcscpy_s(mesaj, 100, L"A fost gasit pe pozitia ( ");
			_itow_s(i2 / 10, numar, 10, 10); // "formez" pozitia numarului
			wcscat_s(mesaj, 100, numar);
			wcscat_s(mesaj, 100, L", ");
			_itow_s(i2 % 10, numar, 10, 10);
			wcscat_s(mesaj, 100, numar);
			wcscat_s(mesaj, 100, L" ) !!!");
			MessageBox(hwnd_global, mesaj, L"Gasit", MB_ICONINFORMATION);// denumesc fereastra
			gasit = 1;
		}
		if (matrice[i2] == -1) {
			for (int k = 0; k < dim; k++) {
				if (i2 == mutari[k].delai) {
					i2 = mutari[k].lai;
					schimbat = 1;
				}
			}
		}
		if (schimbat) {
			schimbat = 0;
		}
		else {
			i2++;
		}
	}
	if (!gasit) MessageBox(hwnd_global, L"Numarul nu a fost gasit!", L"Ups!", MB_ICONINFORMATION);
}

INT_PTR CALLBACK procedura(HWND hDlg, UINT message, WPARAM wParam, LPARAM lParam) { //pt dialog
	UNREFERENCED_PARAMETER(lParam);
	switch (message) {
	case WM_INITDIALOG:
	{
						  int i2 = 0;
						  int schimbat = 0;
						  while (matrice[i2] != 0 && i2 != M) {
							  if (matrice[i2] > 0) {
								  wchar_t string[30];
								  GetDlgItemText(hwnd_global, 1000 * (i2 % 10) + (i2 / 10), string, 30);
								  SendDlgItemMessage(hDlg, IDC_LIST1, LB_ADDSTRING, NULL, (LPARAM)string);
								  //combinatia intre GetDlgItem si SendMessage()
							  }
							  if (matrice[i2] == -1) {
								  for (int k = 0; k < dim; k++) {
									  if (i2 == mutari[k].delai) {
										  i2 = mutari[k].lai;
										  schimbat = 1;
									  }
								  }
							  }
							  if (schimbat) {
								  schimbat = 0;
							  }
							  else {
								  i2++;
							  }
						  }
	}
		return (INT_PTR)TRUE;
	case WM_COMMAND:
		if (LOWORD(wParam) == IDOK || LOWORD(wParam) == IDCANCEL) {
			EndDialog(hDlg, LOWORD(wParam));
			return (INT_PTR)TRUE;
		}
		break;
	}
	return (INT_PTR)FALSE;
}

// Step 4: the Window Procedure //?????
LRESULT CALLBACK WndProc(HWND hwnd, UINT msg, WPARAM wParam, LPARAM lParam) {
	switch (msg) {
	case WM_PAINT:
	{
					 PAINTSTRUCT ps;
					 HDC hdc = BeginPaint(hwnd, &ps);
					 //paint:
					 TextOut(hdc, 120, 35, L"Valoare:", 9);
					 EndPaint(hwnd, &ps);
	}
		break;
	case WM_COMMAND:
	{
					   int wmId = LOWORD(wParam);
					   if (wmId == BUTTON1_ID) {
						   adauga_memorie(hwnd);
						   SetDlgItemText(hwnd, EDIT1_ID, L"");
						   SetFocus(GetDlgItem(hwnd, EDIT1_ID));
					   }
					   if (wmId == BUTTON2_ID) {
						   //DialogBox((HINSTANCE)GetWindowLong(hwnd, GWL_HINSTANCE),
						   DialogBox((HINSTANCE)GetWindowLongPtr(hwnd, GWLP_HINSTANCE),
							   MAKEINTRESOURCE(IDD_DIALOG1),
							   hwnd,
							   (DLGPROC)procedura);
					   }
					   if (wmId == BUTTON3_ID) {
						   int nr = GetDlgItemInt(hwnd, EDIT2_ID, NULL, NULL);
						   cautare(nr);
					   }
					   if (wmId == BUTTON5_ID) {
						   sortare();
					   }
					   if (wmId == BUTTON4_ID) {
						   int nr = GetDlgItemInt(hwnd, EDIT2_ID, NULL, NULL);
						   cautare_stergere(nr);
					   }
	}
		break;
	case WM_CLOSE:
		DestroyWindow(hwnd);
		break;
	case WM_DESTROY:
		PostQuitMessage(0);
		break;
	default:
		return DefWindowProc(hwnd, msg, wParam, lParam); // apeleaza procedura predefinita pt Windows
	}
	return 0;
}

int WINAPI WinMain(HINSTANCE hInstance, HINSTANCE hPrevInstance,
	LPSTR lpCmdLine, int nCmdShow) {
	WNDCLASSEX wc;
	HWND hwnd;
	MSG Msg;
	// Initializari ale matricei:
	srand(time(NULL));
	init();

	//Step 1: Registering the Window Class
	wc.cbSize = sizeof(WNDCLASSEX);
	wc.style = 0;
	wc.lpfnWndProc = WndProc;
	wc.cbClsExtra = 0;
	wc.cbWndExtra = 0;
	wc.hInstance = hInstance;
	wc.hIcon = LoadIcon(NULL, IDI_APPLICATION);
	wc.hCursor = LoadCursor(NULL, IDC_ARROW);
	wc.hbrBackground = (HBRUSH)(COLOR_WINDOW + 1);
	wc.lpszMenuName = NULL;
	wc.lpszClassName = g_szClassName;
	wc.hIconSm = LoadIcon(NULL, IDI_APPLICATION);

	if (!RegisterClassEx(&wc)) {
		MessageBox(NULL, L"Window Registration Failed!", L"Error!",
			MB_ICONEXCLAMATION | MB_OK);
		return 0;
	}

	// Step 2: Creating the Window
	hwnd = CreateWindowEx(
		WS_EX_CLIENTEDGE,
		g_szClassName,
		L"Hashed array trees",
		WS_OVERLAPPEDWINDOW,
		CW_USEDEFAULT, CW_USEDEFAULT, 500, 600,
		NULL, NULL, hInstance, NULL);

	if (hwnd == NULL) {
		MessageBox(NULL, L"Window Creation Failed!", L"Error!",
			MB_ICONEXCLAMATION | MB_OK);
		return 0;
	}

	hwnd_global = hwnd;

	ShowWindow(hwnd, nCmdShow);
	UpdateWindow(hwnd);

	// aici am adaugat butoanele:

	for (int i = 0; i < M; i++) {
		HWND x = CreateWindow(L"EDIT", L"", WS_CHILD | WS_VISIBLE | WS_BORDER | ES_READONLY,
			offsetX + (i % 10) * 40, offsetY + (i / 10) * 30, 40, 20, hwnd, (HMENU)(1000 * (i % 10) + (i / 10)), hInstance, NULL);
		if (matrice[i] == -1) { //aici o sa am memoria ocupata
			SetWindowText(x, L"HHH");
		}
	}
	CreateWindow(L"BUTTON", L"Adauga", WS_CHILD | WS_VISIBLE | BS_DEFPUSHBUTTON,
		30, 30, 80, 25, hwnd, (HMENU)BUTTON1_ID, NULL, NULL);
	CreateWindow(L"BUTTON", L"Dialog", WS_CHILD | WS_VISIBLE | BS_DEFPUSHBUTTON,
		30, 0, 80, 25, hwnd, (HMENU)BUTTON2_ID, NULL, NULL);
	CreateWindow(L"BUTTON", L"Cauta", WS_CHILD | WS_VISIBLE | BS_DEFPUSHBUTTON,
		30, 60, 80, 25, hwnd, (HMENU)BUTTON3_ID, NULL, NULL);
	CreateWindowEx(WS_EX_CLIENTEDGE, L"EDIT", L"", WS_CHILD | WS_VISIBLE | ES_NUMBER | WS_BORDER,
		180, 30, 80, 25, hwnd, (HMENU)EDIT1_ID, NULL, NULL);
	CreateWindowEx(WS_EX_CLIENTEDGE, L"EDIT", L"", WS_CHILD | WS_VISIBLE | ES_NUMBER | WS_BORDER,
		250, 60, 80, 25, hwnd, (HMENU)EDIT2_ID, NULL, NULL);
	CreateWindow(L"BUTTON", L"Cauta si sterge", WS_CHILD | WS_VISIBLE | BS_DEFPUSHBUTTON,
		120, 60, 120, 25, hwnd, (HMENU)BUTTON4_ID, NULL, NULL);
	CreateWindow(L"BUTTON", L"Sortare", WS_CHILD | WS_VISIBLE | BS_DEFPUSHBUTTON,
		180, 0, 120, 25, hwnd, (HMENU)BUTTON5_ID, NULL, NULL);

	// Step 3: The Message Loop
	while (GetMessage(&Msg, NULL, 0, 0) > 0) {
		TranslateMessage(&Msg);
		DispatchMessage(&Msg);
	}
	return Msg.wParam;


}

