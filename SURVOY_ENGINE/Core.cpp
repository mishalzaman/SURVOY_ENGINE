#include "Core.h"
#include "Logger.h"


/*==============================================
CORE
==============================================*/

BAE::Core::Core() :
    _window(nullptr),
    _context(NULL),
    _error(1),
    _screenW(0),
    _screenH(0),
    _title(""),
    _quit(false)
{

}

BAE::Core::~Core() {
    if (_window != nullptr) {
        LOG_INFO("Destroy _window");
        SDL_DestroyWindow(_window);
    }

    if (_context) {
        LOG_INFO("Destroy _context");
        SDL_GL_DeleteContext(_context);
    }

    LOG_INFO("Shutting down BAE");
    LOG_INFO("------END--------");
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

    LOG_INFO("------BEGIN-------");
    LOG_INFO("Initializing Core");
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

    LOG_ERROR("EndRender: Could not find _window");
    throw std::runtime_error("Window was not found during game loop");
}

/*==============================================
INITIALIZATIONS
==============================================*/

bool BAE::Core::_initSDL()
{
    LOG_INFO("Initializing SDL2");
    if (SDL_Init(SDL_INIT_VIDEO) < 0) {
        _error = Code::CORE_SDL;
        return false;
    }

    return true;
}

bool BAE::Core::_initOpengGL()
{
    LOG_INFO("Initializing OpenGL context");
    SDL_GL_SetAttribute(SDL_GL_CONTEXT_PROFILE_MASK, SDL_GL_CONTEXT_PROFILE_CORE);
    SDL_GL_SetAttribute(SDL_GL_CONTEXT_MAJOR_VERSION, 3);
    SDL_GL_SetAttribute(SDL_GL_CONTEXT_MINOR_VERSION, 3);

    return true;
}

bool BAE::Core::_createWindow()
{
    LOG_INFO("Initializing _window");
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
    LOG_INFO("Initializing SDL context");
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
    LOG_INFO("Initializing GLEW");
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
    LOG_INFO("Initializing OpenGL settings");
    glEnable(GL_BLEND);
    glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);

    glViewport(0, 0, _screenW, _screenH);
}

void BAE::Core::_initializeSubSystems()
{
    LOG_INFO("Initializing Sub-systems");
    Scene = std::make_unique<BAE::Scene>(_screenW, _screenH);
    ShaderLibrary = std::make_unique<BAE::ShaderLibrary>(_screenW, _screenH);
    TextureLibrary = std::make_unique<BAE::TextureLibrary>();
    Timer = std::make_unique<BAE::Timer>(16.6667);
    Event = std::make_unique<BAE::Event>();
}
