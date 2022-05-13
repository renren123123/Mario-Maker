#include "Goomba.h"
#include "../SpriteLocations.h"
#include <raymath.h>

/**
 * Constructor for Goomba class. Sets private variable of Goomba class with given parameters. 
 * 
 * @param px start x axis location
 * @param py start y axis location
 * @param texture rendered Goomba image sprite
 */
Goomba::Goomba (float px, float py, Texture texture) : tex(texture), position({ px, py }), velocity({ 0.05,0 }), is_dead(false) {}

/**
 * Renders(draw) Goomba on graphic. Accepts two Vector2 as parameters
 * 
 * @param top_left top left location of mario on graphic 
 * @param size size of Goomba on graphic 
 */
void Goomba::render(Vector2 top_left, Vector2 size) {
    auto src = SpriteLocations::GoombaStep1;
    if (counter == 20) {
        src = SpriteLocations::GoombaSquish;
        is_dead = true;
    }
    if (is_squished == true) {
        src = SpriteLocations::GoombaSquish;
    }

    else if ((runanimationframe / 16) % 2 == 0) {
        src = SpriteLocations::GoombaStep2;
    }

    if (walkingright == false) {
        src.width *= -1;
    }

    DrawTexturePro(tex, src, Rectangle{ 0, 0, 64, 64 }, Vector2Subtract(top_left, Vector2Multiply(position, { 64.f, 64.f })), 0, WHITE);
}

/**
 * Updates location and direction of Goomba entity
 * 
 * @param level TileGrid object to determine collision 
 * @param keyboard_input pressed keyboard by user
 */
void Goomba::update (const TileGrid& level, const InputState & keyboard_input) {


    if (is_squished == true) {
        counter++;
    }
    else {
        position = Vector2Add(position, velocity);
        velocity.y += 0.02;
        runanimationframe++;
    }

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


            if (std::find_if(collisions.collisions.begin(), collisions.collisions.end(), [](auto& a) {return a.collision.collision_side == Side::RIGHT; }) != collisions.collisions.end()) {
                velocity.x = -0.05;
                walkingright = false;
            }
            else if(std::find_if(collisions.collisions.begin(), collisions.collisions.end(), [](auto& a) {return a.collision.collision_side == Side::LEFT; }) != collisions.collisions.end()) {
                velocity.x = 0.05;
                walkingright = true;
            }
        }
        else {
            break;
        }
    }
}

/**
 * Resize hitbox of Goomba entity
 * (Hitbox refers padding of entity image that determins collision with other entity)
 * 
 * @return resized hitbox 
 */
Rectangle Goomba::rect() const {
    if (is_squished == true && counter >=1) {
        return { 0,0,0,0 };
    }
    return {position.x, position.y, 0.9, 0.9};
}

/**
 * Collides entity against Goomba. 
 * If Mario jumps on the top of Goomba, Goomba is disappeared and Mario gets smaller.
 * 
 *  @param collision array of colided entity set 
 */
void Goomba::on_collide(EntityCollision collision) {
    if(collision.other.type() == EntityType::Mario){
        if(collision.side == Side::TOP){
            is_squished = true;
        }
    }
    if (collision.other.type() == EntityType::FireBall) {
        is_dead = true;
    }
}

/**
 * Determines if Goomba should be disappeared or not 
 * 
 * @return is_dead boolean private member variable 
 */
bool Goomba::should_remove() {
    return is_dead;
}
