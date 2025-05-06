#include <SFML/Graphics.hpp>
#include "Fish.h"
#include <vector>
#include <cstdlib>
#include <ctime>

int main() {
    sf::RenderWindow window(sf::VideoMode(1600, 900), "Fish Simulation");
    std::srand(static_cast<unsigned>(std::time(nullptr))); // ������������� ���������� ����������

    // �������� �������� ����
    sf::Texture backgroundTexture;
    if (!backgroundTexture.loadFromFile("background.jpg")) {
        return -1; // ������ ��������
    }
    sf::Sprite backgroundSprite;
    backgroundSprite.setTexture(backgroundTexture);
    backgroundSprite.setScale(
        window.getSize().x / backgroundSprite.getLocalBounds().width,
        window.getSize().y / backgroundSprite.getLocalBounds().height
    );

    // �������� ������� ��� ���
    sf::Texture preyTexture;
    if (!preyTexture.loadFromFile("prey.png")) {
        return -1; // ������ ��������
    }

    sf::Texture predatorTexture;
    if (!predatorTexture.loadFromFile("predator.png")) {
        return -1; // ������ ��������
    }

    // �������� ��� ���
    float preyScale = 0.05f;     // ������� ��� �����
    float predatorScale = 0.02f;  // ������� ��� ��������

    // �������� ��� ���
    float preySpeed = 100.0f;
    float predatorSpeed = 150.0f; // �������� �������� ���������

    // �������� �������� � ������
    std::vector<Fish*> fishes;
    for (int i = 0; i < 30; ++i) {
        fishes.push_back(new Prey(static_cast<float>(rand() % window.getSize().x),
            static_cast<float>(rand() % window.getSize().y),
            preyTexture, preyScale, preySpeed));
    }
    for (int i = 0; i < 5; ++i) {
        fishes.push_back(new Predator(static_cast<float>(rand() % window.getSize().x),
            static_cast<float>(rand() % window.getSize().y),
            predatorTexture, predatorScale, predatorSpeed));
    }

    sf::Clock clock;
    while (window.isOpen()) {
        sf::Event event;
        while (window.pollEvent(event)) {
            if (event.type == sf::Event::Closed)
                window.close();
        }

        float deltaTime = clock.restart().asSeconds();

        // ���������� ��������� ���� ���
        for (auto& fish : fishes) {
            fish->update(fishes, deltaTime);
        }

        // ��������� �����
        window.clear();
        window.draw(backgroundSprite);
        for (auto& fish : fishes) {
            fish->draw(window);
        }
        window.display();
    }

    // ������� ������
    for (auto& fish : fishes) {
        delete fish;
    }

    return 0;
}
