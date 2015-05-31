#include <iostream>
#include <string>
#include <algorithm>
#include <vector>
using namespace std;
/*
void initModule()
{
	std::cout << "Modul C++ boost::python zaimportowany" << std::endl;
}*/
 /*Klasa reprezentujaca pionek na planszy
 * zawiera w sobie informacje o kolorze pionka oraz czy jest on damka. 
 */
struct SimpleChecker
{
		char color;
		bool king;
		SimpleChecker()
		{
			color = 0;
			king = false;
		}
		//konstruktor z parametrami c - kolor b/w oraz k true/false
		SimpleChecker(char c, bool k): color(c), king(k){}
		//zmiana stanu z normalnego pionka na damke
		
};

/* Klasa przechowująca stan gry posiada kolekcje przechowujaca dane o polach planszy
 * posiada reprezentacje planszy w postaci tablicy  oraz ktory kolor pionkow nalezy do gracza a ktory do komputera
 */
class GameState
{
	private:
		static const int lenght = 8;
		SimpleChecker field[lenght][lenght];
		char playerColor;
		char computerColor;
		int whitePieces;
		int blackPieces;
		vector<int> wKings;
		vector<int> bKings;
		//Ocenia ruch przegladajac rekurencyjnie drzewo gry kilka poziomow w glab i zliczajac punkty przyznane po odpowiednim przeliczeniu za każdego pionka
		//Parametr depth - jesli jest rozny od zera przegladamy mozliwe stany jesli jest rowny 0 zliczamy punkty.
		
		typedef vector < vector < int > > moves;
		
		void printAll();
		int evaluate(int depth);
		int stateValue(){return 0;}
		int alfabeta(int depth, int alpha, int beta, bool max_min, char moving_color, int move_number, int field_number, moves& m);
		//int makePossibleMoves(GameState*, int, int, char);
		bool isBeatPossible(int i, int j, int rj, int ri, char color);
		GameState move(int x, int y, int i, int j, char moving_color);
		void commonCenterMoves(int depth, int& alfa, int& beta, int x, int y, int moving_value, bool max_min, char moving_color, int move_number, int field_number, moves& m);
		void commonSideMoves(int depth, int& alfa, int& beta, int x, int y, int moving_value, bool max_min, char moving_color, int move_number, int field_number, moves& m);
		void royalMove(int depth, int& alpha, int& beta, int x, int y, bool max_min, char moving_color, int move_number, int field_number, moves& m);
		void removeKing(vector<int>& v, int x, int y);
		void addKing(int x, int y);
		void checkDirection(int depth, int& alpha, int& beta, int x, int y, bool max_min, char moving_color, int dirX, int dirY, int iterations, int move_number, int field_number, moves& m);
		void commonBeat(int depth, int& alpha, int& beta, bool max_min,int beatenX, int beatenY, int x, int y, char moving_color, char op_color, int move_number, int field_number, moves& m);
		bool nextCommonBeats(int depth, int& alpha, int& beta, bool max_min,int x, int y, char moving_color, char op_color, int move_number, int field_number, moves& m);
		bool possibleBeats(int depth, int& alpha, int& beta, bool max_min,int x, int y, char moving_color, char op_color, int move_number, int field_number, moves& m);
		void checkNextRoyalBeats(int depth, int& alpha, int& beta, bool max_min,int x, int y, char moving_color, char op_color, int iteri, int iterj, int move_number, int field_number, moves& m);
		bool possibleRoyalBeats(int depth, int& alpha, int& beta, bool max_min,int x, int y, char moving_color, char op_color, int move_number, int field_number, moves& m);
		bool isRoyalBeatPossible(int beatingX, int beatingY, int beatenX, int beatenY, int iteri, int iterj);
		GameState& beat(int x, int y);
	public:
		
		/* konstruktor tworzacy nowa plansze gry
		 * Parametry cc oraz pc przyporzadkowuja kolory pionkow
		 */
		GameState(char cc, char pc);
		
		/* konstruktor zmiany stanu, wprowadzamy ostatni stan oraz dane wykonanej zmiany
		 */
		//GameState(GameState&, int, int, int, int);
		
		
		
		//wykonanie ruchu przez komputer, wywoluje evaluate
		std::string makeMove()
		{
			evaluate(6);
			return "";
		}
		//aktualizuje plansze po wykonaniu ruchu przez gracza
		void updateState(std::string stateChange)
		{
			return;
		}
};

GameState::GameState(char cc, char pc): playerColor(pc), computerColor(cc), whitePieces(12), blackPieces(12) 
{
	for(int i = 0 ; i < 8 ; i++)// <-- sprawdzenie pionków przeciwnika które nie są na krawędziach planszy (mogą byc bite)
	{
		for(int j = ((i+1)%2) ; j < 7 ;j += 2)
		{
			if(i < 3)
			{
				field[i][j].color = 'b';
			}
			else
				if(i > 4)
				{
					field[i][j].color = 'w';
				}
				else
					field[i][j].color = 0;
			field[i][j].king = false;
		}
	}
}

/*

#include <boost/python.hpp>
using namespace boost;
using namespace boost::python;


BOOST_PYTHON_MODULE(GameState)
{
	initModule();
    // Create the Python type object for our extension class and define __init__ function.
    class_<GameState>("GameState", init<char, char>())
        .def("makeMove", &GameState::makeMove)  // Add a regular member function.
        .def("updateState", &GameState::updateState)  // Add invite() as a regular function to the module.
    ;
    class_<SimpleChecker>("SimpleChecker", init<char, bool>())
		.def("makeKing", &SimpleChecker::makeKing)
		.def("getColor", &SimpleChecker::getColor)
		.def("isKing", &SimpleChecker::isKing)
		;
}

*/
