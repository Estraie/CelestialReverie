#ifndef SIMULATE_ALGORITHM_H
#define SIMULATE_ALGORITHM_H

# include <vector>
# include <glm/glm.hpp>
# include <simulation/celestial_body.h>
# include <simulation/celestial_system.h>

class sim_constants {
public:
    static const double G;
    static double get_G() { return G; }
};

class simulate_algorithm {
protected:
    // update_algorithm& algorithm;

    explicit simulate_algorithm();
public:
    virtual ~simulate_algorithm() = default;

    // void set_update_algorithm(update_algorithm& new_algorithm);
    virtual void simulate(celestial_system*& system) = 0;
    static simulate_algorithm& get_instance();
};

class update_algorithm {
public:
    virtual ~update_algorithm() = default;
    virtual void update(celestial_body*& body, double dt) = 0;
    virtual void update(celestial_system*& system, double dt,
                        simulate_algorithm*& sim) = 0;
};

class forward_euler : public update_algorithm {
public:
    forward_euler(const forward_euler&) = delete;
    forward_euler& operator=(const forward_euler&) = delete;

    static forward_euler& get_instance();
    void update(celestial_body*& body, double dt) override;
    void update(celestial_system*& system, double dt, 
                simulate_algorithm*& sim) override;

private:
    forward_euler() = default;
};

class implicit_euler : public update_algorithm{
public:
    implicit_euler(const implicit_euler&) = default;
    implicit_euler& operator=(const implicit_euler&) = default;

    static implicit_euler& get_instance();
    void update(celestial_body*& body, double dt) override;
    void update(celestial_system*& system, double dt, 
                simulate_algorithm*& sim) override;

private:
    implicit_euler() = default;
};

class pure_newtonian : public simulate_algorithm {
protected:
    static pure_newtonian instance;
    pure_newtonian() = default;

    virtual void add_gravity(celestial_body*& body, celestial_body*& other);

public:
    virtual void simulate(celestial_system*& system) override;
    static simulate_algorithm& get_instance();
};

class barnes_hut : public simulate_algorithm {
protected:
    static barnes_hut instance;
    glm::dvec3 upper_bound;
    glm::dvec3 lower_bound;
    static constexpr double theta = 0.5;
    class node {
    public:
        celestial_body* body;
        glm::dvec3 center;
        glm::dvec3 barycenter;
        glm::dvec3 semi_edge;
        double mass;
        node* father;
        node* children[8];

        node();
        node(celestial_body* body, glm::dvec3 center, node* father);
        ~node();
    };

    glm::dvec3 get_next_center(node* node, int direction);
    void add_body(node* root, celestial_body* body);
    virtual void add_gravity(node*& root, celestial_body*& body);

    barnes_hut() = default;

public:
    static int get_direction(glm::dvec3& pos, glm::dvec3& center);
    virtual void simulate(celestial_system*& system) override;
    static simulate_algorithm& get_instance();
};

#endif // SIMULATE_ALGORITHM_H
