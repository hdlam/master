#include <Chirp.h>
#include <Vector2D.h>

Vector2D pos; //needed?
Vector2D vel;
Vector2D acc;

float angle;
float maxForce;
float maxSpeed;


Vector2D& steerTo(Vector2D& target){
    Vector2D des = target;
    des.sub(pos);
    float d = des.length();
    if(d> 0){
        des.normalize();
        des.mul(maxForce);
        des.sub(vel);
        return des;
    }
    Vector2D temp;
    return temp;
}
Vector2D& cohesion(){
    Vector2D coh(0,0);
    byte count = 0;
    //do cohesion
    return coh;
}

Vector2D& alignment(){
    Vector2D ali(0,0);
    byte count = 0;
    //do alignment
    return ali;
}

Vector2D& separation(){
    Vector2D sep(0,0);
    return sep;
}



void applyForce(Vector2D& force){
    acc.add(force);
}

void updatePos(){
    //update the position and make the motor do its stuff
    vel.add(acc);
    vel.limit(maxSpeed);
}

void setup()
{
    
    Serial.begin(9600);
}

void moveCalc(){
    Vector2D coh = cohesion();
    Vector2D sep = separation();
    Vector2D ali = alignment();
    
    coh.mul(2);
    sep.mul(3);
    ali.mul(2);    
    
    applyForce(coh);
    applyForce(coh);
    applyForce(coh);

}

void loop()
{
    moveCalc();
    acc.reset();


}


