#include "pch.h"
using namespace std;



void menu()
{
	#pragma region ustawienia
	float czasStopu = 10;
	//populacja pocz¹tkowa
	unsigned populacja = 100;
	//metoda krzy¿owania pmx/ex
	bool pmx = true;
	//metoda mutacji inwersja/transpozycja
	bool inwersja = true;
	float wspolczynnikKrzyzowania = 0.8;
	float wspolczynnikMutacji = 0.01;

	srand(time(NULL));
	#pragma endregion
	Zegar zegar;                // czasomierz
	Graf *graf = NULL;			// wskaznik na aktualny graf

	char wybor = 0;
	do
	{
		#pragma region wypisanie menu
		cout << "1 - Wczytaj dane" << endl;
		cout << "2 - Czas zatrzymania (" << czasStopu << "s)" << endl;
		cout << "3 - Wielkosc populacji poczatkowej (" << populacja << ")" << endl;
		cout << "4 - Wspolczynnik krzyzowania (" << wspolczynnikKrzyzowania << ")" << endl;
		cout << "5 - Wspolczynnik mutacji (" << wspolczynnikMutacji << ")" << endl;
		cout << "6 - Wybor krzyzowania";
		if (pmx)
		{
			cout << " (PMX)" << endl;
		}
		else
		{
			cout << " (EX)" << endl;
		}
		cout << "7 - Wybor mutacji(";
		if (inwersja)
		{
			cout << "inwersja)" << endl;
		}
		else
		{
			cout << "transpozycja)" << endl;
		}
		cout << "8 - Algorytm genetyczny" << endl;
		cout << "Wyjdz: 0" << endl;
		cout << "Wybierz: ";
		cin >> wybor;
		cout << endl;
		#pragma endregion
		switch (wybor)
		{
		case '1':
		{
			string nazwaPliku;
			string odczyt;
			ifstream plik;
			cout << "Podaj nazwe pliku: ";
			cin >> nazwaPliku;

			plik.open(nazwaPliku.c_str());
			if (plik.is_open())
			{
				do
				{
					plik >> odczyt;
				}
				while (odczyt != "DIMENSION:");

				plik >> odczyt;
				int vertex = atoi(odczyt.c_str());

				do
				{
					plik >> odczyt;
				}
				while (odczyt != "EDGE_WEIGHT_FORMAT:");

				plik >> odczyt;
				if (odczyt == "FULL_MATRIX")
				{
					if (graf != NULL)
					{
						delete graf;
					}

					graf = new Graf(vertex);

					do
					{
						plik >> odczyt;
					}
					while (odczyt != "EDGE_WEIGHT_SECTION");

					for (int i = 0; i < vertex; i++)
					{
						for (int j = 0; j < vertex; j++)
						{
							plik >> odczyt;
							int koszt = atoi(odczyt.c_str());

							if (i != j)
							{
								graf->dodajWierzcholek(i, j, koszt);
							}

						}
					}

					cout << "Wczytano - liczba wierzcholkow: " << vertex << endl;
					cout << endl;
				}
				else
				{
					cout << "Nieobslugiwany format " << odczyt << endl;
					cout << endl;
				}

				plik.close();
			}
			else
			{
				cout << "Brak pliku " << nazwaPliku << endl;
				cout << endl;
			}
		}
		break;

		case '2':
		{
			cout << "Poprzedni czas pracy: " << czasStopu << endl;
			cout << "Podaj nowy czas: ";
			cin >> czasStopu;
			if (czasStopu < 0)
			{
				cout << "Czas stopu nie moze byc mniejszy od 0!" << endl;
				czasStopu = 0;
			}
			cout << endl;
		}
		break;

		case '3':
		{
			cout << "Poprzedni populacja poczatkowa: " << populacja << endl;
			cout << "Podaj nowa populacje: ";
			cin >> populacja;
			if (populacja < 2)
			{
				cout << "Populacja nie moze byc mniejsza od 2! Ustawiono 100";
				populacja = 100;
			}
			cout << endl;
		}
		break;

		case '4':
		{
			cout << "Poprzedni wspolczynnik krzyzowania: " << wspolczynnikKrzyzowania << endl;
			cout << "Podaj nowa wspolczynnik krzyzowania: ";
			cin >> wspolczynnikKrzyzowania;
			if (wspolczynnikKrzyzowania > 1)
			{
				wspolczynnikKrzyzowania = 1;
				cout << "Wspolczynnik krzyzowania nie moze byc wiekszy od 1!" << endl;
			}
			else if (wspolczynnikKrzyzowania < 0)
			{
				wspolczynnikKrzyzowania = 0;
				cout << "Wspolczynnik krzyzowania nie moze byc mniejszy od 0!" << endl;
			}
			cout << endl;
		}
		break;

		case '5':
		{
			cout << "Poprzedni wspolczynnik mutacji: " << wspolczynnikMutacji << endl;
			cout << "Podaj nowa wspolczynnik mutacji: ";
			cin >> wspolczynnikMutacji;
			if (wspolczynnikMutacji > 1)
			{
				wspolczynnikMutacji = 1;
				cout << "Wspolczynnik mutacji nie moze byc wiekszy od 1!" << endl;
			}
			else if (wspolczynnikMutacji < 0)
			{
				wspolczynnikMutacji = 0;
				cout << "Wspolczynnik mutacji nie moze byc mniejszy od 0!" << endl;
			}
			cout << endl;
		}
		break;

		case '6':
		{
			pmx = !pmx;
		}
		break;

		case '7':
		{
			inwersja = !inwersja;
		}
		break;

		case '8':
		{
			if (graf != NULL)
			{
				int n = graf->zwrocLiczbeWierzcholkow();
				cout << endl;
				cout << "Populacja poczatkowa: " << populacja << endl;
				cout << "Wspolczynnik krzyzowania: " << wspolczynnikKrzyzowania << endl;
				cout << "Wspolczynnik mutacji: " << wspolczynnikMutacji << endl;
				cout << "Czas stopu: " << czasStopu << " [s]" << endl;
				cout << endl;
				cout << endl;
				zegar.start();
				vector<unsigned> droga(n);
				droga = graf->AG(populacja, pmx, inwersja, wspolczynnikKrzyzowania, wspolczynnikMutacji, czasStopu);
				zegar.stop();

				unsigned drogaOdPoczatku = 0;
				unsigned dlugosc = 0;
				for (int i = 1; i < n; i++)
				{
					dlugosc = graf->zwrocKoszt(droga[i - 1], droga[i]);
					drogaOdPoczatku += dlugosc;
				}
				drogaOdPoczatku += graf->zwrocKoszt(droga[n - 1], droga[0]);
				cout << "Dlugosc trasy: " << drogaOdPoczatku << endl;
				cout << endl;
				cout << "Czas wykonania algorytmu [s]: " << zegar.odczyt() << endl;

			}
			else
			{
				cout << "Brak zaladowanych danych" << endl;
			}
			
		}
		break;


		case '0':
		{
			if (graf != NULL)
			{
				delete graf;
			}
			return;
		}
		break;

		default:
		{
			cout << "Nieprawidlowy wybor" << endl;
			cout << endl;
		}
		}
	}
	while (true);
}