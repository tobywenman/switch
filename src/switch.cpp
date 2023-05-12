#define _USE_MATH_DEFINES
#include <math.h>

#include "inc/switch.h"
#include "inc/matplotlibcpp.h"

#include <iostream>
#include <ctime>
#include <cmath>
#include <stack>
#include <limits>
#include <array>

namespace plt = matplotlibcpp;

particle::particle(float size[2],float angleLim,float length){



    centre.x = static_cast <float> (rand()) / (static_cast <float> (RAND_MAX/size[0]));
    centre.y = static_cast <float> (rand()) / (static_cast <float> (RAND_MAX/size[1]));

    this->length = length;
    this->connected = false;
    this->angle = -angleLim + static_cast <float> (rand()) /( static_cast <float> (RAND_MAX/(angleLim+angleLim)));

    grad[0] = std::cos(this->angle);
    grad[1] = std::sin(this->angle);

    float mag = sqrt(pow(grad[0],2)+pow(grad[1],2));

    grad[0] /= mag;
    grad[1] /= mag;
    
    this->ends[0] = centre.addOffset(grad,length/2);
    this->ends[1] = centre.addOffset(grad,-length/2);
}

point::point(float x, float y){
    this->x = x;
    this->y = y;
}

point point::addOffset(float grad[2],float length){
    point tempPoint(x+(grad[0]*length),y+(grad[1]*length));
    return tempPoint;
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
        std::vector<float> x;
        std::vector<float> y;

        x.push_back(particles[i].ends[0].x);
        x.push_back(particles[i].ends[1].x);

        y.push_back(particles[i].ends[0].y);
        y.push_back(particles[i].ends[1].y);

        plt::plot(x,y);
    }
}

float sim::dist(particle& part1,particle& part2, bool plot=false){
    if (this->intersect(part1, part2)){
        return 0;
    }

    float minDist = std::numeric_limits<float>::infinity();

    point minPoints[2];

    point tempPoint;

    for (int i=0; i<2; i++){
        tempPoint = part1.distToPoint(part2.ends[i]);
        std::cout << "tempPoint test: " << tempPoint.x << ',' << tempPoint.y << '\n';
        std::cout << "i value: " << i << '\n';
        std::cout << "end pos: " << part2.ends[i].x << ',' << part2.ends[i].y << '\n';
        for (int j=0; j<2; j++){
            float tempDist = sqrt(pow(tempPoint.x-part2.ends[i].x,2)+pow(tempPoint.y-part2.ends[i].y,2));
            if (tempDist < minDist){
                minDist = tempDist;
                minPoints[0] = tempPoint;

                minPoints[1] = part2.ends[i];
            }
        }
        std::cout << "minDist: " << minDist << '\n';
        std::cout << "minpoinnts[1]: " << minPoints[1].x << ',' << minPoints[1].y << '\n';
    }

    for (int i=0; i<2; i++){
        tempPoint = part2.distToPoint(part1.ends[i]);
        std::cout << tempPoint.x << ',' << tempPoint.y << '\n';
        for (int j=0; j<2; j++){
            float tempDist = sqrt(pow(tempPoint.x-part1.ends[i].x,2)+pow(tempPoint.y-part1.ends[i].y,2));
            if (tempDist < minDist){
                minDist = tempDist;
                minPoints[0] = tempPoint;

                minPoints[1] = part1.ends[i];
            }
        }
    }

    if (plot){



        std::vector<float> x;
        std::vector<float> y;

        x.push_back(minPoints[0].x);
        x.push_back(minPoints[1].x);
        y.push_back(minPoints[0].y);
        y.push_back(minPoints[1].y);

        plt::plot(x,y,{{"linestyle", "--"}});
    }

    return minDist;

}

// bool sim::checkConduct(){
//     std::stack<particle*> checkStack;

//     for (int i=0; i<partNum;i++){
//         if (particles[i].centre[0]-particles[i].grad[0]*(particles[i].length/2)-conductDist < 0){
//             particles[i].connected = true;
//             checkStack.push(&particles[i]);
//         }
//     }  
// }

point particle::distToPoint(point testPoint){
    float t = (grad[0]*testPoint.x) - (grad[0]*centre.x) + (grad[1]*testPoint.y) - (grad[1]*centre.y);

    if (t > length/2){
        t = length/2;
    }
    else if (t < -length/2){
        t = -length/2;
    }

    return centre.addOffset(grad,t);

}

bool sim::ccw(point A, point B, point C){
    return (C.y-A.y) * (B.x-A.x) > (B.y-A.y) * (C.x-A.x);
}

bool sim::intersect(particle& part1, particle& part2){
    return (ccw(part1.ends[0],part2.ends[0],part2.ends[1]) != ccw(part1.ends[1],part2.ends[0],part2.ends[1])) && (ccw(part1.ends[0],part1.ends[1],part2.ends[0]) != ccw(part1.ends[0],part1.ends[1],part2.ends[1]));
}

int main(){
    sim newSim(2,90,1.5);

    newSim.printAll();

    float dist;

    dist = newSim.dist(newSim.particles[0],newSim.particles[1],true);
    
    std::cout << "Dist: " << dist << '\n';

    plt::show();
    return 0;
}