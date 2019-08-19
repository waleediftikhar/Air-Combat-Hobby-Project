
#include "menu.hpp"


#include "iostream"
#include "exceptions.hpp"


Menu::Menu(sf::RenderWindow &window , std::vector<std::string> str , std::vector<sf::Vector2f> origin)
{
	if (!font.loadFromFile("DroidSans.ttf"))
	{
		throw FileException("Error: Failed to read file DroidSans.ttf");
	}

	TextSetup(str, origin);
	BoxSetup(origin);
	selectedItemIndex = 0;

}


void Menu::TextSetup(std::vector<std::string> str, std::vector<sf::Vector2f> origin)
{
	sf::Text temp;
	for (unsigned int i = 0; i < str.size(); i++)
	{
		temp.setString(str[i]);
		temp.setFont(font);
		temp.setFillColor(sf::Color::White);
		temp.setPosition(origin[i]);
		textBound.push_back(temp.getGlobalBounds());
		temp.setOrigin(textBound[i].width / 2, textBound[i].height / 1.5);
		menu.push_back(temp);
	}
}



Menu::~Menu()
{
}

void Menu::draw(sf::RenderWindow &window)
{
	for (int i = 0; i < menu.size(); i++)
	{
		window.draw(Box[i]);
		window.draw(menu[i]);
	}
}


void Menu::BoxClear(int i) {
		menu[i].setFillColor(sf::Color::White);
		menu[i].setCharacterSize(30);
		menu[i].setStyle(sf::Text::Regular);
		Box[i].setFillColor(sf::Color::Transparent);
		Box[i].setOutlineColor(sf::Color::Transparent);
		Box[i].setOutlineThickness(0);
}


void Menu::BoxSetup(std::vector<sf::Vector2f> origin) {
	sf::RectangleShape temp;
	sf::FloatRect temp_boxBound;
	float temp_xmin, temp_xmax, temp_ymin, temp_ymax;

	for (int i = 0; i < menu.size(); i++) {
		boxWidth = textBound[i].width * 2;
		boxHeight = textBound[i].height * 1.5;

		temp.setSize(sf::Vector2f(boxWidth, boxHeight));
		temp.setPosition(origin[i]);
		temp.setOrigin(boxWidth / 2, boxHeight / 2);
		temp.setFillColor(sf::Color::Transparent);
		temp.setOutlineColor(sf::Color::Transparent);
		temp.setOutlineThickness(2);

		if (i == 0) {
			temp.setOutlineColor(sf::Color::Red);
		}

		temp_boxBound = temp.getGlobalBounds();
		//std::cout << "Height = " << temp_boxBound.height << "\nTop = " << temp_boxBound.top << "\nWidth = " << temp_boxBound.width << "\nLeft = " << temp_boxBound.left << std::endl;

		temp_xmin = temp_boxBound.left;
		temp_xmax = temp_xmin + temp_boxBound.width;

		temp_ymin = temp_boxBound.top;
		temp_ymax = temp_ymin + temp_boxBound.height;

		//std::cout << "x_min = " << temp_xmin << "\nx_max = " << temp_xmax << "\ny_min = " << temp_ymin << "\ny_max = " << temp_ymax << std::endl;

		Box.push_back(temp);
		boxBound.push_back(temp_boxBound);
		x_min.push_back(temp_xmin);
		x_max.push_back(temp_xmax);
		y_min.push_back(temp_ymin);
		y_max.push_back(temp_ymax);
	}
}

int Menu::MouseEvent(sf::Vector2i mousePos) {
	
	//std::cout << mousePos.x << "  " << mousePos.y << std::endl;

	for (int i = 0; i < menu.size(); i++) {
		if ((mousePos.x > static_cast<int>(x_min[i])) & (mousePos.x < static_cast<int>(x_max[i]))) {
			if ((mousePos.y > static_cast<int>(y_min[i])) && (mousePos.y <= static_cast<int>(y_max[i]))) {
				BoxClear(selectedItemIndex);
				selectedItemIndex = i;
				BoxHighlight(i);
				if (i < menu.size() - 1) {
					return i + 1;
				}
				if (i == menu.size() - 1) {
					return 0;
				}
			}
		}
	}
}

void Menu::BoxHighlight(int i) {
	menu[i].setFillColor(sf::Color::Red);
	menu[i].setCharacterSize(30);
	menu[i].setStyle(sf::Text::Bold);
	Box[i].setOutlineColor(sf::Color::Red);
	Box[i].setOutlineThickness(2);
}





void Menu::MoveUp()
{
	if (selectedItemIndex - 1 >= 0)
	{
		BoxClear(selectedItemIndex);
		selectedItemIndex--;
		BoxHighlight(selectedItemIndex);
	}
}

void Menu::MoveDown()
{
	if (selectedItemIndex + 1 < menu.size())
	{
		BoxClear(selectedItemIndex);
		selectedItemIndex++;
		BoxHighlight(selectedItemIndex);
		
	}
}

int Menu::get_Number_Of_Items() {
	return menu.size();
}