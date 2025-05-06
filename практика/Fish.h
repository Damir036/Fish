#pragma once
#include <SFML/Graphics.hpp>
#include <vector>

// Базовый класс для всех рыб
class Fish {
public:
    Fish(float x, float y, sf::Texture& texture, float scale, float speed);
    virtual void update(std::vector<Fish*>& fishes, float deltaTime) = 0;
    void draw(sf::RenderWindow& window);
    sf::Vector2f getPosition() const;
    sf::Sprite getSprite() const;
    sf::Vector2f getVelocity() const;
    void setVelocity(const sf::Vector2f& newVelocity);
    float getSpeed() const;

protected:
    sf::Sprite sprite; // Спрайт рыбы
    sf::Vector2f velocity; // Скорость рыбы
    float speed; // Скорость движения рыбы
    float visionRadius; // Радиус видимости

    sf::Vector2f normalize(const sf::Vector2f& vector); // Нормализация вектора
    void handleBorders(const sf::Vector2u& windowSize); // Обработка пересечения границ экрана
    virtual void applyBehavior(std::vector<Fish*>& fishes) = 0; // Поведение рыбы (для переопределения в наследниках)
};

// Класс для рыбы-жертвы
class Prey : public Fish {
public:
    Prey(float x, float y, sf::Texture& texture, float scale, float speed);
    void update(std::vector<Fish*>& fishes, float deltaTime) override;

private:
    void updateRotation(); // Обновление поворота рыбы
    void applyBehavior(std::vector<Fish*>& fishes) override; // Реализация поведения для жертвы
};

// Класс для рыбы-хищника
class Predator : public Fish {
public:
    Predator(float x, float y, sf::Texture& texture, float scale, float speed);
    void update(std::vector<Fish*>& fishes, float deltaTime) override;
    bool tryToEatPrey(Fish* prey); // Попытка съесть жертву

private:
    void updateRotation(); // Обновление поворота рыбы
    void applyBehavior(std::vector<Fish*>& fishes) override; // Реализация поведения для хищника
};

// Перегрузка операторов для sf::Vector2f
sf::Vector2f& operator/=(sf::Vector2f& left, float right);
sf::Vector2f operator/(const sf::Vector2f& left, float right);
