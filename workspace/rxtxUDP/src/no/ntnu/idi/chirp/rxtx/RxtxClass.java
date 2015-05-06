package no.ntnu.idi.chirp.rxtx;

import java.nio.ByteBuffer;
import java.util.ArrayList;
import java.util.Arrays;
import java.util.Collections;

import jssc.SerialPortException;
//HD

public class RxtxClass implements Runnable
{

	private Serial myPort = null;
	private boolean running = true;
	public String portName;
	private String lastData = "";
	private byte rChar = 'r';
	private byte lChar = 'l';
	RxtxListener chirpBotListener;
	private StringBuffer dataReceived;

	private static String[] getPortList()
	{
		String[] portNames = Serial.list();
		for (int i = 0; i < portNames.length; i++)
		{
			System.out.println(portNames[i]);
		}
		return portNames;
	}
	
	public RxtxClass(String portName) 
	{
		this.portName = portName;
		dataReceived = new StringBuffer();
	}
	
	public void setChirpBotListener(RxtxListener chirpBotListener) {
		this.chirpBotListener = chirpBotListener;
	}
	
	public boolean isRunning(){
		return running;
	}
	
	public void sendRobotData(int leftSpeed, int rightSpeed)
	{
		//System.out.println(portName+" setting l:"+leftSpeed+" r:"+rightSpeed);
		if(rightSpeed < 0)
			rChar = 'R';
		else
			rChar = 'r';
		if(leftSpeed < 0)
			lChar = 'L';
		else
			lChar = 'l';
		sendData(new byte[]{rChar,toByte(rightSpeed),lChar,toByte(leftSpeed)});
	}
	
	public void sendSingleByte(int b){
		byte byteArr[] = {(byte)b};
		sendData(byteArr);
	}
	
	public void sendData(String data)
	{
		sendData(data.getBytes());
	}
	
	public void sendBoidData(ArrayList<Float> data){
		byte byteArray[] =  new byte[data.size()*4+1];
		byteArray[0] = (byte)100;
		for(int i = 0; i < data.size(); i++){
			byte temp[] = float2ByteArrayReversed(data.get(i));
			for (int j = 0; j < 4; j++) {
				byteArray[i*4+j+1] = temp[j];
			}
		}
		sendData(byteArray);
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
	
	public void sendData(byte[] data)
	{
		try
		{
			if(myPort == null) myPort = new Serial(portName, 9600);
			myPort.write(data);
		}
		catch (UnsatisfiedLinkError e)
		{
			System.out.println(e.getMessage());
			e.printStackTrace();
			return;
		}
		catch (SerialPortException e) 
		{
			System.out.println(e.getMessage());
			//e.printStackTrace();
			return;
		}
	}
	
	public void getData(){
		System.out.println("starting thread: "+portName);
		Thread thread = new Thread(this);
		thread.start();
	}
	
	public void getData(String portName)
	{
		this.portName = portName;

		System.out.println("starting thread: "+portName);
		Thread thread = new Thread(this);
		thread.start();
	}
	public void Stop()
	{
		System.out.println("Stopping..");
		if(myPort != null) myPort.dispose();
		running = false;
	}
	
	private boolean identifyRobot()
	{
		long timeout = System.currentTimeMillis();
		if(!portName.startsWith("/dev/rfcomm")) return false;
//		if(portName.contains("/dev/rfcomm3")) return false;
//		if(!portName.startsWith("/dev/ttyACM")) return false;
		try
		{
			myPort = new Serial(portName, 9600);
		}
		catch (UnsatisfiedLinkError e)
		{
			System.out.println(e.getMessage());
		}
		catch (SerialPortException e) 
		{
			System.out.println(e.getMessage());
		}
		System.out.println("time used: "+(System.currentTimeMillis() - timeout));
		timeout = System.currentTimeMillis();
		while(true)
		{
			while (myPort.available() > 0) // If data is available,
			{
				System.out.println("got: "+(char)myPort.read()+" "+(System.currentTimeMillis() - timeout));
				if(new Character((char)myPort.read()) == 'r')
				{
					//got robot
					System.out.println("got robot at: "+portName);
					return true;
				}
			}
			if(System.currentTimeMillis() - timeout > 2000) 
			{
				System.out.println(portName+" reader timed out..");
				myPort.dispose();
				myPort = null;
				return false;
			}
		}
	}
	
	public static ArrayList<RxtxClass> getRobotHandles()
	{
		ArrayList<RxtxClass> robots = new ArrayList<RxtxClass>();
		String[] portlist = RxtxClass.getPortList();
		for (String prt : portlist)
		{
			
			RxtxClass mc = new RxtxClass(prt);
			if(mc.identifyRobot())robots.add(mc);
			
		}
		return robots;
	}
	
	@Override
	public void run()
	{
		
		try
		{
			
			if(myPort == null) myPort = new Serial(portName, 9600);
		}
		catch (UnsatisfiedLinkError e)
		{
			System.out.println(e.getMessage());
			return;
		}
		catch (SerialPortException e) 
		{
			System.out.println(e.getMessage());
			return;
		}
		System.out.println("CTS: "+myPort.getCTS());
		System.out.println("DSR: "+myPort.getDSR());
		System.out.println("opening port: "+portName);
		String val = "";
		long timeout = System.currentTimeMillis();
		while(running)
		{
			while (myPort.available() > 0) // If data is available,
			{
				val += new Character((char)myPort.read()); // read it and store it in val
			}
			if(!val.equals(""))
			{
				if(chirpBotListener != null && !val.contains("r"))
				{
					dataReceived.append(val);
				}
				if(chirpBotListener != null && val.equals("c")){
						String obtainedData = dataReceived.toString();
						String[] velReceived = obtainedData.split(",");
//						System.out.println(Float.parseFloat(velReceived[0])+"float parsed successfully");
						chirpBotListener.dataReceived(Float.parseFloat(velReceived[0]), Float.parseFloat(velReceived[1]));
						dataReceived =  new StringBuffer(); //resets the data 
				}
				if(chirpBotListener != null && val.equals("r"))
					chirpBotListener.robotIsReady();
//				System.out.println(portName+" --- "+val);//(System.currentTimeMillis() - timeout));
			}
			if(!val.equals("")) timeout = System.currentTimeMillis();
			if(System.currentTimeMillis() - timeout > 5000) 
			{
				System.out.println("reader timed out..");
				running = false;
				break;
			}
			val = "";
		}
		System.out.println("closing port: "+portName);
		myPort.dispose();
		myPort.stop();
	}
	
	public static byte toByte(int byteVal)
	{
		return (byte)(Math.abs(byteVal)-128);
	}
	
	
	
//	public static void main(String[] args0)
//	{
//		ArrayList<RxtxClass> robots = getRobotHandles();
//		for (RxtxClass r : robots)
//		{
//			r.sendRobotData(200, 200);
//			try
//			{
//				Thread.sleep(2000);
//			}
//			catch (InterruptedException e)
//			{
//				e.printStackTrace();
//			}
//			r.sendRobotData(0, 0);
//		}
//		for (RxtxClass mc : robots)
//		{
//			mc.Stop();
//		}
//		System.out.println("done..");
//		
//	}

	

}