#ifndef _UI_LABEL_HPP_INCLUDED
#define _UI_LABEL_HPP_INCLUDED

#include <string>
#include <SDL2/SDL.h>
#include <SDL2/SDL_ttf.h>
#include "../Types.hpp"
#include "ECS.hpp"
#include "../AssetManager.hpp"
#include "../Game.hpp"


class UILabel : public Component {
public:
    UILabel(PosType x, PosType y, const std::string& text, const std::string& fontID, const SDL_Color& color);
    ~UILabel();

    void setLabelText(const std::string& text);
    void setLabelText(const std::string& text, const std::string& fontID);
    
    std::string getFontID() const;

    void draw() override;

private:
    SDL_FRect dstRect_;
    std::string labelText_;
    std::string fontID_;
    SDL_Color textColor_;
    SDL_Texture* labelTexture_;
};


#endif // _UI_LABEL_HPP_INCLUDED