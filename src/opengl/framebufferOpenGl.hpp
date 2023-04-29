#pragma once
#include <array>
#include "p6/p6.h"
#include "textureOpenGl.hpp"

class Framebuffer {
private:
    GLuint m_id;

    Texture m_depth_buffer{};

    int m_width;
    int m_height;

public:
    Framebuffer(const int& width, const int& height)
        : m_width{width * 2}, m_height{height * 2}
    {
        glGenFramebuffers(1, &m_id); // Do whatever you need to create the resource

        glBindFramebuffer(GL_FRAMEBUFFER, m_id);

        glBindTexture(GL_TEXTURE_2D, *m_depth_buffer);
        glTexImage2D(GL_TEXTURE_2D, 0, GL_DEPTH_COMPONENT16, m_width, m_height, 0, GL_DEPTH_COMPONENT, GL_FLOAT, nullptr);
        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_NEAREST);
        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST);
        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE);
        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE);

        std::array<float, 4> borderColor = {1.0f, 1.0f, 1.0f, 1.0f};
        glTexParameterfv(GL_TEXTURE_2D, GL_TEXTURE_BORDER_COLOR, borderColor.data());

        glFramebufferTexture(GL_FRAMEBUFFER, GL_DEPTH_ATTACHMENT, *m_depth_buffer, 0);

        glDrawBuffer(GL_NONE); // No color buffer is drawn to.

        // Always check that our framebuffer is ok
        if (glCheckFramebufferStatus(GL_FRAMEBUFFER) != GL_FRAMEBUFFER_COMPLETE)
            assert(false);
    }
    ~Framebuffer()
    {
        glDeleteFramebuffers(1, &m_id); // Do whatever you need to delete the resource
    }
    Framebuffer(const Framebuffer&)            = delete; // We disable copying
    Framebuffer& operator=(const Framebuffer&) = delete; // We disable copying
    Framebuffer(Framebuffer&& other) noexcept            // Move constructor
        : m_id{other.m_id}
    {
        other.m_id = 0; // Make sure that rhs won't delete the _id we just copied
    }
    Framebuffer& operator=(Framebuffer&& other) noexcept // Move assignment operator
    {
        if (this != &other)
        {                                   // Make sure that we don't do silly things when we try to move an object to itself
            glDeleteFramebuffers(1, &m_id); // Delete the previous object
            m_id       = other.m_id;        // Copy the object
            other.m_id = 0;                 // Make sure that rhs won't delete the _id we just copied
        }
        return *this; // move assignment must return a reference to this, so we do it
    }

    GLuint operator*() const { return m_id; } // The getter for the wrapped `_id`. You can also have it as a function called id() but I like to use the dereference operator for that purpose (*my_wrapper).

    int    getWidth() const { return m_width; }
    int    getHeight() const { return m_height; }
    GLuint getTextureId() const { return *m_depth_buffer; }
};