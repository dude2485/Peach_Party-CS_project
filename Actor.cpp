#include "Actor.h"
#include "StudentWorld.h"

// Students:  Add code to this file, Actor.h, StudentWorld.h, and StudentWorld.cpp

//GAME OBJECT FUNCTIONS
GameObject::GameObject(StudentWorld* sw, int imageID, int startX, int startY, int dir, int depth, double size)
	: GraphObject(imageID, startX, startY, dir, depth, size)
{
	sw_ = sw;
}

void GameObject::hit_by_vortex()
{

}

void GameObject::setWalk(int angle)
{

}

//PLAYER FUNCTIONS

Player::Player(StudentWorld* sw, int imageID, int startX, int startY, int player_number)
	: GameObject(sw, imageID, startX, startY, 0, 0, 1.0)
{
	pNum = player_number;
}

void Player::do_something()
{
	if (!is_walking()) {
		//rand direction after tp
		if(justTP == true)
		{
			do {

				int newDir = randInt(1, 4);

				switch (newDir) {
				case 1:
					force_walk_direction(right, right);
					break;
				case 2:
					force_walk_direction(left, left);
					break;
				case 3:
					force_walk_direction(right, up);
					break;
				case 4:
					force_walk_direction(right, down);
					break;
				}
			} while (!validDirection(walkDir));
			justTP = false;
		}

		int act = get_ptr_to_student_world()->getAction(pNum);
		if (act == ACTION_FIRE) {
			if (has_vortex()) {
				Vortex* v = new Vortex(get_ptr_to_student_world(), IID_VORTEX, getX(), getY(), walkDir);
				switch (walkDir) {
				case up:
					v->moveTo(getX(), getY() + SPRITE_HEIGHT);
					break;
				case right:
					v->moveTo(getX() + SPRITE_WIDTH, getY());
					break;
				case down:
					v->moveTo(getX(), getY() - SPRITE_HEIGHT);
					break;
				case left:
					v->moveTo(getX() - SPRITE_WIDTH, getY());
					break;
				}
				
				get_ptr_to_student_world()->add_actor(v);
				get_ptr_to_student_world()->playSound(SOUND_PLAYER_FIRE);
				vortex = false;
				return; //assumption
			}
		}
		else if (act == ACTION_ROLL) {
			dice = randInt(1, 10);
			ticks_to_move = dice * 8;
			walk_state = true;
			set_newSpace(true);
			onEnemy = true;
		}
		else {
			return;
		}
	}

	if (is_walking()) {

		int totalPaths = 0;

		if (validDirection(up))
			totalPaths++;
		if (validDirection(down))
			totalPaths++;
		if (validDirection(right))
			totalPaths++;
		if (validDirection(left))
			totalPaths++;

		if (totalPaths >= 3 && onDir == false) {
			int pathChoice = get_ptr_to_student_world()->getAction(pNum);
			switch (pathChoice) {
			case ACTION_LEFT:
				if (validDirection(left) && walkDir != right) {
					force_walk_direction(left, left);
					totalPaths = 0;
					break;
				}
				return;

			case ACTION_RIGHT:
				if (validDirection(right) && walkDir != left) {
					force_walk_direction(right, right);
					totalPaths = 0;
					break;
				}
				return;

			case ACTION_UP:
				if (validDirection(up) && walkDir != down) {
					force_walk_direction(right, up);
					totalPaths = 0;
					break;
				}
				return;

			case ACTION_DOWN:
				if (validDirection(down) && walkDir != up) {
					force_walk_direction(right, down);
					totalPaths = 0;
					break;
				}
				return;

			default:
				return;

			}
		}

		//directional system
		if (onSquare() && !validDirection(walkDir)) {
			if (walkDir == up || walkDir == down) {
				if (validDirection(right)) {
					force_walk_direction(right, right);
				}	
				else {
					force_walk_direction(left, left);
				}
					
			}
			else if (walkDir == left || walkDir == right) {
				if (validDirection(up))
				{
					force_walk_direction(right, up);
				}
				else
				{
					force_walk_direction(right, down);
				}
			}
		}

		
		
		
		moveAtAngle(walkDir, 2);
		ticks_to_move--;
		if(ticks_to_move == 0)
			walk_state = false;
	}
	onDir = false;
}

void GameObject::force_walk_direction(int dir, int angle)
{
	setDirection(dir);
	setWalk(angle);
}

bool GameObject::validDirection(int dir)
{
	int destX, destY;
	getPositionInThisDirection(dir, 1 * SPRITE_WIDTH, destX, destY);
	bool valid = get_ptr_to_student_world()->is_there_a_square_at_location(destX, destY);
	return valid;
}

bool GameObject::onSquare()
{
	return get_ptr_to_student_world()->is_there_a_square_at_location(getX(), getY());
}

void Player::adjust_coins(const int this_much)
{
	coins += this_much;
	if (coins < 0)
		coins = 0;
}

void Player::adjust_stars(const int this_much)
{
	stars += this_much;
	if (stars < 0)
		stars = 0;
}

void Player::teleport_me_to_random_sq()
{
	GameObject* destSquare = get_ptr_to_student_world()->get_random_square(getX(), getY());
	moveTo(destSquare->getX(), destSquare->getY());
	justTP = true;
}

void Player::swap_coins(Player* p)
{
	int tempCoins = p->get_coins();
	
	p->reset_coins();
	p->adjust_coins(get_coins());

	reset_coins();
	adjust_coins(tempCoins);
}

void Player::swap_stars(Player* p)
{
	int tempStars = p->get_stars();

	p->reset_stars();
	p->adjust_stars(get_stars());

	reset_stars();
	adjust_stars(tempStars);
}

void Player::swap_positions(Player* p)
{
	int tempX, tempY, tempTicks, tempDir, tempWalk;
	bool tempState, tempSpace, temp_onDir;
	
	tempX = p->getX();
	tempY = p->getY();
	tempTicks = p->ticks_to_move;
	tempDir = p->walkDir;
	tempWalk = p->getDirection();
	tempState = p->is_walking();
	tempSpace = p->is_newSpace();
	temp_onDir = p->onDir;

	p->moveTo(getX(), getY());
	p->ticks_to_move = ticks_to_move;
	p->walkDir = walkDir;
	p->setDirection(getDirection());
	p->walk_state = is_walking();
	p->set_newSpace(is_newSpace());
	p->set_onDir(onDir);

	moveTo(tempX, tempY);
	ticks_to_move = tempTicks;
	walkDir = tempDir;
	setDirection(tempWalk);
	walk_state = tempState;
	set_newSpace(tempSpace);
	set_onDir(temp_onDir);

}

//ACTIVATING OBJECT FUNCTIONS

ActivatingObject::ActivatingObject(StudentWorld* sw, int imageID, int startX, int startY, int dir, int depth, double size)
	: GameObject(sw, imageID, startX, startY, dir, depth, size)
{

}

void ActivatingObject::do_something() //check if on player
{

}

//ACTIVATE ON PLAYER FUNCTIONS

ActivateOnPlayer::ActivateOnPlayer(StudentWorld* sw, int imageID, int startX, int startY,
	int dir, int depth, double size, bool activate_when_go_lands)
	:ActivatingObject(sw, imageID, startX, startY, dir, depth, size)
{
}

void ActivateOnPlayer::do_something() //check if on player
{
	if (!is_active())
		return;

	if (get_ptr_to_student_world()->get_player(1)->getX() == this->getX() && get_ptr_to_student_world()->get_player(1)->getY() == this->getY()) {
		doDiffStuff(get_ptr_to_student_world()->get_player(1));
	}
	if (get_ptr_to_student_world()->get_player(2)->getX() == this->getX() && get_ptr_to_student_world()->get_player(2)->getY() == this->getY()) {
		doDiffStuff(get_ptr_to_student_world()->get_player(2));
	}

}

//VORTEX FUNCTIONS

Vortex::Vortex(StudentWorld* sw, int imageID, int startX, int startY, int dir)
	:ActivatingObject(sw, imageID, startX, startY, dir, 0, 1.0)
{

}

void Vortex::do_something()
{
	if (!is_active())
		return;
	moveAtAngle(getDirection(), 2);

	if (getX() < 0 || getX() >= VIEW_WIDTH || getY() < 0 || getY() >= VIEW_HEIGHT)
		set_inactive();

	GameObject* e = nullptr;
	if (get_ptr_to_student_world()->check_vortex_hit(getX(), getY(), e)) {
		e->hit_by_vortex();
		set_inactive();
		get_ptr_to_student_world()->playSound(SOUND_HIT_BY_VORTEX);
	}
}

//COIN SQUARE FUNCTIONS

CoinSquare::CoinSquare(StudentWorld* sw, int imageID, int startX, int startY, int adjust_coins_by)
	:ActivateOnPlayer(sw, imageID, startX, startY, 0, 1, 1.0, true)
{
	coins = adjust_coins_by;
}

void CoinSquare::doDiffStuff(Player* p)
{
	if (!p->is_walking() && p->is_newSpace()) {
		p->set_newSpace(false);
		p->adjust_coins(coins);
		if (coins > 0)
			get_ptr_to_student_world()->playSound(SOUND_GIVE_COIN);
		if (coins < 0)
			get_ptr_to_student_world()->playSound(SOUND_TAKE_COIN);
	}
	
}

//STAR SQUARE FUNCTIONS

StarSquare::StarSquare(StudentWorld* sw, int imageID, int startX, int startY)
	:ActivateOnPlayer(sw, imageID, startX, startY, 0, 1, 1.0, true)
{

}

void StarSquare::doDiffStuff(Player* p)
{
	if (!p->is_walking() && p->is_newSpace()) {
		p->set_newSpace(false);
	}

	if (p->get_coins() < 20)
		return;
	p->adjust_coins(-20);
	p->adjust_stars(1);
	get_ptr_to_student_world()->playSound(SOUND_GIVE_STAR);
}

//DIRECTIONAL SQUARE FUNCTIONS

DirectionalSquare::DirectionalSquare(StudentWorld* sw, int imageID, int startX, int startY, int dir)
	:ActivateOnPlayer(sw, imageID, startX, startY, dir, 1, 1.0, true)
{

}

void DirectionalSquare::doDiffStuff(Player* p)
{
	if (!p->is_walking() && p->is_newSpace()) {
		p->set_newSpace(false);
	}

	switch (getDirection()) {
	case right:
		p->force_walk_direction(right, right);
		break;
	case left:
		p->force_walk_direction(left, left);
		break;
	case up:
		p->force_walk_direction(right, up);
		break;
	case down:
		p->force_walk_direction(right, down);
		break;
	}

	p->set_onDir(true);
}

//BANK SQUARE FUNCTIONS

BankSquare::BankSquare(StudentWorld* sw, int imageID, int startX, int startY)
	:ActivateOnPlayer(sw, imageID, startX, startY, 0, 1, 1.0, true)
{

}

void BankSquare::doDiffStuff(Player* p)
{
	if (!p->is_walking() && p->is_newSpace()) {
		p->set_newSpace(false);
		p->adjust_coins(get_ptr_to_student_world()->get_bank_coins());
		get_ptr_to_student_world()->reset_bank_coins();
		get_ptr_to_student_world()->playSound(SOUND_WITHDRAW_BANK);
	}
	else if(p->is_walking()) {
		if (p->get_coins() >= 5) {
			get_ptr_to_student_world()->deposit_bank_coins(5);
		}
		else {
			get_ptr_to_student_world()->deposit_bank_coins(p->get_coins());
		}
		p->adjust_coins(-5);
		get_ptr_to_student_world()->playSound(SOUND_DEPOSIT_BANK);
	}


}

//EVENT SQUARE FUNCTIONS

EventSquare::EventSquare(StudentWorld* sw, int imageID, int startX, int startY)
	:ActivateOnPlayer(sw, imageID, startX, startY, 0, 1, 1.0, true)
{

}

void EventSquare::doDiffStuff(Player* p)
{
	if (!p->is_walking() && p->is_newSpace()) {
		p->set_newSpace(false);
		int randAct = randInt(1, 3);
		Player* other = get_ptr_to_student_world()->get_other_player(p);
		switch (randAct) {
		case 1:
			p->teleport_me_to_random_sq();
			get_ptr_to_student_world()->playSound(SOUND_PLAYER_TELEPORT);
			break;
		case 2:
			p->swap_positions(other);
			get_ptr_to_student_world()->playSound(SOUND_PLAYER_TELEPORT);
			break;
		case 3:
			if (!p->has_vortex()) {
				p->equip_with_vortex_projectile();
				get_ptr_to_student_world()->playSound(SOUND_GIVE_VORTEX);
			}
			break;
		}
	}
}

//DROPPING SQUARE FUNCTIONS

DroppingSquare::DroppingSquare(StudentWorld* sw, int imageID, int startX, int startY)
	:ActivateOnPlayer(sw, imageID, startX, startY, 0, 1, 1.0, true)
{

}

void DroppingSquare::doDiffStuff(Player* p)
{
	if (!p->is_walking() && p->is_newSpace()) {
		p->set_newSpace(false);
		int randDrop = randInt(1, 2);
		switch (randDrop) {
		case 1:
			p->adjust_coins(-10);
			break;
		case 2:
			p->adjust_stars(-1);
			break;
		}
		get_ptr_to_student_world()->playSound(SOUND_DROPPING_SQUARE_ACTIVATE);
	}
}

//ENEMY FUNCTIONS

Enemy::Enemy(StudentWorld* sw, int imageID, int startX, int startY,
	int dir, int depth, double size, bool activate_when_go_lands, int num_sq_to_move, int number_of_ticks_to_pause)
	:ActivateOnPlayer(sw, imageID, startX, startY, dir, depth, size, activate_when_go_lands)
{
	travDist = num_sq_to_move;
	pauseTicks = number_of_ticks_to_pause;
	ticks_to_move = travDist * 8;
	walkDir = right;
}

void Enemy::do_something()
{
	if(walk_state == false){
		ActivateOnPlayer::do_something();

		pauseTicks--;

		if (pauseTicks == 0) {
			travDist = randInt(1, 3);
			ticks_to_move = travDist * 8;

			do {
				int newDir = randInt(1, 4);

				switch (newDir) {
				case 1:
					force_walk_direction(right, right);
					break;
				case 2:
					force_walk_direction(left, left);
					break;
				case 3:
					force_walk_direction(right, up);
					break;
				case 4:
					force_walk_direction(right, down);
					break;
				}
			} while (!validDirection(walkDir));

			walk_state = true;
			active_enemy = true;
		}
	}

	if (walk_state == true) {

		int totalPaths = 0;

		if (validDirection(up))
			totalPaths++;
		if (validDirection(down))
			totalPaths++;
		if (validDirection(right))
			totalPaths++;
		if (validDirection(left))
			totalPaths++;

		while (totalPaths >= 3) {
			int pathChoice = randInt(1, 4);
			switch (pathChoice) {
			case 1:
				if (validDirection(left)) {
					force_walk_direction(left, left);
					totalPaths = 0;
				}
				break;

			case 2:
				if (validDirection(right)) {
					force_walk_direction(right, right);
					totalPaths = 0;
				}
				break;
				

			case 3:
				if (validDirection(up)) {
					force_walk_direction(right, up);
					totalPaths = 0;
					
				}
				break;
				

			case 4:
				if (validDirection(down)) {
					force_walk_direction(right, down);
					totalPaths = 0;
					
				}
				break;
				
			}
		}

		//directional system
		if (onSquare() && !validDirection(walkDir)) {
			if (walkDir == up || walkDir == down) {
				if (validDirection(right)) {
					force_walk_direction(right, right);
				}
				else {
					force_walk_direction(left, left);
				}

			}
			else if (walkDir == left || walkDir == right) {
				if (validDirection(up))
				{
					force_walk_direction(right, up);
				}
				else
				{
					force_walk_direction(right, down);
				}
			}
		}
	
		moveAtAngle(walkDir, 2);
		ticks_to_move--;
		if (ticks_to_move == 0) {
			walk_state = false;
			pauseTicks = 180;
		}
	}
}

void Enemy::hit_by_vortex()
{
	GameObject* destSquare = get_ptr_to_student_world()->get_random_square(getX(), getY());
	moveTo(destSquare->getX(), destSquare->getY());

	walkDir = right;
	setDirection(0);
	walk_state = false;
	pauseTicks = 180;
}

//BOWSER FUNCTIONS

Bowser::Bowser(StudentWorld* sw, int imageID, int startX, int startY)
	:Enemy(sw, imageID, startX, startY, 0, 0, 1.0, true, 0, 180)
{

}

void Bowser::do_something()
{
	if (!walk()) {
		ActivateOnPlayer::do_something();

		changePause(-1);

		if (getPause() == 0) {
			setTrav(randInt(1, 10));
			setTicks(getTrav() * 8);

			do {
				int newDir = randInt(1, 4);

				switch (newDir) {
				case 1:
					force_walk_direction(right, right);
					break;
				case 2:
					force_walk_direction(left, left);
					break;
				case 3:
					force_walk_direction(right, up);
					break;
				case 4:
					force_walk_direction(right, down);
					break;
				}
			} while (!validDirection(getWalk()));

			setWalkState(true);
			setEnemy(true);
		}
	}

	if (walk()) {
		int totalPaths = 0;

		if (validDirection(up))
			totalPaths++;
		if (validDirection(down))
			totalPaths++;
		if (validDirection(right))
			totalPaths++;
		if (validDirection(left))
			totalPaths++;

		while (totalPaths >= 3) {
			int pathChoice = randInt(1, 4);
			switch (pathChoice) {
			case 1:
				if (validDirection(left)) {
					force_walk_direction(left, left);
					totalPaths = 0;
				}
				break;

			case 2:
				if (validDirection(right)) {
					force_walk_direction(right, right);
					totalPaths = 0;
				}
				break;


			case 3:
				if (validDirection(up)) {
					force_walk_direction(right, up);
					totalPaths = 0;

				}
				break;


			case 4:
				if (validDirection(down)) {
					force_walk_direction(right, down);
					totalPaths = 0;

				}
				break;

			}
		}

		//directional system
		if (onSquare() && !validDirection(getWalk())) {
			if (getWalk() == up || getWalk() == down) {
				if (validDirection(right)) {
					force_walk_direction(right, right);
				}
				else {
					force_walk_direction(left, left);
				}

			}
			else if (getWalk() == left || getWalk() == right) {
				if (validDirection(up))
				{
					force_walk_direction(right, up);
				}
				else
				{
					force_walk_direction(right, down);
				}
			}
		}

		moveAtAngle(getWalk(), 2);
		changeTicks(-1);
		if (getTicks() == 0) {
			setWalkState(false);
			setPause(180);

			int Drop = randInt(1, 4);
			if (Drop == 4) {
				get_ptr_to_student_world()->replace_square(getX(), getY());
				get_ptr_to_student_world()->playSound(SOUND_DROPPING_SQUARE_CREATED);
			}
		}
	}
}

void Bowser::doDiffStuff(Player* p)
{
	if (!p->is_walking() && (p->enemySpace() || actEnemy())) {
		p->set_EnemySpace(false);
		setEnemy(false);
		int BowserRand = randInt(1, 2);

		if (BowserRand == 2) {
			p->reset_coins();
			p->reset_stars();
			get_ptr_to_student_world()->playSound(SOUND_BOWSER_ACTIVATE);
		}
	}
}

//BOO FUNCTIONS

Boo::Boo(StudentWorld* sw, int imageID, int startX, int startY)
	:Enemy(sw, imageID, startX, startY, 0, 0, 1.0, true, 0, 180)
{

}

void Boo::doDiffStuff(Player* p)
{
	if (!p->is_walking() && (p->enemySpace() || actEnemy())) {
		p->set_EnemySpace(false);
		setEnemy(false);
		int BooRand = randInt(1, 2);
		Player* other = get_ptr_to_student_world()->get_other_player(p);

		switch (BooRand) {
		case 1:
			p->swap_coins(other);
			break;
		case 2:
			p->swap_stars(other);
			break;
		}
		get_ptr_to_student_world()->playSound(SOUND_BOO_ACTIVATE);
	}
}

