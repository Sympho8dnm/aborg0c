#include "Class_EditEx_ex.h"

ClsPROC m_pfnEditExProc;/*控件基类的消息回调函数*/

void _editex_register()
{
	EX_CLASSINFO	pClsInfoEdit;

	/* 超类化(从现有控件派生)过程
	 * 超类化的好处是可以直接利用现有控件，省去从头编写控件的时间，提高扩展效率*/

	 /* 1、获取父类控件信息*/
	WCHAR	oldwzCls[] = L"Edit";
	Ex_ObjGetClassInfoEx(oldwzCls, &pClsInfoEdit);

	/* 2、保存父类控件回调函数指针*/
	m_pfnEditExProc = pClsInfoEdit.pfnClsProc;

	/* 3、注册新控件*/
	WCHAR	newwzCls[] = L"EditEx";
	Ex_ObjRegister(newwzCls, pClsInfoEdit.dwStyle, EOS_EX_COMPOSITED | EOS_EX_TABSTOP | EOS_EX_CUSTOMDRAW | EOS_EX_FOCUSABLE/*pClsInfoEdit.dwStyleEx*/, pClsInfoEdit.dwTextFormat, NULL, pClsInfoEdit.hCursor, pClsInfoEdit.dwFlags, _editex_proc);
}


LRESULT CALLBACK _editex_proc(HWND hWnd, HEXOBJ hObj, INT uMsg, WPARAM wParam, LPARAM lParam)
{
	if (uMsg == WM_CREATE)
	{
		Ex_ObjSetPadding(hObj, 0, 5, 5, 5, 5, FALSE);
		Ex_ObjInitPropList(hObj, 10);
		EXARGB	ThemeColor = ExARGB(76, 175, 80, 255);
		Ex_ObjSetProp(hObj, EEEP_CRBORDERNORMAL, ExARGB(0, 0, 0, 100));
		Ex_ObjSetProp(hObj, EEEP_CRBORDERHOVER, ExARGB(0, 0, 0, 150));
		Ex_ObjSetProp(hObj, EEEP_CRBORDERDOWNORCHECKED, ThemeColor);
		Ex_ObjSetProp(hObj, EEEP_CRICONNORMAL, ExARGB(184, 186, 188, 255));
		Ex_ObjSetProp(hObj, EEEP_CRICONDOWNORFOCUS, ExARGB(18, 183, 245, 255));
		Ex_ObjSetProp(hObj, EEEP_STORKEWIDTH, 2);

	}
	else if (uMsg == WM_SETICON)
	{
		/* 设置图标 */
		HEXIMAGE hImage = (HEXIMAGE)Ex_ObjSetLong(hObj, EOL_USERDATA, lParam);

		/* 若有原位图则销毁 */
		if (hImage != 0)
		{
			_img_destroy(hImage);
		}

		if (wParam != 0)
		{
			/* 重绘控件 */
			Ex_ObjInvalidateRect(hObj, 0);
		}

	}
	else if (uMsg == WM_DESTROY)
	{
		_img_destroy((HEXIMAGE)Ex_ObjGetLong(hObj, EOL_USERDATA));

	}
	else if (uMsg == WM_EX_PROPS)
	{
		EX_OBJ_PROPS* EditExprops = (EX_OBJ_PROPS*)lParam;
		Ex_ObjSetProp(hObj, EEEP_CRBKGNORMAL, EditExprops->COLOR_EX_BACKGROUND_NORMAL);
		Ex_ObjSetProp(hObj, EEEP_CRBORDERNORMAL, EditExprops->COLOR_EX_BORDER_NORMAL);
		Ex_ObjSetProp(hObj, EEEP_CRBORDERHOVER, EditExprops->COLOR_EX_BORDER_HOVER);
		Ex_ObjSetProp(hObj, EEEP_CRBORDERDOWNORCHECKED, EditExprops->COLOR_EX_BORDER_DOWNORCHECKED);
		if (EditExprops->COLOR_EX_ICON_NORMAL == 0) {
			Ex_ObjSetProp(hObj, EEEP_CRICONNORMAL, ExARGB(184, 186, 188, 255));
		}
		else {
			Ex_ObjSetProp(hObj, EEEP_CRICONNORMAL, EditExprops->COLOR_EX_ICON_NORMAL);
		}
		if (EditExprops->COLOR_EX_ICON_DOWNORFOCUS == 0) {
			Ex_ObjSetProp(hObj, EEEP_CRICONDOWNORFOCUS, ExARGB(18, 183, 245, 255));
		}
		else {
			Ex_ObjSetProp(hObj, EEEP_CRICONDOWNORFOCUS, EditExprops->COLOR_EX_ICON_DOWNORFOCUS);
		}
		Ex_ObjSetProp(hObj, EEEP_RADIUS, EditExprops->Radius);
		Ex_ObjSetProp(hObj, EEEP_STORKEWIDTH, EditExprops->StrokeWidth);
		Ex_ObjSetProp(hObj, EEEP_ICONPOSITION, EditExprops->nIconPosition);

	}
	else if (uMsg == WM_ERASEBKGND)
	{
		RECT rc = { 0 };
		HEXCANVAS	hCanvas = (HEXCANVAS)wParam;
		HEXBRUSH	hbrush = _brush_create(Ex_ObjGetProp(hObj, EEEP_CRBKGNORMAL));
		BOOL		m_IsDraw = FALSE;/*假为默认边框风格*/
		FLOAT		Radius = (FLOAT)Ex_ObjGetProp(hObj, EEEP_RADIUS);
		FLOAT		StrokeWidth = (FLOAT)Ex_ObjGetProp(hObj, EEEP_STORKEWIDTH);
		FLOAT		nIconPosition = (FLOAT)Ex_ObjGetProp(hObj, EEEP_ICONPOSITION);
		/*获取编辑客户区矩形*/
		Ex_ObjGetClientRect(hObj, &rc);
		/*填充背景*/
		if (Radius == NULL)
		{
			_canvas_fillrect(hCanvas, hbrush, 0, 0, Ex_Scale((FLOAT)rc.right), Ex_Scale((FLOAT)rc.bottom));
		}
		else {
			_canvas_fillroundedrect(hCanvas, hbrush, 0, 0, Ex_Scale((FLOAT)rc.right), Ex_Scale((FLOAT)rc.bottom), Ex_Scale(Radius), Ex_Scale(Radius));
		}
		/*计算编辑框宽高*/
		FLOAT Width = Ex_Scale((FLOAT)(rc.right - rc.left));
		FLOAT Height = Ex_Scale((FLOAT)(rc.bottom - rc.top));

		/*获取图标*/
		HEXIMAGE    hImage = (HEXIMAGE)Ex_ObjGetLong(hObj, EOL_USERDATA);

		/*定义线框正常态颜色*/
		_brush_setcolor(hbrush, Ex_ObjGetProp(hObj, EEEP_CRBORDERNORMAL));

		if ((Ex_ObjGetLong(hObj, EOL_STYLE) & EES_UNDERLINE) == EES_UNDERLINE)
		{
			m_IsDraw = TRUE;/*下划线风格*/
		}

		if ((Ex_ObjGetUIState(hObj) & STATE_HOVER) != 0)
		{
			/*定义热点状态下的线框颜色*/
			_brush_setcolor(hbrush, Ex_ObjGetProp(hObj, EEEP_CRBORDERHOVER));
		}

		if ((Ex_ObjGetUIState(hObj) & STATE_FOCUS) != 0)
		{

			_brush_setcolor(hbrush, Ex_ObjGetProp(hObj, EEEP_CRBORDERDOWNORCHECKED));
		}

		/*绘制线框*/
		if (m_IsDraw)
		{
			_canvas_drawline(hCanvas, hbrush, Ex_Scale((FLOAT)rc.left), Ex_Scale((FLOAT)rc.bottom), Ex_Scale((FLOAT)rc.right), Ex_Scale((FLOAT)rc.bottom), Ex_Scale(StrokeWidth), D2D1_DASH_STYLE_SOLID);
		}
		else
		{
			if (Radius == NULL)
			{
				_canvas_drawrect(hCanvas, hbrush, 0, 0, Ex_Scale((FLOAT)rc.right), Ex_Scale((FLOAT)rc.bottom), Ex_Scale(StrokeWidth), D2D1_DASH_STYLE_SOLID);
			}
			else
			{
				_canvas_drawroundedrect(hCanvas, hbrush, Ex_Scale((FLOAT)rc.left + StrokeWidth), Ex_Scale((FLOAT)rc.top + StrokeWidth), Ex_Scale((FLOAT)rc.right - StrokeWidth), Ex_Scale((FLOAT)rc.bottom - StrokeWidth), Ex_Scale(Radius - StrokeWidth), Ex_Scale(Radius - StrokeWidth), Ex_Scale(StrokeWidth), D2D1_DASH_STYLE_SOLID);
			}
		}

		edit_s* pOwenr = (edit_s*)Ex_ObjGetLong(hObj, EOL_OWNER);
		RECT* P = (RECT*)pOwenr->prctext_;

		/*绘制图标*/
		if (NULL != hImage)
		{
			_canvas_setantialias(hCanvas, TRUE);
			_canvas_setimageantialias(hCanvas, TRUE);

			if (nIconPosition == 0)
			{
				P->left = (INT)Height;
				P->right = (INT)(Width - Ex_Scale(10));
				_canvas_drawimagerect(hCanvas, hImage, Height / 4, Height / 4, Height - Height / 4, Height - Height / 4, 255);
			}
			else if (nIconPosition >= 1)
			{
				P->left = (INT)Ex_Scale(10);
				P->right = (INT)(Width - Height);
				_canvas_drawimagerect(hCanvas, hImage, Width - Height + Height / 4, Height / 4, Width - Height + Height - Height / 4, Height - Height / 4, 255);
			}
		}
		_brush_destroy(hbrush);
	}
	return Ex_ObjCallProc(m_pfnEditExProc, hWnd, hObj, uMsg, wParam, lParam);
}
