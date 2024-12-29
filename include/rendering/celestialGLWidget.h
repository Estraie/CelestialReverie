# ifndef CELESTIAL_GL_WIDGET_H
# define CELESTIAL_GL_WIDGET_H

# include <QOpenGLWidget>
# include <QOpenGLFunctions_3_3_Core>
# include <qtimer.h>
# include <QKeyEvent>
# include <QMouseEvent>
# include <QWheelEvent>
# include <QFileDialog>
# include <QInputDialog>
# include <QMessageBox>
# include <QDateTime>
# include <QDebug>
# include <iostream>
# include <sstream>

# include <glm/glm.hpp>
# include <glm/gtc/matrix_transform.hpp>
# include <glm/gtc/type_ptr.hpp>

# include <simulation/celestial_reverie.h>
# include <rendering/camera.h>

# include <string>

#include "rendering/myglobj.h"

class celestial_gl_widget : public QOpenGLWidget, protected QOpenGLFunctions_3_3_Core {
    Q_OBJECT
public:
    explicit celestial_gl_widget(QWidget *parent = nullptr);
    double get_current_time();
    void set_time(double time);
    ~celestial_gl_widget();
    QTimer* timer;

protected:
    virtual void initializeGL() override;
    virtual void paintGL() override;
    virtual void resizeGL(int w, int h) override;
    void keyPressEvent(QKeyEvent* event) override;
    void mouseMoveEvent(QMouseEvent* event) override;
    void wheelEvent(QWheelEvent *event) override;
    void keyReleaseEvent(QKeyEvent* event) override;
    void focusInEvent(QFocusEvent* event) override;
    void focusOutEvent(QFocusEvent* event) override;
    void render_celestial_system(celestial_system* sys);
    void render_celestial_body(celestial_body* body);
    celestial_reverie sim;
    Camera camera;
    unsigned int shader_program;
    //unsigned int vao;
    //unsigned int vbo;
    //unsigned int ebo;
    //unsigned int celestial_body_vao;
    unsigned int index_count;

    MyGLObj* globj = 0;

    void set_int(const char* name, int value);
    void set_float(const char* name, float value);
    void set_vec3(const char* name, float x, float y, float z);
    void set_vec3(const char* name, const glm::vec3& vec);
    void set_mat3(const char* name, const glm::mat3& mat);
    void set_mat4(const char* name, const glm::mat4& mat);

signals:
    void timeout();

public slots:
    void update_sim();
    void set_pure_newtonian();
    void set_pure_newtonian_parallel();
    void set_barnes_hut();
    void set_barnes_hut_parallel();
    void set_forward_euler();
    void set_implicit_euler();
    void set_no_collision();
    void set_octree_collision();
    void set_time_step();
    void toggle_simulation();
    void load_csv();
    void save_csv();
};

# endif // CELESTIAL_GL_WIDGET_H
