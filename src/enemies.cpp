#include "enemies.h"

Enemy::Enemy() {};
Enemy::~Enemy() {};

void Enemy::load() {

    sprite = LoadTexture("assets/sprites/enemies/grunt/1.png");

    pos = {500, 30, -300};
}

void Enemy::update() {};

void Enemy::draw(Camera *camera){

    DrawBillboard(*camera, sprite, pos, 1, WHITE);

}
