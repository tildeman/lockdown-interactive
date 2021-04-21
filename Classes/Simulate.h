#ifndef _nhatminh_sim
#define _nhatminh_sim

#include "cocos2d.h"
#include <cstdio>

class Simulator: public cocos2d::Scene
{
public:

	struct ball {
		cocos2d::Sprite* sprite;
		int state;
		int direction;
		int movetime;
		int inf_period;
	};

	int count = 500;
	int inf_rad = 5;
	int inf_prob = 10;
	int max_inf_period = 500;

	static cocos2d::Scene* createScene();

	void loadan();

	virtual bool init();

	void update(float del);

	void check_infection();
	// O(n^2) complexity. May lag at certain devices

	// a selector callback
	void menuCloseCallback(cocos2d::Ref* pSender);

	// implement the "static create()" method manually
	CREATE_FUNC(Simulator);
private:
	struct ball items[1000];
};

#endif