#include <vector>
#include <array>

struct point
{
    float x;
    float y;

    point(float x=0, float y=0);

    point addOffset(float grad[2], float length);
};

class particle {
    public:
        point centre;
        float grad[2];
        point ends[2];
        float length;
        float angle;
        bool connected;

        point distToPoint(point testPoint);

        particle(float size[2],float angleLim,float length);    
};

class sim {
    

    
    bool ccw(point A, point B, point C);
    bool intersect(particle& part1, particle& part2);  



    public:
        float dist(particle& part1,particle& part2, bool plot);
        std::vector<particle> particles;
        float conductDist;
        int partNum;
        sim(int size,float angleLim, float partLength);
        void printAll();
        bool checkConduct();
             
        
};


