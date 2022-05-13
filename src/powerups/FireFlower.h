#pragma once

#include "../Entity.h"

class FireFlower : public Entity {
public:

    FireFlower(float px, float py, Texture texture);

    [[nodiscard]] Rectangle rect() const override;
    void on_collide(EntityCollision collision) override;
    void render(Vector2 top_left, Vector2 size) override;
    void update(const TileGrid& level, const InputState& keyboard_input) override;
    bool should_remove() override;
    EntityType type() override { return EntityType::FireFlower; }
    Vector2 getPosition() { return position; }
private:
    Texture tex;
    Vector2 position;
    Vector2 velocity;
    bool is_dead;
};