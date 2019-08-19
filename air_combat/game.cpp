#include <SFML/Window.hpp>
#include <SFML/Graphics.hpp>

#include "exceptions.hpp"
#include <iostream>
#include <vector>
#include "game.hpp"
#include <stdio.h>
#include <map>
int Game::get_planecount() {
	return planecount_;
}

std::vector<Plane>& Game::get_player()
{
	return players_;
}


b2Body* Game::get_ground()
{
	return ground_;
}

void Game::init()
{
	planecount_ = 0;
	std::vector<int> x = { 5, 120 };
	std::vector<int> y = { 5, 20 };
	std::cout << x_ << "   " << y_ << std::endl;
	//int maplen = gamemap_[0].size();
	int player_count = 2;						// IMPLEMENT PLAYER NUMBER CHOOSING IN MAIN MENU
	const char* name = "maps/map1_physics.bmp"; 	// implement map choosing later
	gamemap_ = loadmap(name);
	objectify_map();
	std::map<std::string, Weapon> weapons;
	//Create body for plane

	for (int i = 0; i < player_count; i++) {
		b2BodyDef bodydef;
		bodydef.type = b2_dynamicBody;
		bodydef.position.Set(x[i], y[i]);
		bodydef.angle = 0;

		b2PolygonShape box;
		box.SetAsBox(0.7, 0.5);

		b2FixtureDef bodyfixture;
		bodyfixture.shape = &box;
		bodyfixture.density = 1;

		b2Body *body = world_->CreateBody(&bodydef);
		body->CreateFixture(&bodyfixture);
		body->SetLinearDamping(0.1);
		if (planecount_ == 0)
			body->ApplyLinearImpulse(b2Vec2(30, -1), body->GetWorldCenter(), 1);
		else
			body->ApplyLinearImpulse(b2Vec2(-30, -1), body->GetWorldCenter(), 1);
		planecount_ += 1;
		players_.push_back(Plane("Biplane", 100, 5, 40, 10, weapons, body));
	}
}


void Game::objectify_map()			// create static terrain segments from bmp image
{
	b2BodyDef groundDef;
	groundDef.allowSleep = true;
	groundDef.type = b2_staticBody;
	unsigned int a = 0;
	unsigned int b = 0;
	bool set = false;
	while (a < gamemap_.size() - 1) { 	// check ground level to set correct height
		if (set == true) {
			break;
		}
		b = 0;
		while (b < gamemap_[0].size() - 1) {
			if (gamemap_[b][a] == OTHER) {
				set = true;
				break;
			}
			b++;
		}
		a++;
	}
	groundDef.position.Set(0, 0);
	b2Body *ground = world_->CreateBody(&groundDef);
	ground_ = ground;
	unsigned int x = 0;
	while (x < gamemap_[0].size() - 1) {
		unsigned int y = gamemap_.size() - 1;
		unsigned int ystart = 0;
		while (y >= 0) {
			unsigned int xend = 0;
			if (gamemap_[y][x] == WHITE) {
				//std::cout << "Creating at: x(" << x << "), y(" << y << ")" << std::endl;
				ystart = y;
				unsigned int yend = gamemap_.size() - 1;
				if (x + 1 >= gamemap_[0].size()) {
					std::cout << "Load finished" << std::endl;
					return;
				}
				else {
					xend = x + 1;
				}
				while (gamemap_[yend][xend] != WHITE) {
					if (yend - 1 < 0) {
						break;
					}
					yend--;
				}
				/*std::cout << "Vertices" << std::endl << "0: x(" << x*10 << "), y(" << ystart*10 << ")" << std::endl
					<< "1: x(" << x*10 << "), y(" << 720 << ")" << std::endl
					<< "2: x(" << xend*10 << "), y(" << 720 << ")" << std::endl
					<< "3: x(" << xend*10 << "), y(" << yend*10 << ")" << std::endl;*/
				b2Vec2 vertices[4];
				vertices[0].Set((x), (ystart));
				vertices[1].Set((x), 72.0);
				vertices[2].Set((xend), 72.0);
				vertices[3].Set((xend), (yend));
				int32 vertexcount = 4;
				b2PolygonShape polygon;
				polygon.Set(vertices, vertexcount);
				b2FixtureDef groundfixture;
				groundfixture.shape = &polygon;
				ground->CreateFixture(&groundfixture);
				x = xend;
				y = gamemap_.size() - 1;

			}
			else {
				y--;
			}
		}
		x++;
	}
	std::cout << "Load finished" << std::endl;
}


bool Game::isrunning()
{
	return on_;
}

void Game::update_physics()
{
	world_->Step(1 / 60.f, 8, 3);
}

std::vector<std::vector<Pixel>> Game::loadmap(const char* filename)					// loads 24-bit BMP image file into game as map
{
	FILE* img = fopen(filename, "rb");
	if (!img) {
		throw FileException("Error: Failed to read file");
	}

	unsigned char* header = new unsigned char[54];
	fread(header, 1, 54, img);

	unsigned char filetype1 = header[0]; unsigned char filetype2 = header[1];		// checking file type, should be BM (ASCII 66 and 77)
	if (filetype1 != 66 && filetype2 != 77) {
		delete[] header;
		throw FileException("Invalid file type: not BMP");
	}

	unsigned long width = *(unsigned long*)&header[18];								// typecast to unsigned long-pointer and dereference to read 4 bytes at once
	unsigned long height = *(unsigned long*)&header[22];
	unsigned long padding = width % 4;												// The size of BMP rows are rounded up to a multiple of 4 by padding

	//std::cout << "Width: " << width << " --- " << "Height: " << height << " --- " << "Padding: " << padding << std::endl;

	short int bitsperpixel = *(short int*)&header[28];
	if (bitsperpixel != 24) {
		delete[] header;
		throw FileException("Invalid file type: not 24-bit BMP");
	}

	std::vector<std::vector<Pixel>> gamemap;										// Initialize two-dimensional vector

	for (unsigned long y = 0; y < height; y++) {
		std::vector <Pixel> maprow;
		for (unsigned long x = 0; x < width; x++) {
			maprow.push_back(WHITE);
		}
		gamemap.push_back(maprow);
	}

	unsigned char b = fgetc(img);													// Windows stores pixel colors in the order Blue, Green, Red
	unsigned char g = fgetc(img);
	unsigned char r = fgetc(img);

	unsigned long pixels_read = 1;
	unsigned int row = 0;


	int size = 3 * width * height;
	for (int i = 0; i < size; i += 3) {                     						// accessing pixel color values
		if (b == 0 && g == 0 && r == 255) {
			gamemap[(height - 1) - row][pixels_read - 1] = RED;
			x_ = height - 1 - row;
			y_ = pixels_read - 1;
		}
		else if (b == 255 and g == 255 and r == 255) {
			gamemap[(height - 1) - row][pixels_read - 1] = WHITE;
		}
		else {
			gamemap[(height - 1) - row][pixels_read - 1] = OTHER;
		}
		if (pixels_read == width) {
			row++;
			pixels_read = 0;
			for (int j = padding; j > 0; j--) {
				fgetc(img);
			}
			std::cout << std::endl;
		}
		b = fgetc(img);												// Windows stores pixel colors in the order Blue, Green, Red
		g = fgetc(img);
		r = fgetc(img);
		pixels_read++;

	}
	fclose(img);
	delete[] header;
	return gamemap;
}

b2World *Game::get_world() {
	return world_;
}
