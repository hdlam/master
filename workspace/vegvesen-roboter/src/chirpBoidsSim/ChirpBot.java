/*
 * To change this template, choose Tools | Templates
 * and open the template in the editor.
 */
package chirpBoidsSim;

import java.nio.ByteBuffer;
import java.util.ArrayList;

import no.ntnu.idi.chirp.rxtx.RxtxClass;
import no.ntnu.idi.chirp.rxtx.RxtxListener;

import org.newdawn.slick.Color;
import org.newdawn.slick.Graphics;
import org.newdawn.slick.geom.Vector2f;
/** 
 * @author HD
 */
public class ChirpBot implements RxtxListener{

	private Vector2f pos,vel;
	private ChirpBoidWatcher cbw;
	private float currentAngle; // in radians
	private String ID;
	private RxtxClass rxtx = null;
	private int timeSinceLastRxTx = 0;
	private int rxtxUpdateTime = 16*10; //we need to have 160 here or else the robot will not be able to process the data fast enough
	private boolean canSend;
	private boolean quit;
	/*
	 * double speed = 0; double mxspd = 50; double accel = 1000000000; double
	 * deacc = 15; double turn = 0; double mxturn = 1; double lookAhead = mxspd
	 * * 4; Robot robotAhead;
	 */

	int t = -1;
	public void test(int i){
		t = i;
	}
	
	public void dataReceived(float x, float y){
		vel.x = x; vel.y = y;
	}

	public void robotIsReady() {
		canSend = true;
	}
	
	
	public String getID() {
		return ID;
	}
	public void setID(String ID) {
		this.ID = ID;
	}
	
	public void setRxtx(RxtxClass rxtx) {
		this.rxtx = rxtx;
		rxtx.setChirpBotListener(this);
		canSend = false;
	}
	
	public RxtxClass getRxtx(){
		return rxtx;
	}

	public ChirpBot(float initX, float initY, float initAngle,
			String id, ChirpBoidWatcher cbw) {
		this.cbw = cbw;
		pos = new Vector2f(initX, initY);
		vel = new Vector2f(0, 0);
		currentAngle = initAngle;
		ID = id;
		canSend = true;
		quit = false;
		
	}
	
	public void canSend(){
		canSend = true;
	}
	
	
	float sumX = 0;
	float sumY = 0;
	int posCount = 1;
	int AVG = 5;
	public void setPosition(float x, float y){
		if(cbw.averaging){
			if(posCount++ % AVG == 0){
				pos.x = (x+sumX) / (float)AVG;
				pos.y = (y+sumY) / (float)AVG;
				sumX = 0;
				sumY = 0;
			}
			else{
				sumX += x;
				sumY += y;		
			}
		}
		else{
			pos.x = x;
			pos.y = y;
		}
		
	}
	
	private float sumAngle = 0;
	private int angleCount = 1;
	
	public void setAngle(float angle){
		if(cbw.averaging){
			if(angleCount++ % AVG == 0){
				currentAngle = (angle+sumAngle)/AVG;
				sumAngle = 0;
			}
			else{
				sumAngle += angle;
			}
		}
		else
			currentAngle = angle;
	}
	
	
	public void draw(Graphics g) {
		g.setColor(Color.white);
		g.drawString(getID(), getX(), getY());
		g.drawString(pos.x + "," + pos.y, getX(), getY()+12);
		g.drawString(vel.x + "," +vel.y+ "|"+currentAngle, (float)getX(), (float)getY()+24);
		if(rxtx != null) g.drawString(ID+"-"+rxtx.portName.substring(7)+"", (float)getX(), (float)getY());
		
		if(canSend)
			g.setColor(Color.green);
		else
			g.setColor(Color.red);
		g.drawOval(getX()-cbw.size/2, getY()-cbw.size/2, cbw.size, cbw.size);
		float a = currentAngle;
		g.setColor(Color.cyan);
		g.drawLine(getX(), getY(), (float) (pos.x + cbw.size/2 * Math.cos(a) + Math.sin(a)), (float)(pos.y + cbw.size/2 * Math.sin(a) - Math.cos(a)));
		
		a = (float) Math.atan2(vel.y, vel.x);
		if(vel.length()>0){
			g.setColor(Color.blue);
			g.drawLine(getX(), getY(), (float) (pos.x + vel.length() * Math.cos(a) + Math.sin(a)), (float)(pos.y + vel.length() * Math.sin(a) - Math.cos(a)));
		}
//		g.setColor(Color.green);
//		g.drawString(t+"", pos.x, pos.y-12);
	}
	
	private void tempMove(long delta){
		pos.x += vel.x/delta;
		pos.y += vel.y/delta;
		if(pos.x > cbw.w)
			pos.x = 0;
		if(pos.x < 0)
			pos.x = cbw.w;
		if(pos.y > cbw.h)
			pos.y = 0;
		if(pos.y < 0)
			pos.y = cbw.h;
	}
	
	
	private void createTempData(ArrayList<Float> data, int size){
		int numOfInts = 3+4*(size-1);
		float i=0;
		while(data.size() < numOfInts){
			data.add(i);
			i++;
		}
		System.out.println(data.size());
	}
	public void update(long delta) {
		refractor();
		
		//tempMove(delta);
		
//		int[] motorSpeed = {0,0};//getMotor();
		timeSinceLastRxTx += delta;
		if(timeSinceLastRxTx < rxtxUpdateTime)
		{
			return;
		}
		else{ 
			timeSinceLastRxTx = 0;
		}
		
//		ArrayList<Float> data = new ArrayList<Float>();//float[3+(cbw.prototype.size()-1)*4];
//		this.vel.x += (float)Math.random()*120 - 60;
//		this.vel.y += (float)Math.random()*120 - 60;
//		this.currentAngle = (float) Math.atan2(vel.y, vel.x);
//		tempMove(delta);
		if(rxtx != null && canSend && !quit) {
			/* 3 floats for posx, posy, angle
			*  4 floats for each OTHER chirpBots, because the first 3 bytes already have been used for "this".
			*  4 floats; posx, posy, velx, vely;
			*/
			ArrayList<Float> data = new ArrayList<Float>();//float[3+(cbw.prototype.size()-1)*4];
//			tempData(data);

			data.add(getX());
			data.add(getY());
			data.add(getAngle());
			
			for (int i = 0; i < cbw.prototype.size(); i++) {
				ChirpBot current =  cbw.prototype.get(i);
				if(current == this)
					continue;
				data.add(current.getX());
				data.add(current.getY());
				data.add(current.getVelX());
				data.add(current.getVelY());
			}
			
			//TODO debug
//			createTempData(data, 5);
			
			
//			System.out.println("attempting to send data");
//			if(data.size() < 15){
//				tempData(data, data.size());
//			}
			rxtx.sendBoidData(data);
			canSend = false;
//			rxtx.sendRobotData(motorSpeed[0], motorSpeed[1]);
		}
//		if(rxtx == null){
//			Vector2f snitt = new Vector2f(0,0);
//			int num = 0;
//			for (ChirpBot cb : cbw.prototype) {
//				if(cb.getRxtx() != null)
//					continue;
//				snitt.add(new Vector2f(cb.getX(), cb.getY()));
//				num++;
//			}
//			snitt.scale(1f/num);
//			vel.x = -30;//snitt.x - pos.x;
//			vel.y = 0;//snitt.y - pos.y;
//		}
	}
	
	
	public void printData(){
		ArrayList<Float> data = new ArrayList<Float>();//float[3+(cbw.prototype.size()-1)*4];
//		tempData(data);
		data.add(getX());
		data.add(getY());
		data.add(getAngle());
		
		for (int i = 0; i < cbw.prototype.size(); i++) {
			ChirpBot current =  cbw.prototype.get(i);
			if(current == this)
				continue;
			data.add(current.getX());
			data.add(current.getY());
			data.add(current.getVelX());
			data.add(current.getVelY());
		}
		createTempData(data, cbw.prototype.size());
		printBoidData(data);
	}
	
	public void printBoidData(ArrayList<Float> data){
		byte byteArray[] =  new byte[data.size()*4+1];
		byteArray[0] = (byte)100;
		for(int i = 0; i < data.size(); i++){
			byte temp[] = float2ByteArrayReversed(data.get(i));
			for (int j = 0; j < 4; j++) {
				byteArray[i*4+j+1] = temp[j];
			}
		}
		System.out.println("data start");
		for (int i = 0; i < byteArray.length; i++) {
			System.out.println(byteArray[i]);
		}
		System.out.println("data end");
		System.out.println("length is " + byteArray.length);
		
	}
	public static byte [] float2ByteArrayReversed (float value)
	{   
		byte temp[] = ByteBuffer.allocate(4).putFloat(value).array();
//		for (int i = 0; i < temp.length; i++) {
//			System.out.print((int)temp[i]);
//		}
		for (int i = 0; i < temp.length/2; i++) {
			byte placeHolder = temp[3-i];
			temp[3-i] = temp[i];
			temp[i] = placeHolder;
		}
//		System.out.println();
		return temp;
	}
	
	
	public boolean quit(){
		quit = true;
		if(rxtx == null)
			return true;
		if(!rxtx.isRunning())
			return true;
		if(canSend){
			rxtx.sendSingleByte(0);
			return true;
		}
		return false;
	}
	
	

	public float getVelX() {
		return vel.x;
	}
	public float getVelY() {
		return vel.y;
		
	}
	private void refractor() { // makes sure that the angle is always inside
								// [-PI, PI]
		if (currentAngle > Math.PI) {
			currentAngle -= 2 * Math.PI;
		} else if (currentAngle < -Math.PI) {
			currentAngle += 2 * Math.PI;
		}
	}


	/**
	 * GETTERS
	 * 
	 * 
	 */
	
	public Vector2f getPos(){
		return pos;
	}
	
	public float getX() {
		return pos.x;
	}

	public float getY() {
		return pos.y;
	}

	public float getAngle() {
			return currentAngle;
		
	}
	
	
	




}
