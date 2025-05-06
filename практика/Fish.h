#pragma once
#include <SFML/Graphics.hpp>
#include <vector>

// ������� ����� ��� ���� ���
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
    sf::Sprite sprite; // ������ ����
    sf::Vector2f velocity; // �������� ����
    float speed; // �������� �������� ����
    float visionRadius; // ������ ���������

    sf::Vector2f normalize(const sf::Vector2f& vector); // ������������ �������
    void handleBorders(const sf::Vector2u& windowSize); // ��������� ����������� ������ ������
    virtual void applyBehavior(std::vector<Fish*>& fishes) = 0; // ��������� ���� (��� ��������������� � �����������)
};

// ����� ��� ����-������
class Prey : public Fish {
public:
    Prey(float x, float y, sf::Texture& texture, float scale, float speed);
    void update(std::vector<Fish*>& fishes, float deltaTime) override;

private:
    void updateRotation(); // ���������� �������� ����
    void applyBehavior(std::vector<Fish*>& fishes) override; // ���������� ��������� ��� ������
};

// ����� ��� ����-�������
class Predator : public Fish {
public:
    Predator(float x, float y, sf::Texture& texture, float scale, float speed);
    void update(std::vector<Fish*>& fishes, float deltaTime) override;
    bool tryToEatPrey(Fish* prey); // ������� ������ ������

private:
    void updateRotation(); // ���������� �������� ����
    void applyBehavior(std::vector<Fish*>& fishes) override; // ���������� ��������� ��� �������
};

// ���������� ���������� ��� sf::Vector2f
sf::Vector2f& operator/=(sf::Vector2f& left, float right);
sf::Vector2f operator/(const sf::Vector2f& left, float right);
