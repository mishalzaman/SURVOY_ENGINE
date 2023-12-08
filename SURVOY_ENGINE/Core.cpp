#include "Core.h"

/*==============================================
CORE
==============================================*/

BAE::Core::Core() :
    _window(nullptr),
    _context(NULL),
    _error(1)
{

}

BAE::Core::~Core() {
    if (_window != nullptr) {
        std::cout << "destroy _window" << std::endl;
        SDL_DestroyWindow(_window);
    }

    std::cout << "destroy _context" << std::endl;
    SDL_GL_DeleteContext(_context);

    std::cout << "BAE quit!" << std::endl;
    SDL_Quit();
}


bool BAE::Core::CreateDevice(
    int width,
    int height,
    const char* title
)
{
    _screenW = width;
    _screenH = height;
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

    _openglOptions();
    _initializeSubSystems();

    return true;
}

void BAE::Core::DestroyDevice()
{
}

void BAE::Core::BeginRender()
{
    // Clear the screen
    glClearColor(0.0f, 0.0f, 0.0f, 1.0f);
    glClear(GL_COLOR_BUFFER_BIT);
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

/*==============================================
INITIALIZATIONS
==============================================*/

bool BAE::Core::_initSDL()
{
    std::cout << "initialize SDL" << std::endl;
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
    std::cout << "initialize _window" << std::endl;
    _window = SDL_CreateWindow(_title, SDL_WINDOWPOS_UNDEFINED, SDL_WINDOWPOS_UNDEFINED, _screenW, _screenH, SDL_WINDOW_OPENGL);
    if (!_window) {
        SDL_Quit();
        _error = Code::CORE_WINDOW;
        return false;
    }

    return true;
}

bool BAE::Core::_createContext()
{
    std::cout << "initialize _context" << std::endl;
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
    std::cout << "initialize _glew" << std::endl;
    glewExperimental = GL_TRUE;
    if (glewInit() != GLEW_OK) {
        SDL_DestroyWindow(_window);
        SDL_Quit();
        _error = Code::CORE_GLEW;
        return false;
    }

    return true;
}

void BAE::Core::_openglOptions()
{
    glEnable(GL_BLEND);
    glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);

    glViewport(0, 0, _screenW, _screenH);
}

void BAE::Core::_initializeSubSystems()
{
    Scene = std::make_unique<BAE::Scene>(_screenW, _screenH);
    ShaderLibrary = std::make_unique<BAE::ShaderLibrary>(_screenW, _screenH);
    TextureLibrary = std::make_unique<BAE::TextureLibrary>();
    Timer = std::make_unique<BAE::Timer>(16.6667);
}


/*==============================================
FONT
==============================================*/

