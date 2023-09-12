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
// ScreenRotating.cpp : 定义应用程序的入口点。

#include "stdafx.h"
#include "ScreenRotating.h"
#include <windows.h>

#define MAX_LOADSTRING 100

// 全局变量:
HINSTANCE g_hInst;	   // 当前实例
HWND g_hWndCommandBar; // 命令栏句柄

// 此代码模块中包含的函数的前向声明:
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

	// 执行应用程序初始化:
	if (!InitInstance(hInstance, nCmdShow))
	{
		return FALSE;
	}

	HACCEL hAccelTable;
	hAccelTable = LoadAccelerators(hInstance, MAKEINTRESOURCE(IDC_SCREENROTATING));

	// 主消息循环:
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
//  函数: MyRegisterClass()
//
//  目的: 注册窗口类。
//
//  注释:
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
//   函数: InitInstance(HINSTANCE, int)
//
//   目的: 保存实例句柄并创建主窗口
//
//   注释:
//
//        在此函数中，我们在全局变量中保存实例句柄并
//        创建和显示主程序窗口。
//
BOOL InitInstance(HINSTANCE hInstance, int nCmdShow)
{
	HWND hWnd;
	TCHAR szTitle[MAX_LOADSTRING];		 // 标题栏文本
	TCHAR szWindowClass[MAX_LOADSTRING]; // 主窗口类名

	g_hInst = hInstance; // 将实例句柄存储在全局变量中

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
//  函数: WndProc(HWND, UINT, WPARAM, LPARAM)
//
//  目的: 处理主窗口的消息。
//
//  WM_COMMAND	- 处理应用程序菜单
//  WM_PAINT	- 绘制主窗口
//  WM_DESTROY	- 发送退出消息并返回
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

		// TODO: 在此添加任意绘图代码...

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

	// 寻找最后一个路径分隔符的位置
	LPTSTR lastSlash = _tcsrchr(buffer, _T('\\'));
	if (lastSlash != NULL)
	{
		// 将路径分隔符后的部分截断，只保留目录路径
		*(lastSlash + 1) = _T('\0');
	}

	path = buffer;
	return true;
}

bool OpenShellProgram(HWND hDlg, LPCTSTR programPath, LPCTSTR argument)
{
	// 创建进程的信息结构体
	PROCESS_INFORMATION processInfo;
	ZeroMemory(&processInfo, sizeof(PROCESS_INFORMATION));

	// 创建进程的启动信息结构体
	STARTUPINFO startupInfo;
	ZeroMemory(&startupInfo, sizeof(STARTUPINFO));
	startupInfo.cb = sizeof(STARTUPINFO);

	// 使用 CreateProcess 打开程序
	if (!CreateProcess(programPath, argument != NULL ? const_cast<LPTSTR>(argument) : NULL, NULL, NULL, FALSE, 0, NULL, NULL, &startupInfo, &processInfo))
	{
		// 打开程序失败
		MessageBox(hDlg, L"请确认当前程序目录下的扩展支持程序 Rotate0.exe、Rotate90.exe、Rotate180.exe、Rotate270.exe 以及 ScreenRefresh.exe 是否存在。", L"错误", MB_ICONERROR | MB_OK);
		return false;
	}

	// 关闭进程和线程的句柄
	CloseHandle(processInfo.hProcess);
	CloseHandle(processInfo.hThread);

	// 成功打开程序
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

// “主界面”框的消息处理程序。
INT_PTR CALLBACK Main(HWND hDlg, UINT message, WPARAM wParam, LPARAM lParam)
{
	switch (message)
	{
	case WM_INITDIALOG:
	{
		RECT rectChild, rectParent;
		int DlgWidth, DlgHeight; // 以像素为单位的对话框宽度和高度
		int NewPosX, NewPosY;

		// 设法使“主界面”对话框居中显示
		if (GetWindowRect(hDlg, &rectChild))
		{
			GetClientRect(GetParent(hDlg), &rectParent);
			DlgWidth = rectChild.right - rectChild.left;
			DlgHeight = rectChild.bottom - rectChild.top;
			NewPosX = (rectParent.right - rectParent.left - DlgWidth) / 2;
			NewPosY = (rectParent.bottom - rectParent.top - DlgHeight) / 2;

			// 如果“主界面”框比实际屏幕大
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