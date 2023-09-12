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
// ScreenRotating.cpp : ����Ӧ�ó������ڵ㡣

#include "stdafx.h"
#include "ScreenRotating.h"
#include <windows.h>

#define MAX_LOADSTRING 100

// ȫ�ֱ���:
HINSTANCE g_hInst;	   // ��ǰʵ��
HWND g_hWndCommandBar; // ���������

// �˴���ģ���а����ĺ�����ǰ������:
ATOM MyRegisterClass(HINSTANCE, LPTSTR);
BOOL InitInstance(HINSTANCE, int);
LRESULT CALLBACK WndProc(HWND, UINT, WPARAM, LPARAM);
INT_PTR CALLBACK Main(HWND, UINT, WPARAM, LPARAM);

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
	hAccelTable = LoadAccelerators(hInstance, MAKEINTRESOURCE(IDC_SCREENROTATING));

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
	wc.hIcon = LoadIcon(hInstance, MAKEINTRESOURCE(IDI_SCREENROTATING));
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
	LoadString(hInstance, IDC_SCREENROTATING, szWindowClass, MAX_LOADSTRING);

	if (!MyRegisterClass(hInstance, szWindowClass))
	{
		return FALSE;
	}

	hWnd = CreateWindow(szWindowClass, szTitle, WS_OVERLAPPED,
						CW_USEDEFAULT, CW_USEDEFAULT, CW_USEDEFAULT, CW_USEDEFAULT, NULL, NULL, hInstance, NULL);

	if (!hWnd)
	{
		return FALSE;
	}

	DialogBox(g_hInst, (LPCTSTR)IDD_MAINBOX, hWnd, Main);

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
LRESULT CALLBACK WndProc(HWND hWnd, UINT message, WPARAM wParam, LPARAM lParam)
{
	PAINTSTRUCT ps;
	HDC hdc;

	switch (message)
	{
	case WM_COMMAND:
		break;
	case WM_CREATE:
		g_hWndCommandBar = CommandBar_Create(g_hInst, hWnd, 1);
		CommandBar_AddAdornments(g_hWndCommandBar, 0, 0);
		break;
	case WM_PAINT:
		hdc = BeginPaint(hWnd, &ps);

		// TODO: �ڴ���������ͼ����...

		EndPaint(hWnd, &ps);
		break;
	case WM_DESTROY:
		CommandBar_Destroy(g_hWndCommandBar);
		PostQuitMessage(0);
		break;

	default:
		return DefWindowProc(hWnd, message, wParam, lParam);
	}
	return 0;
}

bool GetSelfExecutablePath(LPCTSTR &path)
{
	static TCHAR buffer[MAX_PATH];
	DWORD result = GetModuleFileName(NULL, buffer, MAX_PATH);
	if (result == 0)
	{
		_tprintf(_T("Failed to get self executable path. Error: %d\n"), GetLastError());
		return false;
	}

	// Ѱ�����һ��·���ָ�����λ��
	LPTSTR lastSlash = _tcsrchr(buffer, _T('\\'));
	if (lastSlash != NULL)
	{
		// ��·���ָ�����Ĳ��ֽضϣ�ֻ����Ŀ¼·��
		*(lastSlash + 1) = _T('\0');
	}

	path = buffer;
	return true;
}

bool OpenShellProgram(HWND hDlg, LPCTSTR programPath, LPCTSTR argument)
{
	// �������̵���Ϣ�ṹ��
	PROCESS_INFORMATION processInfo;
	ZeroMemory(&processInfo, sizeof(PROCESS_INFORMATION));

	// �������̵�������Ϣ�ṹ��
	STARTUPINFO startupInfo;
	ZeroMemory(&startupInfo, sizeof(STARTUPINFO));
	startupInfo.cb = sizeof(STARTUPINFO);

	// ʹ�� CreateProcess �򿪳���
	if (!CreateProcess(programPath, argument != NULL ? const_cast<LPTSTR>(argument) : NULL, NULL, NULL, FALSE, 0, NULL, NULL, &startupInfo, &processInfo))
	{
		// �򿪳���ʧ��
		MessageBox(hDlg, L"��ȷ�ϵ�ǰ����Ŀ¼�µ���չ֧�ֳ��� Rotate0.exe��Rotate90.exe��Rotate180.exe��Rotate270.exe �Լ� ScreenRefresh.exe �Ƿ���ڡ�", L"����", MB_ICONERROR | MB_OK);
		return false;
	}

	// �رս��̺��̵߳ľ��
	CloseHandle(processInfo.hProcess);
	CloseHandle(processInfo.hThread);

	// �ɹ��򿪳���
	return true;
}

void RotateScreen(HWND hDlg, DWORD dwRotationAngle)
{
	LPCTSTR selfPath;
	if (!GetSelfExecutablePath(selfPath))
	{
		return;
	}

	bool isOk = false;

	switch (dwRotationAngle)
	{
	case DMDO_0:
		TCHAR appendedString0[MAX_PATH];
		lstrcpy(appendedString0, selfPath);
		lstrcat(appendedString0, _T("Rotate0.exe"));
		isOk = OpenShellProgram(hDlg, appendedString0, NULL);
		break;

	case DMDO_90:
		TCHAR appendedString1[MAX_PATH];
		lstrcpy(appendedString1, selfPath);
		lstrcat(appendedString1, _T("Rotate90.exe"));
		isOk = OpenShellProgram(hDlg, appendedString1, NULL);
		break;

	case DMDO_180:
		TCHAR appendedString2[MAX_PATH];
		lstrcpy(appendedString2, selfPath);
		lstrcat(appendedString2, _T("Rotate180.exe"));
		isOk = OpenShellProgram(hDlg, appendedString2, NULL);
		break;

	case DMDO_270:
		TCHAR appendedString3[MAX_PATH];
		lstrcpy(appendedString3, selfPath);
		lstrcat(appendedString3, _T("Rotate270.exe"));
		isOk = OpenShellProgram(hDlg, appendedString3, NULL);
		break;
	}
	if (!isOk)
	{
		return;
	}
	TCHAR appendedString4[MAX_PATH];
	lstrcpy(appendedString4, selfPath);
	lstrcat(appendedString4, _T("ScreenRefresh.exe"));
	OpenShellProgram(hDlg, appendedString4, NULL);
}

// �������桱�����Ϣ�������
INT_PTR CALLBACK Main(HWND hDlg, UINT message, WPARAM wParam, LPARAM lParam)
{
	switch (message)
	{
	case WM_INITDIALOG:
	{
		RECT rectChild, rectParent;
		int DlgWidth, DlgHeight; // ������Ϊ��λ�ĶԻ����Ⱥ͸߶�
		int NewPosX, NewPosY;

		// �跨ʹ�������桱�Ի��������ʾ
		if (GetWindowRect(hDlg, &rectChild))
		{
			GetClientRect(GetParent(hDlg), &rectParent);
			DlgWidth = rectChild.right - rectChild.left;
			DlgHeight = rectChild.bottom - rectChild.top;
			NewPosX = (rectParent.right - rectParent.left - DlgWidth) / 2;
			NewPosY = (rectParent.bottom - rectParent.top - DlgHeight) / 2;

			// ����������桱���ʵ����Ļ��
			if (NewPosX < 0)
				NewPosX = 0;
			if (NewPosY < 0)
				NewPosY = 0;
			SetWindowPos(hDlg, 0, NewPosX, NewPosY,
						 0, 0, SWP_NOZORDER | SWP_NOSIZE);
		}
		return (INT_PTR)TRUE;
	}

	case WM_COMMAND:
		if ((LOWORD(wParam) == IDOK) || (LOWORD(wParam) == IDCANCEL))
		{
			EndDialog(hDlg, LOWORD(wParam));
			exit(0);
			return TRUE;
		}
		if (LOWORD(wParam) == IDC_BUTTON_A && HIWORD(wParam) == BN_CLICKED)
		{
			DWORD rotationAngle = DMDO_0;
			RotateScreen(hDlg, rotationAngle);
		}
		if (LOWORD(wParam) == IDC_BUTTON_B && HIWORD(wParam) == BN_CLICKED)
		{
			DWORD rotationAngle = DMDO_90;
			RotateScreen(hDlg, rotationAngle);
		}
		if (LOWORD(wParam) == IDC_BUTTON_C && HIWORD(wParam) == BN_CLICKED)
		{
			DWORD rotationAngle = DMDO_180;
			RotateScreen(hDlg, rotationAngle);
		}
		if (LOWORD(wParam) == IDC_BUTTON_D && HIWORD(wParam) == BN_CLICKED)
		{
			DWORD rotationAngle = DMDO_270;
			RotateScreen(hDlg, rotationAngle);
		}
		break;

	case WM_CLOSE:
		EndDialog(hDlg, message);
		exit(0);
		return TRUE;
	}
	return (INT_PTR)FALSE;
}