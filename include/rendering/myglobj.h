#ifndef MYGLOBJ_H
#define MYGLOBJ_H

#include <QOpenGLFunctions_3_3_Core>

class MyGLObj
{
public:
    MyGLObj(QOpenGLFunctions_3_3_Core* glfuns);
    ~MyGLObj();

    void addVBO(GLsizeiptr size, const GLvoid *data, GLenum usage);
    void addEBO(GLsizeiptr size, const GLvoid *data, GLenum usage);
    void addVertexAttribPointer(GLuint index, GLint size, GLenum type, GLboolean normalized, GLsizei stride, const GLvoid *pointer);
    void drawArrays(GLenum mode, GLint first, GLsizei count);
    void drawElements(GLenum mode, GLsizei count, GLenum type, const GLvoid *indices);

private:
    QOpenGLFunctions_3_3_Core* glfuns;
    unsigned int vao = 0;
    std::vector<unsigned int> vbos;
    std::vector<unsigned int> ebos;
};

#endif // MYGLOBJ_H
