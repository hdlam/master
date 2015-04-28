/*
 * To change this template, choose Tools | Templates
 * and open the template in the editor.
 */
package trafficsim;

import java.awt.Point;
import java.awt.RenderingHints.Key;
import java.io.BufferedReader;
import java.io.IOException;
import java.io.InputStreamReader;
import java.lang.reflect.Array;
import java.net.Inet4Address;
import java.net.UnknownHostException;
import java.util.ArrayList;
import java.util.Arrays;
import java.util.List;
import java.util.logging.Level;
import java.util.logging.Logger;

import no.ntnu.idi.chirp.Multicast;
import no.ntnu.idi.chirp.MulticastListener;
import no.ntnu.idi.chirp.rxtx.RxtxClass;

import org.lwjgl.LWJGLException;
import org.lwjgl.Sys;
import org.lwjgl.input.Keyboard;
import org.lwjgl.opengl.Display;
import org.lwjgl.opengl.DisplayMode;
import org.lwjgl.opengl.GL11;
import org.newdawn.slick.AppGameContainer;
import org.newdawn.slick.BasicGame;
import org.newdawn.slick.Color;
import org.newdawn.slick.GameContainer;
import org.newdawn.slick.Graphics;
import org.newdawn.slick.Input;
import org.newdawn.slick.SlickException;

/**
 * This is the main class for the project
 *
 * @author HD
 */
public class TrafficSim2 extends BasicGame implements MulticastListener{
	Terrain map;
	ArrayList<Robot> prototype;
	static boolean redlight, smartCarSim; 
	final int size = 50;
	static int w = 800;
	static int h = 652;
	boolean zKeyIsDown, xKeyIsDown, render;
	Input input;
	double angle;
	boolean averaging;
	//800 * 652
	//Threshold values
	static double distance;
	static double nearestDist;
	private int numOfBots = 1;
	private Multicast mc;
	private ArrayList<RxtxClass> rxtxrobots = new ArrayList<>();
	public boolean simulation = true;
	
	public TrafficSim2(String title, Multicast mc) {
		super(title);
		map = new Terrain();
		prototype = new ArrayList<Robot>();
		this.mc = mc;
		rxtxrobots = RxtxClass.getRobotHandles();
	}
	
	@Override
	public void init(GameContainer gc) throws SlickException {
		distance = 80;
		angle = Math.PI/3;
		nearestDist = 23;
		render = true;
		
		map = new Terrain();
		prototype = new ArrayList<Robot>();
		redlight = true; smartCarSim = false;
		averaging = false;
		
		zKeyIsDown = false; xKeyIsDown = false;;
		
		input = gc.getInput();
		if(simulation){
			for (int i = 0; i < numOfBots; i++) {
				prototype.add(new Robot(Math.random()*w, Math.random()*h, Math.random(), map, i+"", this));
			}
		}
		
	}
	
	public List<Robot> getPrototype() {
		return prototype;
	}


	@Override
	public void render(GameContainer gc, Graphics graphics) throws SlickException {
		
		for (int i = 0; i < map.getLength(); i++) {
			Point  p = map.get(i);
			Point p2 = map.get(i+1);
			if(i == 5 && redlight){
				graphics.setColor(Color.red);
			}
			else if(i == 17 && !redlight)
				graphics.setColor(Color.red);
			else
				graphics.setColor(Color.green);
			
			graphics.drawLine(p.x, p.y, p2.x, p2.y);
		}
		graphics.setColor(Color.white);
		graphics.drawLine(map.get(map.getLength()).x, map.get(map.getLength()).y, map.get(0).x, map.get(0).y);
		graphics.drawString("platooning is:" + (smartCarSim?"on":"off"), 50, 50);
		graphics.drawString("robots from udp?:" + (simulation?"on":"off"), 50, 38);
		for (int i = 0; i < prototype.size(); i++) {
			prototype.get(i).draw(graphics);
		}
		//graphics.drawString(prototype.size()+"", 300f, 300f);
		
	}
	
	
	
	/**
	 * KEYBOARD SHORTCUTS
	 * x - redlight toggle
	 * q - quit
	 * z - platooning toggle
	 * r - render hitboxes
	 * a - pair COM port
	 * c - each robot finds its closest point
	 * u - refreshes the robots on the comm port (not sure)
	 **/
	@Override
	public void update(GameContainer gc, int delta) throws SlickException {
		for (int j = 0; j < prototype.size(); j++) {
			prototype.get(j).move(delta);
		}
		
		if(input.isKeyPressed(Input.KEY_X)){
			redlight = !redlight;
		}
		if(input.isKeyPressed(Input.KEY_Q) || input.isKeyPressed(Input.KEY_ESCAPE))
			closeRequested();
		if(input.isKeyPressed(Input.KEY_Z))
			smartCarSim =!smartCarSim;
		
		if(input.isKeyPressed(Input.KEY_R)){
			render = !render;
		}
		
		if(input.isKeyPressed(Input.KEY_A)){
			stopAllRobots();
			for (RxtxClass ser : rxtxrobots) {
				checkRxTx(ser).setRxtx(ser);
			}
		}
		if(input.isKeyPressed(Input.KEY_C))
		{
			for (Robot robot: prototype) {
				robot.findClosestPoint();
			}
		}
		if(input.isKeyPressed(Input.KEY_U)) //does not really works
		{
			for (RxtxClass r : rxtxrobots)
			{
				r.sendRobotData(0, 0);
				try 
				{
					Thread.sleep(50);
				} 
				catch (InterruptedException e)
				{
					e.printStackTrace();
				}
				r.Stop();
			}
			try {
				Thread.sleep(1000);
			} catch (InterruptedException e) {
				e.printStackTrace();
			}
			rxtxrobots.clear();
			rxtxrobots = RxtxClass.getRobotHandles();
			System.out.println("Found "+rxtxrobots.size()+" robot(s)");
		}
		if(input.isKeyPressed(Input.KEY_P)){
			prototype = new ArrayList<Robot>();
			if(simulation){
				for (int i = 0; i < numOfBots; i++) {
					prototype.add(new Robot(Math.random()*w, Math.random()*h, Math.random(), map, i+"", this));
				}
			}
			
		}
		if(input.isKeyPressed(Input.KEY_S)){
			simulation = !simulation;
		}
		
		
		
	}
	
	public long getTime() {
		return (Sys.getTime() * 1000) / Sys.getTimerResolution();
	}

	public static void main(String[] args) throws UnknownHostException {
		// TODO code application logic here

		BufferedReader inFromUser = new BufferedReader(new InputStreamReader(System.in)); //the format used to receive data
		Multicast mc = new Multicast(Inet4Address.getLocalHost().getHostAddress()); //gets the data from the camera software
		TrafficSim2 sim = new TrafficSim2("simulator", mc);
		

		mc.addListener(sim);
		
		try {
			mc.openRecieve();
		} catch (IOException e1) {
			e1.printStackTrace();
		}
		
		try {
			AppGameContainer appgc;
			appgc = new AppGameContainer(sim);
			appgc.setDisplayMode(w, h, false);
			appgc.setTargetFrameRate(60);
			appgc.setAlwaysRender(true);
			appgc.start();
			
		} catch (SlickException ex) {
			Logger.getLogger(TrafficSim2.class.getName()).log(Level.SEVERE, null, ex);
		}

		try {
			mc.send(inFromUser.readLine());
		} catch (IOException e) {
			e.printStackTrace();
		}
		mc.closeRecieve();
	}
	
	public boolean closeRequested() {
		mc.closeRecieve();
		for (RxtxClass r : rxtxrobots)
		{
			r.sendRobotData(0, 0);
			try 
			{
				Thread.sleep(50);
			} 
			catch (InterruptedException e)
			{
				e.printStackTrace();
			}
			r.Stop();
		}
		System.exit(0);
		//TODO
		return true;
		
	}
	

	public Robot nearest(double x, double y) {
         double dist = Double.MAX_VALUE;
         Robot nearest = null;
         for (Robot robot : prototype) {
             if(Math.hypot(x-robot.getX(), y-robot.getY())<dist){
                 dist = Math.hypot(x-robot.getX(), y-robot.getY());
                 nearest = robot;
             }
         }
         return nearest;
	 }
	
	@Override
	public void multiCastRecieved(String str) {
		//System.out.println(str);
		for (String part : str.split("\\|")) {
        	if (!part.matches("(X\\d+:\\d\\.\\d+);(Y\\d+:\\d\\.\\d+);(R\\d+:\\d(\\.\\d+)?)"))
        		continue;
            String[] split = part.split(";");
            String id = split[0].split(":")[0].substring(1);
            double x = Double.parseDouble(split[0].split(":")[1])*w;
            double y = Double.parseDouble(split[1].split(":")[1])*h;
            double r = Double.parseDouble(split[2].split(":")[1]);
            
            updateRobotPosition(id, x, y, r);
        }
	}

	private void updateRobotPosition(String id, double x, double y, double r) {
		boolean found = false;
		
		for(Robot robot : prototype) {
			if (robot.getID().equals(id)) {
				robot.setAngle(r);
				robot.setPosition(x, y);
				found = true;
			}
		}
		if (!found) {
			if (prototype.size() == numOfBots) {
				Robot nearest = nearest(x,y);
				nearest.setID(id);
				nearest.setAngle(r);
				nearest.setPosition(x, y);
				//TODO: check rxtx?
			} else {
				Robot robot = new Robot(x, y, r, map, id, this);
				//TODO: fix denne her..
				prototype.add(robot);
			}
		}
	}
	
	private void stopAllRobots() {
		for (RxtxClass serial : rxtxrobots) 
		{
			serial.sendRobotData(0, 0);
		}
		try 
		{
			Thread.sleep(50);
		} 
		catch (InterruptedException e)
		{
			e.printStackTrace();
		}
	}
	
	private Robot checkRxTx(RxtxClass rxtx) //assigns the comm port to the robot seen on screen
	{
		String lastmsg = mc.getLastMsg();
		//System.out.println("end"+lastmsg);
		multiCastRecieved(lastmsg);
		Robot[] robots = new Robot[prototype.size()];
		double[] angles = new double[prototype.size()];
		for (int i = 0; i < angles.length; i++) 
		{
			Robot rob = prototype.get(i);
			robots[i] = rob;
			angles[i] = rob.getAngle();
			//System.out.println("---- "+rob.getID()+" "+rob.getAngle());
		}
		//Start rotate:
		rxtx.sendRobotData(-255, 255);
		try {
			Thread.sleep(500);
		
			rxtx.sendRobotData(0, 0);
			Thread.sleep(500);
		} catch (InterruptedException e) {
			e.printStackTrace();
		}
		lastmsg = mc.getLastMsg();
		//System.out.println("end"+lastmsg);
		multiCastRecieved(lastmsg);
		
		// who rotated?
		Robot maxRotRob = robots[0];
		double maxRotAngle = 0;
		for (int i = 0; i < angles.length; i++) 
		{
			Robot rob = robots[i];
			//System.out.println("---- "+rob.getID()+" "+rob.getAngle());
			double deltaAngle = Math.abs(((rob.getAngle()-angles[i]) + Math.PI) % (Math.PI*2) - Math.PI);
			//System.out.println("robot "+rob.getID()+" rotated: "+deltaAngle+"     "+lastmsg);
			if(maxRotAngle<deltaAngle)
			{ 
				maxRotRob = rob;
				maxRotAngle = deltaAngle;//rob.getAngle()-angles[i];
			}
		}
		
		try {
			Thread.sleep(500);
			
			rxtx.sendRobotData(255, -255);
		
			Thread.sleep(500);
			rxtx.sendRobotData(0, 0);
			Thread.sleep(500);
		} catch (InterruptedException e) {
			e.printStackTrace();
		}
		
		//System.out.println("robot "+maxRotRob.getID()+" got assigned: "+rxtx.portName);
		return maxRotRob;
	}

}
