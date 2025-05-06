#include "Fish.h"
#include <cmath>

// ����������� �������� ������ Fish
Fish::Fish(float x, float y, sf::Texture& texture, float scale, float speed)
    : speed(speed), visionRadius(75.0f) {
    sprite.setTexture(texture);
    sprite.setPosition(x, y);
    sprite.setScale(scale, scale); // ������������� �������
    sprite.setOrigin(texture.getSize().x / 2.0f, texture.getSize().y / 2.0f); // ���������� ������

    // ��������� ��������� ��������
    velocity = sf::Vector2f(static_cast<float>(rand() % 200 - 100) / 100, static_cast<float>(rand() % 200 - 100) / 100);
    velocity = normalize(velocity) * speed;
}

// ��������� ����
void Fish::draw(sf::RenderWindow& window) {
    window.draw(sprite);
}

// ��������� ������� ����
sf::Vector2f Fish::getPosition() const {
    return sprite.getPosition();
}

// ��������� ������� ����
sf::Sprite Fish::getSprite() const {
    return sprite;
}

// ��������� �������� ����
sf::Vector2f Fish::getVelocity() const {
    return velocity;
}

// ��������� ����� �������� ����
void Fish::setVelocity(const sf::Vector2f& newVelocity) {
    velocity = newVelocity;
}

// ��������� �������� �������� ����
float Fish::getSpeed() const {
    return speed;
}

// ������������ �������
sf::Vector2f Fish::normalize(const sf::Vector2f& vector) {
    float length = std::sqrt(vector.x * vector.x + vector.y * vector.y);
    if (length != 0)
        return sf::Vector2f(vector.x / length, vector.y / length);
    return vector;
}

// ��������� ����������� ������ ������
void Fish::handleBorders(const sf::Vector2u& windowSize) {
    sf::Vector2f position = sprite.getPosition();

    if (position.x < 0) {
        position.x = 0;
        velocity.x = -velocity.x;
    }
    else if (position.x > windowSize.x) {
        position.x = static_cast<float>(windowSize.x);
        velocity.x = -velocity.x;
    }

    if (position.y < 0) {
        position.y = 0;
        velocity.y = -velocity.y;
    }
    else if (position.y > windowSize.y) {
        position.y = static_cast<float>(windowSize.y);
        velocity.y = -velocity.y;
    }

    sprite.setPosition(position);
}

// ����������� ������ Prey (����-������)
Prey::Prey(float x, float y, sf::Texture& texture, float scale, float speed)
    : Fish(x, y, texture, scale, speed) {
    // �����������: ��������� ����� ��� ����������� ��������, ���� �� ������������ ��������
}

// ���������� ��������� ����-������
void Prey::update(std::vector<Fish*>& fishes, float deltaTime) {
    sf::Vector2f fleeDirection(0, 0);
    for (auto& fish : fishes) {
        if (Predator* predator = dynamic_cast<Predator*>(fish)) {
            float distance = std::sqrt(std::pow(predator->getPosition().x - sprite.getPosition().x, 2) +
                std::pow(predator->getPosition().y - sprite.getPosition().y, 2));
            if (distance < visionRadius) {
                sf::Vector2f toPredator = sprite.getPosition() - predator->getPosition();
                fleeDirection += normalize(toPredator) * (visionRadius - distance) / visionRadius;
            }
        }
    }

    if (fleeDirection != sf::Vector2f(0, 0)) {
        fleeDirection = normalize(fleeDirection) * speed;
        velocity = fleeDirection;
    }

    applyBehavior(fishes);

    sprite.move(velocity * deltaTime);
    handleBorders(sprite.getTexture()->getSize());
    updateRotation();
}

// ���������� ��������� ��� ����-������
void Prey::applyBehavior(std::vector<Fish*>& fishes) {
    sf::Vector2f separation(0, 0);
    sf::Vector2f alignment(0, 0);
    sf::Vector2f cohesion(0, 0);
    int neighborCount = 0;

    for (auto& fish : fishes) {
        float distance = std::sqrt(std::pow(fish->getPosition().x - sprite.getPosition().x, 2) +
            std::pow(fish->getPosition().y - sprite.getPosition().y, 2));
        if (fish != this && distance < visionRadius) {
            separation += normalize(sprite.getPosition() - fish->getPosition()) / distance;
            alignment += fish->getVelocity();
            cohesion += fish->getPosition();
            neighborCount++;
        }
    }

    if (neighborCount > 0) {
        separation /= neighborCount;
        alignment /= neighborCount;
        cohesion /= neighborCount;

        cohesion = normalize(cohesion - sprite.getPosition()) * speed;
    }

    velocity += separation + alignment + cohesion;
    velocity = normalize(velocity) * speed;
    updateRotation();
}

// ���������� �������� ����-������
void Prey::updateRotation() {
    float angle = std::atan2(velocity.y, velocity.x) * 180 / 3.14159265f;
    sprite.setRotation(angle);
}

// ����������� ������ Predator (����-������)
Predator::Predator(float x, float y, sf::Texture& texture, float scale, float speed)
    : Fish(x, y, texture, scale, speed) {
    // �����������: ��������� ����� ��� ����������� ��������, ���� �� ������������ ��������
}

// ���������� ��������� ����-�������
void Predator::update(std::vector<Fish*>& fishes, float deltaTime) {
    sf::Vector2f chaseDirection(0, 0);
    for (auto it = fishes.begin(); it != fishes.end(); ) {
        if (Prey* prey = dynamic_cast<Prey*>(*it)) {
            float distance = std::sqrt(std::pow(prey->getPosition().x - sprite.getPosition().x, 2) +
                std::pow(prey->getPosition().y - sprite.getPosition().y, 2));
            if (distance < visionRadius) {
                sf::Vector2f toPrey = prey->getPosition() - sprite.getPosition();
                chaseDirection += normalize(toPrey) * (visionRadius - distance) / visionRadius;
                if (distance < 5.0f) {
                    if (tryToEatPrey(prey)) {
                        it = fishes.erase(it); // �������� ��������� ����
                        continue;
                    }
                }
            }
        }
        ++it;
    }
    if (chaseDirection != sf::Vector2f(0, 0)) {
        chaseDirection = normalize(chaseDirection) * speed;
        velocity = chaseDirection;
    }

    applyBehavior(fishes);

    sprite.move(velocity * deltaTime);
    handleBorders(sprite.getTexture()->getSize());
    updateRotation();
}

// ���������� ��������� ��� ����-�������
void Predator::applyBehavior(std::vector<Fish*>& fishes) {
    sf::Vector2f separation(0, 0);
    sf::Vector2f alignment(0, 0);
    sf::Vector2f cohesion(0, 0);
    int neighborCount = 0;

    for (auto& fish : fishes) {
        float distance = std::sqrt(std::pow(fish->getPosition().x - sprite.getPosition().x, 2) +
            std::pow(fish->getPosition().y - sprite.getPosition().y, 2));
        if (fish != this && distance < visionRadius) {
            separation += normalize(sprite.getPosition() - fish->getPosition()) / distance;
            alignment += fish->getVelocity();
            cohesion += fish->getPosition();
            neighborCount++;
        }
    }

    if (neighborCount > 0) {
        separation /= neighborCount;
        alignment /= neighborCount;
        cohesion /= neighborCount;

        cohesion = normalize(cohesion - sprite.getPosition()) * speed;
    }

    velocity += separation + alignment + cohesion;
    velocity = normalize(velocity) * speed;
    updateRotation();
}

// ���������� �������� ����-�������
void Predator::updateRotation() {
    float angle = std::atan2(velocity.y, velocity.x) * 180 / 3.14159265f + 180; // �������� �� 180 �������� ��� ��������
    sprite.setRotation(angle);
}

// ������ ������� ������ ������
bool Predator::tryToEatPrey(Fish* prey) {
    // ����� ����� ���� ����� ������� ������
    return true;
}

// ���������� ���������� ��� sf::Vector2f
sf::Vector2f& operator/=(sf::Vector2f& left, float right) {
    left.x /= right;
    left.y /= right;
    return left;
}

sf::Vector2f operator/(const sf::Vector2f& left, float right) {
    return sf::Vector2f(left.x / right, left.y / right);
}
