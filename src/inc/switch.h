#include <vector>


class particle {
    public:
        float centre[2];
        float grad[2];
        float length;
        float angle;
        bool connected;

        particle(float size[2],float angleLim,float length);    
};

class sim {
    std::vector<particle> particles;

    float dist(particle& part1,particle& part2);

    public:
    float conductDist;
    int partNum;
    sim(int size,float angleLim, float partLength);
    void printAll(std::string filename);
    bool checkConduct();
};