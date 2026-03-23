// windowsx.h — compat shim for Windows Message Crackers and helpers
// Provides the macro helpers from the real windowsx.h that SAGE code uses.
//
// This project is not affiliated with or endorsed by Electronic Arts.
// Command & Conquer is a trademark of Electronic Arts.
// You must own the original game to use this software.
#pragma once
#ifndef ZH_COMPAT_WINDOWSX_H
#define ZH_COMPAT_WINDOWSX_H

#ifndef _WIN32

#ifndef ZH_COMPAT_WINDOWS_H
#include "windows.h"
#endif

// ---------------------------------------------------------------------------
//  Message parameter extraction macros
// ---------------------------------------------------------------------------
#ifndef GET_WM_COMMAND_ID
#define GET_WM_COMMAND_ID(wp, lp)     ((int)(LOWORD(wp)))
#define GET_WM_COMMAND_CMD(wp, lp)    ((UINT)HIWORD(wp))
#define GET_WM_COMMAND_HWND(wp, lp)   ((HWND)(lp))
#endif

#ifndef GET_X_LPARAM
#define GET_X_LPARAM(lp)   ((int)(short)LOWORD(lp))
#define GET_Y_LPARAM(lp)   ((int)(short)HIWORD(lp))
#endif

// ---------------------------------------------------------------------------
//  Control helper macros (SendMessage wrappers)
// ---------------------------------------------------------------------------
// Edit control
#define Edit_GetText(h,b,c)         GetWindowText(h,b,c)
#define Edit_SetText(h,t)           SetWindowText(h,t)
#define Edit_GetTextLength(h)       GetWindowTextLength(h)
#define Edit_SetSel(h,s,e)         ((void)SendMessage(h,EM_SETSEL,(WPARAM)(s),(LPARAM)(e)))
#define Edit_LimitText(h,m)        ((void)SendMessage(h,EM_SETLIMITTEXT,(WPARAM)(m),0L))

// Static
#define Static_SetText(h,t)         SetWindowText(h,t)
#define Static_GetText(h,b,c)       GetWindowText(h,b,c)
#define Static_SetIcon(h,i)        ((HICON)SendMessage(h,STM_SETICON,(WPARAM)(HICON)(i),0L))

// Button
#define Button_GetCheck(h)         ((int)SendMessage(h,BM_GETCHECK,0L,0L))
#define Button_SetCheck(h,s)       ((void)SendMessage(h,BM_SETCHECK,(WPARAM)(int)(s),0L))
#define Button_Enable(h,f)         EnableWindow(h,f)

// ListBox
#define ListBox_GetCount(h)        ((int)SendMessage(h,LB_GETCOUNT,0L,0L))
#define ListBox_AddString(h,s)     ((int)SendMessage(h,LB_ADDSTRING,0L,(LPARAM)(LPCTSTR)(s)))
#define ListBox_DeleteString(h,i)  ((int)SendMessage(h,LB_DELETESTRING,(WPARAM)(int)(i),0L))
#define ListBox_GetCurSel(h)       ((int)SendMessage(h,LB_GETCURSEL,0L,0L))
#define ListBox_SetCurSel(h,i)     ((int)SendMessage(h,LB_SETCURSEL,(WPARAM)(int)(i),0L))
#define ListBox_GetText(h,i,b)     ((int)SendMessage(h,LB_GETTEXT,(WPARAM)(int)(i),(LPARAM)(LPCTSTR)(b)))
#define ListBox_GetTextLen(h,i)    ((int)SendMessage(h,LB_GETTEXTLEN,(WPARAM)(int)(i),0L))
#define ListBox_ResetContent(h)    ((void)SendMessage(h,LB_RESETCONTENT,0L,0L))
#define ListBox_SetItemData(h,i,d) ((int)SendMessage(h,LB_SETITEMDATA,(WPARAM)(int)(i),(LPARAM)(d)))
#define ListBox_GetItemData(h,i)   ((LRESULT)SendMessage(h,LB_GETITEMDATA,(WPARAM)(int)(i),0L))

// ComboBox
#define ComboBox_GetCount(h)       ((int)SendMessage(h,CB_GETCOUNT,0L,0L))
#define ComboBox_AddString(h,s)    ((int)SendMessage(h,CB_ADDSTRING,0L,(LPARAM)(LPCTSTR)(s)))
#define ComboBox_DeleteString(h,i) ((int)SendMessage(h,CB_DELETESTRING,(WPARAM)(int)(i),0L))
#define ComboBox_GetCurSel(h)      ((int)SendMessage(h,CB_GETCURSEL,0L,0L))
#define ComboBox_SetCurSel(h,i)    ((int)SendMessage(h,CB_SETCURSEL,(WPARAM)(int)(i),0L))
#define ComboBox_GetText(h,b,c)    GetWindowText(h,b,c)
#define ComboBox_SetText(h,t)      SetWindowText(h,t)
#define ComboBox_ResetContent(h)   ((void)SendMessage(h,CB_RESETCONTENT,0L,0L))
#define ComboBox_SetItemData(h,i,d)((int)SendMessage(h,CB_SETITEMDATA,(WPARAM)(int)(i),(LPARAM)(d)))
#define ComboBox_GetItemData(h,i)  ((LRESULT)SendMessage(h,CB_GETITEMDATA,(WPARAM)(int)(i),0L))

// Window helpers
#define GetWindowID(h)             GetDlgCtrlID(h)
#define SubclassWindow(h,p)        ((WNDPROC)SetWindowLongPtr(h,GWLP_WNDPROC,(LONG_PTR)(WNDPROC)(p)))

#endif // !_WIN32
#endif // ZH_COMPAT_WINDOWSX_H
