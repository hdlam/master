#include <Chirp.h>
#include <Boids.h>

Boids other[3];
Boids me(20,15, PI);

void debug(Boids b){
}

void setup(){
    Serial.begin(9600);
    other[0].setPos(50,50);
    other[0].setAngle(PI);
    other[0].setVel(10,20);

    other[1].setPos(30,10);
    other[1].setAngle((float)PI/3.0);
    other[1].setVel(14,40);

    other[2].setPos(5,50);
    other[2].setAngle((float)3*PI/2);
    other[2].setVel(15,30);
}



void loop(){
    me.update(other);
}



/*#include <Chirp.h>
#include <Vector2D.h>

Vector2D pos;
Vector2D vel;
Vector2D acc;

float angle;
float maxForce;
float maxSpeed;

float sepDist = 25;
float aliDist = 50;
float cohDist = 100;


Vector2D& steerTo(Vector2D& target){
    Vector2D des(target.x,target.y); //makes sure that we get a copy
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
Vector2D other(0,0);
Vector2D& cohesion(){
    Vector2D coh(0,0);
    byte count = 0;
    //get data
    for(byte i = 0; i < 4; i++){
        if(true){
            coh.add(other);
            count++;
        }

    }
    if(count > 0){
        coh.div(count);
    }
    else
        return coh;
    return steerTo(coh);
}

Vector2D& alignment(){
    Vector2D ali(0,0);
    byte count = 0;
    //do alignment
    for(byte i = 0; i < 4; i++){
        float dist = pos.dist(other);
        if(dist < aliDist){
            ali.add(other.vel);
            count++;
        }
    }
    if(count > 0)
        ali.div(count);
    
    ali.normalize();
    avg.mul(maxForce);

    return ali;
}

Vector2D& separation(){
    Vector2D sep(0,0);
    byte count = 0;
    for(byte i = 0; i < 4; i++){
        float dist = pos.dist(other);
        if(dist < sepDist){
            Vector2D diff(pos.x, pos.y);
            diff.sub(other.pos);
            diff.normalize();
            diff.div(dist);
            sep.add(diff);
            count++;
        }
    } 
    if(count > 0)
        sep.div(count);
    if(sep.length()){
        sep.normalize();
        sep.mul(60);
        sep.sub(vel);
    }
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

*/
