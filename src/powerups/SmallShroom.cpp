#include "SmallShroom.h"
#include "../SpriteLocations.h"
#include <raymath.h>
#include <iostream>

/**
 * Constructor for SmallShroom class. Sets private variable of SmallShroom class with given parameters. 
 * 
 * @param px start x axis location
 * @param py start y axis location
 * @param texture rendered SmallShroom image sprite
 */
SmallShroom::SmallShroom(float px, float py, Texture texture) : tex(texture), position({ px, py }), velocity({ 0.13,0 }), is_dead(false) {}

/**
 * Renders(draw) SmallShroom on graphic. Accepts two Vector2 as parameters
 * 
 * @param top_left top left location of mario on graphic 
 * @param size size of SmallShroom on graphic 
 */
void SmallShroom::render(Vector2 top_left, Vector2 size) {
    top_left = Vector2Subtract(top_left, { 32.f, 32.f });
    DrawTexturePro(tex, SpriteLocations::Mushroom, Rectangle{ 0, 0, 32, 32 }, Vector2Subtract(top_left, Vector2Multiply(position, { 64.f, 64.f })), 0, WHITE);
}

/**
 * Updates location and direction of SmallShroom entity
 * 
 * @param level TileGrid object to determine collision 
 * @param keyboard_input pressed keyboard by user
 */
void SmallShroom::update(const TileGrid& level, const InputState& keyboard_input) {

    position = Vector2Add(position, velocity);
    velocity.y += 0.02;

    //terminate the loop if too many collisions
    for (int coll_idx = 0; coll_idx < 10; coll_idx++) {
        auto collisions = level.collide(rect());

        if (collisions.eject_vector.has_value()) {

            auto eject = collisions.eject_vector.value();

            //ignore collisions that are impossible because of our movement direction
            if (Vector2DotProduct(eject, velocity) > 0) break;
            position = Vector2Add(position, eject);
            auto eject_norm = Vector2Normalize(eject);
            auto velocity_diff = Vector2DotProduct(velocity, eject_norm);
            velocity = Vector2Subtract(velocity, Vector2Multiply(eject_norm, { velocity_diff, velocity_diff }));


            if (std::find_if(collisions.collisions.begin(), collisions.collisions.end(), [](auto& a) {return a.collision.collision_side == Side::RIGHT; }) != collisions.collisions.end()) {
                velocity.x = -0.05;
            }
            else if (std::find_if(collisions.collisions.begin(), collisions.collisions.end(), [](auto& a) {return a.collision.collision_side == Side::LEFT; }) != collisions.collisions.end()) {
                velocity.x = 0.05;
            }
        }
        else {
            break;
        }
    }
}

/**
 * Resize hitbox of SmallShroom entity
 * (Hitbox refers padding of entity image that determins collision with other entity)
 * 
 * @return resized hitbox 
 */
Rectangle SmallShroom::rect() const {
    return { position.x, position.y, 0.9, 0.9 };
}

/**
 * Collides entity against SmallShroom. 
 * If Mario jumps on the top of SmallShroom, SmallShroom is disappeared and Mario gets powerup.
 * 
 *  @param collision array of colided entity set 
 */
void SmallShroom::on_collide(EntityCollision collision) {
    if (collision.side == Side::TOP) {
        is_dead = true;
    }
}

/**
 * Determines if SmallShroom should be disappeared or not 
 * 
 * @return is_dead boolean private member variable 
 */
bool SmallShroom::should_remove() {
    return is_dead;
}
