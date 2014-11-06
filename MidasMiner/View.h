
#ifndef VIEW_H
#define VIEW_H

#include "Control.h"

#include <SDL_events.h>
#include <SDL_timer.h>
#include <SDL_ttf.h>

struct SDL_Texture;

/// representing view of the application
class View
{
private:
    const size_t m_windowWidth;
    const size_t m_windowHeigth;
    const size_t m_objectWidth;
    const size_t m_objectHeigth;

private:
    const size_t m_xStartOffset;
    const size_t m_yStartOffset;

public:
    /// initializes the view (loads libs, etc.)
    void init();

    /// starts the execution loop
    void execute();

    /// construction & destruction
    View();
    ~View();

    /// updates the texture to be rendered during execution loop for timer
    /// changes running state to completed if time has been elapsed
    void updateTimeLeft();

private:
    void createTextures();
    void renderTexture(Matrix::index_t, Matrix::index_t, Matrix::color);
    bool clickedOnBoard(size_t x, size_t y);
    Matrix::index_t getRow(size_t x);
    Matrix::index_t getColumn(size_t y);
    void renderWindow();
    void onEvent(SDL_Event*);
    void performSwapAndDeductionIfNeeded();

private:
    SDL_Texture* m_background;
    SDL_Texture* m_blue;
    SDL_Texture* m_green;
    SDL_Texture* m_purple;
    SDL_Texture* m_red;
    SDL_Texture* m_yellow;
    SDL_Texture* m_time;
    TTF_Font* m_font;
    Control m_control;
    bool m_running;
    bool m_firstObjectIsSelected;
    Matrix::index_t m_firstObjectRow;
    Matrix::index_t m_firstObjectColumn;
    Matrix::index_t m_secondObjectRow;
    Matrix::index_t m_secondObjectColumn;
    SDL_TimerID m_timerId;
    size_t m_timeLeft;

    /// deleting copying
private:
    View(const View&);
    View& operator=(const View&);
};

#endif // VIEW_H