﻿#include "test_obj.h"
#include "resource.h"

HEXDUI m_hExDui_button;

LRESULT CALLBACK OnButtonEvent(HEXOBJ hObj, INT nID, INT nCode, WPARAM wParam, LPARAM lParam)
{
	if (nID == 201)//通过组件ID判断按钮
	{
		Ex_ObjEnable(hObj, FALSE);//禁用自身
		Ex_ObjSetPadding(hObj, 0, 20, 5, 5, 5, TRUE);
	}
	else if (nID == 202)
	{
		Ex_ObjEnable(Ex_ObjGetFromID(m_hExDui_button, 201), TRUE);//通过组件ID取按钮句柄，解除按钮1禁用
	}
	else if (nID == 203)
	{
		Ex_ObjSetText(hObj, L"自身文本被改动", TRUE);//改动自身文本
	}
	else if (nID == 204)
	{
		auto text_length = Ex_ObjGetTextLength(Ex_ObjGetFromID(m_hExDui_button, 201));//取按钮1文本长度
		std::wstring str;
		str.resize(text_length * 2 + 2);
		Ex_ObjGetText(Ex_ObjGetFromID(m_hExDui_button, 201), str.c_str(), text_length * 2);
		Ex_ObjSetText(hObj, (L"按钮1文本:" + str).c_str(), TRUE);
	}
	else if (nID == 205)
	{
		if (nCode == NM_CUSTOMDRAW)
		{
			EX_PAINTSTRUCT2 ecd{ 0 };
			RtlMoveMemory(&ecd, (LPVOID)lParam, sizeof(EX_PAINTSTRUCT2));
			EXARGB crBkg = 0;
			if ((ecd.dwState & STATE_DOWN) != 0)
			{
				crBkg = ExRGB2ARGB(255, 51);
			}
			else if ((ecd.dwState & STATE_HOVER) != 0)
			{
				crBkg = ExRGB2ARGB(16754943, 51);
			}
			else {
				crBkg = ExRGB2ARGB(16777215, 51);
			}
			HEXBRUSH hBrush = _brush_create(crBkg);
			if (hBrush)
			{
				_canvas_fillrect(ecd.hCanvas, hBrush, 0, 0, ecd.p_right, ecd.p_bottom);
				_brush_destroy(hBrush);
			}
		}

	}
	else if (nID == 206)
	{
		if (wParam != 0)
		{
			Ex_MessageBox(hObj, L"开启", L"取开关状态", 128, EMBF_CENTEWINDOW);
		}
		else
		{
			Ex_MessageBox(hObj, L"关闭", L"取开关状态", MB_ICONWARNING, EMBF_CENTEWINDOW);
		}
	}
	return 0;
}


void test_button(HWND hWnd)
{
	HWND hWnd_button = Ex_WndCreate(hWnd, L"Ex_DirectUI", L"测试按钮开关", 0, 0, 300, 250, 0, 0);
	m_hExDui_button = Ex_DUIBindWindowEx(hWnd_button, 0, EWS_NOINHERITBKG | EWS_BUTTON_CLOSE | EWS_BUTTON_MIN | EWS_MOVEABLE | EWS_CENTERWINDOW | EWS_TITLE | EWS_SIZEABLE | EWS_HASICON, 0, 0);
	Ex_DUISetLong(m_hExDui_button, EWL_CRBKG, ExARGB(150, 150, 150, 255));
	std::vector<HEXOBJ> buttons;
	buttons.push_back(Ex_ObjCreateEx(-1, L"button", L"禁用自身", -1, 10, 30, 120, 30, m_hExDui_button, 201, DT_VCENTER | DT_CENTER, 0, 0, NULL));
	buttons.push_back(Ex_ObjCreateEx(-1, L"button", L"解除按钮1禁用", -1, 10, 70, 120, 30, m_hExDui_button, 202, DT_VCENTER | DT_CENTER, 0, 0, NULL));
	buttons.push_back(Ex_ObjCreateEx(-1, L"button", L"改动自身文本", -1, 10, 110, 120, 30, m_hExDui_button, 203, DT_VCENTER | DT_CENTER, 0, 0, NULL));
	buttons.push_back(Ex_ObjCreateEx(-1, L"button", L"取按钮1文本", -1, 10, 150, 120, 30, m_hExDui_button, 204, DT_VCENTER | DT_CENTER, 0, 0, NULL));

	for (auto button : buttons)
	{
		Ex_ObjHandleEvent(button, NM_CLICK, OnButtonEvent);
	}

	HEXOBJ customdrawbutton = Ex_ObjCreateEx(EOS_EX_FOCUSABLE | EOS_EX_CUSTOMDRAW | EOS_EX_COMPOSITED, L"button", L"重画背景按钮", -1, 10, 190, 120, 30, m_hExDui_button, 205, DT_VCENTER | DT_CENTER, 0, 0, NULL);
	Ex_ObjHandleEvent(customdrawbutton, NM_CUSTOMDRAW, OnButtonEvent);

	WCHAR ATOM_switch[] = L"Switch";
	HEXOBJ switchObj = Ex_ObjCreate(ATOM_switch, L"已开启|已关闭", -1, 150, 30, 80, 30, m_hExDui_button);

	HEXOBJ switchObj2 = Ex_ObjCreateEx(-1, ATOM_switch, L"开|关", -1, 150, 80, 60, 30, m_hExDui_button, 206, -1, 0, 0, 0);
	Ex_ObjSendMessage(switchObj2, BM_SETCHECK, 1, 0); /* 设置选中状态 */
	Ex_ObjHandleEvent(switchObj2, NM_CHECK, OnButtonEvent);
	EX_OBJ_PROPS switchprops = { 0 };
	switchprops.COLOR_EX_BACKGROUND_NORMAL = ExARGB(255, 255, 255, 100);
	switchprops.COLOR_EX_BACKGROUND_DOWNORCHECKED = ExARGB(200, 50, 100, 0);
	switchprops.COLOR_EX_BORDER_NORMAL = ExARGB(255, 255, 255, 255);
	switchprops.Radius = 15;
	switchprops.StrokeWidth = 1;
	Ex_ObjSendMessage(switchObj2, WM_EX_PROPS, 0, (LPARAM)&switchprops);

	Ex_DUIShowWindow(m_hExDui_button, SW_SHOWNORMAL, 0, 0, 0);
}


void test_label(HWND hWnd)
{
	HWND hWnd_label = Ex_WndCreate(hWnd, L"Ex_DirectUI", L"测试标签", 0, 0, 400, 400, 0, 0);
	HEXDUI hExDui_label = Ex_DUIBindWindowEx(hWnd_label, 0, EWS_NOINHERITBKG | EWS_BUTTON_CLOSE | EWS_BUTTON_MIN | EWS_MOVEABLE | EWS_CENTERWINDOW | EWS_TITLE | EWS_SIZEABLE | EWS_HASICON, 0, 0);
	Ex_DUISetLong(hExDui_label, EWL_CRBKG, ExARGB(150, 150, 150, 255));
	HEXOBJ label = Ex_ObjCreateEx(-1, L"static", NULL, -1, 10, 30, 180, 150, hExDui_label, 0, DT_VCENTER, 0, 0, NULL);
	std::vector<CHAR> imgdata;
	Ex_ReadFile(L"res/Loading.png", &imgdata);
	Ex_ObjSetBackgroundImage(label, imgdata.data(), imgdata.size(), 0, 0, BIR_DEFAULT, 0, BIF_PLAYIMAGE, 255, TRUE);
	EX_BACKGROUNDIMAGEINFO bkg{ 0 };
	Ex_ObjGetBackgroundImage(label, &bkg);
	output(L"背景信息:", bkg.x, bkg.y, bkg.dwAlpha, bkg.dwRepeat, bkg.hImage, bkg.curFrame, bkg.maxFrame);
	RECT rect;
	Ex_ObjGetRect(label, &rect);
	output(L"标签矩形:", rect.right, rect.bottom);

	HEXOBJ label2 = Ex_ObjCreateEx(-1, L"static", NULL, -1, 200, 30, 180, 150, hExDui_label, 0, DT_VCENTER, 0, 0, NULL);
	Ex_ReadFile(L"res/Loading.gif", &imgdata);
	Ex_ObjSetBackgroundImage(label2, imgdata.data(), imgdata.size(), 0, 0, BIR_DEFAULT, 0, BIF_PLAYIMAGE, 255, TRUE);


	HEXOBJ label3 = Ex_ObjCreateEx(-1, L"static", NULL, -1, 10, 200, 180, 150, hExDui_label, 0, DT_VCENTER, 0, 0, NULL);
	Ex_ReadFile(L"res/webp.webp", &imgdata);
	Ex_ObjSetBackgroundImage(label3, imgdata.data(), imgdata.size(), 0, 0, BIR_DEFAULT, 0, BIF_PLAYIMAGE, 255, TRUE);

	HEXOBJ label4 = Ex_ObjCreateEx(-1, L"static", L"标签可以填充动画,支持APNG,PNG,GIF,JPG,BMP,WEBP格式,标签可以自动换行", -1, 200, 200, 180, 60, hExDui_label, 0, DT_WORDBREAK, 0, 0, NULL);
	Ex_ObjSetColor(label4, COLOR_EX_TEXT_NORMAL, ExARGB(133, 33, 53, 255), TRUE);

	Ex_DUIShowWindow(hExDui_label, SW_SHOWNORMAL, 0, 0, 0);
}

LRESULT CALLBACK OnCheckButtonCheckedEvent(HEXOBJ hObj, INT nID, INT nCode, WPARAM wParam, LPARAM lParam)
{
	if (wParam != 0)
	{
		BOOL fChecked = TRUE;//弹出的信息框里面复选框是选中状态
		Ex_MessageBoxEx(hObj, L"选中啦", L"取CheckButton状态", MB_USERICON, L"不再提示", &fChecked, 10, EMBF_CENTEWINDOW, 0);
	}
	else
	{
		Ex_MessageBox(hObj, L"取消选中啦", L"取CheckButton状态", MB_ICONWARNING, EMBF_CENTEWINDOW);
	}
	return 0;
}

void test_checkbutton(HWND hWnd)
{
	HWND hWnd_checkbutton = Ex_WndCreate(hWnd, L"Ex_DirectUI", L"测试单选框复选框", 0, 0, 300, 250, 0, 0);
	HEXDUI hExDui_checkbutton = Ex_DUIBindWindowEx(hWnd_checkbutton, 0, EWS_NOINHERITBKG | EWS_BUTTON_CLOSE | EWS_BUTTON_MIN | EWS_MOVEABLE | EWS_CENTERWINDOW | EWS_TITLE | EWS_SIZEABLE | EWS_HASICON, 0, 0);
	Ex_DUISetLong(hExDui_checkbutton, EWL_CRBKG, ExARGB(150, 150, 150, 255));

	HEXOBJ checkbutton = Ex_ObjCreateEx(-1, L"checkbutton", L"复选框", -1, 10, 30, 60, 20, hExDui_checkbutton, 0, DT_VCENTER, 0, 0, NULL);
	HEXOBJ radiobuttona = Ex_ObjCreateEx(-1, L"radiobutton", L"单选框1", -1, 10, 60, 80, 20, hExDui_checkbutton, 0, DT_VCENTER, 0, 0, NULL);
	HEXOBJ radiobuttonb = Ex_ObjCreateEx(-1, L"radiobutton", L"单选框2", -1, 100, 60, 80, 20, hExDui_checkbutton, 0, DT_VCENTER, 0, 0, NULL);

	EX_OBJ_PROPS CheckButtonExProps = { 0 };
	WCHAR ATOM_CheckButtonEx[] = L"CheckButtonEx";
	HEXOBJ	CheckButtonObj = Ex_ObjCreate(ATOM_CheckButtonEx, L"扩展复选框", -1, 10, 90, 120, 30, hExDui_checkbutton);
	Ex_ObjSetColor(CheckButtonObj, COLOR_EX_TEXT_NORMAL, ExARGB(255, 255, 255, 255), TRUE);
	CheckButtonExProps.COLOR_EX_BACKGROUND_DOWNORCHECKED = ExARGB(215, 117, 23, 250);
	CheckButtonExProps.COLOR_EX_BORDER_NORMAL = ExARGB(222, 222, 222, 150);
	CheckButtonExProps.COLOR_EX_BORDER_HOVER = ExARGB(222, 222, 222, 200);
	CheckButtonExProps.COLOR_EX_BORDER_DOWNORCHECKED = ExARGB(222, 222, 222, 250);
	Ex_ObjSendMessage(CheckButtonObj, WM_EX_PROPS, 0, (LPARAM)&CheckButtonExProps);
	Ex_ObjSendMessage(CheckButtonObj, BM_SETCHECK, 1, 0);

	HEXOBJ	CheckButtonObj2 = Ex_ObjCreate(ATOM_CheckButtonEx, L"扩展复选框", -1, 10, 120, 120, 30, hExDui_checkbutton);
	CheckButtonExProps.COLOR_EX_BACKGROUND_DOWNORCHECKED = ExARGB(251, 171, 213, 250);
	CheckButtonExProps.COLOR_EX_BORDER_NORMAL = ExARGB(22, 222, 222, 150);
	CheckButtonExProps.COLOR_EX_BORDER_HOVER = ExARGB(22, 222, 222, 200);
	CheckButtonExProps.COLOR_EX_BORDER_DOWNORCHECKED = ExARGB(222, 22, 222, 250);
	Ex_ObjSendMessage(CheckButtonObj2, WM_EX_PROPS, 0, (LPARAM)&CheckButtonExProps);
	Ex_ObjHandleEvent(CheckButtonObj2, NM_CHECK, OnCheckButtonCheckedEvent);

	HEXOBJ	CheckButtonObj3 = Ex_ObjCreate(ATOM_CheckButtonEx, L"三态选择框", -1, 10, 150, 100, 30, hExDui_checkbutton);
	Ex_ObjSendMessage(CheckButtonObj3, BM_SETCHECK, 2, 0);

	WCHAR ATOM_RadioButtonEx[] = L"RadioButtonEx";	/*单选框*/
	HEXOBJ	RadioButtonObj = Ex_ObjCreate(ATOM_RadioButtonEx, L"扩展单选框", -1, 150, 120, 120, 30, hExDui_checkbutton);
	HEXOBJ	RadioButtonObj2 = Ex_ObjCreate(ATOM_RadioButtonEx, L"扩展单选框2", -1, 150, 150, 120, 30, hExDui_checkbutton);
	Ex_ObjSendMessage(RadioButtonObj2, BM_SETCHECK, 1, 0);
	WCHAR ATOM_CheckBox[] = L"CheckBox";
	HEXOBJ	checkboxObj = Ex_ObjCreate(ATOM_CheckBox, L"记住账号", -1, 10, 200, 100, 30, hExDui_checkbutton);
	Ex_ObjHandleEvent(checkboxObj, NM_CHECK, OnCheckButtonCheckedEvent);

	HEXOBJ	checkboxObj2 = Ex_ObjCreate(ATOM_CheckBox, L"记住密码", -1, 150, 200, 100, 30, hExDui_checkbutton);
	Ex_ObjSendMessage(checkboxObj2, BM_SETCHECK, 1, 0);

	Ex_DUIShowWindow(hExDui_checkbutton, SW_SHOWNORMAL, 0, 0, 0);
}

void test_edit(HWND hWnd)
{
	HWND hWnd_edit = Ex_WndCreate(hWnd, L"Ex_DirectUI", L"测试编辑框", 0, 0, 600, 300, 0, 0);
	HEXDUI hExDui_edit = Ex_DUIBindWindowEx(hWnd_edit, 0, EWS_NOINHERITBKG | EWS_BUTTON_CLOSE | EWS_BUTTON_MIN | EWS_MOVEABLE | EWS_CENTERWINDOW | EWS_TITLE | EWS_SIZEABLE | EWS_HASICON, 0, 0);
	Ex_DUISetLong(hExDui_edit, EWL_CRBKG, ExARGB(150, 150, 150, 255));

	HEXOBJ edit = Ex_ObjCreateEx(EOS_EX_FOCUSABLE | EOS_EX_COMPOSITED | EOS_EX_CUSTOMDRAW, L"edit", L"背景图片编辑框", EOS_VISIBLE | EES_HIDESELECTION, 10, 30, 150, 30, hExDui_edit, 0, DT_VCENTER, 0, 0, NULL);
	std::vector<CHAR> imgdata;
	Ex_ReadFile(L"res/editbkg.jpg", &imgdata);
	Ex_ObjSetBackgroundImage(edit, imgdata.data(), imgdata.size(), 0, 0, BIR_DEFAULT, 0, BIF_DEFAULT, 255, TRUE);
	HEXOBJ edit2 = Ex_ObjCreateEx(EOS_EX_FOCUSABLE | EOS_EX_COMPOSITED, L"edit", L"测试密码输入编辑框", EOS_VISIBLE | EES_USEPASSWORD, 10, 70, 150, 30, hExDui_edit, 0, DT_SINGLELINE, 0, 0, NULL);
	HEXOBJ edit3 = Ex_ObjCreateEx(EOS_EX_FOCUSABLE | EOS_EX_COMPOSITED, L"edit", L"测试数值输入编辑框", EOS_VISIBLE | EES_NUMERICINPUT, 10, 110, 150, 30, hExDui_edit, 0, DT_SINGLELINE, 0, 0, NULL);
	HEXOBJ edit4 = Ex_ObjCreateEx(EOS_EX_FOCUSABLE | EOS_EX_COMPOSITED, L"edit", L"测试只读编辑框", EOS_VISIBLE | EES_READONLY, 10, 150, 150, 30, hExDui_edit, 0, DT_SINGLELINE, 0, 0, NULL);
	HEXOBJ edit5 = Ex_ObjCreateEx(EOS_EX_FOCUSABLE | EOS_EX_COMPOSITED, L"edit", L"测试多行编辑框\r\n测试多行编辑框\r\n测试多行编辑框\r\n测试多行编辑框\r\n测试多行编辑框\r\n测试多行编辑框\r\n测试多行编辑框\r\n测试多行编辑框\r\n测试多行编辑框\r\n测试多行编辑框\r\n测试多行编辑框\r\n测试多行编辑框\r\n测试多行编辑框\r\n测试多行编辑框\r\n测试多行编辑框\r\n测试多行编辑框\r\n", EOS_VISIBLE | EOS_VSCROLL, 180, 30, 150, 200, hExDui_edit, 0, DT_VCENTER, 0, 0, NULL);
	HEXOBJ edit6 = Ex_ObjCreateEx(EOS_EX_FOCUSABLE | EOS_EX_COMPOSITED, L"edit", NULL, EOS_VISIBLE | EOS_VSCROLL | EOS_HSCROLL | EES_RICHTEXT, 360, 30, 200, 200, hExDui_edit, 0, DT_LEFT | DT_TOP, 0, 0, NULL);

	std::vector<CHAR> rtf;
	Ex_ReadFile(L"res/test.rtf", &rtf);
	Ex_ObjSendMessage(edit6, EM_LOAD_RTF, rtf.size(), (size_t)rtf.data());

	HEXOBJ edit_transparent = Ex_ObjCreateEx(EOS_EX_FOCUSABLE | EOS_EX_COMPOSITED | EOS_EX_TABSTOP | EOS_EX_CUSTOMDRAW, L"edit", L"测试透明圆角编辑框", EOS_VISIBLE | EES_HIDESELECTION, 10, 190, 150, 40, hExDui_edit, 0, DT_VCENTER, 0, 0, NULL);
	Ex_ObjSetColor(edit_transparent, COLOR_EX_BACKGROUND, ExARGB(200, 120, 130, 100), FALSE);
	Ex_ObjSetColor(edit_transparent, COLOR_EX_TEXT_NORMAL, ExRGB2ARGB(16872215, 100), FALSE);
	Ex_ObjSetRadius(edit_transparent, 10, 10, 10, 0, FALSE);
	Ex_DUIShowWindow(hExDui_edit, SW_SHOWNORMAL, 0, 0, 0);
}

void test_menubutton(HWND hWnd)
{
	HWND hWnd_menubutton = Ex_WndCreate(hWnd, L"Ex_DirectUI", L"测试菜单按钮", 0, 0, 350, 200, 0, 0);
	HEXDUI hExDui_menubutton = Ex_DUIBindWindowEx(hWnd_menubutton, 0, EWS_NOINHERITBKG | EWS_BUTTON_CLOSE | EWS_BUTTON_MIN | EWS_MOVEABLE | EWS_CENTERWINDOW | EWS_TITLE | EWS_SIZEABLE | EWS_HASICON, 0, 0);
	Ex_DUISetLong(hExDui_menubutton, EWL_CRBKG, ExARGB(150, 150, 150, 255));

	HEXOBJ hObjMenuBar = Ex_ObjCreate(L"Page", 0, -1, 0, 30, 300, 22, hExDui_menubutton);
	if (hObjMenuBar != 0) {
		HEXLAYOUT hLayout = _layout_create(ELT_LINEAR, hObjMenuBar);
		HMENU hMenu = LoadMenuW(GetModuleHandleW(0), (LPWSTR)IDR_MENU1);
		if (hMenu) {
			for (INT i = 0; i < GetMenuItemCount(hMenu); i++) {
				WCHAR wzText[256];
				GetMenuStringW(hMenu, i, wzText, 256, MF_BYPOSITION);
				HEXOBJ hObj = Ex_ObjCreateEx(-1, L"MenuButton", wzText, -1, 0, 0, 50, 22, hObjMenuBar, 0, -1, (size_t)GetSubMenu(hMenu, i), 0, 0);
				if (hObj) {
					Ex_ObjSetColor(hObj, COLOR_EX_BACKGROUND, 0, FALSE);
					Ex_ObjSetColor(hObj, COLOR_EX_TEXT_HOVER, ExARGB(255, 255, 255, 50), FALSE);
					Ex_ObjSetColor(hObj, COLOR_EX_TEXT_DOWN, ExARGB(255, 255, 255, 100), FALSE);
					Ex_ObjSetColor(hObj, COLOR_EX_TEXT_NORMAL, ExARGB(255, 255, 255, 255), FALSE);
					_layout_addchild(hLayout, hObj);
				}
			}
		}
		Ex_ObjLayoutSet(hObjMenuBar, hLayout, TRUE);
	}
	Ex_DUIShowWindow(hExDui_menubutton, SW_SHOWNORMAL, 0, 0, 0);
}

void test_custombkg(HWND hWnd)
{
	HWND hWnd_custombkg = Ex_WndCreate(hWnd, L"Ex_DirectUI", L"", 0, 0, 175, 200, 0, 0);
	HEXDUI hExDui_custombkg = Ex_DUIBindWindowEx(hWnd_custombkg, 0, EWS_NOINHERITBKG | EWS_MOVEABLE | EWS_CENTERWINDOW | EWS_NOSHADOW, 0, 0);
	std::vector<CHAR> imgdata;
	Ex_ReadFile(L"res/custombkg.png", &imgdata);
	RECT grid = { 45,40,15,15 };
	Ex_ObjSetBackgroundImage(hExDui_custombkg, imgdata.data(), imgdata.size(), 0, 0, BIR_DEFAULT, &grid, BIF_DEFAULT, 220, TRUE);
	Ex_ObjCreateEx(EOS_EX_TOPMOST, L"sysbutton", L"", EOS_VISIBLE | EWS_BUTTON_CLOSE, 140, 8, 30, 30, hExDui_custombkg, 300, 0, 0, 0, NULL);
	Ex_DUIShowWindow(hExDui_custombkg, SW_SHOWNORMAL, 0, 0, 0);
}

std::vector<LISTVIEW_ITEM> m_list_item_info;

LRESULT CALLBACK OnListViewMsgProc(HWND hWnd, HEXOBJ hObj, INT uMsg, WPARAM wParam, LPARAM lParam, LRESULT* lpResult)
{
	if (uMsg == WM_NOTIFY)
	{
		EX_NMHDR ni{ 0 };
		RtlMoveMemory(&ni, (LPVOID)lParam, sizeof(EX_NMHDR));
		if (hObj == ni.hObjFrom)
		{
			if (ni.nCode == NM_CALCSIZE)
			{
				__set_int((LPVOID)ni.lParam, 4, 25);//改变项目高度
				*lpResult = 1;
				return 1;
			}
			else if (ni.nCode == NM_CUSTOMDRAW)
			{
				EX_CUSTOMDRAW cd{ 0 };
				RtlMoveMemory(&cd, (LPVOID)ni.lParam, sizeof(EX_CUSTOMDRAW));
				if (cd.iItem > 0 && cd.iItem <= 100)
				{
					EXARGB crItemBkg = 0;
					if ((cd.dwState & STATE_SELECT) != 0)
					{
						crItemBkg = ExRGB2ARGB(16777215, 255);
					}
					else if ((cd.dwState & STATE_HOVER) != 0)
					{
						crItemBkg = ExRGB2ARGB(16777215, 150);
					}
					if (crItemBkg != 0)
					{
						HEXBRUSH hBrush = _brush_create(crItemBkg);
						_canvas_fillrect(cd.hCanvas, hBrush, cd.rcPaint.left, cd.rcPaint.top, cd.rcPaint.right, cd.rcPaint.bottom);
						_brush_destroy(hBrush);
					}
					_canvas_drawtext(cd.hCanvas, Ex_ObjGetFont(hObj), m_list_item_info[cd.iItem - 1].color, m_list_item_info[cd.iItem - 1].text, -1, DT_SINGLELINE | DT_VCENTER, cd.rcPaint.left + m_list_item_info[cd.iItem - 1].depth * 5, cd.rcPaint.top, cd.rcPaint.right, cd.rcPaint.bottom);
				}
				*lpResult = 1;
				return 1;
			}
			else if (ni.nCode == LVN_ITEMCHANGED)
			{
				//wParam 新选中项,lParam 旧选中项
				output(L"改变选中ID:", ni.idFrom, L"新选中项:", ni.wParam, L"旧选中项:", ni.lParam);
			}
		}
	}
	else if (uMsg == WM_DESTROY)
	{
		for (auto index : m_list_item_info)
		{
			Ex_FreeBuffer((LPVOID)index.text);//复制的文本要销毁
		}
	}
	return 0;
}

LRESULT OnScrollBarMsg(HWND hWND, HEXOBJ hObj, INT uMsg, WPARAM wParam, LPARAM lParam, LRESULT* lpResult)
{
	if (uMsg == WM_MOUSEHOVER)
	{
		Ex_ObjPostMessage(hObj, SBM_SETVISIBLE, 0, 1);//显示滚动条
	}
	else if (uMsg == WM_MOUSELEAVE)
	{
		Ex_ObjPostMessage(hObj, SBM_SETVISIBLE, 0, 0);//隐藏滚动条
	}
	else if (uMsg == SBM_SETVISIBLE)
	{
		Ex_ObjSetLong(hObj, EOL_ALPHA, lParam != 0 ? 255 : 0);
		Ex_ObjInvalidateRect(hObj, 0);
	}
	return 0;
}


void test_listview(HWND hWnd)
{
	HWND hWnd_listview = Ex_WndCreate(hWnd, L"Ex_DirectUI", L"测试列表框", 0, 0, 200, 200, 0, 0);
	HEXDUI hExDui_listview = Ex_DUIBindWindowEx(hWnd_listview, 0, EWS_NOINHERITBKG | EWS_BUTTON_CLOSE | EWS_BUTTON_MIN | EWS_MOVEABLE | EWS_CENTERWINDOW | EWS_TITLE | EWS_SIZEABLE | EWS_HASICON, 0, 0);
	Ex_DUISetLong(hExDui_listview, EWL_CRBKG, ExARGB(150, 150, 150, 255));

	HEXOBJ listview = Ex_ObjCreateEx(EOS_EX_COMPOSITED, L"listview", NULL, EOS_VISIBLE | ELVS_VERTICALLIST | EOS_VSCROLL, 30, 30, 150, 150, hExDui_listview, 0, -1, 0, 0, OnListViewMsgProc);
	Ex_ObjSetColor(listview, COLOR_EX_BACKGROUND, ExARGB(255, 255, 255, 150), TRUE);
	const INT itemCount = 100;
	m_list_item_info.resize(itemCount);
	for (INT index = 0; index < m_list_item_info.size(); index++)
	{
		m_list_item_info[index].color = ExRGB2ARGB(Random(0, 16777215), 255);
		auto str = L"列表项" + std::to_wstring(index);
		m_list_item_info[index].text = StrDupW(str.c_str());
		m_list_item_info[index].depth = index % 5;
	}
	Ex_ObjSendMessage(listview, LVM_SETITEMCOUNT, itemCount, itemCount);

	HEXOBJ hObjScroll = Ex_ObjScrollGetControl(listview, SB_VERT);
	Ex_ObjPostMessage(hObjScroll, SBM_SETVISIBLE, 0, 0);//隐藏滚动条
	Ex_ObjSetLong(hObjScroll, EOL_OBJPROC, (size_t)OnScrollBarMsg);//改变滚动条回调

	Ex_DUIShowWindow(hExDui_listview, SW_SHOWNORMAL, 0, 0, 0);
}

void test_groupbox(HWND hWnd)
{
	HWND hWnd_groupbox = Ex_WndCreate(hWnd, L"Ex_DirectUI", L"测试分组框", 0, 0, 400, 300, 0, 0);
	HEXDUI hExDui_groupbox = Ex_DUIBindWindowEx(hWnd_groupbox, 0, EWS_NOINHERITBKG | EWS_BUTTON_CLOSE | EWS_BUTTON_MIN | EWS_MOVEABLE | EWS_CENTERWINDOW | EWS_TITLE | EWS_SIZEABLE | EWS_HASICON, 0, 0);
	Ex_DUISetLong(hExDui_groupbox, EWL_CRBKG, ExARGB(150, 150, 150, 255));

	HEXOBJ groupbox = Ex_ObjCreate(L"groupbox", L"分组框", -1, 30, 30, 230, 230, hExDui_groupbox);
	Ex_ObjSetColor(groupbox, COLOR_EX_TEXT_NORMAL, ExARGB(255, 55, 55, 255), FALSE);
	Ex_ObjSetColor(groupbox, COLOR_EX_BORDER, ExARGB(55, 0, 250, 255), FALSE);
	Ex_ObjSetLong(groupbox, GROUPBOX_TEXT_OFFSET, 50);
	Ex_ObjSetLong(groupbox, GROUPBOX_RADIUS, 30);
	Ex_ObjSetLong(groupbox, GROUPBOX_STROKEWIDTH, 3);
	Ex_DUIShowWindow(hExDui_groupbox, SW_SHOWNORMAL, 0, 0, 0);
}

std::vector<HEXOBJ> m_hObj_Navbtn(4);
HEXOBJ m_hObj_PageContainer;
std::vector<HEXOBJ> m_hObj_Page(4);
INT m_nCurIndex = 1;

size_t OnNavButtonPageEasing(LPVOID pEasing, DOUBLE nProgress, DOUBLE nCurrent, LPVOID pEasingContext, INT nTimeSurplus, size_t p1, size_t p2, size_t p3, size_t p4)
{
	for (INT i = 0; i < m_hObj_Page.size(); i++)
	{
		Ex_ObjSetPos(m_hObj_Page[i], 0, i * 760 - nCurrent, 0, 0, 0, SWP_NOSIZE | SWP_NOZORDER | SWP_NOACTIVATE);
	}
	return m_nCurIndex != p1;
}


LRESULT CALLBACK OnNavButtonCheckEvent(HEXOBJ hObj, INT nID, INT nCode, WPARAM wParam, LPARAM lParam)
{
	if (lParam != 0)
	{
		INT nIndex = Ex_ObjGetLong(hObj, EOL_LPARAM);
		HEXEASING pEasing = _easing_create(ET_InOutQuart, 0, ES_SINGLE | ES_THREAD | ES_CALLFUNCTION, (size_t)OnNavButtonPageEasing, 200, 20, EES_PLAY, m_nCurIndex * 760, nIndex * 760, nIndex, 0, 0, 0);
		m_nCurIndex = nIndex;
	}
	return 0;
}

void test_navbutton(HWND hWnd)
{
	HWND hWnd_navbutton = Ex_WndCreate(hWnd, L"Ex_DirectUI", L"测试选项卡", 0, 0, 800, 600, 0, 0);
	HEXDUI hExDui_navbutton = Ex_DUIBindWindowEx(hWnd_navbutton, 0, EWS_NOINHERITBKG | EWS_BUTTON_CLOSE | EWS_BUTTON_MIN | EWS_MOVEABLE | EWS_CENTERWINDOW | EWS_TITLE | EWS_SIZEABLE | EWS_HASICON, 0, 0);
	HEXIMAGE hImage = 0;
	for (INT i = 0; i < 4; i++)
	{
		auto str = L"Tab" + std::to_wstring(i);
		m_hObj_Navbtn[i] = Ex_ObjCreate(L"NavButton", str.c_str(), -1, 20 + i * 85, 40, 80, 80, hExDui_navbutton);
		auto file = L"./navbtn/大图标" + std::to_wstring(i + 1) + L".png";
		_img_createfromfile(file.c_str(), &hImage);
		Ex_ObjSendMessage(m_hObj_Navbtn[i], WM_SETICON, 0, hImage);
		Ex_ObjSetColor(m_hObj_Navbtn[i], COLOR_EX_TEXT_NORMAL, ExRGB2ARGB(16777215, 255), FALSE);
		HEXIMAGE hImg = 0;
		_img_createfromfile(L"./navbtn/顶部按钮背景C.png", &hImg);
		Ex_ObjSendMessage(m_hObj_Navbtn[i], BM_SETIMAGE, 1, hImg);
		_img_createfromfile(L"./navbtn/顶部按钮背景D.png", &hImg);
		Ex_ObjSendMessage(m_hObj_Navbtn[i], BM_SETIMAGE, 2, hImg);
		Ex_ObjInvalidateRect(m_hObj_Navbtn[i], 0);
		Ex_ObjSetLong(m_hObj_Navbtn[i], EOL_LPARAM, i);
		Ex_ObjHandleEvent(m_hObj_Navbtn[i], NM_CHECK, OnNavButtonCheckEvent);
	}
	m_hObj_PageContainer = Ex_ObjCreate(L"page", 0, -1, 20, 120, 760, 600 - 120 - 20, hExDui_navbutton);
	for (INT i = 0; i < 4; i++)
	{
		auto str = L"页面" + std::to_wstring(i);
		m_hObj_Page[i] = Ex_ObjCreateEx(-1, L"static", str.c_str(), -1, 760 * i, 0, 760, 600 - 120 - 20, m_hObj_PageContainer, 0, DT_CENTER | DT_VCENTER, 0, 0, 0);
		Ex_ObjSetFontFromFamily(m_hObj_Page[i], 0, 40, 0, TRUE);
		Ex_ObjSetColor(m_hObj_Page[i], COLOR_EX_BACKGROUND, ExRGB2ARGB(Random(0, 16777215), 255), TRUE);
		Ex_ObjSetColor(m_hObj_Page[i], COLOR_EX_TEXT_NORMAL, ExRGB2ARGB(16777215, 255), TRUE);
	}
	m_nCurIndex = 0;
	Ex_ObjSendMessage(m_hObj_Navbtn[0], BM_SETCHECK, 1, 1);
	Ex_DUISetLong(hExDui_navbutton, EWL_CRBKG, ExRGB2ARGB(0, 255));
	Ex_DUIShowWindow(hExDui_navbutton, SW_SHOWNORMAL, 0, 0, 0);
}

void test_absolute(HWND hWnd)
{
	HWND hWnd_absolute = Ex_WndCreate(hWnd, L"Ex_DirectUI", L"测试绝对布局", 0, 0, 450, 300, 0, 0);
	HEXDUI hExDui_absolute = Ex_DUIBindWindowEx(hWnd_absolute, 0, EWS_NOINHERITBKG | EWS_BUTTON_CLOSE | EWS_BUTTON_MAX | EWS_BUTTON_MIN | EWS_MOVEABLE | EWS_CENTERWINDOW | EWS_TITLE | EWS_SIZEABLE | EWS_HASICON, 0, 0);
	Ex_DUISetLong(hExDui_absolute, EWL_CRBKG, ExARGB(150, 150, 150, 255));
	HEXLAYOUT hLayout = _layout_create(ELT_ABSOLUTE, hExDui_absolute);

	/*HEXOBJ hObj = Ex_ObjCreate(L"Static", L"左20顶50底20宽200", -1, 0, 0, 200, 100, hExDui_absolute);
	Ex_ObjSetColor(hObj, COLOR_EX_BACKGROUND, ExRGB2ARGB(255, 100), TRUE);
	_layout_absolute_setedge(hLayout, hObj, ELCP_ABSOLUTE_TOP, ELCP_ABSOLUTE_TYPE_PX, 50);
	_layout_absolute_setedge(hLayout, hObj, ELCP_ABSOLUTE_LEFT, ELCP_ABSOLUTE_TYPE_PX, 20);
	_layout_absolute_setedge(hLayout, hObj, ELCP_ABSOLUTE_BOTTOM, ELCP_ABSOLUTE_TYPE_PX, 20);

	HEXOBJ hObj2 = Ex_ObjCreate(L"Static", L"顶50右20左220高50%内边距10", -1, 0, 0, 200, 100, hExDui_absolute);
	Ex_ObjSetColor(hObj2, COLOR_EX_BACKGROUND, ExRGB2ARGB(255, 100), TRUE);
	_layout_absolute_setedge(hLayout, hObj2, ELCP_ABSOLUTE_TOP, ELCP_ABSOLUTE_TYPE_PX, 50);
	_layout_absolute_setedge(hLayout, hObj2, ELCP_ABSOLUTE_RIGHT, ELCP_ABSOLUTE_TYPE_PX, 20);
	_layout_absolute_setedge(hLayout, hObj2, ELCP_ABSOLUTE_LEFT, ELCP_ABSOLUTE_TYPE_PX, 240);
	_layout_absolute_setedge(hLayout, hObj2, ELCP_ABSOLUTE_HEIGHT, ELCP_ABSOLUTE_TYPE_PS, 50);
	_layout_setchildprop(hLayout, hObj2, ELCP_MARGIN_BOTTOM, 45);

	HEXOBJ hObj3 = Ex_ObjCreate(L"Static", L"底20右20左220高50%内边距10", -1, 0, 0, 200, 100, hExDui_absolute);
	Ex_ObjSetColor(hObj3, COLOR_EX_BACKGROUND, ExRGB2ARGB(255, 100), TRUE);
	_layout_absolute_setedge(hLayout, hObj3, ELCP_ABSOLUTE_RIGHT, ELCP_ABSOLUTE_TYPE_PX, 20);
	_layout_absolute_setedge(hLayout, hObj3, ELCP_ABSOLUTE_BOTTOM, ELCP_ABSOLUTE_TYPE_PX, 20);
	_layout_absolute_setedge(hLayout, hObj3, ELCP_ABSOLUTE_LEFT, ELCP_ABSOLUTE_TYPE_PX, 240);
	_layout_absolute_setedge(hLayout, hObj3, ELCP_ABSOLUTE_HEIGHT, ELCP_ABSOLUTE_TYPE_PS, 50);
	_layout_setchildprop(hLayout, hObj3, ELCP_MARGIN_TOP, 45);*/

	HEXOBJ hObj = Ex_ObjCreate(L"Static", L"固定在右下角50,50的位置，不变大小", -1, 0, 0, 200, 100, hExDui_absolute);
	Ex_ObjSetColor(hObj, COLOR_EX_BACKGROUND, ExRGB2ARGB(255, 100), TRUE);
	_layout_absolute_setedge(hLayout, hObj, ELCP_ABSOLUTE_RIGHT, ELCP_ABSOLUTE_TYPE_PX, 50);//设置距离右边为50像素
	_layout_absolute_setedge(hLayout, hObj, ELCP_ABSOLUTE_BOTTOM, ELCP_ABSOLUTE_TYPE_PX, 50);// 设置距离底边为50像素

	HEXOBJ hObj2 = Ex_ObjCreate(L"Static", L"固定在左下角50,50的位置，宽度为40%,高度为50", -1, 0, 0, 200, 100, hExDui_absolute);
	Ex_ObjSetColor(hObj2, COLOR_EX_BACKGROUND, ExRGB2ARGB(0, 100), TRUE);
	_layout_absolute_setedge(hLayout, hObj2, ELCP_ABSOLUTE_LEFT, ELCP_ABSOLUTE_TYPE_PX, 50);//设置距离左边为50像素
	_layout_absolute_setedge(hLayout, hObj2, ELCP_ABSOLUTE_BOTTOM, ELCP_ABSOLUTE_TYPE_PX, 50);// 设置距离底边为50像素
	_layout_absolute_setedge(hLayout, hObj2, ELCP_ABSOLUTE_WIDTH, ELCP_ABSOLUTE_TYPE_PS, 40);// 注意单位是PS（百分比）
	_layout_absolute_setedge(hLayout, hObj2, ELCP_ABSOLUTE_HEIGHT, ELCP_ABSOLUTE_TYPE_PX, 50);

	HEXOBJ hObj3 = Ex_ObjCreate(L"Static", L"距离四边均为20%", -1, 0, 0, 0, 0, hExDui_absolute);
	Ex_ObjSetColor(hObj3, COLOR_EX_BACKGROUND, ExRGB2ARGB(16711680, 100), TRUE);
	_layout_absolute_setedge(hLayout, hObj3, ELCP_ABSOLUTE_LEFT, ELCP_ABSOLUTE_TYPE_PS, 20);//注意单位是PS（百分比）
	_layout_absolute_setedge(hLayout, hObj3, ELCP_ABSOLUTE_TOP, ELCP_ABSOLUTE_TYPE_PS, 20);// 注意单位是PS（百分比）
	_layout_absolute_setedge(hLayout, hObj3, ELCP_ABSOLUTE_RIGHT, ELCP_ABSOLUTE_TYPE_PS, 20);// 注意单位是PS（百分比）
	_layout_absolute_setedge(hLayout, hObj3, ELCP_ABSOLUTE_BOTTOM, ELCP_ABSOLUTE_TYPE_PS, 20);// 注意单位是PS（百分比）

	HEXOBJ hObj4 = Ex_ObjCreate(L"Static", L"居中于窗口,宽度为窗口的30%,高度为100像素", -1, 0, 0, 0, 0, hExDui_absolute);
	Ex_ObjSetColor(hObj4, COLOR_EX_BACKGROUND, ExRGB2ARGB(65280, 100), TRUE);
	_layout_absolute_setedge(hLayout, hObj4, ELCP_ABSOLUTE_LEFT, ELCP_ABSOLUTE_TYPE_PS, 50);//注意单位是PS（百分比）
	_layout_absolute_setedge(hLayout, hObj4, ELCP_ABSOLUTE_TOP, ELCP_ABSOLUTE_TYPE_PS, 50);// 注意单位是PS（百分比）
	_layout_absolute_setedge(hLayout, hObj4, ELCP_ABSOLUTE_WIDTH, ELCP_ABSOLUTE_TYPE_PS, 30);// 注意单位是PS（百分比）
	_layout_absolute_setedge(hLayout, hObj4, ELCP_ABSOLUTE_HEIGHT, ELCP_ABSOLUTE_TYPE_PX, 100);// 
	_layout_absolute_setedge(hLayout, hObj4, ELCP_ABSOLUTE_OFFSET_H, ELCP_ABSOLUTE_TYPE_OBJPS, -50);//  水平偏移控件-50%的控件宽度 注意单位是OBJ_PS（控件尺寸的百分比）
	_layout_absolute_setedge(hLayout, hObj4, ELCP_ABSOLUTE_OFFSET_V, ELCP_ABSOLUTE_TYPE_OBJPS, -50);//  水平偏移控件-50%的控件高度 注意单位是OBJ_PS（控件尺寸的百分比）

	Ex_ObjLayoutSet(hExDui_absolute, hLayout, TRUE);

	Ex_DUIShowWindow(hExDui_absolute, SW_SHOWNORMAL, 0, 0, 0);
}

void test_relative(HWND hWnd)
{
	HWND hWnd_relative = Ex_WndCreate(hWnd, L"Ex_DirectUI", L"测试相对布局", 0, 0, 600, 400, 0, 0);
	HEXDUI hExDui_relative = Ex_DUIBindWindowEx(hWnd_relative, 0, EWS_NOINHERITBKG | EWS_BUTTON_CLOSE | EWS_BUTTON_MAX | EWS_BUTTON_MIN | EWS_MOVEABLE | EWS_CENTERWINDOW | EWS_TITLE | EWS_SIZEABLE | EWS_HASICON, 0, 0);
	Ex_DUISetLong(hExDui_relative, EWL_CRBKG, ExARGB(150, 150, 150, 255));
	HEXLAYOUT hLayout = _layout_create(ELT_RELATIVE, hExDui_relative);
	_layout_setprop(hLayout, ELP_PADDING_LEFT, 10);
	_layout_setprop(hLayout, ELP_PADDING_TOP, 25);
	_layout_setprop(hLayout, ELP_PADDING_RIGHT, 10);
	_layout_setprop(hLayout, ELP_PADDING_BOTTOM, 10);

	HEXOBJ hObj = Ex_ObjCreateEx(-1, L"Static", L"控件A：父容器的左下角", -1, 0, 0, 200, 150, hExDui_relative, 0, DT_VCENTER, 0, 0, 0);
	Ex_ObjSetColor(hObj, COLOR_EX_BACKGROUND, ExARGB(255, 0, 0, 100), TRUE);
	_layout_setchildprop(hLayout, hObj, ELCP_RELATIVE_LEFT_ALIGN_OF, -1);//左侧与父容器对齐
	_layout_setchildprop(hLayout, hObj, ELCP_RELATIVE_BOTTOM_ALIGN_OF, -1);// 底边与父容器对齐

	HEXOBJ hObj2 = Ex_ObjCreateEx(-1, L"Static", L"控件B：父容器居中顶部", -1, 0, 0, 200, 150, hExDui_relative, 0, DT_VCENTER, 0, 0, 0);
	Ex_ObjSetColor(hObj2, COLOR_EX_BACKGROUND, ExRGB2ARGB(16711680, 100), TRUE);
	_layout_setchildprop(hLayout, hObj2, ELCP_RELATIVE_TOP_ALIGN_OF, -1);//顶部与父容器对齐
	_layout_setchildprop(hLayout, hObj2, ELCP_RELATIVE_CENTER_PARENT_H, 1);// 水平居中于父容器

	HEXOBJ hObj3 = Ex_ObjCreateEx(-1, L"Static", L"控件C：右侧与A对齐,宽度150,在A和B之间", -1, 0, 0, 150, 150, hExDui_relative, 0, DT_VCENTER, 0, 0, 0);
	Ex_ObjSetColor(hObj3, COLOR_EX_BACKGROUND, ExRGB2ARGB(65280, 100), TRUE);
	_layout_setchildprop(hLayout, hObj3, ELCP_RELATIVE_TOP_OF, hObj);//在A控件顶部
	_layout_setchildprop(hLayout, hObj3, ELCP_RELATIVE_BOTTOM_OF, hObj2);// 在B控件底部
	_layout_setchildprop(hLayout, hObj3, ELCP_RELATIVE_RIGHT_ALIGN_OF, hObj);// 在B右侧对齐于A控件

	HEXOBJ hObj4 = Ex_ObjCreateEx(-1, L"Static", L"控件D：高度100,在A和父控件右边界之间,在父容器底部", -1, 0, 0, 150, 100, hExDui_relative, 0, DT_VCENTER, 0, 0, 0);
	Ex_ObjSetColor(hObj4, COLOR_EX_BACKGROUND, ExRGB2ARGB(16754943, 100), TRUE);
	_layout_setchildprop(hLayout, hObj4, ELCP_RELATIVE_RIGHT_OF, hObj);//在A控件右边
	_layout_setchildprop(hLayout, hObj4, ELCP_RELATIVE_BOTTOM_ALIGN_OF, -1);// 底部对齐于父容器
	_layout_setchildprop(hLayout, hObj4, ELCP_RELATIVE_RIGHT_ALIGN_OF, -1);// 右侧对齐于父容器

	HEXOBJ hObj5 = Ex_ObjCreateEx(-1, L"Static", L"控件E：与D同宽,在D和B之间", -1, 0, 0, 150, 100, hExDui_relative, 0, DT_VCENTER, 0, 0, 0);
	Ex_ObjSetColor(hObj5, COLOR_EX_BACKGROUND, ExRGB2ARGB(8445952, 100), TRUE);
	_layout_setchildprop(hLayout, hObj5, ELCP_RELATIVE_TOP_OF, hObj4);//在D顶部
	_layout_setchildprop(hLayout, hObj5, ELCP_RELATIVE_BOTTOM_OF, hObj2);// 在B底部
	_layout_setchildprop(hLayout, hObj5, ELCP_RELATIVE_LEFT_ALIGN_OF, hObj4);// 左侧对齐于D
	_layout_setchildprop(hLayout, hObj5, ELCP_RELATIVE_RIGHT_ALIGN_OF, hObj4);// 右侧对齐于D

	HEXOBJ hObj6 = Ex_ObjCreateEx(-1, L"Static", L"控件F：150宽度,垂直方向对齐于DE,右对齐于DE", -1, 0, 0, 150, 100, hExDui_relative, 0, DT_VCENTER, 0, 0, 0);
	Ex_ObjSetColor(hObj6, COLOR_EX_BACKGROUND, ExRGB2ARGB(16777215, 100), TRUE);
	_layout_setchildprop(hLayout, hObj6, ELCP_RELATIVE_TOP_ALIGN_OF, hObj5);//顶部对齐于E
	_layout_setchildprop(hLayout, hObj6, ELCP_RELATIVE_BOTTOM_ALIGN_OF, hObj4);// 底部对齐于D
	_layout_setchildprop(hLayout, hObj6, ELCP_RELATIVE_RIGHT_ALIGN_OF, hObj4);// 右对齐于D

	Ex_ObjLayoutSet(hExDui_relative, hLayout, TRUE);
	RECT rect{ 0 };
	Ex_ObjGetRect(hObj4, &rect);
	Ex_DUIShowWindow(hExDui_relative, SW_SHOWNORMAL, 0, 0, 0);
}

void test_linear(HWND hWnd)
{
	HWND hWnd_linear = Ex_WndCreate(hWnd, L"Ex_DirectUI", L"测试线性布局", 0, 0, 600, 400, 0, 0);
	HEXDUI hExDui_linear = Ex_DUIBindWindowEx(hWnd_linear, 0, EWS_NOINHERITBKG | EWS_BUTTON_CLOSE | EWS_BUTTON_MAX | EWS_BUTTON_MIN | EWS_MOVEABLE | EWS_CENTERWINDOW | EWS_TITLE | EWS_SIZEABLE | EWS_HASICON, 0, 0);
	Ex_DUISetLong(hExDui_linear, EWL_CRBKG, ExARGB(150, 150, 150, 255));
	HEXLAYOUT hLayout = _layout_create(ELT_LINEAR, hExDui_linear);
	_layout_setprop(hLayout, ELP_LINEAR_DIRECTION, ELP_DIRECTION_H); //设置布局方向为水平
	_layout_setprop(hLayout, ELP_LINEAR_DALIGN, ELP_LINEAR_DALIGN_CENTER);// 设置布局方向对齐方式为居中

	HEXOBJ hObj = Ex_ObjCreateEx(-1, L"Static", L"容器1", -1, 0, 0, 200, 300, hExDui_linear, 0, DT_VCENTER, 0, 0, 0);
	Ex_ObjSetColor(hObj, COLOR_EX_BACKGROUND, ExRGB2ARGB(255, 100), TRUE);
	_layout_setchildprop(hLayout, hObj, ELCP_LINEAR_ALIGN, ELCP_LINEAR_ALIGN_CENTER);//设置居中于父
	_layout_setchildprop(hLayout, hObj, ELCP_MARGIN_RIGHT, 10);// 设置右边10像素间隔

	HEXOBJ hObj2 = Ex_ObjCreateEx(-1, L"Static", L"容器2", -1, 0, 0, 400, 200, hExDui_linear, 0, DT_VCENTER, 0, 0, 0);
	Ex_ObjSetColor(hObj2, COLOR_EX_BACKGROUND, ExRGB2ARGB(16711680, 100), TRUE);
	_layout_setchildprop(hLayout, hObj2, ELCP_LINEAR_ALIGN, ELCP_LINEAR_ALIGN_CENTER);//设置居中于父
	_layout_setchildprop(hLayout, hObj2, ELCP_MARGIN_RIGHT, 10);// 设置右边10像素间隔

	HEXOBJ hObj3 = Ex_ObjCreateEx(-1, L"Static", L"容器3", -1, 0, 0, 100, 100, hExDui_linear, 0, DT_VCENTER, 0, 0, 0);
	Ex_ObjSetColor(hObj3, COLOR_EX_BACKGROUND, ExRGB2ARGB(65280, 100), TRUE);
	_layout_setchildprop(hLayout, hObj3, ELCP_LINEAR_ALIGN, ELCP_LINEAR_ALIGN_CENTER);//设置居中于父

	Ex_ObjLayoutSet(hExDui_linear, hLayout, TRUE);
	Ex_DUIShowWindow(hExDui_linear, SW_SHOWNORMAL, 0, 0, 0);
}

void test_flow(HWND hWnd)
{
	HWND hWnd_flow = Ex_WndCreate(hWnd, L"Ex_DirectUI", L"测试流式布局", 0, 0, 600, 400, 0, 0);
	HEXDUI hExDui_flow = Ex_DUIBindWindowEx(hWnd_flow, 0, EWS_NOINHERITBKG | EWS_BUTTON_CLOSE | EWS_BUTTON_MAX | EWS_BUTTON_MIN | EWS_MOVEABLE | EWS_CENTERWINDOW | EWS_TITLE | EWS_SIZEABLE | EWS_HASICON, 0, 0);
	Ex_DUISetLong(hExDui_flow, EWL_CRBKG, ExARGB(150, 150, 150, 255));
	HEXLAYOUT hLayout = _layout_create(ELT_FLOW, hExDui_flow);
	_layout_setprop(hLayout, ELP_FLOW_DIRECTION, ELP_DIRECTION_V);//设置布局方向为垂直

	_layout_setprop(hLayout, ELP_PADDING_LEFT, 30);
	_layout_setprop(hLayout, ELP_PADDING_TOP, 30); //设置距离顶边30
	_layout_setprop(hLayout, ELP_PADDING_RIGHT, 30);
	_layout_setprop(hLayout, ELP_PADDING_BOTTOM, 30);

	HEXOBJ hObj = 0;
	for (INT i = 0; i < 20; i++)
	{
		hObj = Ex_ObjCreate(L"Static", L"test", -1, 0, 0, Random(50, 150), Random(50, 150), hExDui_flow);
		Ex_ObjSetColor(hObj, COLOR_EX_BACKGROUND, ExRGB2ARGB(255, 100), TRUE);
		_layout_setchildprop(hLayout, hObj, ELCP_MARGIN_RIGHT, 10);
		_layout_setchildprop(hLayout, hObj, ELCP_MARGIN_BOTTOM, 10);
		if (i % 10 == 0)
		{
			_layout_setchildprop(hLayout, hObj, ELCP_FLOW_NEW_LINE, 1);
		}
	}
	Ex_ObjLayoutSet(hExDui_flow, hLayout, TRUE);

	Ex_DUIShowWindow(hExDui_flow, SW_SHOWNORMAL, 0, 0, 0);
}

void test_table(HWND hWnd)
{
	HWND hWnd_table = Ex_WndCreate(hWnd, L"Ex_DirectUI", L"测试表格布局", 0, 0, 600, 400, 0, 0);
	HEXDUI hExDui_table = Ex_DUIBindWindowEx(hWnd_table, 0, EWS_NOINHERITBKG | EWS_BUTTON_CLOSE | EWS_BUTTON_MAX | EWS_BUTTON_MIN | EWS_MOVEABLE | EWS_CENTERWINDOW | EWS_TITLE | EWS_SIZEABLE | EWS_HASICON, 0, 0);
	Ex_DUISetLong(hExDui_table, EWL_CRBKG, ExARGB(150, 150, 150, 255));
	HEXLAYOUT hLayout = _layout_create(ELT_TABLE, hExDui_table);

	_layout_setprop(hLayout, ELP_PADDING_LEFT, 10);
	_layout_setprop(hLayout, ELP_PADDING_TOP, 30);
	_layout_setprop(hLayout, ELP_PADDING_RIGHT, 10);
	_layout_setprop(hLayout, ELP_PADDING_BOTTOM, 10);
	INT row[4] = { 50,-30,75,-20 };//4行,正数为像素,负数为百分比
	INT cell[3] = { 100,75,-50 };//3列,正数为像素,负数为百分比
	HEXOBJ hObj = 0;
	_layout_table_setinfo(hLayout, row, 4, cell, 3);
	for (INT i = 1; i <= 4; i++)
	{
		for (INT j = 1; j <= 3; j++)
		{
			auto str = std::to_wstring(i) + L"," + std::to_wstring(j);
			hObj = Ex_ObjCreateEx(-1, L"static", (LPCWSTR)str.c_str(), -1, 0, 0, 200, 150, hExDui_table, 0, DT_VCENTER, 0, 0, 0);
			Ex_ObjSetColor(hObj, COLOR_EX_BACKGROUND, ExRGB2ARGB(255, 100), FALSE);
			_layout_setchildprop(hLayout, hObj, ELCP_TABLE_ROW, i); // 设置所属行
			_layout_setchildprop(hLayout, hObj, ELCP_TABLE_CELL, j); //设置所属列
		}
	}
	HEXOBJ hObj2 = Ex_ObjCreateEx(-1, L"static", L"(2,1)[占2行]", -1, 0, 0, 200, 150, hExDui_table, 0, DT_VCENTER, 0, 0, 0);
	Ex_ObjSetColor(hObj2, COLOR_EX_BACKGROUND, ExRGB2ARGB(65535, 150), TRUE);
	_layout_setchildprop(hLayout, hObj2, ELCP_TABLE_CELL, 2);
	_layout_setchildprop(hLayout, hObj2, ELCP_TABLE_ROW, 1);
	_layout_setchildprop(hLayout, hObj2, ELCP_TABLE_ROW_SPAN, 2);//设置跨行数

	HEXOBJ hObj3 = Ex_ObjCreateEx(-1, L"static", L"(1,3)[占3列2行]", -1, 0, 0, 200, 150, hExDui_table, 0, DT_VCENTER, 0, 0, 0);
	Ex_ObjSetColor(hObj3, COLOR_EX_BACKGROUND, ExRGB2ARGB(16711935, 120), TRUE);
	_layout_setchildprop(hLayout, hObj3, ELCP_TABLE_CELL, 1);
	_layout_setchildprop(hLayout, hObj3, ELCP_TABLE_ROW, 3);
	_layout_setchildprop(hLayout, hObj3, ELCP_TABLE_ROW_SPAN, 2);
	_layout_setchildprop(hLayout, hObj3, ELCP_TABLE_CELL_SPAN, 3);//设置跨列数

	Ex_ObjLayoutSet(hExDui_table, hLayout, TRUE);

	Ex_DUIShowWindow(hExDui_table, SW_SHOWNORMAL, 0, 0, 0);
}

std::vector<HEXOBJ> m_hComboBoxButton(7);
HEXOBJ hComboBox;

LRESULT CALLBACK OnComboBoxButtonEvent(HEXOBJ hObj, INT nID, INT nCode, WPARAM wParam, LPARAM lParam)
{
	if (hObj == m_hComboBoxButton[0])
	{
		size_t count = Ex_ObjSendMessage(hComboBox, CB_GETCOUNT, 0, 0);
		std::wstring text = L"测试添加" + std::to_wstring(count + 1);
		Ex_ObjSendMessage(hComboBox, CB_ADDSTRING, 0, (LPARAM)text.c_str());
	}
	else if (hObj == m_hComboBoxButton[1])
	{
		std::wstring text = L"插入项目" ;
		Ex_ObjSendMessage(hComboBox, CB_INSERTSTRING, 1, (LPARAM)text.c_str());
	}
	else if (hObj == m_hComboBoxButton[2])
	{
		size_t len = Ex_ObjGetTextLength(hComboBox) * 2 + 2;
		std::wstring text;
		text.resize(len);
		Ex_ObjGetText(hComboBox, text.c_str(), len);
		output(text);
	}
	else if (hObj == m_hComboBoxButton[3])
	{
		std::wstring text = L"置内容";
		Ex_ObjSetText(hComboBox, text.c_str(), TRUE);
	}
	else if (hObj == m_hComboBoxButton[4])
	{
		Ex_ObjSendMessage(hComboBox, CB_RESETCONTENT, 0, 0);
	}
	else if (hObj == m_hComboBoxButton[5])
	{
		Ex_ObjSendMessage(hComboBox, CB_SHOWDROPDOWN,TRUE, 0);
	}
	else if (hObj == m_hComboBoxButton[6])
	{
		Ex_ObjSendMessage(hComboBox, CB_DELETESTRING, 2, 0);
	}
	return 0;
}

void test_combobox(HWND hWnd)
{
	HWND hWnd_combobox = Ex_WndCreate(hWnd, L"Ex_DirectUI", L"测试组合框", 0, 0, 400, 300, 0, 0);
	HEXDUI hExDui_combobox = Ex_DUIBindWindowEx(hWnd_combobox, 0, EWS_NOINHERITBKG | EWS_BUTTON_CLOSE | EWS_BUTTON_MIN | EWS_MOVEABLE | EWS_CENTERWINDOW | EWS_TITLE | EWS_SIZEABLE | EWS_HASICON, 0, 0);
	Ex_DUISetLong(hExDui_combobox, EWL_CRBKG, ExARGB(150, 150, 150, 255));
	hComboBox = Ex_ObjCreateEx(-1, L"combobox", L"测试组合框", EOS_VISIBLE | ECS_ALLOWEDIT, 10, 30, 100, 30, hExDui_combobox, 0, DT_VCENTER, 0, 0, NULL);
	Ex_ObjSendMessage(hComboBox, CB_ADDSTRING, 0, (size_t)L"TESTa");
	Ex_ObjSendMessage(hComboBox, CB_ADDSTRING, 0, (size_t)L"1234");
	Ex_ObjSendMessage(hComboBox, CB_ADDSTRING, 0, (size_t)L"qwer他！（）");
	Ex_ObjSendMessage(hComboBox, CB_ADDSTRING, 0, (size_t)L"uiop[");

	WCHAR Cls[] = L"button";
	m_hComboBoxButton[0] = Ex_ObjCreate(Cls, L"添加项目", -1,130, 30, 100, 30, hExDui_combobox);
	Ex_ObjHandleEvent(m_hComboBoxButton[0], NM_CLICK, OnComboBoxButtonEvent);

	m_hComboBoxButton[1] = Ex_ObjCreate(Cls, L"插入项目", -1, 130, 70, 100, 30, hExDui_combobox);
	Ex_ObjHandleEvent(m_hComboBoxButton[1], NM_CLICK, OnComboBoxButtonEvent);

	m_hComboBoxButton[2] = Ex_ObjCreate(Cls, L"取内容", -1, 130, 110, 100, 30, hExDui_combobox);
	Ex_ObjHandleEvent(m_hComboBoxButton[2], NM_CLICK, OnComboBoxButtonEvent);

	m_hComboBoxButton[3] = Ex_ObjCreate(Cls, L"置内容", -1, 240, 30, 100, 30, hExDui_combobox);
	Ex_ObjHandleEvent(m_hComboBoxButton[3], NM_CLICK, OnComboBoxButtonEvent);

	m_hComboBoxButton[4] = Ex_ObjCreate(Cls, L"清空表项", -1, 240, 70, 100, 30, hExDui_combobox);
	Ex_ObjHandleEvent(m_hComboBoxButton[4], NM_CLICK, OnComboBoxButtonEvent);

	m_hComboBoxButton[5] = Ex_ObjCreate(Cls, L"弹出列表", -1, 240, 110, 100, 30, hExDui_combobox);
	Ex_ObjHandleEvent(m_hComboBoxButton[5], NM_CLICK, OnComboBoxButtonEvent);

	m_hComboBoxButton[6] = Ex_ObjCreate(Cls, L"删除项目", -1, 130, 150, 100, 30, hExDui_combobox);
	Ex_ObjHandleEvent(m_hComboBoxButton[6], NM_CLICK, OnComboBoxButtonEvent);

	Ex_DUIShowWindow(hExDui_combobox, SW_SHOWNORMAL, 0, 0, 0);
}

HWND m_hWnd_ani = 0;
HEXDUI m_hExDui_ani = 0;
HEXEASING m_easing3 = nullptr;

LRESULT CALLBACK OnAniWndMsgProc(HWND hWnd, HEXDUI hExDui, INT uMsg, WPARAM wParam, LPARAM lParam, LRESULT* lpResult)
{
	if (uMsg == WM_CLOSE)
	{
		AniShow(FALSE);
	}
	return 0;
}

size_t CALLBACK OnAniEasing(LPVOID pEasing, DOUBLE nProgress, DOUBLE nCurrent, LPVOID pEasingContext, INT nTimeSurplus, size_t p1, size_t p2, size_t p3, size_t p4)
{
	INT index = nCurrent * 255;
	Ex_DUISetLong(m_hExDui_ani, EWL_CRBKG, ExARGB(150, 150, 150, index));
	Ex_DUISetLong(m_hExDui_ani, EWL_ALPHA, index);
	SetWindowPos((HWND)Ex_DUIGetLong(m_hExDui_ani, EWL_HWND), 0, p1 + p2 * nCurrent, p3 + p4 * nCurrent, 0, 0, SWP_NOSIZE | SWP_NOACTIVATE | SWP_NOZORDER);
	return 0;
}

void AniShow(BOOL fShow)
{
	Ex_DUISetLong(m_hExDui_ani, EWL_CRBKG, ExARGB(150, 150, 150, fShow ? 0 : 255));
	Ex_DUISetLong(m_hExDui_ani, EWL_ALPHA, fShow ? 0 : 255);
	Ex_DUIShowWindow(m_hExDui_ani, SW_SHOW, 0, 0, 0);

	RECT rc{ 0 };
	GetWindowRect((HWND)Ex_DUIGetLong(m_hExDui_ani, EWL_HWND), &rc);
	_easing_create(ET_InOutQuint, 0, ES_SINGLE | ES_CALLFUNCTION | (fShow ? 0 : ES_REVERSE), (size_t)OnAniEasing, 500, 20, EES_PLAY, 0, 1, rc.left, 0, rc.top - 100, 100);
	Ex_DUIShowWindow(m_hExDui_ani, fShow ? SW_SHOW : SW_HIDE, 0, 0, 0);
}

size_t CALLBACK OnBtnEasing(LPVOID pEasing, DOUBLE nProgress, DOUBLE nCurrent, LPVOID pEasingContext, INT nTimeSurplus, size_t p1, size_t p2, size_t p3, size_t p4)
{
	Ex_ObjSetPos(p1, 0, 0, 0, nCurrent, 50, SWP_NOMOVE | SWP_NOZORDER);
	return 0;
}

size_t CALLBACK OnBtnEasing4(LPVOID pEasing, DOUBLE nProgress, DOUBLE nCurrent, LPVOID pEasingContext, INT nTimeSurplus, size_t p1, size_t p2, size_t p3, size_t p4)
{
	RECT rc{ 0 };
	GetWindowRect(m_hWnd_ani, &rc);
	MoveWindow(m_hWnd_ani, rc.left, rc.top, nCurrent, rc.bottom - rc.top, TRUE);
	return 0;
}

LRESULT CALLBACK OnAniButtonEvent(HEXOBJ hObj, INT nID, INT nCode, WPARAM wParam, LPARAM lParam)
{
	if (nID == 10001)
	{
		if (nCode == NM_CLICK)
		{
			_easing_create(ET_InOutCubic, 0, ES_SINGLE | ES_THREAD | ES_CALLFUNCTION, (size_t)OnBtnEasing, 200, 20, EES_PLAY, 150, 300, hObj, 0, 0, 0);
		}
	}
	else if (nID == 10002)
	{
		if (nCode == NM_CLICK)
		{
			_easing_create(ET_InOutCubic, 0, MAKELONG(ES_MANYTIMES | ES_BACKANDFORTH | ES_THREAD | ES_DISPATCHNOTIFY, 10 * 2), hObj, 150, 20, EES_PLAY, 150, 300, 0, 0, 0, 0);
		}
		else if (nCode == NM_EASING)
		{
			EX_EASINGINFO pEasingInfo{ 0 };
			RtlMoveMemory(&pEasingInfo, (LPVOID)lParam, sizeof(EX_EASINGINFO));
			Ex_ObjSetPos(hObj, 0, 0, 0, pEasingInfo.nCurrent, 50, SWP_NOMOVE | SWP_NOZORDER);
		}
	}
	else if (nID == 10003)
	{
		if (nCode == NM_CLICK)
		{
			if (_easing_getstate(m_easing3) == EES_PAUSE)
			{
				_easing_setstate(m_easing3, EES_PLAY);
			}
			else {
				_easing_setstate(m_easing3, EES_PAUSE);
			}
		}
		else if (nCode == NM_DESTROY)
		{
			_easing_setstate(m_easing3, EES_STOP);
		}
		else if (nCode == NM_EASING)
		{
			EX_EASINGINFO pEasingInfo{ 0 };
			RtlMoveMemory(&pEasingInfo, (LPVOID)lParam, sizeof(EX_EASINGINFO));
			Ex_ObjSetPos(hObj, 0, 0, 0, pEasingInfo.nCurrent, 50, SWP_NOMOVE | SWP_NOZORDER);
		}
	}
	else if (nID == 10004)
	{
		if (nCode == NM_CLICK)
		{
			_easing_create(ET_InOutCirc, 0, MAKELONG(ES_MANYTIMES | ES_BACKANDFORTH | ES_CALLFUNCTION, 4), (size_t)OnBtnEasing4, 400, 25, 0, 400, 150, 0, 0, 0, 0);
		}
	}
	return 0;
}

void test_ani(HWND hWnd)
{
	m_hWnd_ani = Ex_WndCreate(hWnd, L"Ex_DirectUI", L"测试缓动窗口", 0, 0, 400, 300, 0, 0);
	m_hExDui_ani = Ex_DUIBindWindowEx(m_hWnd_ani, 0, EWS_NOINHERITBKG | EWS_BUTTON_CLOSE | EWS_BUTTON_MIN | EWS_MOVEABLE | EWS_CENTERWINDOW | EWS_SIZEABLE | EWS_TITLE | EWS_HASICON, 0, OnAniWndMsgProc);
	HEXOBJ hObj1 = Ex_ObjCreateEx(-1, L"button", L"点击就动1次", -1, 10, 40, 120, 50, m_hExDui_ani, 10001, DT_VCENTER | DT_CENTER, 0, 0, NULL);
	HEXOBJ hObj2 = Ex_ObjCreateEx(-1, L"button", L"来回", -1, 10, 100, 120, 50, m_hExDui_ani, 10002, DT_VCENTER | DT_CENTER, 0, 0, NULL);
	HEXOBJ hObj3 = Ex_ObjCreateEx(-1, L"button", L"点击动/停", -1, 10, 160, 120, 50, m_hExDui_ani, 10003, DT_VCENTER | DT_CENTER, 0, 0, NULL);
	HEXOBJ hObj4 = Ex_ObjCreateEx(-1, L"button", L"点击窗口动", -1, 10, 220, 120, 50, m_hExDui_ani, 10004, DT_VCENTER | DT_CENTER, 0, 0, NULL);
	Ex_ObjHandleEvent(hObj1, NM_CLICK, OnAniButtonEvent);
	Ex_ObjHandleEvent(hObj2, NM_CLICK, OnAniButtonEvent);
	Ex_ObjHandleEvent(hObj2, NM_EASING, OnAniButtonEvent);
	Ex_ObjHandleEvent(hObj3, NM_CLICK, OnAniButtonEvent);
	Ex_ObjHandleEvent(hObj3, NM_EASING, OnAniButtonEvent);
	Ex_ObjHandleEvent(hObj3, NM_DESTROY, OnAniButtonEvent);
	Ex_ObjHandleEvent(hObj4, NM_CLICK, OnAniButtonEvent);
	m_easing3 = _easing_create(ET_InOutCirc, 0, ES_CYCLE | ES_BACKANDFORTH | ES_THREAD | ES_DISPATCHNOTIFY, hObj3, 200, 20, EES_PAUSE, 150, 300, 0, 0, 0, 0);
	AniShow(TRUE);
}



LRESULT CALLBACK OnCustomRedrawWndMsgProc(HWND hWnd, HEXDUI hExDui, INT uMsg, WPARAM wParam, LPARAM lParam, LRESULT* lpResult)
{
	if (uMsg == WM_ERASEBKGND)
	{
		RECT rc{ 0 };
		GetWindowRect(hWnd, &rc);
		OffsetRect(&rc, -rc.left, -rc.top);
		_canvas_setantialias(wParam, TRUE);
		_canvas_clear(wParam, 0);
		HEXBRUSH hBrush = _brush_create(ExRGB2ARGB(16711680, 150));
		_canvas_fillellipse(wParam, hBrush, rc.right / 2, rc.bottom / 2, rc.right / 2 - 2, rc.bottom / 2 - 2);
		_brush_destroy(hBrush);
		*lpResult = 1;
		return 1;
	}
	return 0;
}

void test_customredraw(HWND hWnd)
{
	//异型窗口采用重画背景形式，才不会产生锯齿。用于需要圆角，不规则图形的窗口。
	HWND hWnd_customredraw = Ex_WndCreate(hWnd, L"Ex_DirectUI", L"", 0, 0, 300, 200, 0, 0);
	HEXDUI hExDui_customredraw = Ex_DUIBindWindowEx(hWnd_customredraw, 0, EWS_MOVEABLE | EWS_CENTERWINDOW | EWS_NOSHADOW, 0, OnCustomRedrawWndMsgProc);
	Ex_DUISetLong(hExDui_customredraw, EWL_CRBKG, ExARGB(150, 150, 150, 255));
	Ex_ObjCreateEx(EOS_EX_TOPMOST, L"sysbutton", L"", EOS_VISIBLE | EWS_BUTTON_CLOSE, (300 - 32) / 2, (200 - 32) / 2, 32, 32, hExDui_customredraw, 0, 0, 0, 0, NULL);//改变标题栏按钮位置

	Ex_DUIShowWindow(hExDui_customredraw, SW_SHOWNORMAL, 0, 0, 0);
}

void test_messagebox(HWND hWnd)
{
	HWND hWnd_messagebox = Ex_WndCreate(hWnd, L"Ex_DirectUI", L"", 0, 0, 200, 200, 0, 0);
	HEXDUI hExDui_messagebox = Ex_DUIBindWindowEx(hWnd_messagebox, 0, EWS_NOINHERITBKG | EWS_MOVEABLE | EWS_CENTERWINDOW | EWS_NOSHADOW | EWS_BUTTON_CLOSE, 0, 0);
	Ex_DUISetLong(hExDui_messagebox, EWL_CRBKG, ExARGB(150, 150, 150, 255));
	Ex_MessageBox(hExDui_messagebox, L"test", L"testa", MB_YESNO | MB_ICONQUESTION, EMBF_CENTEWINDOW);

	Ex_DUIShowWindow(hExDui_messagebox, SW_SHOWNORMAL, 0, 0, 0);
}



LRESULT CALLBACK OnColorButtonProc(HWND hWnd, HEXOBJ hObj, INT uMsg, WPARAM wParam, LPARAM lParam)
{
	if (uMsg == WM_MOUSEHOVER)
	{
		Ex_ObjSetUIState(hObj, STATE_HOVER, FALSE, 0, TRUE);
	}
	else if (uMsg == WM_MOUSELEAVE)
	{
		Ex_ObjSetUIState(hObj, STATE_HOVER, TRUE, 0, TRUE);
	}
	else if (uMsg == WM_LBUTTONDOWN)
	{
		Ex_ObjSetUIState(hObj, STATE_DOWN, FALSE, 0, TRUE);
	}
	else if (uMsg == WM_LBUTTONUP)
	{
		Ex_ObjSetUIState(hObj, STATE_DOWN, TRUE, 0, TRUE);
	}
	else if (uMsg == WM_SETFOCUS)
	{
		Ex_ObjSetUIState(hObj, STATE_FOCUS, FALSE, 0, TRUE);
	}
	else if (uMsg == WM_KILLFOCUS)
	{
		Ex_ObjSetUIState(hObj, STATE_FOCUS, TRUE, 0, TRUE);
	}
	else if (uMsg == WM_SYSCOLORCHANGE)
	{
		if (wParam == COLOR_EX_CBTN_CRBKG_NORMAL)
		{
			Ex_ObjSetLong(hObj, EOUL_CBTN_CRBKG_NORMAL, lParam);
		}
		else if (wParam == COLOR_EX_CBTN_CRBKG_HOVER)
		{
			Ex_ObjSetLong(hObj, EOUL_CBTN_CRBKG_HOVER, lParam);
		}
		else if (wParam == COLOR_EX_CBTN_CRBKG_DOWN)
		{
			Ex_ObjSetLong(hObj, EOUL_CBTN_CRBKG_DOWN, lParam);
		}
		else if (wParam == COLOR_EX_CBTN_CRBKG_FOCUS)
		{
			Ex_ObjSetLong(hObj, EOUL_CBTN_CRBKG_FOCUS, lParam);
		}
	}
	else if (uMsg == WM_PAINT)
	{
		EX_PAINTSTRUCT2 ps;
		if (Ex_ObjBeginPaint(hObj, &ps))
		{
			INT crText = 0;
			INT crBkg = 0;
			if ((ps.dwState & STATE_DOWN) == STATE_DOWN)
			{
				crText = Ex_ObjGetColor(hObj, COLOR_EX_TEXT_DOWN);
				crBkg = Ex_ObjGetLong(hObj, EOUL_CBTN_CRBKG_DOWN);
			}
			else if ((ps.dwState & STATE_HOVER) == STATE_HOVER)
			{
				crText = Ex_ObjGetColor(hObj, COLOR_EX_TEXT_HOVER);
				crBkg = Ex_ObjGetLong(hObj, EOUL_CBTN_CRBKG_HOVER);
			}
			else if ((ps.dwState & STATE_FOCUS) == STATE_FOCUS)
			{
				crText = Ex_ObjGetColor(hObj, COLOR_EX_TEXT_FOCUS);
				crBkg = Ex_ObjGetLong(hObj, EOUL_CBTN_CRBKG_FOCUS);
			}
			if (crBkg == 0)
			{
				crBkg = Ex_ObjGetLong(hObj, EOUL_CBTN_CRBKG_NORMAL);
			}
			if (crText == 0)
			{
				crText = Ex_ObjGetColor(hObj, COLOR_EX_TEXT_NORMAL);
			}
			_canvas_clear(ps.hCanvas, crBkg);
			_canvas_drawtext(ps.hCanvas, Ex_ObjGetFont(hObj), crText, (LPCWSTR)Ex_ObjGetLong(hObj, EOL_LPWZTITLE), -1, ps.dwTextFormat, 0, 0, ps.width, ps.height);
			Ex_ObjEndPaint(hObj, &ps);
		}
	}
	return Ex_ObjDefProc(hWnd, hObj, uMsg, wParam, lParam);
}



void test_colorbutton(HWND hWnd)
{
	HWND hWnd_colorbutton = Ex_WndCreate(hWnd, L"Ex_DirectUI", L"测试自定义按钮", 0, 0, 200, 150, 0, 0);
	HEXDUI hExDui_colorbutton = Ex_DUIBindWindowEx(hWnd_colorbutton, 0, EWS_NOINHERITBKG | EWS_MOVEABLE | EWS_CENTERWINDOW | EWS_NOSHADOW | EWS_BUTTON_CLOSE | EWS_TITLE | EWS_HASICON, 0, 0);
	Ex_DUISetLong(hExDui_colorbutton, EWL_CRBKG, ExARGB(150, 150, 150, 255));
	Ex_ObjRegister(L"colorbutton", EOS_VISIBLE, EOS_EX_TABSTOP | EOS_EX_FOCUSABLE, DT_SINGLELINE | DT_CENTER | DT_VCENTER, 4 * sizeof(size_t), 0, 0, OnColorButtonProc);
	HEXOBJ hObj = Ex_ObjCreateEx(-1, L"colorbutton", L"colorbutton", -1, 50, 50, 100, 50, hExDui_colorbutton, 0, -1, 0, 0, NULL);
	if (hObj != 0)
	{
		Ex_ObjSetColor(hObj, COLOR_EX_CBTN_CRBKG_NORMAL, ExRGB2ARGB(16777215, 255), FALSE);
		Ex_ObjSetColor(hObj, COLOR_EX_CBTN_CRBKG_HOVER, ExRGB2ARGB(65535, 255), FALSE);
		Ex_ObjSetColor(hObj, COLOR_EX_CBTN_CRBKG_DOWN, ExRGB2ARGB(65280, 255), FALSE);
		Ex_ObjSetColor(hObj, COLOR_EX_CBTN_CRBKG_FOCUS, ExRGB2ARGB(16777680, 255), FALSE);

		Ex_ObjSetColor(hObj, COLOR_EX_TEXT_HOVER, ExRGB2ARGB(255, 255), FALSE);
		Ex_ObjSetColor(hObj, COLOR_EX_TEXT_DOWN, ExRGB2ARGB(16754943, 255), FALSE);
		Ex_ObjSetColor(hObj, COLOR_EX_TEXT_FOCUS, ExRGB2ARGB(65280, 255), TRUE);

		Ex_ObjSetRadius(hObj, 25, 25, 25, 25, TRUE);
	}

	Ex_DUIShowWindow(hExDui_colorbutton, SW_SHOWNORMAL, 0, 0, 0);
}

HEXOBJ m_hReportListView;
HEXIMAGELIST m_hReportListView_ImgList;

LRESULT CALLBACK OnReportListViewEvent(HEXOBJ hObj, INT nID, INT nCode, WPARAM wParam, LPARAM lParam)
{
	if (nCode == LVN_ITEMCHANGED)
	{
		auto str = L"你选择了第" + std::to_wstring(wParam) + L"项";
		output(str);
	}
	else if (nCode == RLVN_COLUMNCLICK)
	{
		auto str = L"你点击了第" + std::to_wstring(wParam) + L"列";
		output(str);
	}
	else if (nCode == NM_CLICK)
	{
		wParam = Ex_ObjSendMessage(hObj, LVM_GETSELECTIONMARK, 0, 0);//获取现行选中项
		lParam = Ex_ObjSendMessage(hObj, RLVM_GETHITCOL, 0, LOWORD(lParam));//获取命中的列索引
		auto str = L"你点击了第" + std::to_wstring(wParam) + L"项,第" + std::to_wstring(lParam) + L"列";
		output(str);
	}

	return 0;
}

LRESULT CALLBACK OnReportListViewButtonEvent(HEXOBJ hObj, INT nID, INT nCode, WPARAM wParam, LPARAM lParam)
{
	Ex_ObjSendMessage(m_hReportListView, LVM_DELETECOLUMN, 1, 2);
	return 0;
}

LRESULT CALLBACK OnReportListViewWndMsgProc(HWND hWnd, HEXDUI hExDui, INT uMsg, WPARAM wParam, LPARAM lParam, LRESULT* lpResult)
{
	if (uMsg == WM_CLOSE)
	{
		if (m_hReportListView_ImgList)
		{
			_imglist_destroy(m_hReportListView_ImgList);
		}
	}
	return 0;
}

void test_reportlistview(HWND hWnd)
{
	HWND hWnd_reportlistview = Ex_WndCreate(hWnd, L"Ex_DirectUI", L"测试报表列表", 0, 0, 400, 400, 0, 0);
	HEXDUI hExDui_reportlistview = Ex_DUIBindWindowEx(hWnd_reportlistview, 0, EWS_NOINHERITBKG | EWS_MOVEABLE | EWS_CENTERWINDOW | EWS_NOSHADOW | EWS_BUTTON_CLOSE | EWS_TITLE | EWS_HASICON, 0, OnReportListViewWndMsgProc);
	Ex_DUISetLong(hExDui_reportlistview, EWL_CRBKG, ExARGB(150, 150, 150, 255));
	m_hReportListView = Ex_ObjCreateEx(-1, L"ReportListView", L"ReportListView", -1, 25, 50, 350, 250, hExDui_reportlistview, 0, -1, 0, 0, NULL);
	Ex_ObjSetColor(m_hReportListView, COLOR_EX_BACKGROUND, ExRGB2ARGB(16777215, 100), FALSE);
	Ex_ObjSetColor(m_hReportListView, COLOR_EX_BORDER, ExRGB2ARGB(12632256, 100), FALSE);
	Ex_ObjSetColor(m_hReportListView, COLOR_EX_TEXT_HOT, ExRGB2ARGB(16777215, 250), FALSE);
	Ex_ObjSetColor(m_hReportListView, COLOR_EX_TEXT_HOVER, ExRGB2ARGB(12632256, 50), FALSE);

	m_hReportListView_ImgList = _imglist_create(30, 30);
	std::vector<CHAR> imgdata;
	Ex_ReadFile(L"./icon/1.png", &imgdata);
	size_t nImageIndex = _imglist_add(m_hReportListView_ImgList, imgdata.data(), imgdata.size(), 0);
	Ex_ObjSendMessage(m_hReportListView, LVM_SETIMAGELIST, 0, (LPARAM)m_hReportListView_ImgList);

	EX_REPORTLIST_COLUMNINFO col = { 0 };
	col.wzText = L"第一列";
	col.nWidth = 75;
	col.crText = ExRGB2ARGB(255, 255);
	col.dwStyle = 0;
	col.dwTextFormat = DT_LEFT;
	Ex_ObjSendMessage(m_hReportListView, LVM_INSERTCOLUMN, 0, (size_t)&col);

	col.wzText = L"固定列宽";
	col.nWidth = 110;
	col.crText = ExRGB2ARGB(16711680, 255);
	col.dwStyle = ERLV_CS_LOCKWIDTH;
	col.dwTextFormat = DT_LEFT;
	Ex_ObjSendMessage(m_hReportListView, LVM_INSERTCOLUMN, 0, (size_t)&col);

	col.wzText = L"居中可点击";
	col.nWidth = 100;
	col.crText = ExRGB2ARGB(65535, 255);
	col.dwStyle = ERLV_CS_CLICKABLE;
	col.dwTextFormat = DT_CENTER | DT_VCENTER;
	Ex_ObjSendMessage(m_hReportListView, LVM_INSERTCOLUMN, 0, (size_t)&col);

	col.wzText = L"可排序";
	col.nWidth = 60;
	col.crText = ExRGB2ARGB(16777215, 255);
	col.dwStyle = ERLV_CS_CLICKABLE | ERLV_CS_SORTABLE;
	col.dwTextFormat = DT_RIGHT | DT_VCENTER;
	Ex_ObjSendMessage(m_hReportListView, LVM_INSERTCOLUMN, 0, (size_t)&col);

	EX_REPORTLIST_ROWINFO row = { 0 };
	EX_REPORTLIST_ITEMINFO item = { 0 };
	
	for (INT i = 1; i <= 100; i++)
	{
		//先插入表项
		row.lParam = i + 1;
		item.nImageIndex = nImageIndex;
		item.iRow = Ex_ObjSendMessage(m_hReportListView, LVM_INSERTITEM, 0, (size_t)&row);
		//先插入表项
		item.iCol = 1;
		std::wstring wstr = L"第" + std::to_wstring(i) + L"项";
		item.wzText = wstr.c_str();
		Ex_ObjSendMessage(m_hReportListView, LVM_SETITEM, 0, (size_t)&item);//wParam为是否立即更新

		item.iCol = 2;
		item.wzText = L"第二列";
		Ex_ObjSendMessage(m_hReportListView, LVM_SETITEM, 0, (size_t)&item);//wParam为是否立即更新

		item.iCol = 3;
		item.wzText = L"第三列";
		Ex_ObjSendMessage(m_hReportListView, LVM_SETITEM, 0, (size_t)&item);//wParam为是否立即更新

		item.iCol = 4;
		auto str = std::to_wstring(Random(0,1000));
		item.wzText = str.c_str();
		Ex_ObjSendMessage(m_hReportListView, LVM_SETITEM, 0, (size_t)&item);//wParam为是否立即更新
	}
	Ex_ObjSendMessage(m_hReportListView, LVM_UPDATE, 0, 0);//整体更新,以加快绘制速度
	Ex_ObjHandleEvent(m_hReportListView, LVN_ITEMCHANGED, OnReportListViewEvent);
	Ex_ObjHandleEvent(m_hReportListView, RLVN_COLUMNCLICK, OnReportListViewEvent);
	Ex_ObjHandleEvent(m_hReportListView, NM_CLICK, OnReportListViewEvent);

	HEXOBJ hButton = Ex_ObjCreate(L"button", L"删除列", -1, 20, 330, 100, 30, hExDui_reportlistview);
	Ex_ObjHandleEvent(hButton, NM_CLICK, OnReportListViewButtonEvent);


	Ex_DUIShowWindow(hExDui_reportlistview, SW_SHOWNORMAL, 0, 0, 0);
}

HEXOBJ m_hObj_ListView_icon = 0;
HEXIMAGELIST m_hImageList_icon = 0;


LRESULT CALLBACK OnIconWndMsgProc(HWND hWnd, HEXDUI hExDui, INT uMsg, WPARAM wParam, LPARAM lParam, LRESULT* lpResult)
{
	if (uMsg == WM_SIZE)
	{
		Ex_ObjMove(m_hObj_ListView_icon, 25, 50, LOWORD(lParam) - 50, HIWORD(lParam) - 75, TRUE);
	}
	else if (uMsg == WM_DESTROY)
	{
		_imglist_destroy(m_hImageList_icon);
	}
	return 0;
}

void test_iconlistview(HWND hWnd)
{
	HWND hWnd_iconlistview = Ex_WndCreate(hWnd, L"Ex_DirectUI", L"测试图标列表", 0, 0, 500, 300, 0, 0);
	HEXDUI hExDui_iconlistview = Ex_DUIBindWindowEx(hWnd_iconlistview, 0, EWS_NOINHERITBKG | EWS_MOVEABLE | EWS_CENTERWINDOW | EWS_NOSHADOW | EWS_BUTTON_CLOSE | EWS_TITLE | EWS_HASICON, 0, OnIconWndMsgProc);

	m_hObj_ListView_icon = Ex_ObjCreateEx(-1, L"iconlistview", L"iconlistview", EOS_VISIBLE | EOS_HSCROLL | EOS_VSCROLL | EILVS_BUTTON, 25, 50, 450, 225, hExDui_iconlistview, 0, -1, 0, 0, NULL);
	Ex_ObjSendMessage(m_hObj_ListView_icon, ILVM_SETITEMSIZE, 0, MAKELONG(70, 75));//设置表项尺寸为70,75
	//创建添加图片组信息

	m_hImageList_icon = _imglist_create(36, 36);
	std::vector<CHAR> imgdata;
	for (INT i = 1; i <= 3; i++)
	{
		auto str = L"./icon/" + std::to_wstring(i) + L".png";
		Ex_ReadFile(str.c_str(), &imgdata);
		_imglist_add(m_hImageList_icon, imgdata.data(), imgdata.size(), 0);
	}
	//设置列表的图片组
	Ex_ObjSendMessage(m_hObj_ListView_icon, LVM_SETIMAGELIST, 0, (size_t)m_hImageList_icon);
	EX_ICONLISTVIEW_ITEMINFO ilvi{ 0 };

	for (INT i = 1; i <= 1000; i++)
	{
		ilvi.nIndex = i;
		auto str = L"第" + std::to_wstring(i) + L"项";
		ilvi.pwzText = str.c_str();
		ilvi.nImageIndex = i % 3;
		if (ilvi.nImageIndex == 0) ilvi.nImageIndex = 3;
		Ex_ObjSendMessage(m_hObj_ListView_icon, LVM_INSERTITEM, 0, (size_t)&ilvi);
	}
	Ex_ObjSendMessage(m_hObj_ListView_icon, LVM_UPDATE, 0, 0);

	Ex_DUISetLong(hExDui_iconlistview, EWL_CRBKG, ExARGB(150, 150, 150, 255));
	Ex_DUIShowWindow(hExDui_iconlistview, SW_SHOWNORMAL, 0, 0, 0);
}

void test_treelistview(HWND hWnd)
{
	HWND hWnd_treelistview = Ex_WndCreate(hWnd, L"Ex_DirectUI", L"测试树形列表", 0, 0, 350, 350, 0, 0);
	HEXDUI hExDui_treelistview = Ex_DUIBindWindowEx(hWnd_treelistview, 0, EWS_NOINHERITBKG | EWS_MOVEABLE | EWS_CENTERWINDOW | EWS_NOSHADOW | EWS_BUTTON_CLOSE | EWS_TITLE | EWS_HASICON, 0, 0);
	Ex_DUISetLong(hExDui_treelistview, EWL_CRBKG, ExARGB(150, 150, 150, 255));
	LPCWSTR class_treeview = L"treeview";
	HEXOBJ treeview = Ex_ObjCreateEx(EOS_EX_FOCUSABLE, class_treeview, L"fasdf", -1, 50, 50, 250, 250, hExDui_treelistview, 0, DT_VCENTER, 0, 0, NULL);
	EX_TREEVIEW_INSERTINFO ti = { 0 };

	Ex_ObjSetColor(treeview, COLOR_EX_BACKGROUND, ExARGB(255, 255, 255, 125), FALSE);
	Ex_ObjSetColor(treeview, COLOR_EX_BORDER, ExARGB(255, 255, 255, 255), TRUE);
	ti.fExpand = TRUE;
	ti.tzText = L"节点1";
	Ex_ObjSendMessage(treeview, TVM_INSERTITEMW, 0, (size_t)&ti);

	ti.tzText = L"节点2";
	Ex_ObjSendMessage(treeview, TVM_INSERTITEMW, 0, (size_t)&ti);

	ti.tzText = L"节点3";
	ti.itemParent = (EX_TREEVIEW_NODEITEM*)Ex_ObjSendMessage(treeview, TVM_INSERTITEMW, 0, (size_t)&ti);

	ti.tzText = L"节点3-1";
	Ex_ObjSendMessage(treeview, TVM_INSERTITEMW, 0, (size_t)&ti);

	ti.tzText = L"节点3-2";
	Ex_ObjSendMessage(treeview, TVM_INSERTITEMW, 0, (size_t)&ti);

	ti.tzText = L"节点3-3";
	ti.itemParent = (EX_TREEVIEW_NODEITEM*)Ex_ObjSendMessage(treeview, TVM_INSERTITEMW, 0, (size_t)&ti);

	ti.tzText = L"节点3-3-1";
	Ex_ObjSendMessage(treeview, TVM_INSERTITEMW, 0, (size_t)&ti);

	ti.tzText = L"节点3-3-2";
	Ex_ObjSendMessage(treeview, TVM_INSERTITEMW, 0, (size_t)&ti);

	ti.itemParent = 0;
	ti.tzText = L"节点4";
	Ex_ObjSendMessage(treeview, TVM_INSERTITEMW, 0, (size_t)&ti);

	ti.tzText = L"节点5";
	ti.itemParent = (EX_TREEVIEW_NODEITEM*)Ex_ObjSendMessage(treeview, TVM_INSERTITEMW, 0, (size_t)&ti);

	ti.tzText = L"节点5-1";
	Ex_ObjSendMessage(treeview, TVM_INSERTITEMW, 0, (size_t)&ti);

	ti.tzText = L"节点5-2";
	ti.itemParent = (EX_TREEVIEW_NODEITEM*)Ex_ObjSendMessage(treeview, TVM_INSERTITEMW, 0, (size_t)&ti);

	ti.tzText = L"节点5-2-1";
	Ex_ObjSendMessage(treeview, TVM_INSERTITEMW, 0, (size_t)&ti);

	ti.tzText = L"节点5-2-2";
	ti.itemParent = (EX_TREEVIEW_NODEITEM*)Ex_ObjSendMessage(treeview, TVM_INSERTITEMW, 0, (size_t)&ti);

	ti.tzText = L"节点5-2-2-1";
	Ex_ObjSendMessage(treeview, TVM_INSERTITEMW, 0, (size_t)&ti);

	ti.tzText = L"节点5-2-2-2";
	ti.itemParent = (EX_TREEVIEW_NODEITEM*)Ex_ObjSendMessage(treeview, TVM_INSERTITEMW, 0, (size_t)&ti);

	ti.tzText = L"节点5-2-2-2-1";
	ti.itemParent = (EX_TREEVIEW_NODEITEM*)Ex_ObjSendMessage(treeview, TVM_INSERTITEMW, 0, (size_t)&ti);

	ti.tzText = L"节点5-2-2-2-1-1";
	ti.itemParent = (EX_TREEVIEW_NODEITEM*)Ex_ObjSendMessage(treeview, TVM_INSERTITEMW, 0, (size_t)&ti);

	ti.tzText = L"节点5-2-2-2-1-1-1";
	ti.itemParent = (EX_TREEVIEW_NODEITEM*)Ex_ObjSendMessage(treeview, TVM_INSERTITEMW, 0, (size_t)&ti);

	Ex_ObjSendMessage(treeview, TVM_UPDATE, 0, 0);

	Ex_DUIShowWindow(hExDui_treelistview, SW_SHOWNORMAL, 0, 0, 0);
}


LRESULT CALLBACK OnMatrixMsgProc(HWND hWnd, HEXOBJ hObj, INT uMsg, WPARAM wParam, LPARAM lParam, LRESULT* lpResult)
{
	if (uMsg == WM_PAINT)
	{
		EX_PAINTSTRUCT2 ps;
		Ex_ObjBeginPaint(hObj, &ps);
		_canvas_clear(ps.hCanvas, ExRGB2ARGB(16777215, 100));
		DOUBLE nCurent = (DOUBLE)Ex_ObjGetLong(hObj, EOL_LPARAM) / 100;
		HEXMATRIX mx = _matrix_create();

		_matrix_translate(mx, (FLOAT)ps.width / 2, (FLOAT)ps.height / 2);
		_matrix_rotate(mx, (FLOAT)nCurent * 90);
		_matrix_scale(mx,1 + (FLOAT)nCurent * 0.25, 1 + (FLOAT)nCurent * 0.25);
		_matrix_translate(mx, -(FLOAT)ps.width / 2, -(FLOAT)ps.height / 2);
		_canvas_settransform(ps.hCanvas, mx);

		HEXBRUSH hBrush = _brush_create(Ex_ObjGetColor(hObj, COLOR_EX_BACKGROUND));
		_canvas_fillellipse(ps.hCanvas, hBrush,(FLOAT) ps.width / 2, (FLOAT)ps.height / 2, 75.f, 50.f);
		_brush_destroy(hBrush);

		_canvas_settransform(ps.hCanvas, 0);
		_matrix_destroy(mx);
		Ex_ObjEndPaint(hObj, &ps);
		*lpResult = 1;
		return 1;
	}
	else if (uMsg == WM_EX_EASING)
	{
		EX_EASINGINFO es{ 0 };
		RtlMoveMemory(&es, (void**)lParam, sizeof(EX_EASINGINFO));
		if (es.nProgress == 1)//如果进度=1则表示缓动结束
		{
			Ex_ObjSetLong(hObj, EOL_USERDATA, 0);//设置控件当前缓动指针为空
		}
		Ex_ObjSetLong(hObj, EOL_LPARAM, es.nCurrent);
		Ex_ObjInvalidateRect(hObj, 0);
	}
	else if (uMsg == WM_MOUSEHOVER)
	{
		HEXEASING hEasing = _easing_create(ET_OutElastic, 0, ES_SINGLE | ES_THREAD | ES_DISPATCHNOTIFY, hObj, 500, 20, EES_PLAY, 0, 100, 0, 0, 0, 0);
		HEXEASING oldhEasing =(HEXEASING) Ex_ObjSetLong(hObj, EOL_USERDATA, (size_t)hEasing);
		if (oldhEasing != 0)//如果前一个缓动未结束,则停止前面的缓动
		{
			_easing_setstate(oldhEasing, EES_STOP);
		}
		Ex_ObjSetUIState(hObj, STATE_HOVER, FALSE, 0,FALSE);//设置点燃状态
	}
	else if (uMsg == WM_MOUSELEAVE)
	{
		HEXEASING hEasing = _easing_create(ET_OutElastic, 0, ES_SINGLE | ES_THREAD | ES_DISPATCHNOTIFY | ES_REVERSE, hObj, 500, 20, EES_PLAY, 0, 100, 0, 0, 0, 0);
		HEXEASING oldhEasing = (HEXEASING)Ex_ObjSetLong(hObj, EOL_USERDATA, (size_t)hEasing);
		if (oldhEasing!=0)//如果前一个缓动未结束,则停止前面的缓动
		{
			_easing_setstate(oldhEasing, EES_STOP);
		}
		Ex_ObjSetUIState(hObj, STATE_HOVER, TRUE, 0, FALSE);//删除点燃状态
	}
	return 0;
}

void test_matrix(HWND hWnd)
{
	HWND hWnd_matrix = Ex_WndCreate(hWnd, L"Ex_DirectUI", L"测试矩阵", 0, 0, 400,400, 0, 0);
	HEXDUI hExDui_matrix = Ex_DUIBindWindowEx(hWnd_matrix, 0, EWS_NOINHERITBKG | EWS_MOVEABLE | EWS_CENTERWINDOW | EWS_NOSHADOW | EWS_BUTTON_CLOSE | EWS_TITLE | EWS_HASICON, 0, 0);
	Ex_DUISetLong(hExDui_matrix, EWL_CRBKG, ExARGB(150, 150, 150, 255));
	LPCWSTR ckass_static = L"static";
	HEXOBJ hObj = Ex_ObjCreateEx(EOS_EX_FOCUSABLE, ckass_static, L"fasdf", -1, 50, 50, 200, 250, hExDui_matrix, 0, DT_VCENTER, 0, 0, OnMatrixMsgProc);
	Ex_ObjSetColor(hObj, COLOR_EX_BACKGROUND, ExRGB2ARGB(255, 255), TRUE);

	Ex_DUIShowWindow(hExDui_matrix, SW_SHOWNORMAL, 0, 0, 0);
}

void test_buttonex(HWND hWnd)
{
	HWND hWnd_buttonex = Ex_WndCreate(hWnd, L"Ex_DirectUI", L"测试扩展按钮", 0, 0, 300, 300, 0, 0);
	HEXDUI hExDui_buttonex = Ex_DUIBindWindowEx(hWnd_buttonex, 0, EWS_NOINHERITBKG | EWS_MOVEABLE | EWS_CENTERWINDOW | EWS_NOSHADOW | EWS_BUTTON_CLOSE | EWS_TITLE | EWS_HASICON, 0, 0);
	Ex_DUISetLong(hExDui_buttonex, EWL_CRBKG, ExARGB(150, 150, 150, 255));
	LPCWSTR ATOM_ButtonEx = L"ButtonEx";
	HEXOBJ BtnExObj = Ex_ObjCreate(ATOM_ButtonEx, L"☏直角纯色按钮", -1, 50, 50, 100, 30, hExDui_buttonex);
	EXARGB ThemeColor = RGB(76, 175, 80);
	EX_OBJ_PROPS ButtonExprops = { 0 };
	ButtonExprops.COLOR_EX_BACKGROUND_NORMAL = ExRGB2ARGB(ThemeColor, 225);
	ButtonExprops.COLOR_EX_BACKGROUND_HOVER = ExRGB2ARGB(ThemeColor, 250);
	ButtonExprops.COLOR_EX_BACKGROUND_DOWNORCHECKED = ExRGB2ARGB(ThemeColor, 200);
	Ex_ObjSendMessage(BtnExObj, WM_EX_PROPS, 0, (size_t)&ButtonExprops);


	HEXOBJ BtnExObj2 = Ex_ObjCreate(ATOM_ButtonEx, L"点击进入 👉", -1, 50, 100, 100, 30, hExDui_buttonex);
	EX_OBJ_PROPS ButtonExprops2 = { 0 };
	ButtonExprops2.COLOR_EX_BACKGROUND_NORMAL = ExRGB2ARGB(7037666, 255);
	ButtonExprops2.COLOR_EX_BACKGROUND_HOVER = ExRGB2ARGB(6182117, 255);
	ButtonExprops2.COLOR_EX_BACKGROUND_DOWNORCHECKED = ExRGB2ARGB(4865258, 255);
	ButtonExprops2.Radius = 15;
	Ex_ObjSendMessage(BtnExObj2, WM_EX_PROPS, 0, (size_t)&ButtonExprops2);

	HEXOBJ BtnExObj3 = Ex_ObjCreate(ATOM_ButtonEx, L"Metro按钮☪", -1, 50, 150, 100, 30, hExDui_buttonex);
	EX_OBJ_PROPS ButtonExprops3 = { 0 };
	ButtonExprops3.COLOR_EX_BACKGROUND_NORMAL = ExARGB(130, 130, 130, 255);
	ButtonExprops3.COLOR_EX_BACKGROUND_HOVER = ExARGB(130, 130, 130, 235);
	ButtonExprops3.COLOR_EX_BACKGROUND_DOWNORCHECKED = ExARGB(200, 200, 200, 100);
	ButtonExprops3.COLOR_EX_BORDER_NORMAL = ExARGB(130, 130, 130, 255);
	ButtonExprops3.COLOR_EX_BORDER_HOVER = ExARGB(0, 0, 0, 135);
	ButtonExprops3.COLOR_EX_BORDER_DOWNORCHECKED = ExARGB(0, 0, 0, 150);
	ButtonExprops3.StrokeWidth = 2;
	Ex_ObjSendMessage(BtnExObj3, WM_EX_PROPS, 0, (LPARAM)&ButtonExprops3);

	HEXOBJ BtnExObj4 = Ex_ObjCreate(ATOM_ButtonEx, L"图标在左", -1, 50, 200, 100, 30, hExDui_buttonex);
	EX_OBJ_PROPS ButtonExprops4 = { 0 };
	ButtonExprops4.COLOR_EX_BACKGROUND_NORMAL = ExRGB2ARGB(10066176, 255);
	ButtonExprops4.COLOR_EX_BACKGROUND_HOVER = ExRGB2ARGB(10066176, 220);
	ButtonExprops4.COLOR_EX_BACKGROUND_DOWNORCHECKED = ExRGB2ARGB(10066176, 200);
	Ex_ObjSendMessage(BtnExObj4, WM_EX_PROPS, 0, (LPARAM)&ButtonExprops4);
	Ex_ObjSetFontFromFamily(BtnExObj4, L"楷体", 16, -1, TRUE);
	HEXIMAGE hImg;
	_img_createfromfile(L"buttonex\\5.png", &hImg);
	Ex_ObjSendMessage(BtnExObj4, WM_SETICON, 0, (LPARAM)hImg);   /* 设置图标; */

	EX_IMAGEINFO IMG0 = { 0 };
	_img_createfromfile(L"buttonex/4正常.png", &IMG0.IMG_NORMAL);//注意用完销毁
	_img_createfromfile(L"buttonex/4点燃.png", &IMG0.IMG_HOVER);
	_img_createfromfile(L"buttonex/4按下.png", &IMG0.IMG_DOWNORCHECKED);
	HEXOBJ BtnExObj5 = Ex_ObjCreate(ATOM_ButtonEx, NULL, -1, 50, 250, 100, 30, hExDui_buttonex);/*图片按钮*/
	Ex_ObjSendMessage(BtnExObj5, BM_SETIMAGE, 0, (LPARAM)&IMG0);

	HEXOBJ BtnExObj6 = Ex_ObjCreate(ATOM_ButtonEx, L"背景按钮", -1, 180, 50, 100, 30, hExDui_buttonex);
	Ex_ObjSetColor(BtnExObj6, COLOR_EX_TEXT_NORMAL, ExRGB2ARGB(65535, 180), FALSE);
	Ex_ObjSetColor(BtnExObj6, COLOR_EX_TEXT_HOVER, ExRGB2ARGB(65535, 255), FALSE);
	Ex_ObjSetColor(BtnExObj6, COLOR_EX_TEXT_DOWN, ExRGB2ARGB(65535, 255), FALSE);
	EX_IMAGEINFO IMG = { 0 };
	_img_createfromfile(L"buttonex/正常.png", &IMG.IMG_NORMAL);//注意用完销毁
	_img_createfromfile(L"buttonex/进入.png", &IMG.IMG_HOVER);
	_img_createfromfile(L"buttonex/按下.png", &IMG.IMG_DOWNORCHECKED);
	Ex_ObjSendMessage(BtnExObj6, BM_SETIMAGE, 0, (LPARAM)&IMG);

	HEXOBJ BtnExObj7 = Ex_ObjCreate(ATOM_ButtonEx, L"渐变按钮🔊", -1, 180, 100, 100, 30, hExDui_buttonex);
	EX_OBJ_PROPS ButtonExprops7 = { 0 };
	ButtonExprops7.COLOR_EX_BKG_CRBegin = ExARGB(0, 173, 241, 255);
	ButtonExprops7.COLOR_EX_BKG_CREnd = ExARGB(100, 25, 129, 255);
	ButtonExprops7.StrokeWidth = 2;
	Ex_ObjSendMessage(BtnExObj7, WM_EX_PROPS, 0, (LPARAM)&ButtonExprops7);
	Ex_ObjSetFontFromFamily(BtnExObj7, L"楷体", 16, -1, TRUE);

	HEXOBJ BtnExObj8 = Ex_ObjCreate(ATOM_ButtonEx, L"线框按钮", -1, 180, 150, 100, 30, hExDui_buttonex);/*图标按钮*/
	EX_OBJ_PROPS ButtonExprops8 = { 0 };
	ButtonExprops8.COLOR_EX_BRD_CRBegin = ExARGB(227, 34, 103, 255);
	ButtonExprops8.COLOR_EX_BRD_CREnd = ExRGB2ARGB(16746496, 255);
	ButtonExprops8.StrokeWidth = 2;
	Ex_ObjSendMessage(BtnExObj8, WM_EX_PROPS, 0, (LPARAM)&ButtonExprops8);
	Ex_ObjSetColor(BtnExObj8, COLOR_EX_TEXT_NORMAL, ExRGB2ARGB(65535, 180), FALSE);
	Ex_ObjSetColor(BtnExObj8, COLOR_EX_TEXT_HOVER, ExRGB2ARGB(65535, 255), FALSE);
	Ex_ObjSetColor(BtnExObj8, COLOR_EX_TEXT_DOWN, ExRGB2ARGB(65535, 255), FALSE);
	Ex_ObjSetFontFromFamily(BtnExObj8, L"楷体", 16, -1, TRUE);

	HEXOBJ BtnExObj9 = Ex_ObjCreate(ATOM_ButtonEx, L"图标在上", -1, 180, 200, 100, 40, hExDui_buttonex);
	EX_OBJ_PROPS ButtonExprops9 = { 0 };
	ButtonExprops9.COLOR_EX_BACKGROUND_NORMAL = ExRGB2ARGB(10061616, 255);
	ButtonExprops9.COLOR_EX_BACKGROUND_HOVER = ExRGB2ARGB(10061616, 220);
	ButtonExprops9.COLOR_EX_BACKGROUND_DOWNORCHECKED = ExRGB2ARGB(10061616, 200);
	ButtonExprops9.nIconPosition = 2;
	Ex_ObjSendMessage(BtnExObj9, WM_EX_PROPS, 0, (LPARAM)&ButtonExprops9);
	Ex_ObjSetFontFromFamily(BtnExObj9, L"楷体", 16, -1, TRUE);
	_img_createfromfile(L"buttonex\\4.png", &hImg);
	Ex_ObjSendMessage(BtnExObj9, WM_SETICON, 0, (LPARAM)hImg);   /* 设置图标; */

	HEXOBJ BtnExObj10 = Ex_ObjCreate(ATOM_ButtonEx, L"图标在右", -1, 180, 250, 100, 30, hExDui_buttonex);
	EX_OBJ_PROPS ButtonExprops10 = { 0 };
	ButtonExprops10.COLOR_EX_BACKGROUND_NORMAL = ExARGB(255, 255, 255, 50);
	ButtonExprops10.COLOR_EX_BACKGROUND_HOVER = ExARGB(255, 255, 255, 80);
	ButtonExprops10.COLOR_EX_BACKGROUND_DOWNORCHECKED = ExARGB(255, 255, 255, 100);
	ButtonExprops10.COLOR_EX_BORDER_NORMAL = ExARGB(0, 0, 0, 150);
	ButtonExprops10.COLOR_EX_BORDER_HOVER = ExARGB(0, 0, 0, 180);
	ButtonExprops10.COLOR_EX_BORDER_DOWNORCHECKED = ExARGB(0, 0, 0, 200);
	ButtonExprops10.StrokeWidth = 1;
	ButtonExprops10.nIconPosition = 1;
	ButtonExprops10.Radius = 7;
	Ex_ObjSendMessage(BtnExObj10, WM_EX_PROPS, 0, (LPARAM)&ButtonExprops10);
	Ex_ObjSetFontFromFamily(BtnExObj10, L"楷体", 16, -1, TRUE);
	_img_createfromfile(L"buttonex\\3.png", &hImg);
	Ex_ObjSendMessage(BtnExObj10, WM_SETICON, 0, (LPARAM)hImg);   /* 设置图标; */

	Ex_DUIShowWindow(hExDui_buttonex, SW_SHOWNORMAL, 0, 0, 0);
}

LRESULT CALLBACK OnEditChangeEvent(HEXOBJ hObj, INT nID, INT nCode, WPARAM wParam, LPARAM lParam)
{
	switch (nCode)
	{
	case EN_CHANGE:
	{
		output(L"编辑框内容改变", nID);
	}
	case NM_CHAR:
	{
		output(L"编辑框输入字符", nID, wParam);
	}
	default:
		break;
	}
	return 0;
}

void test_editex(HWND hWnd)
{
	HWND hWnd_editex = Ex_WndCreate(hWnd, L"Ex_DirectUI", L"测试扩展编辑框", 0, 0, 400, 200, 0, 0);
	HEXDUI hExDui_editex = Ex_DUIBindWindowEx(hWnd_editex, 0, EWS_NOINHERITBKG | EWS_MOVEABLE | EWS_CENTERWINDOW | EWS_NOSHADOW | EWS_BUTTON_CLOSE | EWS_TITLE | EWS_HASICON, 0, 0);
	Ex_DUISetLong(hExDui_editex, EWL_CRBKG, ExARGB(150, 150, 150, 255));

	HEXIMAGE hImage;
	LPCWSTR ATOM_EditEx = L"EditEx";
	EX_OBJ_PROPS EditExprops = { 0 };
	HEXOBJ m_UserNameEdit = Ex_ObjCreate(ATOM_EditEx, NULL, EOS_VISIBLE | EES_UNDERLINE, 30, 50, 150, 30, hExDui_editex);
	Ex_ObjSendMessage(m_UserNameEdit, EM_SETCUEBANNER, ExARGB(255, 255, 255, 100), (LPARAM)L"请输入账户名称");
	Ex_ObjSetColor(m_UserNameEdit, COLOR_EX_TEXT_NORMAL, ExARGB(255, 255, 255, 200), FALSE);
	_img_createfromfile(L"editex\\icon_contacts_normal.png", &hImage);
	Ex_ObjSendMessage(m_UserNameEdit, WM_SETICON, 0, (LPARAM)hImage);

	HEXOBJ m_PasswordEdit = Ex_ObjCreate(ATOM_EditEx, NULL, EOS_VISIBLE | EES_UNDERLINE | EES_USEPASSWORD, 30, 100, 150, 30, hExDui_editex);
	Ex_ObjSendMessage(m_PasswordEdit, EM_SETCUEBANNER, ExARGB(255, 255, 255, 100), (LPARAM)L"请输入账户密码");
	Ex_ObjSetColor(m_PasswordEdit, COLOR_EX_TEXT_NORMAL, ExARGB(255, 255, 255, 200), FALSE);
	_img_createfromfile(L"editex\\psw_normal.png", &hImage);
	Ex_ObjSendMessage(m_PasswordEdit, WM_SETICON, 0, (LPARAM)hImage);


	HEXOBJ m_SearchEdit = Ex_ObjCreate(ATOM_EditEx, NULL, -1, 200, 50, 150, 30, hExDui_editex);
	Ex_ObjSendMessage(m_SearchEdit, EM_SETCUEBANNER, ExARGB(255, 255, 255, 100), (LPARAM)L"搜索一下");
	Ex_ObjSetColor(m_SearchEdit, COLOR_EX_TEXT_NORMAL, ExARGB(255, 255, 255, 200), FALSE);
	EditExprops.COLOR_EX_BACKGROUND_NORMAL = ExARGB(20, 20, 120, 150);
	EditExprops.COLOR_EX_BORDER_HOVER = ExARGB(255, 77, 77, 150);
	EditExprops.COLOR_EX_BORDER_DOWNORCHECKED = ExARGB(255, 77, 77, 200);
	EditExprops.Radius = 15;
	EditExprops.StrokeWidth = 1;
	EditExprops.nIconPosition = 1;
	Ex_ObjSendMessage(m_SearchEdit, WM_EX_PROPS, 0, (LPARAM)&EditExprops);
	_img_createfromfile(L"editex\\search_normal.png", &hImage);
	Ex_ObjSendMessage(m_SearchEdit, WM_SETICON, 0, (LPARAM)hImage);

	HEXOBJ m_SearchEdit2 = Ex_ObjCreate(ATOM_EditEx, NULL, -1, 200, 100, 150, 30, hExDui_editex);
	Ex_ObjSendMessage(m_SearchEdit2, EM_SETCUEBANNER, ExARGB(255, 255, 255, 100), (LPARAM)L"搜索一下");
	_img_createfromfile(L"editex\\search_normal.png", &hImage);
	Ex_ObjSendMessage(m_SearchEdit2, WM_SETICON, 0, (LPARAM)hImage);   /* 设置图标; */
	Ex_ObjSetColor(m_SearchEdit2, COLOR_EX_TEXT_NORMAL, ExARGB(255, 255, 255, 200), FALSE);

	Ex_ObjHandleEvent(m_SearchEdit2, EN_CHANGE, OnEditChangeEvent);
	Ex_ObjHandleEvent(m_SearchEdit2, NM_CHAR, OnEditChangeEvent);

	Ex_DUIShowWindow(hExDui_editex, SW_SHOWNORMAL, 0, 0, 0);
}

HMENU m_hMenu;

LRESULT CALLBACK OnMenuBtnMsgProc(HWND hWnd, HEXOBJ hObj, INT uMsg, WPARAM wParam, LPARAM lParam, LRESULT* lpResult)
{
	if (uMsg == WM_PAINT)
	{
		EX_PAINTSTRUCT2 ps{ 0 };
		Ex_ObjBeginPaint(hObj, &ps);
		if ((ps.dwState & STATE_DOWN) != 0)
		{
			_canvas_clear(ps.hCanvas, ExRGB2ARGB(0, 50));
		}
		else if ((ps.dwState & STATE_HOVER) != 0)
		{
			_canvas_clear(ps.hCanvas, ExRGB2ARGB(0, 20));
		}
		else {
			_canvas_clear(ps.hCanvas, ExRGB2ARGB(0, 0));
		}
		FLOAT nWidthText = 0;
		FLOAT nHeightText = 0;
		_canvas_calctextsize(ps.hCanvas, Ex_ObjGetFont(hObj), (LPCWSTR)Ex_ObjGetLong(hObj, EOL_LPWZTITLE), -1, ps.dwTextFormat, 0, ps.width, ps.height, &nWidthText, &nHeightText);
		HEXIMAGE hImg = Ex_ObjGetLong(hObj, EOL_LPARAM);
		INT nWidthIcon = 0;
		INT nHeightIcon = 0;
		if (hImg != 0)
		{
			_img_getsize(hImg, &nWidthIcon, &nHeightIcon);
		}
		if (hImg != 0)
		{
			_canvas_drawimage(ps.hCanvas, hImg, (ps.width - nWidthIcon) / 2, (ps.height - nHeightIcon - nHeightText - 3) / 2, 255);
		}
		_canvas_drawtext(ps.hCanvas, Ex_ObjGetFont(hObj), Ex_ObjGetColor(hObj, COLOR_EX_TEXT_NORMAL), (LPCWSTR)Ex_ObjGetLong(hObj, EOL_LPWZTITLE), -1, ps.dwTextFormat | DT_CENTER | DT_VCENTER, (ps.width - nWidthText) / 2, (ps.height - nHeightIcon - nHeightText - 3) / 2 + nHeightIcon + 3, (ps.width + nWidthText) / 2, (ps.height - nHeightIcon - nHeightText - 3) / 2 + nHeightIcon + 3 + nHeightText);

		Ex_ObjEndPaint(hObj, &ps);
		*lpResult = 1;
		return 1;

	}
	else if (uMsg == WM_EX_LCLICK)
	{
		EndMenu();
		*lpResult = 1;
		return 1;
	}
	return 0;
}

LRESULT CALLBACK OnMenuItemMsgProc(HWND hWnd, HEXOBJ hObj, INT uMsg, WPARAM wParam, LPARAM lParam, LRESULT* lpResult)
{
	if (uMsg == WM_ERASEBKGND)
	{
		if (__get((LPVOID)lParam, 0) == wParam)
		{
			EX_PAINTSTRUCT2 ps{ 0 };
			RtlMoveMemory(&ps, (LPVOID)lParam, sizeof(EX_PAINTSTRUCT2));
			if (ps.height > 10)
			{
				if ((ps.dwState & STATE_HOVER) != 0)
				{
					_canvas_clear(ps.hCanvas, ExRGB2ARGB(16711680, 100));
				}
				else {
					_canvas_clear(ps.hCanvas, 0);
				}
				*lpResult = 1;
				return 1;
			}
		}
	}
	else if (uMsg == WM_EX_LCLICK)
	{
		Ex_ObjGetLong(hObj, EOL_ID);
	}
	return 0;
}

LRESULT CALLBACK OnMenuWndMsgProc(HWND hWnd, HEXDUI hExDUI, INT uMsg, WPARAM wParam, LPARAM lParam, LRESULT* lpResult)
{
	if (uMsg == WM_INITMENUPOPUP)
	{
		RECT rc{ 0 };
		if (wParam == (size_t)m_hMenu)
		{
			size_t value = 1;
			SetPropW(hWnd, L"IsMainMenu", (HANDLE)value);
			GetWindowRect(hWnd, &rc);
			SetWindowPos(hWnd, 0, 0, 0, rc.right - rc.left + 10, rc.bottom - rc.top + 10 + 108, SWP_NOMOVE | SWP_NOZORDER | SWP_NOACTIVATE);

			rc.right = rc.right - rc.left - 10;
			rc.bottom = rc.bottom - rc.top - 10 + 108;
			rc.left = 6;
			rc.top = 40;

			//创建顶部按钮
			HEXIMAGE hImg;
			_img_createfromfile(L"custommenu/btn1.png", &hImg);
			Ex_ObjCreateEx(-1, L"button", L"消息", EOS_VISIBLE, rc.left, rc.top, rc.right * 0.333, 70, hExDUI, -1, -1, hImg, 0, OnMenuBtnMsgProc);

			_img_createfromfile(L"custommenu/btn2.png", &hImg);
			Ex_ObjCreateEx(-1, L"button", L"收藏", EOS_VISIBLE, rc.left + rc.right * 0.333, rc.top, rc.right * 0.333, 70, hExDUI, -2, -1, hImg, 0, OnMenuBtnMsgProc);

			_img_createfromfile(L"custommenu/btn3.png", &hImg);
			Ex_ObjCreateEx(-1, L"button", L"文件", EOS_VISIBLE, rc.left + rc.right * 0.666, rc.top, rc.right * 0.333, 70, hExDUI, -3, -1, hImg, 0, OnMenuBtnMsgProc);

			HEXOBJ hObj = Ex_ObjCreateEx(EOS_EX_TRANSPARENT | EOS_EX_TOPMOST, L"Static", 0, EOS_VISIBLE, 0, 0, 45, 38, hExDUI, 0, -1, 0, 0, 0);
			std::vector<CHAR> data;
			Ex_ReadFile(L"custommenu/Icon.png", &data);
			Ex_ObjSetBackgroundImage(hObj, data.data(), data.size(), 0, 0, BIR_NO_REPEAT, 0, 0, 255, TRUE);
			rc.top = rc.top + 75;
			rc.bottom = rc.bottom - 75;

		}
		else {
			size_t value = 0;
			SetPropW(hWnd, L"IsMainMenu", (HANDLE)value);

			GetWindowRect(hWnd, &rc);
			SetWindowPos(hWnd, 0, 0, 0, rc.right - rc.left + 10, rc.bottom - rc.top + 10, SWP_NOMOVE | SWP_NOZORDER | SWP_NOACTIVATE);
			rc.right = rc.right - rc.left - 10;
			rc.bottom = rc.bottom - rc.top - 10;
			rc.left = 6;
			rc.top = 8;

		}
		HEXOBJ hObjfind = Ex_ObjFind(hExDUI, 0, L"Item", 0);
		INT t = rc.top;
		RECT rcObj{ 0 };
		while (hObjfind != 0)
		{
			Ex_ObjGetRect(hObjfind, &rcObj);
			Ex_ObjMove(hObjfind, rc.left, t, rc.right, rcObj.bottom - rcObj.top, TRUE);
			Ex_ObjSetColor(hObjfind, COLOR_EX_TEXT_NORMAL, ExRGB2ARGB(0, 255), TRUE);
			Ex_ObjSetLong(hObjfind, EOL_OBJPROC, (size_t)OnMenuItemMsgProc);
			t = t + rcObj.bottom - rcObj.top;
			hObjfind = Ex_ObjGetObj(hObjfind, GW_HWNDNEXT);
		}
	}
	else if (uMsg == WM_ERASEBKGND)
	{
		RECT rc{ 0 };
		GetWindowRect(hWnd, &rc);
		_canvas_clear(wParam, 0);
		HEXIMAGE hImg;
		if (GetPropW(hWnd, L"IsMainMenu") != 0)
		{
			_img_createfromfile(L"custommenu/Main.png", &hImg);
			_canvas_drawimagefromgrid(wParam, hImg, 0, 0, rc.right - rc.left - 5, rc.bottom - rc.top - 5, 0, 0, 68, 68, 46, 42, 13, 12, 0, 230);
		}
		else {
			_img_createfromfile(L"custommenu/Sub.png", &hImg);
			_canvas_drawimagefromgrid(wParam, hImg, 0, 0, rc.right - rc.left - 5, rc.bottom - rc.top - 5, 0, 0, 24, 24, 8, 9, 10, 10, 0, 230);
		}
		_img_destroy(hImg);
		*lpResult = 1;
		return 1;
	}
	return 0;
}



LRESULT CALLBACK OnMenuButtonEvent(HEXOBJ hObj, INT nID, INT nCode, WPARAM wParam, LPARAM lParam)
{
	POINT pt;
	GetCursorPos(&pt);
	Ex_TrackPopupMenu(m_hMenu, 0, pt.x, pt.y, 0, hObj, 0, OnMenuWndMsgProc, EMNF_NOSHADOW);
	return 0;
}


void test_custommenu(HWND hWnd)
{
	HWND hWnd_custommenu = Ex_WndCreate(hWnd, L"Ex_DirectUI", L"测试扩展菜单", 0, 0, 300, 200, 0, 0);
	HEXDUI hExDui_custommenu = Ex_DUIBindWindowEx(hWnd_custommenu, 0, EWS_NOINHERITBKG | EWS_MOVEABLE | EWS_CENTERWINDOW | EWS_NOSHADOW | EWS_BUTTON_CLOSE | EWS_TITLE | EWS_HASICON, 0, 0);
	Ex_DUISetLong(hExDui_custommenu, EWL_CRBKG, ExARGB(150, 150, 150, 255));

	HEXOBJ m_button = Ex_ObjCreate(L"button", L"弹出菜单", -1, 50, 50, 100, 30, hExDui_custommenu);
	Ex_ObjHandleEvent(m_button, NM_CLICK, OnMenuButtonEvent);
	m_hMenu = CreatePopupMenu();
	AppendMenuW(m_hMenu, MF_STRING | MF_ENABLED, 301, L"Item 1");
	CheckMenuItem(m_hMenu, 301, MF_BYCOMMAND | MF_CHECKED);
	AppendMenuW(m_hMenu, MF_STRING | MF_ENABLED, 302, L"Disabled Item");
	AppendMenuW(m_hMenu, MF_SEPARATOR, 666, L"Disabled Item");
	AppendMenuW(m_hMenu, MF_SEPARATOR, 0, L"Disabled Item");
	DeleteMenu(m_hMenu, 666, MF_BYCOMMAND);

	//创建一个新菜单
	HMENU hMenu = CreateMenu();
	AppendMenuW(hMenu, MF_STRING | MF_ENABLED, 3001, L"SubItem 1");//添加项目
	AppendMenuW(m_hMenu, MF_POPUP, (UINT_PTR)hMenu, L"More");
	EnableMenuItem(m_hMenu, 302, MF_DISABLED);

	Ex_DUIShowWindow(hExDui_custommenu, SW_SHOWNORMAL, 0, 0, 0);
}

LRESULT CALLBACK OnSideButtonMsgProc(HWND hWnd, HEXOBJ hObj, INT uMsg, WPARAM wParam, LPARAM lParam, LRESULT* lpResult)
{
	if (uMsg == WM_NOTIFY)
	{
		if (wParam == 1001)//wParam表示事件对象的ID，1001最里层按钮
		{
			EX_NMHDR ni{ 0 };
			RtlMoveMemory(&ni, (LPVOID)lParam, sizeof(EX_NMHDR));
			if (ni.nCode == NM_CLICK)
			{
				std::wstring str;
				str.resize(512);
				Ex_ObjGetText(hObj, str.c_str(), 512);
				auto title = str + L"收到了单击事件";
				Ex_MessageBox(hObj, title.c_str(), 0, 0, 0);
			}
		}
	}
	return 0;
}

LRESULT CALLBACK OnParentButtonMsgProc(HWND hWnd, HEXOBJ hObj, INT uMsg, WPARAM wParam, LPARAM lParam, LRESULT* lpResult)
{
	if (uMsg == WM_NOTIFY)
	{
		if (wParam == 1001)//wParam表示事件对象的ID，1001最里层按钮
		{
			EX_NMHDR ni{ 0 };
			RtlMoveMemory(&ni, (LPVOID)lParam, sizeof(EX_NMHDR));
			if (ni.nCode == NM_CLICK)
			{
				if (Ex_MessageBox(hObj, L"按钮直接父控件收到了单击事件，是否拦截？", 0, MB_YESNO, 0) == IDYES)//选择是
				{
					*lpResult = 1;//返回值置1(真)表示拦截
					return 1;//函数返回值表示是否使用返回值,lpResult才是真正的返回值
				}
			}
		}
	}
	return 0;
}

LRESULT CALLBACK OnEventButtonMsgProc(HWND hWnd, HEXOBJ hObj, INT uMsg, WPARAM wParam, LPARAM lParam, LRESULT* lpResult)
{
	if (uMsg == WM_EX_LCLICK)//左键单击消息,拦截这条则不会触发任何事件
	{
		Ex_MessageBox(hObj, L"按钮收到了单击消息", 0, 0, 0);
	}
	else if (uMsg == WM_NOTIFY)
	{
		EX_NMHDR ni{ 0 };
		RtlMoveMemory(&ni, (LPVOID)lParam, sizeof(EX_NMHDR));
		if (ni.hObjFrom == hObj)
		{
			if (ni.nCode == NM_CLICK)
			{
				Ex_MessageBox(hObj, L"按钮收到了单击事件", 0, 0, 0);
			}
		}
	}
	return 0;
}

LRESULT CALLBACK OnEventButtonEvent(HEXOBJ hObj, INT nID, INT nCode, WPARAM wParam, LPARAM lParam)
{
	if (nCode == NM_CLICK)
	{
		Ex_MessageBox(hObj, L"按钮收到了独立的单击事件", 0, 0, 0);
	}
	return 0;
}

LRESULT CALLBACK OnEventWndMsgProc(HWND hWnd, HEXDUI hExDui, INT uMsg, WPARAM wParam, LPARAM lParam, LRESULT* lpResult)
{
	if (uMsg == WM_NOTIFY)
	{
		if (wParam == 1001)//wParam表示事件对象的ID，1001最里层按钮
		{
			EX_NMHDR ni{ 0 };
			RtlMoveMemory(&ni, (LPVOID)lParam, sizeof(EX_NMHDR));
			if (ni.nCode == NM_CLICK)
			{
				Ex_MessageBox(hExDui, L"按钮所属窗口收到了单击事件", 0, 0, 0);
			}
		}
	}
	return 0;
}

void test_event(HWND hWnd)
{
	HWND hWnd_event = Ex_WndCreate(hWnd, L"Ex_DirectUI", L"测试事件分发", 0, 0, 400, 300, 0, 0);
	HEXDUI hExDui_event = Ex_DUIBindWindowEx(hWnd_event, 0, EWS_NOINHERITBKG | EWS_MOVEABLE | EWS_CENTERWINDOW | EWS_NOSHADOW | EWS_BUTTON_CLOSE | EWS_TITLE | EWS_HASICON, 0, OnEventWndMsgProc);
	Ex_DUISetLong(hExDui_event, EWL_CRBKG, ExARGB(150, 150, 150, 255));

	/*消息(WM_/XXM_)分发的流程是：
	控件产生消息->控件类回调函数->控件回调函数->默认控件回调函数(在此部分消息会产生事件)
	事件(NM_/XXN_)分发的流程是：
	控件产生事件->JS回调函数->控件自身收到WM_NOTIFY->控件直接父控件收到WM_NOTIFY->控件间接父控件从内而外逐层收到WM_NOTIFY消息(需启用消息冒泡)->所属窗口收到WM_NOTIFY消息
	消息和事件在分发途中均可拦截,HandEvent*/

	HEXOBJ button_outside = Ex_ObjCreateEx(-1, L"Static", L"按钮外间接父控件", -1, 25, 35, 350, 250, hExDui_event, 0, DT_SINGLELINE, 0, 0, OnSideButtonMsgProc);
	if (button_outside != 0)
	{
		Ex_ObjSetColor(button_outside, COLOR_EX_BACKGROUND, ExRGB2ARGB(65535, 100), TRUE);
		HEXOBJ button_inside = Ex_ObjCreateEx(-1, L"Static", L"按钮内间接父控件", -1, 10, 20, 330, 200, button_outside, 0, DT_SINGLELINE, 0, 0, OnSideButtonMsgProc);
		if (button_inside != 0)
		{
			Ex_ObjSetColor(button_inside, COLOR_EX_BACKGROUND, ExRGB2ARGB(16777215, 100), TRUE);
			HEXOBJ button_parent = Ex_ObjCreateEx(-1, L"Static", L"按钮直接父控件", -1, 10, 20, 310, 160, button_inside, 0, DT_SINGLELINE, 0, 0, OnParentButtonMsgProc);
			if (button_parent != 0)
			{
				Ex_ObjSetColor(button_parent, COLOR_EX_BACKGROUND, ExRGB2ARGB(16711680, 100), TRUE);
				HEXOBJ button = Ex_ObjCreateEx(-1, L"button", L"按钮", -1, 10, 20, 200, 100, button_parent, 1001, -1, 0, 0, OnEventButtonMsgProc);
				if (button != 0)
				{
					Ex_ObjHandleEvent(button, NM_CLICK, OnEventButtonEvent);
					Ex_ObjEnableEventBubble(button, TRUE);// 启用控件的事件冒泡，事件冒泡是指事件将根据父控件逐层传递至窗口
				}
			}
		}
	}
	Ex_DUIShowWindow(hExDui_event, SW_SHOWNORMAL, 0, 0, 0);
}


void test_loading(HWND hWnd)
{
	HWND hWnd_loading = Ex_WndCreate(hWnd, L"Ex_DirectUI", L"测试加载动画", 0, 0, 300, 250, 0, 0);
	HEXDUI hExDui_loading = Ex_DUIBindWindowEx(hWnd_loading, 0, EWS_NOINHERITBKG | EWS_MOVEABLE | EWS_CENTERWINDOW | EWS_NOSHADOW | EWS_BUTTON_CLOSE | EWS_TITLE | EWS_HASICON, 0, 0);
	Ex_DUISetLong(hExDui_loading, EWL_CRBKG, ExARGB(150, 150, 150, 255));
	WCHAR ATOM_Mosaic_Loading[] = L"MosaicLoading";/*马赛克加载动画*/
	HEXOBJ	Mosaic_LoadingObj = Ex_ObjCreate(ATOM_Mosaic_Loading, L"MosaicLoading", -1, 50, 50, 50, 50, hExDui_loading);
	Ex_ObjSetColor(Mosaic_LoadingObj, COLOR_EX_BACKGROUND, RGB(255, 10, 10), TRUE);/*动画点颜色，只能提供RGB色*/

	WCHAR ATOM_Win10_Loading[] = L"Win10Loading";/*win10加载动画*/
	/*默认风格为圆形转圈*/
	HEXOBJ Win10_LoadingObj = Ex_ObjCreate(ATOM_Win10_Loading, L"加载中，请稍后", -1, 150, 30, 80, 80, hExDui_loading);
	HEXOBJ Win10_LoadingObj2 = Ex_ObjCreate(ATOM_Win10_Loading, L"正在读取数据，请稍后", EOS_VISIBLE | ELDS_LINE, 50, 150, 200, 60, hExDui_loading);
	Ex_ObjSetColor(Win10_LoadingObj2, COLOR_EX_BACKGROUND, ExRGB(0, 136, 255), TRUE);/*动画点颜色，只能提供RGB色*/

	Ex_DUIShowWindow(hExDui_loading, SW_SHOWNORMAL, 0, 0, 0);
}


HEXOBJ	m_hObj_soliderbar;

LRESULT CALLBACK OnSoliderBarPosChangeEvent(HEXOBJ hObj, INT nID, INT nCode, WPARAM wParam, LPARAM lParam)
{
	std::wstring t = L"我是滑块条,你可以尝试拖动我喔,当前值是：" + std::to_wstring(lParam);
	Ex_ObjSetText(m_hObj_soliderbar, t.c_str(), TRUE);
	return 0;
}

void test_soliderbar(HWND hWnd)
{
	HWND hWnd_soliderbar = Ex_WndCreate(hWnd, L"Ex_DirectUI", L"测试滑块条", 0, 0, 400, 200, 0, 0);
	HEXDUI hExDui_soliderbar = Ex_DUIBindWindowEx(hWnd_soliderbar, 0, EWS_NOINHERITBKG | EWS_MOVEABLE | EWS_CENTERWINDOW | EWS_NOSHADOW | EWS_BUTTON_CLOSE | EWS_TITLE | EWS_HASICON, 0, 0);
	Ex_DUISetLong(hExDui_soliderbar, EWL_CRBKG, ExARGB(150, 150, 150, 255));

	WCHAR ATOM_SoliderBarEx[] = L"SoliderBarEx";	/*滑块条*/
	HEXOBJ m_SoliderBarEx = Ex_ObjCreate(ATOM_SoliderBarEx, NULL, -1, 80, 50, 250, 20, hExDui_soliderbar);

	Ex_ObjSetColor(m_SoliderBarEx, COLOR_EX_BACKGROUND, ExARGB(0, 136, 255, 250), TRUE);
	Ex_ObjHandleEvent(m_SoliderBarEx, SBN_VALUE, OnSoliderBarPosChangeEvent);

	HEXOBJ m_SoliderBarEx2 = Ex_ObjCreate(ATOM_SoliderBarEx, NULL, EOS_VISIBLE | ESBS_VERTICAL, 50, 60, 20, 100, hExDui_soliderbar);
	Ex_ObjSetColor(m_SoliderBarEx2, COLOR_EX_BACKGROUND, ExARGB(100, 136, 255, 250), TRUE);
	Ex_ObjHandleEvent(m_SoliderBarEx2, SBN_VALUE, OnSoliderBarPosChangeEvent);
	Ex_ObjSetLong(m_SoliderBarEx2, SBL_BLOCK_POINT, 1);

	HEXOBJ m_SoliderBarEx3 = Ex_ObjCreate(ATOM_SoliderBarEx, NULL, EOS_VISIBLE | ESBS_VERTICAL, 350, 60, 20, 100, hExDui_soliderbar);
	Ex_ObjSetColor(m_SoliderBarEx3, COLOR_EX_BACKGROUND, ExARGB(100, 136, 255, 250), TRUE);
	Ex_ObjHandleEvent(m_SoliderBarEx3, SBN_VALUE, OnSoliderBarPosChangeEvent);
	Ex_ObjSetColor(m_SoliderBarEx3, COLOR_EX_TEXT_NORMAL, ExARGB(255, 255, 255, 125), TRUE);/*滑块条底色*/
	Ex_ObjSetColor(m_SoliderBarEx3, COLOR_EX_TEXT_CHECKED, ExARGB(200, 16, 25, 250), TRUE);/*滑块条走过的颜色*/

	HEXOBJ m_SoliderBarEx4 = Ex_ObjCreate(ATOM_SoliderBarEx, NULL, -1, 80, 150, 250, 20, hExDui_soliderbar);
	Ex_ObjSetColor(m_SoliderBarEx4, COLOR_EX_BACKGROUND, ExARGB(0, 136, 255, 250), TRUE);
	Ex_ObjHandleEvent(m_SoliderBarEx4, SBN_VALUE, OnSoliderBarPosChangeEvent);
	Ex_ObjSetLong(m_SoliderBarEx4, SBL_BLOCK_POINT, 1);

	m_hObj_soliderbar = Ex_ObjCreate(L"static", L"我是滑块条,你可以尝试拖动我喔,当前值是：", -1, 80, 80, 300, 30, hExDui_soliderbar);

	Ex_DUIShowWindow(hExDui_soliderbar, SW_SHOWNORMAL, 0, 0, 0);
}

void test_rotateimgbox(HWND hWnd)
{
	HWND hWnd_rotateimgbox = Ex_WndCreate(hWnd, L"Ex_DirectUI", L"测试旋转图片框", 0, 0, 200, 200, 0, 0);
	HEXDUI hExDui_rotateimgbox = Ex_DUIBindWindowEx(hWnd_rotateimgbox, 0, EWS_NOINHERITBKG | EWS_MOVEABLE | EWS_CENTERWINDOW | EWS_NOSHADOW | EWS_BUTTON_CLOSE | EWS_TITLE | EWS_HASICON, 0, 0);
	Ex_DUISetLong(hExDui_rotateimgbox, EWL_CRBKG, ExARGB(150, 150, 150, 255));
	WCHAR	ImageBoxCls[] = L"RotateImageBox";
	HEXOBJ	ImageBoxobj = Ex_ObjCreateEx(EOS_EX_FOCUSABLE, ImageBoxCls, NULL, EOS_VISIBLE | ERIBS_ROTATE, 50, 50, 100, 100, hExDui_rotateimgbox, 0, -1, 0, 0, 0);

	std::vector<CHAR> imgdata;
	Ex_ReadFile(L"res/rotateimgbox.jpg", &imgdata);
	Ex_ObjSetBackgroundImage(ImageBoxobj, imgdata.data(), imgdata.size(), 0, 0, BIR_DEFAULT, 0, BIF_PLAYIMAGE, 255, TRUE);
	Ex_ObjSetRadius(ImageBoxobj, 50, 50, 50, 50, TRUE);

	Ex_DUIShowWindow(hExDui_rotateimgbox, SW_SHOWNORMAL, 0, 0, 0);
}

LRESULT CALLBACK OnDragMsgProc(HWND hWnd, HEXOBJ hObj, INT uMsg, WPARAM wParam, LPARAM lParam, LRESULT* lpResult)
{
	if (uMsg == WM_LBUTTONDOWN)
	{
		Ex_ObjSetLong(hObj, EOL_USERDATA, lParam);
		Ex_ObjSetUIState(hObj, STATE_DOWN, FALSE, 0, FALSE);
	}
	else if (uMsg == WM_LBUTTONUP)
	{
		Ex_ObjSetLong(hObj, EOL_USERDATA, 0);
		Ex_ObjSetUIState(hObj, STATE_DOWN, TRUE, 0, FALSE);
	}
	else if (uMsg == WM_MOUSEMOVE)
	{
		if ((Ex_ObjGetUIState(hObj) & STATE_DOWN) != 0)
		{
			auto userdata = Ex_ObjGetLong(hObj, EOL_USERDATA);
			//获取按下位置
			POINT ptOrg;
			ptOrg.x = LOWORD(userdata);
			ptOrg.y = HIWORD(userdata);
			//获取当前鼠标位置
			POINT pt;
			GetCursorPos(&pt);
			ScreenToClient(hWnd, &pt);
			auto parent = Ex_ObjGetParent(hObj);
			RECT rcParent{ 0 };
			//获取组件矩形
			RECT rcObj{ 0 };
			Ex_ObjGetRect(hObj, &rcObj);

			if (parent != 0)
			{		
				Ex_ObjGetRect(parent, &rcParent);
				if (pt.x - ptOrg.x > rcParent.left && pt.x - ptOrg.x + rcObj.right - rcObj.left<rcParent.right && pt.y - ptOrg.y>rcParent.top && pt.y - ptOrg.y + rcObj.bottom - rcObj.top < rcParent.bottom)
				{
					Ex_ObjSetPos(hObj, 0, pt.x - ptOrg.x - rcParent.left, pt.y - ptOrg.y - rcParent.top, 0, 0, SWP_NOSIZE | SWP_NOZORDER | SWP_NOACTIVATE);
				}
			}
			else {
				HEXDUI hexdui = Ex_DUIFromWindow(hWnd);
				Ex_DUIGetClientRect(hexdui, &rcParent);
				if (pt.x - ptOrg.x > 0 && pt.x - ptOrg.x + rcObj.right - rcObj.left < rcParent.right && pt.y - ptOrg.y>0 && pt.y - ptOrg.y + rcObj.bottom - rcObj.top < rcParent.bottom)
				{
					Ex_ObjSetPos(hObj, 0, pt.x - ptOrg.x, pt.y - ptOrg.y, 0, 0, SWP_NOSIZE | SWP_NOZORDER | SWP_NOACTIVATE);
				}
			}
		}
	}
	return 0;
}

void test_dragobj(HWND hWnd)
{
	HWND hWnd_dragobj = Ex_WndCreate(hWnd, L"Ex_DirectUI", L"测试拖动组件", 0, 0, 500, 500, 0, 0);
	HEXDUI hExDui_dragobj = Ex_DUIBindWindowEx(hWnd_dragobj, 0, EWS_NOINHERITBKG | EWS_MOVEABLE | EWS_CENTERWINDOW | EWS_NOSHADOW | EWS_BUTTON_CLOSE | EWS_TITLE | EWS_HASICON, 0, 0);
	Ex_DUISetLong(hExDui_dragobj, EWL_CRBKG, ExARGB(150, 150, 150, 255));
	WCHAR	Cls[] = L"static";
	WCHAR	title[] = L"我可以拖动";
	HEXOBJ	hObj1 = Ex_ObjCreateEx(EOS_EX_FOCUSABLE, Cls, title, EOS_VISIBLE | EOS_BORDER, 25, 35, 250, 250, hExDui_dragobj, 0, DT_SINGLELINE | DT_VCENTER | DT_CENTER, 0, 0, OnDragMsgProc);
	if (hObj1 != 0)
	{
		Ex_ObjSetColor(hObj1, COLOR_EX_BACKGROUND, ExRGB2ARGB(255, 100), TRUE);
		HEXOBJ	hObj2 = Ex_ObjCreateEx(EOS_EX_FOCUSABLE, Cls, title, EOS_VISIBLE | EOS_BORDER, 25, 35, 150, 150, hObj1, 0, DT_SINGLELINE | DT_VCENTER | DT_CENTER, 0, 0, OnDragMsgProc);
		if (hObj2 != 0)
		{
			Ex_ObjSetColor(hObj2, COLOR_EX_BACKGROUND, ExRGB2ARGB(16722680, 100), TRUE);
		}
	}
	Ex_DUIShowWindow(hExDui_dragobj, SW_SHOWNORMAL, 0, 0, 0);
}


LRESULT CALLBACK OnDropObjDataMsgProc(HWND hWnd, HEXOBJ hObj, INT uMsg, WPARAM wParam, LPARAM lParam, LRESULT* lpResult)
{
	if (uMsg == WM_EX_DROP)//先触发本消息
	{
		EX_DROPINFO di{ 0 };
		RtlMoveMemory(&di, (LPVOID)lParam, sizeof(EX_DROPINFO));
		if (Ex_ObjCheckDropFormat(hObj, di.pDataObject, CF_UNICODETEXT) || Ex_ObjCheckDropFormat(hObj, di.pDataObject, CF_TEXT))//判断是不是文本
		{
			INT len = Ex_ObjGetDropString(hObj, di.pDataObject, 0, 0);
			if (len > 0)
			{
				std::wstring str;
				str.resize(len * 2 + 2);
				Ex_ObjGetDropString(hObj, di.pDataObject, (LPWSTR)str.c_str(), len + 1);
				output(L"接收到文本拖拽:", str);
				*lpResult = DROPEFFECT_COPY;//按复制模式处理
				return 1;
			}
		}
	}
	else if (uMsg == WM_DROPFILES)//若上面未处理,且控件拥有#EOS_EX_ACCEPTFILES风格,则继续触发本消息
	{
		UINT fileNumber = DragQueryFileW((HDROP)wParam, 0xFFFFFFFF, NULL, 0);
		for (int index = 0; index < fileNumber; index++)
		{
			UINT fileNameLength = DragQueryFileW((HDROP)wParam, index, NULL, 0);
			if (fileNameLength)
			{
				std::wstring fileName;
				fileName.resize(fileNameLength);
				DragQueryFileW((HDROP)wParam, index, (LPWSTR)fileName.c_str(), fileNameLength + 2);
				output(L"接收到文件拖拽:",fileName);
			}
		}
		*lpResult = DROPEFFECT_LINK;//按建立快捷方式处理
		return 1;
	}
	return 0;
}

void test_dropobj(HWND hWnd)
{
	HWND hWnd_dropobj = Ex_WndCreate(hWnd, L"Ex_DirectUI", L"测试接收拖曳信息", 0, 0, 300, 300, 0, 0);
	HEXDUI hExDui_dropobj = Ex_DUIBindWindowEx(hWnd_dropobj, 0, EWS_NOINHERITBKG | EWS_MOVEABLE | EWS_CENTERWINDOW | EWS_NOSHADOW | EWS_BUTTON_CLOSE | EWS_TITLE | EWS_HASICON , 0, 0);
	Ex_DUISetLong(hExDui_dropobj, EWL_CRBKG, ExARGB(150, 150, 150, 255));
	WCHAR	Cls[] = L"static";
	WCHAR	title[] = L"拖拽文本、文件到这里";
	HEXOBJ	hObj1 = Ex_ObjCreateEx(EOS_EX_DRAGDROP | EOS_EX_ACCEPTFILES, Cls, title, EOS_VISIBLE | EOS_BORDER, 25, 25, 250, 250, hExDui_dropobj, 0, DT_SINGLELINE | DT_VCENTER | DT_CENTER, 0, 0, OnDropObjDataMsgProc);

	Ex_DUIShowWindow(hExDui_dropobj, SW_SHOWNORMAL, 0, 0, 0);
}

LRESULT CALLBACK OnProgressBarProc(HWND hWnd,HEXOBJ hObj,INT uMsg,WPARAM wParam,LPARAM lParam, LRESULT* lpResult)
{
	if (uMsg == WM_TIMER)
	{
		size_t nPos = Ex_ObjSendMessage(hObj, PBM_GETPOS, 0, 0);
		size_t nRange = Ex_ObjSendMessage(hObj, PBM_GETRANGE, 0, 0);
		Ex_ObjSendMessage(hObj, PBM_SETPOS, nPos + Random(1, 20), 0);
		Ex_ObjSendMessage(hObj, WM_PAINT, 0, 0);
		if (nRange == nPos)
		{
			Ex_ObjKillTimer(hObj);
			Ex_MessageBox(hObj, L"加载完毕", L"提示", MB_OK, EMBF_CENTEWINDOW);
		}
	}

	return 0;
}

void test_progressbar(HWND hWnd)
{
	HWND hWnd_progressbar = Ex_WndCreate(hWnd, L"Ex_DirectUI", L"测试进度条", 0, 0, 400, 300, 0, 0);
	HEXDUI hExDui_progressbar = Ex_DUIBindWindowEx(hWnd_progressbar, 0, EWS_NOINHERITBKG | EWS_MOVEABLE | EWS_CENTERWINDOW | EWS_NOSHADOW | EWS_BUTTON_CLOSE | EWS_TITLE | EWS_HASICON, 0, 0);
	Ex_DUISetLong(hExDui_progressbar, EWL_CRBKG, ExARGB(150, 150, 150, 255));
	WCHAR Cls[] = L"ProgressBar";
	HEXOBJ hObj = Ex_ObjCreate(Cls, NULL, -1, 50, 100, 300, 20, hExDui_progressbar);
	Ex_ObjSetLong(hObj, EOL_OBJPROC, (size_t)OnProgressBarProc);
	Ex_ObjSetColor(hObj, COLOR_EX_TEXT_NORMAL, ExRGB2ARGB(16777215, 255), FALSE);
	Ex_ObjSendMessage(hObj, PBM_SETRADIUS, 10, 0);
	Ex_ObjSendMessage(hObj, PBM_SETRANGE, 255, 0);
	Ex_ObjInvalidateRect(hObj, NULL);
	Ex_ObjSetTimer(hObj, 50);

	Ex_DUIShowWindow(hExDui_progressbar, SW_SHOWNORMAL, 0, 0, 0);
}

LRESULT CALLBACK OnNchitTestButtonMsgProc(HWND hWnd,HEXOBJ hObj,INT uMsg,WPARAM wParam,LPARAM lParam, LRESULT* lpReturn)
{
	if (uMsg == WM_NCHITTEST)
	{
		if (!(LOWORD(lParam) >= 20 && LOWORD(lParam) <= 60 && HIWORD(lParam) >= 20 && HIWORD(lParam) <= 60))
		{
			return HTTRANSPARENT;
		}
	}
	else if (uMsg == WM_ERASEBKGND)
	{
		RECT rc{ 0 };
		Ex_ObjGetRect(hObj, &rc);
		HEXBRUSH hBrush = _brush_create(ExRGB2ARGB(16746496, 255));
		_canvas_drawrect(wParam, hBrush, 0, 0, rc.right - rc.left, rc.bottom - rc.top, 1, 0);
		_brush_setcolor(hBrush, ExRGB2ARGB(255, 255));
		_canvas_fillrect(wParam, hBrush, 20, 20, 60, 60);
		_brush_destroy(hBrush);
		return 1;
	}
	else if (uMsg == WM_LBUTTONDOWN)
	{
		output(L"被按下");
	}
	else if (uMsg == WM_LBUTTONUP)
	{
		output(L"被放开");
	}
	return 0;
}

void test_nchittest(HWND hWnd)
{
	HWND hWnd_nchittest = Ex_WndCreate(hWnd, L"Ex_DirectUI", L"测试限制通知区域", 0, 0, 400, 200, 0, 0);
	HEXDUI hExDui_nchittest = Ex_DUIBindWindowEx(hWnd_nchittest, 0, EWS_NOINHERITBKG | EWS_MOVEABLE | EWS_CENTERWINDOW | EWS_NOSHADOW | EWS_BUTTON_CLOSE | EWS_TITLE | EWS_HASICON, 0, 0);
	Ex_DUISetLong(hExDui_nchittest, EWL_CRBKG, ExARGB(150, 150, 150, 255));
	HEXOBJ hObj = Ex_ObjCreateEx(EOS_EX_FOCUSABLE, L"static", L"鼠标操作只能在红色区域里", -1, 50, 50, 300, 100, hExDui_nchittest, 0, DT_CENTER | DT_VCENTER | DT_SINGLELINE, 0, 0, OnNchitTestButtonMsgProc);

	Ex_DUIShowWindow(hExDui_nchittest, SW_SHOWNORMAL, 0, 0, 0);
}

std::vector<HEXOBJ> m_dialog_button(3);
std::vector<HEXOBJ> m_dialog_edit(2);

LRESULT CALLBACK OnDialogButtonEvent(HEXOBJ hObj, INT nID, INT nCode, WPARAM wParam, LPARAM lParam)
{
	if (nCode == NM_CLICK)
	{
		if (hObj == m_dialog_button[0])
		{
			OPENFILENAMEW file_info{ 0 };
			file_info.lStructSize = sizeof(OPENFILENAMEW);
			file_info.hwndOwner = 0;
			file_info.lpstrFilter = L"Picture File(*.bmp,*.jpg)\0*.bmp;*.jpg;\0\0";
			file_info.nFilterIndex = 1;
			WCHAR szFile[256];
			file_info.lpstrFile = szFile;
			file_info.nMaxFile = sizeof(szFile);
			file_info.lpstrTitle = L"打开";
			file_info.nMaxFileTitle = sizeof(file_info.lpstrTitle);
			file_info.Flags= OFN_PATHMUSTEXIST | OFN_FILEMUSTEXIST | OFN_EXPLORER;
			if (GetOpenFileNameW(&file_info))
			{
				Ex_ObjSetText(m_dialog_edit[0], (LPCWSTR)file_info.lpstrFile, TRUE);
			}
		}
		else if (hObj == m_dialog_button[1])
		{
			OPENFILENAMEW file_info{ 0 };
			file_info.lStructSize = sizeof(OPENFILENAMEW);
			file_info.hwndOwner = 0;
			file_info.lpstrFilter = L"Picture File(*.bmp,*.jpg)\0*.bmp;*.jpg;\0\0";
			file_info.nFilterIndex = 1;
			WCHAR szFile[256];
			file_info.lpstrFile = szFile;
			file_info.nMaxFile = sizeof(szFile);
			file_info.lpstrTitle = L"打开";
			file_info.nMaxFileTitle = sizeof(file_info.lpstrTitle);
			file_info.Flags = OFN_PATHMUSTEXIST | OFN_FILEMUSTEXIST | OFN_EXPLORER;
			if (GetOpenFileNameW(&file_info))
			{
				Ex_ObjSetText(m_dialog_edit[1], (LPCWSTR)file_info.lpstrFile, TRUE);
			}
		}
		else if (hObj == m_dialog_button[2])
		{
			auto text_length1 = Ex_ObjGetTextLength(m_dialog_edit[0]);
			std::wstring str1;
			str1.resize(text_length1 * 2 + 2);
			Ex_ObjGetText(m_dialog_edit[0], str1.c_str(), text_length1 * 2);
			
			auto text_length2 = Ex_ObjGetTextLength(m_dialog_edit[1]);
			std::wstring str2;
			str2.resize(text_length2 * 2 + 2);
			Ex_ObjGetText(m_dialog_edit[1], str2.c_str(), text_length2 * 2);

			output(str1);
			output(str2);
		}
	}
	return 0;
}

void test_dialog(HWND hWnd)
{
	HWND hWnd_dialog = Ex_WndCreate(hWnd, L"Ex_DirectUI", L"测试打开对话框", 0, 0, 400, 200, 0, 0);
	HEXDUI hExDui_dialog = Ex_DUIBindWindowEx(hWnd_dialog, 0, EWS_NOINHERITBKG | EWS_MOVEABLE | EWS_CENTERWINDOW | EWS_NOSHADOW | EWS_BUTTON_CLOSE | EWS_TITLE | EWS_HASICON, 0, 0);
	Ex_DUISetLong(hExDui_dialog, EWL_CRBKG, ExARGB(150, 150, 150, 255));
	m_dialog_button[0]= Ex_ObjCreate( L"button", L"按钮1", -1, 10, 50, 100, 30, hExDui_dialog);
	Ex_ObjHandleEvent(m_dialog_button[0], NM_CLICK, OnDialogButtonEvent);
	m_dialog_edit[0] = Ex_ObjCreate(L"edit", L"", -1, 110, 50, 250, 30, hExDui_dialog);

	m_dialog_button[1] = Ex_ObjCreate(L"button", L"按钮2", -1, 10, 90, 100, 30, hExDui_dialog);
	Ex_ObjHandleEvent(m_dialog_button[1], NM_CLICK, OnDialogButtonEvent);
	m_dialog_edit[1] = Ex_ObjCreate(L"edit", L"", -1, 110, 90, 250, 30, hExDui_dialog);

	m_dialog_button[2] = Ex_ObjCreate(L"button", L"按钮3", -1, 10, 130, 100, 30, hExDui_dialog);
	Ex_ObjHandleEvent(m_dialog_button[2], NM_CLICK, OnDialogButtonEvent);


	Ex_DUIShowWindow(hExDui_dialog, SW_SHOWNORMAL, 0, 0, 0);
}