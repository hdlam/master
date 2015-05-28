package trafficsim;

import org.lwjgl.Sys;
import org.newdawn.slick.AppGameContainer;
import org.newdawn.slick.BasicGame;
import org.newdawn.slick.GameContainer;
import org.newdawn.slick.Graphics;
import org.newdawn.slick.SlickException;
import org.newdawn.slick.geom.Circle;
import org.newdawn.slick.geom.Polygon;
import org.newdawn.slick.geom.Shape;
import org.newdawn.slick.geom.Transform;
import org.newdawn.slick.geom.Vector2f;

public class SlickTest extends BasicGame{
	
	Circle c;
	Shape s;
	Transform t;
	float angle = 0;
	Vector2f v;
	
	public SlickTest(String title) {
		super(title);
	}
	@Override
	public void init(GameContainer arg0) throws SlickException {
		// TODO Auto-generated method stub
		c = new Circle(50, 50, 50);
		s = new Polygon(new float[]{
				34f,34f,
				43f,54f,
				60f,40f,
				100f,100f
				});
		t = new Transform();
		v = new Vector2f(50,50);
	}

	@Override
	public void render(GameContainer gc, Graphics g) throws SlickException {
		// TODO Auto-generated method stub
//		g.draw(c);
//		g.draw(s);
//		g.drawString("angle is:" +angle, 50, 50);
		g.drawOval(v.x, v.y, 20, 20);
	}
	
	
	
	
	@Override
	public void update(GameContainer arg0, int arg1) throws SlickException {
		// TODO Auto-generated method stub
		angle+=0.1;
		s = s.transform(t.createRotateTransform(angle, s.getCenterX(), s.getCenterY()));
		System.out.println(v.x + " .  " + v.y);
		if(angle > 40){
			v.scale(0.5f);
			System.out.println(v.x + " .  " + v.y);
			v.scale(0.5f);
			System.out.println(v.x + " .  " + v.y);
			System.exit(0);
			
		}
	}

	public static void main(String[] args){
		// TODO code application logic here
		SlickTest t = new SlickTest("test");
		System.out.println(Math.atan2(0,0));
		try {
			AppGameContainer appgc;
			appgc = new AppGameContainer(t);
			appgc.setDisplayMode(800, 800, false);
			appgc.setTargetFrameRate(60);
			appgc.setAlwaysRender(true);
			appgc.start();
			
		} catch (SlickException ex) {
			System.err.println(ex);
		}

	}


}
