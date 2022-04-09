#include "gameObject/battleUnits/BattleUnit.h"

void BattleUnit::initTexture2D(ASGE::Renderer* renderer)
{
  setupInitSprites(
    renderer, unit_sniper, "data/images/battleMap/units/skeleton.png", 16, 16, 10, 00, 00, 1);
  setupInitSprites(
    renderer, unit_medic, "data/images/battleMap/units/pig.png", 16, 16, 10, 00, 00, 1);
  setupInitSprites(
    renderer, unit_gunner, "data/images/battleMap/units/orc.png", 16, 16, 10, 00, 00, 1);

  setupInitSprites(
    renderer,
    unit_selected,
    "data/images/battleMap/units/unit_selected.png",
    16,
    16,
    12,
    sprite()->getX(),
    sprite()->getY() + 8,
    2);
  unit_selected->opacity(0.50);

  setupInitSprites(
    renderer,
    unit_in_range,
    "data/images/battleMap/units/unit_in_range.png",
    16,
    16,
    11,
    sprite()->getX(),
    sprite()->getY() + 8,
    2);

  setupInitSprites(
    renderer,
    unit_is_marked,
    "data/images/battleMap/units/unit_enemy.png",
    16,
    16,
    11,
    sprite()->getX(),
    sprite()->getY() + 8,
    2);
}

/// For each instance created within the main class, set the xPos of the Enemy
void BattleUnit::setCoords(float xPos, float yPos)
{
  x_pos = xPos;
  y_pos = yPos;
}

int BattleUnit::getUnitType() const
{
  return type_of_unit;
}

void BattleUnit::setUnitCount(int teamsize, int p1sniper, int p2sniper, int p1medic, int p2medic)
{
  team_size      = teamsize;
  p1_unit_sniper = p1sniper;
  p1_unit_medic  = p1medic;
  p2_unit_sniper = p2sniper;
  p2_unit_medic  = p2medic;
}

void BattleUnit::getSprite(int i)
{
  if (i < team_size)
  {
    if (i < p1_unit_sniper)
    {
      sprite()->getSprite()->loadTexture("data/images/battleMap/units/skeleton.png");
    }
    else if (i < p1_unit_sniper + p1_unit_medic)
    {
      sprite()->getSprite()->loadTexture("data/images/battleMap/units/pig.png");
    }
    else
    {
      sprite()->getSprite()->loadTexture("data/images/battleMap/units/orc.png");
    }
  }
  else
  {
    if (i - team_size < p2_unit_sniper)
    {
      sprite()->getSprite()->loadTexture("data/images/battleMap/units/skeleton.png");
    }
    else if (i - team_size < p2_unit_sniper + p2_unit_medic)
    {
      sprite()->getSprite()->loadTexture("data/images/battleMap/units/pig.png");
    }
    else
    {
      sprite()->getSprite()->loadTexture("data/images/battleMap/units/orc.png");
    }
  }

  sprite()->setDimensions(16, 16, 2);
  sprite()->setBounds(0, 24);
}

void BattleUnit::setUnitType()
{
  if (sprite()->getSprite()->getTexture() == unit_sniper->getTexture())
  {
    type_of_unit = 1;
  }
  else if (sprite()->getSprite()->getTexture() == unit_medic->getTexture())
  {
    type_of_unit = 2;
  }
  else if (sprite()->getSprite()->getTexture() == unit_gunner->getTexture())
  {
    type_of_unit = 3;
  }
}

float BattleUnit::getActionPoints() const
{
  float action_points = 20;
  return action_points;
}

float BattleUnit::getMovementCost() const
{
  float movement_cost = 2;

  if (type_of_unit == UnitType::SNIPER)
  {
    movement_cost = 3;
    return movement_cost;
  }
  if (type_of_unit == UnitType::MEDIC)
  {
    movement_cost = 2;
    return movement_cost;
  }
  if (type_of_unit == UnitType::GUNNER)
  {
    movement_cost = 2;
    return movement_cost;
  }

  return movement_cost;
}

[[maybe_unused]] float BattleUnit::getAttackCost() const
{
  float attack_cost = 6;

  if (type_of_unit == UnitType::SNIPER)
  {
    attack_cost = 8;
    return attack_cost;
  }
  if (type_of_unit == UnitType::MEDIC)
  {
    attack_cost = 6;
    return attack_cost;
  }
  if (type_of_unit == UnitType::GUNNER)
  {
    attack_cost = 4;
    return attack_cost;
  }

  return attack_cost;
}

float BattleUnit::setBaseDamage() const
{
  float base_damage = 4;

  if (type_of_unit == UnitType::SNIPER)
  {
    base_damage = 6;
    return base_damage;
  }
  if (type_of_unit == UnitType::MEDIC)
  {
    base_damage = 2;
    return base_damage;
  }
  if (type_of_unit == UnitType::GUNNER)
  {
    base_damage = 4;
    return base_damage;
  }

  return base_damage;
}

float BattleUnit::setHealth() const
{
  float health = 15;

  if (type_of_unit == UnitType::SNIPER)
  {
    health = 10;
    return health;
  }
  if (type_of_unit == UnitType::MEDIC)
  {
    health = 15;
    return health;
  }
  if (type_of_unit == UnitType::GUNNER)
  {
    health = 20;
    return health;
  }

  return health;
}

void BattleUnit::getSelectedUnit(ASGE::Renderer* renderer)
{
  unit_selected->xPos(sprite()->getX());
  unit_selected->yPos(sprite()->getY() + 8);
  renderer->render(*unit_selected);
}

void BattleUnit::getHealthText(ASGE::Renderer* renderer, ASGE::Text* health_text) const
{
  health_text->setString(getHealthString());

  health_text->setPositionX(sprite()->getX() + 8);
  health_text->setPositionY(sprite()->getY() - 4);

  if (unit_health > 0)
  {
    renderer->renderText(*health_text);
  }
}

std::string BattleUnit::getHealthString() const
{
  return unit_health_string;
}

float BattleUnit::setRange() const
{
  float range = 10;

  if (type_of_unit == UnitType::SNIPER)
  {
    range = 15;
    return range;
  }
  if (type_of_unit == UnitType::MEDIC)
  {
    range = 10;
    return range;
  }
  if (type_of_unit == UnitType::GUNNER)
  {
    range = 10;
    return range;
  }

  return range;
}

float BattleUnit::setAbility() const
{
  float ability = 3;

  if (type_of_unit == UnitType::SNIPER)
  {
    ability = UnitAbility::MARK;
    return ability;
  }
  if (type_of_unit == UnitType::MEDIC)
  {
    ability = UnitAbility::HEAL;
    return ability;
  }
  if (type_of_unit == UnitType::GUNNER)
  {
    ability = UnitAbility::NONE;
    return ability;
  }

  return ability;
}

void BattleUnit::refreshUnitDetails()
{
  ap = getActionPoints();
}

void BattleUnit::updateUnitDetails()
{
  old_unit_health  = setHealth();
  unit_range       = setRange();
  unit_damage      = setBaseDamage();
  attack_ap_cost   = getAttackCost();
  movement_ap_cost = getMovementCost();

  if (!unit_check)
  {
    unit_health = setHealth();

    if (type_of_unit != 3)
    {
      std::cout << unit_health << " UNIT HEALTH " << type_of_unit << " UNIT TYPE" << std::endl;
      unit_check = true;
    }
    else if (timed_unit_check)
    {
      std::cout << unit_health << " UNIT HEALTH " << type_of_unit << " UNIT TYPE" << std::endl;
      unit_check = true;
    }
  }

  unit_health_string = std::to_string(static_cast<int>(unit_health));
}

void BattleUnit::setUnitDetails()
{
  if (sprite()->getSprite()->getTexture() == unit_sniper->getTexture())
  {
    type_of_unit = 1;
  }
  else if (sprite()->getSprite()->getTexture() == unit_medic->getTexture())
  {
    type_of_unit = 2;
  }
  else if (sprite()->getSprite()->getTexture() == unit_gunner->getTexture())
  {
    type_of_unit = 3;
  }

  ap               = getActionPoints();
  unit_damage      = setBaseDamage();
  unit_health      = setHealth();
  unit_range       = setRange();
  unit_ability     = setAbility();
  movement_ap_cost = getMovementCost();
  attack_ap_cost   = getAttackCost();

  unit_health_string = std::to_string(static_cast<int>(unit_health));
}

void BattleUnit::moveUnit(const std::string& move_dir)
{
  if (move_dir == "up")
  {
    sprite()->setY(sprite()->getY() - 24);
  }
  else if (move_dir == "left")
  {
    sprite()->setX(sprite()->getX() - 24);
  }
  else if (move_dir == "down")
  {
    sprite()->setY(sprite()->getY() + 24);
  }
  else if (move_dir == "right")
  {
    sprite()->setX(sprite()->getX() + 24);
  }
  else
  {
    return;
  }

  movementAP();
}

void BattleUnit::movementAP()
{
  ap -= movement_ap_cost;
}

void BattleUnit::attackUnit()
{
  ap -= attack_ap_cost;
}

void BattleUnit::updateHealth(float damage, bool attack)
{
  if (attack)
  {
    if (!is_marked)
    {
      unit_health -= damage;
    }
    else
    {
      unit_health -= (damage + 2);
    }
  }
  else
  {
    unit_health += heal_amount;
  }

  (unit_health > 0) ? unit_health_string = std::to_string(static_cast<int>(unit_health))
                    : unit_health_string = "";

  if (unit_health > old_unit_health)
  {
    unit_health = old_unit_health;
  }
}

void BattleUnit::useAbility()
{
  if (type_of_unit == UnitType::SNIPER)
  {
    ap -= 8;
  }
  if (type_of_unit == UnitType::MEDIC)
  {
    ap -= 15;
  }
}

float BattleUnit::useAbilityCost()
{
  if (type_of_unit == UnitType::SNIPER)
  {
    return 8;
  }
  if (type_of_unit == UnitType::MEDIC)
  {
    return 15;
  }

  return 8;
}

void BattleUnit::updateAbility(ASGE::Renderer* renderer)
{
  unit_is_marked->xPos(sprite()->getX());
  unit_is_marked->yPos(sprite()->getY() + 8);

  if (unit_health > 0 && is_marked)
  {
    renderer->render(*unit_is_marked);
  }
}

void BattleUnit::isInRange(ASGE::Renderer* renderer)
{
  unit_in_range->xPos(sprite()->getX());
  unit_in_range->yPos(sprite()->getY() + 8);

  if (unit_health > 0)
  {
    renderer->render(*unit_in_range);
  }

  in_range = true;
}

void BattleUnit::notInRange()
{
  in_range = false;
}

bool BattleUnit::clickCollision(ASGE::SharedEventData data)
{
  const auto* click = dynamic_cast<const ASGE::ClickEvent*>(data.get());

  if (click->action == ASGE::KEYS::KEY_PRESSED)
  {
    return static_cast<bool>(
      sprite()
        ->getBoundingBox(sprite()->getSprite()->scale())
        .collision(static_cast<float>(click->xpos), static_cast<float>(click->ypos), 5, 5));
  }

  return false;
}

void BattleUnit::setupInitSprites(
  ASGE::Renderer* renderer, std::unique_ptr<ASGE::Sprite>& sprite,
  const std::string& texture_file_path, float width, float height, int16_t z_order,
  float x_pos_sprite, float y_pos_sprite, float scale)
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
  sprite->xPos(x_pos_sprite);
  sprite->yPos(y_pos_sprite);
  sprite->scale(scale);

  sprite->setMagFilter(ASGE::Texture2D::MagFilter::NEAREST);
}
