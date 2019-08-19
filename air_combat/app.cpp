

#include <SFML/Graphics.hpp>
#include <SFML/Window.hpp>
#include <SFML/Audio.hpp>
#include <iostream>
#include "game.hpp"
#include "app.hpp"
#include <math.h>
#include "menu.hpp"

#include "exceptions.hpp"
//???

bool App::check_collisions()
{
	for (unsigned int i = 0; i < game_.get_player().size(); i++) {
		for (b2ContactEdge *edge = game_.get_player()[i].get_body()->GetContactList(); edge; edge = edge->next) {
			if (edge->other->GetType() == b2BodyType::b2_dynamicBody) {
				if (edge->other->IsBullet()) {
					game_.get_player()[i].change_hp(-20);
					int z = 1;
					if (i == 1)
						z = 0;
					edge->other->SetType(b2_kinematicBody);
					crashsound_.play();
					std::cout << "bullet git " << std::endl;
				}
				else {
					game_.get_player()[0].change_hp(-100);
					game_.get_player()[1].change_hp(-100);
					crashsound_.play();
				}
			}
			else if (edge->other->GetType() == b2BodyType::b2_staticBody) {
				game_.get_player()[i].change_hp(-100);
				crashsound_.play();
			}
		}
		if (game_.get_player()[i].change_hp(0) <= 0)
			return true;
	}
	return false;
}

void App::render(sf::Sprite& player, int c)
{
	b2Vec2 coordinates = game_.get_player()[c].get_body()->GetPosition();
	float x = coordinates(0) * 10;
	float y = coordinates(1) * 10;
	player.setPosition(x, y);
	player.setRotation(game_.get_player()[c].get_body()->GetAngle() * 180 / 3.14);
	if (game_.get_player()[c].get_body()->GetLinearVelocity()(0) < 0) {
		player.setScale(-0.3, 0.3);
	}
	else {
		player.setScale(0.3, 0.3);
	}
	player.setOrigin(74, 31);
	main_window.draw(player);

	for (b2Body* body = game_.get_world()->GetBodyList(); body; body = body->GetNext()) {
		if (body->IsBullet()) {
			b2Vec2 coordinates = body->GetPosition();
			sf::CircleShape shape(3);
			shape.setPosition(coordinates(0) * 10, coordinates(1) * 10);
			shape.setFillColor(sf::Color::Black);
			main_window.draw(shape);

		}
	}
}


void App::count_bullets(int c)
{
	bool delete_first = false;
	for (auto i = game_.get_player()[c].get_counters().begin();
		i != game_.get_player()[c].get_counters().end(); i++) {
		*i += 1;
		if (*i > 120) {
			delete_first = true;
		}
	}
	if (delete_first == true) {
		game_.get_player()[c].get_counters().pop_front();
		b2Body* temp = game_.get_player()[c].get_bullets().front();
		game_.get_world()->DestroyBody(temp);
		game_.get_player()[c].get_bullets().pop_front();
	}
}


void App::handle_input(sf::Event event, bool& right1, bool& left1, bool& up1, bool& down1, bool& shoot1, bool& right2, bool& left2, bool& up2, bool& down2, bool& shoot2, bool& closed)
{
	//player1 controls
	if (event.type == sf::Event::Closed) {
		closed = true;
	}
	else if (sf::Keyboard::isKeyPressed(sf::Keyboard::Escape)) {
		closed = true;
	}

	if (sf::Keyboard::isKeyPressed(sf::Keyboard::Up)) {
		up1 = true;
	}
	else {
		up1 = false;
	}
	if (sf::Keyboard::isKeyPressed(sf::Keyboard::Right)) {
		right1 = true;
	}
	else {
		right1 = false;
	}
	if (sf::Keyboard::isKeyPressed(sf::Keyboard::Left)) {
		left1 = true;
	}
	else {
		left1 = false;
	}
	if (sf::Keyboard::isKeyPressed(sf::Keyboard::Down)) {
		down1 = true;
	}
	else {
		down1 = false;
	}
	if (sf::Keyboard::isKeyPressed(sf::Keyboard::M)) {
		shoot1 = true;
	}



	//player2 controls
	if (sf::Keyboard::isKeyPressed(sf::Keyboard::W)) {
		up2 = true;
	}
	else {
		up2 = false;
	}
	if (sf::Keyboard::isKeyPressed(sf::Keyboard::D)) {
		right2 = true;
	}
	else {
		right2 = false;
	}
	if (sf::Keyboard::isKeyPressed(sf::Keyboard::A)) {
		left2 = true;
	}
	else {
		left2 = false;
	}
	if (sf::Keyboard::isKeyPressed(sf::Keyboard::S)) {
		down2 = true;
	}
	else {
		down2 = false;
	}
	if (sf::Keyboard::isKeyPressed(sf::Keyboard::Tab)) {
		shoot2 = true;
	}
}


void App::position_handler(int c)
{
	b2Vec2 pos = game_.get_player()[c].get_body()->GetPosition();
	b2Vec2 vel = game_.get_player()[c].get_body()->GetLinearVelocity();
	if (pos(0) * 10 > 1280) {
		game_.get_player()[c].get_body()->SetTransform(b2Vec2(0, pos(1)), game_.get_player()[c].get_body()->GetAngle());
	}
	else if (pos(0) * 10 < 0) {
		game_.get_player()[c].get_body()->SetTransform(b2Vec2(1280 / 10, pos(1)), game_.get_player()[c].get_body()->GetAngle());
	}
	pos = game_.get_player()[c].get_body()->GetPosition();
	if (vel(0) != 0) {
		game_.get_player()[c].get_body()->SetTransform(b2Vec2(pos(0), pos(1)), std::atan(vel(1) / vel(0)));
	}
	//handle the angle of the plane to match direction of the plane
	else {
		if (vel(1) > 0) {
			game_.get_player()[c].get_body()->SetTransform(b2Vec2(pos(0), pos(1)), 3 * 3.1415 / 2);
		}
		else if (vel(1) < 0) {
			game_.get_player()[c].get_body()->SetTransform(b2Vec2(pos(0), pos(1)), 3.1415 / 2);
		}
	}
}



void App::moveaction(bool up, bool down, bool left, bool right, bool& shoot, int c)
{
	//Move the plane depending on the flags.
	//Arguments given from handle_input()
	if (right)
		game_.get_player()[c].move_plane(1, -1);
	if (left)
		game_.get_player()[c].move_plane(-1, -1);
	if (up)
		game_.get_player()[c].move_plane(0, -1);
	else if (down)
		game_.get_player()[c].move_plane(0, 2);
	if (shoot) {
		if (game_.get_player()[c].shoot(game_.get_world()))
			gunsound_.play();

		shoot = false;
	}
}


void App::print_bar(int hp, int x, int y) {
	sf::RectangleShape bar;
	bar.setFillColor(sf::Color::Red);
	bar.setSize(sf::Vector2f(hp, 30));
	bar.setPosition(x, y);
	main_window.draw(bar);
}

void App::print_hp() {
	sf::Text text;
	sf::Font font;
	font.loadFromFile("DroidSans.ttf");
	text.setFont(font);
	text.setFillColor(sf::Color::White);
	text.setString("Player2 hp:");
	text.setPosition(1080, 650);
	print_bar(game_.get_player()[1].get_hp(), 1080, 685);
	main_window.draw(text);
	text.setString("Player1 hp:");
	text.setPosition(50, 650);
	print_bar(game_.get_player()[0].get_hp(), 50, 685);
	main_window.draw(text);


}

void App::game_loop(unsigned planeIndex , unsigned mapIndex)
{
	game_.init();
	bool running = true;
	sf::Texture player1;
	player1.loadFromFile(planes[planeIndex]);
	sf::Sprite player1spr;
	player1spr.setTexture(player1);
	sf::Texture map;
	map.loadFromFile(maps[mapIndex]);
	sf::Sprite background;
	background.setScale(1, 1);
	background.setTexture(map);
	//Background(planes[planeIndex]);
	//Background(maps[mapIndex]);

	bool right1 = false;
	bool left1 = false;
	bool up1 = false;
	bool down1 = false;
	bool shoot1 = false;

	bool right2 = false;
	bool left2 = false;
	bool up2 = false;
	bool down2 = false;
	bool shoot2 = false;

	bool closed = false;

	while (running == true && main_window.isOpen()) {
		sf::Event event;
		while (main_window.pollEvent(event)) {
			handle_input(event, right1, left1, up1, down1, shoot1, right2, left2, up2, down2, shoot2, closed);
		}
		if (closed == true) {
			break;
		}
		for (int c = 0; c < game_.get_planecount(); c++) {
			count_bullets(c);
			if (c == 0) {
				moveaction(up1, down1, left1, right1, shoot1, c);
			}
			else {
				moveaction(up2, down2, left2, right2, shoot2, c);
			}
			position_handler(c);
		}
		main_window.clear(sf::Color::Black);
		main_window.draw(background);
		game_.update_physics();
		for (int c = 0; c < game_.get_planecount(); c++) {
			render(player1spr, c);
		}
		print_hp();
		main_window.display();
		if (check_collisions()) {
			sf::Text end_text;
			sf::Font font;
			font.loadFromFile("DroidSans.ttf");
			end_text.setFont(font);
			if (game_.get_player()[0].change_hp(0) <= 0) {
				if (game_.get_player()[1].change_hp(0) <= 0)
					end_text.setString("Both lose!\nPress Q to go back to menu");
				else
					end_text.setString("Player2 wins!\nPress Q to go back to menu");
			}
			else
				end_text.setString("Player1 wins!\nPress Q to go back to menu");
			end_text.setPosition(sf::Vector2f(400, 300));
			end_text.setFillColor(sf::Color::Red);
			main_window.draw(end_text);
			main_window.display();
			while (!sf::Keyboard::isKeyPressed(sf::Keyboard::Q)) {
				while (main_window.pollEvent(event)) {
					if (event.type == sf::Event::Closed) {
							return;
					}
				}
			}
			delete game_.get_world();
			planeIndex = 0;
			mapIndex = 0;
			main_window.clear();
			return;
		}

	}
}



void App::Background(std::string str) {
	sf::Sprite sprite;
	sf::Texture mainTexture;
	if (!mainTexture.loadFromFile(str))
		std::cout << "Error Loading Background" << std::endl;
	sprite.setTexture(mainTexture);
	main_window.draw(sprite);
}

int App::main_menu(){
	std::vector<std::string> vecStr;
	std::vector<sf::Vector2f> origin;
	sf::Vector2f temp_origin;
	vecStr.push_back("Play");
	vecStr.push_back("Multiplayers");
	vecStr.push_back("My Planes");
	vecStr.push_back("Missions");
	vecStr.push_back("Exit");

	for (unsigned i = 0; i < vecStr.size(); i++) {
		temp_origin.x = main_window.getSize().x / 2;
		temp_origin.y = (main_window.getSize().y / (vecStr.size() + 1) * 3) + i*60;
		origin.push_back(temp_origin);
	}


	main_window.clear();
	Menu menu(main_window , vecStr , origin);
	main_window.display();
    while (main_window.isOpen()){
        main_window.clear();
		Background(background[0]);
		menu.draw(main_window);
        sf::Event event;
       
		while (main_window.pollEvent(event))
		{
			switch (event.type)
			{
			case sf::Event::KeyReleased:
				switch (event.key.code)
				{
				case sf::Keyboard::Up:
					menu.MoveUp();
					break;

				case sf::Keyboard::Down:
					menu.MoveDown();
					break;

				case sf::Keyboard::Return:
					switch (menu.GetPressedItem())
					{
					case 0:
						std::cout << "Play button has been pressed" << std::endl;
						return 1;
						break;

					case 1:
						std::cout << "Multiplayers button has been pressed" << std::endl;
						return 2;
						break;

					case 2:
						std::cout << "Select Plane button has been pressed" << std::endl;
						return 3;
						break;
					case 3:
						std::cout << "Select Map button has been pressed" << std::endl;
						return 4;
						break;
					case 4:
						std::cout << "Close button has been pressed" << std::endl;
						return 0;
						//main_window.close();
						break;
					}

					break;
				}

				break;
			case sf::Event::Closed:
				main_window.close();
				break;
			}
			
			
			if (sf::Mouse::isButtonPressed(sf::Mouse::Left)) {
				int valid = -1;
				valid = menu.MouseEvent(sf::Mouse::getPosition(main_window));
				if (valid >= 0 && valid < menu.get_Number_Of_Items())
					return valid;
				else
					break;
			}
		}
		menu.MouseEvent(sf::Mouse::getPosition(main_window));
		main_window.display();
    }
    return 0;
}


void App::show_instructions() {
	main_window.clear();
	sf::Sprite sprite;
	sf::Texture instrtext;
	if (!instrtext.loadFromFile("images/instructions.jpg")) {
		std::cout << "Error loading instructions screen" << std::endl;
	}
	sprite.setTexture(instrtext);
	main_window.draw(sprite);
	main_window.display();
	while (main_window.isOpen()) {
		sf::Event event;
		while (main_window.pollEvent(event)) {
			if (event.type == sf::Event::Closed)
				return;
			if (sf::Keyboard::isKeyPressed(sf::Keyboard::Space))
				return;

		}

	}
}

void App::app_main(){
	background.push_back("images/background1.jpg");
	background.push_back("images/background2.jpg");
	
	planes.push_back("images/jetred_r.png");
	planes.push_back("images/jetgreen_r.png");
	planes.push_back("images/jetgray_r.png");
	

	maps.push_back("maps/High_Altitude.jpg");
	maps.push_back("maps/Mountain.jpg");
	maps.push_back("maps/Field.jpg");
	maps.push_back("maps/City.jpg");

	sf::Music music;                                        // initialize music buffers
	if (!music.openFromFile("Sound/turmoil.wav"))
		std::cout << "Music file missing" << std::endl;
	music.play();

	sf::SoundBuffer gunbuffer;
	gunbuffer.loadFromFile("Sound/gun.wav");
	sf::Sound gun;
	gun.setBuffer(gunbuffer);
	gunsound_ = gun;
	sf::SoundBuffer crashbuffer;
	crashbuffer.loadFromFile("Sound/crash.wav");
	sf::Sound crash;
	crash.setBuffer(crashbuffer);
	crashsound_ = crash;
	unsigned planeIndex = 0, mapIndex = 0, playerIndex = 0;

    while(main_window.isOpen()){
		
        int ret = main_menu();
        std::cout << ret << std::endl;
        if (ret == 0 ){  
            main_window.close();
        }
		
		if (ret == 1) {
			show_instructions();
			music.pause();
			game_loop(planeIndex, mapIndex);
			game_ = Game();
			music.play();
			planeIndex = 0;
			mapIndex = 0;
			playerIndex = 0;
			main_window.clear();
		}

        if (ret == 2){
            playerIndex = multiplayers();
        }

		if (ret == 3) {
			planeIndex = selectPlane();//select plane
		}
		
		if (ret == 4) {
			mapIndex = selectMap();
			//select map
		}
    }
}

int main(){
    sf::RenderWindow window(sf::VideoMode(1280, 720), "Main menu",sf::Style::Default);
    Game game = Game();
    App test = App(window, game);
    test.app_main();
}

unsigned App::multiplayers() {
	std::vector<std::string> vecStr;
	std::vector<sf::Vector2f> origin;
	sf::Vector2f temp_origin;
	vecStr.push_back("One Player");
	vecStr.push_back("Two Players");
	vecStr.push_back("Exit");

	for (unsigned i = 0; i < vecStr.size(); i++) {
		temp_origin.x = main_window.getSize().x / 2;
		temp_origin.y = (main_window.getSize().y / (vecStr.size() + 1) * 3) + i * 60;
		origin.push_back(temp_origin);
	}


	main_window.clear();
	Menu menu(main_window, vecStr, origin);
	main_window.display();
	while (main_window.isOpen()) {
		main_window.clear();
		Background(background[1]);
		menu.draw(main_window);
		sf::Event event;

		while (main_window.pollEvent(event))
		{
			switch (event.type)
			{
			case sf::Event::KeyReleased:
				switch (event.key.code)
				{
				case sf::Keyboard::Up:
					menu.MoveUp();
					break;

				case sf::Keyboard::Down:
					menu.MoveDown();
					break;

				case sf::Keyboard::Return:
					switch (menu.GetPressedItem())
					{
					case 0:
						std::cout << "One Player" << std::endl;
						return 0;
						break;

					case 1:
						std::cout << "Two players" << std::endl;
						return 1;
						break;
					case 2:
						std::cout << "Close button has been pressed" << std::endl;
						main_window.close();
						return 0;
						
						break;
					}

					break;
				}

				break;
			case sf::Event::Closed:
				main_window.close();
				break;
			}


			if (sf::Mouse::isButtonPressed(sf::Mouse::Left)) {
				int valid = -1;
				valid = menu.MouseEvent(sf::Mouse::getPosition(main_window));
				if (valid >= 0 && valid < menu.get_Number_Of_Items())
					return valid;
				else
					break;
			}
		}
		menu.MouseEvent(sf::Mouse::getPosition(main_window));
		main_window.display();
	}
	return 0;

}



unsigned App::selectPlane() {

	std::vector<std::string> vecStr;
	std::vector<sf::Vector2f> origin;
	sf::Vector2f temp_origin;
	vecStr.push_back("Red");
	vecStr.push_back("Green");
	vecStr.push_back("Gray");
	vecStr.push_back("Exit");

	for (unsigned i = 0; i < vecStr.size(); i++) {
		temp_origin.x = main_window.getSize().x / 2;
		temp_origin.y = (main_window.getSize().y / (vecStr.size() + 1) * 3) + i * 60;
		origin.push_back(temp_origin);
	}


	main_window.clear();
	Menu menu(main_window, vecStr, origin);
	main_window.display();
	while (main_window.isOpen()) {
		main_window.clear();
		Background(background[1]);
		menu.draw(main_window);
		sf::Event event;

		while (main_window.pollEvent(event))
		{
			switch (event.type)
			{
			case sf::Event::KeyReleased:
				switch (event.key.code)
				{
				case sf::Keyboard::Up:
					menu.MoveUp();
					break;

				case sf::Keyboard::Down:
					menu.MoveDown();
					break;

				case sf::Keyboard::Return:
					switch (menu.GetPressedItem())
					{
					case 0:
						std::cout << "Red" << std::endl;
						return 0;
						break;

					case 1:
						std::cout << "Green" << std::endl;
						return 1;
						break;
					case 2:
						std::cout << "Gray" << std::endl;
						return 2;
						break;
					case 3:
						std::cout << "Close button has been pressed" << std::endl;
						main_window.close();
						return 0;
						break;
					}

					break;
				}

				break;
			case sf::Event::Closed:
				main_window.close();
				break;
			}


			if (sf::Mouse::isButtonPressed(sf::Mouse::Left)) {
				int valid = -1;
				valid = menu.MouseEvent(sf::Mouse::getPosition(main_window));
				if (valid >= 0 && valid < menu.get_Number_Of_Items())
					return valid;
				else
					break;
			}
		}
		menu.MouseEvent(sf::Mouse::getPosition(main_window));
		main_window.display();
	}
	return 0;
}

unsigned App::selectMap() {
	std::vector<std::string> vecStr;
	std::vector<sf::Vector2f> origin;
	sf::Vector2f temp_origin;
	vecStr.push_back("High Altitude");
	vecStr.push_back("Mountain");
	vecStr.push_back("Fields");
	vecStr.push_back("City");
	vecStr.push_back("Exit");

	for (unsigned i = 0; i < vecStr.size(); i++) {
		temp_origin.x = main_window.getSize().x / 2;
		temp_origin.y = (main_window.getSize().y / (vecStr.size() + 1) * 3) + i * 60;
		origin.push_back(temp_origin);
	}


	main_window.clear();
	Menu menu(main_window, vecStr, origin);
	main_window.display();
	while (main_window.isOpen()) {
		main_window.clear();
		Background(background[1]);
		menu.draw(main_window);
		sf::Event event;

		while (main_window.pollEvent(event))
		{
			switch (event.type)
			{
			case sf::Event::KeyReleased:
				switch (event.key.code)
				{
				case sf::Keyboard::Up:
					menu.MoveUp();
					break;

				case sf::Keyboard::Down:
					menu.MoveDown();
					break;

				case sf::Keyboard::Return:
					switch (menu.GetPressedItem())
					{
					case 0:
						std::cout << "High Altitude" << std::endl;
						return 0;
						break;

					case 1:
						std::cout << "Mountain" << std::endl;
						return 1;
						break;

					case 2:
						std::cout << "Field" << std::endl;
						return 2;
						break;
					case 3:
						std::cout << "City" << std::endl;
						return 3;
						break;
					case 4:
						std::cout << "Close button has been pressed" << std::endl;
						main_window.close();
						return 0;
						break;
					}

					break;
				}

				break;
			case sf::Event::Closed:
				main_window.close();
				break;
			}


			if (sf::Mouse::isButtonPressed(sf::Mouse::Left)) {
				int valid = -1;
				valid = menu.MouseEvent(sf::Mouse::getPosition(main_window));
				if (valid >= 0 && valid < menu.get_Number_Of_Items())
					return valid;
				else
					break;
			}
		}
		menu.MouseEvent(sf::Mouse::getPosition(main_window));
		main_window.display();
	}
	return 0;


}
