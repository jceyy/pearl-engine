#ifndef _BUTTON_COMPONENT_HPP_INCLUDED
#define _BUTTON_COMPONENT_HPP_INCLUDED
/*
#include "ECS.hpp"
#include "UILabel.hpp"

class ButtonComponent : public Component {
public:
    // ButtonComponent(const std::string label, const std::string fontID, const SDL_FRect& pos);
    ButtonComponent(const std::string& label, const std::string fontID, const SDL_FRect& pos,
                    const SDL_Color& idleBackground, const SDL_Color& idleBorder, 
                    const SDL_Color& selectBackground, const SDL_Color& selectBorder,
                    int borderThickness);
    ~ButtonComponent();

    void init() override;
    void update () override;
    void draw() override;

    // void setIdleColors(const SDL_Color& idleBackground, const SDL_Color& idleBorder) noexcept;
    // void setSelectColors(const SDL_Color& selectBackground, const SDL_Color& selectBorder) noexcept;
    // void setThickness(int thickness) noexcept;
    void setText(const std::string& text) noexcept;
    void setText(const std::string& text, const std::string& fontID) noexcept;


private:
    std::string textLabel_;
    std::string fontID_;
    SDL_Color idleBackground_;
    SDL_Color idleBorder_;
    SDL_Color selectBackground_;
    SDL_Color selectBorder_;
    int borderThickness_;
    bool textChanged_;

    UILabel* uiLabel_;

};*/

#endif // _BUTTON_COMPONENT_HPP_INCLUDED