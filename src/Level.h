//
// Created by 16182 on 3/2/2022.
//

#ifndef MARIO_MAKER_LEVEL_H
#define MARIO_MAKER_LEVEL_H

#include "TileGrid.h"
#include "Entity.h"
#include "InputState.h"
#include<memory>
#include<nlohmann/json.hpp>
#include "EntitySpawn.h"
#include "Mario.h"


class Level {
public:
    explicit Level(Texture tileset_texture, Texture sprite_texture,float px, float py): grid_(tileset_texture, 100, 16), mario_(std::make_unique<Mario>(px, py, sprite_texture, this)){}


    void update(InputState keyboard_input);
    void render(Vector2 top_left, Vector2 size);

    void delete_entity(float x, float y);

    Entity* add_entity_editor(EntitySpawn ent, Texture tex) {
        entity_spawns_.push_back(ent);
        auto spawned = ent.make(tex, &mario());
        auto ret = spawned.get();
        entities_.push_back(std::move(spawned));
        return ret;
    }

    Entity* add_entity_transient(EntitySpawn ent, Texture tex) {
        auto spawned = ent.make(tex, &mario());
        auto ret = spawned.get();
        entities_.push_back(std::move(spawned));
        return ret;
    }

    void set_tile(int x, int y, Tile tile){
        grid_.at_mut(x, y) = tile;
    }

    Mario& mario(){
        return *mario_;
    }
    Vector2 get_camera_offset(){
        auto r = mario_->rect();
        return {r.x + r.width / 2, 0};
    }

    int number_of_entities() {
        return entities_.size();
    }

    nlohmann::json to_json();
    static Level from_json(const nlohmann::json &json, Texture tile_tex, Texture sprite_tex);

private:
    Level(const nlohmann::json& grid_json, const nlohmann::json& entities_json, Texture grid_tex, Texture sprite_tex);
public:TileGrid grid_;
    std::vector<std::unique_ptr<Entity>> entities_;
    std::vector<EntitySpawn> entity_spawns_;
    std::unique_ptr<Mario> mario_;
};


#endif //MARIO_MAKER_LEVEL_H
