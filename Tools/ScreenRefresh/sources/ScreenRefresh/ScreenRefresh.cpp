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
// ScreenRefresh.cpp : 定义应用程序的入口点。

#include "stdafx.h"
#include "ScreenRefresh.h"

#define MAX_LOADSTRING 100

// 全局变量:
HINSTANCE g_hInst;	   // 当前实例
HWND g_hWndCommandBar; // 命令栏句柄

// 此代码模块中包含的函数的前向声明:
ATOM MyRegisterClass(HINSTANCE, LPTSTR);
BOOL InitInstance(HINSTANCE, int);
LRESULT CALLBACK WndProc(HWND, UINT, WPARAM, LPARAM);

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
	hAccelTable = LoadAccelerators(hInstance, MAKEINTRESOURCE(IDC_SCREENREFRESH));

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
	wc.hIcon = LoadIcon(hInstance, MAKEINTRESOURCE(IDI_SCREENREFRESH));
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
	LoadString(hInstance, IDC_SCREENREFRESH, szWindowClass, MAX_LOADSTRING);

	if (!MyRegisterClass(hInstance, szWindowClass))
	{
		return FALSE;
	}

	// 获取屏幕尺寸
	int screenWidth = GetSystemMetrics(SM_CXSCREEN);
	int screenHeight = GetSystemMetrics(SM_CYSCREEN);

	// 创建全屏窗口
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
//  函数: WndProc(HWND, UINT, WPARAM, LPARAM)
//
//  目的: 处理主窗口的消息。
//
//  WM_COMMAND	- 处理应用程序菜单
//  WM_PAINT	- 绘制主窗口
//  WM_DESTROY	- 发送退出消息并返回
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
		// 创建定时器
		s_nTimerId = SetTimer(hWnd, 1, 1000, NULL);
		if (taskbarWnd != NULL)
		{
			// 隐藏任务栏
			ShowWindow(taskbarWnd, SW_HIDE);
		}
		break;

	case WM_TIMER:
		// 处理定时器消息
		s_bIsBlack = !s_bIsBlack;
		InvalidateRect(hWnd, NULL, TRUE);

		if (++s_nFlashCount >= 3)
		{
			// 停止定时器
			KillTimer(hWnd, s_nTimerId);
			s_nTimerId = 0;

			// 关闭窗口
			PostMessage(hWnd, WM_CLOSE, 0, 0);
		}
		break;

	case WM_PAINT:
	{
		PAINTSTRUCT ps;
		HDC hdc = BeginPaint(hWnd, &ps);

		// 根据背景颜色绘制矩形
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
			// 显示任务栏
			ShowWindow(taskbarWnd, SW_SHOW);
		}
		// 销毁窗口
		DestroyWindow(hWnd);
		break;

	case WM_DESTROY:
		// 退出应用程序
		PostQuitMessage(0);
		break;

	default:
		return DefWindowProc(hWnd, msg, wParam, lParam);
	}

	return 0;
}