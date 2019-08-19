#pragma once
#include "SFML/Graphics.hpp"
#include "exceptions.hpp"

//#define MAX_NUMBER_OF_ITEMS 5

class Menu
{
public:
	Menu(sf::RenderWindow &window , std::vector<std::string> str, std::vector<sf::Vector2f>);
	~Menu();

	void draw(sf::RenderWindow &window);
	void MoveUp();
	void MoveDown();
	void BoxSetup(std::vector<sf::Vector2f>);
	void TextSetup(std::vector<std::string>, std::vector<sf::Vector2f>);
	//void setTextOrigin(std::vector<sf::Vector2f>);
	int GetPressedItem() { return selectedItemIndex; }
	int MouseEvent(sf::Vector2i mousePos);
	void BoxHighlight(int);
	void BoxClear(int);
	int get_Number_Of_Items();

private:
	int selectedItemIndex;
	sf::Font font;
	std::vector<sf::Text> menu;
	std::vector<sf::RectangleShape> Box;
	std::vector<sf::FloatRect>  textBound;
	std::vector<sf::FloatRect> boxBound;
	float boxWidth, boxHeight;
	std::vector<float> x_min;
	std::vector<float> x_max;
	std::vector<float> y_min;
	std::vector<float> y_max;

};
