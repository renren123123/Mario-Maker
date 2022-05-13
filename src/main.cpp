#include <raylib.h>
#include "TileGrid.h"
#include "Mario.h"
#include "Level.h"
#include "Menu.h"
#include "BuilderUI.h"
#include <iostream>
#include <fstream>

Level load_level(std::string file_name, Texture tiles, Texture sprites){
    nlohmann::json level_json;
    std::ifstream(file_name) >> level_json;
    return Level::from_json(level_json, tiles, sprites);
}

int main(){
    // Initialization
    //--------------------------------------------------------------------------------------
    const int screenWidth = 1024;                                       //set width size of screen
    const int screenHeight = 1024;                                      //set height size of screen

    InitWindow(screenWidth, screenHeight, "Mario Maker");               //initializing game graphic window

    Image background = LoadImage("images/mario_background.png");        //set background image
    Texture2D background_texture = LoadTextureFromImage(background);    //load background image to turn into texture
    UnloadImage(background);                                            //unload background image after turn into texture

    Image mario_img = LoadImage("images/mario_sprites.png");                   //set regular mario sprite
    Texture2D sprite_texture = LoadTextureFromImage(mario_img);          //load regular mario sprite to turn into texture
    UnloadImage(mario_img);                                             //unload regular mario sprite after turn into texture

    Image tile_img = LoadImage("images/mario_sprites_2.png");           //set tile sprite
    Texture2D tile_texture = LoadTextureFromImage(tile_img);            //load tile sprite to turn into texture
    UnloadImage(tile_img);                                              //unload tile sprite after turn into texture

    Image end_img = LoadImage("images/dead_screen.png");        //set end_screen image
    Texture2D end_texture = LoadTextureFromImage(end_img);    //load end_screen image to turn into texture
    UnloadImage(end_img);

    Image delete_img = LoadImage("images/delete_button.png");        //set delete button
    Texture2D delete_tex = LoadTextureFromImage(delete_img);        //load delete button image to turn into texture
    UnloadImage(delete_img);

    SetTargetFPS(60);                                                   // Set our game to run at 60 frames-per-second

    bool in_menu = false;
    bool in_builder = false;
    bool last_enter_key = false;


    Level level = load_level("saved_level.json", tile_texture, sprite_texture);
//    Level level = Level(tile_texture, sprite_texture, 5, 5);
    BuilderUI builder_ui(level, sprite_texture, tile_texture, delete_tex);
    Menu menu{
        [&](auto file_name){ level = load_level(file_name, tile_texture, sprite_texture); in_builder = false;},
        [&](){in_builder = true; menu.close(); in_menu = false;}};

    // Main game loop
    while (!WindowShouldClose()){

        PollInputEvents();


        if(IsKeyDown(KEY_ENTER) != last_enter_key){
            last_enter_key = IsKeyDown(KEY_ENTER);

            if(IsKeyDown(KEY_ENTER)) {
                in_menu = !in_menu;
                if(in_menu){
                    menu.open();
                } else {
                    menu.close();
                }
            }
        }

        InputState input {
            .left = IsKeyDown(KEY_LEFT),
            .right = IsKeyDown(KEY_RIGHT),
            .up = IsKeyDown(KEY_UP),
            .down = IsKeyDown(KEY_DOWN),
            .space = IsKeyDown(KEY_SPACE),
            .f = IsKeyDown(KEY_F)
        };

//        level.mario().update(level.grid_, input);
        if(in_menu){
            menu.handle_events();
        } else if(in_builder){
            builder_ui.handle_events();
        } else {
            if (!level.mario().is_dead())
                level.update(input);
        }

        Camera2D cam{};
        cam.rotation = 0;
        cam.offset = Vector2Add(Vector2Multiply(level.get_camera_offset(), {-64.f, -64.f}), {512, 0});
        cam.target = {0,0};
        cam.zoom = 1.0;

        BeginDrawing();
        {
            ClearBackground(RAYWHITE);

            if (level.mario().is_dead())
                cam.offset = { 0, 0 };

            BeginMode2D(cam);
            {
                if (level.mario().is_dead())
                    DrawTextureTiled(end_texture, { 100, 0, 1500, 705 }, { 0, 0, 1024, 1024 }, { 0,0 }, 0, 1, WHITE);
                else {
                    // draw mountains
                    float paralax_mountains = level.mario().rect().x * -40;
                    float paralax_clouds = level.mario().rect().x * -53;

                    DrawTextureTiled(background_texture, { 0, 512, 1024,512 }, { -1024, 512, 6 * 1024,512 }, { paralax_mountains,0 }, 0, 1, WHITE);
                    DrawTextureTiled(background_texture, { 0, 0, 1024,512 }, { -1024, 0, 6 * 1024,512 }, { paralax_clouds,0 }, 0, 1, WHITE);
                    Vector2 top_left = { (float)0, (float)0 };
                    Vector2 bottom_right = { (float)screenWidth, (float)screenHeight };


                    level.render(top_left, bottom_right);
                }
            }
            EndMode2D();
            

        }
        if(in_builder){
            builder_ui.render({0,0},{screenWidth, screenHeight});
        }
        if(in_menu) {
            menu.render();
        };


        EndDrawing();

    }
    /*
    while (!WindowShouldClose()) {
        // dead screen
        BeginDrawing();
        {
            Camera2D cam{};
            cam.rotation = 0;
            cam.offset = {0, 0};
            cam.target = { 0,0 };
            cam.zoom = 1.0;

            ClearBackground(RAYWHITE);
            BeginMode2D(cam);
            {
                DrawTextureTiled(end_texture, { 0, 0, 512,512 }, { 512,512 }, { 0,0 }, 0, 1, WHITE);
            }
            EndMode2D();
        }
        EndDrawing();
    }*/

    std::ofstream("saved_level.json") << level.to_json();

    CloseWindow();                                                      // Close window and OpenGL context

    return 0;
}