#include "Simulate.h"
#include <cmath>
#include <cstdio>
#include <cstdlib>

double pi = 3.14159265;
float ball_radius = 5.75;

USING_NS_CC;

Scene* Simulator::createScene(){
	return Simulator::create();
}

void Simulator::loadan(){
	int seed,i,inf_item;
	FILE* f = fopen("rand_save", "r");
	if (f == nullptr) {
		seed = 0;
	}
	else {
		fscanf(f, "%d", &seed);
	}
	srand(seed);

	FILE* fw = fopen("rand_save", "w");
	fprintf(fw, "%d", seed + 1);
	fclose(fw);

	FILE* cnf = fopen("simulator_options.ini", "r");
	if (cnf==nullptr){
		FILE* cnf_w = fopen("simulator_options.ini", "w");
		fprintf(cnf_w, "%d %d %d %d\nTo change the simulator options, the numbers above are:\n- Item count\n- Infection radius\n- Probability of infection\n- Maximum period of infection", Simulator::count, Simulator::inf_rad, Simulator::inf_prob, Simulator::max_inf_period);
		fclose(cnf_w);
	}
	else {
		fscanf(cnf, "%d %d %d %d", &count, &inf_rad, &inf_prob, &max_inf_period);
	}
	if (count > 1000 || count <= 0) count = 500;

	auto visibleSize = Director::getInstance()->getVisibleSize();

	for (i = 0; i < count; i++) {
		items[i].sprite = Sprite::create("ball-b.png");
		items[i].sprite->setPosition(Vec2(rand() % int(visibleSize.width - (ball_radius * 2)) + ball_radius, rand() % int(visibleSize.height - (ball_radius * 2)) + ball_radius));
		items[i].sprite->setScale(0.25);
		this->addChild(items[i].sprite);

		items[i].direction = rand();
		items[i].state = 0;
	}
	inf_item = rand() % count;
	items[inf_item].state = 1;
	items[inf_item].inf_period = rand() % max_inf_period + 1;
}

bool Simulator::init(){
	if ( !Scene::init() )
	{
		return false;
	}

	this->loadan();

	this->scheduleUpdate();

	return true;
}

void Simulator::menuCloseCallback(Ref* pSender)
{
	//Close the cocos2d-x game scene and quit the application
	Director::getInstance()->end();

	/*To navigate back to native iOS screen(if present) without quitting the application, do not use Director::getInstance()->end() as given above,instead trigger a custom event created in RootViewController.mm as below*/

	//EventCustom customEndEvent("game_scene_close_event");
	//_eventDispatcher->dispatchEvent(&customEndEvent);
}

void Simulator::check_infection() {
	int i, j;
	Vec2 posi, posj;
	for (i = 0; i < count; i++) {
		if (items[i].state == 1) {
			for (j = 0; j < count; j++) {
				posi = items[i].sprite->getPosition();
				posj = items[j].sprite->getPosition();
				if ((posj.x - posi.x) * (posj.x - posi.x) + (posj.y - posi.y) * (posj.y - posi.y) <= inf_rad * inf_rad && rand() % 100 < inf_prob && items[j].state == 0) {
					items[j].state = 1;
					items[j].inf_period = rand() % max_inf_period + 1;
				}
			}
		}
	}
}

void Simulator::update(float del) {
	auto vsz = Director::getInstance()->getVisibleSize();
	Vec2 pos;

	int i;

	Simulator::check_infection();

	for (i = 0; i < count; i++) {
		pos = items[i].sprite->getPosition();
		items[i].movetime--;

		if (pos.x <= ball_radius || pos.x >= vsz.width - ball_radius || pos.y >= vsz.height - ball_radius || pos.y <= ball_radius) {
			if (pos.x <= ball_radius) {
				items[i].direction = (rand() % 180);
				pos.x = ball_radius + 1;
			}
			else if (pos.x >= vsz.width - ball_radius) {
				items[i].direction = rand() % 180 + 180;
				pos.x = vsz.width - ball_radius - 1;
			}

			if (pos.y >= vsz.height - ball_radius) {
				items[i].direction = rand() % 180 + 90;
				pos.y = vsz.height - ball_radius - 1;
			}
			else if (pos.y <= ball_radius) {
				items[i].direction = rand() % 180 + 270;
				pos.y = ball_radius + 1;
			}
		}
		else {
			if (items[i].movetime <= 0) {
				items[i].movetime = (rand() % 100 + 1);
				items[i].direction = rand();
			}
			pos.x += 150 * sin(items[i].direction * pi / 180) * del;
			pos.y += 150 * cos(items[i].direction * pi / 180) * del;
		}

		items[i].sprite->setPosition(pos);

		if (items[i].state == 1) items[i].inf_period--;
		if (items[i].state == 1 && items[i].inf_period == 0) items[i].state = 2;

		if (items[i].state == 1) items[i].sprite->setTexture("ball-c.png");
		if (items[i].state == 2) items[i].sprite->setTexture("ball-d.png");
	}
}