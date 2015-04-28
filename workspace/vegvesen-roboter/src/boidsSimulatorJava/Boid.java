package boidsSimulatorJava;
//package com.lyndonarmitage.boids;
//
//import org.newdawn.slick.Color;
//import org.newdawn.slick.GameContainer;
//import org.newdawn.slick.Graphics;
//import org.newdawn.slick.geom.Vector2f;
//
//import java.util.ArrayList;
//
///**
// * Created with IntelliJ IDEA.
// * Created By: Lyndon Armitage
// * Date: 22/02/13
// * <p/>
// * http://www.red3d.com/cwr/boids/
// */
//public class Boid {
//
//	// http://stackoverflow.com/questions/7586063/how-to-calculate-the-angle-between-two-points-relative-to-the-horizontal-axis
//	private float angle = 0f; // remember to use atan
//	private Vector2f pos;
//	private Vector2f vel;
//	private Color color;
//
//	private static final float width = 8;
//	private static final float height = 10;
//	private static final int lineWidth = 1;
//
//	private static final float viewDistance = 50f;
//
//	public Boid() {
//		init(0f, 0f, Color.white, 0f, 0f);
//	}
//
//	public Boid(float x, float y) {
//		init(x, y, Color.white, 0f, 0f);
//	}
//
//	public Boid(float x, float y, Color color) {
//		init(x, y, color, 0f, 0f);
//	}
//
//	public Boid(float x, float y, Color color, float velX, float velY) {
//		init(x, y, color, velX, velY);
//	}
//
//	public Boid(float x, float y, float velX, float velY) {
//		init(x, y, Color.white, velX, velY);
//	}
//
//	private void init(float x, float y, Color color, float velX, float velY) {
//		pos = new Vector2f(x, y);
//		vel = new Vector2f(velX, velY);
//		this.color = color;
//	}
//
//	public Color getColor() {
//		return color;
//	}
//
//	public Vector2f getPos() {
//		return pos;
//	}
//
//	public Vector2f getVel() {
//		return vel;
//	}
//
//	public void render(GameContainer gc, Graphics g) {
//		//g.drawString("boid", pos.x, pos.y);
//		g.rotate(pos.x, pos.y, angle);
//		g.setLineWidth(lineWidth);
//		g.setColor(color);
////		g.drawLine(pos.x - (width / 2), pos.y - (height / 2), pos.x + (width / 2), pos.y - (height / 2)); // bottom line
//		g.setColor(Color.pink);
//		g.drawLine(pos.x - (width / 2), pos.y - (height / 2), pos.x + (width / 2), pos.y - (height / 2)); // bottom line
//		g.setColor(color);
//		g.drawLine(pos.x + (width / 2), pos.y - (height / 2), pos.x, pos.y + (height / 2)); // right to top
//		g.drawLine(pos.x, pos.y + (height / 2), pos.x - (width / 2), pos.y - (height / 2)); // top to left
//		g.resetTransform();
//	}
//
//	public void update(GameContainer gc, int delta, ArrayList<Boid> boids) {
//
//		// some look at the closest boid in view code
//		Boid target = null;
//		float dist = 0;
//		for (Boid b : boids) {
//			if (b.getPos().x == pos.x && b.getPos().y == pos.y || !isBoidInView(b)) continue;
//			if (target == null || getDistanceToBoid(b) < dist) {
//				dist = getDistanceToBoid(b);
//				target = b;
//			}
//		}
//		if (target != null) {
//			angle = getAngleToBoid(target);
//		}
//
//		updatePos(gc, delta);
//	}
//
//	private void updatePos(GameContainer gc, int delta) {
//		pos.x += vel.x / delta;
//		pos.y += vel.y / delta;
//		if (pos.x > gc.getWidth()) {
//			pos.x = gc.getWidth() - pos.x;
//		}
//		if (pos.y > gc.getHeight()) {
//			pos.y = gc.getHeight() - pos.y;
//		}
//		if (pos.x < 0) {
//			pos.x = gc.getWidth();
//		}
//		if (pos.y < 0) {
//			pos.y = gc.getHeight();
//		}
//	}
//
//	public void renderArc(Graphics g) {
//		g.setColor(Color.green);
//		g.drawOval(pos.x - (viewDistance / 2), pos.y - (viewDistance / 2), viewDistance, viewDistance);
//	}
//
//	private float getAngleToBoid(Boid target) {
//		float deltaX = target.getPos().x - pos.x;
//		float deltaY = target.getPos().y - pos.y;
//		float angle = (float) (Math.atan2(deltaY, deltaX) * 180 / Math.PI);
//		angle -= 90; // seems to be off by 90 degrees probably due to how the graphics are set up
//		if (angle > 360f) {
//			angle = 360f - angle;
//		} else if (angle < 0f) {
//			angle = 360f + angle;
//		}
//		return angle;
//	}
//
//	private float getDistanceToBoid(Boid target) {
//		Vector2f v = target.getPos();
//		return (float) Math.sqrt(Math.pow(v.x - pos.x, 2) + Math.pow(v.y - pos.y, 2));
//	}
//
//	private boolean isBoidInView(Boid target) {
//		float dx = Math.abs(target.getPos().x - pos.x);
//		float dy = Math.abs(target.getPos().y - pos.y);
//		float radius = viewDistance / 2;
//		if (dx > radius) {
//			return false;
//		}
//		if (dy > radius) {
//			return false;
//		}
//		if (dx + dy <= radius) {
//			return true;
//		}
//		if (Math.pow(dx, 2) + Math.pow(dy, 2) <= Math.pow(radius, 2)) {
//			return true;
//		} else {
//			return false;
//		}
//	}
//}