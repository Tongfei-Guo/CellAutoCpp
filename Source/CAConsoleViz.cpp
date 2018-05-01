#include "CAConsoleViz.h"
#include <iostream>
#include <chrono>
#include <thread>
#include <iomanip>
#include <ctime>

void CAConsoleViz::Visualize(int nsteps, int fps){
    if (fps<=0){
        std::cout<<"ERROR: fps<=0 ("<<fps<<")"<<std::endl;
        return;
    }

    auto ts=m_world->get_timestamp();
    if (ts[0].size()>100 || ts.size()>100){
        std::cout<<"ERROR: cannot visualize a world as big as: "
            <<ts[0].size()<<"x"<<ts.size()<<std::endl;
        return;
    }

    srand(time(NULL));

    for(int step=0; step<=nsteps; step++){
        if (step!=0){
            m_world->forall_step(1);
        }
        system("clear");
        auto ts=m_world->get_timestamp();
        for(int y=0; y<ts[0].size(); y++){
            for(int x=0; x<ts.size(); x++){
                std::cout << m_consoleviz_func(&(ts[x][y]));
            }
            std::cout<<std::endl;
        }

        std::cout<<"Round: "<<std::setw(6)<<step<<std::endl;

        std::this_thread::sleep_for(std::chrono::milliseconds(1000/fps));
    }
}
