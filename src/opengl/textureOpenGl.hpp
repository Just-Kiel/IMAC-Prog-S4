#pragma once
#include "p6/p6.h"

class Texture {
private:
    GLuint m_id;

public:
    Texture()
    {
        glGenTextures(1, &m_id); // Do whatever you need to create the resource
    }
    ~Texture()
    {
        glDeleteTextures(1, &m_id); // Do whatever you need to delete the resource
    }
    Texture(const Texture&)            = delete; // We disable copying
    Texture& operator=(const Texture&) = delete; // We disable copying
    Texture(Texture&& other) noexcept            // Move constructor
        : m_id{other.m_id}
    {
        other.m_id = 0; // Make sure that rhs won't delete the _id we just copied
    }
    Texture& operator=(Texture&& other) noexcept // Move assignment operator
    {
        if (this != &other)
        {                               // Make sure that we don't do silly things when we try to move an object to itself
            glDeleteTextures(1, &m_id); // Delete the previous object
            m_id       = other.m_id;    // Copy the object
            other.m_id = 0;             // Make sure that rhs won't delete the _id we just copied
        }
        return *this; // move assignment must return a reference to this, so we do it
    }

    GLuint operator*() const { return m_id; } // The getter for the wrapped `_id`. You can also have it as a function called id() but I like to use the dereference operator for that purpose (*my_wrapper).

    void image2D(const int& width, const int& height, const void* data, const GLenum& format, const GLenum& type) const
    {
        glBindTexture(GL_TEXTURE_2D, m_id);
        glTexImage2D(GL_TEXTURE_2D, 0, format, width, height, 0, format, type, data);

        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
        glBindTexture(GL_TEXTURE_2D, 0);
    }

    void cubemap(const std::vector<img::Image>& data, const GLenum& format, const GLenum& type) const
    {
        glBindTexture(GL_TEXTURE_CUBE_MAP, m_id);

        for (unsigned int i = 0; i < 6; ++i)
        {
            glTexImage2D(GL_TEXTURE_CUBE_MAP_POSITIVE_X + i, 0, format, static_cast<int>(data[i].width()), static_cast<int>(data[i].height()), 0, format, type, data[i].data());
        }

        glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
        glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
        glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE);
        glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE);
        glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_WRAP_R, GL_CLAMP_TO_EDGE);

        glBindTexture(GL_TEXTURE_CUBE_MAP, 0);
    }

    void activate(const int& textureUnit) const
    {
        glActiveTexture(GL_TEXTURE0 + textureUnit);
        glBindTexture(GL_TEXTURE_2D, m_id);
    }
};