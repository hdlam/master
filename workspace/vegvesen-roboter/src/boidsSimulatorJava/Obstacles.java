package boidsSimulatorJava;

import org.newdawn.slick.Color;
import org.newdawn.slick.GameContainer;
import org.newdawn.slick.Graphics;
import org.newdawn.slick.geom.Circle;
import org.newdawn.slick.geom.Shape;
import org.newdawn.slick.geom.Transform;
import org.newdawn.slick.geom.Vector2f;

public class Obstacles extends Shape{
	Vector2f vel;
	Color col;
	
	
	public Obstacles(float x, float y, Color c, float vX, float vY) {
		col = c;
		vel = new Vector2f(vX, vY);
		this.x = x;
		this.y = y;
	}
	
	public void update(GameContainer gc, int delta){
		this.x += vel.x/delta;
		this.y += vel.y/delta;
	}
	
	public void render(GameContainer gc, Graphics g) {
		g.draw(this);
	}

	@Override
	protected void createPoints() {
		// TODO Auto-generated method stub
		
	}

	@Override
	public Shape transform(Transform arg0) {
		// TODO Auto-generated method stub
		return null;
	}
	
	

}
