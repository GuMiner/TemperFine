#pragma once
#include <map>
#include <GL/glew.h>
#include <stb/stb_truetype.h>
#include <stb/stb_image.h>
#include <stb/stb_image_write.h>
#include "ShaderManager.h"
#include "TextInfo.h"
#include "Vertex.h"
#include "vmath.hpp"

// Manages the in-game font. *Note that this only supports a single font.*
class FontManager
{
    // Maps characters to the TextInfo representing each character,
    std::map<int, TextInfo> fontData;

    // Holds the font texture that is filled as necessary. This is bound to GL_TEXTURE0 for now, but probably should have it's own binding point.
    GLuint fontTexture;
    int width;
    int height;

    int usedWidth;
    int usedHeight;
    int lastMaxHeight;

    // Holds our font shader information.
    GLuint fontShader;
    GLint projLocation, mvLocation;
	GLint fontImageLocation;

    // Holds STB font info for loading in new font data as necessary
    stbtt_fontinfo fontInfo;
    unsigned char *loadedFontFile;

    const int verticesPerChar = 4;
    void AddToFontTexture(CharInfo& charInfo);
    CharInfo& GetCharacterInfo(int fontPixelHeight, int character);

    // Sentence information
    int nextSentenceId;
    std::map<int, SentenceInfo> sentences;

    int GetSentenceVertexCount(std::string& sentence);
    universalVertices AllocateSentenceVertices(std::string& sentence, int pixelHeight, vmath::vec3 textColor);

public:
    FontManager();
    bool LoadFont(ShaderManager* shaderManager, const char *fontName);

    int CreateNewSentence();
    void UpdateSentence(int sentenceId, std::string& sentence, int pixelHeight, vmath::vec3 textColor);
    void RenderSentence(int sentenceId, vmath::mat4& perpective, vmath::mat4& mvMatrix);

    ~FontManager();
};

