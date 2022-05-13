//
// Created by 16182 on 4/5/2022.
//

#ifndef MARIO_MAKER_MENU_H
#define MARIO_MAKER_MENU_H
#include<raylib.h>
#include<vector>
#include<string>
#include<functional>
class Menu {
public:
    Menu(std::function<void(std::string)> load_level, std::function<void()> make_level): load_level(load_level), make_level(make_level){}
    void open();
    void close();
    void render();
    void handle_events();
    
private:
    struct MenuItem {
        Rectangle rect;
        std::function<void()> on_click;
        std::string name;
        void render(){
            DrawRectangleRec(rect, GRAY);
            DrawText(name.c_str(), rect.x+10, rect.y+10, 20, BLACK);
        }
    };
    enum {
        CLOSED,
        TOP,
        MAKE,
        LOAD
    } current_mode = CLOSED;
    std::vector<std::string> level_files();
    std::vector<MenuItem> available_levels;


    std::vector<MenuItem> make_load_menu();
    std::function<void(std::string)> load_level;
    std::function<void()> make_level;
    static bool in_rect(Rectangle const& r, int x, int y);
    static constexpr Rectangle play_button_rect = {100, 100, 600, 200};
    static constexpr Rectangle make_button_rect = {100, 400, 600, 200};
};


#endif //MARIO_MAKER_MENU_H
