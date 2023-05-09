#define _USE_MATH_DEFINES
#include <math.h>

#include <fstream>

#include "inc/switch.h"
#include "inc/matplotlibcpp.h"

#include <cstdlib>
#include <iostream>
#include <ctime>
#include <cmath>
#include <stack>

namespace plt = matplotlibcpp;

particle::particle(float size[2],float angleLim,float length){
    for(int i=0; i<2;i++){
        centre[i] = static_cast <float> (rand()) / (static_cast <float> (RAND_MAX/size[i]));
    }

    this->length = length;
    this->connected = false;
    this->angle = -angleLim + static_cast <float> (rand()) /( static_cast <float> (RAND_MAX/(angleLim+angleLim)));
    grad[0] = std::cos(this->angle);
    grad[1] = std::sin(this->angle);

    float mag = sqrt(pow(grad[0],2)+pow(grad[1],2));

    grad[0] /= mag;
    grad[1] /= mag;

    for (int i = 0; i < 2; i++){
        this->ends[i][0] = centre[i]-grad[i];
        this->ends[i][1] = centre[i]+grad[i];
    }

}

sim::sim(int partNum,float angleLim, float partLength){

    std::srand((unsigned) std::time(NULL));

    float angle = angleLim * (M_PI/180);
    this->conductDist = 1;
    this->partNum = partNum;

    float gridSize[2] = {10,10};
    for (int i=0; i<partNum;i++){
        particles.push_back(particle(gridSize,angle,partLength));
    }

    

}

void sim::printAll(){



    for (int i=0; i<partNum;i++){
        std::vector<float> x (particles[i].ends[0],particles[i].ends[0]+2);
        std::vector<float> y (particles[i].ends[1],particles[i].ends[1]+2);
        
        plt::plot(x,y);
    }
}

float sim::dist(particle& part1,particle& part2){
    
}

bool sim::checkConduct(){
    std::stack<particle*> checkStack;

    for (int i=0; i<partNum;i++){
        if (particles[i].centre[0]-particles[i].grad[0]*(particles[i].length/2)-conductDist < 0){
            particles[i].connected = true;
            checkStack.push(&particles[i]);
        }
    }  


}

int main(){
    sim newSim(30,45,1.5);

    newSim.printAll();

    plt::show();
    return 0;
}