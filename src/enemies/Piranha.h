#ifndef MARIO_MAKER_PIRANHA_H
#define MARIO_MAKER_PIRANHA_H


#include "../Entity.h"
#include "../Mario.h"

class Piranha : public Entity {
public:
	Piranha(float px, float py, Texture texture);

	[[nodiscard]] Rectangle rect() const override;
	void on_collide(EntityCollision collision) override;
	void render(Vector2 top_left, Vector2 size) override;
	void update(const TileGrid& level, const InputState& keyboard_input) override;
	bool should_remove() override;
	EntityType type() override { return EntityType::SpikeEnemy; }

	int get_Dormanplant() {
		return dormantPlant;
	}
	int get_Activeplant() {
		return activePlant;
	}

private:
	Texture tex;
	Vector2 initPos;
	Vector2 position;
	int dormantPlant; // keep track of how long the plant is dormant
	int activePlant; // keep track of how long the plant is active
	bool is_dead;
};

#endif //MARIO_MAKER_PIRANHA_H
