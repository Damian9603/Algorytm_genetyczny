#include "pch.h"
#include "Graf.h"
#include "Zegar.h"
#include "elementTK.h"
#include <cstdlib>
#include <ctime>
#include <algorithm>
#include <queue>
#include <random>

using namespace std;

Graf::Graf(int liczbaWierzcholkow)
{
	this->liczbaWierzcholkow = liczbaWierzcholkow;
	macierzGrafu.resize(liczbaWierzcholkow);

	for (int i = 0; i < liczbaWierzcholkow; i++)
	{
		macierzGrafu[i].resize(liczbaWierzcholkow);

		for (int j = 0; j < liczbaWierzcholkow; j++)
			macierzGrafu[i][j] = 0;
	}

}

Graf::~Graf()
{
}


bool Graf::dodajWierzcholek(unsigned v, unsigned w, unsigned koszt)
{
	if (macierzGrafu[v][w] > 0)
	{
		return false;
	}
	else
	{
		macierzGrafu[v][w] = koszt;
		return true;
	}
}

unsigned Graf::zwrocKoszt(unsigned v, unsigned w)
{
	return macierzGrafu[v][w];
}

unsigned Graf::zwrocLiczbeWierzcholkow()
{
	return liczbaWierzcholkow;
}

void Graf::populacjaPoczatkowa(unsigned ilosc)
{
	populacjaOsobnikow.resize(ilosc);
	for (int i = 0; i < ilosc; i++)
	{
		populacjaOsobnikow[i].resize(liczbaWierzcholkow);
	}
	
	vector<bool> wykorzystane(liczbaWierzcholkow, false);
	for (int i = 0; i < ilosc; i++)
	{
		for (int j = 0; j < liczbaWierzcholkow; j++)
		{
			wykorzystane[j] = false;
		}
		unsigned wierzcholek;
		int j = 0;
		do
		{
			wierzcholek = rand() % liczbaWierzcholkow;
			if (wykorzystane[wierzcholek])
			{
				continue;
			}
			else
			{
				wykorzystane[wierzcholek] = true;
				populacjaOsobnikow[i][j] = wierzcholek;
			}
			j++;
		} while (j<liczbaWierzcholkow);
	}
	dlugoscOsobnikow.resize(ilosc);
	obliczDlugoscOsobnikow(ilosc);
}

void Graf::obliczDlugoscOsobnikow(unsigned pop)
{
	for (int i = 0; i < pop; i++)
	{
		unsigned dlugosc = 0;
		for (int j = 1; j < liczbaWierzcholkow; j++)
		{
			dlugosc += zwrocKoszt(populacjaOsobnikow[i][j - 1], populacjaOsobnikow[i][j]);
		}
		dlugosc += zwrocKoszt(populacjaOsobnikow[i][liczbaWierzcholkow - 1], populacjaOsobnikow[i][0]);
		dlugoscOsobnikow[i].ustawOsobnik(i);
		dlugoscOsobnikow[i].ustawDlugosc(dlugosc);
	}
}

void Graf::ranking(unsigned pop)
{
	sort(dlugoscOsobnikow.begin(), dlugoscOsobnikow.end());
}

vector<unsigned> Graf::PMX(vector<unsigned> rodzic1, vector<unsigned> rodzic2)
{
	//losowanie początkowego i końcowego punktu krzyżowania
	int poczatek = rand() % (liczbaWierzcholkow / 2);
	int koniec = (rand() % (liczbaWierzcholkow / 2)) + (liczbaWierzcholkow / 2);
	int dlugosc = koniec - poczatek;
	//wektor potmka
	vector<unsigned> potomek (liczbaWierzcholkow, INT32_MAX);
	//wektor wykorzystanych wierzchołków w potomku
	vector<bool> wykorzystane(liczbaWierzcholkow,false);
	//kopiowanie wyznaczonego fragmentu rodzica 1 do potomka
	for (int i = poczatek; i < koniec + 1; i++)
	{
		potomek[i] = rodzic1[i];
		wykorzystane[potomek[i]] = true;
	}


	#pragma region Tworzenie par
	vector<vector<unsigned>> pary(dlugosc + 1);
	for (int i = 0; i < dlugosc + 1; i++)
	{
		pary[i].resize(2);
	}
	int j = 0;
	for (int i = poczatek; i < koniec + 1; i++)
	{
		pary[j][0] = rodzic2[i];
		pary[j][1] = rodzic1[i];
		j++;
	}
	#pragma endregion
	//wpisywanie par do potomka
	for (int i = 0; i < dlugosc + 1; i++)
	{
		bool wyk = wykorzystane[pary[i][0]];
		//sprawdzanie czy wierzchołek nie został już wpisany do potomka
		if (wyk ==false)
		{
			while (wykorzystane[pary[i][0]]==false)
			{
				int miejsce = -1;
				for (int j = 0; j < liczbaWierzcholkow; j++)
				{
					//odnajdywanie indeksu, gdzie znajduje się druga liczba pary w rodzicu 1
					if (pary[i][1] == rodzic2[j])
					{
						miejsce = j;
						break;
					}
				}
				//wpisanie do potomka pierwszej liczby z pary, jeżeli miejsce jest puste
				if (potomek[miejsce] == INT32_MAX)
				{
					potomek[miejsce] = pary[i][0];
					wykorzystane[pary[i][0]] = true;
				}
				//dalsze poszukiwanie odpowiedniego miejsca
				else
				{
					pary[i][1] = rodzic1[miejsce];
				}
			}
		}

		
	}
	j = 0;
	//wpisywanie pozostałych wierzchołków
	do
	{
		//jeżeli wierzchołek nie został wpisany do potomka
		if (!wykorzystane[rodzic2[j]])
		{
			bool stop = false;
			int i = j;
			while (potomek[i] != INT32_MAX) 
			{
				i++;
				if (i == liczbaWierzcholkow && stop == true)
				{
					break;
				}
				else if (i == liczbaWierzcholkow)
				{
					i = 0;
					stop = true;
				}
			}
			//wpisywanie wierzchołka jeżeli jest to możliwe
			if (i < liczbaWierzcholkow)
			{
				potomek[i] = rodzic2[j];
				wykorzystane[rodzic2[j]] == true;
			}
			
		}
		j++;
		//przerywanie pętli, gdy wszystie wierzchołki zostały sprawdzone i wpisane
		if (j == liczbaWierzcholkow)
		{
			break;
		}
	} while (wszystko(wykorzystane)==false);

	return potomek;
}

vector<unsigned> Graf::EX(vector<unsigned> rodzic1, vector<unsigned> rodzic2)
{
	#pragma region tworzenie tablicy krawędzi
	vector<vector<elementTK>> tablicaKrawedzi;
	tablicaKrawedzi.resize(liczbaWierzcholkow);
	for (int i = 0; i < liczbaWierzcholkow; i++)
	{
		tablicaKrawedzi[i].resize(2);
	}
	for (int i = 0; i < liczbaWierzcholkow; i++)
	{
		unsigned miejsce1 = INT32_MAX;
		unsigned miejsce2 = INT32_MAX;
		for (int j = 0; j < liczbaWierzcholkow; j++)
		{
			if (rodzic1[j] == i)
			{
				miejsce1 = j;
				break;
			}
		}

		for (int j = 0; j < liczbaWierzcholkow; j++)
		{
			if (rodzic2[j] == i)
			{
				miejsce2 = j;
				break;
			}
		}

		if (miejsce1 == 0)
		{
			tablicaKrawedzi[i][0].ustawWierzcholek(rodzic1[liczbaWierzcholkow - 1]);
			tablicaKrawedzi[i][1].ustawWierzcholek(rodzic1[miejsce1 + 1]);
		}
		else if (miejsce1 == liczbaWierzcholkow - 1)
		{
			tablicaKrawedzi[i][0].ustawWierzcholek(rodzic1[miejsce1 - 1]);
			tablicaKrawedzi[i][1].ustawWierzcholek(rodzic1[0]);
		}
		else
		{
			tablicaKrawedzi[i][0].ustawWierzcholek(rodzic1[miejsce1 - 1]);
			tablicaKrawedzi[i][1].ustawWierzcholek(rodzic1[miejsce1 + 1]);
		}
		
		if (miejsce2 == 0)
		{
			if (tablicaKrawedzi[i][0].zwrocWierzcholek() == rodzic2[liczbaWierzcholkow - 1])
			{
				tablicaKrawedzi[i][0].ustawPlus();
			}
			else if (tablicaKrawedzi[i][1].zwrocWierzcholek() == rodzic2[liczbaWierzcholkow - 1])
			{
				tablicaKrawedzi[i][1].ustawPlus();
			}
			else
			{
				elementTK el;
				el.ustawWierzcholek(rodzic2[liczbaWierzcholkow - 1]);
				tablicaKrawedzi[i].push_back(el);
			}


			if (tablicaKrawedzi[i][0].zwrocWierzcholek() == rodzic2[miejsce2 + 1])
			{
				tablicaKrawedzi[i][0].ustawPlus();
			}
			else if (tablicaKrawedzi[i][1].zwrocWierzcholek() == rodzic2[miejsce2 + 1])
			{
				tablicaKrawedzi[i][1].ustawPlus();
			}
			else
			{
				elementTK el;
				el.ustawWierzcholek(rodzic2[miejsce2 + 1]);
				tablicaKrawedzi[i].push_back(el);
			}
		}
		else if (miejsce2 == liczbaWierzcholkow - 1)
		{
			if (tablicaKrawedzi[i][0].zwrocWierzcholek() == rodzic2[miejsce2 - 1])
			{
				tablicaKrawedzi[i][0].ustawPlus();
			}
			else if (tablicaKrawedzi[i][1].zwrocWierzcholek() == rodzic2[miejsce2 - 1])
			{
				tablicaKrawedzi[i][1].ustawPlus();
			}
			else
			{
				elementTK el;
				el.ustawWierzcholek(rodzic2[miejsce2 - 1]);
				tablicaKrawedzi[i].push_back(el);
			}


			if (tablicaKrawedzi[i][0].zwrocWierzcholek() == rodzic2[0])
			{
				tablicaKrawedzi[i][0].ustawPlus();
			}
			else if (tablicaKrawedzi[i][1].zwrocWierzcholek() == rodzic2[0])
			{
				tablicaKrawedzi[i][1].ustawPlus();
			}
			else
			{
				elementTK el;
				el.ustawWierzcholek(rodzic2[0]);
				tablicaKrawedzi[i].push_back(el);
			}

		}
		else
		{
			if (tablicaKrawedzi[i][0].zwrocWierzcholek() == rodzic2[miejsce2 - 1])
			{
				tablicaKrawedzi[i][0].ustawPlus();
			}
			else if (tablicaKrawedzi[i][1].zwrocWierzcholek() == rodzic2[miejsce2 - 1])
			{
				tablicaKrawedzi[i][1].ustawPlus();
			}
			else
			{
				elementTK el;
				el.ustawWierzcholek(rodzic2[miejsce2 - 1]);
				tablicaKrawedzi[i].push_back(el);
			}


			if (tablicaKrawedzi[i][0].zwrocWierzcholek() == rodzic2[miejsce2 + 1])
			{
				tablicaKrawedzi[i][0].ustawPlus();
			}
			else if (tablicaKrawedzi[i][1].zwrocWierzcholek() == rodzic2[miejsce2 + 1])
			{
				tablicaKrawedzi[i][1].ustawPlus();
			}
			else
			{
				elementTK el;
				el.ustawWierzcholek(rodzic2[miejsce2 + 1]);
				tablicaKrawedzi[i].push_back(el);
			}

		}

	}
	#pragma endregion

	vector<unsigned> potomek (liczbaWierzcholkow);
	unsigned aktualny = rand() % liczbaWierzcholkow;
	potomek[0] = aktualny;
	potomek[liczbaWierzcholkow - 1] = INT32_MAX;
	int i = 1;
	vector<bool> wykorzystane (liczbaWierzcholkow, false);
	wykorzystane[aktualny] = true;
	while (potomek[liczbaWierzcholkow - 1] == INT32_MAX)
	{
		//usunięcie odniesień do dodanego wierzchołka do drogi z tablicy krawędzi
		for (int j = 0; j < liczbaWierzcholkow; j++)
		{
			for (int k = 0; k < tablicaKrawedzi[j].size(); k++)
			{
				if (tablicaKrawedzi[j][k].zwrocWierzcholek() == aktualny)
				{
					tablicaKrawedzi[j].erase(tablicaKrawedzi[j].begin() + k);
					break;
				}
			}
		}
		bool skip = false;
		for (int j = 0; j < tablicaKrawedzi[aktualny].size(); j++)
		{
			//sprawdzanie czy w tablicy krawędzi dla danego wierzchołka jest plus
			if (tablicaKrawedzi[aktualny][j].zwrocPlus())
			{
				skip = true;
				aktualny = tablicaKrawedzi[aktualny][j].zwrocWierzcholek();
			}
		}
		if (skip)
		{

		}
		else
		{
			//jeżeli nie ma plusa i wektor krawędzi nie jest pusty to sprawdzamy, który wierzchołek ma najkrótszy wektor krawędzi
			if (tablicaKrawedzi[aktualny].size() > 0)
			{
				vector<unsigned> dlugosc(tablicaKrawedzi[aktualny].size());
				for (int j = 0; j < tablicaKrawedzi[aktualny].size(); j++)
				{
					dlugosc[j] = tablicaKrawedzi[tablicaKrawedzi[aktualny][j].zwrocWierzcholek()].size();
				}
				unsigned min = INT32_MAX;
				unsigned dlug = INT32_MAX;
				for (int j = 0; j < tablicaKrawedzi[aktualny].size(); j++)
				{
					if (dlug > dlugosc[j])
					{
						dlug = dlugosc[j];
						min = j;
					}
				}
				aktualny = tablicaKrawedzi[aktualny][min].zwrocWierzcholek();
			}
			//jeżeli nie ma plusa i wektor krawędzi jest pusty to losujemy kolejny wierzchołek
			else
			{
				unsigned los = INT32_MAX;
				do
				{
					los = rand() % liczbaWierzcholkow;
				} while (wykorzystane[los] == true);
				aktualny = los;
			}
			
		}
		potomek[i] = aktualny;
		i++;
		wykorzystane[aktualny] = true;


	}

	return potomek;
}

vector<unsigned> Graf::inwersja(vector<unsigned> droga)
{
	//losowanie początka i końca
	unsigned poczatek = rand() % liczbaWierzcholkow;
	unsigned koniec = rand() % liczbaWierzcholkow;
	while (koniec == poczatek)
	{
		koniec = rand() % liczbaWierzcholkow;
	}
	if (poczatek > koniec)
	{
		unsigned pom = koniec;
		koniec = poczatek;
		poczatek = pom;
	}
	unsigned dlugosc = koniec - poczatek;
	dlugosc /= 2;
	int j = 0;
	//zamiana kolejnośći
	for (int i = poczatek; i < (koniec - dlugosc); i++)
	{
		unsigned pom = droga[i];
		droga[i] = droga[koniec - j];
		droga[koniec - j] = pom;
		j++;
	}
	return droga;
}

vector<unsigned> Graf::transpozycja(vector<unsigned> droga)
{
	//losowanie wierzchołków do zamiany
	unsigned pierwszy = rand() % liczbaWierzcholkow;
	unsigned drugi = rand() % liczbaWierzcholkow;
	while (drugi == pierwszy)
	{
		drugi = rand() % liczbaWierzcholkow;
	}
	//zamiana wierzchołków
	unsigned pom = droga[pierwszy];
	droga[pierwszy] = droga[drugi];
	droga[drugi] = pom;
	return droga;
}

vector<unsigned> Graf::AG(unsigned pop, bool pmx, bool inw, float wspKrzyz, float wspMut, float czasStopu)
{
	unsigned rozmiarElity = pop - (pop * wspKrzyz);
	if (rozmiarElity % 2 != pop % 2)
	{
		rozmiarElity++;
	}
	populacjaPoczatkowa(pop);
	vector<unsigned> tablicaRankingu(pop);
	tablicaRankingu[dlugoscOsobnikow[0].zwrocOsobnik()] = pop;
	for (int i = 1; i < pop; i++)
	{
		tablicaRankingu[i] = pop - i + tablicaRankingu[i - 1];
	}
	bool czyPozostalCzas = true;
	Zegar zegar;
	zegar.start();
	while (czyPozostalCzas)
	{
		//ustalanie maksymalnej wartości w rankingu
		unsigned rankingMax = (pop*pop + pop) / 2;
		random_device rd;
		default_random_engine eng{ rd() };
		uniform_int_distribution<> dist(0, rankingMax);
		ranking(pop);
		vector<vector<unsigned>> elita(rozmiarElity);
		//tworzenie elity elita
		for (int i = 0; i < rozmiarElity; i++)
		{
			elita[i] = populacjaOsobnikow[dlugoscOsobnikow[i].zwrocOsobnik()];
		}



		//krzyżowanie
		for (int i = rozmiarElity; i < pop; i += 2)
		{
			//losowanie indeksu pierwszego rodzica w rankingu
			unsigned indeks1 = dist(eng);
			unsigned rodzic1Numer;
			unsigned rodzic2Numer;
			vector<unsigned> rodzic1 = populacjaOsobnikow[dlugoscOsobnikow[pop - 1].zwrocOsobnik()];
			vector<unsigned> rodzic2 = populacjaOsobnikow[dlugoscOsobnikow[pop - 1].zwrocOsobnik()];
			for (int j = 0; j < pop; j++)
			{
				//sprawdzanie która droga staje się pierwszym rodzicem
				if (indeks1 <= tablicaRankingu[j])
				{
					rodzic1Numer = j;
					break;
				}
			}
			do
			{
				unsigned indeks2 = dist(eng);
				for (int j = 0; j < pop; j++)
				{
					if (indeks2 <= tablicaRankingu[j])
					{
						//sprawdzanie która droga staje się drugim rodzicem
						rodzic2Numer = j;
						break;
					}
				}
			} while (rodzic1Numer == rodzic2Numer);
			rodzic1 = populacjaOsobnikow[dlugoscOsobnikow[rodzic1Numer].zwrocOsobnik()];
			rodzic2 = populacjaOsobnikow[dlugoscOsobnikow[rodzic2Numer].zwrocOsobnik()];
			//krzyżowanie metodą PMX
			if (pmx == true)
			{
				populacjaOsobnikow[i] = PMX(rodzic1, rodzic2);
				populacjaOsobnikow[i + 1] = PMX(rodzic2, rodzic1);
			}
			//krzyżowanie metodą EX
			else
			{
				populacjaOsobnikow[i] = EX(rodzic1, rodzic2);
				populacjaOsobnikow[i + 1] = EX(rodzic2, rodzic1);
			}

		}
		//wpisywanie elity do następnego pokolenia
		for (int i = 0; i < rozmiarElity; i++)
		{
			populacjaOsobnikow[i] = elita[i];
		}

		ranking(pop);
		zegar.stop();
		//sprawdzanie warunku stopu
		if (zegar.odczyt() > czasStopu)
		{
			czyPozostalCzas = false;
		}
		//mutacja
		for (int i = 0; i < (pop*wspMut); i++)
		{
			unsigned losMut = rand() % pop;
			//mutacja przez inwersję
			if (inw)
			{
				populacjaOsobnikow[losMut] = inwersja(populacjaOsobnikow[losMut]);
			}
			//mutacja przez transpozycję
			else
			{
				populacjaOsobnikow[losMut] = transpozycja(populacjaOsobnikow[losMut]);
			}
		}

		obliczDlugoscOsobnikow(pop);
		
		


	}
	return populacjaOsobnikow[0];
}


//metoda sprawdzająca czy wszytkie wierzchołki zostały wykorzystane
bool Graf::wszystko(vector<bool> wyk)
{
	for (int i = 0; i < liczbaWierzcholkow; i++)
	{
		if (wyk[i] == false)
		{
			return false;
		}
	}
	return true;
}