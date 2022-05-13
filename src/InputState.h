//
// Created by 16182 on 3/2/2022.
//

#ifndef MARIO_MAKER_INPUTSTATE_H
#define MARIO_MAKER_INPUTSTATE_H

/**
 * Struct called InputState. 
 * Each boolean variable called left, right, up, down, and space determines direction or jump. 
 * If boolean vairable is true, it refers respective direction key is pressed by user. 
 * The entity will be moved or jumped based on these boolean varaibles. 
 */
struct InputState {
    bool left, right, up, down, space, f;      
};

#endif //MARIO_MAKER_INPUTSTATE_H
