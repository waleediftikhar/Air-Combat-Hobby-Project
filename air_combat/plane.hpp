#ifndef PLANE_HPP_INCLUDED
#define PLANE_HPP_INCLUDED
#include <iostream>
#include <map>
#include "Box2D/Box2D.h"
#include <list>
class Weapon
{
public:
	std::string& get_type();
private:
	std::string type_ = "gun";
};

class Plane
{
public:
	Plane() {}
	Plane(std::string type, int hp, float speed,
		float max_speed, float acceleration,
		std::map<std::string, Weapon> weapons, b2Body *body) :
		type_(type), hp_(hp), speed_(speed),
		max_speed_(max_speed), acceleration_(acceleration),
		weapons_(weapons), body_(body) {}
	std::string& get_type();
	b2Body *get_body();
	void move_plane(float x, float y);
	bool shoot(b2World *world);
	std::list<b2Body*> &get_bullets();
	std::list<int> &get_counters();
	int change_hp(int hit);
	int get_hp();
	//void deletebullet(b2Body* bullet);
private:
	std::string type_;
	int hp_; // hitpoints
	float speed_;
	float max_speed_;
	float acceleration_;
	std::map<std::string, Weapon> weapons_;
	b2Body *body_;
	std::list<b2Body*> bullets_;
	std::list<int> bulletcounters_;


};






#endif //PLANE_HPP INCLUDED
