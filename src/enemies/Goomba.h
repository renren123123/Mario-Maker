#ifndef MARIO_MAKER_GOOMBA_H
#define MARIO_MAKER_GOOMBA_H

#include "../Entity.h"

class Goomba : public Entity {
public:

    Goomba(float px, float py, Texture texture);

    [[nodiscard]] Rectangle rect() const override;
    void on_collide(EntityCollision collision) override;
    void render(Vector2 top_left, Vector2 size) override;
    void update(const TileGrid& level, const InputState & keyboard_input) override;
    bool should_remove() override;
    EntityType type() override { return EntityType::JumpEnemy; }

    Vector2 get_position() { return{ position }; };
    bool is_goomba_dead() {
        return is_dead;
    };
    bool is_goomba_squished() {
        return is_squished;
    }
private:
    Texture tex;
    Vector2 position;
    Vector2 velocity;
    bool is_dead;
    bool walkingright = true;
    int runanimationframe = 0;
    bool counterstart = false;
    bool is_squished = false;
    int counter = 0;
};

#endif