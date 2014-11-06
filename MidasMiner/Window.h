
#ifndef WINDOW_H
#define WINDOW_H

#include <SDL_ttf.h>
#include <string>
#include <memory>

struct SDL_Texture;
struct SDL_Color;
struct SDL_Rect;
struct SDL_Window;
struct SDL_Renderer;
 
class Window {
public:
    //Start SDL and TTF, create the window and renderer
    static void init(std::string title, size_t width, size_t height);

    //Quit SDL and TTF
    static void quit();

    /**
    * Draw an SDL_Texture to an SDL_Renderer at position x, y, with some desired
    * width and height
    * @param tex The source texture we want to draw
    * @param x The x coordinate to draw too
    * @param y The y coordinate to draw too
    * @param w The width of the texture to draw
    * @param h The height of the texture to draw
    */
    static void renderTexture(SDL_Texture *tex, int x, int y, int w, int h);

    /**
    * Draw an SDL_Texture to an SDL_Renderer at position x, y, preserving
    * the texture's width and height
    * @param tex The source texture we want to draw
    * @param x The x coordinate to draw too
    * @param y The y coordinate to draw too
    */
    static void renderTexture(SDL_Texture *tex, int x, int y);

    /// Load image from specified URL
    /// @throw std::runtime_error of fails to open the specified image
    static SDL_Texture* loadImage(const std::string& file);

    /// loads font from specified font and size
    /// @throw std::runtime_error of fails to open the specified file
    static TTF_Font* loadFont(const std::string& file, int fontSize);

    /// @brief returns texture for specified message
    /// @pre font != nullptr
    static SDL_Texture* getTexture(const std::string& message, TTF_Font* font, SDL_Color color);

    //Clear window
    static void clear();

    //Present renderer
    static void present();

    //Get the window's box
    static SDL_Rect box();
 
private:
    static std::unique_ptr<SDL_Window, void (*)(SDL_Window*)> m_window;
    static std::unique_ptr<SDL_Renderer, void (*)(SDL_Renderer*)> m_renderer;
    static SDL_Rect m_box;
};
 
#endif // WINDOW_H

