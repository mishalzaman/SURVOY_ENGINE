#include "Core.h"


/*==============================================
CORE
==============================================*/

BAE::Core::Core() :
    _window(nullptr),
    _context(NULL),
    _error(1),
    _title(""),
    _quit(false)
{

}

BAE::Core::~Core() {
    if (_window != nullptr) {
        //LOG_INFO("Destroy _window");
        SDL_DestroyWindow(_window);
    }

    if (_context) {
        SDL_GL_DeleteContext(_context);
    }

    SDL_Quit();
}


bool BAE::Core::CreateDevice(
    const char* title
)
{
    _title = title;

    if (
        !_initSDL() ||
        !_initOpengGL() ||
        !_createWindow() ||
        !_createContext() ||
        !_initGlew()
        ) {
        return false;
    }

    _openGLSettings();
    _initializeSubSystems();

    return true;
}

void BAE::Core::DestroyDevice()
{
}

void BAE::Core::BeginRender()
{
    // Clear the screen
    glClearColor(0.5f, 0.5f, 0.8f, 1.0f);
    glClear(GL_COLOR_BUFFER_BIT);
    glClear(GL_DEPTH_BUFFER_BIT);
}

void BAE::Core::EndRender()
{
    // Swap buffers
    if (_window) {
        SDL_GL_SwapWindow(_window);
        return;
    }

    throw std::runtime_error("Window was not found during game loop");
}

void BAE::Core::BeginShutdown()
{
    std::cout << "beginning shutdown" << std::endl;
    _quit = true;
}

/*==============================================
WINDOW
==============================================*/

void BAE::Core::_resizeViewport(const int nWidth, const int nHeight)
{
    int setNewW = 0;
    int setNewH = 0;
    int offsetX = 0;
    int offsetY = 0;

    // Calculate the new dimensions while maintaining the 4:3 ratio
    if (nWidth * 3 > nHeight * 4) {
        setNewW = nHeight * 4 / 3;
        setNewH = nHeight;
        offsetX = (nWidth - setNewW) / 2;
    }
    else {
        setNewW = nWidth;
        setNewH = nWidth * 3 / 4;
        offsetY = (nHeight - setNewH) / 2;
    }

    glViewport(offsetX, offsetY, setNewW, setNewH);
}

/*==============================================
INITIALIZATIONS
==============================================*/

bool BAE::Core::_initSDL()
{
    if (SDL_Init(SDL_INIT_VIDEO) < 0) {
        _error = Code::CORE_SDL;
        return false;
    }

    return true;
}

bool BAE::Core::_initOpengGL()
{
    SDL_GL_SetAttribute(SDL_GL_CONTEXT_PROFILE_MASK, SDL_GL_CONTEXT_PROFILE_CORE);
    SDL_GL_SetAttribute(SDL_GL_CONTEXT_MAJOR_VERSION, 3);
    SDL_GL_SetAttribute(SDL_GL_CONTEXT_MINOR_VERSION, 3);

    return true;
}

bool BAE::Core::_createWindow()
{
    _window = SDL_CreateWindow(_title, SDL_WINDOWPOS_UNDEFINED, SDL_WINDOWPOS_UNDEFINED, Defaults::BASE_SCREEN_WIDTH, Defaults::BASE_SCREEN_HEIGHT, SDL_WINDOW_OPENGL | SDL_WINDOW_RESIZABLE);
    if (!_window) {
        SDL_Quit();
        _error = Code::CORE_WINDOW;
        return false;
    }

    return true;
}

bool BAE::Core::_createContext()
{
    _context = SDL_GL_CreateContext(_window);
    if (!_context) {
        SDL_DestroyWindow(_window);
        SDL_Quit();
        _error = Code::CORE_CONTEXT;
        return false;
    }

    return true;
}

bool BAE::Core::_initGlew()
{
    glewExperimental = GL_TRUE;
    if (glewInit() != GLEW_OK) {
        SDL_DestroyWindow(_window);
        SDL_Quit();
        _error = Code::CORE_GLEW;
        return false;
    }

    return true;
}

void BAE::Core::_openGLSettings()
{
    glEnable(GL_BLEND);
    glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);

    //glPolygonMode(GL_FRONT_AND_BACK, GL_LINE);

    glEnable(GL_DEPTH_TEST);
    glEnable(GL_CULL_FACE);
    glCullFace(GL_BACK);
    glFrontFace(GL_CCW);
}

void BAE::Core::_initializeSubSystems()
{
    Timer = std::make_unique<BAE::Timer>(16.6667);
}
