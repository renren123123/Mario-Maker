//
// Created by 16182 on 3/30/2022.
//

#include "EntitySpawn.h"
#include "enemies/Boo.h"
#include "enemies/Goomba.h"
#include "enemies/Piranha.h"
#include "powerups/FireFlower.h"
#include "powerups/Mushroom.h"
#include "powerups/SmallShroom.h"
#include "powerups/TanookieLeaf.h"
#include "FireBall.h"
#include "Mario.h"
#include <memory>

/** 
 * Spawn each entity respect to given parameter. 
 * For unknwon entity type, throw runtime error. 
 * 
 * @param tex rendered entity image sprite 
 * @param mario Pointer to Mario entity 
 */

std::unique_ptr<Entity> EntitySpawn::make(Texture tex, Mario* mario) {
    switch(type){
        case Type::Boo:
            return std::make_unique<Boo>(x, y, tex, mario);
        case Type::Goomba:
            return std::make_unique<Goomba>(x, y, tex);
        case Type::Piranha:
            return std::make_unique<Piranha>(x, y, tex);
        case Type::FireFlower:
            return std::make_unique<FireFlower>(x, y, tex);
        case Type::Mushroom:
            return std::make_unique<Mushroom>(x, y, tex);
        case Type::SmallShroom:
            return std::make_unique<SmallShroom>(x, y, tex);
        case Type::TanookieLeaf:
            return std::make_unique<TanookieLeaf>(x, y, tex);
        case Type::FireBall:
            return std::make_unique<FireBall>(x, y, tex, mario->is_right());
        default:
            throw std::runtime_error("unhandeled entity type id");
    }
}

/** 
 * Converting float x, float y and Entity type to json to load/save level.  
 */

nlohmann::json EntitySpawn::to_json() {
    nlohmann::json json;
    json["x"] = x;
    json["y"] = y;
    json["type"] = type;
    return json;
}

/** 
 * Get json object respect to given parameter in order to load/save level.
 * @param json reference type of json object 
 */
EntitySpawn EntitySpawn::from_json(const nlohmann::json &json) {
    return {json["x"], json["y"], json["type"]};
}
