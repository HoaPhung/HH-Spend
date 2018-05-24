#include "stdafx.h"
#include "TypeCosts.h"

TypeCosts::TypeCosts()
{
	amount = 0;
}

TypeCosts::TypeCosts(wstring ty, int am)
{
	type = ty;
	amount = am;
}

TypeCosts::~TypeCosts()
{
}

wstring TypeCosts::getTypeCost()
{
	return type;
}

int TypeCosts::getAmount()
{
	return amount;
}

void TypeCosts::setTypeCost(wstring ty)
{
	type = ty;
}

void TypeCosts::setAmount(int am)
{
	amount = am;
}
