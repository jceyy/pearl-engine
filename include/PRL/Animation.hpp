#ifndef ANIMATION_HPP
#define ANIMATION_HPP


class Animation{
public:
    Animation();
    Animation(int i, int frames, int fps);

    int index;
    int frames;
    int fps;
};

#endif // ANIMATION_HPP