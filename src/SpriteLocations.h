//
// Created by 16182 on 3/9/2022.
//

#ifndef MARIO_MAKER_SPRITELOCATIONS_H
#define MARIO_MAKER_SPRITELOCATIONS_H

#include<raymath.h>
namespace SpriteLocations {
    constexpr Rectangle MarioSmall = {259, 9, 12, 15};
    constexpr Rectangle MarioBig = {197, 48, 14, 27};
    constexpr Rectangle MarioFire = {200, 109, 14, 27};
    constexpr Rectangle MarioTanookie = {57, 421, 21, 29};
    constexpr Rectangle MarioTanookieRun = {81, 421, 22, 29};

    constexpr Rectangle BooClosed = {336, 865, 16, 16,};
    constexpr Rectangle BooOpen = {354, 865, 16, 16};
    constexpr Rectangle GoombaStep1 = {47, 724, 16, 16};
    constexpr Rectangle GoombaStep2 = {65, 724, 16, 16};
    constexpr Rectangle GoombaSquish = {83, 724, 16, 16};
    constexpr Rectangle Mushroom = {567, 995, 16, 16};
    constexpr Rectangle TanookieLeafRight = {513, 995, 16, 16};
    constexpr Rectangle TanookieLeafLeft = {531, 995, 16, 16};
    constexpr Rectangle Piranha = { 21.4, 834, 16, 32 };
    constexpr Rectangle FireFlower = { 459,995,16,16 };
    constexpr Rectangle FireBall = {498, 622, 12, 15};
    constexpr Rectangle Delete = {39, 39, 230, 230};
}

namespace TileLocations {
    constexpr Rectangle Ground = {137, 188, 16, 16};
}

#endif //MARIO_MAKER_SPRITELOCATIONS_H
