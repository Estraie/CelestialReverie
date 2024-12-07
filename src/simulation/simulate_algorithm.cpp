# include <vector>
# include <glm/glm.hpp>
# include <simulation/celestial_body.h>
# include <simulation/celestial_system.h>
# include <simulation/simulate_algorithm.h>

// class sim_constants {
// private:
//     static double G;
// public:
//     static double get_G() {
//         return G;
//     }
//     static void set_G(double g) {
//         G = g;
//     } 
// };
const double sim_constants::G = 6.67408e-11;

forward_euler& forward_euler::get_instance() {
    static forward_euler instance;
    return instance;
}

void forward_euler::update(celestial_body*& body, double dt) {
    body->velocity += body->acceleration * dt;
    body->position += body->velocity * dt;
}

void forward_euler::update(celestial_system*& system, double dt) {
    for(auto& body : system->bodies){
        update(body, dt);
    }
}

// class update_algorithm {
// public:
//     virtual void update(celestial_body*& body, double dt) = 0;
//     virtual void update(celestial_system*& system, double dt) = 0;
// };

// class forward_euler : public update_algorithm {
// public:
//     forward_euler(const forward_euler&) = delete;
//     forward_euler& operator=(const forward_euler&) = delete;

//     static forward_euler& get_instance() {
//         static forward_euler instance;
//         return instance;
//     }

//     void update(celestial_body*& body, double dt) override {
//         body->velocity += body->acceleration * dt;
//         body->position += body->velocity * dt;
//     }

//     void update(celestial_system*& system, double dt) override {
//         for(auto& body : system->bodies){
//             update(body, dt);
//         }
//     }

// private:
//     forward_euler() = default;
// };

// class simulate_algorithm {
// private:
//     explicit simulate_algorithm(){};
//     // explicit simulate_algorithm(const simulate_algorithm&) = delete;
// public:
//     virtual void simulate(celestial_system*& system) = 0;
//     static simulate_algorithm& get_instance();
// };

simulate_algorithm::simulate_algorithm(update_algorithm& algorithm) : algorithm(algorithm) {}

simulate_algorithm& pure_newtonian::get_instance() {
    static pure_newtonian instance;
    return instance;
}

simulate_algorithm& barnes_hut::get_instance() {
    static barnes_hut instance;
    return instance;
}

void pure_newtonian::add_gravity(celestial_body*& body, celestial_body*& other) {
    auto dist_vec = other->position- body->position;
    double G = sim_constants::get_G();
    body->acceleration += 
        G * other->mass * dist_vec / glm::dot(dist_vec, dist_vec);
    other->acceleration -= 
        G * body->mass * dist_vec / glm::dot(dist_vec, dist_vec);   
}

void pure_newtonian::simulate(celestial_system*& system) {
    for(int i = 0; i < system->bodies.size(); i++){
        for(int j = i + 1; j < system->bodies.size(); j++){
            add_gravity(system->bodies[i], system->bodies[j]);
        }
    }
}

barnes_hut::node::node() : 
    body(NULL), center(glm::dvec3(0.0)), father(NULL), mass(0.0), barycenter(glm::dvec3(0.0)) 
{
    for(int i = 0; i < 8; i++){
        children[i] = NULL;
    }
}

barnes_hut::node::node(celestial_body* body, glm::dvec3 center, node* father) : 
    body(body), center(center), father(father), mass(body->mass), barycenter(body->position)
{
    for(int i = 0; i < 8; i++){
        children[i] = NULL;
    }
}

barnes_hut::node::~node() {
    for(int i = 0; i < 8; i++){
        if(children[i] != NULL) {
            delete children[i];
            children[i] = NULL;
        }
    }
}

glm::dvec3 barnes_hut::get_next_center(node* node, int direction) {
    glm::dvec3 next_center = node->center;
    glm::dvec3 factor = glm::dvec3(1.0, 1.0, 1.0);
    if(!(direction & 1)) {
        factor.x = -1.0;
    }
    if(!(direction & 2)) {
        factor.y = -1.0;
    }
    if(!(direction & 4)) {
        factor.z = -1.0;
    }
    return next_center + factor * node->semi_edge;
}

void barnes_hut::add_body(node* root, celestial_body* body) {
    int direction;

    // Case NULL node
    if(root->mass == 0.0) {
        root->body = body;
        root->mass = body->mass;
        root->barycenter = body->position;
        return;
    }
    else {
        direction = get_direction(root->body->position, root->center);
        if(root->children[direction] == NULL) {
            root->children[direction] = new node(root->body, get_next_center(root, direction), root);
            root->children[direction]->semi_edge = root->semi_edge / 2.0;
        }
        else {
            add_body(root->children[direction], root->body);
        }
        root->body = NULL;
    }

    // Case leaf node
    direction = get_direction(body->position, root->center);
    if(root->children[direction] == NULL) {
        root->children[direction] = new node(body, get_next_center(root, direction), root);
        root->children[direction]->semi_edge = root->semi_edge / 2.0;
    }
    else {
        add_body(root->children[direction], body);
    }

    root->barycenter = (root->barycenter * root->mass + body->position * body->mass) / 
        (root->mass + body->mass);
    root->mass += body->mass;
}

void barnes_hut::add_gravity(node*& root, celestial_body*& body) { // TODO
    if(root == NULL || body == NULL) {
        return;
    }
    if(root->body == NULL) {
        if(glm::length(body->position - root->barycenter) > glm::length(root->semi_edge.x)) {
            body->acceleration += 
                sim_constants::get_G() * root->mass * (body->position - root->barycenter) / 
                glm::dot(body->position - root->barycenter, body->position - root->barycenter);
        }
        else {
            for(int i = 0; i < 8; i++) {
                add_gravity(root->children[i], body);
            }
        }
    } else {
        body->acceleration += 
            sim_constants::get_G() * root->mass * (body->position - root->body->position) / 
            glm::dot(body->position - root->body->position, body->position - root->body->position);
    }
}

int barnes_hut::get_direction(glm::dvec3& pos, glm::dvec3& center) {
    int direction = 0;
    if(pos.x > center.x) 
        direction += 1;
    if(pos.y > center.y) 
        direction += 2;
    if(pos.z > center.z) 
        direction += 4;
    return direction;
}

void barnes_hut::simulate(celestial_system*& system) {
    node* root = new node();
    upper_bound.x = upper_bound.y = upper_bound.z = -std::numeric_limits<double>::infinity();
    lower_bound.x = lower_bound.y = lower_bound.z = std::numeric_limits<double>::infinity();
    for(auto& body : system->bodies){
        upper_bound.x = std::max(upper_bound.x, body->position.x);
        upper_bound.y = std::max(upper_bound.y, body->position.y);
        upper_bound.z = std::max(upper_bound.z, body->position.z);
        lower_bound.x = std::min(lower_bound.x, body->position.x);
        lower_bound.y = std::min(lower_bound.y, body->position.y);
        lower_bound.z = std::min(lower_bound.z, body->position.z);
    }
    root->center = (upper_bound + lower_bound) / 2.0;
    root->semi_edge = (upper_bound - lower_bound) / 2.0;
    for(auto& body : system->bodies){
        add_body(root, body);
    }
    for(auto& body : system->bodies){
        add_gravity(root, body);
    }
    delete root;
}

// class pure_newtonian : public simulate_algorithm {
// private:
//     static pure_newtonian instance;
//     pure_newtonian() = default;
// public:
//     void add_gravity(celestial_body*& body, celestial_body*& other) {
//         auto dist_vec = other->position- body->position;
//         double G = sim_constants::get_G();
//         body->acceleration += 
//             G * other->mass * dist_vec / glm::dot(dist_vec, dist_vec);
//         other->acceleration -= 
//             G * body->mass * dist_vec / glm::dot(dist_vec, dist_vec);   
//     }

//     void simulate(celestial_system*& system) override{
//         for(int i = 0; i < system->bodies.size(); i++){
//             for(int j = i + 1; j < system->bodies.size(); j++){
//                 add_gravity(system->bodies[i], system->bodies[j]);
//             }
//         }
//     }

//     static simulate_algorithm& get_instance(){
//         return instance;
//     }
// };

// class barnes_hut : public simulate_algorithm {
// private:
//     barnes_hut() = default;
//     static barnes_hut instance;
//     static constexpr double theta = 0.5;
//     glm::dvec3 upper_bound; // (max_x, max_y, max_z)
//     glm::dvec3 lower_bound; // (min_x, min_y, min_z)

//     class node{
//     public:
//         celestial_body* body;
//         glm::dvec3 center;
//         glm::dvec3 barycenter;
//         glm::dvec3 semi_edge; // (upper_bound - lower_bound)[of this node] / 2
//         glm::f64 mass;
//         node* father;
//         node* children[8];
//         node() : body(NULL), center(glm::dvec3(0.0)), father(NULL), mass(0.0), barycenter(glm::dvec3(0.0)) {
//             for(int i = 0; i < 8; i++){
//                 children[i] = NULL;
//             }
//         }
//         node(celestial_body* body, glm::dvec3 center, node* father) : 
//             body(body), center(center), father(father), mass(body->mass), barycenter(body->position)
//         {
//             for(int i = 0; i < 8; i++){
//                 children[i] = NULL;
//             }
//         }
//         ~node() {
//             for(int i = 0; i < 8; i++){
//                 if(children[i] != NULL) {
//                     delete children[i];
//                 }
//             }
//         }
//     };

//     glm::dvec3 get_next_center(node* node, int direction) {
//         glm::dvec3 next_center = node->center;
//         glm::dvec3 factor = glm::dvec3(1.0, 1.0, 1.0);
//         if(!(direction & 1)) {
//             factor.x = -1.0;
//         }
//         if(!(direction & 2)) {
//             factor.y = -1.0;
//         }
//         if(!(direction & 4)) {
//             factor.z = -1.0;
//         }
//         return next_center + factor * node->semi_edge;
//     }
    
//     void add_body(node* root, celestial_body* body) {
//         int direction;

//         // Case NULL node
//         if(root->mass == 0.0) {
//             root->body = body;
//             root->mass = body->mass;
//             root->barycenter = body->position;
//             return;
//         }
//         else {
//             direction = get_direction(root->body->position, root->center);
//             if(root->children[direction] == NULL) {
//                 root->children[direction] = new node(root->body, get_next_center(root, direction), root);
//                 root->children[direction]->semi_edge = root->semi_edge / 2.0;
//             }
//             else {
//                 add_body(root->children[direction], root->body);
//             }
//             root->body = NULL;
//         }

//         // Case leaf node
//         direction = get_direction(body->position, root->center);
//         if(root->children[direction] == NULL) {
//             root->children[direction] = new node(body, get_next_center(root, direction), root);
//             root->children[direction]->semi_edge = root->semi_edge / 2.0;
//         }
//         else {
//             add_body(root->children[direction], body);
//         }

//         root->barycenter = (root->barycenter * root->mass + body->position * body->mass) / 
//             (root->mass + body->mass);
//         root->mass += body->mass;
//     }

//     void add_gravity(node*& root, celestial_body*& body) { // TODO
//         if(root == NULL || body == NULL) {
//             return;
//         }
//         if(root->body == NULL) {
//             if(glm::length(body->position - root->barycenter) > glm::length(root->semi_edge.x)) {
//                 body->acceleration += 
//                     sim_constants::get_G() * root->mass * (body->position - root->barycenter) / 
//                     glm::dot(body->position - root->barycenter, body->position - root->barycenter);
//             }
//             else {
//                 for(int i = 0; i < 8; i++) {
//                     add_gravity(root->children[i], body);
//                 }
//             }
//         } else {
//             body->acceleration += 
//                 sim_constants::get_G() * root->mass * (body->position - root->body->position) / 
//                 glm::dot(body->position - root->body->position, body->position - root->body->position);
//         }
//     }
// public:
//     static int get_direction(glm::dvec3 pos, glm::dvec3 center) {
//         int direction = 0;
//         if(pos.x > center.x) 
//             direction += 1;
//         if(pos.y > center.y) 
//             direction += 2;
//         if(pos.z > center.z) 
//             direction += 4;
//         return direction;
//     }
//     void simulate(celestial_system*& system) override {
//         node* root = new node();
//         upper_bound.x = upper_bound.y = upper_bound.z = -std::numeric_limits<double>::infinity();
//         lower_bound.x = lower_bound.y = lower_bound.z = std::numeric_limits<double>::infinity();
//         for(auto& body : system->bodies){
//             upper_bound.x = std::max(upper_bound.x, body->position.x);
//             upper_bound.y = std::max(upper_bound.y, body->position.y);
//             upper_bound.z = std::max(upper_bound.z, body->position.z);
//             lower_bound.x = std::min(lower_bound.x, body->position.x);
//             lower_bound.y = std::min(lower_bound.y, body->position.y);
//             lower_bound.z = std::min(lower_bound.z, body->position.z);
//         }
//         root->center = (upper_bound + lower_bound) / 2.0;
//         root->semi_edge = (upper_bound - lower_bound) / 2.0;
//         for(auto& body : system->bodies){
//             add_body(root, body);
//         }
//         for(auto& body : system->bodies){
//             add_gravity(root, body);
//         }
//         delete root;
//     }
//     static simulate_algorithm& get_instance() {
//         return instance;
//     }
// };
