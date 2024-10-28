#include "StudentWorld.h"
#include "GameConstants.h"

#include "Board.h"
#include "Actor.h"
#include <iostream>

#include <string>
using namespace std;

GameWorld* createStudentWorld(string assetPath)
{
	return new StudentWorld(assetPath);
}

// Students:  Add code to this file, StudentWorld.h, Actor.h, and Actor.cpp

StudentWorld::StudentWorld(string assetPath)
	: GameWorld(assetPath)
{
	bankCoins = 0;
	oss.setf(ios::fixed);
	oss.precision(0);

	peach = nullptr;
	yoshi = nullptr;
}



int StudentWorld::init()
{
	startCountdownTimer(99);  // this placeholder causes timeout after 5 seconds

	int bNum = getBoardNumber();
	string boardFile = assetPath();
	Board gameBoard;

	switch (bNum) {
	case 1:
		boardFile += "board01.txt";
		break;
	case 2:
		boardFile += "board02.txt";
		break;
	case 3:
		boardFile += "board03.txt";
		break;
	case 4:
		boardFile += "board04.txt";
		break;
	case 5:
		boardFile += "board05.txt";
		break;
	case 6:
		boardFile += "board06.txt";
		break;
	case 7:
		boardFile += "board07.txt";
		break;
	case 8:
		boardFile += "board08.txt";
		break;
	case 9:
		boardFile += "board09.txt";
		break;
	}

	Board::LoadResult result = gameBoard.loadBoard(boardFile);

	if (result == Board::load_fail_file_not_found) {
		cerr << "Could not find board data file\n";
		return GWSTATUS_BOARD_ERROR;
	}
	else if (result == Board::load_fail_bad_format) {
		cerr << "Your board was improperly formatted\n";
		return GWSTATUS_BOARD_ERROR;
	}
	else if (result == Board::load_success) {
		cerr << "Successfully loaded board\n";

		for (int x = 0; x < 16; x++) {
			for (int y = 0; y < 16; y++) {
				Board::GridEntry ge = gameBoard.getContentsOf(x, y);
				switch (ge) {
				case Board::empty:
					break;
				case Board::player:
					//cerr << "Location " << x << ", " << y << " has a Peach, Yoshi, and blue coin square\n";
					peach = new Player(this, IID_PEACH, SPRITE_WIDTH * x, SPRITE_HEIGHT * y, 1);
					yoshi = new Player(this, IID_YOSHI, SPRITE_WIDTH * x, SPRITE_HEIGHT * y, 2);
					gameObjects.push_back(new CoinSquare(this, IID_BLUE_COIN_SQUARE, SPRITE_WIDTH * x, SPRITE_HEIGHT * y, 3));
					break;
				case Board::blue_coin_square:
					//cerr << "Location " << x << ", " << y << " has a blue coin square\n";
					gameObjects.push_back(new CoinSquare(this, IID_BLUE_COIN_SQUARE, SPRITE_WIDTH * x, SPRITE_HEIGHT * y, 3));
					break;
				case Board::red_coin_square:
					//cerr << "Location " << x << ", " << y << " has a red coin square\n";
					gameObjects.push_back(new CoinSquare(this, IID_RED_COIN_SQUARE, SPRITE_WIDTH * x, SPRITE_HEIGHT * y, -3));
					break;
				case Board::left_dir_square:
					//cerr << "Location " << x << ", " << y << " has a left dir square\n";
					gameObjects.push_back(new DirectionalSquare(this, IID_DIR_SQUARE, SPRITE_WIDTH * x, SPRITE_HEIGHT * y, 180));
					break;
				case Board::right_dir_square:
					//cerr << "Location " << x << ", " << y << " has a right dir square\n";
					gameObjects.push_back(new DirectionalSquare(this, IID_DIR_SQUARE, SPRITE_WIDTH * x, SPRITE_HEIGHT * y, 0));
					break;
				case Board::up_dir_square:
					//cerr << "Location " << x << ", " << y << " has an up dir square\n";
					gameObjects.push_back(new DirectionalSquare(this, IID_DIR_SQUARE, SPRITE_WIDTH * x, SPRITE_HEIGHT * y, 90));
					break;
				case Board::down_dir_square:
					//cerr << "Location " << x << ", " << y << " has a down dir square\n";
					gameObjects.push_back(new DirectionalSquare(this, IID_DIR_SQUARE, SPRITE_WIDTH * x, SPRITE_HEIGHT * y, 270));
					break;
				case Board::event_square:
					//cerr << "Location " << x << ", " << y << " has an event square\n";
					gameObjects.push_back(new EventSquare(this, IID_EVENT_SQUARE, SPRITE_WIDTH * x, SPRITE_HEIGHT * y));
					break;
				case Board::bank_square:
					//cerr << "Location " << x << ", " << y << " has a bank square\n";
					gameObjects.push_back(new BankSquare(this, IID_BANK_SQUARE, SPRITE_WIDTH * x, SPRITE_HEIGHT * y));
					break;
				case Board::star_square:
					//cerr << "Location " << x << ", " << y << " has a star square\n";
					gameObjects.push_back(new StarSquare(this, IID_STAR_SQUARE, SPRITE_WIDTH * x, SPRITE_HEIGHT * y));
					break;
				case Board::bowser:
					//cerr << "Location " << x << ", " << y << " has a bowser and blue coin square\n";
					gameObjects.push_back(new Bowser(this, IID_BOWSER, SPRITE_WIDTH * x, SPRITE_HEIGHT * y));
					gameObjects.push_back(new CoinSquare(this, IID_BLUE_COIN_SQUARE, SPRITE_WIDTH * x, SPRITE_HEIGHT * y, 3));
					break;
				case Board::boo:
					//cerr << "Location " << x << ", " << y << " has a boo and blue coin square\n";
					gameObjects.push_back(new Boo(this, IID_BOO, SPRITE_WIDTH * x, SPRITE_HEIGHT * y));
					gameObjects.push_back(new CoinSquare(this, IID_BLUE_COIN_SQUARE, SPRITE_WIDTH * x, SPRITE_HEIGHT * y, 3));
					break;
				}


			}
		}

	}

	return GWSTATUS_CONTINUE_GAME;
}

int StudentWorld::move()
{
	// This code is here merely to allow the game to build, run, and terminate after you hit ESC.
	// Notice that the return value GWSTATUS_NOT_IMPLEMENTED will cause our framework to end the game.

	if (timeRemaining() <= 0) {
		playSound(SOUND_GAME_FINISHED);
		if (peach->get_stars() > yoshi->get_stars()) {
			setFinalScore(peach->get_stars(), peach->get_coins());
			return GWSTATUS_PEACH_WON;
		}
		if (peach->get_stars() < yoshi->get_stars()) {
			setFinalScore(yoshi->get_stars(), yoshi->get_coins());
			return GWSTATUS_YOSHI_WON;
		}
		if (peach->get_coins() > yoshi->get_coins()) {
			setFinalScore(peach->get_stars(), peach->get_coins());
			return GWSTATUS_PEACH_WON;
		}
		if (peach->get_coins() < yoshi->get_coins()) {
			setFinalScore(yoshi->get_stars(), yoshi->get_coins());
			return GWSTATUS_YOSHI_WON;
		}

		int tie = randInt(1, 2);
		if (tie == 1) {
			setFinalScore(peach->get_stars(), peach->get_coins());
			return GWSTATUS_PEACH_WON;
		}
		if (tie == 2) {
			setFinalScore(yoshi->get_stars(), yoshi->get_coins());
			return GWSTATUS_YOSHI_WON;
		}
	}

	peach->do_something();
	yoshi->do_something();

	for (unsigned long i = 0; i < gameObjects.size(); i++) {
		gameObjects[i]->do_something();	
	}

	std::vector<GameObject*>::iterator it;

	for (unsigned long i = 0; i < gameObjects.size(); i++) {
		if (!gameObjects[i]->is_active()) {
			it = gameObjects.begin()+i;
			delete gameObjects[i];
			gameObjects.erase(it);
			i--;
		}
	}

	
	setGameStatText(updateStats());
	


	return GWSTATUS_CONTINUE_GAME;
}

void StudentWorld::cleanUp()
{
	std::vector<GameObject*>::iterator it;

	while (!gameObjects.empty()) {
		it = gameObjects.begin();
		delete gameObjects[0];
		gameObjects.erase(it);
	}

	delete peach;
	delete yoshi;
}

bool StudentWorld::is_there_a_square_at_location(int dest_x, int dest_y) const
{
	for (unsigned long i = 0; i < gameObjects.size(); i++) {
		if (gameObjects[i]->getX() == dest_x && gameObjects[i]->getY() == dest_y && gameObjects[i]->is_a_square())
			return true;
	}
	return false;
}

string StudentWorld::updateStats()
{
	oss << "P1 Roll: " << peach->get_dice() << " Stars: " << peach->get_stars() << " $$: " << peach->get_coins() << " ";
	if (peach->has_vortex())
		oss << "VOR ";
	oss << "| Time: " << timeRemaining() << " | Bank: " << get_bank_coins() << " | ";
	oss << "P2 Roll: " << yoshi->get_dice() << " Stars: " << yoshi->get_stars() << " $$: " << yoshi->get_coins();
	if (yoshi->has_vortex())
		oss << " VOR";

	

	string s = oss.str();
	oss.str("");
	return s;
}

void StudentWorld::add_actor(GameObject* actor)
{
	gameObjects.push_back(actor);
}

GameObject* StudentWorld::get_random_square(double x, double y) const
{
	int totalSquares = 0;
	std::vector<int> squarePos;
	for (unsigned long i = 0; i < gameObjects.size(); i++) {
		if (gameObjects[i]->is_a_square() && gameObjects[i]->getX() != x && gameObjects[i]->getY() != y)
		{
			totalSquares++;
			squarePos.push_back(i);
		}
	}

	int targetSquare = squarePos[randInt(1, totalSquares)];

	return gameObjects[targetSquare];
}

Player* StudentWorld::get_other_player(Player* p) const
{
	if (p->get_pNum() == 1)
		return yoshi;
	
	return peach;
}

bool StudentWorld::check_vortex_hit(int x, int y, GameObject*& e)
{
	for (unsigned long i = 0; i < gameObjects.size(); i++) {
		if (gameObjects[i]->can_be_hit_by_vortex() && ((gameObjects[i]->getX() + 15) >= x && gameObjects[i]->getX() <= (x + 15)) && ((gameObjects[i]->getY() + 15) >= y && gameObjects[i]->getY() <= (y + 15)))
		{
			e = gameObjects[i];
			return true;
		}
	}
	return false;
}

GameObject* StudentWorld::get_square_at_location(double x, double y) const
{
	GameObject* s = nullptr;
	for (unsigned long i = 0; i < gameObjects.size(); i++) {
		if (gameObjects[i]->is_a_square() && gameObjects[i]->getX() == x && gameObjects[i]->getY() == y)
		{
			s = gameObjects[i];
			break;
		}
	}

	return s;
}

void StudentWorld::replace_square(int x, int y)
{
	std::vector<GameObject*>::iterator it;
	
	for (unsigned long i = 0; i < gameObjects.size(); i++) {
		if (gameObjects[i]->is_a_square() && gameObjects[i]->getX() == x && gameObjects[i]->getY() == y)
		{
			it = gameObjects.begin() + i;
			delete gameObjects[i];
			gameObjects.erase(it);
			break;
		}
	}

	gameObjects.push_back(new DroppingSquare(this, IID_DROPPING_SQUARE, x, y));
}