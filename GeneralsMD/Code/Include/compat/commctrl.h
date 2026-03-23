// commctrl.h — compat shim for Windows Common Controls
// These are only used by editor/tool code that won't run on Linux/macOS.
// Provide minimal type stubs so the headers compile.
#pragma once
#ifndef ZH_COMPAT_COMMCTRL_H
#define ZH_COMPAT_COMMCTRL_H

#ifndef _WIN32

#ifndef ZH_COMPAT_WINDOWS_H
#include "windows.h"
#endif

// Common control window class names (just strings)
#define TOOLBARCLASSNAMEA    "ToolbarWindow32"
#define STATUSCLASSNAMEA     "msctls_statusbar32"
#define TRACKBAR_CLASSA      "msctls_trackbar32"
#define UPDOWN_CLASSA        "msctls_updown32"
#define PROGRESS_CLASSA      "msctls_progress32"
#define HOTKEY_CLASSA        "msctls_hotkey32"
#define TREEVIEW_CLASSA      "SysTreeView32"
#define LISTVIEW_CLASSA      "SysListView32"
#define TABCONTROL_CLASSA    "SysTabControl32"
#define ANIMATE_CLASSA       "SysAnimate32"
#define TOOLBARCLASSNAME     TOOLBARCLASSNAMEA
#define STATUSCLASSNAME      STATUSCLASSNAMEA

// Common init flags
#define ICC_WIN95_CLASSES    0x000000FF
#define ICC_BAR_CLASSES      0x00000004
#define ICC_TREEVIEW_CLASSES 0x00000002
#define ICC_LISTVIEW_CLASSES 0x00000001

typedef struct tagINITCOMMONCONTROLSEX {
    DWORD dwSize;
    DWORD dwICC;
} INITCOMMONCONTROLSEX, *LPINITCOMMONCONTROLSEX;

static inline BOOL InitCommonControlsEx(const LPINITCOMMONCONTROLSEX lpInitCtrls) {
    (void)lpInitCtrls; return FALSE;
}
static inline void InitCommonControls(void) {}

// NMHDR — notification message header
typedef struct tagNMHDR {
    HWND  hwndFrom;
    UINT_PTR idFrom;
    UINT  code;
} NMHDR, *LPNMHDR;

// LV_ ListView definitions (minimal)
#define LVS_REPORT        0x0001
#define LVS_LIST          0x0003
#define LVS_ICON          0x0000
#define LVS_SMALLICON     0x0002
#define LVM_FIRST         0x1000
#define LVM_INSERTCOLUMNA (LVM_FIRST+27)
#define LVM_INSERTITEMA   (LVM_FIRST+7)
#define LVM_DELETEALLITEMS (LVM_FIRST+9)
#define LVCF_FMT          0x0001
#define LVCF_WIDTH        0x0002
#define LVCF_TEXT         0x0004
#define LVCF_SUBITEM      0x0008
#define LVCFMT_LEFT       0x0000
#define LVIF_TEXT         0x0001
#define LVIF_IMAGE        0x0002
#define LVIF_STATE        0x0008
#define LVIS_SELECTED     0x0002
#define LVIS_FOCUSED      0x0001

typedef struct tagLVCOLUMNA {
    UINT  mask;
    int   fmt;
    int   cx;
    LPSTR pszText;
    int   cchTextMax;
    int   iSubItem;
} LVCOLUMNA, *LPLVCOLUMNA;
#define LVCOLUMN  LVCOLUMNA
#define LPLVCOLUMN LPLVCOLUMNA

typedef struct tagLVITEMA {
    UINT   mask;
    int    iItem;
    int    iSubItem;
    UINT   state;
    UINT   stateMask;
    LPSTR  pszText;
    int    cchTextMax;
    int    iImage;
    LPARAM lParam;
} LVITEMA, *LPLVITEMA;
#define LVITEM  LVITEMA
#define LPLVITEM LPLVITEMA

// TV_ TreeView definitions (minimal)
#define TVS_HASLINES      0x0001
#define TVS_LINESATROOT   0x0002
#define TVS_HASBUTTONS    0x0004
#define TVS_SHOWSELALWAYS 0x0020
#define TVIS_SELECTED     0x0002
#define TVIF_TEXT         0x0001
#define TVIF_IMAGE        0x0002
#define TVIF_SELECTEDIMAGE 0x0020
#define TVIF_CHILDREN     0x0040
#define TVIF_HANDLE       0x0010
#define TVI_ROOT          ((HTREEITEM)(LONG_PTR)0xFFFF0000)
#define TVI_LAST          ((HTREEITEM)(LONG_PTR)0xFFFF0001)
#define TVI_SORT          ((HTREEITEM)(LONG_PTR)0xFFFF0002)
#define TVM_INSERTITEMA   (0x1100+0)
#define TVM_DELETEITEM    (0x1100+1)
#define TVM_DELETEALLITEMS (0x1100+9)
#define TVM_GETITEMA      (0x1100+12)
#define TVM_SETITEMA      (0x1100+13)
#define TVM_GETNEXTITEM   (0x1100+10)
#define TVGN_ROOT         0x0000
#define TVGN_NEXT         0x0001
#define TVGN_CHILD        0x0004
#define TVGN_FIRSTVISIBLE 0x0005

typedef void* HTREEITEM;

typedef struct tagTVITEMA {
    UINT      mask;
    HTREEITEM hItem;
    UINT      state;
    UINT      stateMask;
    LPSTR     pszText;
    int       cchTextMax;
    int       iImage;
    int       iSelectedImage;
    int       cChildren;
    LPARAM    lParam;
} TVITEMA, *LPTVITEMA;
#define TVITEM TVITEMA
#define LPTVITEM LPTVITEMA

typedef struct tagTVINSERTSTRUCTA {
    HTREEITEM hParent;
    HTREEITEM hInsertAfter;
    TVITEMA   item;
} TVINSERTSTRUCTA, *LPTVINSERTSTRUCTA;
#define TVINSERTSTRUCT  TVINSERTSTRUCTA
#define LPTVINSERTSTRUCT LPTVINSERTSTRUCTA

#endif // !_WIN32
#endif // ZH_COMPAT_COMMCTRL_H
