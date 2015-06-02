package boidsSimulatorJava;

import org.newdawn.slick.*;
import org.newdawn.slick.geom.Circle;
import org.newdawn.slick.geom.Shape;
import org.lwjgl.input.Keyboard;

import java.io.FileWriter;
import java.io.IOException;
import java.util.ArrayList;
import java.util.Random;

/**
 * Created with IntelliJ IDEA.
 * Created By: Lyndon Armitage, edited by HD
 * Date: 22/02/13
 */
public class BoidsGame extends BasicGame {

//	private static final int WIDTH = 800;
//	private static final int HEIGHT = 652;
	private static final int WIDTH = 1024;
	private static final int HEIGHT = 1024;

	private boolean debugOn = true;
	private boolean drawArc = true;
	
	private boolean paused = true;
	private ArrayList<ChirpBoid> boids = null;
	private static int numOfBots = 4;
	
	private ArrayList<Shape> obstacles;
	int scenario = 3;
	int run = 6;
	boolean write = false;
	Input input;

	public static void main(String args[]) {
		BoidsGame game = new BoidsGame("Boids simulator");
		AppGameContainer appgc;
		try {
			appgc = new AppGameContainer(game, WIDTH, HEIGHT, false);
			appgc.setShowFPS(true);
			appgc.setTargetFrameRate(60);
			appgc.setPaused(false);
			appgc.setVSync(true);
			appgc.setAlwaysRender(true);
			appgc.start();
		} catch (SlickException e) {
			e.printStackTrace();
		}
	}

	public BoidsGame(String title) {
		super(title);
	}

	
	
	
	@Override
	public void init(GameContainer gc) throws SlickException {
		boids = new ArrayList<ChirpBoid>();
		// add the boids
		input = gc.getInput();
		obstacles = new ArrayList<Shape>();
		createBoids();
		gc.setPaused(false);
		if(write){
			try {
				fw = new FileWriter("simulScenario"+ scenario +"_"+run+".csv");
			} catch (IOException e) {
				e.printStackTrace();
			}
		}
		time = System.currentTimeMillis();
	}
	
	int factorial(int num){
		if(num == 0)
			return 1;
		else
			return factorial(num-1)*num;
	}
	
	void refactorAngles(float[] angles){
		for (int i = 0; i < angles.length; i++) {
			if (angles[i] > Math.PI) {
				angles[i] -= 2 * Math.PI;
			} else if (angles[i] < -Math.PI) {
				angles[i] += 2 * Math.PI;
			}
			
			angles[i] = Math.abs(angles[i]);
		}
	}

	int iteration = 0;
	FileWriter fw;
	private long time;
	void createStats(){
		float distSum = 0;
		float anglSum = 0;
		float speedSum = 0;
		int count = 0;
		
		int numToCompare = 2;
		int numOfCompares = factorial(boids.size())/((boids.size()-numToCompare)*factorial(numToCompare)); //4n2
		
		
		float[] dists = new float[numOfCompares];
		float[] angles = new float[numOfCompares];
		
		for (int i = 0; i < boids.size(); i++) {
			for (int j = i+1; j < boids.size(); j++) {
				if(boids.get(i) == boids.get(j)){
					continue;
				}
				else{
					dists[count] = boids.get(i).getPos().distance(boids.get(j).getPos());
					angles[count] =  Math.abs(boids.get(i).getAngle() - boids.get(j).getAngle());
					distSum += dists[count];
					count++;
				}
			}
		}
		for (ChirpBoid cb : boids) {
			speedSum += cb.getVel().length();
		}
		float speedAvg = speedSum/numOfBots;
		
		refactorAngles(angles);
		//calc avg. dist
		
		for (int i = 0; i < angles.length; i++) {
			anglSum += angles[i];
		}
		float distAvg = distSum/numOfCompares;
		float anglAvg = anglSum/numOfCompares;
		float distsdSum = 0;
		float anglsdSum = 0;
		float speedsdSum = 0;
		for (int i = 0; i < dists.length; i++) {
			distsdSum += Math.pow((dists[i]-distAvg),2);
			anglsdSum += Math.pow((angles[i]-anglAvg),2);
		}
		for (int i = 0; i < numOfBots; i++) {
			speedsdSum += Math.pow((boids.get(i).getVel().length()-speedAvg),2);
		}
		distsdSum = (float) Math.sqrt(distsdSum / (count));
		anglsdSum = (float) Math.sqrt(anglsdSum / (count));
		speedsdSum = (float) Math.sqrt(speedsdSum / (count));
		//sdSum = (float) Math.sqrt(sdSum / (count-1));
//		System.out.println(System.currentTimeMillis());
		if(write){
			try {
				fw.write(iteration + "," + distAvg +","+ distsdSum + "," + anglAvg + "," + anglsdSum + "," + speedAvg + "," +speedsdSum+ "\n");
			} catch (IOException e) {
				// TODO Auto-generated catch block
				e.printStackTrace();
			}
		}
		iteration++;
		
	}
	
	int countIteration = 4;
	@Override
	public void update(GameContainer gc, int delta) throws SlickException {
		if(System.currentTimeMillis()-time > 180000){
			System.out.println("timeout");
			if(write){
				try {
					fw.close();
				} catch (IOException e) {
					// TODO Auto-generated catch block
					e.printStackTrace();
				}
			}
//			paused=true;
			gc.exit();
			System.exit(0);
		}
 
		if(input.isKeyPressed(Input.KEY_Q) || input.isKeyPressed(Input.KEY_ESCAPE)){
			if(write){
				try {
					fw.close();
				} catch (IOException e) {
					// TODO Auto-generated catch block
					e.printStackTrace();
				}
			}
//			paused=true;
			System.exit(0);
		}
		if (input.isKeyPressed(Input.KEY_P)) {
			createBoids();
		}
		if (input.isKeyPressed(Input.KEY_T)) {
			System.out.println(iteration);
		}
//		if (input.isKeyPressed(Input.KEY_1))
//			numOfBots = 1;
//		if (input.isKeyPressed(Input.KEY_2))
//			numOfBots = 2;
//		if (input.isKeyPressed(Input.KEY_3))
//			numOfBots = 3;
//		if (input.isKeyPressed(Input.KEY_4))
//			numOfBots = 4;
//		if (input.isKeyPressed(Input.KEY_5))
//			numOfBots = 5;
//		if (input.isKeyPressed(Input.KEY_6))
//			numOfBots = 6;
//		if (input.isKeyPressed(Input.KEY_7))
//			numOfBots = 7;
//		if (input.isKeyPressed(Input.KEY_8))
//			numOfBots = 8;

		if (input.isMousePressed(input.MOUSE_LEFT_BUTTON)) {
//			ChirpBoid boid = new ChirpBoid((int) (Math.random() * WIDTH), (int) (Math.random() * HEIGHT), intToCol((int) (2)),(int) (Math.random() * 120) - 60,(int) (Math.random() * 120) - 60, true);
			ChirpBoid boid = new ChirpBoid((int) (input.getMouseX()), (int) (input.getMouseY()), intToCol((int) (2)),(int) (Math.random() * 120) - 60,(int) (Math.random() * 120) - 60, true);
			boids.add(boid);
		}

		if (input.isKeyPressed(Input.KEY_SPACE))
			paused = !paused;
		if (paused){
			time = System.currentTimeMillis();
			return;
		}
		for (ChirpBoid b : boids) {
		b.update(gc, delta, boids, obstacles);
		}
		countIteration++;
		if(countIteration >= 5){
			createStats();
			countIteration = 0;
		}
	
	}
	
	private void createBoids(){
		boids.clear();
//		for (int i = 0; i < numOfBots; i++) {
////			ChirpBoid boid = new ChirpBoid((int)(Math.random()*WIDTH), (int)(Math.random()*HEIGHT), intToCol(i), rnd.nextInt(200) - 100, rnd.nextInt(200) - 100);
//			ChirpBoid boid = new ChirpBoid((int)(Math.random()*WIDTH), (int)(Math.random()*HEIGHT), intToCol((int)(Math.random()*10)), (int)(Math.random()*120)-60, (int)(Math.random()*120)-60, true);
////			ChirpBoid boid = new ChirpBoid((WIDTH)/2, (HEIGHT)/2, intToCol(i), (int)(Math.random()*120)-60, (int)(Math.random()*120)-60);
//			boids.add(boid);
//		}
//		}
		
		switch (scenario) {
		case 1:
			scenario1();
			break;
		case 2:
			scenario2();
			break;
		case 3:
			randomScenario();
			break;
		case 4:
			break;
		default:
			for (int i = 0; i < numOfBots; i++) {
				ChirpBoid boid = new ChirpBoid((int)(Math.random()*WIDTH), (int)(Math.random()*HEIGHT), intToCol((int)(Math.random()*10)), (int)(Math.random()*120)-60, (int)(Math.random()*120)-60, true);
				boids.add(boid);
			}
			break;
		}
//		scenario2();
//		randomScenario();
//		scenario1();
	}
	
	void randomScenario(){
//		for (int i = 0; i < numOfBots; i++) {
////	ChirpBoid boid = new ChirpBoid((int)(Math.random()*WIDTH), (int)(Math.random()*HEIGHT), intToCol(i), rnd.nextInt(200) - 100, rnd.nextInt(200) - 100);
//			ChirpBoid boid = new ChirpBoid((int)(Math.random()*WIDTH), (int)(Math.random()*HEIGHT), intToCol((int)(0)), (int)(Math.random()*120)-60, (int)(Math.random()*120)-60, true);
////	ChirpBoid boid = new ChirpBoid((WIDTH)/2, (HEIGHT)/2, intToCol(i), (int)(Math.random()*120)-60, (int)(Math.random()*120)-60);
//			boids.add(boid);
//	
//}
		boids.add(new ChirpBoid(340f, 18f, intToCol(0), 15, 20,true));
		boids.add(new ChirpBoid(666f, 50f, intToCol(0), -15, -20,true));
		boids.add(new ChirpBoid(157f, 553f, intToCol(0), -15, 15,true));
		boids.add(new ChirpBoid(740f, 565f, intToCol(0), -15, -20,true));
		Shape s = new Circle(667f, 479f, (float) 30); 		//random
		
		obstacles.add(s);
	}
	
	void scenario1(){
		//scenario1
		boids.add(new ChirpBoid(30f, 30f, intToCol(0), 20, 20,true));
		boids.add(new ChirpBoid(30f, HEIGHT-30, intToCol(0), 20, -20,true));
		boids.add(new ChirpBoid(WIDTH-30, 30f, intToCol(0), -20, 20,true));
		boids.add(new ChirpBoid(WIDTH-30, HEIGHT-30, intToCol(0), -20, -20,true));
		Shape s = new Circle(WIDTH/2f+40f, HEIGHT/2f+40f, (float) 30); 		//scenario1		
		obstacles.add(s);
	
	}
	void scenario2(){
		//scenario1
		boids.add(new ChirpBoid(30f, 30f, intToCol(0), 10, 10,true));
		boids.add(new ChirpBoid(30f, 60f, intToCol(0), 10, 10,true));
		boids.add(new ChirpBoid(60f, 30f, intToCol(0), 10, 10,true));
		boids.add(new ChirpBoid(WIDTH-30, HEIGHT-30, intToCol(0), 0, 0,false));
		Shape s = new Circle(WIDTH/2, HEIGHT/2f, (float) 30); 		//scenario2		
		obstacles.add(s);
		
	}

	@Override
	public void render(GameContainer gc, Graphics g) throws SlickException {

		for (ChirpBoid b : boids) {
			b.render(gc, g);
		}
		g.setColor(Color.red);
		for(Shape s : obstacles){
			g.draw(s);
		}

	}

//	private void createObstacles(){
//		
//		//Shape s = new Circle(WIDTH/2, HEIGHT/2, (float) (Math.random()*40)); 		//scnario
//		Shape s = new Circle(667f, 479f, (float) 30); 		//random
//		
//		obstacles.add(s);
//	}
	
	
	private Color intToCol(int i){
		switch (i) {
		case 0:
			return Color.lightGray;
		case 1:
			return Color.magenta;
	
		case 2:
			return Color.cyan;
			
		case 3:
			return Color.blue;
			
		case 4:
			return Color.yellow;
			
		case 5:
			return Color.orange;
			
		case 6:
			return Color.pink;
			
		case 7:
			return Color.gray;
			
		default:
			return Color.white;
		}
	}
}