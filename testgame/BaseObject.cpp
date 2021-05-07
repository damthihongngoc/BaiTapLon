#include "BaseObject.h"

//Dùng để load ảnh và render ảnh lên màn hình
BaseObject::BaseObject()
{
    p_object_ = NULL;
    rect_.x = 0;
    rect_.y = 0;
    rect_.w = 0;
    rect_.h = 0;
}

BaseObject::~BaseObject()
{
    Free();
}

bool BaseObject::LoadImageFile(std::string path, SDL_Renderer* screen)
{
    //
    Free();
    SDL_Texture* newTexture = NULL;

    //Load image
    SDL_Surface* loadedSurface = IMG_Load(path.c_str());
    if (loadedSurface != NULL)
    {
        //Xóa phông nền của ảnh
        Uint32 key = SDL_MapRGB(loadedSurface->format,
            COLOR_KEY_R,
            COLOR_KEY_G,
            COLOR_KEY_B);
        SDL_SetColorKey(loadedSurface, SDL_TRUE, key);

        
        newTexture = SDL_CreateTextureFromSurface(screen, loadedSurface);
        if (newTexture != NULL)
        {
            //Lấy vị trí của ảnh
            rect_.w = loadedSurface->w;
            rect_.h = loadedSurface->h;
        }

        //bỏ surface 
        SDL_FreeSurface(loadedSurface);
    }

    p_object_ = newTexture;
    return p_object_ != NULL;
}

void BaseObject::Free() // Hàm giải phóng đối tượng
{
    if (p_object_ != NULL)
    {
        SDL_DestroyTexture(p_object_);
        p_object_ = NULL;
        rect_.w = 0;
        rect_.h = 0;
    }
}

void BaseObject::Render(SDL_Renderer* des, const SDL_Rect* clip /*=NULL*/)
{
    SDL_Rect renderQuad = { rect_.x, rect_.y, rect_.w, rect_.h };// Vi tri va kich thuoc anh
    if (clip != NULL)
    {
        renderQuad.w = clip->w;
        renderQuad.h = clip->h;
    }
    SDL_RenderCopy(des, p_object_, clip, &renderQuad); // đẩy thông số của p_object cho des với kích thước và vị trí nằm trong biến renderQuad
}