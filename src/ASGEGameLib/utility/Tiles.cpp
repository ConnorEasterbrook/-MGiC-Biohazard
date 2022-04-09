#include "utility/Tiles.h"

void Tiles::initTileInit(ASGE::Renderer* renderer)
{
  ASGE::FILEIO::File tile_map;
  if (tile_map.open("data/map/battle_map.tmx"))
  {
    ASGE::FILEIO::IOBuffer buffer = tile_map.read();
    std::string file_string(buffer.as_char(), buffer.length);

    map.loadFromString(file_string, ".");

    initTileMap(renderer);
  }

  std::cout << walkable_cc << " destructible blocks." << std::endl;
  std::cout << collidible_cc << " collidible blocks." << std::endl;
  std::cout << decoration_cc << " decoration blocks." << std::endl;
}

void Tiles::initTileMap(ASGE::Renderer* renderer)
{
  for (const auto& layer : map.getLayers())
  {
    if (layer->getType() == tmx::Layer::Type::Tile)
    {
      auto tile_layer = layer->getLayerAs<tmx::TileLayer>();
      int y           = 0;

      for (unsigned int row = 0; row < layer->getSize().y; row++)
      {
        int x = 0;

#pragma unroll(3)
        for (unsigned int col = 0; col < layer->getSize().x; col++)
        {
          auto tile_info   = tile_layer.getTiles()[row * tile_layer.getSize().x + col];
          const auto* tile = map.getTilesets()[0].getTile(tile_info.ID);

          initTiles(layer, tile, x, y, renderer);

          x += tile_size * tile_scale;
        }
        y += tile_size * tile_scale;
      }
    }
  }
}

void Tiles::initTiles(const auto& layer, const auto* tile, int x, int y, ASGE::Renderer* renderer)
{
  if (tile != nullptr)
  {
    if (layer->getName() == "Walkable")
    {
      walkable_cc++;

      auto& walk_tile = walkable_tile.emplace_back(renderer->createUniqueSprite());

      if (walk_tile->loadTexture(tile->imagePath))
      {
        tileSpriteSetup(tile, walk_tile, x, y);
      }
    }
    else if (layer->getName() == "Collidible")
    {
      collidible_cc++;

      auto& collide_tile = collidible_tile.emplace_back(renderer->createUniqueSprite());

      if (collide_tile->loadTexture(tile->imagePath))
      {
        tileSpriteSetup(tile, collide_tile, x, y);
      }
    }
    else
    {
      decoration_cc++;

      auto& dec_sprite = decoration_tiles.emplace_back(renderer->createUniqueSprite());

      if (dec_sprite->loadTexture(tile->imagePath))
      {
        tileSpriteSetup(tile, dec_sprite, x, y);
      }
    }
  }
}

void Tiles::tileSpriteSetup(const auto* tile, auto& sprite, int x, int y)
{
  sprite->srcRect()[0] = static_cast<float>(tile->imagePosition.x);
  sprite->srcRect()[1] = static_cast<float>(tile->imagePosition.y);
  sprite->srcRect()[2] = static_cast<float>(tile->imageSize.x);
  sprite->srcRect()[3] = static_cast<float>(tile->imageSize.y);

  sprite->width(static_cast<float>(tile->imageSize.x));
  sprite->height(static_cast<float>(tile->imageSize.y));
  sprite->scale(static_cast<float>(tile_scale));

  sprite->setMagFilter(ASGE::Texture2D::MagFilter::NEAREST);

  sprite->xPos(static_cast<float>(x));
  sprite->yPos(static_cast<float>(y));
}

void Tiles::renderTiles(ASGE::Renderer* renderer)
{
#pragma unroll(1)
  for (auto& decorations : decoration_tiles)
  {
    decorations->setGlobalZOrder(1);
    renderer->render(*decorations);
  }

#pragma unroll(1)
  for (auto& collidibles : collidible_tile)
  {
    collidibles->setGlobalZOrder(0);
    renderer->render(*collidibles);
  }

#pragma unroll(1)
  for (auto& walkable : walkable_tile)
  {
    walkable->setGlobalZOrder(-1);
    renderer->render(*walkable);
  }
}
