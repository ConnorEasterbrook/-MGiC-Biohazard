#ifndef ASGENETGAME_BATTLEUNIT_H
#define ASGENETGAME_BATTLEUNIT_H

#include "../GameObject.h"
#include "../include/ASGEGameLib/MyPacket.hpp"
#include <Engine/InputEvents.hpp>
#include <Engine/Keys.hpp>

class BattleUnit : public GameObject {
public:
    BattleUnit() = default;
    ~BattleUnit() override = default;

    // UNIT INIT FUNCTIONS
    void initTexture2D(ASGE::Renderer *renderer);
    void setCoords(float xPos, float yPos);
    [[nodiscard]] int getUnitType() const;

    // LOAD MAP FUNCTIONS
    void setUnitCount(int team_size, int p1sniper, int p2sniper, int p1medic, int p2medic);
    void getSprite(int i);
    void setUnitType();

    // ACTION POINT FUNCTIONS
    [[nodiscard]] float getActionPoints() const;
    [[nodiscard]] float getMovementCost() const;
    [[nodiscard]] float getAttackCost() const;

    // BASE UNIT DETAIL FUNCTIONS
    [[nodiscard]] float setBaseDamage() const;
    [[nodiscard]] float setHealth() const;
    void getSelectedUnit(ASGE::Renderer *renderer);
    void getHealthText(ASGE::Renderer *renderer, ASGE::Text *health_text) const;
    [[nodiscard]] std::string getHealthString() const;
    [[nodiscard]] float setRange() const;
    [[nodiscard]] float setAbility() const;

    // UNIT ACTION FUNCTIONS
    void refreshUnitDetails();
    void updateUnitDetails();
    void setUnitDetails();
    void moveUnit(const std::string &move_dir);
    [[maybe_unused]] void attackUnit();
    [[maybe_unused]] void updateHealth(float damage, bool attack);
    void useAbility();
    [[nodiscard]] float useAbilityCost();
    void updateAbility(ASGE::Renderer *renderer);
    void isInRange(ASGE::Renderer *renderer);
    void notInRange();
    bool clickCollision(ASGE::SharedEventData data) override;


    friend MyPacket &operator<<(MyPacket &packet, BattleUnit &unit);
    friend MyPacket &operator>>(MyPacket &packet, BattleUnit &unit);
    enum UnitType// Control unit type
    {
        SNIPER = 1,
        MEDIC = 2,
        GUNNER = 3,
    };
    int type_of_unit{};

    enum UnitAbility// Control unit ability
    {
        MARK = 1,
        HEAL = 2,
        NONE = 3,
    };

    float ap{};
    float movement_ap_cost{};
    float attack_ap_cost{};
    float heal_amount = 10;
    float unit_damage{};
    float old_unit_health{};
    float unit_health{};
    std::string unit_health_string{};
    float unit_range{};
    float unit_ability{};
    float xpos = 0;
    float ypos = 0;

    int team_number{};
    bool in_range = false;
    bool is_marked = false;
    bool timed_unit_check = false;

private:
    void movementAP();

    void setupInitSprites(
            ASGE::Renderer *renderer, std::unique_ptr<ASGE::Sprite> &sprite,
            const std::string &texture_file_path, float width, float height, int16_t z_order,
            float x_pos_sprite, float y_pos_sprite, float scale);

    float x_pos{};
    float y_pos{};

    bool unit_check = false;

    int team_size = 4;
    int p1_unit_sniper = 1;
    int p1_unit_medic = 1;
    int p2_unit_sniper = 1;
    int p2_unit_medic = 1;

    std::unique_ptr<ASGE::Sprite> unit_sniper = nullptr;
    std::unique_ptr<ASGE::Sprite> unit_medic = nullptr;
    std::unique_ptr<ASGE::Sprite> unit_gunner = nullptr;

    std::unique_ptr<ASGE::Sprite> unit_selected = nullptr;
    std::unique_ptr<ASGE::Sprite> unit_in_range = nullptr;
    std::unique_ptr<ASGE::Sprite> unit_is_marked = nullptr;
};

#endif// ASGENETGAME_BATTLEUNIT_H
