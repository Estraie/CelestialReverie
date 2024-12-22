# include <vector>
# include <simulation/celestial_body.h>
# include <simulation/celestial_system.h>

celestial_system::~celestial_system(){
    for(auto body : bodies){
        delete body;
    }
}

celestial_system::celestial_system(double time) : time(time){}

void celestial_system::add_body(celestial_body* body){
    bodies.push_back(body);
}

void celestial_system::set_time(double new_time){
    time = new_time;
}

double celestial_system::get_time(){
    return time;
}

celestial_system* celestial_system::duplicate(){
    celestial_system* new_system = new celestial_system();
    for(auto body : bodies){
        new_system->add_body(body->duplicate());
    }
    return new_system;
}

std::ostream& operator<<(std::ostream& os, celestial_system& sys){
//    os << "***** Celestial System *****" << std::endl;
//    os << "Time: " << sys.get_time() << std::endl;
    for(auto body : sys.bodies){
        *body << os;
//        os << *body;
    }
    return os;
}

int celestial_system::size(){
    return bodies.size();
}

void celestial_system::del_body(celestial_body* body){
    auto it = bodies.begin();
    delete body;
    while (it!= bodies.end() && *it != body) {
        ++it;
    }
    bodies.erase(it);
}

void celestial_system::collide(int i, int j){
    
    auto moment_a = bodies[i]->velocity * bodies[i]->mass;
    auto moment_b = bodies[j]->velocity * bodies[j]->mass;
    auto mass = bodies[i]->mass + bodies[j]->mass;
    auto velocity = (moment_a+moment_b)/mass;
    del_body(bodies[i]);
    bodies[j]->velocity = velocity;
    bodies[j]->mass = mass;

}