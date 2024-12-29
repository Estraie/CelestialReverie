# include <rendering/celestialGLWidget.h>

// # include <shader.h>

const int MAX_LIGHTS = 10;

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
    "const int MAX_LIGHTS = 10;"
    "uniform int num_lights;"
    "uniform vec3 light_positions[MAX_LIGHTS];"
    "uniform vec3 light_colors[MAX_LIGHTS];"
    "uniform vec3 albedo;"
    "uniform float ao;"
    "uniform vec3 camPos;"
    "uniform vec3 vEmission;"
    "uniform vec3 acceleration;"
    "uniform vec3 camera_front;"
    "uniform float current_time;"
    "const float PI = 3.14159265359;"
    "float distribution_ggx(vec3 N, vec3 H, float roughness)"
    "{"
    "    float a = roughness * roughness;"
    "    float a2 = a * a;"
    "    float NdotH = max(dot(N, H), 0.0);"
    "    float NdotH2 = NdotH * NdotH;"
    "    float nom = a2;"
    "    float denom = (NdotH2 * (a2 - 1.0) + 1.0);"
    "    denom = PI * denom * denom;"
    "    return nom / denom;"
    "}"
    "float geometry_schlick_ggx(float NdotV, float roughness)"
    "{"
    "    float r = (roughness + 1.0);"
    "    float k = (r * r) / 8.0;"
    "    float nom = NdotV;"
    "    float denom = NdotV * (1.0 - k) + k;"
    "    return nom / denom;"
    "}"
    "float geometry_smith(vec3 N, vec3 V, vec3 L, float roughness)"
    "{"
    "    float NdotV = max(dot(N, V), 0.0);"
    "    float NdotL = max(dot(N, L), 0.0);"
    "    float ggx2 = geometry_schlick_ggx(NdotV, roughness);"
    "    float ggx1 = geometry_schlick_ggx(NdotL, roughness);"
    "    return ggx1 * ggx2;"
    "}"
    "vec3 fresnel_schlick(float cos_theta, vec3 F0)"
    "{"
    "    return F0 + (1.0 - F0) * pow(clamp(1.0 - cos_theta, 0.0, 1.0), 5.0);"
    "}"
    "vec3 calculate_lighting(vec3 N, vec3 V, vec3 L, vec3 albedo, float roughness, vec3 F0, float ao)"
    "{"
    "    vec3 H = normalize(V + L);"
    "    float NDF = distribution_ggx(N, H, roughness);"
    "    float G = geometry_smith(N, V, L, roughness);"
    "    vec3 F = fresnel_schlick(max(dot(H, V), 0.0), F0);"
    "    vec3 kS = F;"
    "    vec3 kD = vec3(1.0) - kS;"
    "    kD *= 1.0;"
    "    vec3 numerator = NDF * G * F;"
    "    float denominator = 4.0 * max(dot(N, V), 0.0) * max(dot(N, L), 0.0);"
    "    vec3 specular = numerator / max(denominator, 0.0001);"
    "    vec3 diffuse = albedo / PI;"
    "    return (kD * diffuse + specular) * light_colors[0] * max(dot(N, L), 0.0);"
    "}"
    "float fractal_noise(vec3 p, int octaves, float persistence, float lacunarity) {"
    "    float value = 0.0;"
    "    float amplitude = 1.0;"
    "    float frequency = 1.0;"
    "    for(int i = 0; i < octaves; i++) {"
    "        value += amplitude * fract(sin((sin(p.x) + sin(p.y) + sin(p.z) + sin(current_time)) * 100) * 4.5453);"
    "        amplitude *= persistence;"
    "        frequency *= lacunarity;"
    "        p *= frequency;"
    "    }"
    "    return value;"
    "}"
    "float hash31(vec3 p) {"
    "    p = fract(p * vec3(452.127, 932.618, 789.233));"
    "    p += dot(p, p + 123.23);"
    "    return fract(p.x * p.y * p.z);"
    "}"
    "float noise(vec3 p) {"
    "    vec3 q = floor(p);"
    "    vec3 f = fract(p);"
    "    return mix(mix(mix(hash31(q + vec3(0, 0, 0)), hash31(q + vec3(1, 0, 0)), f.x),"
    "                   mix(hash31(q + vec3(0, 1, 0)), hash31(q + vec3(1, 1, 0)), f.x), f.y),"
    "               mix(mix(hash31(q + vec3(0, 0, 1)), hash31(q + vec3(1, 0, 1)), f.x),"
    "                   mix(hash31(q + vec3(0, 1, 1)), hash31(q + vec3(1, 1, 1)), f.x), f.y),"
    "               f.z);"
    "}"
    "float fbm(vec3 p) {"
    "    float f = 0.;"
    "    f +=      .5 * noise(p);"
    "    f +=     .25 * noise(p * 2.);"
    "    f +=    .125 * noise(p * 4.);"
    "    f +=   .0625 * noise(p * 8.);"
    "    f +=  .03125 * noise(p * 16.);"
    "    return f;"
    "}"
    "float func(vec3 p) {"
    "    vec3 timeOffset = vec3(sin(current_time * 1.8), cos(current_time * 1.8), sin(current_time * 1.5));"
    "    return fbm(timeOffset + p * fbm(p + .2 * current_time) + noise(p + noise(p + noise(p))));"
    "}"
    "vec3 my_render(vec3 p) {"
    "   vec3 bgEmission = vec3(1.0);"
    "   float k = pow(func(9.0 * p), 1.2);"
    "   vec3 color = mix(bgEmission, vEmission, vec3(k));"
    "   return color;"
    "}"
    "void main()"
    "{"
    "   vec3 N = normalize(Normal);"
    "   vec3 V = normalize(camPos - WorldPos);"
    "   vec3 F0 = vec3(0.04);"
    "   F0 = mix(F0, albedo, 0.0);"
    "   vec3 result = vec3(0.0);"
    "   for(int i = 0; i < num_lights; ++i) {"
    "       vec3 L = normalize(light_positions[i] - WorldPos);"
    "       result += calculate_lighting(N, V, L, albedo, 1.0, F0, ao);"
    "   }"
    "   vec3 ambient = vec3(0.01) * albedo * ao;"
    // "   vec3 diffuse = max(dot(N, V), 0.0) * albedo;"
    "   vec3 color = ambient + result;"
    "   color = color / (color + vec3(1.0));"
    "   color = pow(color, vec3(1.0/2.2));"
    "   float opacity = 1.0;"
    "   if(vEmission.x != 0.0 || vEmission.y != 0.0 || vEmission.z != 0.0) {"
    "       color = my_render(Normal);"
    "   }"
    "   vec3 proj_acc = normalize(acceleration - dot(acceleration, camera_front) * camera_front);"
    "   vec3 proj_N = normalize(N - dot(N, camera_front) * camera_front);"
    "   if(dot(proj_acc, proj_N) > 0.99) {"
    "       color = vec3(0, 1, 0);"
    "   }"
    "   FragColor = vec4(color, opacity);"
    "}"
    ;

// "       vec3 bgEmission = vec3(1.0);"
// "       float k = pow(func(9.0 * N), 1.2);"
// "       color = mix(bgEmission, color, vec3(k));"
// "       float cosine = dot(N, V);"
// "       color = max(1.0 - cosine * (1.0 - cosine), 0.0) * vEmission;"
// "       color *= fractal_noise((N) * 0.01, 6, 0.2, 0.5) * 0.5 + 0.5;"
//    "       float alpha = 0.5;"
//    "       float halo = exp(-alpha * cosine * cosine);"
//    "       color += halo * 1.0;"
//    "       color = color / (color + vec3(1.0));"
//    "       color = pow(color, vec3(1.0/2.2));"

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
    //vao = vbo = ebo = celestial_body_vao = 0;

    // _______________________________ Initialize the Simulation _______________________________
    celestial_body* a = new celestial_body(5.0e10, 2.5, {10, 0, -50}, {0, 1, 0});
    celestial_body* b = new star(6.0e10, 2.5, {-10, 0, -50}, {0, -1, 0});
    celestial_body* c = new planet(7.0e10, 3, {0, 10, -50}, {-1, 0, 0.5});
    celestial_body* d = new star(80.0e10, 3.5, {0, -10, -20}, {1, 0, 0.5});
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
    camera = Camera(glm::vec3(0.0f, 0.0f, 50.0f));
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
    int num_lights = 0;
    set_float("current_time", sys->time);
    for (auto& body : sys->bodies) {
        if(body->is_emissive() && num_lights < MAX_LIGHTS) {
            set_vec3(("light_positions[" + std::to_string(num_lights) + "]").c_str(), body->position);
            set_vec3(("light_colors[" + std::to_string(num_lights) + "]").c_str(), body->color);
            num_lights++;
        }
    }
    set_int("num_lights", num_lights);
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

    set_vec3("albedo", body->color);
    set_vec3("acceleration", body->acceleration);
    set_vec3("camera_front", camera.Front);

    if(body->is_emissive()) {
        set_vec3("vEmission", body->color);
    } else {
        set_vec3("vEmission", glm::vec3(0.0));
    }

    if(globj == 0) {
        globj = new MyGLObj(this);
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
        globj->addVBO(data.size() * sizeof(float), &data[0], GL_STATIC_DRAW);
        globj->addEBO(indices.size() * sizeof(unsigned int), &indices[0], GL_STATIC_DRAW);
        globj->addVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 8 * sizeof(float), (void*)0);
        globj->addVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, 8 * sizeof(float), (void*)(3 * sizeof(float)));
        globj->addVertexAttribPointer(2, 2, GL_FLOAT, GL_FALSE, 8 * sizeof(float), (void*)(6 * sizeof(float)));
    }
    globj->drawElements(GL_TRIANGLE_STRIP, index_count, GL_UNSIGNED_INT, 0);

    /*
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
    */
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
    delete globj;
    //glDeleteVertexArrays(1, &celestial_body_vao);
    //glDeleteBuffers(1, &vbo);
    //glDeleteBuffers(1, &ebo);
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
        case Qt::Key_Space:
            camera.ProcessKeyboard(UP, deltaTime);
            break;
        case Qt::Key_C:
            camera.ProcessKeyboard(DOWN, deltaTime);
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

void celestial_gl_widget::set_pure_newtonian_parallel() {
    sim.set_simulate_algorithm<pure_newtonian_parallel>();
}

void celestial_gl_widget::set_barnes_hut() {
    sim.set_simulate_algorithm<barnes_hut>();
}

void celestial_gl_widget::set_barnes_hut_parallel() {
    sim.set_simulate_algorithm<barnes_hut_parallel>();
}

void celestial_gl_widget::set_forward_euler() {
    sim.set_update_algorithm<forward_euler>();
}

void celestial_gl_widget::set_implicit_euler() {
    sim.set_update_algorithm<implicit_euler>();
}

void celestial_gl_widget::set_no_collision() {
    sim.set_collision_detection<dummy_detection>();
}

void celestial_gl_widget::set_octree_collision() {
    sim.set_collision_detection<ocd>();
}

void celestial_gl_widget::set_time_step() {
    bool ok;
    double time_step = QInputDialog::getDouble(
        this, 
        "Set time step", 
        "Enter time step:", 
        sim.get_time_step(), 
        0.01, 
        1000000.0, 
        2, //Number of digits after decimal point
        &ok
    );
    sim.set_time_step(time_step);
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
    if(timer->isActive()) {
        timer->stop();
    }
    QString filename = QFileDialog::getOpenFileName(
        this,
        "Open celestial system",
        ".",
        "Comma Separated Values(*.csv)"
    );

    if (filename.isEmpty()) {
        qDebug() << "Cancelled loading.";
        return;
    }

    QFile file(filename);
    if (!file.open(QIODevice::ReadOnly | QIODevice::Text)) {
        QMessageBox::critical(nullptr, "Error", "Cannot read target file!");
        return;
    }

    QTextStream in(&file);

    int sz = sim.get_current_frame()->bodies.size();
    while (!in.atEnd()) {
        QString line = in.readLine();
        if (line.trimmed().isEmpty()) continue; // Skip empty lines

        QStringList tokens = line.split(',');
        if (tokens.size() != 15) {
            QMessageBox::critical(nullptr, "Error", "Invalid file format!");
            return;
        }

        bool ok = true;
        int category = tokens[0].toInt(&ok); if (!ok) break;
        double mass = tokens[1].toDouble(&ok); if (!ok) break;
        double radius = tokens[2].toDouble(&ok); if (!ok) break;
        double posX = tokens[3].toDouble(&ok); if (!ok) break;
        double posY = tokens[4].toDouble(&ok); if (!ok) break;
        double posZ = tokens[5].toDouble(&ok); if (!ok) break;
        double velX = tokens[6].toDouble(&ok); if (!ok) break;
        double velY = tokens[7].toDouble(&ok); if (!ok) break;
        double velZ = tokens[8].toDouble(&ok); if (!ok) break;
        double accX = tokens[9].toDouble(&ok); if (!ok) break;
        double accY = tokens[10].toDouble(&ok); if (!ok) break;
        double accZ = tokens[11].toDouble(&ok); if (!ok) break;
        double colorR = tokens[12].toDouble(&ok); if (!ok) break;
        double colorG = tokens[13].toDouble(&ok); if (!ok) break;
        double colorB = tokens[14].toDouble(&ok); if (!ok) break;

        celestial_body* body;
        if(category == 0) {
            body = new celestial_body(mass, radius, {posX, posY, posZ}, {velX, velY, velZ}, {accX, accY, accZ}, {colorR, colorG, colorB});
        } else if(category == 1) {
            body = new planet(mass, radius, {posX, posY, posZ}, {velX, velY, velZ}, {accX, accY, accZ}, {colorR, colorG, colorB});
        } else if(category == 2) {
            body = new star(mass, radius, {posX, posY, posZ}, {velX, velY, velZ}, {accX, accY, accZ}, {colorR, colorG, colorB});
        }
        sim.get_current_frame()->add_body(body);
    }

    if (in.status() != QTextStream::Ok) {
        QMessageBox::critical(nullptr, "Error", "Failed to parse the file!");
        return;
    }

    file.close();

    
    sim.get_current_frame()->set_time(0);
    sim.clear_buffer(sz);
    sim.add_buffer();

    update();
    QMessageBox::information(nullptr, "Information", "Successfully loaded!");
}

void celestial_gl_widget::set_int(const char* name, int value) {
    glUniform1i(glGetUniformLocation(shader_program, name), value);
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


