#include <utility/SpriteComponent.h>

void SpriteComponent::render(ASGE::Renderer *renderer) {
    if (sprite) {
        renderer->renderSprite(*sprite);
    }
}

void SpriteComponent::newSprite(
        const std::string &texture_file_name, ASGE::Renderer *renderer, int z_order) {
    sprite = renderer->createUniqueSprite();
    sprite->setGlobalZOrder(static_cast<int16_t>(z_order));
    sprite->loadTexture(texture_file_name);
}

void SpriteComponent::setSprite(const std::string &texture_file_name) {
    sprite->loadTexture(texture_file_name);
}

void SpriteComponent::setX(float x) {
    sprite->xPos(x);
}

void SpriteComponent::setY(float y) {
    sprite->yPos(y);
}

float SpriteComponent::getX() {
    return sprite->xPos();
}

float SpriteComponent::getY() {
    return sprite->yPos();
}

void SpriteComponent::setBounds(float posX, float posY) {
    src_x = posX;
    src_y = posY;

    getSprite()->srcRect()[0] = posX;
    getSprite()->srcRect()[1] = posY;
    getSprite()->srcRect()[2] = width;
    getSprite()->srcRect()[3] = height;
}

void SpriteComponent::setDimensions(float w, float h, float s) {
    width = w;
    height = h;

    getSprite()->width(w);
    getSprite()->height(h);
    getSprite()->scale(s);

    getSprite()->setMagFilter(ASGE::Texture2D::MagFilter::NEAREST);
}

BoundingBox SpriteComponent::getBoundingBox(float scale) const {
    BoundingBox bounding_box;
    bounding_box.xPos = sprite->xPos();
    bounding_box.yPos = sprite->yPos();
    bounding_box.width = sprite->width() * scale;
    bounding_box.height = sprite->height() * scale;

    return bounding_box;
}

const std::unique_ptr<ASGE::Sprite> &SpriteComponent::getSprite() const {
    return sprite;
}
