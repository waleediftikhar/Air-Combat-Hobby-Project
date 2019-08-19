#ifndef APP_HPP_INCLUDED
#define APP_HPP_INCLUDED

#include <SFML/Graphics.hpp>
#include <SFML/Window.hpp>
#include <SFML/Audio.hpp>
#include <iostream>
#include "game.hpp"
#include "app.hpp"
#include <math.h>
#include "menu.hpp"

#include "exceptions.hpp"


class App
{
	public:
	    App (sf::RenderWindow &win, Game &game) : main_window(win), game_(game) {}
        int main_menu();
        void app_main();
        void game_loop(unsigned , unsigned);
		unsigned selectPlane();
		unsigned selectMap();
		unsigned multiplayers();
		void render(sf::Sprite &player, int c);
		void Background(std::string);

		void count_bullets(int c);
		void position_handler(int c);
		void handle_input(sf::Event event, bool& right1, bool& left1, bool& up1, bool& down1,
			bool& shoot1, bool& right2, bool& left2, bool& up2, bool& down2, bool& shoot2, bool& closed);
		void moveaction(bool up, bool down, bool left, bool right, bool& shoot, int c);
		bool check_collisions();
		void show_instructions();
		void print_hp();
		void print_bar(int hp, int x, int y);


	private:
	    sf::RenderWindow &main_window;
	    Game &game_;
		std::vector<std::string> background;
		std::vector<std::string> maps;
		std::vector<std::string> planes;
		sf::Sound gunsound_;
		sf::Sound crashsound_;
};


#endif // APP_HPP_INCLUDED
