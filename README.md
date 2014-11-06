
# BUILDING

Visual Studio Professional 2012 has been used to develop the application.

Following resources are used as reference libraries:

    - http://www.libsdl.org/download-2.0.php
    - http://www.libsdl.org/projects/SDL_image/
    - http://www.libsdl.org/projects/SDL_ttf/

Following unpacked packages downloaded from the URLs above has been located
next to top level MidasMiner directory

    - SDL2_image-2.0.0
    - SDL2_ttf-2.0.12
    - SDL2-2.0.1

# KNOWN ISSUES & STYLE

The application is developed in agile style.  It's minimally complete to cover
the specified initial requirements.  If new requirements are added the project
may require refactoring.

Emphasis is put more on the code structure, organization, on illustrating how
certain features can potentially be implemented in minimal time (about 15 hours
or so).  In most of other situations the code should have been developed in
TDD style to achieve code with a higher quality.

Due to missing working experience with SDL, naive solutions are provided.
Whereas in production code the given solutions should be more elaborately
thought and considered.

The functionality doesn't include animations even though the falling down
of the new objects is implemented (performed after swap).
