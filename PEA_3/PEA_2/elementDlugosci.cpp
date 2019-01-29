#include "pch.h"
#include "elementDlugosci.h"


elementDlugosci::elementDlugosci()
{
	osobnik = INT32_MAX;
	dlugosc = INT32_MAX;
}

elementDlugosci::elementDlugosci(unsigned os, unsigned dl)
{
	osobnik = os;
	dlugosc = dl;
}


elementDlugosci::~elementDlugosci()
{
}

unsigned elementDlugosci::zwrocDlugosc()
{
	return dlugosc;
}

unsigned elementDlugosci::zwrocOsobnik()
{
	return osobnik;
}

void elementDlugosci::ustawDlugosc(unsigned dl)
{
	dlugosc = dl;
}

void elementDlugosci::ustawOsobnik(unsigned os)
{
	osobnik = os;
}

bool elementDlugosci::operator<(const elementDlugosci & el)
{
	if (dlugosc < el.dlugosc)
	{
		return true;
	}
	else
	{
		return false;
	}
	
}

std::ostream & operator<<(std::ostream & os, elementDlugosci const & el)
{
	os << el.osobnik << ": " << el.dlugosc;
	return os;
}
