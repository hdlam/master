#include <Boids.h>


void printVector(Vector2D v){
    Serial.print(v.x);
    Serial.print(",");
    Serial.println(v.y);
}
//publics
Boids::Boids(float x, float y, float ang){
    pos.x = x;
    pos.y = y;
    this->angle = ang;
}

void Boids::update(const Boids other[]){
    moveCalc(other);
    vel.add(acc);
    vel.limit(maxSpeed);
    acc.reset();
    Serial.print("VEL is: ");
    printVector(vel);
}


//getters/setters
void Boids::setPos(float x, float y){
    pos.x = x;
    pos.y = y;
}

void Boids::setVel(float x, float y){
    vel.x = x;
    vel.y = y;
}

float Boids::getVelX(){
    return vel.x;
}
float Boids::getVelY(){
    return vel.y;
}

void Boids::setAngle(float angle){
    this->angle = angle;

}

float Boids::getAngle(){
    return this->angle;
}

void Boids::rotate(float rotAmount){
    angle += rotAmount;
    vel.rotate(rotAmount);

}
byte Boids::getNumOfBots(){
    return this->numOfRobs;
}

//privates

Vector2D& Boids::steerTo(Vector2D& target){
    Vector2D des(target.x,target.y); //makes sure that we get a copy
    des.sub(pos);
    float d = des.length();
    if(d > 0){
        des.normalize();
        des.mul(maxForce);
        des.sub(vel);
        return des;
    }
    Vector2D temp;
    return temp;
}

/*Vector2D& Boids::cohesion(const Boids other[]){
    Vector2D coh(0,0);
    coh.add(other[numOfRobs-1].pos);
    return steerTo(coh);

}*/

Vector2D& Boids::cohesion(const Boids other[]){
    Vector2D coh(0,0);
    byte count = 0;
    for(byte i = 0; i < numOfRobs-1; i++){
        if(pos.dist(other[i].pos) < cohDist){
            coh.add(other[i].pos);
            count++;
        }
        /*if(pos.dist(other[i].pos) > aliDist){
            coh.add(other[i].pos);
            count++;
        }*/
    }
    if(count > 0){
        coh.div(count);
    }
    else{ 
        return coh;
    }
    return steerTo(coh);
}

Vector2D& Boids::alignment(const Boids other[]){
    Vector2D ali(0,0);
    byte count = 0;
    for(byte i = 0; i < numOfRobs-1; i++){
        float dist = pos.dist(other[i].pos);
        if(dist < aliDist){
            ali.add(other[i].vel);
            count++;
        }
    }
    if(count > 0)
        ali.div(count);

    ali.normalize();
    ali.mul(maxForce);

    return ali;
}

Vector2D& Boids::separation(const Boids other[]){
    Vector2D sep(0,0);
    byte count = 0;
    for(byte i = 0; i < numOfRobs-1; i++){
        float dist = pos.dist(other[i].pos);
        if(dist < sepDist && dist > 0){
            Vector2D diff(pos.x, pos.y);
            diff.sub(other[i].pos);
            diff.normalize();
            diff.div(dist);
            sep.add(diff);
            count++;
            Serial.print("got a robot near!!!! DIST WAS:");
            Serial.println(dist);
        }

    }
    if(count > 0)
        sep.div(count);
    if(sep.length() > 0){
        sep.normalize();
        sep.mul(60);
        sep.sub(vel);
    }
    //Serial.print("separation: ");
    //printVector(sep);
    return sep;


}

Vector2D& Boids::awayFromWall(const Boids other[]){
    Vector2D awayFromWall;
    if(pos.x > wallX - sepDist/2){
        awayFromWall.x = -30;
    }
    else if(pos.x < sepDist/2){
        awayFromWall.x = 30;
    }
    if(pos.y > wallY - sepDist/2){
        awayFromWall.y = -30;
    }
    else if(pos.y < sepDist/2){
        awayFromWall.y = 30;
    }
    return awayFromWall;
}


void Boids::moveCalc(const Boids other[]){
    Vector2D coh = cohesion(other);
    Vector2D ali = alignment(other);
    Vector2D sep = separation(other);
    Vector2D afw = awayFromWall(other);


    Serial.println("coh");
    printVector(coh);
    Serial.println("ali");
    printVector(ali);
    Serial.println("sep");
    printVector(sep);


    coh.mul(2);
    ali.mul(2);
    sep.mul(4);
    afw.mul(1);

    applyForce(coh);
    applyForce(ali);
    applyForce(sep);
    applyForce(afw);
    //Serial.println("ACC is (after applyForce)");
    //printVector(acc);



}

void Boids::applyForce(const Vector2D& force){
    acc.add(force);
}

boolean Boids::boidsNearby(const Boids other[]){
    for(byte i = 0; i < numOfRobs-1; i++){
        float dist = pos.dist(other[i].pos);
        if(dist < sepDist && dist > 0)
            return true;
    }
    return false;
}
