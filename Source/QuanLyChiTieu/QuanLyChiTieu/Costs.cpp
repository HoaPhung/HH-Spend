#include "stdafx.h"
#include "Costs.h"

Costs::Costs()
{
}

Costs::Costs(wstring type, wstring content, int money)
{
	sType = type;
	sContent = content;
	sMoney = money;
}

Costs::~Costs()
{
}

wstring Costs::getType()
{
	return sType;
}

wstring Costs::getContent()
{
	return sContent;
}

int Costs::getMoney()
{
	return sMoney;
}

void Costs::setType(wstring type)
{
	sType = type;
}

void Costs::setContent(wstring content)
{
	sContent = content;
}

void Costs::setMoney(int money)
{
	sMoney = money;
}
