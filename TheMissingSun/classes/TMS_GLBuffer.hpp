#ifndef TMS_GLBUFFER_HPP
#define TMS_GLBUFFER_HPP

#include "../include/glad/glad.h"

/* Wrapper for an OpenGL buffer containing data of type T. */
template <typename T>
class TMS_GLBuffer
{
public:
    TMS_GLBuffer(const GLenum target, const GLenum usage = GL_STATIC_DRAW);
    TMS_GLBuffer(const GLenum target, const T data[], const size_t elemNum, const GLenum usage = GL_STATIC_DRAW);
    ~TMS_GLBuffer();
    TMS_GLBuffer(const TMS_GLBuffer& otherBuffer) = delete;
    TMS_GLBuffer& operator= (const TMS_GLBuffer& otherBuffer) = delete;
    TMS_GLBuffer(TMS_GLBuffer&& otherBuffer);
    TMS_GLBuffer& operator= (TMS_GLBuffer&& otherBuffer);

    /***************** METHODS *****************/
    void bind() const; // Bind the buffer to the stored target.
    void setData(const T data[], const size_t elemNum); // Set the buffer's data.
    void setTarget(const GLenum target); // Set the binding target.

private:
    GLuint _id; // Buffer ID.
    size_t _size; // Capacity of the buffer.
    GLenum _target; // Buffer binding target.
    GLenum _usage; // Usage of the buffer (static/dynamic draw).
};

/***************** DEFINITIONS *****************/
template <typename T>
inline TMS_GLBuffer<T>::TMS_GLBuffer(const GLenum target, const GLenum usage) :
    _size(0),
    _target(target),
    _usage(usage)
{
    glGenBuffers(1, &_id);
}

template <typename T>
inline TMS_GLBuffer<T>::TMS_GLBuffer(const GLenum target, const T data[], const size_t elemNum, const GLenum usage) :
    _target(target),
    _usage(usage)
{
    _size = elemNum * sizeof(T); // Store the size of the data,
    /* Generate the buffer and store the data. */
    glGenBuffers(1, &_id);
    glBindBuffer(_target, _id);
    glBufferData(_target, _size, data, _usage);
    glBindBuffer(_target, 0);
}

template <typename T>
inline TMS_GLBuffer<T>::~TMS_GLBuffer()
{
    glDeleteBuffers(1, &_id);
}

template <typename T>
inline TMS_GLBuffer<T>::TMS_GLBuffer(TMS_GLBuffer&& otherBuffer) :
    _size(otherBuffer._size),
    _target(otherBuffer._target),
    _usage(otherBuffer._usage)
{
    _id = otherBuffer._id;
    otherBuffer._id = 0;
}

template <typename T>
inline TMS_GLBuffer<T>& TMS_GLBuffer<T>::operator=(TMS_GLBuffer&& otherBuffer)
{
    if (otherBuffer != *this)
    {
        glDeleteBuffers(1, &_id);
        _id = otherBuffer._id;
        otherBuffer._id = 0;

        _size = otherBuffer._size;
        _target = otherBuffer._target;
        _usage = otherBuffer._usage;
    }
    return *this;
}

template <typename T>
inline void TMS_GLBuffer<T>::bind() const
{
    glBindBuffer(_target, _id);
}

template <typename T>
inline void TMS_GLBuffer<T>::setData(const T data[], const size_t elemNum)
{
    glBindBuffer(_target, _id);
    /* Store the new data. Reallocate the buffer if necessary. */
    size_t dataSize = elemNum * sizeof(T);
    if (dataSize > _size)
    {
        _size = dataSize;
        glBufferData(_target, _size, data, _usage);
    }
    else glBufferSubData(_target, 0, dataSize, data);

    glBindBuffer(_target, 0);
}

template <typename T>
inline void TMS_GLBuffer<T>::setTarget(const GLenum target)
{
    _target = target;
}

#endif