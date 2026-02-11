#include "ECS/UILabel.hpp"

UILabel::UILabel(PosType x, PosType y, const std::string& text, const std::string& fontID, const SDL_Color& color) :
dstRect_({x, y, 0, 0}), labelText_(text), fontID_(fontID), 
textColor_(color), labelTexture_(nullptr) {
    setLabelText(labelText_);
}


UILabel::~UILabel() {
    if (labelTexture_ != nullptr) {
        SDL_DestroyTexture(labelTexture_);
        labelTexture_ = nullptr;
    }
}

void UILabel::setLabelText(const std::string& text) {
    setLabelText(text, fontID_);
}


void UILabel::setLabelText(const std::string& text, const std::string& fontID) {
    labelText_ = text;
    fontID_ = fontID;

    if (labelTexture_ != nullptr) SDL_DestroyTexture(labelTexture_);

    SDL_Surface* surface = TTF_RenderText_Blended(Game::assetManager->getFont(fontID_), labelText_.c_str(), textColor_);
    labelTexture_ = SDL_CreateTextureFromSurface(Game::renderer, surface);
    dstRect_.w = surface->w;
    dstRect_.h = surface->h;
    SDL_FreeSurface(surface);
    // std::cout << "Debug " << w << " " << h << std::endl;
}


std::string UILabel::getFontID() const {
    return fontID_;
}


void UILabel::draw() {
    SDL_RenderCopyF(Game::renderer, labelTexture_, nullptr, &dstRect_);
}