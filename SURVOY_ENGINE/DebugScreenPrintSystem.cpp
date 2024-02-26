#include "DebugScreenPrintSystem.h"

ECS::DebugScreenPrintSystem::DebugScreenPrintSystem(EntityManager& entityManager, EventManager& eventManager):
    _entityManager(entityManager), _eventManager(eventManager), _fontSize(12), _lineHeight(1.5)
{
}

void ECS::DebugScreenPrintSystem::onNotify(const Event& event)
{
}

void ECS::DebugScreenPrintSystem::Load()
{
    FT_Library ft;
    if (FT_Init_FreeType(&ft))
    {
        std::cout << "ERROR::FREETYPE: Could not init FreeType Library" << std::endl;
    }

    FT_Face face;
    if (FT_New_Face(ft, "assets/font/bbterm.ttf", 0, &face))
    {
        std::cout << "ERROR::FREETYPE: Failed to load font" << std::endl;
    }
    else {
        // set size to load glyphs as
        FT_Set_Pixel_Sizes(face, 0, _fontSize);

        // disable byte-alignment restriction
        glPixelStorei(GL_UNPACK_ALIGNMENT, 1);

        ECS::FontCharactersComponent* chars = _entityManager.getComponent<ECS::FontCharactersComponent>(
            _entityManager.getIdByTag("Font")
        );

        if (chars) {
            std::cout << "loading character set 128" << std::endl;
            // load first 128 characters of ASCII set
            for (unsigned char c = 0; c < 128; c++)
            {
                // Load character glyph 
                if (FT_Load_Char(face, c, FT_LOAD_RENDER))
                {
                    std::cout << "ERROR::FREETYTPE: Failed to load Glyph" << std::endl;
                    continue;
                }
                // generate texture
                unsigned int texture;
                glGenTextures(1, &texture);
                glBindTexture(GL_TEXTURE_2D, texture);
                glTexImage2D(
                    GL_TEXTURE_2D,
                    0,
                    GL_RED,
                    face->glyph->bitmap.width,
                    face->glyph->bitmap.rows,
                    0,
                    GL_RED,
                    GL_UNSIGNED_BYTE,
                    face->glyph->bitmap.buffer
                );
                // set texture options
                glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE);
                glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE);
                glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
                glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
                // now store character for later use
                Character character = {
                    texture,
                    glm::ivec2(face->glyph->bitmap.width, face->glyph->bitmap.rows),
                    glm::ivec2(face->glyph->bitmap_left, face->glyph->bitmap_top),
                    static_cast<unsigned int>(face->glyph->advance.x)
                };
                chars->Characters.insert(std::pair<char, Character>(c, character));
            }
            glBindTexture(GL_TEXTURE_2D, 0);
        }
    }
    FT_Done_Face(face);
    FT_Done_FreeType(ft);

    // create the buffers
    ECS::BuffersComponent* buffers = _entityManager.getComponent<ECS::BuffersComponent>(
        _entityManager.getIdByTag("Font")
    );

    if (buffers) {
        std::cout << "loading font buffers" << std::endl;
        glGenVertexArrays(1, &buffers->VAO);
        glGenBuffers(1, &buffers->VBO);
        glBindVertexArray(buffers->VAO);
        glBindBuffer(GL_ARRAY_BUFFER, buffers->VBO);
        glBufferData(GL_ARRAY_BUFFER, sizeof(float) * 6 * 4, NULL, GL_DYNAMIC_DRAW);
        glEnableVertexAttribArray(0);
        glVertexAttribPointer(0, 4, GL_FLOAT, GL_FALSE, 4 * sizeof(float), 0);
        glBindBuffer(GL_ARRAY_BUFFER, 0);
        glBindVertexArray(0);
    }
}

void ECS::DebugScreenPrintSystem::Render()
{
    ECS::DebugPrintComponent* debugPrint = _entityManager.getComponent<ECS::DebugPrintComponent>(
        _entityManager.getIdByTag("DebugPrint")
    );

    float x = _fontSize;
    float y = 768 - _fontSize * _lineHeight;

    for (auto& text : debugPrint->PrintTexts) {
        _renderLine(text.Text, x, y, text.Colour, 1.2);

        y = y - (_fontSize * _lineHeight);
    }

    debugPrint->PrintTexts.clear();
}

void ECS::DebugScreenPrintSystem::UpdateOnFixedTimestep(float deltaTime)
{
    ECS::DebugPrintComponent* debugPrint = _entityManager.getComponent<ECS::DebugPrintComponent>(
        _entityManager.getIdByTag("DebugPrint")
    );

    if (debugPrint) {
        PrintText text;
        text.Text = "FPS: " + std::to_string(deltaTime);
        debugPrint->PrintTexts.push_back(text);
    }
}

void ECS::DebugScreenPrintSystem::Unload()
{
    ECS::BuffersComponent* buffers = _entityManager.getComponent<ECS::BuffersComponent>(
        _entityManager.getIdByTag("Font")
    );

    if (buffers) {
        glDeleteVertexArrays(1, &buffers->VAO);
        glDeleteBuffers(1, &buffers->VBO);
    }
}

void ECS::DebugScreenPrintSystem::_renderLine(std::string text, float x, float y, glm::vec3 colour, float scale)
{
    ECS::ProgramComponent* shader = _entityManager.getComponent<ECS::ProgramComponent>(
        _entityManager.getIdByTag("FontShader")
    );
    ECS::BuffersComponent* buffers = _entityManager.getComponent<ECS::BuffersComponent>(
        _entityManager.getIdByTag("Font")
    );
    ECS::FontCharactersComponent* chars = _entityManager.getComponent<ECS::FontCharactersComponent>(
        _entityManager.getIdByTag("Font")
    );
    ECS::RenderTargetDimensionsComponent* renderTarget = _entityManager.getComponent<ECS::RenderTargetDimensionsComponent>(
        _entityManager.getIdByTag("RenderPipeline")
    );

    if (shader && buffers && renderTarget) {
        shader->Program.use();
        shader->Program.setVec3("textColor", colour);
        glm::mat4 projection = glm::ortho(0.0f, static_cast<float>(renderTarget->ScreenWidth), 0.0f, static_cast<float>(renderTarget->ScreenHeight));
        shader->Program.setMat4("projection", projection);
        glActiveTexture(GL_TEXTURE0);
        glBindVertexArray(buffers->VAO);

        // iterate through all characters
        std::string::const_iterator c;
        for (c = text.begin(); c != text.end(); c++)
        {
            Character ch = chars->Characters[*c];

            float xpos = x + ch.Bearing.x * scale;
            float ypos = y - (ch.Size.y - ch.Bearing.y) * scale;

            float w = ch.Size.x * scale;
            float h = ch.Size.y * scale;
            // update VBO for each character
            float vertices[6][4] = {
                { xpos,     ypos + h,   0.0f, 0.0f },
                { xpos,     ypos,       0.0f, 1.0f },
                { xpos + w, ypos,       1.0f, 1.0f },

                { xpos,     ypos + h,   0.0f, 0.0f },
                { xpos + w, ypos,       1.0f, 1.0f },
                { xpos + w, ypos + h,   1.0f, 0.0f }
            };
            // render glyph texture over quad
            glBindTexture(GL_TEXTURE_2D, ch.TextureID);
            // update content of VBO memory
            glBindBuffer(GL_ARRAY_BUFFER, buffers->VBO);
            glBufferSubData(GL_ARRAY_BUFFER, 0, sizeof(vertices), vertices); // be sure to use glBufferSubData and not glBufferData

            glBindBuffer(GL_ARRAY_BUFFER, 0);
            // render quad
            glDrawArrays(GL_TRIANGLES, 0, 6);
            // now advance cursors for next glyph (note that advance is number of 1/64 pixels)
            x += (ch.Advance >> 6) * scale; // bitshift by 6 to get value in pixels (2^6 = 64 (divide amount of 1/64th pixels by 64 to get amount of pixels))
        }
        glBindVertexArray(0);
        glBindTexture(GL_TEXTURE_2D, 0);
    }
}
