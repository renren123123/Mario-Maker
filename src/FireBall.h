#ifndef MARIO_MAKER_FIREBALL_H
#define MARIO_MAKER_FIREBALL_H

#include "Entity.h"

class FireBall : public Entity {
public:

    FireBall(float px, float py, Texture texture, int is_right);

    [[nodiscard]] Rectangle rect() const override;
    void on_collide(EntityCollision collision) override;
    void render(Vector2 top_left, Vector2 size) override;
    void update(const TileGrid& level, const InputState& keyboard_input) override;
    bool should_remove() override;
    EntityType type() override { return EntityType::FireBall; }
private:
    Texture tex;
    Vector2 position;
    Vector2 velocity;
    bool is_dead;
    int alive_count;
    int facing_right; // 1 for right, -1 for left
    int facing_down = 1; // 1 for down, -1 for up

    int frames_since_jump{};
    static constexpr float jump_instant_accel = 0.3;
    static constexpr float jump_continuous_accel = 0.025;
    static constexpr int jump_continuous_frames = 12;
    static constexpr int jump_continuous_delay = 4;
    static constexpr float gravity = 0.02;
    bool bounce = false;
};

#endif