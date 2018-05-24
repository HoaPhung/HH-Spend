#pragma once

#include "stdafx.h"
#include <string>

using namespace std;
class Costs
{
private:
	wstring sType;
	wstring sContent;
	int sMoney;
public:
	Costs();
	Costs(wstring type, wstring content, int money);
	~Costs();

	wstring getType();
	wstring getContent();
	int getMoney();

	void setType(wstring type);
	void setContent(wstring content);
	void setMoney(int money);
};