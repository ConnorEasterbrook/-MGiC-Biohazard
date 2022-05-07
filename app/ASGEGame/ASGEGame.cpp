#include "ASGEGame.hpp"

/// Initialises the game.
/// Setup your game and initialise the core components.
/// @param settings
ASGENetGame::ASGENetGame(const ASGE::GameSettings &settings) : OGLGame(settings)
{
    key_callback_id = inputs->addCallbackFnc(ASGE::E_KEY, &ASGENetGame::keyHandler, this);
    mouse_callback_id = inputs->addCallbackFnc(ASGE::E_MOUSE_CLICK, &ASGENetGame::clickHandler, this);

    inputs->use_threads = false;
    toggleFPS();

    auto client = std::make_unique<GCNetClient>();
    net_client = client.get();
    client->connect("127.0.0.1", 31276);
    game_components.emplace_back(std::move(client));

    init();
}

/// Destroys the game.
ASGENetGame::~ASGENetGame()
{
    this->inputs->unregisterCallback(key_callback_id);
    this->inputs->unregisterCallback(mouse_callback_id);
}

void ASGENetGame::init()
{
    initText();
    initMenu();
    initLobby();
    initGame();
}

void ASGENetGame::initText()
{
    ASGE::FILEIO::File font_file;
    if (font_file.open("data/text/HypotheticalmoonofMercury.ttf"))
    {
        auto buffer = font_file.read();

        font_index = renderer->loadFontFromMem(
                "Attack", buffer.as_unsigned_char(), static_cast<unsigned int>(buffer.length), 26);
        renderer->setFont(font_index);
        font_file.close();
    }
    chat_text.setString("Chatbox /n");
}

void ASGENetGame::initMenu()
{
    // Init Camera
    menu_camera =
            ASGE::Camera{static_cast<float>(game_window_width), static_cast<float>(game_window_height)};

    std::cout << "INIT CAMERA" << std::endl;
    menu_camera.lookAt(ASGE::Point2D{static_cast<float>(game_window_width) / 2.F,
                                     static_cast<float>(game_window_height) / 2.F});

    setupInitSprites(menu_background, "data/images/mainMenu/background.png", 1280, 720, -2, 0, 0, 1);

    amount_menu_buttons = static_cast<int>(std::size(menu_buttons));

    for (int i = 0; i < amount_menu_buttons; i++)
    {
        if (i < 1)
        {
            menu_buttons[i].addSpriteComponent(
                    "data/images/buttons/Start_Button.png",
                    std::make_unique<SpriteComponent>(),
                    renderer.get(),
                    1);
        }
        else if (i < 2)
        {
            menu_buttons[i].addSpriteComponent(
                    "data/images/buttons/Join_Button.png",
                    std::make_unique<SpriteComponent>(),
                    renderer.get(),
                    1);
        }
        else if (i < 3)
        {
            menu_buttons[i].addSpriteComponent(
                    "data/images/buttons/Exit_Button.png",
                    std::make_unique<SpriteComponent>(),
                    renderer.get(),
                    1);
        }
        menu_buttons[i].sprite()->getSprite()->xPos(
                (game_window_width / 2) -
                ((menu_buttons->getSpriteSheetWidth() * menu_buttons->sprite()->getSprite()->scale()) / 2));
        menu_buttons[i].sprite()->getSprite()->yPos(
                static_cast<float>(i) *
                        (menu_buttons->getSpriteSheetHeight() * menu_buttons->sprite()->getSprite()->scale() +
                         padding_between_menu_buttons) +
                top_of_screen_padding);
    }
}

void ASGENetGame::initLobby()
{
    setupInitSprites(lobby_background, "data/images/lobby/lobbybg.png", 1280, 720, -2, 0, 0, 1);
    setupInitSprites(lobbyP1, "data/images/lobby/p1lobby.png", 600, 200, -1, 50, 150, 1);
    setupInitSprites(lobbyP2, "data/images/lobby/p2lobby.png", 600, 200, -1, 50, 400, 1);
}

void ASGENetGame::initGame()
{
    tiles.initTileInit(renderer.get());

    initCharacterSelection();
    initBattle();
}

void ASGENetGame::initCharacterSelection()
{
    initCharacterSelectionButtons();
    initCharacterSelectionPngs();
    initCharacterSelectionText();
}

void ASGENetGame::initCharacterSelectionButtons()
{
    amount_character_select_buttons = static_cast<int>(std::size(character_select_buttons));
    float half_button_width = (Buttons().getSpriteSheetWidth() * Buttons().getSpriteScaler()) / 2;
    float half_button_height = (Buttons().getSpriteSheetHeight() * Buttons().getSpriteScaler() / 2);

    character_select_play.setSpriteSheetWidth(114);
    character_select_play.addSpriteComponent(
            "data/images/buttons/Play_Button.png", std::make_unique<SpriteComponent>(), renderer.get(), 1);
    character_select_play.sprite()->getSprite()->xPos(
            game_window_width / 2 -
            ((character_select_play.getSpriteSheetWidth() * character_select_play.getSpriteScaler()) / 2));
    character_select_play.sprite()->getSprite()->yPos(game_window_height / 2 - half_button_height);
    character_select_play.setIsActive(true);

    for (int i = 0; i < amount_character_select_buttons; i++)
    {
        character_select_buttons[i].setIsActive(true);
        if (i < amount_character_select_buttons / 2)
        {
            character_select_buttons[i].addSpriteComponent(
                    "data/images/buttons/Plus_Button.png",
                    std::make_unique<SpriteComponent>(),
                    renderer.get(),
                    1);
            if (i < amount_character_select_buttons / 4)
            {
                character_select_buttons[i].sprite()->getSprite()->xPos(
                        (static_cast<float>(i) * (game_window_width / 4)) + (game_window_width / 4) -
                        half_button_width);
                character_select_buttons[i].sprite()->getSprite()->yPos(half_button_height);
            }
            else
            {
                character_select_buttons[i].sprite()->getSprite()->xPos(
                        (static_cast<float>(i - amount_character_select_buttons / 4) * (game_window_width / 4)) +
                        (game_window_width / 4) - half_button_width);
                character_select_buttons[i].sprite()->getSprite()->yPos(
                        (game_window_height / 2) +
                        (Buttons().getSpriteSheetHeight() * Buttons().getSpriteScaler()));
            }
        }
        else
        {
            character_select_buttons[i].addSpriteComponent(
                    "data/images/buttons/Minus_Button.png",
                    std::make_unique<SpriteComponent>(),
                    renderer.get(),
                    1);
            if (i < (amount_character_select_buttons * 3 / 4))
            {
                character_select_buttons[i].sprite()->getSprite()->xPos(
                        (static_cast<float>(i - amount_character_select_buttons / 2) * (game_window_width / 4)) +
                        (game_window_width / 4) - half_button_width);
                character_select_buttons[i].sprite()->getSprite()->yPos(
                        (game_window_height / 2) -
                        (Buttons().getSpriteSheetHeight() * Buttons().getSpriteScaler() * 2));
            }
            else
            {
                character_select_buttons[i].sprite()->getSprite()->xPos(
                        (static_cast<float>(i - (amount_character_select_buttons * 3 / 4)) *
                         (game_window_width / 4)) +
                        (game_window_width / 4) - half_button_width);
                character_select_buttons[i].sprite()->getSprite()->yPos(
                        game_window_height - (3 * half_button_height));
            }
        }
    }
}

void ASGENetGame::initCharacterSelectionPngs()
{
    amount_character_select_pngs = static_cast<int>(std::size(character_select_pngs));

    for (int i = 0; i < amount_character_select_pngs; i++)
    {
        if (i < 2)
        {
            character_select_pngs[i].addSpriteComponent(
                    "data/images/characterSelection/Skeleton_Character.png",
                    std::make_unique<SpriteComponent>(),
                    renderer.get(),
                    1);
            character_select_pngs[i].sprite()->getSprite()->scale(6);
            character_select_pngs[i].sprite()->getSprite()->xPos(
                    (game_window_width / 4) - (((character_select_pngs->sprite()->getSprite()->width()) *
                                                character_select_pngs[i].sprite()->getSprite()->scale()) /
                                               2));
        }
        else if (i < 4)
        {
            character_select_pngs[i].addSpriteComponent(
                    "data/images/characterSelection/Pig_Character.png",
                    std::make_unique<SpriteComponent>(),
                    renderer.get(),
                    1);
            character_select_pngs[i].sprite()->getSprite()->scale(6);
            character_select_pngs[i].sprite()->getSprite()->xPos(
                    (game_window_width / 2) - (((character_select_pngs->sprite()->getSprite()->width()) *
                                                character_select_pngs[i].sprite()->getSprite()->scale()) /
                                               2));
        }
        else
        {
            character_select_pngs[i].addSpriteComponent(
                    "data/images/characterSelection/Orc_Character.png",
                    std::make_unique<SpriteComponent>(),
                    renderer.get(),
                    1);
            character_select_pngs[i].sprite()->getSprite()->scale(6);
            character_select_pngs[i].sprite()->getSprite()->xPos(
                    (game_window_width * 3 / 4) - (((character_select_pngs->sprite()->getSprite()->width()) *
                                                    character_select_pngs[i].sprite()->getSprite()->scale()) /
                                                   2));
        }

        character_select_pngs[i].sprite()->getSprite()->setMagFilter(
                ASGE::GameSettings::MagFilter::NEAREST);

        if (i % 2 == 0)
        {
            character_select_pngs[i].sprite()->getSprite()->yPos(
                    (game_window_height / 4) - ((character_select_pngs->sprite()->getSprite()->height() *
                                                 character_select_pngs[i].sprite()->getSprite()->scale())) *
                                                       2 / 3);
        }
        else
        {
            character_select_pngs[i].sprite()->getSprite()->yPos(
                    (game_window_height * 3 / 4) - (character_select_pngs->sprite()->getSprite()->height() *
                                                    character_select_pngs[i].sprite()->getSprite()->scale()) /
                                                           3);
        }
    }
}

void ASGENetGame::initCharacterSelectionText()
{
    for (int i = 0; i < amount_character_select_pngs; i++)
    {
        setupTextRender(
                amount_characters[i],
                15,
                character_select_pngs[i].sprite()->getX() - 20,
                character_select_pngs[i].sprite()->getY() +
                        (character_select_pngs[i].sprite()->getSprite()->height() *
                         character_select_pngs[i].sprite()->getSprite()->scale() / 2),
                ASGE::COLOURS::WHITE);
        amount_characters[i].setFont(renderer->getFont(font_index));
        amount_characters[i].setScale(2);
    }
}

void ASGENetGame::initBattle()
{
    // Init Camera
    battle_camera =
            ASGE::Camera{static_cast<float>(game_window_width), static_cast<float>(game_window_height)};

    std::cout << "INIT CAMERA" << std::endl;
    battle_camera.lookAt(ASGE::Point2D{static_cast<float>(game_window_width) / 2.F,
                                       static_cast<float>(game_window_height) / 2.F});

    // Init Player
    player.addSpriteComponent(
            "data/images/battleMap/crosshair.png", std::make_unique<SpriteComponent>(), renderer.get(), 10);

    if (player.sprite()->getSprite() != nullptr)
    {
        player.sprite()->setDimensions(50, 50, 2);
        player.sprite()->setBounds(0, 0);
    }

    player.setPlayerSpawn(768, 432);
    player.teleportPlayerSpawn();

    initUnits();
}

void ASGENetGame::initUnits()
{
    initUnitSprite();

    for (int i = 0; i < unit_count; i++)
    {
        units[i].initTexture2D(renderer.get());
        units[i].setUnitDetails();

        setupTextRender(
                unit_health_text,
                15,
                units[i].sprite()->getX() + 12,
                units[i].sprite()->getY() - 24,
                ASGE::COLOURS::WHITE);

        unit_ap_text.setScale(2);
    }

    ability_button.addSpriteComponent(
            "data/images/battleMap/ability_button.png",
            std::make_unique<SpriteComponent>(),
            renderer.get(),
            10);

    if (ability_button.sprite()->getSprite() != nullptr)
    {
        ability_button.sprite()->setDimensions(16, 16, 5);
        ability_button.sprite()->setBounds(0, 0);
    }

    ability_button.sprite()->setX(player.sprite()->getX() + 600);
    ability_button.sprite()->setY(player.sprite()->getY() + 300);
    ability_button.visibility = true;
}

void ASGENetGame::initUnitSprite()
{
    float add_x = 0;
    float add_y = 0;

    for (int i = 0; i < unit_count; i++)
    {
        if (units[i].getUnitType() == 1)
        {
            units[i].addSpriteComponent(
                    "data/images/battleMap/units/skeleton.png",
                    std::make_unique<SpriteComponent>(),
                    renderer.get(),
                    10);
        }
        else if (units[i].getUnitType() == 2)
        {
            units[i].addSpriteComponent(
                    "data/images/battleMap/units/pig.png",
                    std::make_unique<SpriteComponent>(),
                    renderer.get(),
                    10);
        }
        else
        {
            units[i].addSpriteComponent(
                    "data/images/battleMap/units/orc.png",
                    std::make_unique<SpriteComponent>(),
                    renderer.get(),
                    10);
        }

        if (units[i].sprite()->getSprite() != nullptr)
        {
            units[i].sprite()->setDimensions(16, 16, 2);
            units[i].sprite()->setBounds(0, 24);

            if (i < 4)
            {
                units[i].sprite()->setY(
                        (696 -
                         (units[i].sprite()->getSprite()->height() * units[i].sprite()->getSprite()->scale())) +
                        add_y);
                units[i].sprite()->setX(
                        (384 -
                         (units[i].sprite()->getSprite()->width() * units[i].sprite()->getSprite()->scale())) +
                        add_x);
                units[i].setCoords(units[i].sprite()->getX(), units[i].sprite()->getY());

                units[i].team_number = 1;
            }
            else
            {
                units[i].sprite()->setY(
                        (96 -
                         (units[i].sprite()->getSprite()->height() * units[i].sprite()->getSprite()->scale())) +
                        add_y);
                units[i].sprite()->setX(
                        (1176 -
                         (units[i].sprite()->getSprite()->width() * units[i].sprite()->getSprite()->scale())) +
                        add_x);
                units[i].setCoords(units[i].sprite()->getX(), units[i].sprite()->getY());

                units[i].team_number = 2;
            }

            if (i == 0 || i == 4)
            {
                add_x += 24;
            }
            else if (i == 1 || i == 5)
            {
                add_y += 24;
            }
            else if (i == 2 || i == 6)
            {
                add_x -= 24;
            }
        }

        units[i].visibility = true;
    }
}

/// Processes key inputs.
/// This function is added as a callback to handle the game's
/// keyboard input. For this game, calls to this function
/// are not thread safe, so you may alter the game's state
/// but your code needs to designed to prevent data-races.
/// @param data
/// @see KeyEvent
void ASGENetGame::keyHandler(ASGE::SharedEventData data)
{
    const auto *key = dynamic_cast<const ASGE::KeyEvent *>(data.get());

    if (key->key == ASGE::KEYS::KEY_ESCAPE)
    {
        signalExit();
    }

    if (general_state == MENU)
    {
        keyHandlerMenu(key);
    }
    else if (general_state == LOBBY)
    {
        keyHandlerLobby(key);
    }
    else if (general_state == GAME)
    {
        keyHandlerGame(key);
    }
}

void ASGENetGame::keyHandlerMenu(const auto *key)
{
    if (key->key == ASGE::KEYS::KEY_ENTER && key->action == ASGE::KEYS::KEY_RELEASED)
    {
        if (menu_option == 0)
        {
            general_state = GeneralState::GAME;
        }
        else if (menu_option == 1)
        {
            lobby_connections = lobby_connections + 1;
            multiplayer = true;
            player2 = net_client->player2;
            if (!player2)
            {
                player1 = true;
            }
            sendInitPacket();

            general_state = GeneralState::LOBBY;

            sendIntPacket(lobby_connections);
        }
        else if (menu_option == 2)
        {
            signalExit();
        }
    }

    if (key->key == ASGE::KEYS::KEY_DOWN && key->action == ASGE::KEYS::KEY_RELEASED)
    {
        if (menu_option >= 2)
        {
            menu_option = 0;
        }
        else
        {
            menu_option++;
        }
    }
    if (key->key == ASGE::KEYS::KEY_UP && key->action == ASGE::KEYS::KEY_RELEASED)
    {
        if (menu_option <= 0)
        {
            menu_option = 2;
        }
        else
        {
            menu_option--;
        }
    }
}

void ASGENetGame::keyHandlerLobby(const auto *key)
{
    if (general_state == GeneralState::LOBBY)
    {
        if (key->key == ASGE::KEYS::KEY_1 && key->action == ASGE::KEYS::KEY_RELEASED)
        {
            if (player1 && lobby_connections == 2)
            {
                general_state = GeneralState::GAME;
                sendGeneralStatePacket(2);
            }
        }

        if (key->key >= 32 && key->key <= 90 && key->action == ASGE::KEYS::KEY_RELEASED)
        {
            user_input += static_cast<char>(key->key);
        }

        if (key->key == ASGE::KEYS::KEY_BACKSPACE && key->action == ASGE::KEYS::KEY_RELEASED)
        {
            if (user_input.length() != 0)
            {
                user_input.erase(user_input.length() - 1);
            }
        }

        if (key->key == ASGE::KEYS::KEY_ENTER && key->action == ASGE::KEYS::KEY_RELEASED)
        {
            std::cout << user_input << std::endl;
            old_user_input = user_input;
            net_client->chat.emplace_back(old_user_input + "\n");
            MyPacket str_packet;
            str_packet << MyPacket::PacketID::CHAT;
            str_packet << old_user_input;
            net_client->socket.send(
                    reinterpret_cast<const std::byte *>(&str_packet.data()[0]), str_packet.length());

            user_input = "";

            if (old_user_input == "PLAY")
            {
                if (player1 == true && lobby_connections == 2)
                {
                    general_state = GeneralState::GAME;
                    sendGeneralStatePacket(GeneralState::GAME);
                }
            }
        }
    }
}

void ASGENetGame::keyHandlerGame(const auto *key)
{
    if (game_state == GameState::GAMEPLAY)
    {
        keyHandlerGameplay(key);
    }
    else if (game_state == GameState::BATTLE)
    {
        keyHandlerBattleMap(key);
    }
}

void ASGENetGame::keyHandlerGameplay(const auto *key)
{
    if (key->key == ASGE::KEYS::KEY_P && key->action == ASGE::KEYS::KEY_PRESSED)
    {
        game_state = GameState::BATTLE;
    }
}

void ASGENetGame::keyHandlerBattleMap(const auto *key)
{
    if (player.visibility)
    {
        if (key->key == ASGE::KEYS::KEY_UP && key->action == ASGE::KEYS::KEY_PRESSED)
        {
            player.upMovementKey = true;
        }
        if (key->key == ASGE::KEYS::KEY_UP && key->action == ASGE::KEYS::KEY_RELEASED)
        {
            player.upMovementKey = false;
        }
        if (key->key == ASGE::KEYS::KEY_DOWN && key->action == ASGE::KEYS::KEY_RELEASED)
        {
            player.downMovementKey = false;
        }
        if (key->key == ASGE::KEYS::KEY_DOWN && key->action == ASGE::KEYS::KEY_PRESSED)
        {
            player.downMovementKey = true;
        }
        if (key->key == ASGE::KEYS::KEY_LEFT && key->action == ASGE::KEYS::KEY_PRESSED)
        {
            player.leftMovementKey = true;
        }
        if (key->key == ASGE::KEYS::KEY_LEFT && key->action == ASGE::KEYS::KEY_RELEASED)
        {
            player.leftMovementKey = false;
        }
        if (key->key == ASGE::KEYS::KEY_RIGHT && key->action == ASGE::KEYS::KEY_RELEASED)
        {
            player.rightMovementKey = false;
        }
        if (key->key == ASGE::KEYS::KEY_RIGHT && key->action == ASGE::KEYS::KEY_PRESSED)
        {
            player.rightMovementKey = true;
        }
    }

    keyHandlerBattleMapUnits(key);
    keyHandlerBattleMapCheats(key);
}

void ASGENetGame::keyHandlerBattleMapUnits(const auto *key)
{
    if (units[unit_selected].visibility)
    {
        if (tiles.getWalkable(units[unit_selected]))
        {
            old_x_pos = units[unit_selected].sprite()->getX();
            old_y_pos = units[unit_selected].sprite()->getY();
            if ((!player1 && turn_over) || (!player2 && !turn_over))
            {
                if (
                        key->key == ASGE::KEYS::KEY_W && key->action == ASGE::KEYS::KEY_RELEASED &&
                        units[unit_selected].ap - units[unit_selected].getMovementCost() >= 0)
                {
                    units[unit_selected].moveUnit("up");

                    std::cout << units[unit_selected].ap << std::endl;
                }
                if (
                        key->key == ASGE::KEYS::KEY_A && key->action == ASGE::KEYS::KEY_RELEASED &&
                        units[unit_selected].ap - units[unit_selected].getMovementCost() >= 0)
                {
                    units[unit_selected].moveUnit("left");

                    std::cout << units[unit_selected].ap << std::endl;
                }
                if (
                        key->key == ASGE::KEYS::KEY_S && key->action == ASGE::KEYS::KEY_RELEASED &&
                        units[unit_selected].ap - units[unit_selected].getMovementCost() >= 0)
                {
                    units[unit_selected].moveUnit("down");
                    std::cout << units[unit_selected].ap << std::endl;
                }
                if (
                        key->key == ASGE::KEYS::KEY_D && key->action == ASGE::KEYS::KEY_RELEASED &&
                        units[unit_selected].ap - units[unit_selected].getMovementCost() >= 0)
                {
                    units[unit_selected].moveUnit("right");

                    std::cout << units[unit_selected].ap << std::endl;
                }


                if (key->key == ASGE::KEYS::KEY_ENTER && key->action == ASGE::KEYS::KEY_RELEASED)
                {
                    keyHandlerBattleMapUnitsEndTurn();
                }
            }
        }
    }
}

void ASGENetGame::keyHandlerBattleMapUnitsEndTurn()
{
    old_unit_selected = unit_selected;
    units[old_unit_selected].refreshUnitDetails();

    if (unit_selected < 7)
    {
        if (!turn_over)
        {

            unit_selected += 4;
            std::cout << "UNIT SELECTED: " << unit_selected << std::endl;
            turn_over = true;
            if (!player2 && multiplayer)
            {
                net_client->turn_over = turn_over;
            }
            endTurnPacket();
        }
        else
        {
            unit_selected -= 3;
            std::cout << "UNIT SELECTED: " << unit_selected << std::endl;
            turn_over = false;
            if (!player1 && multiplayer)
            {
                net_client->turn_over = turn_over;
            }
            endTurnPacket();
        }
    }
    else
    {
        unit_selected = 0;
        turn_over = false;
        if (!player1 && multiplayer)
        {
            net_client->turn_over = turn_over;
        }
        endTurnPacket();
        std::cout << "UNIT SELECTED: " << unit_selected << std::endl;
    }
}

void ASGENetGame::keyHandlerBattleMapCheats(const auto *key)
{
    if (key->key == ASGE::KEYS::KEY_P && key->action == ASGE::KEYS::KEY_PRESSED)
    {
        units[unit_selected].ap = units[unit_selected].getActionPoints();
        std::cout << units[unit_selected].unit_health << std::endl;
    }
}

void ASGENetGame::clickHandler(ASGE::SharedEventData data)
{
    const auto *click = dynamic_cast<const ASGE::ClickEvent *>(data.get());

    double x_pos = click->xpos;
    double y_pos = click->ypos;

    if (click->action == ASGE::KEYS::KEY_PRESSED)
    {
        Logging::DEBUG("x_pos: " + std::to_string(x_pos));
        Logging::DEBUG("y_pos: " + std::to_string(y_pos));
    }

    if (game_state == GAMEPLAY)
    {
        clickHandlerCharacterSelection(data);
    }
    else if (game_state == BATTLE)
    {
        clickHandlerBattleMap(data);

        if (
                ability_button.sprite()
                        ->getBoundingBox(ability_button.sprite()->getSprite()->scale())
                        .collision(static_cast<float>(click->xpos), static_cast<float>(click->ypos), 5, 5) &&
                click->action == ASGE::KEYS::KEY_PRESSED)
        {
            (!ability_mode) ? ability_mode = true : ability_mode = false;
        }
    }
}

void ASGENetGame::clickHandlerCharacterSelection(ASGE::SharedEventData data)
{
    if (game_state == GameState::GAMEPLAY)
    {
        if (character_select_play.clickCollision(data))
        {
            character_select_play.setIsActive(false);
            game_state = GameState::BATTLE;

            if (!player1 && multiplayer)
            {
                player2Packet();
            }
            else if (!player2 && multiplayer)
            {
                player1Packet();
            }
        }
    }
    else
    {
        character_select_play.setIsActive(true);
    }

    for (int i = 0; i < amount_character_select_buttons; i++)
    {
        if (character_select_buttons[i].clickCollision(data))
        {
            character_select_buttons[i].setIsActive(false);
            if (!player2)
            {
                if (p1_unit_sniper + p1_unit_medic < 4)

                {
                    switch (i)
                    {
                        case 0:
                            p1_unit_sniper++;

                            break;
                        case 1:
                            p1_unit_medic++;
                            break;
                    }
                }
            }
            if (!player1)
            {
                if (p2_unit_sniper + p2_unit_medic < 4)
                {
                    switch (i)
                    {
                        case 3:
                            p2_unit_sniper++;
                            break;
                        case 4:
                            p2_unit_medic++;
                            break;
                    }
                }
            }
            switch (i)
            {

                case 6:
                    if (!player2)
                    {
                        if (p1_unit_sniper > 0)
                        {
                            p1_unit_sniper--;
                        }
                    }
                    break;
                case 7:
                    if (!player2)
                    {
                        if (p1_unit_medic > 0)
                        {
                            p1_unit_medic--;
                        }
                    }
                    break;
                case 9:
                    if (!player1)
                    {
                        if (p2_unit_sniper > 0)
                        {
                            p2_unit_sniper--;
                        }
                    }
                    break;
                case 10:
                    if (!player2)
                    {
                        if (p2_unit_medic > 0)
                        {
                            p2_unit_medic--;
                        }
                    }
                    break;
            }
        }
        else
        {
            character_select_buttons[i].setIsActive(true);
        }
    }
}

void ASGENetGame::clickHandlerBattleMap(ASGE::SharedEventData data)
{
    for (int i = 0; i < unit_count; i++)
    {
        if ((!player1 && turn_over) || (!player2 && !turn_over))
        {
            if (units[i].clickCollision(data) && units[i].in_range)
            {
                std::cout << "unit type: " << units[i].getUnitType() << std::endl;

                if (!ability_mode)
                {
                    if (
                            units[i].team_number != units[unit_selected].team_number &&
                            units[unit_selected].ap - units[unit_selected].getAttackCost() >= 0)
                    {
                        units[unit_selected].attackUnit();
                        units[i].updateHealth(units[unit_selected].unit_damage, true);
                    }
                }
                else
                {
                    clickHandlerBattleMapAbilities(i);
                }
                if (multiplayer)
                {
                    unitPacket();
                }
            }
        }
    }
}

void ASGENetGame::clickHandlerBattleMapAbilities(int i)
{
    if (units[unit_selected].getUnitType() == units[unit_selected].UnitType::SNIPER)
    {
        if (
                units[i].team_number != units[unit_selected].team_number &&
                units[unit_selected].ap - units[unit_selected].useAbilityCost() >= 0)
        {
            units[unit_selected].useAbility();
            units[i].is_marked = true;
        }
    }
    else if (units[unit_selected].getUnitType() == units[unit_selected].UnitType::MEDIC)
    {
        std::cout << "PIGGY USED HEALING" << std::endl;
        if (
                units[i].team_number == units[unit_selected].team_number &&
                units[unit_selected].ap - units[unit_selected].useAbilityCost() >= 0)
        {
            std::cout << "HEALING " << units[i].getUnitType() << " WITH ABILITY" << std::endl;
            units[unit_selected].useAbility();
            units[i].updateHealth(units[unit_selected].heal_amount, false);
        }
    }
    else
    {
        return;
    }
}

/// Updates your game and all it's components.
/// @param us
void ASGENetGame::update(const ASGE::GameTime &us)
{
    for (auto &gc: game_components)
    {
        gc->update(us.deltaInSecs());
    }

    if (general_state == GeneralState::LOBBY)
    {
        updateLobby();
    }
    else if (general_state == GeneralState::GAME)
    {
        battle_camera.update(us);
        updateGame(us);
    }
}

// SUPPOSED TO HANDLE MESSAGES BEING SENT FROM IN-LOBBY BUT IT NEVER SENDS.
// LOBBY PLAYER COUNT ALSO DOES NOT INCREASE
void ASGENetGame::updateLobby()
{
    text_input.setString(user_input);

    if (!join_host)
    {
        std::cout << "Lobby connections: " << lobby_connections << std::endl;

        std::string str;
        str = std::to_string(lobby_connections);

        join_host = true;
    }
    lobby_connections = lobby_connections + net_client->lobby;
    net_client->lobby = 0;
    if (player2)
    {


        if (net_client->state == 0)
        {
            general_state = GeneralState::MENU;
        }
        if (net_client->state == 1)
        {
            general_state = GeneralState::LOBBY;
        }
        if (net_client->state == 2)
        {
            general_state = GeneralState::GAME;
        }
    }
    updateLobbyChat();
}

void ASGENetGame::updateLobbyChat()
{
    chat_text.setString("Chatroom: \n");
    for (auto &words: net_client->chat)
    {
        chat_text.setString(chat_text.getString() + words);
    }
}

void ASGENetGame::updateGame(const ASGE::GameTime &us)
{
    if (game_state == GameState::GAMEPLAY)
    {
        updateGameplay();
    }
    else if (game_state == GameState::BATTLE)
    {
        updateBattleMap(us);
    }
}

void ASGENetGame::updateGameplay()
{
    for (int i = 0; i < amount_character_select_pngs; i++)
    {

        switch (i)
        {
            case 0:
                amount_characters[i].setString(std::to_string(p1_unit_sniper));
                break;
            case 1:
                amount_characters[i].setString(std::to_string(p2_unit_sniper));
                break;
            case 2:
                amount_characters[i].setString(std::to_string(p1_unit_medic));
                break;
            case 3:
                amount_characters[i].setString(std::to_string(p2_unit_medic));
                break;
            case 4:
                amount_characters[i].setString(
                        std::to_string(team_size - (p1_unit_sniper + p1_unit_medic)));
                break;
            case 5:
                amount_characters[i].setString(
                        std::to_string(team_size - (p2_unit_sniper + p2_unit_medic)));
                break;
        }
    }
}

void ASGENetGame::updateBattleMap(const ASGE::GameTime &us)
{
    game_time -= static_cast<float>(us.deltaInSecs());
    player1_in_game = net_client->in_game1;
    player2_in_game = net_client->in_game2;
    player.visibility = true;
    player.playerUpdate();
    if (!player1 && multiplayer)
    {
        p1_unit_medic = net_client->p1_medic;
        p1_unit_sniper = net_client->p1_sniper;
        player2_in_game = true;
        MyPacket init_packet;
        init_packet << MyPacket::PacketID::INGAME2;
        init_packet << true;
        net_client->socket.send(&init_packet.data()[0], init_packet.length());
    }
    else if (!player2 && multiplayer)
    {
        p2_unit_medic = net_client->p2_medic;
        p2_unit_sniper = net_client->p2_sniper;
        player1_in_game = true;
        MyPacket init_packet;
        init_packet << MyPacket::PacketID::INGAME1;
        init_packet << true;
        net_client->socket.send(&init_packet.data()[0], init_packet.length());
    }
    if ((player2_in_game && player1_in_game) || (!multiplayer))
    {
#pragma unroll(1)
        for (int i = 0; i < unit_count; i++)
        {
            units[i].setUnitCount(team_size, p1_unit_sniper, p2_unit_sniper, p1_unit_medic, p2_unit_medic);
            units[i].setUnitType();
            units[i].updateUnitDetails();


            if (unit_selected < 7 && units[unit_selected].unit_health < 0)
            {
                if (!turn_over)
                {
                    unit_selected += 4;
                    std::cout << "UNIT SELECTED: " << unit_selected << std::endl;
                    turn_over = true;
                }
                else
                {
                    unit_selected -= 3;
                    std::cout << "UNIT SELECTED: " << unit_selected << std::endl;
                    turn_over = false;
                }
            }
            else if (unit_selected > 7 && units[unit_selected].unit_health < 0)
            {
                unit_selected = 0;

                std::cout << "UNIT SELECTED: " << unit_selected << std::endl;
            }

            (game_time >= 0) ? units[i].timed_unit_check = false : units[i].timed_unit_check = true;
        }


        if (multiplayer)
        {
            turn_over = net_client->turn_over;
            unit_selected = net_client->unit_selected;
            if (game_state == GameState::BATTLE)
            {
                unitPacket();
                unitCo();
                updateUnits();
            }
        }


        if (!tiles.getWalkable(units[unit_selected]))
        {
            units[unit_selected].ap += units[unit_selected].getMovementCost();
            units[unit_selected].sprite()->setX(old_x_pos);
            units[unit_selected].sprite()->setY(old_y_pos);
        }
    }

    battle_camera.lookAt(ASGE::Point2D{
            static_cast<float>(
                    player.sprite()->getX() +
                    (player.sprite()->getSprite()->width() * player.sprite()->getSprite()->scale() / 2)),
            static_cast<float>(
                    player.sprite()->getY() +
                    (player.sprite()->getSprite()->height() * player.sprite()->getSprite()->scale() / 2))});
}

/// Render your game and its scenes here.
void ASGENetGame::render(const ASGE::GameTime & /*us*/)
{
    if (general_state == MENU)
    {
        renderMenu();
    }
    else if (general_state == LOBBY)
    {
        renderLobby();
    }
    else if (general_state == GAME)
    {
        renderGame();
    }
}

void ASGENetGame::renderMenu()
{
    renderer->renderSprite(*menu_background);

    for (int i = 0; i < amount_menu_buttons; i++)
    {
        menu_buttons[i].setIsActive(i == menu_option);
        menu_buttons[i].goRender(renderer.get());
    }
}

void ASGENetGame::renderLobby()
{
    renderer->renderSprite(*lobby_background);

    if (lobby_connections == 1)
    {
        renderer->renderSprite(*lobbyP1);
    }
    else if (lobby_connections == 2)
    {
        renderer->renderSprite(*lobbyP1);
        renderer->renderSprite(*lobbyP2);
    }

    text_input.setFont(renderer->getFont(font_index));
    setupTextRender(text_input, 9, 620, 630, ASGE::COLOURS::WHITE);
    renderer->renderText(text_input);

    lobby_text.setString("Press '1' or type 'PLAY' to continue");
    lobby_text.setFont(renderer->getFont(font_index));
    setupTextRender(lobby_text, 9, 620, 130, ASGE::COLOURS::WHITE);
    renderer->renderText(lobby_text);

    chat_text.setFont(renderer->getFont(font_index));
    setupTextRender(chat_text, 9, 620, 230, ASGE::COLOURS::WHITE);
    renderer->renderText(chat_text);
}

void ASGENetGame::renderGame()
{
    if (game_state == GameState::GAMEPLAY)
    {
        renderGameplay();
    }
    else if (game_state == GameState::BATTLE)
    {
        renderBattleMap();
    }
}

void ASGENetGame::renderGameplay()
{
    renderer->setProjectionMatrix(menu_camera.getView());
    character_select_play.goRender(renderer.get());
    for (int i = 0; i < amount_character_select_buttons; i++)
    {
        if ((!player2 && (i <= 2 || (i > 5 && i <= 8))) || (!player1 && (i > 8 || (i <= 5 && i > 2))))
        {
            character_select_buttons[i].goRender(renderer.get());
        }
    }
    for (int i = 0; i < amount_character_select_pngs; i++)
    {
        if ((!player2 && ((i == 0) || (i == 2) || (i == 4))) || (!player1 && ((i == 1) || (i == 3) || (i == 5))))
        {
            character_select_pngs[i].visibility = true;
            character_select_pngs[i].goRender(renderer.get());
            renderer->renderText(amount_characters[i]);
        }
    }
}

void ASGENetGame::renderBattleMap()
{

    renderer->setProjectionMatrix(battle_camera.getView());

    tiles.renderTiles(renderer.get());

#pragma unroll(1)
    for (int i = 0; i < unit_count; i++)
    {
        units[i].goRender(renderer.get());
        units[i].getSprite(i);
        units[i].updateAbility(renderer.get());

        units[i].getHealthText(renderer.get(), &unit_health_text);

        if (checkUnitRange(i) && units[i].team_number != units[unit_selected].team_number)
        {
            units[i].isInRange(renderer.get());
        }
        else if (
                checkUnitRange(i) && units[i].team_number == units[unit_selected].team_number && ability_mode)
        {
            if (units[unit_selected].type_of_unit == units[unit_selected].UnitType::MEDIC)
            {
                units[i].isInRange(renderer.get());
            }
        }
        else
        {
            units[i].notInRange();
        }
    }

    units[unit_selected].getSelectedUnit(renderer.get());

    player.goRender(renderer.get());

    ability_button.sprite()->setX(player.sprite()->getX() + 500);
    ability_button.sprite()->setY(player.sprite()->getY() + 250);
    ability_button.goRender(renderer.get());

    setupTextRender(
            unit_ap_text,
            15,
            player.sprite()->getX() - 500,
            player.sprite()->getY() - 250,
            ASGE::COLOURS::WHITE);

    unit_ap_text.setString("Action Points Remaining: " + std::to_string(units[unit_selected].ap));
    renderer->renderText(unit_ap_text);

    if (ability_mode)
    {
        std::cout << "CURRENTLY IN ABILITY MODE" << std::endl;
    }
}

bool ASGENetGame::checkUnitRange(int i)
{
    if (
            units[i].sprite()->getX() <
                    units[unit_selected].sprite()->getX() + (units[unit_selected].unit_range * 24) &&
            units[i].sprite()->getX() >
                    units[unit_selected].sprite()->getX() - (units[unit_selected].unit_range * 24))
    {
        return units[i].sprite()->getY() <
                       units[unit_selected].sprite()->getY() + (units[unit_selected].unit_range * 24) &&
               units[i].sprite()->getY() >
                       units[unit_selected].sprite()->getY() - (units[unit_selected].unit_range * 24);
    }

    return false;
}

/// Calls to fixedUpdate use the same fixed timestep
/// irrespective of how much time is passed. If you want
/// deterministic behaviour on clients, this is the place
/// to go.
///
/// https://gamedev.stackexchange.com/questions/1589/when-should-i-use-a-fixed-or-variable-time-step
/// "Use variable timesteps for your game and fixed steps for physics"
/// @param us
void ASGENetGame::fixedUpdate(const ASGE::GameTime &us)
{
    Game::fixedUpdate(us);
}

void ASGENetGame::setupInitSprites(
        std::unique_ptr<ASGE::Sprite> &sprite, const std::string &texture_file_path, float width,
        float height, int16_t z_order, float x_pos, float y_pos, float scale)
{
    sprite = renderer->createUniqueSprite();
    sprite->loadTexture(texture_file_path);

    if (!sprite->loadTexture(texture_file_path))
    {
        std::cout << "SPRITE FAILED TO INITIALISE" << std::endl;
        return;
    }

    sprite->height(height);
    sprite->width(width);
    sprite->setGlobalZOrder(z_order);
    sprite->xPos(x_pos);
    sprite->yPos(y_pos);
    sprite->scale(scale);

    sprite->setMagFilter(ASGE::Texture2D::MagFilter::NEAREST);
}

void ASGENetGame::setupTextRender(
        ASGE::Text &text, int16_t z_order, float x_pos, float y_pos, ASGE::Colour colour)
{
    text.setFont(renderer->getFont(font_index));
    text.setZOrder(z_order);
    text.setPositionX(x_pos);
    text.setPositionY(y_pos);
    text.setColour(colour);
}
void ASGENetGame::sendIntPacket(int data)
{
    MyPacket int_packet;
    int_packet << MyPacket::PacketID::INT;
    int_packet << data;
    net_client->socket.send(&int_packet.data()[0], int_packet.length());
}
void ASGENetGame::sendGeneralStatePacket(int state)
{
    MyPacket state_packet;
    state_packet << MyPacket::PacketID::STATE;
    state_packet << state;

    net_client->socket.send(&state_packet.data()[0], state_packet.length());
}
void ASGENetGame::sendInitPacket()
{
    MyPacket init_packet;
    init_packet << MyPacket::PacketID::BOOL;
    init_packet << true;
    net_client->socket.send(&init_packet.data()[0], init_packet.length());
}
void ASGENetGame::player2Packet()
{
    MyPacket player_packet;
    player_packet << MyPacket::PacketID::TEAM2;
    player_packet << p2_unit_medic;
    player_packet << p2_unit_sniper;
    net_client->socket.send(&player_packet.data()[0], player_packet.length());
}
void ASGENetGame::player1Packet()
{
    MyPacket player_packet;
    player_packet << MyPacket::PacketID::TEAM1;
    player_packet << p1_unit_medic;
    player_packet << p1_unit_sniper;
    net_client->socket.send(&player_packet.data()[0], player_packet.length());
}
void ASGENetGame::endTurnPacket()
{
    MyPacket turn_packet;
    turn_packet << MyPacket::PacketID::TURN;
    turn_packet << turn_over;
    turn_packet << unit_selected;
    net_client->socket.send(&turn_packet.data()[0], turn_packet.length());
}
void ASGENetGame::unitPacket()
{
    for (int i = 0; i < unit_count; ++i)
    {
        BattleUnit unit = BattleUnit();

        unit.unit_health = units[i].unit_health;
        if (units[i].unit_health > 0)
        {
            units[i].visibility = true;
        }
        else
        {
            units[i].visibility = false;
        }
        unit.visibility = units[i].visibility;


        MyPacket unit_packet;

        unit_packet << MyPacket::PacketID::PLAYER;
        unit_packet << i;
        unit_packet << unit;
        net_client->socket.send(&unit_packet.data()[0], unit_packet.length());
    }
}
void ASGENetGame::updateUnits()
{
    if ((!player2 && turn_over) || (!player1 && !turn_over))
    {
        for (int i = 0; i < unit_count; ++i)
        {
            units[i].unit_health = net_client->units[i].unit_health;
            units[i].unit_health_string = std::to_string(static_cast<int>(units[i].unit_health));
            units[i].visibility = net_client->units[i].visibility;
            units[i].sprite()->setX(net_client->units[i].xpos);
            units[i].sprite()->setY(net_client->units[i].ypos);
        }
    }
}
void ASGENetGame::unitCo()
{
    for (int i = 0; i < unit_count; ++i)
    {


        float x = units[i].sprite()->getX();
        float y = units[i].sprite()->getY();
        MyPacket unit_Co_packet;

        unit_Co_packet << MyPacket::PacketID::COORDINATES;
        unit_Co_packet << i;
        unit_Co_packet << x;
        unit_Co_packet << y;
        net_client->socket.send(&unit_Co_packet.data()[0], unit_Co_packet.length());
    }
}
