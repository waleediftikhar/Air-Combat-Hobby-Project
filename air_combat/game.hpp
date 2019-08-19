#ifndef GAME_HPP_INCLUDED
#define GAME_HPP_INCLUDED

#include <string>
#include "Box2D/Box2D.h"
#include "plane.hpp"

enum Pixel { WHITE, RED, OTHER };
/* SUBJECT TO CHANGE white = empty space (flyable), red = spawn point for player.
Every other color is loaded into the game as OTHER pixel that causes collision (but entire image still loaded into graphics) */


class Game
{
public:
	Game() {}
	std::vector<std::vector<Pixel>> loadmap(const char* filename);
	void init();
	bool isrunning();
	void update_physics();
	std::vector<Plane>& get_player();
	b2World *get_world();
	void objectify_map();
	b2Body *get_ground();
	int get_planecount();

private:

	std::vector<std::vector<Pixel>> gamemap_;
	std::vector<Plane> players_;
	int planecount_;
	Plane player_;
	bool on_;			// is game running?
	b2World *world_ = new b2World(b2Vec2(0.f, 10.0f));
	int x_;
	int y_;			// Player spawn point in pixels
	b2Body *ground_;

};

#endif // GAME_HPP_INCLUDED
