// Hanvon EBook Hacker
// Copyright (C) 2017-2023 Fankes Studio(qzmmcn@163.com)
// https://github.com/fankes/HanvonEBookHacker
//
// This software is non-free but opensource software: you can redistribute it
// and/or modify it under the terms of the GNU Affero General Public License
// as published by the Free Software Foundation; either
// version 3 of the License, or any later version.
//
// This software is distributed in the hope that it will be useful,
// but WITHOUT ANY WARRANTY; without even the implied warranty of
// MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the GNU
// Affero General Public License for more details.
//
// You should have received a copy of the GNU Affero General Public License
// and eula along with this software.  If not, see
// <https://www.gnu.org/licenses/>
//
// This file is created by fankes on 2023/9/12.
//
// ScreenRefresh.cpp : ����Ӧ�ó������ڵ㡣

#include "stdafx.h"
#include "ScreenRefresh.h"

#define MAX_LOADSTRING 100

// ȫ�ֱ���:
HINSTANCE g_hInst;	   // ��ǰʵ��
HWND g_hWndCommandBar; // ���������

// �˴���ģ���а����ĺ�����ǰ������:
ATOM MyRegisterClass(HINSTANCE, LPTSTR);
BOOL InitInstance(HINSTANCE, int);
LRESULT CALLBACK WndProc(HWND, UINT, WPARAM, LPARAM);

int WINAPI WinMain(HINSTANCE hInstance,
				   HINSTANCE hPrevInstance,
				   LPTSTR lpCmdLine,
				   int nCmdShow)
{
	MSG msg;

	// ִ��Ӧ�ó����ʼ��:
	if (!InitInstance(hInstance, nCmdShow))
	{
		return FALSE;
	}

	HACCEL hAccelTable;
	hAccelTable = LoadAccelerators(hInstance, MAKEINTRESOURCE(IDC_SCREENREFRESH));

	// ����Ϣѭ��:
	while (GetMessage(&msg, NULL, 0, 0))
	{
		if (!TranslateAccelerator(msg.hwnd, hAccelTable, &msg))
		{
			TranslateMessage(&msg);
			DispatchMessage(&msg);
		}
	}

	return (int)msg.wParam;
}

//
//  ����: MyRegisterClass()
//
//  Ŀ��: ע�ᴰ���ࡣ
//
//  ע��:
//
ATOM MyRegisterClass(HINSTANCE hInstance, LPTSTR szWindowClass)
{
	WNDCLASS wc;

	wc.style = CS_HREDRAW | CS_VREDRAW;
	wc.lpfnWndProc = WndProc;
	wc.cbClsExtra = 0;
	wc.cbWndExtra = 0;
	wc.hInstance = hInstance;
	wc.hIcon = LoadIcon(hInstance, MAKEINTRESOURCE(IDI_SCREENREFRESH));
	wc.hCursor = 0;
	wc.hbrBackground = (HBRUSH)GetStockObject(WHITE_BRUSH);
	wc.lpszMenuName = 0;
	wc.lpszClassName = szWindowClass;

	return RegisterClass(&wc);
}

//
//   ����: InitInstance(HINSTANCE, int)
//
//   Ŀ��: ����ʵ�����������������
//
//   ע��:
//
//        �ڴ˺����У�������ȫ�ֱ����б���ʵ�������
//        ��������ʾ�����򴰿ڡ�
//
BOOL InitInstance(HINSTANCE hInstance, int nCmdShow)
{
	HWND hWnd;
	TCHAR szTitle[MAX_LOADSTRING];		 // �������ı�
	TCHAR szWindowClass[MAX_LOADSTRING]; // ����������

	g_hInst = hInstance; // ��ʵ������洢��ȫ�ֱ�����

	LoadString(hInstance, IDS_APP_TITLE, szTitle, MAX_LOADSTRING);
	LoadString(hInstance, IDC_SCREENREFRESH, szWindowClass, MAX_LOADSTRING);

	if (!MyRegisterClass(hInstance, szWindowClass))
	{
		return FALSE;
	}

	// ��ȡ��Ļ�ߴ�
	int screenWidth = GetSystemMetrics(SM_CXSCREEN);
	int screenHeight = GetSystemMetrics(SM_CYSCREEN);

	// ����ȫ������
	hWnd = CreateWindow(szWindowClass, L"ScreenRefresh", WS_POPUP | WS_VISIBLE,
						0, 0, screenWidth, screenHeight, NULL, NULL, hInstance, NULL);

	if (!hWnd)
	{
		return FALSE;
	}

	ShowWindow(hWnd, nCmdShow);
	UpdateWindow(hWnd);

	if (g_hWndCommandBar)
	{
		CommandBar_Show(g_hWndCommandBar, TRUE);
	}

	return TRUE;
}

//
//  ����: WndProc(HWND, UINT, WPARAM, LPARAM)
//
//  Ŀ��: ���������ڵ���Ϣ��
//
//  WM_COMMAND	- ����Ӧ�ó���˵�
//  WM_PAINT	- ����������
//  WM_DESTROY	- �����˳���Ϣ������
//
//
LRESULT CALLBACK WndProc(HWND hWnd, UINT msg, WPARAM wParam, LPARAM lParam)
{
	static int s_nFlashCount = 0;
	static bool s_bIsBlack = false;
	static UINT_PTR s_nTimerId = 0;

	HWND taskbarWnd = FindWindow(L"HHTaskBar", NULL);

	switch (msg)
	{
	case WM_CREATE:
		// ������ʱ��
		s_nTimerId = SetTimer(hWnd, 1, 1000, NULL);
		if (taskbarWnd != NULL)
		{
			// ����������
			ShowWindow(taskbarWnd, SW_HIDE);
		}
		break;

	case WM_TIMER:
		// ����ʱ����Ϣ
		s_bIsBlack = !s_bIsBlack;
		InvalidateRect(hWnd, NULL, TRUE);

		if (++s_nFlashCount >= 3)
		{
			// ֹͣ��ʱ��
			KillTimer(hWnd, s_nTimerId);
			s_nTimerId = 0;

			// �رմ���
			PostMessage(hWnd, WM_CLOSE, 0, 0);
		}
		break;

	case WM_PAINT:
	{
		PAINTSTRUCT ps;
		HDC hdc = BeginPaint(hWnd, &ps);

		// ���ݱ�����ɫ���ƾ���
		RECT rect;
		GetClientRect(hWnd, &rect);
		HBRUSH hBrush = CreateSolidBrush(s_bIsBlack ? RGB(0, 0, 0) : RGB(255, 255, 255));
		FillRect(hdc, &rect, hBrush);
		DeleteObject(hBrush);

		EndPaint(hWnd, &ps);
		break;
	}

	case WM_CLOSE:
		if (taskbarWnd != NULL)
		{
			// ��ʾ������
			ShowWindow(taskbarWnd, SW_SHOW);
		}
		// ���ٴ���
		DestroyWindow(hWnd);
		break;

	case WM_DESTROY:
		// �˳�Ӧ�ó���
		PostQuitMessage(0);
		break;

	default:
		return DefWindowProc(hWnd, msg, wParam, lParam);
	}

	return 0;
}