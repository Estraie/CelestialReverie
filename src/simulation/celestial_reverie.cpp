#include <simulation/celestial_reverie.h>
#include <algorithm>
#include <fstream>
#include <iostream>
#include <sstream>

celestial_reverie::celestial_reverie() 
    : current_time(0), time_step(1), save_interval(100) {
    sim_algorithm = &pure_newtonian::get_instance();
    upd_algorithm = &implicit_euler::get_instance();
    cd = &dummy_detection::get_instance();
    current_frame = new celestial_system();
}

celestial_reverie::~celestial_reverie() {
    delete current_frame;
    for (auto frame : frames) {
        delete frame;
    }
}

bool celestial_reverie::load_celestial_system(const std::string& filename) {
    std::ifstream infile(filename);
    if (!infile.is_open()) {
        std::cerr << "Failed to open file: " << filename << std::endl;
        return false;
    }

    for(auto frame : frames) {
        delete frame;
        frame = nullptr;
    }
    frames.clear();
    if(current_frame != nullptr) {
        delete current_frame;
        current_frame = nullptr;
    }
    current_frame = new celestial_system();

    std::string line;
    while (std::getline(infile, line)) {
        std::istringstream iss(line);
        double mass, radius;
        glm::dvec3 position, velocity, polar_position, acceleration;

        char delimiter;
        if (
            iss >> 
                mass >> delimiter >> radius >> delimiter >>
                position.x >> delimiter >> position.y >> delimiter >> position.z >> delimiter >>
                velocity.x >> delimiter >> velocity.y >> delimiter >> velocity.z
            //  >> delimiter >>
            // polar_position.x >> delimiter >> polar_position.y >> delimiter >> polar_position.z >> delimiter >>
            // acceleration.x >> delimiter >> acceleration.y >> delimiter >> acceleration.z
        ) {
            celestial_body* body = new celestial_body(mass, radius, position, velocity, polar_position, acceleration);
            current_frame->bodies.push_back(body);
            return true;
        } else {
            std::cerr << "Error parsing line: " << line << std::endl;
            infile.close();
            return false;
        }
        return true;
    }

    infile.close();

    frames.push_back(current_frame);
    return true;
}

bool celestial_reverie::dump_celestial_system(const std::string& filename) {
    std::ofstream outfile(filename);
    if (!outfile.is_open()) {
        std::cerr << "Failed to open file: " << filename << std::endl;
        return false;
    }

    for (const auto& body : current_frame->bodies) {
        outfile 
            << body->mass << ',' << body->radius << ','
            << body->position.x << ',' << body->position.y << ',' << body->position.z << ','
            << body->velocity.x << ',' << body->velocity.y << ',' << body->velocity.z 
            //  << ','
            //  << body->polar_position.x << ',' << body->polar_position.y << ',' << body->polar_position.z << ','
            //  << body->acceleration.x << ',' << body->acceleration.y << ',' << body->acceleration.z
            << '\n';
    }

    outfile.close();
    return true;
}

bool celestial_reverie::add_celestial_body(celestial_body* body) {
    current_frame->add_body(body);
    return true;
}

void celestial_reverie::set_time_step(double time_step) {
    this->time_step = time_step;
}

double celestial_reverie::get_time_step() {
    return time_step;
}

void celestial_reverie::set_save_interval(double save_interval) {
    this->save_interval = save_interval;
}

celestial_system* celestial_reverie::simulate() {
    if (frames.empty() || current_time - frames.back()->time > save_interval) {
        add_buffer();
    }
    //sim_algorithm->simulate(current_frame);
    upd_algorithm->update(current_frame, time_step, sim_algorithm);
    cd->detect(current_frame);
    // std::cout << current_frame->bodies.size() << " bodies\n";
    current_time += time_step;
    return current_frame;
}

celestial_system* celestial_reverie::back_to(double time) {
//    std::cout << "Current time: " << current_time << "; Back to " << time << "\n";
//    return current_frame;
    if(current_frame) {
        delete current_frame;
        current_frame = nullptr;
    }
    auto it = std::lower_bound(frames.begin(), frames.end(), time, [](celestial_system* frame, double time) {
        return frame->time < time;
    });

    if (it == frames.begin()) {
//        std::cout << "Null???\n";

        current_frame = frames[0]->duplicate();
        return current_frame;
    }

    --it;

//    std::cout << "Original Frame: \n" << *current_frame;

    current_frame = (*it)->duplicate();
//    std::cout << "Target Frame: \n" << *current_frame;
    current_time = current_frame->time;

    while (current_time < time) {
        simulate();
    }

    return current_frame;
}

celestial_system* celestial_reverie::get_current_frame() {
    return current_frame;
}

void celestial_reverie::clear_buffer(int size) {
    if(size == 0) {
        size = current_frame->bodies.size();
    }
    for(int i = 0; i < size; i++) {
        delete current_frame->bodies[i];
    }
    current_frame->bodies.erase(
        current_frame->bodies.begin(),
        current_frame->bodies.begin() + size
    );
    for(celestial_system* frame : frames) {
        delete frame;
    }
    frames.clear();
}

void celestial_reverie::add_buffer() {
    frames.push_back(current_frame->duplicate());
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
