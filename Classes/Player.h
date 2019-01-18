#ifndef PLAYER_H
#define PLAYER_H

#include <ctime>

class Player
{
private:
	std::time_t mTimeBetweenGameLaunchings;

	Player();
public:
	static Player* getInstance();
	~Player();

	const std::time_t& getTimeBetweenGameLaunchings();
};

#endif //PLAYER_H
