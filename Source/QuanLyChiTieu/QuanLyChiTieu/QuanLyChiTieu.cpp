// QuanLyChiTieu.cpp : Defines the entry point for the application.
//

#include "stdafx.h"
#include "QuanLyChiTieu.h"
#include "Costs.h"
#include "PieChart.h"
#include "TypeCosts.h"
#include <windowsX.h>
#include <winuser.h>
#include <iostream>
#include <fstream>
#include <string>
#include <cstring>
#include <atlstr.h> 
#include <vector>
#include <sstream>
#include <locale>
#include <codecvt>
#include <commctrl.h>
#pragma comment(linker,"\"/manifestdependency:type='win32' name='Microsoft.Windows.Common-Controls' version='6.0.0.0' processorArchitecture='*' publicKeyToken='6595b64144ccf1df' language='*'\"")
#pragma comment(lib, "ComCtl32.lib")
using namespace std;
// Global Variables:
HINSTANCE hInst;                                // current instance
WCHAR szTitle[MAX_LOADSTRING];                  // The title bar text
WCHAR szWindowClass[MAX_LOADSTRING];            // the main window class name										
HWND gGroupBoxAdd;
HWND gStaticType;
HWND gComboBoxType;
HWND gComboBoxChoice;
HWND gEditGetMoney;
int gTotal = 0;
HWND gStaticContent;
HWND gStaticMoney;
HWND gEditContent;
HWND gEditMoney;
HWND gGroupBoxList;
HWND gBtnAdd;
HWND gListView;
int gIndex = 0;
HWND gGroupBoxChart;
PieChart * gChart = NULL;
vector<Costs> gCost;
vector<TypeCosts> gTypeCosts;

// Forward declarations of functions included in this code module:
ATOM                MyRegisterClass(HINSTANCE hInstance);
BOOL                InitInstance(HINSTANCE, int);
LRESULT CALLBACK    WndProc(HWND, UINT, WPARAM, LPARAM);
INT_PTR CALLBACK    About(HWND, UINT, WPARAM, LPARAM);
Costs Exception(HWND hWnd);			// bắt các lỗi khi nhập dữ liệu
bool checkMoney(wstring money);		// Kiểm tra số tiền nhập vào có hợp lệ hay không
HFONT setFont();					//set font hệ thống
void createGroupAdd(HWND hWnd);		// tạo nhóm thêm một loại chi tiêu
void createGroupList(HWND hWnd);	// tạo nhóm danh sách các loại chi tiêu
void createGroupChart(HWND hWnd);	// tạo nhóm biểu đồ
vector<Costs> readData();			// đọc file 
void saveData(vector<Costs> costs);	// ghi dữ liệu vào file
void addItemListView(int index, Costs costs); //thêm item vào listview
void createType();	// tạo danh sách các loại chi tiêu



int APIENTRY wWinMain(_In_ HINSTANCE hInstance,
                     _In_opt_ HINSTANCE hPrevInstance,
                     _In_ LPWSTR    lpCmdLine,
                     _In_ int       nCmdShow)
{
    UNREFERENCED_PARAMETER(hPrevInstance);
    UNREFERENCED_PARAMETER(lpCmdLine);

    // TODO: Place code here.

    // Initialize global strings
    LoadStringW(hInstance, IDS_APP_TITLE, szTitle, MAX_LOADSTRING);
    LoadStringW(hInstance, IDC_QUANLYCHITIEU, szWindowClass, MAX_LOADSTRING);
    MyRegisterClass(hInstance);

    // Perform application initialization:
    if (!InitInstance (hInstance, nCmdShow))
    {
        return FALSE;
    }

    HACCEL hAccelTable = LoadAccelerators(hInstance, MAKEINTRESOURCE(IDC_QUANLYCHITIEU));

    MSG msg;

    // Main message loop:
    while (GetMessage(&msg, nullptr, 0, 0))
    {
        if (!TranslateAccelerator(msg.hwnd, hAccelTable, &msg))
        {
            TranslateMessage(&msg);
            DispatchMessage(&msg);
        }
    }

    return (int) msg.wParam;
}



//
//  FUNCTION: MyRegisterClass()
//
//  PURPOSE: Registers the window class.
//
ATOM MyRegisterClass(HINSTANCE hInstance)
{
    WNDCLASSEXW wcex;

    wcex.cbSize = sizeof(WNDCLASSEX);

    wcex.style          = CS_HREDRAW | CS_VREDRAW;
    wcex.lpfnWndProc    = WndProc;
    wcex.cbClsExtra     = 0;
    wcex.cbWndExtra     = 0;
    wcex.hInstance      = hInstance;
    wcex.hIcon          = LoadIcon(hInstance, MAKEINTRESOURCE(IDI_QUANLYCHITIEU));
    wcex.hCursor        = LoadCursor(nullptr, IDC_ARROW);
    wcex.hbrBackground  = (HBRUSH)(COLOR_BTNFACE + 1);
    wcex.lpszMenuName   = MAKEINTRESOURCEW(IDC_QUANLYCHITIEU);
    wcex.lpszClassName  = szWindowClass;
    wcex.hIconSm        = LoadIcon(wcex.hInstance, MAKEINTRESOURCE(IDI_SMALL));

    return RegisterClassExW(&wcex);
}

//
//   FUNCTION: InitInstance(HINSTANCE, int)
//
//   PURPOSE: Saves instance handle and creates main window
//
//   COMMENTS:
//
//        In this function, we save the instance handle in a global variable and
//        create and display the main program window.
//
BOOL InitInstance(HINSTANCE hInstance, int nCmdShow)
{
   hInst = hInstance; // Store instance handle in our global variable

   HWND hWnd = CreateWindowW(szWindowClass, szTitle, WS_OVERLAPPEDWINDOW,
	   50, 0, 650, 730, nullptr, nullptr, hInstance, nullptr);

   if (!hWnd)
   {
      return FALSE;
   }

   ShowWindow(hWnd, nCmdShow);
   UpdateWindow(hWnd);

   return TRUE;
}

//
//  FUNCTION: WndProc(HWND, UINT, WPARAM, LPARAM)
//
//  PURPOSE:  Processes messages for the main window.
//
//  WM_COMMAND  - process the application menu
//  WM_PAINT    - Paint the main window
//  WM_DESTROY  - post a quit message and return
//
//
LRESULT CALLBACK WndProc(HWND hWnd, UINT message, WPARAM wParam, LPARAM lParam)
{
    switch (message)
    {
	case WM_CREATE:
	{
		createGroupAdd(hWnd);

		createGroupList(hWnd);

		createType();

		createGroupChart(hWnd);

		gCost = readData();


		for (int i = 0; i < gCost.size(); i++)
		{
			addItemListView(gIndex, gCost[i]);
			gTotal += gCost[i].getMoney();
		}

	}
	break;
    case WM_COMMAND:
        {
			
		if (HIWORD(wParam) == CBN_SELCHANGE)
		{
			wchar_t buffer[256];
			int tmp = 0;
			int selection = SendMessage(gComboBoxChoice, CB_GETCURSEL, NULL, NULL);
			if (selection != CB_ERR)
			{
				switch (selection)
				{
				case 0:
					tmp = gTypeCosts[0].getAmount();
					break;
				case 1:
					tmp = gTypeCosts[1].getAmount();
					break;
				case 2:
					tmp = gTypeCosts[2].getAmount();
					break;
				case 3:
					tmp = gTypeCosts[3].getAmount();
					break;
				case 4:
					tmp = gTypeCosts[4].getAmount();
					break;
				case 5:
					tmp = gTypeCosts[5].getAmount();
					break;
				case 6:
					tmp = gTotal;
					break;
				}
			}
				wsprintfW(buffer, L"%d", tmp);
				SetWindowText(gEditGetMoney, buffer);
			}

            int wmId = LOWORD(wParam);
            
            switch (wmId)
            {
			case ID_BUTTON_ADD:
			{
				Costs costs=Exception(hWnd);
				if (costs.getType() != L"")
				{
					addItemListView(gIndex, costs);
					gCost.push_back(costs);
					InvalidateRect(hWnd, NULL, TRUE);

					SetWindowText(gEditContent, _T(""));
					SetWindowText(gEditMoney, _T(""));
					SetWindowText(gComboBoxType, _T(""));
				}
			}
			break;
			/*case CBN_SELCHANGE:
			{
				wchar_t buffer[256];
				int tmp = 0;
				 int selection= SendMessage(gComboBoxChoice, CB_GETCURSEL, NULL, NULL);
				 if (selection != CB_ERR)
				 {
					 switch (selection)
					 {
					 case 0:
						 tmp = gTypeCosts[0].getAmount();
						 break;
					 case 1:
						 tmp = gTypeCosts[1].getAmount();
						 break;
					 case 2:
						 tmp = gTypeCosts[2].getAmount();
						 break;
					 case 3:
						 tmp = gTypeCosts[3].getAmount();
						 break;
					 case 4:
						 tmp = gTypeCosts[4].getAmount();
						 break;
					 case 5:
						 tmp = gTypeCosts[5].getAmount();
						 break;
					 case 6:
						 tmp = gTotal;
						 break;
					 }
				 }
				 wsprintfW(buffer, L"%d", tmp);
				 SetWindowText(gEditGetMoney, buffer);
			}
			break;*/

            case IDM_ABOUT:
                DialogBox(hInst, MAKEINTRESOURCE(IDD_ABOUTBOX), hWnd, About);
                break;
            case IDM_EXIT:
                DestroyWindow(hWnd);
                break;
            default:
                return DefWindowProc(hWnd, message, wParam, lParam);
            }
        }
        break;
    case WM_PAINT:
        {
            PAINTSTRUCT ps;
            HDC hdc = BeginPaint(hWnd, &ps);
            // Vẽ biểu đồ
			gChart->drawPieChart(hdc);
			// Vẽ chú giải cho biểu đồ
			gChart->annotate(hdc);

			EndPaint(hWnd, &ps);
        }
        break;
    case WM_DESTROY:
	{
		saveData(gCost);
		PostQuitMessage(0);
	}
		break;
    default:
        return DefWindowProc(hWnd, message, wParam, lParam);
    }
    return 0;
}

// Message handler for about box.
INT_PTR CALLBACK About(HWND hDlg, UINT message, WPARAM wParam, LPARAM lParam)
{
    UNREFERENCED_PARAMETER(lParam);
    switch (message)
    {
    case WM_INITDIALOG:
        return (INT_PTR)TRUE;

    case WM_COMMAND:
        if (LOWORD(wParam) == IDOK || LOWORD(wParam) == IDCANCEL)
        {
            EndDialog(hDlg, LOWORD(wParam));
            return (INT_PTR)TRUE;
        }
        break;
    }
    return (INT_PTR)FALSE;
}

Costs Exception(HWND hWnd)
{
	Costs costs;

	LPWSTR content = NULL;
	LPWSTR money = NULL;
	LPWSTR type = NULL;

	int sizeContent;
	int sizeMoney;
	int sizeType;

	sizeContent = GetWindowTextLength(gEditContent);
	sizeMoney = GetWindowTextLength(gEditMoney);
	sizeType = GetWindowTextLength(gComboBoxType);
	
	
	content = new WCHAR[sizeContent + 1];
	money = new WCHAR[sizeMoney + 1];
	type = new WCHAR[sizeType + 1];


	GetWindowText(gEditContent, content, sizeContent + 1);
	GetWindowText(gEditMoney, money, sizeMoney + 1);
	GetWindowText(gComboBoxType, type, sizeType + 1);

	// check string
	if (sizeType == 0 && sizeContent != 0 && sizeMoney != 0)
		MessageBox(hWnd, L"Bạn chưa chọn loại chi tiêu!", L"Notify", MB_OK);

	else if (sizeType == 0 && sizeContent == 0 && sizeMoney != 0)
		MessageBox(hWnd, L"Bạn chưa chọn loại chi tiêu và nhập nội dung!", L"Notify", MB_OK);

	else if (sizeType == 0 && sizeContent != 0 && sizeMoney == 0)
		MessageBox(hWnd, L"Bạn chưa chọn loại chi tiêu và nhập số tiền!", L"Notify", MB_OK);

	else if (sizeType == 0 && sizeContent == 0 && sizeMoney == 0)
		MessageBox(hWnd, L"Bạn chưa chọn loại chi tiêu, nhập nội dung và số tiền!", L"Notify", MB_OK);

	else if (sizeType != 0 && sizeContent == 0 && sizeMoney == 0)
		MessageBox(hWnd, L"Bạn chưa nhập nội dung và số tiền!", L"Notify", MB_OK);

	else if (sizeType != 0 && sizeContent == 0 && sizeMoney != 0)
		MessageBox(hWnd, L"Bạn chưa nhập nội dung!", L"Notify", MB_OK);

	else if (sizeType != 0 && sizeContent != 0 && sizeMoney == 0)
		MessageBox(hWnd, L"Bạn chưa nhập số tiền!", L"Notify", MB_OK);

	else if (checkMoney(money) == false)
		MessageBox(hWnd, L"Bạn đã nhập sai ô số tiền. Trong ô này bạn chỉ được nhập ký tự số!", L"Notify", MB_OK);
	else
	{
		int iMoney = _wtoi(money);
		costs.setType(type);
		costs.setContent(content);
		costs.setMoney(iMoney);
	}
	return costs;
}

bool checkMoney(wstring money)
{
	for (int i = 0; i < money.length(); i++) {
		if (money[i] < 48 || money[i]>57)
			return false;
	}
	return true;
}

HFONT setFont()
{
	LOGFONT lf;
	GetObject(GetStockObject(DEFAULT_GUI_FONT), sizeof(LOGFONT), &lf);
	HFONT hFont = CreateFont(lf.lfHeight, lf.lfWidth,
		lf.lfEscapement, lf.lfOrientation, lf.lfWeight,
		lf.lfItalic, lf.lfUnderline, lf.lfStrikeOut, lf.lfCharSet,
		lf.lfOutPrecision, lf.lfClipPrecision, lf.lfQuality,
		lf.lfPitchAndFamily, lf.lfFaceName);
	return hFont;
}

void createGroupAdd(HWND hWnd)
{
	HFONT hFont = setFont();
	gGroupBoxAdd = CreateWindow(L"BUTTON", L"Thêm một loại chi tiêu", WS_CHILD | WS_VISIBLE | BS_GROUPBOX,
		10, 15, 600, 100, hWnd, (HMENU)ID_GROUPBOX_ADD, hInst, 0);

	gStaticType = CreateWindow(L"STATIC", L"Loại chi tiêu", WS_CHILD | WS_VISIBLE,
		20, 40, 150, 20, hWnd, 0, hInst, 0);
	SendMessage(gStaticType, WM_SETFONT, WPARAM(hFont), TRUE);

	gComboBoxType = CreateWindow(L"COMBOBOX", L"", WS_CHILD | WS_VISIBLE | CBS_DROPDOWNLIST,
		20, 60, 100, 500, hWnd, (HMENU)ID_COMBOBOX_TYPE, hInst, NULL);
	SendMessage(gComboBoxType, WM_SETFONT, WPARAM(hFont), TRUE);

	SendMessage(gComboBoxType, CB_ADDSTRING, NULL, (LPARAM)L"Ăn uống");
	SendMessage(gComboBoxType, CB_ADDSTRING, NULL, (LPARAM)L"Di chuyển");
	SendMessage(gComboBoxType, CB_ADDSTRING, NULL, (LPARAM)L"Nhà cửa");
	SendMessage(gComboBoxType, CB_ADDSTRING, NULL, (LPARAM)L"Xe cộ");
	SendMessage(gComboBoxType, CB_ADDSTRING, NULL, (LPARAM)L"Nhu yếu");
	SendMessage(gComboBoxType, CB_ADDSTRING, NULL, (LPARAM)L"Dịch vụ");
	SendMessage(gComboBoxType, WM_SETFONT, WPARAM(hFont), TRUE);

	gStaticContent = CreateWindow(L"STATIC", L"Nội dung", WS_CHILD | WS_VISIBLE,
		170, 40, 100, 20, hWnd, 0, hInst, 0);
	SendMessage(gStaticContent, WM_SETFONT, WPARAM(hFont), TRUE);

	gEditContent = CreateWindow(L"EDIT", L"", WS_CHILD | WS_VISIBLE | WS_BORDER,
		170, 60, 150, 25, hWnd, (HMENU)ID_EDIT_CONTENT, hInst, NULL);
	SendMessage(gEditContent, 0, WPARAM(hFont), TRUE);

	gStaticMoney = CreateWindow(L"STATIC", L"Số tiền", WS_CHILD | WS_VISIBLE,
		350, 40, 100, 20, hWnd, 0, hInst, 0);
	SendMessage(gStaticMoney, WM_SETFONT, WPARAM(hFont), TRUE);

	gEditMoney = CreateWindow(L"EDIT", L"", WS_CHILD | WS_VISIBLE | WS_BORDER,
		350, 60, 100, 25, hWnd, (HMENU)ID_EDIT_MONEY, hInst, NULL);
	SendMessage(gEditMoney, 0, WPARAM(hFont), TRUE);

	gBtnAdd = CreateWindowEx(0, L"BUTTON", L"Thêm", WS_CHILD | WS_VISIBLE | BS_PUSHBUTTON,
		500, 60, 70, 25, hWnd, (HMENU)ID_BUTTON_ADD, hInst, NULL);
	SendMessage(gBtnAdd, WM_SETFONT, WPARAM(hFont), TRUE);
}

void createGroupList(HWND hWnd)
{
	INITCOMMONCONTROLSEX icc;
	icc.dwSize = sizeof(icc);
	icc.dwICC = ICC_WIN95_CLASSES;
	InitCommonControlsEx(&icc);
	HFONT hFont = setFont();
	gGroupBoxList = CreateWindow(L"BUTTON", L"Danh sách chi tiêu", WS_CHILD | WS_VISIBLE | BS_GROUPBOX,
		10, 130, 600, 290, hWnd, (HMENU)ID_GROUPBOX_LIST, hInst, 0);

	gListView = CreateWindow(WC_LISTVIEWW, L"",
		WS_CHILD | WS_VISIBLE |
		WS_VSCROLL | LVS_REPORT | WS_BORDER,
		20, 150, 580, 200,
		hWnd, (HMENU)ID_LISTVIEW, hInst, NULL);

	LVCOLUMN lvCol1;
	lvCol1.mask = LVCF_FMT | LVCF_TEXT | LVCF_WIDTH;
	lvCol1.fmt = LVCFMT_LEFT;
	lvCol1.cx = 150;
	lvCol1.pszText = _T("Loại chi tiêu");
	ListView_InsertColumn(gListView, 0, &lvCol1);

	LVCOLUMN lvCol2;
	lvCol2.mask = LVCF_FMT | LVCF_TEXT | LVCF_WIDTH;
	lvCol2.fmt = LVCFMT_LEFT;
	lvCol2.cx = 280;
	lvCol2.pszText = _T("Nội dung");
	ListView_InsertColumn(gListView, 1, &lvCol2);

	LVCOLUMN lvCol3;
	lvCol3.mask = LVCF_FMT | LVCF_TEXT | LVCF_WIDTH;
	lvCol3.fmt = LVCFMT_RIGHT;
	lvCol3.cx = 130;
	lvCol3.pszText = _T("Số tiền");
	ListView_InsertColumn(gListView, 2, &lvCol3);


	gComboBoxChoice = CreateWindow(L"COMBOBOX", L"", WS_CHILD | WS_VISIBLE | CBS_DROPDOWNLIST,
		20, 360, 430, 500, hWnd, (HMENU)ID_COMBOBOX_CHOICE, hInst, NULL);
	SendMessage(gComboBoxChoice, WM_SETFONT, WPARAM(hFont), TRUE);

	SendMessage(gComboBoxChoice, CB_ADDSTRING, NULL, (LPARAM)L"Ăn uống");
	SendMessage(gComboBoxChoice, CB_ADDSTRING, NULL, (LPARAM)L"Di chuyển");
	SendMessage(gComboBoxChoice, CB_ADDSTRING, NULL, (LPARAM)L"Nhà cửa");
	SendMessage(gComboBoxChoice, CB_ADDSTRING, NULL, (LPARAM)L"Xe cộ");
	SendMessage(gComboBoxChoice, CB_ADDSTRING, NULL, (LPARAM)L"Nhu yếu");
	SendMessage(gComboBoxChoice, CB_ADDSTRING, NULL, (LPARAM)L"Dịch vụ");
	SendMessage(gComboBoxChoice, CB_ADDSTRING, NULL, (LPARAM)L"Tổng tiền đã chi");
	SendMessage(gComboBoxChoice, WM_SETFONT, WPARAM(hFont), TRUE);
	

	gEditGetMoney = CreateWindow(L"EDIT", L"", WS_CHILD | WS_VISIBLE | WS_BORDER,
		470, 360, 130,20, hWnd, (HMENU)ID_EDIT_GET_MONEY, hInst, NULL);
	SendMessage(gEditGetMoney, 0, WPARAM(hFont), TRUE);

}

void createGroupChart(HWND hWnd)
{
	HFONT hFont = setFont();
	gGroupBoxChart = CreateWindow(L"BUTTON", L"Biểu đồ", WS_CHILD | WS_VISIBLE | BS_GROUPBOX,
		10, 430, 600, 220, hWnd, (HMENU)ID_GROUPBOX_CHART, hInst, 0);
	int radius = 100;
	POINT center = { 470,545 };
	gChart = new PieChart(center, radius);
	gChart->upDateChart(gTypeCosts);
}

vector<Costs> readData()
{
	vector<Costs> costs;
	locale loc(locale(), new codecvt_utf8<wchar_t>);
	wifstream ifs("data.txt");
	ifs.imbue(loc);

	if (ifs.is_open())
	{
		while (!ifs.eof())
		{
			wstring buffer, item;
			getline(ifs, buffer);
			if (!buffer.empty() && buffer[0] == 65279) buffer = buffer.substr(1, buffer.size() - 1);
			if (!buffer.empty())
			{
				wstringstream ss(buffer);
				vector<wstring> items;

				while (getline(ss, item, L';')) items.push_back(item);
				Costs temp(items[0], items[1], stoi(items[2]));
				costs.push_back(temp);
			}
		}
	}

	ifs.close();

	return costs;
}

void saveData(vector<Costs> costs)
{
	locale loc(locale(), new codecvt_utf8<wchar_t>);
	wofstream ofs("data.txt");
	ofs.imbue(loc);

	for (int i = 0; i < costs.size(); ++i)
	{
		wstring buffer = costs[i].getType() + L';' +
			costs[i].getContent() + L';' +
			to_wstring(costs[i].getMoney());
		
		ofs << buffer;
		if (i < costs.size() - 1) ofs << endl;
	}
	ofs.close();
}

void addItemListView(int index, Costs costs)
{
	LVITEM lv;

	lv.mask = LVIF_TEXT;
	lv.iItem = index;

	lv.iSubItem = 0;
	lv.pszText = new WCHAR[costs.getType().length() + 1];
	wcscpy(lv.pszText, costs.getType().c_str());
	ListView_InsertItem(gListView, &lv);

	LPWSTR content = new WCHAR[costs.getContent().length() + 1];
	wcscpy(content, costs.getContent().c_str());
	ListView_SetItemText(gListView, index, 1, content);

	lv.iSubItem = 2;
	_itow(costs.getMoney(), lv.pszText, 10);
	ListView_SetItem(gListView, &lv);

	gIndex++;

	int test = false;
	for (int i = 0; i < 6; ++i)
	{
		
		if (costs.getType().compare(gTypeCosts[i].getTypeCost()) == 0)
		{
			int temp = gTypeCosts[i].getAmount() + costs.getMoney();
			gTypeCosts[i].setAmount(temp);
		}
		
	}

	gChart->upDateChart(gTypeCosts);
}

void createType()
{

	TypeCosts temp1(L"Ăn uống",0);
	TypeCosts temp2(L"Di chuyển",0);
	TypeCosts temp3(L"Nhà cửa",0);
	TypeCosts temp4(L"Xe cộ",0);
	TypeCosts temp5(L"Nhu yếu",0);
	TypeCosts temp6(L"Dịch vụ",0);
	

	gTypeCosts.push_back(temp1);
	gTypeCosts.push_back(temp2);
	gTypeCosts.push_back(temp3);
	gTypeCosts.push_back(temp4);
	gTypeCosts.push_back(temp5);
	gTypeCosts.push_back(temp6);
}
