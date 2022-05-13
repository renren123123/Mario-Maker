//
// Created by 16182 on 3/2/2022.
//

#include "Level.h"

/**
 * Constructor for level class. Add tile and entity to level respectively.
 * 
 * @param grid_json json type of TileGrid
 * @param entities_json json type of Entity
 * @param grid_tex  texture type of TileGrid
 * @param sprite_tex  texture type of Entity
 */
Level::Level(const nlohmann::json &grid_json, const nlohmann::json& entities_json, Texture grid_tex, Texture sprite_tex): grid_(TileGrid::from_json(grid_json, grid_tex)), mario_(std::make_unique<Mario>(5, 5, sprite_tex, this)) {
    for(const auto &ent : entities_json){
        add_entity_editor(EntitySpawn::from_json(ent), sprite_tex);
    }
}

/**
 * Updates location and direction of each entity in level
 * 
 * @param keyboard_input pressed keyboard by user to determine directions/jump of mario entity 
 */
void Level::update(InputState keyboard_input) {
    mario_->setLevel(this);
    for(auto & e : entities_){
        e->update(grid_, keyboard_input);
    }

    mario().update(grid_, keyboard_input);

    for(auto & e1 : entities_){
        for(auto & e2 : entities_){
            if(&e1 != &e2){
                if(auto collision = collide_rect(e1->rect(),e2->rect()); collision.has_value()){
                    e1->on_collide({ .other = *e2, .side = collision->collision_side});
                }
            }
        }
    }

    for(auto & e : entities_){
        if(auto collision = collide_rect(e->rect(),mario_->rect()); collision.has_value()){
            e->on_collide({ .other = *mario_, .side = collision->collision_side});
        }
        if(auto collision = collide_rect(mario_->rect(),e->rect()); collision.has_value()){
            mario_->on_collide({ .other = *e, .side = collision->collision_side});
        }
    }

    entities_.erase(std::remove_if(entities_.begin(), entities_.end(), [](auto & e){ return e->should_remove();}), entities_.end());
}

/**
 * Renders(draw) entity on graphic. Accepts two Vector2 as parameters
 *
 * @param top_left top left location of mario on graphic
 * @param size size of FireFlower on graphic
 */
void Level::render(Vector2 top_left, Vector2 size) {
    grid_.render(top_left, size);
//    for(auto & e : entities_){
//        Rectangle r = e->rect();
//        Vector2 pos = {r.x * 64, r.y * 64};
//        Vector2 size = {r.width * 64, r.height * 64};
//        DrawRectangleV(pos, size, GREEN);
//    }
    for(auto & e : entities_){
        e->render(top_left, size);
    }
    mario_->render(top_left, size);
}

/**
 * This function converts tile location/data into json in order to save/load level 
 * 
 * @return converted json
 */
nlohmann::json Level::to_json() {
    auto json = nlohmann::json{};
    json["tiles"] = std::move(grid_.to_json());
    json["entities"] = nlohmann::json::array();
    for(auto spawn : entity_spawns_){
        json["entities"].push_back(spawn.to_json());
    }
    return json;
}

/**
 * This function converts json into level class
 * 
 * @param json json type data 
 * @param tile_tex texture type of tile for level
 * @param sprite_tex texture type of sprite for level
 * @return converted level class 
 */
Level Level::from_json(const nlohmann::json &json, Texture tile_tex, Texture sprite_tex) {
    return {json["tiles"], json["entities"], tile_tex, sprite_tex};
}


void Level::delete_entity(float x, float y) {
    entities_.erase(std::remove_if(entities_.begin(), entities_.end(), [&](auto& e) {

        return (e->rect().x == static_cast<int>(x)) && (static_cast<int>(e->rect().y) == static_cast<int>(y));
        }), entities_.end());
}

