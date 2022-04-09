#ifndef ASGENETGAME_TILES_H
#define ASGENETGAME_TILES_H

#include "iostream"
#include <Engine/FileIO.hpp>
#include <Engine/Renderer.hpp>
#include <Engine/Sprite.hpp>
#include <tmxlite/Map.hpp>
#include <tmxlite/TileLayer.hpp>
#include <tmxlite/Tileset.hpp>

class Tiles
{
 public:
  Tiles()  = default;
  ~Tiles() = default;

  void initTileInit(ASGE::Renderer* renderer);
  void initTileMap(ASGE::Renderer* renderer);
  void initTiles(const auto& layer, const auto* tile, int x, int y, ASGE::Renderer* renderer);
  void tileSpriteSetup(const auto* tile, auto& sprite, int x, int y);
  void renderTiles(ASGE::Renderer* renderer);

 private:
  tmx::Map map;

  std::unique_ptr<ASGE::Sprite> unit_sniper = nullptr;

  int tile_size     = 8;
  int tile_scale    = 3;
  int decoration_cc = 0;
  int collidible_cc = 0;
  int walkable_cc   = 0;
  std::vector<std::unique_ptr<ASGE::Sprite>> decoration_tiles;
  std::vector<std::unique_ptr<ASGE::Sprite>> collidible_tile;
  std::vector<std::unique_ptr<ASGE::Sprite>> walkable_tile;
};

#endif // ASGENETGAME_TILES_H
