#include "PreCompile.h"
#include "TileMap.h"

TileMap::TileMap() 
{
}

TileMap::~TileMap() 
{
}

void TileMap::Start()
{
	TileRenderer = CreateComponent<GameEngineTileMap>();
}
