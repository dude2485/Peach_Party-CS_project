#ifndef ACTOR_H_
#define ACTOR_H_

#include "GraphObject.h"
#include "StudentWorld.h"

class GameObject : public GraphObject {
public:
	GameObject(StudentWorld* sw, int imageID, int startX, int startY, int dir, int depth, double size);
	virtual ~GameObject() {}
	virtual void do_something() = 0;
	virtual bool is_a_square() const = 0;
	virtual bool can_be_hit_by_vortex() const = 0;
	virtual void hit_by_vortex();  // tell an game object it has been hit by a vortex
	bool is_active() const { return active; }        // is a game object still alive or should it be removed from game?
	void set_inactive() { active = false; }
	StudentWorld* get_ptr_to_student_world() { return sw_; }

	bool validDirection(int dir);
	void force_walk_direction(int dir, int angle);
	virtual void setWalk(int angle);
	bool onSquare();

private:
	StudentWorld* sw_;
	bool active = true;
};

class Player : public GameObject {
public:
	Player(StudentWorld* sw, int imageID, int startX, int startY, int player_number);
	virtual ~Player() {}
	virtual void do_something();
	void reset_walk_direction() { walkDir = right; }
	void reset_coins() { coins = 0; }
	void reset_stars() { stars = 0; }
	bool is_a_square() const { return false; }
	bool can_be_hit_by_vortex() const { return false; }
	int get_dice() const { return dice; }    // used to display stats on status line
	int get_stars() const { return stars; }   // used to display stats on status line
	int get_coins() const { return coins; }   // used to display stats on status line
	bool has_vortex() const { return vortex; } // used to display stats on status line
	bool is_walking() const { return walk_state; }
	
	void adjust_stars(const int this_much);
	void adjust_coins(const int this_much);
	void swap_positions(Player* p);  // with other player
	void swap_stars(Player* p);  // with other player
	void swap_coins(Player* p);  // with other player
	void equip_with_vortex_projectile() { vortex = true; }
	void teleport_me_to_random_sq();
	bool is_newSpace() const { return newSpace; }
	void set_newSpace(bool value) { newSpace = value; }

	
	void set_onDir(bool value) { onDir = value; }
	int get_pNum() { return pNum; }
	void setWalk(int angle) { walkDir = angle; }
	bool enemySpace() { return onEnemy; }
	void set_EnemySpace(bool val) { onEnemy = val; }

private:
	int pNum;
	int coins = 0;
	int stars = 0;
	int dice = 0;
	bool vortex = false;
	bool walk_state = false;
	int walkDir = right;
	int ticks_to_move = 0;
	bool newSpace = false;
	bool onDir = false;

	bool onEnemy = false;
	bool justTP = false;
};

class ActivatingObject : public GameObject {
public:
	ActivatingObject(StudentWorld* sw, int imageID, int startX, int startY, int dir, int depth, double size);
	virtual ~ActivatingObject() {}
	virtual void do_something();
};

class ActivateOnPlayer : public ActivatingObject {
public:
	ActivateOnPlayer(StudentWorld* sw, int imageID, int startX, int startY,
		int dir, int depth, double size, bool activate_when_go_lands);
	virtual ~ActivateOnPlayer() {}
	virtual void do_something();
private:
	virtual void doDiffStuff(Player* p) = 0;
};

class Vortex : public ActivatingObject {
public:
	Vortex(StudentWorld* sw, int imageID, int startX, int startY, int dir);
	virtual ~Vortex() {}
	bool is_a_square() const { return false; }
	bool can_be_hit_by_vortex() const { return false; }
	std::vector<GameObject*> do_i_activate();
	virtual void do_something();

};

class StarSquare : public ActivateOnPlayer {
public:
	StarSquare(StudentWorld* sw, int imageID, int startX, int startY);
	virtual ~StarSquare() {}
	bool is_a_square() const { return true; }
	bool can_be_hit_by_vortex() const { return false; }
private:
	virtual void doDiffStuff(Player* p);
};

class CoinSquare : public ActivateOnPlayer {
public:
	CoinSquare(StudentWorld* sw, int imageID, int startX, int startY, int adjust_coins_by);
	virtual ~CoinSquare() {}
	bool is_a_square() const { return true; }
	bool can_be_hit_by_vortex() const { return false; }
private:
	virtual void doDiffStuff(Player* p);
	int coins;
};

class DirectionalSquare : public ActivateOnPlayer {
public:
	DirectionalSquare(StudentWorld* sw, int imageID, int startX, int startY, int dir);
	virtual ~DirectionalSquare() {}
	bool is_a_square() const { return true; }
	bool can_be_hit_by_vortex() const { return false; }
private:
	virtual void doDiffStuff(Player* p);
};

class BankSquare : public ActivateOnPlayer {
public:
	BankSquare(StudentWorld* sw, int imageID, int startX, int startY);
	virtual ~BankSquare() {}
	bool is_a_square() const { return true; }
	bool can_be_hit_by_vortex() const { return false; }
private:
	virtual void doDiffStuff(Player* p);
};

class EventSquare : public ActivateOnPlayer {
public:
	EventSquare(StudentWorld* sw, int imageID, int startX, int startY);
	virtual ~EventSquare() {}
	bool is_a_square() const { return true; }
	bool can_be_hit_by_vortex() const { return false; }
private:
	virtual void doDiffStuff(Player* p);
};

class Enemy : public ActivateOnPlayer {
public:
	Enemy(StudentWorld* sw, int imageID, int startX, int startY,
		int dir, int depth, double size, bool activate_when_go_lands, int num_sq_to_move, int number_of_ticks_to_pause);
	virtual ~Enemy() {}
	virtual void do_something();
	bool is_a_square() const { return false; }
	bool can_be_hit_by_vortex() const { return true; }
	void setWalk(int angle) { walkDir = angle; }
	void hit_by_vortex();  // called when enemy is hit by a vortex projectile (called by vortex projectile)

	bool actEnemy() { return active_enemy; }
	void setEnemy(bool val) { active_enemy = val; }
	bool walk() { return walk_state; }
	void changePause(int val) { pauseTicks += val; }
	int getPause() { return pauseTicks; }
	void setPause(int val) { pauseTicks = val; }

	int getTrav() { return travDist; }
	void setTrav(int val) { travDist = val; }

	int getTicks() { return ticks_to_move; }
	void setTicks(int val) { ticks_to_move = val; }
	void changeTicks(int val) { ticks_to_move += val; }

	int getWalk() { return walkDir; }

	void setWalkState(bool val) { walk_state = val; }

private:
	bool walk_state = false;
	int walkDir;
	int travDist;
	int pauseTicks;
	int ticks_to_move;
	bool active_enemy = false;
};

class DroppingSquare : public ActivateOnPlayer {
public:
	DroppingSquare(StudentWorld* sw, int imageID, int startX, int startY);
	virtual ~DroppingSquare() {}
	bool is_a_square() const { return true; }
	bool can_be_hit_by_vortex() const { return false; }
private:
	virtual void doDiffStuff(Player* p);
};

class Bowser : public Enemy {
public:
	Bowser(StudentWorld* sw, int imageID, int startX, int startY);
	virtual ~Bowser() {}
	virtual void do_something();
private:
	virtual void doDiffStuff(Player* p);
};

class Boo : public Enemy {
public:
	Boo(StudentWorld* sw, int imageID, int startX, int startY);
	virtual ~Boo() {}
private:
	virtual void doDiffStuff(Player* p);
};

#endif // ACTOR_H_