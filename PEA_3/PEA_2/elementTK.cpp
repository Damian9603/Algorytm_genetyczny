#include "pch.h"
#include "elementTK.h"


elementTK::elementTK()
{
	wierzcholek = INT32_MAX;
	plus = false;
}


elementTK::~elementTK()
{
}

void elementTK::ustawPlus()
{
	plus = true;
}

void elementTK::ustawWierzcholek(unsigned w)
{
	wierzcholek = w;
}

bool elementTK::zwrocPlus()
{
	return plus;
}

unsigned elementTK::zwrocWierzcholek()
{
	return wierzcholek;
}

std::ostream & operator<<(std::ostream & napis, elementTK const & element)
{
	if (element.plus)
	{
		napis << element.wierzcholek << "+";
	}
	else
	{
		napis << element.wierzcholek;
	}
	return napis;
}
