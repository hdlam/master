package boidsSimulatorJava;

import org.newdawn.slick.Color;
import org.newdawn.slick.GameContainer;
import org.newdawn.slick.Graphics;
import org.newdawn.slick.geom.Shape;
import org.newdawn.slick.geom.Vector2f;

import java.lang.reflect.Array;
import java.util.ArrayList;

/**
 * Created with IntelliJ IDEA.
 * Created By: Lyndon Armitage
 * Date: 22/02/13
 * <p/>
 * http://www.red3d.com/cwr/boids/
 */
public class ChirpBoid {

	// http://stackoverflow.com/questions/7586063/how-to-calculate-the-angle-between-two-points-relative-to-the-horizontal-axis
	private float angle = 0f; // remember to use atan
	private Vector2f pos;
	private Vector2f vel;
	private Vector2f acc;
	private Color color;
	private ArrayList<ChirpBoid> bb;
	public boolean debug = false;
	
	private float cohDist = 250f;
	private float sepDist = 120f;
	private float aliDist = 175f;
//	private final float cohDist = 800f;
//	private final float sepDist = 200f;
//	private final float aliDist = 500f;
	
	private float maxSpeed = 60f;
	private float maxF = 50f;
	
	int size = 50;
	boolean move;


	public ChirpBoid(float x, float y, Color color, float velX, float velY, boolean move) {
		pos = new Vector2f(x, y);
		vel = new Vector2f(velX, velY);
		acc = new Vector2f(0,0);
		this.move = move;
		this.color = color;
		
	}

	public Color getColor() {
		return color;
	}

	public Vector2f getPos() {
		return pos;
	}

	public Vector2f getVel() {
		return vel;
	}

	public void render(GameContainer gc, Graphics g) {
		g.setLineWidth(1);
		g.setColor(color);
		g.rotate(pos.x, pos.y, angle);
		g.setColor(color);
		double a = Math.atan2((vel.y), (vel.x));//currentAngle;
		if(bb != null){
			g.setColor(Color.red);
			Vector2f coh = cohesion(bb);
			Vector2f sep = separation(bb);
			Vector2f ali = alignment(bb);
			g.drawLine((float)(pos.x),(float)(pos.y), pos.x+coh.x, pos.y+coh.y);
			g.drawLine((float)(pos.x),(float)(pos.y), pos.x+sep.x, pos.y+sep.y);
			g.drawLine((float)(pos.x),(float)(pos.y), pos.x+ali.x, pos.y+ali.y);
		}
		g.setColor(Color.cyan);
		a = a-90;
		g.drawLine((float)(pos.x),(float)(pos.y),	(float)(pos.x + 5 * Math.cos(a) + 1 * Math.sin(a)), (float)(pos.y + 20 * Math.sin(a) - 1 * Math.cos(a)));
		a = a+180;
		g.drawLine((float)(pos.x),(float)(pos.y),	(float)(pos.x + 5 * Math.cos(a) + 1 * Math.sin(a)), (float)(pos.y + 20 * Math.sin(a) - 1 * Math.cos(a)));
		g.setColor(Color.green);
		g.setLineWidth(5);
		a = a+-90;
		g.drawLine((float)(pos.x),(float)(pos.y),	(float)(pos.x + vel.length() * Math.cos(a) + 1 * Math.sin(a)), (float)(pos.y + vel.length() * Math.sin(a) - 1 * Math.cos(a)));
		g.setColor(color);
		g.drawOval(pos.x - (size / 2), pos.y - (size / 2), size, size);
		g.setColor(Color.white);
		g.setLineWidth(1);
		g.drawOval(pos.x - (sepDist), pos.y - (sepDist ), sepDist*2, sepDist*2);
		g.drawOval(pos.x - (aliDist), pos.y - (aliDist ), aliDist*2, aliDist*2);
		g.drawOval(pos.x - (cohDist), pos.y - (cohDist ), cohDist*2, cohDist*2);
		g.resetTransform();
	}
	

	public void update(GameContainer gc, int delta, ArrayList<ChirpBoid> boids, ArrayList<Shape> obs) {
		if(move){
			moveCalc(boids, delta, obs, gc);
//			if(debug)
				updatePos(gc, delta);
		}
	}
	
	private void moveCalc(ArrayList<ChirpBoid> boi, int delta, ArrayList<Shape> obs, GameContainer gc){
		ArrayList<ChirpBoid> boids = (ArrayList<ChirpBoid>) boi.clone();
		bb = boi;
		Vector2f coh = cohesion(boids);
		Vector2f sep = separation(boids);
		Vector2f ali = alignment(boids);
		Vector2f afw = awayFromWall(boids, gc);
		Vector2f avo = avoidObstacle(obs);
		
		
		coh.scale(2.0f);
	    sep.scale(3.0f);
	    ali.scale(2.0f);
	    afw.scale(2.0f);
	    avo.scale(3.0f);
	    //System.out.println(sep);
		
		applyForce(coh, delta);
		applyForce(sep, delta);
		applyForce(ali, delta);
		applyForce(avo, delta);
		applyForce(afw, delta);
//		if(debug){
//			System.out.println("acc after applied:");
//			System.out.println(acc);
//			System.out.println();
//		}
//		
	}
	
	private void applyForce(Vector2f force, int delta){
		acc.x += force.x/delta;
		acc.y += force.y/delta;
	}

	private void updatePos(GameContainer gc, int delta) {
		
		vel.x += acc.x/delta;
		vel.y += acc.y/delta;
		if(vel.length() > maxSpeed){
			vel.normalise();
			vel.scale(maxSpeed);
		}
		else if(vel.length() > maxSpeed){
			vel.normalise();
			vel.scale(maxSpeed);
		}
		
//		if(debug){
//			System.out.println("velo:");
//			System.out.println(vel);
//		}
		pos.x += vel.x / delta;
		pos.y += vel.y / delta;
		if (pos.x+size/2 > gc.getWidth()) {
			vel.x = 0;
		}
		if (pos.y+size/2 > gc.getHeight()) {
			vel.y = 0;
		}
		if (pos.x-size/2 < 0) {
			vel.x = 0;
		}
		if (pos.y-size/2 < 0) {
			vel.y = 0;
		}
		acc.set(0, 0);
	}



	private float getDistanceToBoid(ChirpBoid target) {
		Vector2f opos = target.getPos();
		return (float) Math.sqrt(Math.pow(opos.x - pos.x, 2) + Math.pow(opos.y - pos.y, 2));
	}
	
	private Vector2f cohesion(ArrayList<ChirpBoid> boids){
		Vector2f coh = new Vector2f(0,0);
		int count = 0;
		for (ChirpBoid b : boids) {
			if (b == this) continue;
			if (getDistanceToBoid(b) < cohDist && b.color == this.color) {
				coh.add(b.pos);
				count++;
			}
		}
		if(count > 0){
			coh.scale(1f/count);
		}
		else{
			return coh;
		}
		
		return steerTo(coh);
	}
	
	
	private Vector2f alignment(ArrayList<ChirpBoid> boids){
		Vector2f avg = new Vector2f(0,0);
		int count = 0;
		for (ChirpBoid b : boids) {
			if(getDistanceToBoid(b) < aliDist && b.color == this.color){
				avg.add(b.vel);
				count++;
			}
		}
		if(count > 0)
			avg.scale(1f/count);
		
		avg.normalise();
		avg.scale(maxF);
		
		
		return avg;
	}
	
	private Vector2f separation(ArrayList<ChirpBoid> boids){
		Vector2f sep = new Vector2f(0,0);
		int count = 0;
		for (ChirpBoid b : boids) {
			if(b == this)
				continue;
			float dist = getDistanceToBoid(b);
			if(dist > 0  && dist < sepDist){
				Vector2f diff = pos.copy();
				diff.sub(b.pos);
				diff.normalise();
				diff.scale(1f/dist);
				sep.add(diff);
				count++;
			}
			
		}
		if(count>0)
			sep.scale(1f/count);
		if(sep.length()>0){
			sep.normalise();
			sep.scale(60f);
//			if(debug){
//				System.out.println(sep);
//				System.out.println();
//			}
			sep.sub(vel);
		}
		
		return sep;
	}
	private Vector2f avoidObstacle(ArrayList<Shape> obs){
		Vector2f sep = new Vector2f(0,0);
		int count = 0;
		for (Shape o : obs) {
			float dist = getDistanceToShape(o);
			if(dist > 0  && dist < sepDist){
				Vector2f diff = pos.copy();
				Vector2f shapePos = new Vector2f(o.getCenterX(), o.getCenterY());
				diff.sub(shapePos);
				diff.normalise();
				diff.scale(1f/dist);
				sep.add(diff);
				count++;
			}
			
		}
		if(count>0)
			sep.scale(1f/count);
		if(sep.length()>0){
			sep.normalise();
			sep.scale(60f);
			sep.sub(vel);
		}
		
		return sep;
	}
	
	private Vector2f awayFromWall(ArrayList<ChirpBoid> boids, GameContainer gc){
	    Vector2f awayFromWall = new Vector2f(0,0);
	    if(pos.x > gc.getWidth() - sepDist/2){
	        awayFromWall.x = -30;
	    }   
	    else if(pos.x < sepDist/2){
	        awayFromWall.x = 30; 
	    }   
	    if(pos.y > gc.getHeight() - sepDist/2){
	        awayFromWall.y = -30;
	    }   
	    else if(pos.y < sepDist/2){
	        awayFromWall.y = 30; 
	    }   
	    return awayFromWall;
	}

	
	
	
	private float getDistanceToShape(Shape o) {
		Vector2f opos = new Vector2f(o.getCenterX(), o.getCenterY());
		return (float) Math.sqrt(Math.pow(opos.x - pos.x, 2) + Math.pow(opos.y - pos.y, 2));
	}
	
	private Vector2f steerTo(Vector2f target){
		Vector2f desired = target.sub(pos);
		float d = desired.length();
		if(d > 0){
			desired.normalise();
			desired.scale(maxF);
			
			desired.sub(vel);
			
			return desired;
		}
		else return new Vector2f(0,0);
	}

	public float getAngle() {
		return (float) Math.atan2(vel.y, vel.x);
	}
		


	
	
}