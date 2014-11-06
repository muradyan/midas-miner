
#include "View.h"
#include "Window.h"

#include <SDL.h>
#include <cassert>
#include <cstdint>
#include <sstream>
#include <functional>
#include <iomanip>

View::View()
    : m_windowWidth(755)
    , m_windowHeigth(600)
    , m_objectWidth(44)
    , m_objectHeigth(44)
    , m_xStartOffset(325)
    , m_yStartOffset(100)
    , m_background(nullptr)
    , m_blue(nullptr)
    , m_green(nullptr)
    , m_purple(nullptr)
    , m_red(nullptr)
    , m_yellow(nullptr)
    , m_time(nullptr)
    , m_font(nullptr)
    , m_control()
    , m_running(false)
    , m_firstObjectIsSelected(false)
    , m_firstObjectRow(0)
    , m_firstObjectColumn(0)
    , m_secondObjectRow(0)
    , m_secondObjectColumn(0)
    , m_timerId(0)
    , m_timeLeft(Control::gameDuration)
{
}

void View::createTextures()
{
    assert(m_background == nullptr);
    m_background = Window::loadImage("../Resources/Background.jpg");
    assert(m_background != nullptr);
    assert(m_blue == nullptr);
    m_blue = Window::loadImage("../Resources/Blue.png");
    assert(m_blue != nullptr);
    assert(m_green == nullptr);
    m_green = Window::loadImage("../Resources/Green.png");
    assert(m_green != nullptr);
    assert(m_purple == nullptr);
    m_purple = Window::loadImage("../Resources/Purple.png");
    assert(m_purple != nullptr);
    assert(m_red == nullptr);
    m_red = Window::loadImage("../Resources/Red.png");
    assert(m_red != nullptr);
    assert(m_yellow == nullptr);
    m_yellow = Window::loadImage("../Resources/Yellow.png");
    assert(m_yellow != nullptr);
    m_font = Window::loadFont("../Resources/source-sans-pro.regular.ttf", 70);
    assert(m_font != nullptr);
}

namespace {
    uint32_t ticker(uint32_t interval, void* param)
    {
        assert(nullptr != param);
        View* v = static_cast<View*>(param);
        assert(nullptr != v);
        v->updateTimeLeft();
        return interval;
    }
}

void View::updateTimeLeft()
{
    std::stringstream s;
    s << std::setfill('0') << std::setw(2) << m_timeLeft;
    SDL_Color color;
    color.r = 255;
    color.b = 255;
    color.g = 39;
    assert(nullptr != m_font);
    if (m_time != nullptr) {
        SDL_DestroyTexture(m_time);
    }
    m_time = Window::getTexture(s.str(), m_font, color);
    if (m_timeLeft-- == 0) { 
        m_running = false;
    }
}

void View::init()
{
    Window::init("Midas Miner", m_windowWidth, m_windowHeigth);
    createTextures();
    m_timerId = SDL_AddTimer(1000, ticker, this);
}

void View::renderWindow()
{
    assert(nullptr != m_background);    
    Window::renderTexture(m_background, 0, 0);
    if (m_time != nullptr) {
        Window::renderTexture(m_time, 80, 420);
    }
    const Matrix& matrix = m_control.getMatrix();
    for (Matrix::index_t i = 0; i < matrix.numberOfRows; ++i) {
        for (Matrix::index_t j = 0; j < matrix.numberOfColumns; ++j) {
            renderTexture(i, j, matrix.element(i, j));
        }
    }
}

bool View::clickedOnBoard(size_t x, size_t y)
{
    return (x > m_xStartOffset) && (x < (m_xStartOffset + Matrix::numberOfRows * m_objectWidth))
        && (y > m_yStartOffset) && (y < (m_yStartOffset + Matrix::numberOfColumns * m_objectHeigth));
}

Matrix::index_t View::getRow(size_t y)
{
    return (y - m_yStartOffset) / m_objectHeigth;
}

Matrix::index_t View::getColumn(size_t x)    
{
    return (x - m_xStartOffset) / m_objectWidth; 
}

void View::onEvent(SDL_Event* e)
{
    assert(nullptr != e);
    if (e->type == SDL_MOUSEBUTTONDOWN) {
        if (clickedOnBoard(e->button.x, e->button.y)) {
            if (m_firstObjectIsSelected) {
                m_secondObjectRow = getRow(e->button.y);
                m_secondObjectColumn = getColumn(e->button.x);
                performSwapAndDeductionIfNeeded();
            } else {
                m_firstObjectRow = getRow(e->button.y);
                m_firstObjectColumn = getColumn(e->button.x);
            }
            m_firstObjectIsSelected = !m_firstObjectIsSelected;
        }
    } else if (e->type == SDL_QUIT) {
        m_running = false;
    }
}

void View::performSwapAndDeductionIfNeeded()
{
    bool b = m_control.trySwap(m_firstObjectRow, m_firstObjectColumn, 
        m_secondObjectRow, m_secondObjectColumn);
    if (b) {
        while (m_control.deduct());
    }
}
    

/// @todo if events didn't cause any model state change then extra render call is not required
void View::execute()
{
    m_running = true;
    m_control.init();
    SDL_Event event;
    while(m_running) {
        while(SDL_PollEvent(&event)) {
            onEvent(&event);
        }
        renderWindow();
        Window::present();
    }
}

void View::renderTexture(Matrix::index_t i, Matrix::index_t j, Matrix::color c)
{
    size_t x = j * m_objectWidth + m_xStartOffset;
    size_t y = i * m_objectHeigth + m_yStartOffset;
    switch(c)
    {
    case Matrix::blue: 
        assert(nullptr != m_blue);
        Window::renderTexture(m_blue, x, y);
        break;
    case Matrix::green: 
        assert(nullptr != m_green);
        Window::renderTexture(m_green, x, y);
        break;
    case Matrix::purple: 
        assert(nullptr != m_purple);
        Window::renderTexture(m_purple, x, y);
        break;
    case Matrix::red: 
        assert(nullptr != m_red);
        Window::renderTexture(m_red, x, y);
        break;
    case Matrix::yellow: 
        assert(nullptr != m_yellow);
        Window::renderTexture(m_yellow, x, y);
        break;
    case Matrix::max_color: 
        assert(false); 
    }
}

View::~View()
{
    assert(m_background != nullptr);
    SDL_DestroyTexture(m_background);
    assert(m_blue != nullptr);
    SDL_DestroyTexture(m_blue);
    assert(m_yellow != nullptr);
    SDL_DestroyTexture(m_yellow);
    assert(m_green != nullptr);
    SDL_DestroyTexture(m_green);
    assert(m_red != nullptr);
    SDL_DestroyTexture(m_red);
    assert(m_purple != nullptr);
    SDL_DestroyTexture(m_purple);
    assert(nullptr != m_font);
    TTF_CloseFont(m_font);
    if (m_time != nullptr) {
        SDL_DestroyTexture(m_time);
    }
    SDL_RemoveTimer(m_timerId);
    Window::quit();
}
