package com.idi.ntnu.chirpremote;

import java.nio.ByteBuffer;
import java.util.ArrayList;

import android.util.Log;

import com.idi.ntnu.chirpremote.Bluetooth.BluetoothException;
import com.idi.ntnu.chirpremote.TouchSurface.TouchListener;

/**
 * A class to handle communication between the remote control and the robot.
 * The class contains functionality for sending commands to the robot, and to
 * receive data from the robot.
 * @author Erik Samuelsson
 *
 */
public class Commander implements TouchListener{

	private Bluetooth bluetooth;

	/**
	 * Constructor taking the Bluetooth connection object.
	 * @param bluetooth The connection used for communication.
	 */
	public Commander(Bluetooth bluetooth) {
		this.bluetooth = bluetooth;
	}
	
	/**
	 * Tell the robot that the game has ended with a victory.
	 * @throws BluetoothException
	 */
	public void sendVictory() throws BluetoothException {
		byte[] bytes = new byte[3];
		bytes[0] = 'w';
		bluetooth.send(bytes);
	}
	
	/**
	 * Tell the robot that the game is finished.
	 * @throws BluetoothException
	 */
	public void sendGameEnd() throws BluetoothException {
		byte[] bytes = new byte[3];
		bytes[0] = 'n';
		bluetooth.send(bytes);
	}
	
	/**
	 * Send the light threshold values to the robot. 
	 * @param minLightIntensity The light intensity of the start area.
	 * @param maxLightIntensity The light intensity of the end area.
	 * @throws BluetoothException
	 */
	public void sendTħreshold(byte minLightIntensity, byte maxLightIntensity) throws BluetoothException {
		byte[] bytes = new byte[3];
		bytes[0] = 't';
		bytes[1] = minLightIntensity;
		bytes[2] = maxLightIntensity;
		bluetooth.send(bytes);
	}
	
	boolean once = true;
	@Override
	public void event(double magnitude, double angle) {
		
		ArrayList<Float> data = new ArrayList<Float>();
		data.add(440.2802f);
		data.add(260.0599f);
		data.add(1.7825844f);
		
		data.add(674.65106f);
		data.add(211.5392f);
		data.add(1.0f);
		data.add(2.0f);
		
		data.add(532.15924f);
		data.add(167.01085f);
		data.add(3.0f);
		data.add(4.0f);
		
		data.add(108.11751f);
		data.add(290.60864f);
		data.add(5.0f);
		data.add(6.0f);
		
		
		sendBoidData(data);
		
	}
	public void sendBoidData(ArrayList<Float> data){
		byte byteArray[] =  new byte[data.size()*4];
		for(int i = 0; i < data.size(); i++){
			byte temp[] = float2ByteArrayReversed(data.get(i));
			for (int j = 0; j < 4; j++) {
				byteArray[i*4+j] = temp[j];
			}
		}
		StringBuffer sb = new  StringBuffer();
		for (int i = 0; i < byteArray.length; i++) {
			sb.append(byteArray[i]);
		}
		Log.d("test", sb.toString());
		try {
			bluetooth.send(byteArray);
		} catch (BluetoothException e) {
			e.printStackTrace();
		}
	
	}
	public static byte [] float2ByteArrayReversed (float value)
	{   
		byte temp[] = ByteBuffer.allocate(4).putFloat(value).array();
		for (int i = 0; i < temp.length; i++) {
			System.out.print((int)temp[i]);
		}
		for (int i = 0; i < temp.length/2; i++) {
			byte placeHolder = temp[3-i];
			temp[3-i] = temp[i];
			temp[i] = placeHolder;
		}
		System.out.println();
		return temp;
	}

	public static byte [] float2ByteArray (float value)
	{  
		return ByteBuffer.allocate(4).putFloat(value).array();
	}
//	@Override
//	public void event(double magnitude, double angle) {
//		byte[] bytes = new byte[3];
//		bytes[0] = 'm';
//
//		if (magnitude < 0.10){
//			bytes[1] = 0;
//			bytes[2] = 0;
//		} else {
//			double powerLeft;
//			double powerRight;
//			double innerMotor, outerMotor;
//			double angleAbs = Math.abs(angle);
//			
//			innerMotor = Math.cos(angleAbs*2);
//			if(angleAbs > Math.PI * 3f/4f) {
//				innerMotor *= -1;
//			}
//			if(angleAbs < Math.PI/2) {
//				outerMotor = 1;
//			} else if(angleAbs < Math.PI * 3f/4f) {
//				outerMotor = Math.cos(angleAbs*4);
//			} else {
//				outerMotor = -1;
//			}
//			
//			innerMotor = 1 - 2 * Math.acos(innerMotor) / Math.PI;
//			outerMotor = 1 - 2 * Math.acos(outerMotor) / Math.PI;
//			
//			
//			if(Math.signum(angle) > 0) {
//				powerRight = innerMotor;
//				powerLeft = outerMotor;
//			} else {
//				powerRight = outerMotor;
//				powerLeft = innerMotor;
//			}
//			powerLeft *= magnitude;
//			powerRight *= magnitude;
//			
//			bytes[1] = (byte) (127 * powerLeft);
//			bytes[2] = (byte) (127 * powerRight);
//		}
//		
//
//		try {
//			bluetooth.send(bytes);
//		} catch (BluetoothException e) {
//			// TODO Auto-generated catch block
//			e.printStackTrace();
//		}
//	}

}
