#include "PlayerObject.h"


#define const int WIDTH_PLAYER_OBJECT = 64;
#define const int HEIGHT_PLAYER_OBJECT = 40;

PlayerObject::PlayerObject()
{
    is_falling_ = false;
    y_val_ = 1;
}

PlayerObject::~PlayerObject()
{
    Free();
}
// hàm dùng để bắt sự kiện bên ngoài như chuột và bàn phím 
void PlayerObject::HandleInputAction(SDL_Event events, SDL_Renderer* screen)
{
    if (events.type == SDL_KEYDOWN) // phím ấn xuống
    {
        switch (events.key.keysym.sym)
        {
        case SDLK_SPACE:
        {
            if (is_falling_ == false)
            {
                y_val_ = -15;  // mỗi lần ấn phím thì vị trí chiều cao  - 15 
                Mix_Chunk* beep_sound = Mix_LoadWAV("sound//audio_wing.wav");
                if (beep_sound != NULL)
                    Mix_PlayChannel(-1, beep_sound, 0);
            }
        }
        } 
    }
    else if (events.type == SDL_KEYUP) //phím nhả ra
    {
        switch (events.key.keysym.sym)
        {
        case SDLK_SPACE:
        {
            y_val_ = 5;   //  nhả phím thì vị trí chiều cao  - 5 
        }
        break;
        }
    }
}


bool PlayerObject::LoadImg(std::string path, SDL_Renderer* screen)
{
    bool ret = BaseObject::LoadImageFile(path, screen);
    return ret;
}

void PlayerObject::Show(SDL_Renderer* des)
{
    BaseObject::Render(des);
}

void PlayerObject::DoFalling(SDL_Renderer* des)
{
    rect_.y += y_val_;
    if ((rect_.y + rect_.h) >= GROUND_MAP)
    {
        LoadImg("img//yellowbird3.png", des);
        is_falling_ = true;
        DoGround(des);
    }
}   

void PlayerObject::DoGround(SDL_Renderer* screen)
{
    y_val_ = 0;
    rect_.y = GROUND_MAP - rect_.h;
    is_die_ = true;
}