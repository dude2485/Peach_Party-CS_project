#ifndef STUDENTWORLD_H_
#define STUDENTWORLD_H_

#include <string>
#include "GameWorld.h"

#include <vector>
#include <sstream>

class GameObject;
class Player;

class StudentWorld : public GameWorld
{
public:
	StudentWorld(std::string assetPath);
	virtual ~StudentWorld() { cleanUp(); }
	virtual int init();
	virtual int move();
	virtual void cleanUp();

	// Add a new player to the StudentWorld
	void add_player(GameObject* new_player) {}

	// Add a new actor to the StudentWorld (not for players, but all other game objects)
	void add_actor(GameObject* actor);

	// Determine if there is a square at the specified location. Used to determine if a gameobject
	// like a player, enemy or vortex can move onto the specified location
	bool is_there_a_square_at_location(int dest_x, int dest_y) const;

	// Get a pointer to the square at the specified location
	GameObject* get_square_at_location(double x, double y) const;

	// get # of coins in the bank
	int get_bank_coins() const { return bankCoins; }

	// add coins to the bank (when player passes over a bank square)
	void deposit_bank_coins(int coins) { bankCoins += coins; }

	// reset # of coins in the bank to zero
	void reset_bank_coins() { bankCoins = 0; }

	// Get a random location on the board that contains a square
	GameObject* get_random_square(double x, double y) const;

	// Given a player object pointer, returns a pointer to the other player object. Used for swapping
	// actions.
	Player* get_other_player(Player* p) const;

	Player* get_player(int pNum) const {
		if (pNum == 1)
			return peach;
		
		return yoshi;
	}

	bool check_vortex_hit(int x, int y, GameObject*& e);

	void replace_square(int x, int y);

	std::string updateStats();

private:
	Player* peach;
	Player* yoshi;

	std::vector<GameObject*> gameObjects;

	int bankCoins;

	std::ostringstream oss;
};

#endif // STUDENTWORLD_H_