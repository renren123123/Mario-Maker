//
// Created by 16182 on 2/23/2022.
//

#ifndef MARIO_MAKER_TILEGRID_H
#define MARIO_MAKER_TILEGRID_H

#include<raylib.h>
#include<array>
#include<random>
#include<optional>
#include<nlohmann/json.hpp>
#include"Tile.h"
#include"SpriteLocations.h"

class TileGrid {
    public:
        TileGrid(Texture texture, size_t width, size_t height) {
            resize(width, height);
            tex = texture;
        };

        void render(Vector2 top_left, Vector2 bottom_right) const;
        Tile at(int x, int y) const;
        Tile & at_mut(int x, int y);
        TileCollisionSet collide(Rectangle rect) const;
        nlohmann::json to_json() const;
        static TileGrid from_json(const nlohmann::json& json, Texture tex);

    private:
        TileGrid(Texture texture, size_t width, size_t height, const nlohmann::json& rows);
        void resize(size_t width, size_t height);
        static std::optional<Collision> collide_grid(Rectangle rect, int x, int y) ;
        size_t width;
        size_t height;
        std::vector<Tile> tiles{};
        Texture tex;
};


#endif //MARIO_MAKER_TILEGRID_H
