#ifndef CELESTIAL_REVERIE_H
#define CELESTIAL_REVERIE_H

#include <simulation/celestial_body.h>
#include <simulation/celestial_system.h>
#include <simulation/simulate_algorithm.h>
#include <string>
#include <vector>
#include <glm/glm.hpp>

class celestial_reverie {
private:
    std::vector<celestial_system*> frames;
    celestial_system* current_frame;
    simulate_algorithm* sim_algorithm;
    update_algorithm* upd_algorithm;
    double current_time;
    double time_step;
    double save_interval;

public:
    celestial_reverie();
    ~celestial_reverie();

    bool load_celestial_system(const std::string& filename);
    bool dump_celestial_system(const std::string& filename);
    bool add_celestial_body(celestial_body* body);
    void set_time_step(double time_step);
    void set_save_interval(double save_interval);

    template<typename T>
    void set_simulate_algorithm();

    template<typename T>
    void set_update_algorithm();

    celestial_system* simulate();
    celestial_system* back_to(double time);
};

#endif // CELESTIAL_REVERIE_H