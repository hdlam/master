#include <Vector2D.h>


class Boids
{
    public:
        Vector2D pos;
        Vector2D vel; //is 0,0
        Vector2D acc; //same
        float angle; //should be set in the start

        float maxSpeed = 30;
        float maxForce = 100;

        float sepDist = 150;
        float aliDist = 200;
        float cohDist = 800;

        Boids(float x = 0, float y = 0, float ang = 0);
        void update(const Boids other[]);

        void setPos(float x, float y);

        void setVel(float x, float y);
        float getVelX();
        float getVelY();


        void setAngle(float angle);
        float getAngle();

        void rotate(float rotAmount);

        boolean boidsNearby(const Boids other[]);
        friend void debug(Boids b);
        
        byte getNumOfBots();
    private:
        byte numOfRobs = 4;
        int wallX = 800;
        int wallY = 652;
        Vector2D& steerTo(Vector2D& target);
        Vector2D& cohesion(const Boids other[]);
        Vector2D& alignment(const Boids other[]);
        Vector2D& separation(const Boids other[]);
        Vector2D& awayFromWall(const Boids other[]);
        void moveCalc(const Boids other[]);
        void applyForce(const Vector2D& force);
};
