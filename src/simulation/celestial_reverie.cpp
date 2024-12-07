#include <simulation/celestial_reverie.h>

celestial_reverie::celestial_reverie() 
    : current_time(0), time_step(1), save_interval(100) {
    sim_algorithm = &pure_newtonian::get_instance();
    upd_algorithm = &forward_euler::get_instance();
    current_frame = new celestial_system();
}

celestial_reverie::~celestial_reverie() {
    delete current_frame;
    for (auto frame : frames) {
        delete frame;
    }
}

bool celestial_reverie::load_celestial_system(const std::string& filename) {
    // Implementation here
    return false;
}

bool celestial_reverie::dump_celestial_system(const std::string& filename) {
    // Implementation here
    return false;
}

bool celestial_reverie::add_celestial_body(celestial_body* body) {
    current_frame->add_body(body);
    return true;
}

void celestial_reverie::set_time_step(double time_step) {
    this->time_step = time_step;
}

void celestial_reverie::set_save_interval(double save_interval) {
    this->save_interval = save_interval;
}

celestial_system* celestial_reverie::simulate() {
    if (frames.empty() || current_time - frames.back()->time > save_interval) {
        frames.push_back(current_frame->duplicate());
    }
    sim_algorithm->simulate(current_frame);
    upd_algorithm->update(current_frame, time_step);
    return current_frame;
}

celestial_system* celestial_reverie::back_to(double time) {
    // Implementation here
    return nullptr;
}

// Template implementations
// template<typename T>
// void celestial_reverie::set_simulate_algorithm() {
//     sim_algorithm = &T::get_instance();
// }

// template<typename T>
// void celestial_reverie::set_update_algorithm() {
//     upd_algorithm = &T::get_instance();
// }
