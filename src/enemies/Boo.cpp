#include "Boo.h"
#include "../SpriteLocations.h"
#include <raymath.h>

/**
 * Constructor for Boo class. Sets private variable of Boo class with given parameters. 
 * 
 * @param px start x axis location
 * @param py start y axis location
 * @param texture rendered Boo image sprite
 */
Boo::Boo(float px, float py, Texture texture, Mario* mpt) : position({ px, py }), velocity({ 0,0 }), tex(texture), MarioPointer(mpt),is_dead(false) {}

/**
 * Renders(draw) Boo on graphic. Accepts two Vector2 as parameters
 * 
 * @param top_left top left location of mario on graphic 
 * @param size size of Boo on graphic 
 */
void Boo::render(Vector2 top_left, Vector2 size) {
	auto src = SpriteLocations::BooOpen;
	Vector2 mPos = MarioPointer->getPosition();
	if (mPos.x < this->position.x) {
		if (is_shy == true) {
			src = SpriteLocations::BooClosed;
			src.width *= -1;
		}
		else {
			src = SpriteLocations::BooOpen;
		}
	}

	else {
		if (is_shy == true) {
			src = SpriteLocations::BooClosed;
		}
		else {
			src = SpriteLocations::BooOpen;
			src.width *= -1;
		}
	}
	DrawTexturePro(tex, src, Rectangle{ 0, 0, 64, 64 }, Vector2Subtract(top_left, Vector2Multiply(position, { 64.f, 64.f })), 0, WHITE);
}

/**
 * Updates location and direction of Boo entity
 * 
 * @param level TileGrid object to determine collision 
 * @param keyboard_input pressed keyboard by user
 */
void Boo::update(const TileGrid& level, const InputState& keyboard_input) {
	Vector2 mPos = MarioPointer->getPosition();
	if (mPos.x < this->position.x) {
		if (MarioPointer->is_right() == -1) {
			is_shy = false;
			position.x -= 0.03;
			if (mPos.y > this->position.y) {
				position.y += 0.03;
			}
			else if (mPos.y < this->position.y) {
				position.y -= 0.03;
			}
		}
		else {
			is_shy = true;
			position.x += 0;
			position.y += 0;
		}
	}
	else if (mPos.x > this->position.x) {
		if (MarioPointer->is_right() == 1) {
			is_shy = false;
			position.x += 0.03;
			if (mPos.y > this->position.y) {
				position.y += 0.03;
			}
			else if (mPos.y < this->position.y) {
				position.y -= 0.03;
			}
		}
		else {
			is_shy = true;
			position.x += 0;
			position.y += 0;
		}
	}

	
}

/**
 * Resize hitbox of Boo entity
 * (Hitbox refers padding of entity image that determins collision with other entity)
 * 
 * @return resized hitbox 
 */
Rectangle Boo::rect() const {
	return { position.x, position.y, 0.9, 0.9 };
}

/**
 * Collides entity against Boo. 
 * If Mario jumps on the top of Boo, Boo is disappeared and Mario gets smaller.
 * 
 *  @param collision array of colided entity set 
 */
void Boo::on_collide(EntityCollision collision) {
	if(collision.other.type() == EntityType::Mario){
		is_dead = false;
	}
}

/**
 * Determines if Boo should be disappeared or not 
 * 
 * @return is_dead boolean private member variable 
 */
bool Boo::should_remove() {
	return is_dead;
}