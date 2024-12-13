# include <rendering/celestialGLWidget.h>

// # include <shader.h>

const char* vertex_shader = 
    "#version 330 core\n"
    "layout (location = 0) in vec3 aPos;"
    "layout (location = 1) in vec3 aNormal;"
    "layout (location = 2) in vec2 aTexCoords;"
    "out vec2 TexCoords;"
    "out vec3 WorldPos;"
    "out vec3 Normal;"
    "uniform mat4 projection;"
    "uniform mat4 view;"
    "uniform mat4 model;"
    "uniform mat3 normalMatrix;"
    "void main()"
    "{"
    "    TexCoords = aTexCoords;"
    "    WorldPos = vec3(model * vec4(aPos, 1.0));"
    "    Normal = normalMatrix * aNormal;   "
    "    gl_Position =  projection * view * vec4(WorldPos, 1.0);"
    "}"
    ;
const char* fragment_shader = 
    "#version 330 core\n"
    "out vec4 FragColor;"
    "in vec2 TexCoords;"
    "in vec3 WorldPos;"
    "in vec3 Normal;"
    "uniform vec3 albedo;"
    "uniform float ao;"
    "uniform vec3 camPos;"
    "const float PI = 3.14159265359;"
    "void main()"
    "{"
    "   vec3 N = normalize(Normal);"
    "   vec3 V = normalize(camPos - WorldPos);"
    "   vec3 ambient = vec3(0.3) * albedo * ao;"
    "   vec3 diffuse = max(dot(N, V), 0.0) * albedo;"
    "   vec3 color = ambient + diffuse;"
    "   FragColor = vec4(color, 1.0);"
    "}"
    ;


celestial_gl_widget::celestial_gl_widget(QWidget* parent) : QOpenGLWidget(parent){}

void celestial_gl_widget::initializeGL() {
    initializeOpenGLFunctions();
    unsigned int vertex_shader_id = glCreateShader(GL_VERTEX_SHADER);
    glShaderSource(vertex_shader_id, 1, &vertex_shader, NULL);
    glCompileShader(vertex_shader_id);
    int success;
    char info_log[512];
    glGetShaderiv(vertex_shader_id, GL_COMPILE_STATUS, &success);
    if (!success) {
        glGetShaderInfoLog(vertex_shader_id, 512, NULL, info_log);
        std::cout << "ERROR::SHADER::VERTEX::COMPILATION_FAILED\n" << info_log << std::endl;
    }
    unsigned int fragment_shader_id = glCreateShader(GL_FRAGMENT_SHADER);
    glShaderSource(fragment_shader_id, 1, &fragment_shader, NULL);
    glCompileShader(fragment_shader_id);
    glGetShaderiv(fragment_shader_id, GL_COMPILE_STATUS, &success);
    if (!success) {
        glGetShaderInfoLog(fragment_shader_id, 512, NULL, info_log);
        std::cout << "ERROR::SHADER::FRAGMENT::COMPILATION_FAILED\n" << info_log << std::endl;
    }
    
    shader_program = glCreateProgram();
    glAttachShader(shader_program, vertex_shader_id);
    glAttachShader(shader_program, fragment_shader_id);
    glLinkProgram(shader_program);
    glGetProgramiv(shader_program, GL_LINK_STATUS, &success);
    if (!success) {
        glGetProgramInfoLog(shader_program, 512, NULL, info_log);
        std::cout << "ERROR::SHADER::PROGRAM::LINKING_FAILED\n" << info_log << std::endl;
    }
    glDeleteShader(vertex_shader_id);
    glDeleteShader(fragment_shader_id);
    glEnable(GL_DEPTH_TEST);

    // _______________________________Initialize VAO, VBO and EBO_______________________________
    vao = vbo = ebo = celestial_body_vao = 0;

    // _______________________________ Initialize the Simulation _______________________________
    celestial_body* a = new celestial_body(5.0e10, 2.5, {10, 0, -50}, {0, 1, 0});
    celestial_body* b = new celestial_body(6.0e10, 2.5, {-10, 0, -50}, {0, -1, 0});
    celestial_body* c = new celestial_body(7.0e10, 3, {0, 10, -50}, {-1, 0, 0.5});
    celestial_body* d = new celestial_body(80.0e10, 350, {0, -10, -500}, {1, 0, -5000});
    sim.add_celestial_body(a);
    sim.add_celestial_body(b);
    sim.add_celestial_body(c);
    sim.add_celestial_body(d);
    sim.set_time_step(0.01);
    sim.set_simulate_algorithm<barnes_hut>();
//    sim.set_simulate_algorithm<pure_newtonian>();
//    barnes_hut::get_instance();
    std::cout << "Initialized!\n";

    // _______________________________   Initialize the Camera   _______________________________
    camera = Camera(glm::vec3(0.0f, 0.0f, 3.0f));
    glm::mat4 projection = glm::perspective(glm::radians(camera.Zoom), (float)width() / (float)height(), 0.1f, 1e8f);
    glUseProgram(shader_program);
    set_mat4("projection", projection);

    glUseProgram(shader_program);
    set_vec3("albedo", 0.5, 0.0, 0.5);
    set_float("ao", 1.0);

    // ___________________________________   Set Focusable   ____________________________________
    setFocusPolicy(Qt::WheelFocus);
    setMouseTracking(true);

//    timer = new QTimer(this);
//    connect(timer, SIGNAL(timeout()), this, SLOT(update_sim()));
//    timer -> start(20);
}

void celestial_gl_widget::paintGL() {
    glClearColor(0.1f, 0.1f, 0.1f, 1.0f);
    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
    glUseProgram(shader_program);
    glm::mat4 view = camera.GetViewMatrix();
    set_mat4("view", view);
    set_vec3("camPos", camera.Position);

    celestial_system* sys = sim.get_current_frame();
    render_celestial_system(sys);
}

void celestial_gl_widget::render_celestial_system(celestial_system* sys) {
//    std::cout << *sys << std::endl;
    for (auto& body : sys->bodies) {
        render_celestial_body(body);
    }
}

void celestial_gl_widget::render_celestial_body(celestial_body* body) {
    glm::mat4 model = glm::mat4(1.0f);
    model = glm::translate(model, glm::vec3(body->position));
    model = glm::scale(model, glm::vec3(body->radius));
    set_mat4("model", model);
    set_mat3("normalMatrix", glm::transpose(glm::inverse(glm::mat3(model))));
    if(celestial_body_vao == 0) {
        glGenVertexArrays(1, &celestial_body_vao);
        glGenBuffers(1, &vbo);
        glGenBuffers(1, &ebo);
        std::vector<glm::vec3> positions;
        std::vector<glm::vec3> normals;
        std::vector<glm::vec2> texCoords;
        std::vector<unsigned int> indices;

        const unsigned int X_SEGMENTS = 64;
        const unsigned int Y_SEGMENTS = 64;
        const float PI = 3.14159265359;
        for(unsigned int i = 0; i <= X_SEGMENTS; ++i) {
            for(unsigned int j = 0; j <= Y_SEGMENTS; ++j) {
                float x_seg = (float) i / (float) X_SEGMENTS;
                float y_seg = (float) j / (float) Y_SEGMENTS;
                float x_pos = std::cos(2.0f * PI * x_seg) * std::sin(PI * y_seg);
                float y_pos = std::cos(PI * y_seg);
                float z_pos = std::sin(2.0f * PI * x_seg) * std::sin(PI * y_seg);
                positions.push_back(glm::vec3(x_pos, y_pos, z_pos));
                normals.push_back(glm::vec3(x_pos, y_pos, z_pos));
            }
        }

        bool oddRow = false;
        for(unsigned int j = 0; j < Y_SEGMENTS; ++j) {
            if (!oddRow) {
                for(unsigned int i = 0; i <= X_SEGMENTS; ++i) {
                    indices.push_back(j * (X_SEGMENTS + 1) + i);
                    indices.push_back((j + 1) * (X_SEGMENTS + 1) + i);
                }
            }
            else {
                for(int i = X_SEGMENTS; i >= 0; --i) {
                    indices.push_back((j + 1) * (X_SEGMENTS + 1) + i);
                    indices.push_back(j * (X_SEGMENTS + 1) + i);
                }
            }
            oddRow = !oddRow;
        }
        index_count = indices.size();

        std::vector<float> data;
        for(unsigned int i = 0; i < positions.size(); ++i) {
            data.push_back(positions[i].x);
            data.push_back(positions[i].y);
            data.push_back(positions[i].z);
            if(normals.size() > 0) {
                data.push_back(normals[i].x);
                data.push_back(normals[i].y);
                data.push_back(normals[i].z);
            }
            if(texCoords.size() > 0) {
                data.push_back(texCoords[i].x);
                data.push_back(texCoords[i].y);
            }
        }
        glBindVertexArray(celestial_body_vao);
        glBindBuffer(GL_ARRAY_BUFFER, vbo);
        glBufferData(GL_ARRAY_BUFFER, data.size() * sizeof(float), &data[0], GL_STATIC_DRAW);
        glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, ebo);
        glBufferData(GL_ELEMENT_ARRAY_BUFFER, indices.size() * sizeof(unsigned int), &indices[0], GL_STATIC_DRAW);
        glEnableVertexAttribArray(0);
        glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 8 * sizeof(float), (void*)0);
        glEnableVertexAttribArray(1);
        glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, 8 * sizeof(float), (void*)(3 * sizeof(float)));
        glEnableVertexAttribArray(2);
        glVertexAttribPointer(2, 2, GL_FLOAT, GL_FALSE, 8 * sizeof(float), (void*)(6 * sizeof(float)));
    }
    glBindVertexArray(celestial_body_vao);
    glDrawElements(GL_TRIANGLE_STRIP, index_count, GL_UNSIGNED_INT, 0);
}

void celestial_gl_widget::resizeGL(int w, int h) {
    glViewport(0, 0, w, h);
}

void celestial_gl_widget::update_sim(){
    sim.simulate();
    update();
}

double celestial_gl_widget::get_current_time(){
    return sim.get_current_frame()->get_time();
}

void celestial_gl_widget::set_time(double time){
    sim.back_to(time);
    update();
}

celestial_gl_widget::~celestial_gl_widget() {
    makeCurrent();
    glDeleteVertexArrays(1, &celestial_body_vao);
    glDeleteBuffers(1, &vbo);
    glDeleteBuffers(1, &ebo);
    glDeleteProgram(shader_program);

    doneCurrent();
}

void celestial_gl_widget::keyPressEvent(QKeyEvent *event) {
    float deltaTime = 0.1f;
    switch (event->key()) {
        case Qt::Key_W:
            camera.ProcessKeyboard(FORWARD, deltaTime);
            break;
        case Qt::Key_S:
            camera.ProcessKeyboard(BACKWARD, deltaTime);
            break;
        case Qt::Key_A:
            camera.ProcessKeyboard(LEFT, deltaTime);
            break;
        case Qt::Key_D:
            camera.ProcessKeyboard(RIGHT, deltaTime);
            break;
        case Qt::Key_Alt:
            setCursor(Qt::ArrowCursor);
            break;
        default:
            QWidget::keyPressEvent(event);
    }
}

void celestial_gl_widget::keyReleaseEvent(QKeyEvent *event) {
    if (event->key() == Qt::Key_Alt) {
        setCursor(Qt::BlankCursor);
        QCursor::setPos(mapToGlobal(rect().center()));
    }
    QWidget::keyReleaseEvent(event);
}

void celestial_gl_widget::focusInEvent(QFocusEvent *event) {
    if(timer->isActive()){
        setCursor(Qt::BlankCursor);
        QCursor::setPos(mapToGlobal(rect().center()));
        QWidget::focusInEvent(event);
    }
}

void celestial_gl_widget::focusOutEvent(QFocusEvent *event) {
    setCursor(Qt::ArrowCursor);
    QWidget::focusOutEvent(event);
}

void celestial_gl_widget::mouseMoveEvent(QMouseEvent *event) {
    if (cursor().shape() == Qt::BlankCursor) {
        QPoint currentMousePos = event->pos();
        float xOffset = currentMousePos.x() - rect().center().x();
        float yOffset = rect().center().y() - currentMousePos.y();
        camera.ProcessMouseMovement(xOffset, yOffset);
        QCursor::setPos(mapToGlobal(rect().center()));
    }

    QWidget::mouseMoveEvent(event);
}


void celestial_gl_widget::wheelEvent(QWheelEvent *event) {
    float yOffset = event->angleDelta().y() / 120.0f;
    camera.ProcessMouseScroll(yOffset);
    glm::mat4 projection = glm::perspective(glm::radians(camera.Zoom), (float)width() / (float)height(), 0.1f, 1e8f);
    glUseProgram(shader_program);
    set_mat4("projection", projection);
}

void celestial_gl_widget::set_pure_newtonian() {
    sim.set_simulate_algorithm<pure_newtonian>();
}

void celestial_gl_widget::set_barnes_hut() {
    sim.set_simulate_algorithm<barnes_hut>();
}

void celestial_gl_widget::toggle_simulation(){
    if(timer->isActive()){
        timer->stop();
    }else{
        timer->start(20);
    }
}

QString toQString(celestial_system& sys) {
    std::ostringstream oss;
    oss << sys;
    return QString::fromStdString(oss.str());
}

void celestial_gl_widget::save_csv() {
    if(timer->isActive()){
        timer->stop();
    }

    QString filename = QFileDialog::getSaveFileName(
        this,
        "Save current frame",
        QDateTime::currentDateTime().toString("yyyy-MM-dd_hh-mm-ss") + ".csv",
        "Comma Separated Values(*.csv)"
    );
    if (!filename.isEmpty()) {
        QFile file(filename);

        if (file.exists()) {
            int ret = QMessageBox::warning(
                nullptr,
                "File exists",
                "File exists, do cover?",
                QMessageBox::Yes | QMessageBox::No
            );

            if (ret == QMessageBox::No) {
                qDebug() << "Cancelled covering.";
                return;
            }
        }

        if (file.open(QIODevice::WriteOnly | QIODevice::Text)) {
            QTextStream out(&file);
//            out << "Sample content.\n";
            out << toQString(*sim.get_current_frame());
            file.close();
            QMessageBox::information(nullptr, "Information", "Successfully saved!");
        } else {
            QMessageBox::critical(nullptr, "Error", "Cannot write target file!");
        }
    } else {
        qDebug() << "Cancelled saving.";
    }
}

void celestial_gl_widget::load_csv() {

}

void celestial_gl_widget::set_float(const char* name, float value) {
    glUniform1f(glGetUniformLocation(shader_program, name), value);
}
void celestial_gl_widget::set_vec3(const char* name, float x, float y, float z) {
    glUniform3f(glGetUniformLocation(shader_program, name), x, y, z);
}
void celestial_gl_widget::set_vec3(const char* name, const glm::vec3& vec) {
    glUniform3f(glGetUniformLocation(shader_program, name), vec.x, vec.y, vec.z);
}
void celestial_gl_widget::set_mat3(const char* name, const glm::mat3& mat) {
    glUniformMatrix3fv(glGetUniformLocation(shader_program, name), 1, GL_FALSE, &mat[0][0]);
}
void celestial_gl_widget::set_mat4(const char* name, const glm::mat4& mat) {
    glUniformMatrix4fv(glGetUniformLocation(shader_program, name), 1, GL_FALSE, &mat[0][0]);
}


