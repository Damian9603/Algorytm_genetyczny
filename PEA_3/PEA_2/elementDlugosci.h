#pragma once
//klasa pomocnicza wykorzystywana przy obliczaniu d³ugoœci i sortowaniu dróg
class elementDlugosci
{
public:
	elementDlugosci();
	elementDlugosci(unsigned os, unsigned dl);
	~elementDlugosci();
	unsigned zwrocDlugosc();
	unsigned zwrocOsobnik();
	void ustawDlugosc(unsigned dl);
	void ustawOsobnik(unsigned os);
	bool operator <(const elementDlugosci & el);
	friend std::ostream& operator<< (std::ostream&, elementDlugosci const&);
private:
	unsigned osobnik;
	unsigned dlugosc;
};

