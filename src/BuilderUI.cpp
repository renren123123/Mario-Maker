//
// Created by 16182 on 3/23/2022.
//

#include "BuilderUI.h"

/**
 * Constructor for BuilderUI class. Sets private variable of BuilderUI class with given parameters. 
 * 
 * @param level level object that needs to be saved.
 * @param sprite_tex Texture entity image sprite 
 * @param title_tex Texture tile image sprite
 */

BuilderUI::BuilderUI(Level & level, Texture sprite_tex, Texture tile_tex, Texture x_tex): level(level), sprite_tex(sprite_tex), tile_tex(tile_tex), delete_tex(x_tex) {}

/**
 * Handles events based on keyboard input by user. 
 * User can set tite or add mushroom entity to level. 
 */
void BuilderUI::handle_events() {
    bool pressed_this_frame = IsMouseButtonDown(MOUSE_BUTTON_LEFT);
    if(pressed_this_frame && (!pressed_last_frame || current_action == TILE)){
        if(GetMouseX() > 1024 - 128){
            current_action = icon_at(GetMouseX(), GetMouseY());
        }
        else {
            auto offset = level.get_camera_offset();
            float x = GetMouseX() / 64.f + offset.x - 8;
            float y = GetMouseY() / 64.f + offset.y;
            if (current_action == TILE) {
                level.set_tile(x, y, { .solid = true, .tex_src = TileLocations::Ground });
            }
            else if (current_action == MUSHROOM) {
                level.add_entity_editor({x, y, EntitySpawn::Type::Mushroom}, sprite_tex);
            }
            else if (current_action == SMALLSHROOM) {
                level.add_entity_editor({x, y, EntitySpawn::Type::SmallShroom}, sprite_tex);
            }
            else if (current_action == FIREFLOWER) {
                level.add_entity_editor({x, y, EntitySpawn::Type::FireFlower}, sprite_tex);
            }
            else if (current_action == TANOOKIE) {
                level.add_entity_editor({x, y, EntitySpawn::Type::TanookieLeaf}, sprite_tex);
            }
            else if (current_action == GOOMBA) {
                level.add_entity_editor({x, y, EntitySpawn::Type::Goomba}, sprite_tex);
            }
            else if (current_action == BOO) {
                level.add_entity_editor({x, y, EntitySpawn::Type::Boo}, sprite_tex);
            }
            else if (current_action == PIRANHA) {
                level.add_entity_editor({x, y, EntitySpawn::Type::Piranha}, sprite_tex);
            }
            else if (current_action == DELETE) {
                level.delete_entity(x, y);
            }
        }
    }

    pressed_last_frame = pressed_this_frame;
}


/**
 * Renders(draw) BuilderUI on graphic. Accepts two Vector2 as parameters
 * 
 * @param top_left top left location of BuilderUI on graphic 
 * @param size size of BuilderUI on graphic 
 */
void BuilderUI::render(Vector2 top_left, Vector2 size) {
    DrawRectangle(1024 - 128, 0, 128, 1024, BLUE);
    DrawTexturePro(tile_tex, TileLocations::Ground, {1024 - 115, 0, 115, 115 }, {0,0}, 0, WHITE);
    DrawTexturePro(sprite_tex, SpriteLocations::Mushroom, { 1024 - 115, 115, 115, 115 }, { 0,0 }, 0, WHITE);
    DrawTexturePro(sprite_tex, SpriteLocations::Mushroom, { 1024 - 87, 259, 58, 58 }, { 0,0 }, 0, WHITE);
    DrawTexturePro(sprite_tex, SpriteLocations::FireFlower, { 1024 - 115, 345, 115, 115 }, { 0,0 }, 0, WHITE);
    DrawTexturePro(sprite_tex, SpriteLocations::TanookieLeafLeft, { 1024 - 115, 460, 115, 128 }, { 0,0 }, 0, WHITE);
    DrawTexturePro(sprite_tex, SpriteLocations::GoombaStep1, { 1024 - 115, 575, 115, 115 }, { 0,0 }, 0, WHITE);
    DrawTexturePro(sprite_tex, SpriteLocations::BooOpen, { 1024 - 115, 690, 115, 115 }, { 0,0 }, 0, WHITE);
    DrawTexturePro(sprite_tex, SpriteLocations::Piranha, { 1024 - 115, 805, 115, 115 }, { 0,0 }, 0, WHITE);
    DrawTexturePro(delete_tex, SpriteLocations::Delete, { 1024 - 115, 920, 115, 115 }, { 0,0 }, 0, WHITE);
}
