#include "CommonFunc.h"
#include "PlayerObject.h"
#include "ImpTimer.h"
#include "BlockObject.h"
#include "Geometric.h"
#include"BaseObject.h"
#define SCREEN_WIDTH 1280
#define SCREEN_HEIGHT 640


BaseObject g_background;
BaseObject g_ground;

TTF_Font* g_font_text = NULL;
TTF_Font* g_font_MENU = NULL;

bool InitData()
{
    BOOL check = true;
    int ret = SDL_Init(SDL_INIT_VIDEO);
    if (ret < 0) // ret<0 là hàm có lỗi 
        return false;

    SDL_SetHint(SDL_HINT_RENDER_SCALE_QUALITY, "1"); // Thiết lập chế độ tỉ lệ với chất lượng
    //Tạo cửa sổ window
    g_window = SDL_CreateWindow("FlappyBird",
        SDL_WINDOWPOS_UNDEFINED,
        SDL_WINDOWPOS_UNDEFINED,
        SCREEN_WIDTH, SCREEN_HEIGHT,
        SDL_WINDOW_OPENGL);

    if (g_window != NULL) 
    {
        g_screen = SDL_CreateRenderer(g_window, -1, SDL_RENDERER_ACCELERATED);
        if (g_screen != NULL)
        {
            SDL_SetRenderDrawColor(g_screen, RENDER_DRAW_COLOR,
                RENDER_DRAW_COLOR,
                RENDER_DRAW_COLOR,
                RENDER_DRAW_COLOR);
            int imgFlags = IMG_INIT_PNG;
            if (!(IMG_Init(imgFlags) && imgFlags))
                check = false;
        }

        if (TTF_Init() == -1)
        {
            check = false;
        }

        if (Mix_OpenAudio(22050, MIX_DEFAULT_FORMAT, 2, 4096) == -1)
        {
            check = false;
        }

        g_font_text = TTF_OpenFont("font//ARCADE.ttf", 30);
        if (g_font_text == NULL)
        {
            return false;
        }

        g_font_MENU = TTF_OpenFont("font//ARCADE.ttf", 80);
        if (g_font_MENU == NULL)
        {
            return false;
        }
    }

    return check;
}


bool LoadBackground()
{
    bool ret = g_background.LoadImageFile("img//background.png", g_screen);
    return ret;
}
 

void close()
{
    g_background.Free();
    SDL_DestroyRenderer(g_screen);
    g_screen = NULL;

    SDL_DestroyWindow(g_window);
    g_window = NULL;

    IMG_Quit();
    SDL_Quit();
}

int main(int argc, char* argv[])
{
    if (InitData() == false)
    {
        return -1;
    }

    ImpTimer fps;
    bool quit = false;
    
     //Make menu game 
    int ret_menu = SDLCommonFunc::ShowMenu(g_screen, g_font_MENU, "Play Game", "Exit", "img//menu3.png");
    if (ret_menu == 1)
        quit = true; 

    if (!LoadBackground())
    {
        return -1;
    }

    bool ret = g_ground.LoadImageFile("img//ground2.png", g_screen);
    if (ret == false)
    {
        return -1;
    }
    g_ground.SetRect(0, GROUND_MAP);

again_label:

    TextObject text_count_;
    text_count_.setColor(TextObject::WHITE_TEXT);
    TextObject text_time_;
    text_time_.setColor(TextObject::WHITE_TEXT);

    PlayerObject player; 
    ret = player.LoadImg("img//yellowbird1.png", g_screen); // Load PlayerObject lên 
    player.SetRect(100, 100); // Set vị trí cho PlayerObject
    if (ret == false)
    {
        return -1;
    }
    // Tạo block
    BlockManager manage_block;
    ret = manage_block.InitBlockList(g_screen);
    if (ret == false)
        return -1;

    while (!quit)
    {
        fps.start();
        while (SDL_PollEvent(&g_event) != 0)
        {
            if (g_event.type == SDL_QUIT)
            {
                quit = true;
            }

            player.HandleInputAction(g_event, g_screen);
        }
        // Set lại màu cho màn hình
        SDL_SetRenderDrawColor(g_screen, RENDER_DRAW_COLOR,
            RENDER_DRAW_COLOR,
            RENDER_DRAW_COLOR,
            RENDER_DRAW_COLOR);
        // Xóa màn hình để làm mới
        SDL_RenderClear(g_screen); //
         // load backgroud rồi cho vào màn hình
        g_background.Render(g_screen, NULL);

        manage_block.SetPlayerRect(player.GetRect()); // set vị trí playerobject

        bool is_falling = player.GetFalling();
        if (is_falling == true)
        {
            manage_block.SetStopMoving(true);
        }

        manage_block.Render(g_screen);

        bool end_game = manage_block.GetColState();
        if (end_game == true)
        {
            player.SetFalling(true);
        }
        player.DoFalling(g_screen);
        player.Show(g_screen);

        //Vẽ khung chữ nhật
        GeometricFormat rectange_size(0, 0, SCREEN_WIDTH , 32);
        ColorData color_data(36, 36, 36);
        Gemometric::RenderRectange(rectange_size, color_data, g_screen);

       GeometricFormat outlie_size(1, 1, SCREEN_WIDTH - 1, 30);
        ColorData color_data1(255, 255, 255);
        Gemometric::RenderOutline(outlie_size, color_data1, g_screen);
        // Hiện điểm 
        int count = manage_block.GetCount();
        std::string count_str = std::to_string(count);
        std::string score_str("Score:");
        score_str += count_str;
        text_count_.SetText(score_str);
        text_count_.loadFromRenderedText(g_font_text, g_screen);
        text_count_.RenderText(g_screen, SCREEN_WIDTH * 0.4, 2);
       //
        std::string  str_time_("Time:");
        Uint32 time_val = SDL_GetTicks() / 1000;
        std::string str_val = std::to_string(time_val);
        str_time_ += str_val;
        text_time_.SetText(str_time_);
        text_time_.loadFromRenderedText(g_font_text, g_screen);
        text_time_.RenderText(g_screen, SCREEN_WIDTH * 0.7, 2);

        g_ground.Render(g_screen);
        //Update lại màn hình
        SDL_RenderPresent(g_screen);
       

        // Make menu game over
        bool game_over = player.GetIsDie();
        if (game_over == true)
        {
            Sleep(500);
            int ret_menu = SDLCommonFunc::ShowMenu(g_screen, g_font_MENU,
                "Play Again", "Exit",
                "img//menuend3.png");
           
            if (ret_menu == 1)
            {
                

                quit = true;
                continue;
            }
            else
            {
                quit = false;
                manage_block.FreeBlock();
                goto again_label;
            }
            
        } 

        
        int val1 = fps.get_ticks();
        if (fps.get_ticks() < 1000 / FRAMES_PER_SECOND)
        {
            SDL_Delay((1000 / FRAMES_PER_SECOND) - fps.get_ticks());
        }
    }

    close();
    return 0;
}