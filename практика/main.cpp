#include <SFML/Graphics.hpp>
#include "Fish.h"
#include <vector>
#include <cstdlib>
#include <ctime>

int main() {
    sf::RenderWindow window(sf::VideoMode(1600, 900), "Fish Simulation");
    std::srand(static_cast<unsigned>(std::time(nullptr))); // Инициализация случайного генератора

    // Загрузка текстуры фона
    sf::Texture backgroundTexture;
    if (!backgroundTexture.loadFromFile("background.jpg")) {
        return -1; // Ошибка загрузки
    }
    sf::Sprite backgroundSprite;
    backgroundSprite.setTexture(backgroundTexture);
    backgroundSprite.setScale(
        window.getSize().x / backgroundSprite.getLocalBounds().width,
        window.getSize().y / backgroundSprite.getLocalBounds().height
    );

    // Загрузка текстур для рыб
    sf::Texture preyTexture;
    if (!preyTexture.loadFromFile("prey.png")) {
        return -1; // Ошибка загрузки
    }

    sf::Texture predatorTexture;
    if (!predatorTexture.loadFromFile("predator.png")) {
        return -1; // Ошибка загрузки
    }

    // Масштабы для рыб
    float preyScale = 0.05f;     // Масштаб для жертв
    float predatorScale = 0.02f;  // Масштаб для хищников

    // Скорости для рыб
    float preySpeed = 100.0f;
    float predatorSpeed = 150.0f; // Скорость хищников увеличена

    // Создание векторов с рыбами
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

        // Обновление состояния всех рыб
        for (auto& fish : fishes) {
            fish->update(fishes, deltaTime);
        }

        // Отрисовка сцены
        window.clear();
        window.draw(backgroundSprite);
        for (auto& fish : fishes) {
            fish->draw(window);
        }
        window.display();
    }

    // Очистка памяти
    for (auto& fish : fishes) {
        delete fish;
    }

    return 0;
}
