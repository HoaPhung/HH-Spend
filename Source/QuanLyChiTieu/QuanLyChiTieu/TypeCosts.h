#pragma once

#include "stdafx.h"
#include <string>
using namespace std;
class TypeCosts
{
private:
	wstring type;
	int amount;
public:
	TypeCosts();
	TypeCosts(wstring ty, int am);
	~TypeCosts();
	wstring getTypeCost();
	int getAmount();
	void setTypeCost(wstring ty);
	void setAmount(int am);
};
