#ifndef GRAF_H
#define GRAF_H
#include "pch.h"
#include <vector>
#include "elementDlugosci.h"

class Graf
{
    public:
        Graf(int liczbaWierzcholkow);
        virtual ~Graf();
        bool dodajWierzcholek(unsigned v, unsigned w, unsigned koszt);
        unsigned zwrocKoszt(unsigned v, unsigned w);
		unsigned zwrocLiczbeWierzcholkow();
		std::vector<unsigned> AG(unsigned pop, bool pmx, bool inw, float wspKrzyz, float wspMut, float czasStopu);

    protected:
        unsigned liczbaWierzcholkow;

    private:
		std::vector<std::vector<unsigned>> macierzGrafu;
		std::vector<std::vector<unsigned>> populacjaOsobnikow;
		std::vector<elementDlugosci> dlugoscOsobnikow;

		bool wszystko(std::vector<bool> wyk);
		void populacjaPoczatkowa(unsigned ilosc);
		void obliczDlugoscOsobnikow(unsigned pop);
		void ranking(unsigned pop);
		std::vector<unsigned> PMX(std::vector<unsigned> rodzic1, std::vector<unsigned> rodzic2);
		std::vector<unsigned> EX(std::vector<unsigned> rodzic1, std::vector<unsigned> rodzic2);
		std::vector<unsigned> inwersja(std::vector<unsigned> droga);
		std::vector<unsigned> transpozycja(std::vector<unsigned> droga);
};

#endif // GRAF_H
