# ifndef CELESTIAL_GL_WIDGET_H
# define CELESTIAL_GL_WIDGET_H

# include <QOpenGLWidget>
# include <QOpenGLFunctions_3_3_Core>
# include <qtimer.h>

# include <glm/glm.hpp>
# include <glm/gtc/matrix_transform.hpp>
# include <glm/gtc/type_ptr.hpp>

# include <simulation/celestial_reverie.h>
# include <rendering/camera.h>

# include <string>

class celestial_gl_widget : public QOpenGLWidget, protected QOpenGLFunctions_3_3_Core {
    Q_OBJECT
public:
    explicit celestial_gl_widget(QWidget *parent = nullptr);
    double get_current_time();
    ~celestial_gl_widget();
protected:
    virtual void initializeGL() override;
    virtual void paintGL() override;
    virtual void resizeGL(int w, int h) override;
    void render_celestial_system(celestial_system* sys);
    void render_celestial_body(celestial_body* body);
    celestial_reverie sim;
    Camera camera;
    unsigned int shader_program;
    unsigned int vao;
    unsigned int vbo;
    unsigned int ebo;
    unsigned int celestial_body_vao;
    unsigned int index_count;

    QTimer* timer;

    void set_float(const char* name, float value);
    void set_vec3(const char* name, float x, float y, float z);
    void set_vec3(const char* name, const glm::vec3& vec);
    void set_mat3(const char* name, const glm::mat3& mat);
    void set_mat4(const char* name, const glm::mat4& mat);

signals:
    void timeout();

public slots:
    void update_sim();
//    void toggle_simulation();
};

# endif // CELESTIAL_GL_WIDGET_H
