#ifndef MARIO_MAKER_ENTITY_H
#define MARIO_MAKER_ENTITY_H

#include"Tile.h"
#include"TileGrid.h"
#include "InputState.h"
#include<memory>


//Class called Entity. This class is for villians and mario powerUps. 

class Entity;

struct EntityCollision {
    Entity& other;
    Side side;
};

enum class EntityType {
    Mario,
    JumpEnemy,
    SpikeEnemy,
    Mushroom,
    TanookieLeaf,
    SmallShroom,
    Piranha,
    FireFlower,
    FireBall
};


class Entity {
public:

    //the entity's hitbox
    [[nodiscard]] virtual Rectangle rect() const = 0;

    //update the entity based on a collision with another entity
    virtual void on_collide(EntityCollision collision) = 0;

    //draw the entity to the screen
    virtual void render(Vector2 top_left, Vector2 size) = 0;

    //update the entity
    virtual void update(const TileGrid &level, const InputState &keyboard_input) = 0;

    //whether the entity should be removed from the current entities list
    virtual bool should_remove() = 0;

    virtual EntityType type() = 0;

    virtual ~Entity() = default;
};


#endif //MARIO_MAKER_ENTITY_H
