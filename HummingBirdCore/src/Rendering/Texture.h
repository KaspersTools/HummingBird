//
// Created by Kasper de Bruin on 06/12/2023.
//

#pragma once

#include <PCH/pch.h>

class Texture {

public:
    explicit Texture(const std::string& path);
    ~Texture();

    void load();
    void bind(unsigned int slot = 0) const;
    void unbind() const;

    inline int getWidth() const { return m_width; }
    inline int getHeight() const { return m_height; }

    inline int getTextureID() const { return m_rendererID; }

private:
    unsigned int   m_rendererID;
    std::string    m_filePath;
    unsigned char* m_localBuffer;
    int m_width,   m_height, m_BPP;

};
