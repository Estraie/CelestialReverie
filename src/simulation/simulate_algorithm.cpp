# include <vector>
# include <cmath>
# include <algorithm>
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

void forward_euler::update(celestial_system*& sys, double dt, 
                           simulate_algorithm* sim) {
    sim->simulate(sys);
    sys->set_time(sys->get_time() + dt);
    for(auto& body : sys->bodies){
        update(body, dt);
    }
}

implicit_euler& implicit_euler::get_instance() {
    static implicit_euler instance;
    return instance;
}

void implicit_euler::update(celestial_body*& body, double dt) {
    body->velocity += body->acceleration * dt;
    body->position += body->velocity * dt;
}

void implicit_euler::update(celestial_system*& sys, double dt, 
                            simulate_algorithm* sim) {
    
    sim -> simulate(sys);
    auto tmp_sys = sys->duplicate();

    sys->set_time(sys->get_time() + dt);
    for(auto& body : tmp_sys->bodies){
        update(body, dt);
    }

    sim -> simulate(tmp_sys);

    for(size_t i = 0; i < sys->size(); i++){
        sys->bodies[i]->acceleration = 
            (sys->bodies[i]->acceleration + tmp_sys->bodies[i]->acceleration);
        update(sys->bodies[i], dt);
    }

}


// simulate_algorithm::simulate_algorithm(update_algorithm& algorithm) : algorithm(algorithm) {}

simulate_algorithm::simulate_algorithm(){}

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
        G * other -> mass * glm::normalize(dist_vec) / glm::dot(dist_vec, dist_vec);
    other->acceleration -= 
        G * body->mass * glm::normalize(dist_vec) / glm::dot(dist_vec, dist_vec);   
}

void pure_newtonian::simulate(celestial_system*& system) {
    for(auto& body : system->bodies){
        body->acceleration = glm::dvec3(0.0);
    }
    for(size_t i = 0; i < system->bodies.size(); i++){
        for(size_t j = i + 1; j < system->bodies.size(); j++){
            // auto dist_vec = system->bodies[i]->position- system->bodies[j]->position;            
            // if (glm::length(dist_vec) < system->bodies[i]->radius + system->bodies[j]->radius){
            //     system->collide(i,j);
            // }            
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
    // std::cout << "Adding " << *body << "\n";
    // Case NULL node
    if(root->mass == 0.0) {
        // std::cout << "Adding to current Node\n";
        root->body = body;
        root->mass = body->mass;
        root->barycenter = body->position;
        return;
    }
    else {
        // std::cout << "Adding to current Node\n";
        if(root->body != NULL) {
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
                sim_constants::get_G() * root->mass * glm::normalize(root->barycenter - body->position) / 
                glm::dot(body->position - root->barycenter, body->position - root->barycenter);
        }
        else {
            for(int i = 0; i < 8; i++) {
                add_gravity(root->children[i], body);
            }
        }
    } else if (root->body != body){
        body->acceleration += 
            sim_constants::get_G() * root->mass * glm::normalize(root->body->position - body->position) / 
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
        body->acceleration = glm::dvec3(0.0);
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

bool collision_detection::is_colliding(celestial_body* body1, celestial_body* body2) {
    if(!body1 || !body2)
        return false;
    auto dist_vec = body2->position - body1->position;
    double dist = glm::length(dist_vec);
    return dist < body1->radius + body2->radius;
}

celestial_body* collision_detection::merge(celestial_body* body1, celestial_body* body2) {
    double mass = body1->mass + body2->mass;
    double radius = std::cbrt(
        body1->radius * body1->radius * body1->radius + 
        body2->radius * body2->radius * body2->radius
    );
    glm::dvec3 position = (body1->position * body1->mass + body2->position * body2->mass) / mass;
    glm::dvec3 velocity = (body1->velocity * body1->mass + body2->velocity * body2->mass) / mass;
    glm::dvec3 acceleration = (body1->acceleration * body1->mass + body2->acceleration * body2->mass) / mass;
    glm::dvec3 color = glm::max(body1->color, body2->color);
    celestial_body* new_body;
    if(body1->is_emissive() || body2->is_emissive()) {
        new_body = new star(
            mass, radius, position, velocity, acceleration, color
        );
    } else {
        new_body = new celestial_body(
            mass, radius, position, velocity, acceleration, color
        );
    }
    return new_body;
}

void dummy_detection::detect(celestial_system*& system) {

}
collision_detection& dummy_detection::get_instance() {
    static dummy_detection instance;
    return instance;
}

ocd::AABB::AABB(glm::dvec3 lower_bound, glm::dvec3 upper_bound): lower_bound(lower_bound), upper_bound(upper_bound) {}
bool ocd::AABB::contains(const glm::dvec3& point) const {
    return (point.x >= lower_bound.x && point.x <= upper_bound.x &&
            point.y >= lower_bound.y && point.y <= upper_bound.y &&
            point.z >= lower_bound.z && point.z <= upper_bound.z);
}

glm::dvec3 ocd::AABB::center() const {
    return (lower_bound + upper_bound) / 2.0;
}

glm::dvec3 ocd::AABB::size() const {
    return upper_bound - lower_bound;
}

ocd::OctreeNode::OctreeNode(const ocd::AABB& aabb): boundary(aabb), is_leaf(true){}

ocd::OctreeNode::~OctreeNode() {
    for(int i = 0; i < 8; i++) {
        if(children[i] != NULL) {
            delete children[i];
            children[i] = NULL;
        }
    }
}

void ocd::OctreeNode::insert(celestial_body* body, size_t index) {
    if (!boundary.contains(body->position)) return;
    count++;
    if (is_leaf) {
        bodies.push_back(body);
        indices.push_back(index);
        if (bodies.size() > 30) {
            subdivide();
        }
    } else {
        for (auto& child : children) {
            if (child) child->insert(body, index);
        }
    }
}

void ocd::OctreeNode::subdivide() {
    is_leaf = false;
    glm::dvec3 center = boundary.center();
    glm::dvec3 size = boundary.size() / 2.0;

    children[0] = new OctreeNode(AABB(boundary.lower_bound, center));
    children[1] = new OctreeNode(AABB(glm::dvec3(center.x, boundary.lower_bound.y, boundary.lower_bound.z), glm::dvec3(boundary.upper_bound.x, center.y, center.z)));
    children[2] = new OctreeNode(AABB(glm::dvec3(boundary.lower_bound.x, center.y, boundary.lower_bound.z), glm::dvec3(center.x, boundary.upper_bound.y, center.z)));
    children[3] = new OctreeNode(AABB(glm::dvec3(center.x, center.y, boundary.lower_bound.z), glm::dvec3(boundary.upper_bound.x, boundary.upper_bound.y, center.z)));
    children[4] = new OctreeNode(AABB(glm::dvec3(boundary.lower_bound.x, boundary.lower_bound.y, center.z), glm::dvec3(center.x, center.y, boundary.upper_bound.z)));
    children[5] = new OctreeNode(AABB(glm::dvec3(center.x, boundary.lower_bound.y, center.z), glm::dvec3(boundary.upper_bound.x, center.y, boundary.upper_bound.z)));
    children[6] = new OctreeNode(AABB(glm::dvec3(boundary.lower_bound.x, center.y, center.z), glm::dvec3(center.x, boundary.upper_bound.y, boundary.upper_bound.z)));
    children[7] = new OctreeNode(AABB(center, boundary.upper_bound));

    std::vector<celestial_body*> old_bodies = bodies;
    std::vector<size_t> old_indices = indices;
    bodies.clear();
    indices.clear();
    for (size_t i = 0; i < old_bodies.size(); ++i) {
        for (auto& child : children) {
            if (child) child->insert(old_bodies[i], old_indices[i]);
        }
    }
}

std::vector<std::pair<size_t, size_t>> ocd::OctreeNode::check_collisions() {
    std::vector<std::pair<size_t, size_t>> collisions;
    for (size_t i = 0; i < bodies.size(); ++i) {
        for (size_t j = i + 1; j < bodies.size(); ++j) {
            if (is_colliding(bodies[i], bodies[j])) {
                collisions.push_back({indices[i], indices[j]});
            }
        }
    }

    if (!is_leaf) {
        for (auto& child : children) {
            if (child) {
                auto child_collisions = child->check_collisions();
                collisions.insert(collisions.end(), child_collisions.begin(), child_collisions.end());
            }
        }
    }

    return collisions;
}

void ocd::detect(celestial_system*& sys) {
    glm::dvec3 lower_bound = glm::dvec3(std::numeric_limits<double>::infinity());
    glm::dvec3 upper_bound = glm::dvec3(-std::numeric_limits<double>::infinity());
    for (celestial_body*& body : sys->bodies) {
        upper_bound.x = std::max(upper_bound.x, body->position.x + body->radius);
        upper_bound.y = std::max(upper_bound.y, body->position.y + body->radius);
        upper_bound.z = std::max(upper_bound.z, body->position.z + body->radius);
        lower_bound.x = std::min(lower_bound.x, body->position.x - body->radius);
        lower_bound.y = std::min(lower_bound.y, body->position.y - body->radius);
        lower_bound.z = std::min(lower_bound.z, body->position.z - body->radius);
    }
    bound = AABB(lower_bound, upper_bound);
    root = new OctreeNode(bound);
    for(size_t i = 0; i < sys->bodies.size(); i++) {
        root->insert(sys->bodies[i], i);
    }
    // std::cout << "Built with" << root->count << " bodies\n";
    auto collisions = root->check_collisions();
    std::vector<celestial_body*> merged_bodies;
    for(auto& collision : collisions) {
        if(sys->bodies[collision.first] && sys->bodies[collision.second]) {
            celestial_body* merged = collision_detection::merge(sys->bodies[collision.first], sys->bodies[collision.second]);
            merged_bodies.push_back(merged);
            delete sys->bodies[collision.first];
            sys->bodies[collision.first] = nullptr;
            delete sys->bodies[collision.second];
            sys->bodies[collision.second] = nullptr;
        }
    }

    sys->bodies.erase(std::remove_if(sys->bodies.begin(), sys->bodies.end(), [](celestial_body* ptr) {return ptr == nullptr; }), sys->bodies.end());
    // sys->bodies.erase(std::remove(sys->bodies.begin(), sys->bodies.end(), nullptr), sys->bodies.end());
    sys->bodies.insert(sys->bodies.end(), merged_bodies.begin(), merged_bodies.end());
    delete root;
    root = NULL;
}

collision_detection& ocd::get_instance() {
    static ocd instance;
    return instance;
}
