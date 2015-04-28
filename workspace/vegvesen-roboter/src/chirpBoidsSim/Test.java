package chirpBoidsSim;

import org.newdawn.slick.geom.Vector2f;

public class Test {
	
	public static void main(String[] args) {
		Vector2f nonRot = new Vector2f(2,0);
		float a = (float) (Math.PI/2);
		Vector2f rot = new  Vector2f((float)(nonRot.x*Math.cos(a) - nonRot.y*Math.sin(a)), (float)(nonRot.x*Math.sin(a)+nonRot.y*Math.cos(a)));
		
		System.out.println(nonRot);
		System.out.println(rot);
	}

}
































