#include "Class_GroupBox_ex.h"

void _groupbox_regsiter()
{
    Ex_ObjRegister(L"GroupBox", EOS_VISIBLE, EOS_EX_FOCUSABLE, DT_LEFT | DT_TOP | DT_SINGLELINE, 3 * sizeof(size_t), 0, 0, _groupbox_proc);
}

void _groupbox_paint(HEXOBJ hObj, obj_s *pObj)
{
    EX_PAINTSTRUCT ps{0};
    if (Ex_ObjBeginPaint(hObj, &ps))
    {
        _canvas_clear(ps.hCanvas, 0);
        LPCWSTR lpText = pObj->pstrTitle_;
        RECT rcText{};
        FLOAT radius = Ex_ObjGetLong(hObj, GROUPBOX_RADIUS);
        FLOAT textoffset = Ex_ObjGetLong(hObj, GROUPBOX_TEXT_OFFSET);
        if (textoffset == 0)
            textoffset = 10;

        FLOAT strokewidth = Ex_ObjGetLong(hObj, GROUPBOX_STROKEWIDTH);
        if (strokewidth == 0)
            strokewidth = 2;

        HEXBRUSH hBrush = _brush_create(0);
        if (lpText)
        {
            FLOAT retWidth, retHeight;
            _canvas_calctextsize(ps.hCanvas, pObj->hFont_, lpText, -1, ps.dwTextFormat, 0, ps.uWidth, ps.uHeight, &retWidth, &retHeight);
            rcText.right = retWidth;
            rcText.bottom = retHeight;
            _canvas_drawtext(ps.hCanvas, pObj->hFont_, _obj_getcolor(pObj, COLOR_EX_TEXT_NORMAL), lpText, -1, ps.dwTextFormat, ps.rcText.left + radius + textoffset, ps.rcText.top, ps.rcText.right, ps.rcText.bottom);
        }

        _brush_setcolor(hBrush, _obj_getcolor(pObj, COLOR_EX_BORDER));
        HEXPATH hPath = NULL;
        _path_create(EPF_DISABLESCALE, &hPath);
        _path_open(hPath);

        ps.rcPaint.right -= strokewidth;
        ps.rcPaint.bottom -= strokewidth;
        ps.rcPaint.top += strokewidth;
        ps.rcPaint.left += strokewidth;

        _path_beginfigure2(hPath, static_cast<FLOAT>(ps.rcPaint.left + textoffset + rcText.right + 5 + radius), static_cast<FLOAT>(ps.rcPaint.top + rcText.bottom / 2));
        _path_addarc(hPath, static_cast<FLOAT>(ps.rcPaint.right - radius), static_cast<FLOAT>(ps.rcPaint.top + rcText.bottom / 2), static_cast<FLOAT>(ps.rcPaint.right), static_cast<FLOAT>(ps.rcPaint.top + rcText.bottom / 2 + radius), radius, radius, TRUE);
        _path_addarc(hPath, static_cast<FLOAT>(ps.rcPaint.right), static_cast<FLOAT>(ps.rcPaint.bottom - radius), static_cast<FLOAT>(ps.rcPaint.right - radius), static_cast<FLOAT>(ps.rcPaint.bottom), radius, radius, TRUE);
        _path_addarc(hPath, static_cast<FLOAT>(ps.rcPaint.left + radius), static_cast<FLOAT>(ps.rcPaint.bottom), static_cast<FLOAT>(ps.rcPaint.left), static_cast<FLOAT>(ps.rcPaint.bottom - radius), radius, radius, TRUE);
        _path_addarc(hPath, static_cast<FLOAT>(ps.rcPaint.left), static_cast<FLOAT>(ps.rcPaint.top + rcText.bottom / 2 + radius), static_cast<FLOAT>(ps.rcPaint.left + radius), static_cast<FLOAT>(ps.rcPaint.top + rcText.bottom / 2), radius, radius, TRUE);
        _path_addline(hPath, static_cast<FLOAT>(ps.rcPaint.left + radius), static_cast<FLOAT>(ps.rcPaint.top + rcText.bottom / 2), static_cast<FLOAT>(ps.rcText.left + radius + textoffset - 5), static_cast<FLOAT>(ps.rcPaint.top + rcText.bottom / 2));

        _path_endfigure(hPath, FALSE);
        _path_close(hPath);

        _canvas_drawpath(ps.hCanvas, hPath, hBrush, strokewidth, D2D1_DASH_STYLE::D2D1_DASH_STYLE_SOLID);
        _path_destroy(hPath);
        _brush_destroy(hBrush);
        Ex_ObjEndPaint(hObj, &ps);
    }
}

LRESULT CALLBACK _groupbox_proc(HWND hWnd, HEXOBJ hObj, INT uMsg, WPARAM wParam, LPARAM lParam)
{
    INT nError = 0;
    obj_s *pObj = nullptr;
    if (_handle_validate(hObj, HT_OBJECT, (LPVOID *)&pObj, &nError))
    {
        if (uMsg == WM_PAINT)
        {
            _groupbox_paint(hObj, pObj);
        }
    }
    return Ex_ObjDefProc(hWnd, hObj, uMsg, wParam, lParam);
}