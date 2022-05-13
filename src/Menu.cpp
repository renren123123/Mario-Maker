//
// Created by 16182 on 4/5/2022.
//

#include "Menu.h"
#include<filesystem>
#include<iostream>

/**
 * Getting level that is stored in json type and converted them into vector of string. 
 * 
 * @return ret vector of string type of level files
 */
std::vector<std::string> Menu::level_files() {
    std::vector<std::string> ret;
    for(auto & f : std::filesystem::directory_iterator{"."}){
        if(f.path().extension() == ".json"){
            ret.push_back(f.path().string());
        }
    }
    return ret;
}

/**
 * Loading menu item into vector
 * 
 * @return ret vector of menu item 
 */
std::vector<Menu::MenuItem> Menu::make_load_menu() {
    std::vector<Menu::MenuItem> ret;
    auto files = level_files();
    for(int i = 0; i < files.size(); i++){
        ret.push_back({.rect={100, 100.f+100*i, 200, 50}, .on_click=[=](){load_level(files[i]);}, .name=files[i]});
    }
    return ret;
}


/**
 * Let user open Menu
 */
void Menu::open(){
    if(current_mode == CLOSED){
        current_mode = TOP;
    }
}

/**
 * Let user close Menu
 */
void Menu::close(){
    current_mode = CLOSED;
}

/**
 * Renders menu item. Menu item inclused play button and make button.
 */
void Menu::render(){
    switch(current_mode){
        case TOP:
            DrawRectangleRec(play_button_rect, GRAY);
            DrawText("play level", play_button_rect.x+50, play_button_rect.y+50, 50, BLACK);
            DrawRectangleRec(make_button_rect, GRAY);
            DrawText("make level", make_button_rect.x+50, make_button_rect.y+50, 50, BLACK);
            break;
        case CLOSED:
            break;
        case LOAD:
            for(auto& item : available_levels){
                item.render();
            }
            break;
        case MAKE:
            break;
    }
}

/**
 * Handles events depends on what user does. 
 */
void Menu::handle_events(){
    int mx = GetMouseX();
    int my = GetMouseY();
    switch(current_mode){
        case TOP:
            if(IsMouseButtonDown(MOUSE_BUTTON_LEFT)){
                if(in_rect(play_button_rect, mx, my)){
                    available_levels = make_load_menu();
                    current_mode = LOAD;
                } else if(in_rect(make_button_rect, mx, my)){
                    make_level();
                }
            }
            break;
        case CLOSED:
            break;
        case LOAD:
            for(auto& item : available_levels){
                if(IsMouseButtonDown(MOUSE_BUTTON_LEFT) && in_rect(item.rect, mx, my)){
                    item.on_click();
                }
            }
            break;
        case MAKE:
            break;
    }

}

/**
 * Check if location of Rectangle is in bound
 * 
 * @return true if it is in bound else false
 */
bool Menu::in_rect(Rectangle const&r, int x, int y){
    return x >= r.x && x < r.x + r.width && y >= r.y && y < r.y + r.height;
}