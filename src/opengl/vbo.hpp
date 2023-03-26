#pragma once
#include "p6/p6.h"

class VBO {
private:
    GLuint m_id;

public:
    VBO()
    {
        glGenBuffers(1, &m_id); // Do whatever you need to create the resource
    }
    ~VBO()
    {
        glDeleteBuffers(1, &m_id); // Do whatever you need to delete the resource
    }
    VBO(const VBO&)            = delete; // We disable copying
    VBO& operator=(const VBO&) = delete; // We disable copying
    VBO(VBO&& other)
    noexcept // Move constructor
        : m_id{other.m_id}
    {
        other.m_id = 0; // Make sure that rhs won't delete the _id we just copied
    }
    VBO& operator=(VBO&& other) noexcept // Move assignment operator
    {
        if (this != &other)
        {                              // Make sure that we don't do silly things when we try to move an object to itself
            glDeleteBuffers(1, &m_id); // Delete the previous object
            m_id       = other.m_id;   // Copy the object
            other.m_id = 0;            // Make sure that rhs won't delete the _id we just copied
        }
        return *this; // move assignment must return a reference to this, so we do it
    }

    GLuint operator*() const { return m_id; } // The getter for the wrapped `_id`. You can also have it as a function called id() but I like to use the dereference operator for that purpose (*my_wrapper).
};