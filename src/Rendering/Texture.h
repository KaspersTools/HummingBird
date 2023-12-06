//
// Created by Kasper de Bruin on 06/12/2023.
//

#ifndef KBTOOLS_TEXTURE_H
#define KBTOOLS_TEXTURE_H

#include <string>

class Texture {

public:
    Texture(const std::string& path);
    ~Texture();

    void Load();
    void Bind(unsigned int slot = 0) const;
    void Unbind() const;

    inline int GetWidth() const { return m_width; }
    inline int GetHeight() const { return m_height; }

    inline int GetTextureID() const { return m_rendererID; }

private:
    unsigned int m_rendererID;
    std::string m_filePath;
    unsigned char* m_localBuffer;
    int m_width, m_height, m_BPP;

};


#endif //KBTOOLS_TEXTURE_H
