#pragma once
#include <Windows.h>
#include<vector>
using namespace std;


#define WIDTH 33
#define HEIGHT 25


enum MAP
{
	MAPWIDTH = 13,
	MAPHEIGHT = 13,

	MAP_NONE=-1,

	MAP_BLOCK ,
	MAP_BLOCKT,
	MAP_BLOCKL,
	MAP_BLOCKB,
	MAP_BLOCKR,

	MAP_ICE,

	MAP_FOREST,

	MAP_RIVER,

	MAP_GBLOCK,
	MAP_GBLOCKT,
	MAP_GBLOCKL,
	MAP_GBLOCKB,
	MAP_GBLOCKR,

	MAP_EAGLE,
	MAP_ENDFALGE,
	MAP_END

};

enum TANK
{
	TANK_START = 0,

	T_ENUMY_UP_0 = 0,
	T_ENUMY_UP_1,
	T_ENUMY_RED_UP,


	T_ENUMY_LT_0,
	T_ENUMY_LT_1,
	T_ENUMY_RED_LT,

	T_ENUMY_DN_0,
	T_ENUMY_DN_1,
	T_ENUMY_RED_DN,

	T_ENUMY_RT_0,
	T_ENUMY_RT_1,
	T_ENUMY_RED_RT,


	T_PLAYER_UP_0,
	T_PLAYER_UP_1,
	T_PLAYER_RT_0,
	T_PLAYER_RT_1,
	T_PLAYER_DN_0,
	T_PLAYER_DN_1,
	T_PLAYER_LT_0,
	T_PLAYER_LT_1,

	TANK_END
};

enum OBJECT
{
	OBJE_START=0,
	OBJE_ENEMY=0,
	OBJE_EXPLOSION00,
	OBJE_EXPLOSION01,
	OBJE_EXPLOSION02,
	OBJE_EXPLOSION03,
	OBJE_EXPLOSION04,
	OBJE_MISSILE,
	OBJE_PLAYER,
	OBJE_SHIELD00,
	OBJE_SHIELD01,
	OBJE_STAGE,
	OBJE_END
};

enum GAMESTATE
{
	GAMEWAIT,
	GAMESTART,
	GAMEEND,
};