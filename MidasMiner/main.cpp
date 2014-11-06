
#include "View.h"

#include <SDL.h>
#include <iostream>
 
int main(int argc, char* argv[])
{
    try 
    {
        View v;
        v.init();
        v.execute();
    } 
    catch (const std::runtime_error &e) 
    {
        std::cout << e.what() << std::endl;
        return -1;
    }
    return 0;
}