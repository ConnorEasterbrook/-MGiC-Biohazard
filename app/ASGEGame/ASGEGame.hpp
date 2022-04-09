#pragma once

#include <ASGEGameLib/GCNetClient.hpp>
#include <ASGEGameLib/GCNetServer.hpp>
#include <ASGEGameLib/GComponent.hpp>
#include <ASGEGameLib/MyPacket.hpp>
#include <ASGEGameLib/gameObject/Buttons.hpp>
#include <ASGEGameLib/gameObject/GameObject.h>
#include <ASGEGameLib/gameObject/Player.h>
#include <ASGEGameLib/gameObject/battleUnits/BattleUnit.h>
#include <ASGEGameLib/utility/SpriteComponent.h>
#include <ASGEGameLib/utility/Tiles.h>
#include <ASGEGameLib/utility/Vector2.h>
#include <Engine/FileIO.hpp>
#include <Engine/Logger.hpp>
#include <Engine/OGLGame.hpp>
#include <tmxlite/Map.hpp>
#include <tmxlite/TileLayer.hpp>
#include <tmxlite/Tileset.hpp>
#include <vector>

class ASGENetGame : public ASGE::OGLGame
{
 public:
  explicit ASGENetGame(const ASGE::GameSettings& settings);
  ~ASGENetGame() override;

  ASGENetGame(const ASGENetGame&) = delete;
  ASGENetGame& operator=(const ASGENetGame&) = delete;

 private:
  void init();
  void initText();
  void initMenu();
  void initLobby();
  void initGame();
  void initCharacterSelection();
  void initBattle();
  void initUnits();
  void initUnitSprite();

  void keyHandler(ASGE::SharedEventData data);
  void keyHandlerMenu(const auto* key);
  void keyHandlerLobby(const auto* key);
  void keyHandlerGame(const auto* key);
  void keyHandlerGameplay(const auto* key);
  void keyHandlerBattleMap(const auto* key);
  void keyHandlerBattleMapUnits(const auto* key);
  void keyHandlerBattleMapCheats(const auto* key);
  void clickHandler(ASGE::SharedEventData data);
  void clickHandlerCharacterSelection(ASGE::SharedEventData data);
  void clickHandlerBattleMap(ASGE::SharedEventData data);
  void clickHandlerBattleMapAbilities(int i);

  void update(const ASGE::GameTime& us) override;
  void updateLobby();
  void updateLobbyChat();
  void updateGame(const ASGE::GameTime& us);
  void updateGameplay();
  void updateBattleMap(const ASGE::GameTime& us);

  void render(const ASGE::GameTime& us) override;
  void renderMenu();
  void renderLobby();
  void renderGame();
  void renderGameplay();
  void renderBattleMap();

  bool checkUnitRange(int i);
  void fixedUpdate(const ASGE::GameTime& us) override;

  void setupInitSprites(
    std::unique_ptr<ASGE::Sprite>& sprite, const std::string& texture_file_path, float width,
    float height, int16_t z_order, float x_pos, float y_pos, float scale);
  void
  setupTextRender(ASGE::Text& text, int16_t z_order, float x_pos, float y_pos, ASGE::Colour colour);

  std::vector<std::unique_ptr<GameComponent>> game_components;
  std::string key_callback_id{}; /**< Key Input Callback ID. */
  std::string mouse_callback_id{};
  GCNetClient* net_client;

  // GENERAL VARIABLES
  int font_index{};
  float game_window_width  = static_cast<float>(ASGE::SETTINGS.window_width);
  float game_window_height = static_cast<float>(ASGE::SETTINGS.window_height);

  enum GeneralState // Control general video game states
  {
    MENU  = 1,
    LOBBY = 2,
    GAME  = 3,
  };
  int general_state = GeneralState::MENU;

  // MENU VARIABLES
  ASGE::Camera menu_camera;
  std::unique_ptr<ASGE::Sprite> menu_background = nullptr;
  Buttons menu_buttons[3];
  int amount_menu_buttons            = 0;
  float padding_between_menu_buttons = 10;
  float top_of_screen_padding        = 250;

  int menu_option = 0; // Control menu states.
                       // No enum as an integer achieves the same thing with less code.

  // LOBBY VARIABLES
  std::unique_ptr<ASGE::Sprite> lobby_background = nullptr;
  std::unique_ptr<ASGE::Sprite> lobbyP1          = nullptr;
  std::unique_ptr<ASGE::Sprite> lobbyP2          = nullptr;
  std::string user_input;
  std::string old_user_input;
  ASGE::Text text_input{};
  ASGE::Text lobby_text;
  ASGE::Text chat_text;

  bool join_host        = false;
  int lobby_connections = 0;

  // CHARACTER SELECTION VARIABLES
  Buttons character_select_buttons[12];
  int amount_character_select_buttons = 0;
  GameObject character_select_pngs[6];
  int amount_character_select_pngs = 0;
  ASGE::Text amount_characters[6]{};

  std::unique_ptr<ASGE::Sprite> character_sniper = nullptr;
  std::unique_ptr<ASGE::Sprite> character_medic  = nullptr;
  std::unique_ptr<ASGE::Sprite> character_gunner = nullptr;

  // BATTLE MAP VARIABLES
  ASGE::Camera battle_camera;
  Player player;
  bool turn_over = false;
  BattleUnit units[8];
  int unit_count        = 8;
  int team_size         = 4;
  int unit_selected     = 0;
  int old_unit_selected = 0;
  int p1_unit_sniper    = 1;
  int p1_unit_medic     = 1;
  int p2_unit_sniper    = 1;
  int p2_unit_medic     = 1;
  ASGE::Text unit_health_text{};
  std::string unit_ap{};
  ASGE::Text unit_ap_text{};
  float game_time = static_cast<float>(0.1);
  GameObject ability_button;
  bool ability_mode = false;

  enum GameState // Control game states.
  {
    GAMEPLAY = 1,
    BATTLE   = 2,
  };
  int game_state = GameState::GAMEPLAY;

  // TILED VARIABLES
  Tiles tiles;
};
