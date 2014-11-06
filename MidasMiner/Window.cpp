
#include "Window.h"

#include <SDL.h>
#include <SDL_image.h>
#include <stdexcept>
#include <cassert>

//Initialize the unique_ptr's deleters here
std::unique_ptr<SDL_Window, void (*)(SDL_Window*)> Window::m_window 
    = std::unique_ptr<SDL_Window, void (*)(SDL_Window*)>(nullptr, SDL_DestroyWindow);

std::unique_ptr<SDL_Renderer, void (*)(SDL_Renderer*)> Window::m_renderer
    = std::unique_ptr<SDL_Renderer, void (*)(SDL_Renderer*)>(nullptr, SDL_DestroyRenderer);

SDL_Rect Window::m_box;

void Window::init(std::string title, size_t width, size_t height)
{
    //initialize all SDL subsystems
    if (SDL_Init(SDL_INIT_EVERYTHING) == -1) {
        throw std::runtime_error("SDL Init Failed");
    }
    size_t image_flags = IMG_INIT_PNG || IMG_INIT_JPG;
    if ((IMG_Init(image_flags) & image_flags) != image_flags) {
        throw std::runtime_error("IMG Init Failed");
    }
    if (TTF_Init() == -1) {
        throw std::runtime_error("TTF Init Failed");
    }
 
    //Setup our window size
    m_box.x = 0;
    m_box.y = 0;
    m_box.w = width;
    m_box.h = height;
    //Create our window
    m_window.reset(SDL_CreateWindow(title.c_str(), SDL_WINDOWPOS_CENTERED, 
        SDL_WINDOWPOS_CENTERED, m_box.w, m_box.h, SDL_WINDOW_SHOWN));
    //Make sure it created ok
    if (m_window == nullptr) {
        throw std::runtime_error("Failed to create window");
    }
 
    //Create the renderer
    m_renderer.reset(SDL_CreateRenderer(m_window.get(), -1, 
        SDL_RENDERER_ACCELERATED | SDL_RENDERER_PRESENTVSYNC));
    //Make sure it created ok
    if (m_renderer == nullptr) {
        throw std::runtime_error("Failed to create renderer");
    }
}

void Window::quit()
{
    IMG_Quit();
    SDL_Quit();
    TTF_Quit();
}

void Window::renderTexture(SDL_Texture *tex, int x, int y, int w, int h)
{
    assert(nullptr != tex);
    assert(m_renderer.get() != nullptr);
    SDL_Rect dst;
    dst.x = x;
    dst.y = y;
    dst.w = w;
    dst.h = h;
    SDL_RenderCopy(m_renderer.get(), tex, nullptr, &dst);
}

void Window::renderTexture(SDL_Texture *tex, int x, int y)
{
    int w, h;
    assert(nullptr != tex);
    SDL_QueryTexture(tex, NULL, NULL, &w, &h);
    Window::renderTexture(tex, x, y, w, h);
}

SDL_Texture* Window::loadImage(const std::string& file)
{
    assert(m_renderer.get() != nullptr);
    SDL_Texture *texture = IMG_LoadTexture(m_renderer.get(), file.c_str());
    if (texture == nullptr) {
        throw std::runtime_error("Failed to load texture: " + file);
    }
    return texture;
}

TTF_Font* Window::loadFont(const std::string& file, int fontSize)
{
    TTF_Font *font = nullptr;
    font = TTF_OpenFont(file.c_str(), fontSize);
    if (font == nullptr) {
        throw std::runtime_error("Failed to load font: " + file + TTF_GetError());
    }
    return font;
}

SDL_Texture* Window::getTexture(const std::string& message, TTF_Font* font, SDL_Color color)
{
    assert(nullptr != font);
    SDL_Surface *surf = TTF_RenderText_Blended(font, message.c_str(), color);
    assert(nullptr != surf);
    assert(m_renderer.get() != nullptr);
    SDL_Texture *texture = SDL_CreateTextureFromSurface(m_renderer.get(), surf);
    if (texture == nullptr) {
        throw std::runtime_error("Failed to load texture from font");
    }
    SDL_FreeSurface(surf);
    return texture;
}

void Window::clear()
{
    assert(m_renderer.get() != nullptr);
    SDL_RenderClear(m_renderer.get());
}

void Window::present()
{
    assert(m_renderer.get() != nullptr);
    SDL_RenderPresent(m_renderer.get());
}

SDL_Rect Window::box()
{
    SDL_GetWindowSize(m_window.get(), &m_box.w, &m_box.h);
    return m_box;
}
