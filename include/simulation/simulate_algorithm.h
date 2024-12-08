#ifndef SIMULATE_ALGORITHM_H
#define SIMULATE_ALGORITHM_H

# include <vector>
# include <glm/glm.hpp>
# include <simulation/celestial_body.h>
# include <simulation/celestial_system.h>

class sim_constants {
public:
    static const double G;
    static const double Ray_Speed;
    static double get_G() { return G; }
    static double get_c() { return Ray_Speed; }
};

class update_algorithm {
public:
    virtual ~update_algorithm() = default;
    virtual void update(celestial_body*& body, double dt) = 0;
    virtual void update(celestial_system*& system, double dt) = 0;
};

class forward_euler : public update_algorithm {
public:
    forward_euler(const forward_euler&) = delete;
    forward_euler& operator=(const forward_euler&) = delete;

    static forward_euler& get_instance();
    void update(celestial_body*& body, double dt) override;
    void update(celestial_system*& system, double dt) override;

private:
    forward_euler() = default;
};

class simulate_algorithm {
protected:
    update_algorithm& algorithm;

    explicit simulate_algorithm(update_algorithm& algo = forward_euler::get_instance());
public:
    virtual ~simulate_algorithm() = default;

    void set_update_algorithm(update_algorithm& new_algorithm);
    virtual void simulate(celestial_system*& system) = 0;
    static simulate_algorithm& get_instance();
};

class pure_newtonian : public simulate_algorithm {
private:
    static pure_newtonian instance;
    pure_newtonian() = default;

    void add_gravity(celestial_body*& body, celestial_body*& other);

public:
    void simulate(celestial_system*& system) override;
    static simulate_algorithm& get_instance();
};

class barnes_hut : public simulate_algorithm {
private:
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
    void add_gravity(node*& root, celestial_body*& body);

    barnes_hut() = default;

public:
    static int get_direction(glm::dvec3& pos, glm::dvec3& center);
    void simulate(celestial_system*& system) override;
    static simulate_algorithm& get_instance();
};

class pure_newtonian_sr : public simulate_algorithm {
private:
    static pure_newtonian_sr instance;
    pure_newtonian_sr() = default;

    void add_gravity(celestial_body*& body, celestial_body*& other);
    void cal_acceleration(celestial_body*& body);

public:
    void simulate(celestial_system*& system) override;
    static simulate_algorithm& get_instance();
};

#endif // SIMULATE_ALGORITHM_H
