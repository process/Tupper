#include <stdio.h>
#include <stdlib.h>
#include <math.h>
#include <gmp.h>
#include <windows.h>
#include <GL/gl.h>

#include "resource.h"

char * SELFREFER = "960939379918958884971672962127852754715004339660129306651505519271702802395266424689642842174350718121267153782770623355993237280874144307891325963941337723487857735749823926629715517173716995165232890538221612403238855866184013235585136048828693337902491454229288667081096184496091705183454067827731551705405381627380967602565625016981482083418783163849115590225610003652351370343874461848378737238198224849863465033159410054974700593138339226497249461751545728366702369745461014655997933798537483143786841806593422227898388722980000748404719";
char DrawArea[17][107];

HDC hDC;
HGLRC hRC;

LRESULT CALLBACK GLProc(HWND hwnd, UINT message, WPARAM wParam, LPARAM lParam);
INT_PTR CALLBACK DlgProc(HWND hwnd, UINT message, WPARAM wParam, LPARAM lParam);
void initGL(HWND hwnd);
void PutPixel(float x, float y);

HWND hwndWindow;
HWND hwndChild;

char MouseDown;

int WINAPI WinMain(HINSTANCE hInstance, HINSTANCE hPrevInstance, LPSTR lpCmdLine, int nCmdShow)
{
	WNDCLASSEX wndCls;
	MSG msg;

	ZeroMemory(&wndCls, sizeof(WNDCLASSEX));
	wndCls.cbSize = sizeof(WNDCLASSEX);
	wndCls.style = CS_OWNDC | CS_HREDRAW | CS_VREDRAW;
	wndCls.hInstance = hInstance;
	wndCls.lpfnWndProc = GLProc;
	wndCls.hIcon = LoadIcon(NULL, IDI_APPLICATION);
	wndCls.hCursor = LoadCursor(NULL, IDC_ARROW);
	wndCls.hbrBackground = CreateSolidBrush(0);
	wndCls.lpszClassName = "Windy";
	wndCls.hIconSm = LoadIcon(NULL, IDI_APPLICATION);

	RegisterClassEx(&wndCls);
	
	hwndWindow = CreateDialog(
		hInstance,
		MAKEINTRESOURCE(IDD_DIALOG1),
		NULL, DlgProc);
		
	HWND PencilButton = GetDlgItem(hwndWindow, PENCIL);
	SendMessage(PencilButton, BM_SETCHECK, BST_CHECKED, 0);
		
	hwndChild = CreateWindowEx(
		0, "Windy", "GL", WS_CHILD | WS_BORDER,
		21, 10, 535, 85,
		hwndWindow, NULL, hInstance, NULL);
	
	ShowWindow(hwndChild, 1);
	UpdateWindow(hwndChild);
	
	initGL(hwndChild);
	
	HWND EditBox = GetDlgItem(hwndWindow, K_EDITBOX);
	SendMessage(EditBox, WM_SETTEXT, (WPARAM)NULL, (LPARAM)SELFREFER);
	SendMessage(hwndWindow, WM_COMMAND, DRAWGRAPH, 0);
	
	while(GetMessage(&msg, NULL, 0, 0))
	{
		TranslateMessage(&msg);
		DispatchMessage(&msg);
	}

	return msg.wParam;
}

void initGL(HWND hwnd)
{
	hDC = GetDC(hwnd);	

	PIXELFORMATDESCRIPTOR pfd=
	{
		sizeof(PIXELFORMATDESCRIPTOR),
		1,
		PFD_DRAW_TO_WINDOW |
		PFD_SUPPORT_OPENGL |
		PFD_DOUBLEBUFFER,
		PFD_TYPE_RGBA,
		16,0, 0, 0, 0, 0, 0,
		0, 0, 0, 0, 0, 0, 0,
		16, 0, 0, PFD_MAIN_PLANE,
		0, 0, 0, 0
	};

	unsigned int PixelFormat = ChoosePixelFormat(hDC,&pfd);
	SetPixelFormat(hDC,PixelFormat,&pfd);

	hRC = wglCreateContext(hDC);

	wglMakeCurrent(hDC,hRC);

	glDisable(GL_DEPTH_TEST);

	glMatrixMode(GL_PROJECTION);
	glLoadIdentity();
	glOrtho(0,107,17,0,0,1);
	glViewport(0,0,535,85);

	glMatrixMode(GL_MODELVIEW);
	glLoadIdentity();
	glClearColor(255,255,255,255);
	glColor3f(0,0,0);

	glClear(GL_COLOR_BUFFER_BIT);
	SwapBuffers(hDC);
}

void PutPixel(float x, float y)
{
	glVertex2f(x, y);
	glVertex2f(x, y + 1);
	glVertex2f(x + 1, y + 1);
	glVertex2f(x + 1, y);
}

void MakeImage()
{
	int x,y;
	
	glClear(GL_COLOR_BUFFER_BIT);
	glBegin(GL_QUADS);
	
	for(y = 0; y < 17; y++)		
		for(x = 0; x < 107; x++)
			if(DrawArea[y][x])
				PutPixel(106-x, y);
				
	glEnd();
	SwapBuffers(hDC);
}

void GraphTupp()
{	
	mpz_t TupperNum;
	mpz_t Remainder;
	mpz_t Den;
	mpz_t Temp;
	mpz_t Num;
	
	mpz_init(TupperNum);
	mpz_init(Remainder);
	mpz_init(Den);
	mpz_init(Temp);
	mpz_init(Num);
	
	int x, y;
	
	HWND EditBox = GetDlgItem(hwndWindow, K_EDITBOX);
	char * TupperNumStr = (char*)malloc(1024);
	SendMessage(EditBox, WM_GETTEXT, 1024, (LPARAM)TupperNumStr);
	
	ZeroMemory(&DrawArea, 17*107);
	
	for(y = 0; y < 17; y++)
	{
		mpz_set_str(TupperNum, TupperNumStr, 10);
		mpz_add_ui(TupperNum, TupperNum, y);
		
		mpz_set_ui(Temp, 17);
		mpz_mod(Remainder, TupperNum, Temp);
		
		mpz_set_ui(Temp, 17);
		mpz_fdiv_q(Num, TupperNum, Temp);
			
		for(x = 0; x < 107; x++)
		{
			mpz_set_si(Temp, (-17) * x);
			mpz_sub(Temp, Temp, Remainder);
			int Exp = mpz_get_ui(Temp);
			
			mpz_set_ui(Temp, 2);
			mpz_pow_ui(Den, Temp, Exp);
			
			mpz_fdiv_q(Temp, Num, Den);
			mpz_mod_ui(Temp, Temp, 2);
						
			if(mpz_cmp_d(Temp, 0))
				DrawArea[y][x] = 1;
		}
	}
	
	MakeImage();
}

void GetK()
{
	mpz_t K;
	mpz_init_set_ui(K, 0);
	
	int x,y;
	for(x = 106; x >= 0; x--) //Most significant bit is (0,16), least is (106,0)
	{
		for(y = 16; y >= 0; y--)
		{
			if(DrawArea[y][x])
				mpz_add_ui(K, K, 1);
				
			if(x || y)
				mpz_mul_ui(K, K, 2);	//Don't LSH for last bit
		}
	}
	
	mpz_mul_ui(K, K, 17);
	
	char * TupperNumStr = (char*)malloc(1024);
	mpz_get_str(TupperNumStr, 10, K);
	
	HWND EditBox = GetDlgItem(hwndWindow, K_EDITBOX);
	SendMessage(EditBox, WM_SETTEXT, 0, (LPARAM)TupperNumStr);
}

void HandleMouse(int x, int y)
{
	char draw;

	HWND PencilButton = GetDlgItem(hwndWindow, PENCIL);
	if(SendMessage(PencilButton, BM_GETCHECK, 0, 0) == BST_CHECKED)
		draw = 1;
	else
		draw = 0;
	
	x /= 5;
	x = 106 - x;
	y /= 5; //find pixel coordinates
	
	DrawArea[y][x] = draw;
	
/*	
	int x0, y0;
	int dx = x - x0;
	int dy = y - y0;
	
	if(dx)
	{
		float m = (float) dy / (float) dx;
		float b = y0 - (m * x0);
		
		dx = (x > x0) ? 1 : -1;
		
		while (x0 != x) 
		{
			x0 += dx;
			y0 = m*x0 + b;
			DrawArea[y0][x0] = draw;
		}
	}
	
	x0 = x;
	y0 = y;
*/		
	MakeImage();	
}

INT_PTR CALLBACK DlgProc(HWND hwnd, UINT message, WPARAM wParam, LPARAM lParam)
{	
	switch(message)
	{			
		case WM_DESTROY:
		case WM_CLOSE:
			ReleaseDC(WindowFromDC(hDC), hDC);
			wglDeleteContext(hRC);
			PostQuitMessage(0);
			return 1;
			
		case WM_COMMAND:
			switch(wParam & 0xFF)
			{
				case DRAWGRAPH:
					GraphTupp();
					break;
					
				case GETK:
					GetK();
					break;
					
				case CLEAR:
					ZeroMemory(&DrawArea, 17*107);
					MakeImage();
			}
			return 1;
	}
	return 0;
}

LRESULT CALLBACK GLProc(HWND hwnd, UINT message, WPARAM wParam, LPARAM lParam)
{	
	switch(message)
	{
		case WM_LBUTTONDOWN:
			MouseDown = 1;
			HandleMouse(lParam & 0xFFFF, (lParam >> 16) & 0xFFFF);
			return 0;
			
		case WM_MOUSEMOVE:
			if(GetAsyncKeyState(VK_LBUTTON) & 0x8000 && MouseDown)
				HandleMouse(lParam & 0xFFFF, (lParam >> 16) & 0xFFFF);
			else MouseDown = 0;
			return 0;
	}
	return DefWindowProc(hwnd, message, wParam, lParam);
}
