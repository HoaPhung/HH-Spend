#pragma once

#include "stdafx.h"
#include "Costs.h"
#include <vector>
#include <string>
#include "TypeCosts.h"

using namespace std;

struct TypeCostChart
{
	wstring type;
	COLORREF color;
	int amount;
	float startAngle;
	float sweepAngle;
};

class PieChart
{
private:
	POINT center;
	int radius;
	vector<TypeCostChart> typeCosts;
	float getSweepAngle(int index);
public:
	PieChart();
	PieChart(POINT p, int r);
	~PieChart();
	void drawPieChart(HDC hdc);
	void upDateChart(vector<TypeCosts> costs);
	void annotate(HDC hdc);// Chú thích cho biểu đồ
};
