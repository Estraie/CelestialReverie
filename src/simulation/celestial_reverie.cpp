# include <simulation/celestial_body.h>
# include <simulation/celestial_system.h>
# include <simulation/simulate_algorithm.h>
# include <string>
# include <vector>
# include <glm/glm.hpp>

class celestial_reverie {
private:
    std::vector<celestial_system*> frames;
    celestial_system* current_frame;
    simulate_algorithm* sim_algorithm;
    update_algorithm* upd_algorithm;
    double current_time = 0;
    double time_step;
    double save_interval;
public:
    celestial_reverie() {
        sim_algorithm = &pure_newtonian::get_instance();
        upd_algorithm = &forward_euler::get_instance();
        time_step = 1;
        save_interval = 100;
        current_frame = new celestial_system();
    }
    ~celestial_reverie() {}
    bool load_celestial_system(const std::string& filename) {}
    bool dump_celestial_system(const std::string& filename) {}
    bool add_celestial_body(celestial_body* body) {
        current_frame->add_body(body);
        return true;
    }
    void set_time_step(double time_step) {
        this->time_step = time_step;
    }
    void set_save_interval(double save_interval) {
        this->save_interval = save_interval;
    }

    template<typename T>
    void set_simulate_algorithm() {
        sim_algorithm = &T::get_instance();
    }

    template<typename T>
    void set_update_algorithm() {
        upd_algorithm = &T::get_instance();
    }

    celestial_body* simulate(){
        if(frames.size() == 0 || current_time - frames.back()->time > save_interval) {
            frames.push_back(current_frame->duplicate());
        }
        sim_algorithm->simulate(current_frame);
        upd_algorithm->update(current_frame, time_step);
    }
    celestial_body* back_to(double time){
        return NULL;
    }
};