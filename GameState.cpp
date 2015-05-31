#include "GameState.cpp"
int GameState::evaluate(int depth)
{
	moves m;
	cout << alfabeta(depth, -5, 20, false, computerColor, 0,0, m);
	return 0;
}
// wypisuje cala plansze w razie potrzeby
void GameState::printAll()
{
	for (int i = 0 ; i < 8; i++)
	{
		for(int j = 0 ; j < 8; j++)
		{
			cout << ((field[i][j].color == 0) ? ' ' : field[i][j].color) << ((j == 7) ? '\n' : ' ');
		}
	}
}


//sprawdza wszystkie możliwości ruchu i wykonuje po kolei 
int GameState::alfabeta(int depth, int alpha, int beta, bool max_min, char moving_color, int move_number, int field_number, moves& m)
{
	//printAll();
	if(blackPieces == 0 || whitePieces == 0 || depth == 0) // <-- jesteśmy w liściu obecnego drzewa gry
	{ 
		if(blackPieces == 0)
		{
			if(playerColor == 'b')
				return -1000000;
			else
				return 1000000;
		}
		if(whitePieces == 0)
		{
			if(playerColor == 'w')
				return -1000000;
			else
				return 1000000;
		}
		if(depth == 0)
			return stateValue();
		
	}
	char op_color = (moving_color == 'b') ? 'w' : 'b' ;
	int i, j;
	vector<int> kings = (moving_color == 'b') ? bKings : wKings;
	bool beating = false;// <-- zmienna informująca o możliwości bicia
	for(i = 1 ; i < 7 ; i++)// <-- sprawdzenie pionków przeciwnika które nie są na krawędziach planszy (mogą byc bite)
	{
		for(j = 2 - ((i+1)%2) ; j < 7 ;j += 2)
		{
			if(field[i][j].color == op_color)
			{
				beating = (possibleBeats(depth, alpha, beta, max_min, i, j,moving_color, op_color, field_number, move_number,m)) ? true : beating;
				if(alpha >= beta)
				{
					return (max_min) ? alpha : beta;
				}
			}
		}
	}
	if(!kings.empty())// <--jeśli mamy damki, sprawdzamy ich możliwość bicia
	{
		for(unsigned int k = 0; k < kings.size() ; k += 2)
		{
			beating = (possibleRoyalBeats(depth, alpha, beta, max_min, kings[k], kings[k+1], moving_color, op_color, field_number, move_number,m)) ? true : beating;
			if(alpha >= beta) return (max_min) ? alpha : beta;	
		}
	}

	if(!beating)// <-- nie ma opcji bicia
	{	
		
		int start_value = (moving_color == 'b') ? 0 : 1 ;
		int end_value = (moving_color == 'b')  ? 7 : 8;
		int moving_value = (moving_color == 'b') ? 1 : -1;
		int i, j;
		for(i = start_value ; i < end_value ; ++i)
		{
			for(j = 2-((i+1)%2) ; j < 7 ; j += 2)
			{
				if(field[i][j].color == moving_color && field[i][j].king != true)
				{
					commonCenterMoves(depth, alpha, beta, i, j, moving_value, max_min, moving_color, field_number, move_number,m);
					if(alpha >= beta)
					{

						return (max_min) ? alpha : beta;
					}
				}
			}
		}
		for(i = start_value; i < end_value ; i+=2)
		{
			if(field[i][0].color == moving_color && field[i][0].king != true)
			{
				commonSideMoves(depth, alpha, beta, i, 0, moving_value, max_min, moving_color, field_number, move_number,m);
				if(alpha >= beta)
				{
					return (max_min) ? alpha : beta;
				}
			}
			if(field[i][7].color == moving_color && field[i][7].king != true)
			{
				commonSideMoves(depth, alpha, beta, i, 7, moving_value, max_min, moving_color, field_number, move_number,m);
				if(alpha >= beta)
				{
					return (max_min) ? alpha : beta;
				}
			}
		}
		for(unsigned i = 0 ; i < kings.size(); i+=2)
		{
			royalMove(depth, alpha, beta, wKings[i], wKings[i+1], max_min, moving_color, field_number, move_number,m);
		}
		
	}
	
	return (max_min) ? alpha : beta;
}

bool GameState::possibleRoyalBeats(int depth, int& alpha, int& beta, bool max_min,int x, int y, char moving_color, char op_color, int move_number, int field_number, moves& m)
{
	bool anybeats = false;
	for(int i = 1 ; i < 7 ; i++)
	{
		for(int j = 2 - ((i+1)%2) ; j < 7 ;j += 2)
		{
			if(field[i][j].color == op_color && ((i-j) == (x-y) || (i+j) == (x+y))) // <------------- pionek w zaięgu damki
			{
				int iteri = (x < i) ? 1 : -1;
				int iterj = (y < j) ? 1 : -1;
				if(isRoyalBeatPossible(x,y,i,j, iteri, iterj))
				{
					anybeats = true;
					move(x,y,i+iteri, j+iterj, moving_color).beat(i,j).checkNextRoyalBeats(depth, alpha, beta, max_min, i, j, moving_color, op_color, iteri, iterj, field_number, move_number,m);
					if(alpha >=beta) 
					return true;
				}
			}
		}
	}
	return anybeats;
}

void GameState::checkNextRoyalBeats(int depth, int& alpha, int& beta, bool max_min,int x, int y, char moving_color, char op_color, int iteri, int iterj, int move_number, int field_number, moves& m)
{
	int i = iteri;
	int j = iterj;
	int countx = (iteri > 0) ? 7-x : x;
	int county = (iterj > 0) ? 7-y : y;
	int count = min(countx, county);
	
	bool anybeats = false;
	while(count >= 0 && field[x+i][y+j].color == 0)
	{
		anybeats = (possibleRoyalBeats(depth, alpha, beta, max_min, x+i, y+j, moving_color, op_color, field_number, move_number,m)) ? true : anybeats;
		count--;
		i += iteri;
		j += iterj;
		
	}
	i-= iteri;
	j-= iterj;
	if(!anybeats)
	{
		
		while(i != 0 && j !=0)
		{
			if(max_min)
			{
				alpha = max(alpha, move(x+iteri, y+iterj, x+i, y+j, moving_color).alfabeta(depth-1, alpha, beta, !max_min, op_color,-1,-1,m));
				if(alpha >=beta) 
					return;
			}
			else
			{
				beta = max(beta, move(x+iteri, y+iterj, x+i, y+j, moving_color).alfabeta(depth-1, alpha, beta, !max_min, op_color,-1, -1,m));
				if(alpha >=beta) 
					return;
			} 
		}
	}
}

bool GameState::isRoyalBeatPossible(int beatingX, int beatingY, int beatenX, int beatenY, int iteri, int iterj)
{
	int i = iteri;
	int j = iterj;
	for(;beatingX + i != beatenX && beatingY != beatenY; i+=iteri, j+=iterj)
	{
		if(field[beatingX+i][beatingY+j].color != 0)
		return false;
	}
	return (field[beatenX+iteri][beatenY+iterj].color == 0);
}


void GameState::commonBeat(int depth, int& alpha, int& beta, bool max_min,int beatenX, int beatenY, int x, int y, char moving_color, char op_color, int move_number, int field_number, moves& m)
{
	GameState gs = beat(beatenX, beatenY);
	gs = gs.move(beatenX+x, beatenY+y,beatenX-x, beatenY-y, moving_color);
	if(gs.field[beatenX-x][beatenY-y].king == true || !(gs.nextCommonBeats(depth, alpha, beta, max_min, beatenX-x, beatenY-y, moving_color, op_color,move_number, field_number, m)))
	{
		if(max_min)
		{
			alpha = max(alpha, gs.alfabeta(depth-1, alpha, beta, !max_min, op_color, -1, -1,m));
		}
		else
		{
			beta = max(beta, gs.alfabeta(depth-1, alpha, beta, !max_min, op_color, -1, -1,m));
		} 
	}

}


bool GameState::nextCommonBeats(int depth, int& alpha, int& beta, bool max_min,int x, int y, char moving_color, char op_color, int move_number, int field_number, moves& m)
{
	bool anybeats = false;
	if(x > 1 && y > 1 && field[x-1][y-1].color == op_color && field[x-2][y-2].color == 0)
	{
		commonBeat(depth, alpha, beta, max_min, x-1, y-1, 1, 1, moving_color, op_color, field_number, move_number,m);
		anybeats = true;
		if(alpha >= beta) return true;
	}
	if(x > 1 && y < 6 && field[x-1][y+1].color == op_color && field[x-2][y+2].color == 0)
	{
		commonBeat(depth, alpha, beta, max_min, x-1, y+1, 1, -1, moving_color, op_color, field_number, move_number,m);
		anybeats = true;
		if(alpha >= beta) return true;
	}
	if(x < 6 && y < 6 && field[x+1][y+1].color == op_color && field[x+2][y+2].color == 0)
	{
		commonBeat(depth, alpha, beta, max_min, x+1, y+1, -1, -1, moving_color, op_color, field_number, move_number,m);
		anybeats = true;
		if(alpha >= beta) return true;
	}
	if(x < 6 && y > 1 && field[x+1][y-1].color == op_color && field[x+2][y-2].color == 0)
	{
		commonBeat(depth, alpha, beta, max_min, x+1, y-1, -1, 1, moving_color, op_color, field_number, move_number,m);
		anybeats = true;
		if(alpha >= beta) return true;
	}
	return anybeats;
}


bool GameState::possibleBeats(int depth, int& alpha, int& beta, bool max_min,int x, int y, char moving_color, char op_color, int move_number, int field_number, moves& m)
{
	bool beating = false;
	if(isBeatPossible(x,y,1,1,moving_color))
	{
		beating = true;
		commonBeat(depth, alpha, beta, max_min, x, y, 1, 1, moving_color, op_color, field_number, move_number,m);
		if(alpha >= beta) return true;
	}
	if(isBeatPossible(x,y,-1,1,moving_color))
	{
		beating = true;
		commonBeat(depth, alpha, beta, max_min, x, y, -1, 1, moving_color, op_color, field_number, move_number,m);
		if(alpha >= beta) return true;
	}
	if(isBeatPossible(x,y,-1,-1,moving_color))
	{
		beating = true;
		commonBeat(depth, alpha, beta, max_min, x, y, -1, -1, moving_color, op_color, field_number, move_number,m);
		if(alpha >= beta) return true;
	}
	if(isBeatPossible(x,y,1,-1,moving_color))
	{
		beating = true;
		commonBeat(depth, alpha, beta, max_min, x, y, 1, -1, moving_color, op_color, field_number, move_number,m);
		if(alpha >= beta) return true;
	}
	return beating;
}

GameState& GameState::beat(int x, int y)
{
	if(field[x][y].king)
	{
		if(field[x][y].color == 'w')
		{
			removeKing(wKings, x, y);
		}
		if(field[x][y].color == 'b')
		{
			removeKing(bKings, x, y);
		}
	}
	field[x][y].color = 0;
	return *this;
}


void GameState::removeKing(vector<int>& v, int x, int y)
{
	for(unsigned int i = 0 ; i < v.size(); i+=2)
	{
		if(v[i] == x && v[i+1] == y)
		{
			vector<int>::iterator first = v.begin();
			advance( first, i);
			vector<int>::iterator last = first + 1;
			v.erase(first,last);
		}
	}
}

void GameState::addKing(int x, int y)
{
	if(field[x][y].color == 'w')
	{
		wKings.push_back(x);
		wKings.push_back(y);
	}
	if(field[x][y].color == 'b')
	{
		bKings.push_back(x);
		bKings.push_back(y);
	}
}


void GameState::royalMove(int depth, int& alpha, int& beta, int x, int y, bool max_min, char moving_color, int move_number, int field_number, moves& m)
{
	checkDirection(depth, alpha, beta, x, y, max_min, moving_color, -1, -1, ((x<y)?x:y), field_number, move_number,m);
	if(alpha >= beta)return;
	checkDirection(depth, alpha, beta, x, y, max_min, moving_color, -1, 1, (x<7-y)?x:7-y, field_number, move_number,m);
	if(alpha >= beta)return;
	checkDirection(depth, alpha, beta, x, y, max_min, moving_color, 1, 1, (7-x<7-y)?7-x:7-y, field_number, move_number,m);
	if(alpha >= beta)return;
	checkDirection(depth, alpha, beta, x, y, max_min, moving_color, 1, -1, (7-x<y)?7-x:y, field_number, move_number,m);
}

void GameState::checkDirection(int depth, int& alpha, int& beta, int x, int y, bool max_min, char moving_color, int dirX, int dirY, int iterations, int move_number, int field_number, moves& m)
{
	int i = dirX;
	int j = dirY;
	while(iterations > 0 && field[x+i][y+j].color == 0)
	{
		if(max_min)
		{
			alpha = max(alpha, this->move(x,y,x+i,y+j, moving_color).alfabeta(depth -1, alpha, beta, !max_min, moving_color, -1, -1,m));
		}
		else
		{
			beta = min(beta, this->move(x,y,x+i,y+j, moving_color).alfabeta(depth -1, alpha, beta, !max_min, moving_color, -1, -1,m));
		}
		if(alpha >= beta)
		return;
		i += dirX;
		j += dirY;
		iterations--;
	}
}


void GameState::commonCenterMoves(int depth, int& alpha, int& beta, int x, int y, int moving_value, bool max_min, char moving_color, int move_number, int field_number, moves& m)
{	
	char next_color = (moving_color == 'w') ? 'b' : 'w'; 
	if(field[x+moving_value][y-1].color == 0)
	{
		if(max_min)
		{
			alpha = max(alpha, this->move(x, y, moving_value, y-1, moving_color).alfabeta(depth -1, alpha, beta, !max_min, next_color, -1, -1,m));
			
		}
		else
		{
			beta = min(beta, this->move(x, y, moving_value, y-1, moving_color).alfabeta(depth -1, alpha, beta, !max_min, next_color, -1, -1,m));
		}
	}
	if(alpha > beta) return;
	if(field[x+moving_value][y+1].color == 0)
	{
		if(max_min)
		{
			alpha = max(alpha, this->move(x, y, moving_value, y+1, moving_color).alfabeta(depth -1, alpha, beta, !max_min, next_color, -1, -1,m));
		}
		else
		{
			beta = min(beta, this->move(x, y, moving_value, y+1, moving_color).alfabeta(depth -1, alpha, beta, !max_min, next_color, -1, -1,m));
		}
	}	
}

void GameState::commonSideMoves(int depth, int& alfa, int& beta, int x, int y, int moving_value, bool max_min, char moving_color, int move_number, int field_number, moves& m)
{
	char next_color = (moving_color == 'w') ? 'b' : 'w';
	int side_moving_value = (y > 0) ? 6 : 1;
	if(field[x+moving_value][y+side_moving_value].color == 0)
	{
		if(max_min)
		{
			alfa = max(alfa, this->move(x, y, moving_value, side_moving_value, moving_color).alfabeta(depth -1, alfa, beta, !max_min, next_color, -1, -1,m));
		}
		else
		{
			beta = min(beta, this->move(x, y, moving_value, side_moving_value, moving_color).alfabeta(depth -1, alfa, beta, !max_min, next_color, -1, -1,m));
		}
	}
	
}
GameState GameState::move(int x, int y, int i, int j, char moving_color)
{
	GameState gs(*this);
	gs.field[x+i][y+j] = gs.field[x][y];
	gs.field[x][y].color = 0;
	if((moving_color == 'b' && x+i == 7) || (moving_color == 'w' && y+j == 0))
	{
		gs.field[x+i][y+j].king = true;
		gs.addKing(x+i, y+j);
	}
	return gs;
}

//sprawdzenie czy podane pole może być bite
bool GameState :: isBeatPossible(int i, int j, int rj, int ri, char color)
{
	return (field[i+ri][j+rj].color == color && field[i-ri][j-rj].color == 0);
} 
