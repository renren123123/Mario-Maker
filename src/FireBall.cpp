#include "FireBall.h"
#include "SpriteLocations.h"
#include <raymath.h>
#include <iostream>

/**
 * Constructor for FireBall class. Sets private variable of FireBall class with given parameters.
 *
 * @param px start x axis location
 * @param py start y axis location
 * @param texture rendered FireBall image sprite
 */
FireBall::FireBall(float px, float py, Texture texture, int is_right) : tex(texture), position({ px, py }), is_dead(false), facing_right(is_right), alive_count(0) {
    velocity = { 1*0.1f, 0.1f };
}

/**
 * Renders(draw) FireBall on graphic. Accepts two Vector2 as parameters
 *
 * @param top_left top left location of mario on graphic
 * @param size size of FireBall on graphic
 */
void FireBall::render(Vector2 top_left, Vector2 size) {
    DrawTexturePro(tex, SpriteLocations::FireBall, Rectangle{ 0, 0, 30, 30}, Vector2Subtract(top_left, Vector2Multiply(position, { 64.f, 64.f })), 0, WHITE);
}

/**
 * Updates location and direction of FireBall entity
 *
 * @param level TileGrid object to determine collision
 * @param keyboard_input pressed keyboard by user
 */
#include<iostream>
void FireBall::update(const TileGrid& level, const InputState& keyboard_input) {
    alive_count++;
    if (alive_count == 120)
        is_dead = true;

    position = Vector2Add(position, Vector2Multiply(velocity, {(float)facing_right, 1}));

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
            //velocity = Vector2Subtract(velocity, Vector2Multiply(eject_norm, { velocity_diff, velocity_diff }));


            if (std::find_if(collisions.collisions.begin(), collisions.collisions.end(), [](auto& a) {return a.collision.collision_side == Side::RIGHT; }) != collisions.collisions.end()) {
                is_dead = true;
            }
            else if (std::find_if(collisions.collisions.begin(), collisions.collisions.end(), [](auto& a) {return a.collision.collision_side == Side::LEFT; }) != collisions.collisions.end()) {
                is_dead = true;
            }
            else if (std::find_if(collisions.collisions.begin(), collisions.collisions.end(), [](auto& a) {return a.collision.collision_side == Side::BOTTOM; }) != collisions.collisions.end()) {
                //facing_down = -1;
                bounce = true;
                frames_since_jump = 0;
                velocity.y -= jump_instant_accel;
            }
        }
        else {
            break;
        }
    }

    if (bounce
        && (frames_since_jump < jump_continuous_frames)
        && (frames_since_jump >= jump_continuous_delay)) {
        velocity.y -= jump_continuous_accel;
    }

    velocity.y += gravity;
    frames_since_jump++;
}

/**
 * Resize hitbox of FireBall entity
 * (Hitbox refers padding of entity image that determins collision with other entity)
 *
 * @return resized hitbox
 */
Rectangle FireBall::rect() const {
    return { position.x, position.y, 0.45, 0.45 };
}

/**
 * Collides entity against FireBall.
 * If Mario collides with FireBall, FireBall is disappeared and Mario gets powerup.
 *
 *  @param collision array of colided entity set
 */
void FireBall::on_collide(EntityCollision collision) {
    if (collision.other.type() != EntityType::Mario && collision.other.type() != EntityType::FireBall) {
        is_dead = true;
    }
}

/**
 * Determines if FireBall should be disappeared or not
 *
 * @return is_dead boolean private member variable
 */
bool FireBall::should_remove() {
    return is_dead;
}
