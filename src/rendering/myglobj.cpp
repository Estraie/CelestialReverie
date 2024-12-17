#include <rendering/myglobj.h>

/* 继承QOpenGLWidget并重写initializeGL、resizeGL或paintGL函数时，不需要调用makeCurrent，
 * 但在其他函数中调用GL函数时必须调用makeCurrent()和doneCurrent()，因此，为了确保MyGLObj
 * 被正确析构，必须用new创建MyGLObj对象，用delete释放,并保证构造和析构位于initializeGL、resizeGL或paintGL函数中，
 * 或者位于makeCurrent()和doneCurrent()之间
 */
MyGLObj::MyGLObj(QOpenGLFunctions_3_3_Core* glfuns)
{
    this->glfuns = glfuns;
    glfuns->glGenVertexArrays(1, &vao);
}

//addVBO, addEBO, addVertexAttribPointer应在同一个MyGLObj对象上按顺序连续调用
void MyGLObj::addVBO(GLsizeiptr size, const GLvoid *data, GLenum usage=GL_STATIC_DRAW)
{
    unsigned int vbo;
    glfuns->glGenBuffers(1, &vbo);
    vbos.push_back(vbo);
    glfuns->glBindVertexArray(vao);
    glfuns->glBindBuffer(GL_ARRAY_BUFFER, vbo);
    glfuns->glBufferData(GL_ARRAY_BUFFER, size, data, usage);
}

void MyGLObj::addEBO(GLsizeiptr size, const GLvoid *data, GLenum usage=GL_STATIC_DRAW)
{
    unsigned int ebo;
    glfuns->glGenBuffers(1, &ebo);
    ebos.push_back(ebo);
    glfuns->glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, ebo);
    glfuns->glBufferData(GL_ELEMENT_ARRAY_BUFFER, size, data, usage);
}

void MyGLObj::addVertexAttribPointer(GLuint index, GLint size, GLenum type, GLboolean normalized, GLsizei stride, const GLvoid *pointer)
{
    glfuns->glVertexAttribPointer(index, size, type, normalized, stride, pointer);
    glfuns->glEnableVertexAttribArray(index);
}

void MyGLObj::drawArrays(GLenum mode, GLint first, GLsizei count)
{
    glfuns->glBindVertexArray(vao);
    glfuns->glDrawArrays(mode, first, count);
}

void MyGLObj::drawElements(GLenum mode, GLsizei count, GLenum type, const GLvoid *indices)
{
    glfuns->glBindVertexArray(vao);
    glfuns->glDrawElements(mode, count, type, indices);
}

MyGLObj::~MyGLObj()
{
    glfuns->glDeleteVertexArrays(1, &vao);

    for (auto& vbo : vbos)
        glfuns->glDeleteBuffers(1, &vbo);

    for (auto& ebo : ebos)
        glfuns->glDeleteBuffers(1, &ebo);
}
