#include "TanookieLeaf.h"
#include <raymath.h>
#include <iostream>

/**
 * Constructor for TanookieLeaf class. Sets private variable of TanookieLeaf class with given parameters. 
 * 
 * @param px start x axis location
 * @param py start y axis location
 * @param texture rendered TanookieLeaf image sprite
 */
TanookieLeaf::TanookieLeaf (float px, float py, Texture texture) : tex(texture), position({ px, py }), velocity({ 0,0 }) , is_dead(false) {}

/**
 * Renders(draw) TanookieLeaf on graphic. Accepts two Vector2 as parameters
 * 
 * @param top_left top left location of mario on graphic 
 * @param size size of TanookieLeaf on graphic 
 */
void TanookieLeaf::render(Vector2 top_left, Vector2 size) {
    DrawTexturePro(tex, SpriteLocations::TanookieLeafLeft, Rectangle{ 0, 0, 64, 64 }, Vector2Subtract(top_left, Vector2Multiply(position, { 64.f, 64.f })), 0, WHITE);
}

/**
 * Updates location and direction of TanookieLeaf entity
 * 
 * @param level TileGrid object to determine collision 
 * @param keyboard_input pressed keyboard by user
 */
void TanookieLeaf::update (const TileGrid& level, const InputState & keyboard_input) {

    position = Vector2Add(position, velocity);
    velocity.y += 0.02;

    //terminate the loop if too many collisions
    for(int coll_idx = 0; coll_idx < 10; coll_idx++) {
        auto collisions = level.collide(rect());

        if (collisions.eject_vector.has_value()) {

            auto eject = collisions.eject_vector.value();

            //ignore collisions that are impossible because of our movement direction
            if (Vector2DotProduct(eject, velocity) > 0) break;
            position = Vector2Add(position, eject);
            auto eject_norm = Vector2Normalize(eject);
            auto velocity_diff = Vector2DotProduct(velocity, eject_norm);
            velocity = Vector2Subtract(velocity, Vector2Multiply(eject_norm, { velocity_diff, velocity_diff }));
        } else {
            break;
        }
    }
}

/**
 * Resize hitbox of TanookieLeaf entity
 * (Hitbox refers padding of entity image that determins collision with other entity)
 * 
 * @return resized hitbox 
 */
Rectangle TanookieLeaf::rect() const {
    return {position.x, position.y, 0.9, 0.9};
}

/**
 * Collides entity against TanookieLeaf. 
 * If TanookieLeaf collides with Mario, Leaf is disappeared and Mario gets powerup.
 * 
 *  @param collision array of colided entity set 
 */
void TanookieLeaf::on_collide(EntityCollision collision) {
    if(collision.other.type() == EntityType::Mario){
        is_dead = true;
    }
}

/**
 * Determines if TanookieLeaf should be disappeared or not 
 * 
 * @return is_dead boolean private member variable 
 */
bool TanookieLeaf::should_remove() {
    return is_dead;
}
