#include "stdafx.h"
#include "QuanLyChiTieu.h"
#include "PieChart.h"
#include "TypeCosts.h"

float PieChart::getSweepAngle(int index)
{
	int total = 0; 
	for (int i = 0; i < typeCosts.size(); ++i)
		total += typeCosts[i].amount;
	float percent = (float)typeCosts[index].amount / total;
	percent = percent * 360;
	return percent;
}

PieChart::PieChart()
{

}

PieChart::PieChart(POINT p, int r)
{
	// tâm
	center = p;
	//bán kính
	radius = r;
	typeCosts.resize(6);
	// màu biểu đồ
	typeCosts[0].color = RGB(0, 255, 0);
	typeCosts[1].color = RGB(218, 12, 59);
	typeCosts[2].color = RGB(37, 40, 218);
	typeCosts[3].color = RGB(149, 21, 255);
	typeCosts[4].color = RGB(218, 206, 50);
	typeCosts[5].color = RGB(255, 25, 41);
}

PieChart::~PieChart()
{
}

void PieChart::drawPieChart(HDC hdc)
{
	for (int i = 0; i < typeCosts.size(); ++i)
	{
		if (typeCosts[i].amount != 0)
		{
			SetDCBrushColor(hdc, RGB(GetRValue(typeCosts[i].color),GetGValue( typeCosts[i].color),GetBValue( typeCosts[i].color)));
			SetDCPenColor(hdc, RGB(241, 241, 241));
			SelectObject(hdc, GetStockObject(DC_BRUSH));
			SelectObject(hdc, GetStockObject(DC_PEN));

			BeginPath(hdc);
			MoveToEx(hdc, this->center.x, this->center.y, NULL);
			AngleArc(hdc, this->center.x, this->center.y, this->radius, this->typeCosts[i].startAngle, this->typeCosts[i].sweepAngle);
			LineTo(hdc, this->center.x, this->center.y);
			EndPath(hdc);
			StrokeAndFillPath(hdc);
		}
	}
}

void PieChart::upDateChart(vector<TypeCosts> costs)
{
	for (int i = 0; i < typeCosts.size(); ++i)
	{
		typeCosts[i].type = costs[i].getTypeCost();
		typeCosts[i].amount = costs[i].getAmount();
		
	}

	for (int i = 0; i < typeCosts.size(); ++i)
		typeCosts[i].sweepAngle = getSweepAngle(i);

	typeCosts[0].startAngle = 90;

	for (int i = 1; i < typeCosts.size(); ++i)
		typeCosts[i].startAngle = typeCosts[i - 1].startAngle + typeCosts[i - 1].sweepAngle;
}

void PieChart::annotate(HDC hdc)
{
	HFONT hFontInside = CreateFont(15, 0, 0, 0, 800, FALSE, FALSE, FALSE, VIETNAMESE_CHARSET,
		OUT_TT_PRECIS, CLIP_DEFAULT_PRECIS, DEFAULT_QUALITY, DEFAULT_PITCH | FF_DONTCARE, TEXT("Calibri"));
	HFONT hFontOutside = CreateFont(15, 0, 0, 0, FW_DONTCARE, FALSE, FALSE, FALSE, VIETNAMESE_CHARSET,
		OUT_TT_PRECIS, CLIP_DEFAULT_PRECIS, DEFAULT_QUALITY, DEFAULT_PITCH | FF_DONTCARE, TEXT("Calibri"));
	vector<POINT> vPoint;
	POINT temp;

	temp.x = 30;
	temp.y = 450;
	vPoint.push_back(temp);

	temp.x = 200;
	temp.y = 450;
	vPoint.push_back(temp);

	temp.x = 30;
	temp.y = 510;
	vPoint.push_back(temp);

	temp.x = 200;
	temp.y = 510;
	vPoint.push_back(temp);

	temp.x = 30;
	temp.y = 570;
	vPoint.push_back(temp);

	temp.x = 200;
	temp.y = 570;
	vPoint.push_back(temp);

	SetBkMode(hdc, TRANSPARENT);
	SelectObject(hdc, hFontInside);
	SetTextColor(hdc, RGB(255, 255, 255));

	for (int i = 0; i < this->typeCosts.size(); ++i)
	{
		SetDCBrushColor(hdc, RGB(GetRValue(typeCosts[i].color), GetGValue(typeCosts[i].color), GetBValue(typeCosts[i].color)));
		Rectangle(hdc, vPoint[i].x, vPoint[i].y, vPoint[i].x + 50, vPoint[i].y + 50);
		RECT rect = { vPoint[i].x, vPoint[i].y, vPoint[i].x + 50, vPoint[i].y + 50 };
		DrawText(hdc, (to_wstring((int)round(this->getSweepAngle(i) / 360 * 100)) + L'%').c_str(), -1, &rect, DT_SINGLELINE | DT_NOCLIP | DT_CENTER | DT_VCENTER);
	}

	SelectObject(hdc, hFontInside);
	SetTextColor(hdc, RGB(0, 0, 0));
	for (int i = 0; i < this->typeCosts.size(); ++i)
	{
		RECT rect = { vPoint[i].x + 60, vPoint[i].y, vPoint[i].x + 150, vPoint[i].y + 50 };
		DrawText(hdc, typeCosts[i].type.c_str(), -1, &rect, DT_SINGLELINE | DT_NOCLIP | DT_LEFT | DT_VCENTER);
	}
}
