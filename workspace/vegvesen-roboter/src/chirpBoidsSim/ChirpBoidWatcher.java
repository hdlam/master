/*
 * To change this template, choose Tools | Templates
 * and open the template in the editor.
 */
package chirpBoidsSim;

import java.io.BufferedReader;
import java.io.FileWriter;
import java.io.IOException;
import java.io.InputStreamReader;
import java.net.Inet4Address;
import java.net.UnknownHostException;
import java.util.ArrayList;
import java.util.List;
import java.util.logging.Level;
import java.util.logging.Logger;

import no.ntnu.idi.chirp.Multicast;
import no.ntnu.idi.chirp.MulticastListener;
import no.ntnu.idi.chirp.rxtx.RxtxClass;

import org.lwjgl.Sys;
import org.newdawn.slick.AppGameContainer;
import org.newdawn.slick.BasicGame;
import org.newdawn.slick.Color;
import org.newdawn.slick.GameContainer;
import org.newdawn.slick.Graphics;
import org.newdawn.slick.Input;
import org.newdawn.slick.SlickException;
import org.newdawn.slick.geom.Vector2f;
import org.newdawn.slick.util.FastTrig;

/**
 * This is the main class for the Boids, it will render the Boids as seen on screen.
 *
 * @author HD
 */
public class ChirpBoidWatcher extends BasicGame implements MulticastListener{
	ArrayList<ChirpBot> prototype;
	final int size = 50;
	static int w = 800;//800
	static int h = 652; //652
	Input input;
	//800 * 652
	//Threshold values
	private int numOfBots = 1;
	private Multicast mc;
	
	//ranges of certain behaviours:
	private int sep = 150*2;
	private int ali = 200*2;
	private int coh = 800*2;
	
	
	private ArrayList<RxtxClass> rxtxrobots = new ArrayList<>();
	public boolean averaging;
	public String udpString;
	FileWriter fw;
	String filename = "gibberish.csv";
	boolean start = false;
	int iteration;
	long time;
	
	public boolean debug = true;
	public boolean debugRender = true;
	
	public ChirpBoidWatcher(String title, Multicast mc) {
		super(title);
		prototype = new ArrayList<ChirpBot>();
		this.mc = mc;
		rxtxrobots = RxtxClass.getRobotHandles();
		
		//Settings;
		averaging = true;
		udpString = "";
//		debug = true;
		
	}
	
	@Override
	public void init(GameContainer gc) throws SlickException {
		prototype = new ArrayList<ChirpBot>();
		input = gc.getInput();
		if(debug){
			for (int i = 0; i < numOfBots; i++) {
				ChirpBot temp =  new ChirpBot((float) Math.random()*w, (float) Math.random()*h, (float) Math.PI, i+"", this);
				prototype.add(temp);
			}
		}
		 try {
			fw = new FileWriter(filename);
		} catch (IOException e) {
			e.printStackTrace();
		}
		iteration = 0;
		time = System.currentTimeMillis();
	}
	
	public List<ChirpBot> getPrototype() {
		return prototype;
	}
	
	
	public void renderrxtxLED(GameContainer gc, Graphics graphics){
		for (int i = 0; i < 4; i++) {
			boolean found =  false;
			for (int j = 0; j < rxtxrobots.size(); j++) {
				if(rxtxrobots.get(j).portName.contains(i+"")){
					found = true;
					break;
				}
			}
			if(!found){
				graphics.setColor(Color.red);
			}
			else{
				graphics.setColor(Color.green);
			}
			graphics.fillRect(w-(40-i*10), 5, 10, 10);
			graphics.setColor(Color.black);
			graphics.drawRect(w-(40-i*10), 5, 10, 10);
				
		}
	}
	public void renderDebugInfo(GameContainer gc, Graphics graphics){
		Vector2f snitt = new Vector2f(0,0);
		int num = 0;
		for (ChirpBot cb : prototype) {
//			graphics.setColor(Color.blue);
//			graphics.drawOval(cb.getX()-sep/2, cb.getY()-sep/2, sep, sep);
//			graphics.setColor(Color.gray);
//			graphics.drawOval(cb.getX()-ali/2f, cb.getY()-ali/2f, ali, ali);
//			graphics.setColor(Color.red);
//			graphics.drawOval(cb.getX()-coh/2, cb.getY()-coh/2, coh, coh);
			snitt.add(new Vector2f(cb.getX(), cb.getY()));
			num++;
		}
		snitt.scale(1f/num);
		graphics.setColor(Color.green);
		graphics.drawRect(snitt.x-10, snitt.y-10, 20, 20);
		graphics.drawLine(snitt.x-10, snitt.y-10,snitt.x+10, snitt.y+10);
		graphics.drawLine(snitt.x-10, snitt.y+10,snitt.x+10, snitt.y-10);
		graphics.drawString(snitt.x + "," + snitt.y, snitt.x, snitt.y);
		String[] udpStringArr = udpString.split("\\|");
//		if(udpStringArr.length >2){
//			graphics.drawString(udpStringArr[0]+"         "+udpStringArr[1], 0, 0);
//			graphics.drawString(udpStringArr[2]+"         "+udpStringArr[3], 0, 12);
//		}
		for (int i = 0; i < udpStringArr.length; i++) {
			graphics.drawString(udpStringArr[i], 0, 12*i);
		}
	}

	@Override
	public void render(GameContainer gc, Graphics graphics) throws SlickException {
		for (int i = 0; i < prototype.size(); i++) {
			graphics.setColor(Color.white);
			prototype.get(i).draw(graphics);
		}
		 //rxtx info "LED"
		
		if(debugRender){
			renderrxtxLED(gc, graphics);
			//debug
			renderDebugInfo(gc, graphics);
		}
		//
//		graphics.drawString(prototype.size()+"", 300f, 300f);
		graphics.drawString(iteration+"", w-50,h-20);
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
	
	void createStats(){
		float distSum = 0;
		float anglSum = 0;
		float speedSum = 0;
		int count = 0;
		
		int numToCompare = 2;
		int numOfCompares = factorial(prototype.size())/((prototype.size()-numToCompare)*factorial(numToCompare)); //4n2
		
		
		float[] dists = new float[numOfCompares];
		float[] angles = new float[numOfCompares];
		
		for (int i = 0; i < prototype.size(); i++) {
			for (int j = i+1; j < prototype.size(); j++) {
				if(prototype.get(i) == prototype.get(j)){
					continue;
				}
				else{
					dists[count] = prototype.get(i).getPos().distance(prototype.get(j).getPos());
					angles[count] =  Math.abs(prototype.get(i).getAngle() - prototype.get(j).getAngle());
					distSum += dists[count];
					count++;
				}
			}
		}
		for (int i = 0; i < numOfBots; i++) {
			speedSum += prototype.get(i).getVelocity();
		}
		
		refactorAngles(angles);
		//calc avg. dist
		
		for (int i = 0; i < angles.length; i++) {
			anglSum += angles[i];
		}
		float distAvg = distSum/numOfCompares;
		float anglAvg = anglSum/numOfCompares;
		float speedAvg = speedSum/numOfBots;
		float distsdSum = 0;
		float anglsdSum = 0;
		float speedsdSum = 0;
		
		for (int i = 0; i < dists.length; i++) {
			distsdSum += Math.pow((dists[i]-distAvg),2);
			anglsdSum += Math.pow((angles[i]-anglAvg),2);
		}
		
		for (int i = 0; i < numOfBots; i++) {
			speedsdSum += Math.pow((prototype.get(i).getVelocity()-speedAvg),2);
		}
		distsdSum = (float) Math.sqrt(distsdSum / (count));
		anglsdSum = (float) Math.sqrt(anglsdSum / (count));
		speedsdSum = (float) Math.sqrt(speedsdSum / (count));
		//sdSum = (float) Math.sqrt(sdSum / (count-1));
		
		System.out.println(System.currentTimeMillis());
		try {
//			fw.write(iteration + "," + distAvg +","+ distsdSum + "," + anglAvg + "," + anglsdSum + "," + speedAvg + "," +speedsdSum+ "\n");
			fw.write((System.currentTimeMillis()-time) + "," + speedAvg + "\n");
		} catch (IOException e) {
			// TODO Auto-generated catch block
			e.printStackTrace();
		}
		iteration++;
		
	}
	
	/**
	 * KEYBOARD SHORTCUTS
	 * w - pair COM port inplace,  
	 * a - pair COM porto
	 * e - getData from all rxtx bots
	 * s - stops all the robots if they are still moving in the start, does not work when they are running
	 * q - quit
	 * p - refreshes the list of robots, if any artifacts happens, use this. 
	 * r - toggle render of information
	 **/
	int numOfRobotAssigned = 0;
	int count = 4;
	@Override
	public void update(GameContainer gc, int delta) throws SlickException {
		if(System.currentTimeMillis()-time > 180000){
			System.out.println("timeout");
			closeRequested();
		}
		
		
		for (int j = 0; j < prototype.size(); j++) {
			prototype.get(j).update(delta);
		}
		for (int i = 0; i < rxtxrobots.size(); i++) {
			if(!rxtxrobots.get(i).isRunning()){
				System.out.println(rxtxrobots.get(i).portName +  " removed");
				rxtxrobots.remove(i);
			}
		}
		if(start){
			count++;
			if(count == 5){
				createStats();
				count = 0;
			}
		}
		else{
			start = true;
			time = System.currentTimeMillis();
		}
		//mouse assignment;
		if (input.isMousePressed(Input.MOUSE_LEFT_BUTTON)) {
            if(numOfRobotAssigned < numOfBots){
            	nearest(input.getMouseX(),input.getMouseY()).setRxtx(rxtxrobots.get(numOfRobotAssigned));
            	rxtxrobots.get(numOfRobotAssigned).sendSingleByte(0);
            	numOfRobotAssigned++;
            }
            
        }
		if(input.isKeyPressed(Input.KEY_Q) || input.isKeyPressed(Input.KEY_ESCAPE))
			closeRequested();
		
		if(input.isKeyPressed(Input.KEY_E)){
			start = true;
			for (int i = 0; i < prototype.size(); i++) {
				if(prototype.get(i).getRxtx() != null){
					System.out.println("got from " + i);
					prototype.get(i).getRxtx().getData();
					prototype.get(i).canSend();
				}
			}
//			ChirpBot temp;
//			for (int i = 0; i < prototype.size(); i++) {
//				if(prototype.get(i).getRxtx() == null){
//					temp = prototype.get(i);
//					prototype.remove(i);
//					prototype.add(temp);
//					break;
//				}
//			}
		}
		if(input.isKeyPressed(Input.KEY_W)){
			start = true;
			nearest(0, 0).setRxtx(rxtxrobots.get(0));
			nearest(w-700, 0).setRxtx(rxtxrobots.get(1));
			nearest(0, h).setRxtx(rxtxrobots.get(2));
			rxtxrobots.get(0).getData();
			rxtxrobots.get(1).getData();
			rxtxrobots.get(2).getData();
			for (int i = 0; i < prototype.size()-1; i++) {
				prototype.get(i).test(i);;
			}
		}
		if(input.isKeyPressed(Input.KEY_T)){
			nearest(0, 0).setRxtx(rxtxrobots.get(0));
			nearest(w, 0).setRxtx(rxtxrobots.get(1));
			nearest(0, h).setRxtx(rxtxrobots.get(2));
//			nearest(w, h).setRxtx(rxtxrobots.get(3));
			rxtxrobots.get(0).getData();
			rxtxrobots.get(1).getData();
			rxtxrobots.get(2).getData();
//			rxtxrobots.get(3).getData();
			for (int i = 0; i < prototype.size()-1; i++) {
					prototype.get(i).test(i);;
			}
			
			for (int i = 0; i < rxtxrobots.size(); i++) {
				if(rxtxrobots.get(i).portName.contains("/dev/rfcomm3")){
					RxtxClass temp = rxtxrobots.get(i);
					rxtxrobots.remove(i);
					temp.Stop();
				}
			}
//			ChirpBot temp;
//			for (int i = 0; i < prototype.size(); i++) {
//				if(prototype.get(i).getRxtx() == null){
//					temp = prototype.get(i);
//					prototype.remove(i);
//					prototype.add(temp);
//					break;
//				}
//			}
		}
		
		if(input.isKeyPressed(Input.KEY_R)){
			debugRender = !debugRender;
			start = true;
		}
			
		if(input.isKeyPressed(Input.KEY_A)){
			for (RxtxClass ser : rxtxrobots) {
				ser.sendSingleByte(0); //stops all robots
			}
			for (RxtxClass ser : rxtxrobots) {
				ChirpBot found = checkRxTx(ser);
				found.setRxtx(ser);
				found.getRxtx().getData(ser.portName);
				//prototype.get(0).getRxtx().getData(rxtxrobots.get(0).portName);
				
			}
		}
		//all keys below are for debugging
		if(input.isKeyPressed(Input.KEY_S)){
			for (RxtxClass ser : rxtxrobots) {
				ser.sendSingleByte(0);
				
			}
		}
		
//		if(input.isKeyPressed(Input.KEY_N)){ //debugging purpose only
//			System.out.println("port:"+ prototype.get(0).getRxtx().portName);
//			prototype.get(0).getRxtx().sendData(new byte[]{1,2,3,4});
//		}
//		if(input.isKeyPressed(Input.KEY_Y)){
//			prototype.get(0).setRxtx(rxtxrobots.get(0));
//			prototype.get(0).getRxtx().getData(rxtxrobots.get(0).portName);
//		}
//		if(input.isKeyPressed(Input.KEY_U)) 
//		{
//			prototype.get(0).getRxtx().sendSingleByte(114);
//			
//			//what's beneath does not really work
////			for (RxtxClass r : rxtxrobots)
////			{
////				r.sendRobotData(0, 0);
////				try 
////				{
////					Thread.sleep(50);
////				} 
////				catch (InterruptedException e)
////				{
////					e.printStackTrace();
////				}
////				r.Stop();
////			}
////			try {
////				Thread.sleep(1000);
////			} catch (InterruptedException e) {
////				e.printStackTrace();
////			}
////			rxtxrobots.clear();
////			rxtxrobots = RxtxClass.getRobotHandles();
////			System.out.println("Found "+rxtxrobots.size()+" robot(s)");
//		}
//		if(input.isKeyPressed(Input.KEY_I)) 
//		{
//			prototype.get(0).getRxtx().sendSingleByte(82);
//		
//		}
//		if(input.isKeyPressed(Input.KEY_U)) 
//		{
//			prototype.get(0).getRxtx().sendSingleByte(0);
//		
//		}
		if(input.isKeyPressed(Input.KEY_P)){
			prototype = new ArrayList<ChirpBot>();
			if(debug){
				for (int i = 0; i < numOfBots; i++) {
					ChirpBot temp =  new ChirpBot((float) Math.random()*w, (float) Math.random()*h, (float) Math.PI, i+"", this);
					prototype.add(temp);
				}
			}
			
		}
		if(input.isKeyPressed(Input.KEY_M)){
			prototype.get(0).printData();
		}
		
		
		
	}
	
	public long getTime() {
		return (Sys.getTime() * 1000) / Sys.getTimerResolution();
	}
	public static ChirpBoidWatcher sim;
	public static void main(String[] args) throws UnknownHostException {
		BufferedReader inFromUser = new BufferedReader(new InputStreamReader(System.in)); //the format used to receive data
		Multicast mc = new Multicast(Inet4Address.getLocalHost().getHostAddress()); //gets the data from the camera software
		sim = new ChirpBoidWatcher("simulator", mc);
		

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
			appgc.setShowFPS(false);
			appgc.setVSync(true);
			appgc.setAlwaysRender(true);
			appgc.start();
			
			
		} catch (SlickException ex) {
			Logger.getLogger(ChirpBoidWatcher.class.getName()).log(Level.SEVERE, null, ex);
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
		for (ChirpBot cb : prototype) {
			//this waits until the robot is ready for a new command, and sends a stop command.
			long time = System.currentTimeMillis();
			while (!cb.quit()) {
				if(System.currentTimeMillis() - 2000 >= time){
					cb.getRxtx().sendSingleByte(0);
					break;
				}
			}
		}
		for (RxtxClass r : rxtxrobots)
		{
//			r.sendSingleByte(0);
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
			fw.close();
		} catch (IOException e) {
			// TODO Auto-generated catch block
			e.printStackTrace();
		}
		System.exit(0);
		return true;
		
	}
	

	public ChirpBot nearest(double x, double y) {
         double dist = Double.MAX_VALUE;
         ChirpBot nearest = null;
         for (ChirpBot robot : prototype) {
             if(Math.hypot(x-robot.getX(), y-robot.getY())<dist){
                 dist = Math.hypot(x-robot.getX(), y-robot.getY());
                 nearest = robot;
             }
         }
         return nearest;
	 }
	
	@Override
	public void multiCastRecieved(String str) {
//		System.out.println(str);
		udpString = "";
		udpString = str;
		int count = 0;
		for (String part : str.split("\\|")) {
			if(count >=numOfBots)
				break;
        	if (!part.matches("(X\\d+:\\d\\.\\d+);(Y\\d+:\\d\\.\\d+);(R\\d+:\\d(\\.\\d+)?)"))
        		continue;
            String[] split = part.split(";");
            String id = split[0].split(":")[0].substring(1);
            float x = Float.parseFloat(split[0].split(":")[1])*w;
            float y = Float.parseFloat(split[1].split(":")[1])*h;
            float r = Float.parseFloat(split[2].split(":")[1]);
            
            updateRobotPosition(id, x, y, r);
            count++;
        }
	}

	private void updateRobotPosition(String id, float x, float y, float r) {
		boolean found = false;
		
		for(ChirpBot robot : prototype) {
			if (robot.getID().equals(id)) {
				robot.setAngle(r);
				robot.setPosition(x, y);
				found = true;
			}
		}
		if (!found) {
			if (prototype.size() == numOfBots) {
				ChirpBot nearest = nearest(x,y);
				nearest.setID(id);
				nearest.setAngle(r);
				nearest.setPosition(x, y);
				//TODO: check rxtx?
			} else {
				ChirpBot robot = new ChirpBot(x, y, r, id, this);
				//TODO: fix denne her..
				prototype.add(robot);
			}
		}
	}
	
	private ChirpBot checkRxTx(RxtxClass rxtx) //assigns the comm port to the robot seen on screen
	{
		String lastmsg = mc.getLastMsg();
		//System.out.println("end"+lastmsg);
		multiCastRecieved(lastmsg);
		ChirpBot[] robots = new ChirpBot[prototype.size()];
		double[] angles = new double[prototype.size()];
		for (int i = 0; i < angles.length; i++) 
		{
			ChirpBot rob = prototype.get(i);
			robots[i] = rob;
			angles[i] = rob.getAngle();
			//System.out.println("---- "+rob.getID()+" "+rob.getAngle());
		}
		//Start rotate:
		rxtx.sendSingleByte(114);
		try {
			Thread.sleep(500);
		
			rxtx.sendSingleByte(0);
			Thread.sleep(500);
		} catch (InterruptedException e) {
			e.printStackTrace();
		}
		lastmsg = mc.getLastMsg();
		//System.out.println("end"+lastmsg);
		multiCastRecieved(lastmsg);
		
		// who rotated?
		ChirpBot maxRotRob = robots[0];
		double maxRotAngle = 0;
		for (int i = 0; i < angles.length; i++) 
		{
			ChirpBot rob = robots[i];
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
			
			rxtx.sendSingleByte(76);;
		
			Thread.sleep(500);
			rxtx.sendSingleByte(0);
			Thread.sleep(500);
		} catch (InterruptedException e) {
			e.printStackTrace();
		}
		
		//System.out.println("robot "+maxRotRob.getID()+" got assigned: "+rxtx.portName);
		return maxRotRob;
	}

}
