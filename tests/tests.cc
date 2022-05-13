#include "catch.hpp"
#include <raylib.h>
#include <raymath.h>
#include <memory>
#include <iostream>
#include "../src/TileGrid.h"
#include "../src/Tile.h"
#include "../src/Mario.h"
#include "../src/enemies/Goomba.h"
#include "../src/enemies/Boo.h"
#include "../src/powerups/Mushroom.h"
#include "../src/powerups/SmallShroom.h"
#include "../src/Level.h"
#include "../src/SpriteLocations.h"
#include "../src/enemies/Piranha.h"


/**
 * This is a test for Basic Collision functionality between each entity in different case/direction. 
 */
TEST_CASE("Test Collision,[Collision]") {

	//Check when there is no collision between entity
	SECTION("Not Colliding") {
		Rectangle rect1{5, 5, 5,5};
		Rectangle rect2{10, 10, 5, 5};
		REQUIRE_FALSE(collide_rect(rect1, rect2).has_value());
	}

	//Check when if two different entity collides on left side of one entity 
	SECTION("Colliding left") {
		Rectangle rect1{5, 5, 5, 5};
		Rectangle rect2{4, 5, 5, 5};
		auto c = collide_rect(rect1, rect2);
		REQUIRE(collide_rect(rect1, rect2).has_value());			//check if rect1 and rect 2 
		REQUIRE(c->collision_side == Side::LEFT);					//check if rect1 and rect2 collides on left side
	}	

	//Check when if two different entity collides on right side of one entity 
	SECTION("Colliding right") {
		Rectangle rect1{5, 5, 5, 5};
		Rectangle rect2{6, 5, 5, 5};
		auto c = collide_rect(rect1, rect2);
		REQUIRE(collide_rect(rect1, rect2).has_value());			//check if rect1 and rect 2
		REQUIRE(c->collision_side == Side::RIGHT);					//check if rect1 and rect2 collides on right side
	}	

	//Check when if two different entity collides on up/top side of one entity
	SECTION("Colliding up") {
		Rectangle rect1{5, 5, 5, 5};
		Rectangle rect2{5, 4, 5, 5};
		auto c = collide_rect(rect1, rect2);
		REQUIRE(collide_rect(rect1, rect2).has_value());			//check if rect1 and rect 2
		REQUIRE(c->collision_side == Side::TOP);					//check if rect1 and rect2 collides on up/top side
	}

	//Check when if two different entity collides on down/bottom side of one entity
	SECTION("Colliding down") {
		Rectangle rect1{5, 5, 5, 5};
		Rectangle rect2{5, 6, 5, 5};
		auto c = collide_rect(rect1, rect2);
		REQUIRE(collide_rect(rect1, rect2).has_value());			//check if rect1 and rect 2
		REQUIRE(c->collision_side == Side::BOTTOM);					//check if rect1 and rect2 collides on down/bottom side
	}
}

/**
 * This is a test for Entity movement based on Key board input by User
 */
TEST_CASE("Movement") {

	Level level{ Texture{},Texture{},5,5 };
	Mario& mario = level.mario();

	//Check if mario entity can move in right direction 
	SECTION("Move Right") {
		Vector2 initialpos = mario.getPosition();
		InputState s = { false,true,false,false,false };
		level.update(s);

		Vector2 nextpos = mario.getPosition();

		REQUIRE(nextpos.x > initialpos.x);
	}

	//Check if mario entity can move in left direction
	SECTION("Move Left") {
		Vector2 initialpos = mario.getPosition();
		InputState s = { true,false,false,false,false };
		level.update(s);

		Vector2 nextpos = mario.getPosition();

		REQUIRE(nextpos.x < initialpos.x); 
	}


	//Check if mario entity can move up direction (jump)
	SECTION("Jumping,[going up]") {

		InputState s = { false,false,false,false,false };
		for (int i = 0; i < 100; i++) {
			level.update(s);
		}

		Vector2 initialpos = mario.getPosition();
		InputState s1 = { false,false,false,false,true };
		level.update(s1);

		Vector2 nextpos = mario.getPosition();

		REQUIRE(nextpos.y < initialpos.y); //check if y coordinate changed
	}

	//Check if mario entity can move down direction (after mario jumps)
	SECTION("Jumping,[going down]") {

		InputState s = { false,false,false,false,false };
		for (int i = 0; i < 100; i++) {
			level.update(s);
		}

		Vector2 initialpos = mario.getPosition();
		InputState s1 = { false,false,false,false,true };
		level.update(s1);

		InputState s2 = { false,false,false,false,false };
		for (int i = 0; i < 100; i++) {
			level.update(s2);
		}

		Vector2 nextpos = mario.getPosition();

		REQUIRE(nextpos.y == initialpos.y);
	}

}
/**
 * This is a test to check functionality of Goomba Entity
 */
TEST_CASE("Goomba") {

	//Check if Goomba entity can move in right direction 
	SECTION("Goomba moving right") {
		Level level{ Texture{},Texture{},30,8 };

		Tile t{true, TileLocations::Ground};

		for (int i = 0; i < 30; i++) {
			level.set_tile(i, 10, t);
		}

		EntitySpawn g(5, 9, EntitySpawn::Type::Goomba);
		Goomba* goomba = (Goomba*)(level.add_entity_editor(g, Texture{}));
		
		Vector2 initialpos = goomba->get_position();


		InputState s = { false,false,false,false,false };
		for (int i = 0; i < 5; i++) {
			level.update(s);
		}

		Vector2 finalPos = goomba->get_position();

		REQUIRE(finalPos.x > initialpos.x);
	}

	//Check if Goomba entity can move in left direction 
	SECTION("Goomba moving left after hitting block") {
		Level level{ Texture{},Texture{},30,8 };

		Tile t{true, TileLocations::Ground};

		for (int i = 0; i < 30; i++) {
			level.set_tile(i, 10, t);
		}

		EntitySpawn g(5, 10, EntitySpawn::Type::Goomba);
		Goomba* goomba = (Goomba*)(level.add_entity_editor(g, Texture{}));

		level.set_tile(8, 9, t);

		Vector2 initialpos = goomba->get_position();


		InputState s = { true,false,false,false,false };
		for (int i = 0; i < 100; i++) {
			level.update(s);
		}

		Vector2 finalPos = goomba->get_position();

		REQUIRE(finalPos.x < initialpos.x);


	}

	//Check if Goomba entity disappear if Mario steps on Goomba
	SECTION("Mario kills Goomba while stepping on top") {
		Level level{ Texture{},Texture{},5,8 };

		Tile t{true, TileLocations::Ground};

		for (int i = 0; i < 30; i++) {
			level.set_tile(i, 10, t);
		}

		level.set_tile(5, 12, t);
		EntitySpawn g(5, 10, EntitySpawn::Type::Goomba);
		Goomba* goomba = (Goomba*)(level.add_entity_editor(g, Texture{}));

		

		bool goomdead = goomba->is_goomba_squished();
		REQUIRE(goomdead == false);

		InputState s = { false,false,false,false,false };
		for (int i = 0; i < 100; i++) {
			level.update(s);
		}

		goomdead = goomba->is_goomba_squished();
		REQUIRE(goomdead);
	}

	//Check if Mario can get killed/reduced by Goombda
	SECTION("Goomba damages mario") {
		Level level{ Texture{},Texture{},10,8 };
		Mario& mario = level.mario();

		Tile t{true, TileLocations::Ground};

		for (int i = 0; i < 30; i++) {
			level.set_tile(i, 10, t);
		}


		EntitySpawn g(5, 10, EntitySpawn::Type::Goomba);
		Goomba* goomba = (Goomba*)(level.add_entity_editor(g, Texture{}));



		MarioPowerUp currmar = mario.get_PowerUp();
		REQUIRE(currmar == MarioPowerUp::Big);

		InputState s = { false,false,false,false,false };
		for (int i = 0; i < 100; i++) {
			level.update(s);
		}

		currmar = mario.get_PowerUp();
		REQUIRE(currmar == MarioPowerUp::SmallInv);
	}

}

/**
 * This is a test to check functionality of Boo Entity
 */
TEST_CASE("BOO") {

	//Check if Boo follows wherever Mario goes 
	SECTION("Boo moves towards mario") {
		Level level{ Texture{},Texture{},10,8 };


		Tile t{ true, TileLocations::Ground };


		for (int i = 0; i < 30; i++) {
			level.set_tile(i, 10, t);
		}

		EntitySpawn b(5, 0, EntitySpawn::Type::Boo);
		Boo* boo = (Boo*)(level.add_entity_editor(b, Texture{}));

		Vector2 initialpos = boo->get_position();


		InputState s = { false,true,false,false,false };
		level.update(s);
		s = { false,false,false,false,false };
		for (int i = 0; i < 100; i++) {
			level.update(s);
		}

		Vector2 finalpos = boo->get_position();

		REQUIRE(initialpos.x < finalpos.x);
		REQUIRE(initialpos.y < finalpos.y);
	}

	//Check if Mario can get killed/reduced by Boo
	SECTION("Boo Damages Mario") {
		Level level{ Texture{},Texture{},10,8 };
		Mario& mario = level.mario();

		Tile t{true, TileLocations::Ground};

		for (int i = 0; i < 30; i++) {
			level.set_tile(i, 10, t);
		}


		EntitySpawn b(5, 5, EntitySpawn::Type::Boo);
		Boo* boo = (Boo*)(level.add_entity_editor(b, Texture{}));


		MarioPowerUp currmar = mario.get_PowerUp();
		REQUIRE(currmar == MarioPowerUp::Big);

		InputState s = { false,true,false,false,false };
		level.update(s);
		level.update(s);
		s = { false,false,false,false,false };
		for (int i = 0; i < 200; i++) {
			level.update(s);
		}

		currmar = mario.get_PowerUp();
		REQUIRE(currmar == MarioPowerUp::SmallInv);
	}

}


/**
 * This is a test to check functionality of Small Mushroom Entity
 */
TEST_CASE("Small Mushroom") {

	//Check if Small Mushroom can move in right direction when respective key input is pressed by user 
	SECTION("Small Mushroom moving right") {
		Level level{ Texture{},Texture{},30,8 };

		Tile t{true, TileLocations::Ground};

		for (int i = 0; i < 30; i++) {
			level.set_tile(i, 10, t);
		}

		EntitySpawn g(5, 9, EntitySpawn::Type::SmallShroom);
		SmallShroom* smallshroom = (SmallShroom*)(level.add_entity_editor(g, Texture{}));

		Vector2 initialpos = smallshroom->getPosition();


		InputState s = { false,false,false,false,false };
		for (int i = 0; i < 5; i++) {
			level.update(s);
		}

		Vector2 finalPos = smallshroom->getPosition();

		REQUIRE(finalPos.x > initialpos.x);
	}

	//Check if small mushroom entity can move in opposite direction after hitting tile block 
	SECTION("Small mushroom left after hitting block") {
		Level level{ Texture{},Texture{},30,8 };

		Tile t{true, TileLocations::Ground};

		for (int i = 0; i < 30; i++) {
			level.set_tile(i, 10, t);
		}

		EntitySpawn g(5, 10, EntitySpawn::Type::SmallShroom);
		SmallShroom* smallshroom = (SmallShroom*)(level.add_entity_editor(g, Texture{}));

		level.set_tile(8, 9, t);

		Vector2 initialpos = smallshroom->getPosition();


		InputState s = { true,false,false,false,false };
		for (int i = 0; i < 100; i++) {
			level.update(s);
		}

		Vector2 finalPos = smallshroom->getPosition();

		REQUIRE(finalPos.x < initialpos.x);
	}


	//Check if Mario entity changes into smaller size after stepping on small mushroom entity 
	SECTION("Mario gets Small after stepping on small mushroom") {
		Level level{ Texture{},Texture{},5,8 };
		Mario& mario = level.mario();
		

		Tile t{true, TileLocations::Ground};

		for (int i = 0; i < 30; i++) {
			level.set_tile(i, 10, t);
		}

		MarioPowerUp currmar = mario.get_PowerUp();
		REQUIRE(currmar == MarioPowerUp::Big);

		level.set_tile(8, 9, t);
		EntitySpawn g(5, 6, EntitySpawn::Type::SmallShroom);
		SmallShroom* smallshroom = (SmallShroom*)(level.add_entity_editor(g, Texture{}));

		InputState s = { false,false,false,false,false };
		for (int i = 0; i < 100; i++) {
			level.update(s);
		}

		currmar = mario.get_PowerUp();
		REQUIRE(currmar == MarioPowerUp::Small);

	}
}


/**
 * This is a test to check functionality of Mushroom Entity
 */
TEST_CASE("Mushroom") {

	////Check if Mushroom can move in right direction when respective key input is pressed by user 
	SECTION("Mushroom moving right") {
		Level level{ Texture{},Texture{},30,8 };

		Tile t{true, TileLocations::Ground};

		for (int i = 0; i < 30; i++) {
			level.set_tile(i, 10, t);
		}

		EntitySpawn g(5, 9, EntitySpawn::Type::Mushroom);
		Mushroom* mush = (Mushroom*)(level.add_entity_editor(g, Texture{}));

		Vector2 initialpos = mush->getPosition();


		InputState s = { false,false,false,false,false };
		for (int i = 0; i < 5; i++) {
			level.update(s);
		}

		Vector2 finalPos = mush->getPosition();

		REQUIRE(finalPos.x > initialpos.x);
	}

	//Check if Mushroom entity can move in opposite direction after hitting tile block 
	SECTION("Mushroom left after hitting block") {
		Level level{ Texture{},Texture{},30,8 };

		Tile t{true, TileLocations::Ground};

		for (int i = 0; i < 30; i++) {
			level.set_tile(i, 10, t);
		}

		EntitySpawn g(5, 10, EntitySpawn::Type::Mushroom);
		Mushroom* mush = (Mushroom*)(level.add_entity_editor(g, Texture{}));

		level.set_tile(8, 9, t);

		Vector2 initialpos = mush->getPosition();


		InputState s = { true,false,false,false,false };
		for (int i = 0; i < 100; i++) {
			level.update(s);
		}

		Vector2 finalPos = mush->getPosition();

		REQUIRE(finalPos.x < initialpos.x);
	}

	//Check if Mario entity changes into bigger size after stepping on small mushroom entity	
	SECTION("Mario gets big after stepping on mushroom") {
		Level level{ Texture{},Texture{},5,8 };
		Mario& mario = level.mario();


		Tile t{true, TileLocations::Ground};

		for (int i = 0; i < 30; i++) {
			level.set_tile(i, 10, t);
		}

		level.set_tile(8, 9, t);

		MarioPowerUp currmar = mario.get_PowerUp();
		REQUIRE(currmar == MarioPowerUp::Big);

		EntitySpawn g(5, 6, EntitySpawn::Type::SmallShroom);
		SmallShroom* smallshroom = (SmallShroom*)(level.add_entity_editor(g, Texture{}));

		InputState s = { false,false,false,false,false };
		for (int i = 0; i < 100; i++) {
			level.update(s);
		}

		currmar = mario.get_PowerUp();
		REQUIRE(currmar == MarioPowerUp::Small);

		EntitySpawn g1(5, 6, EntitySpawn::Type::Mushroom);
		Mushroom* mush = (Mushroom*)(level.add_entity_editor(g1, Texture{}));

		for (int i = 0; i < 100; i++) {
			level.update(s);
		}

		currmar = mario.get_PowerUp();
		REQUIRE(currmar == MarioPowerUp::Big);

	}

}


/**
 * This is a test to check functionality of FireFlower Entity
 */
TEST_CASE("FireFlower") {

	//Check if Mario tunrs into Fire-Mario when it hits FireFloewr block
	SECTION("Mario gets a FireFlower powerup walking on the fireflower") {
		Level level{ Texture{},Texture{},5,8 };
		Mario& mario = level.mario();

		Tile t{true, TileLocations::Ground};

		for (int i = 0; i < 30; i++) {
			level.set_tile(i, 10, t);
		}

		EntitySpawn g(10, 9 ,EntitySpawn::Type::FireFlower);
		Entity* fire = (Entity*)(level.add_entity_editor(g, Texture{}));



		InputState s = { false,true,false,false,false };
		for (int i = 0; i < 100; i++) {
			level.update(s);
		}


		MarioPowerUp currmar = mario.get_PowerUp();
		REQUIRE(currmar == MarioPowerUp::Fire);
	}

}

/**
 * This is a test to check functionality of Tanookie Entity
 */
TEST_CASE("Tanookie Leaf") {

	//Check if mario sprite changes(mario changes power up status to Tanookie) after it collides with Tanookie
	SECTION("Mario gets a Tanookie powerup walking on the Tanookie Leaf") {
		Level level{ Texture{},Texture{},5,8 };
		Mario& mario = level.mario();

		Tile t{ true, TileLocations::Ground };

		for (int i = 0; i < 30; i++) {
			level.set_tile(i, 10, t);
		}

		EntitySpawn g(10, 9, EntitySpawn::Type::TanookieLeaf);
		Entity* fire = (Entity*)(level.add_entity_editor(g, Texture{}));



		InputState s = { false,true,false,false,false };
		for (int i = 0; i < 100; i++) {
			level.update(s);
		}


		MarioPowerUp currmar = mario.get_PowerUp();
		REQUIRE(currmar == MarioPowerUp::Tanookie);
	}
}

/**
 * This is a test to check functionality of Piranha plant Entity
 */
TEST_CASE("Piranha Plant") {

	//Check if Piranha plants appears and disappaers alternatively uniformly
	SECTION("Piranha plant appears and disappears") {
		Level level{ Texture{},Texture{},30,8 };

		Tile t{ true, TileLocations::Ground };

		for (int i = 0; i < 30; i++) {
			level.set_tile(i, 10, t);
		}

		EntitySpawn g(5, 10, EntitySpawn::Type::Piranha);
		Piranha* piranha = (Piranha*)(level.add_entity_editor(g, Texture{}));

		int active = piranha->get_Activeplant();
		int dormant = piranha->get_Dormanplant();

		REQUIRE(active == 0);
		REQUIRE(dormant == 0);

		InputState s = { true,false,false,false,false };
		for (int i = 0; i < 100; i++) {
			level.update(s);
		}

		active = piranha->get_Activeplant();
		dormant = piranha->get_Dormanplant();

		REQUIRE(active > 0);
		REQUIRE(dormant == 0);

		for (int i = 0; i < 25; i++) {
			level.update(s);
		}

		active = piranha->get_Activeplant();
		dormant = piranha->get_Dormanplant();

		REQUIRE(active == 0);
		REQUIRE(dormant > 0);

	}

	//Check if Mario entity dies or changes powerup status after collides with Piranha entity 
	SECTION("Mario Takes Damage Stepping on Piranha") {
		Level level{ Texture{},Texture{},10,8 };
		Mario& mario = level.mario();

		Tile t{ true, TileLocations::Ground };

		for (int i = 0; i < 30; i++) {
			level.set_tile(i, 10, t);
		}

		EntitySpawn g(5, 9, EntitySpawn::Type::Piranha);
		Piranha* piranha = (Piranha*)(level.add_entity_editor(g, Texture{}));

		MarioPowerUp currmar = mario.get_PowerUp();
		REQUIRE(currmar == MarioPowerUp::Big);

		InputState s = { true,false,false,false,false };
		for (int i = 0; i < 100; i++) {
			level.update(s);
		}

		currmar = mario.get_PowerUp();
		REQUIRE(currmar == MarioPowerUp::SmallInv);
	}
}