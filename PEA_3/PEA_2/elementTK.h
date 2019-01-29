#pragma once
//Klasa pomocnicza wykorzystywana przy EX, w tablicy krawêdzi
class elementTK
{
public:
	elementTK();
	~elementTK();
	void ustawPlus();
	void ustawWierzcholek(unsigned);
	bool zwrocPlus();
	unsigned zwrocWierzcholek();
	friend std::ostream& operator<< (std::ostream&, elementTK const&);
private:
	unsigned wierzcholek;
	bool plus = false;
};

