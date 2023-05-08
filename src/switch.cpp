#define _USE_MATH_DEFINES
#include <math.h>

#include <fstream>

#include "inc/switch.h"

#include <cstdlib>
#include <iostream>
#include <ctime>
#include <cmath>
#include <stack>

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

void sim::printAll(std::string filename){

    std::ofstream file;

    file.open(filename);

    file << "switch\n";

    for (int i=0; i<partNum;i++){
        file << this->particles[i].centre[0] << ',' << this->particles[i].centre[1] << ',' << (this->particles[i].angle*(180/M_PI)) << ',' << this->particles[i].length << ',' << this->particles[i].connected << ",\n";
    }
    file.close();
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

    newSim.checkConduct();

    newSim.printAll("../src/test.csv");
    return 0;
}