package no.ntnu.chirp_bot.main;

import android.app.Activity;
import android.bluetooth.BluetoothAdapter;
import android.bluetooth.BluetoothDevice;
import android.bluetooth.BluetoothSocket;
import android.content.Intent;
import android.os.Bundle;
import android.os.Handler;
import android.view.Menu;
import android.view.View;
import android.view.View.OnClickListener;
import android.widget.Button;
import android.widget.TextView;
import no.ntnu.chirp_bot.R;
import no.ntnu.chirp_bot.drawables.SensorView;

import java.io.IOException;
import java.io.InputStream;
import java.io.OutputStream;
import java.lang.reflect.InvocationTargetException;
import java.lang.reflect.Method;
import java.util.Set;
import java.util.UUID;

public class ChirpActivity extends Activity implements OnClickListener 
{

	BluetoothAdapter 	btAdapter;
	BluetoothDevice		btDevice;
	BluetoothSocket		btSocket;
	private OutputStream 	btOutputStream;
	private InputStream 	btInputStream;
	final Handler 			inputHandler	= new Handler();
	private boolean 		stopWorker		= false;
	private boolean			stopKit			= true;
	boolean connected 		= false;

	Button connectButton;
	SensorView sensorView;
	Button fwdButton;
	Button revButton;
	Button rightButton;
	Button leftButton;
	Button rightfwdButton;
	Button leftfwdButton;
	Button stopButton;
	TextView statusText;
	private Thread workerThread;
	private Thread kitThread;
	
	public SensorDataHandler sdh = new SensorDataHandler();
	


	@Override
	protected void onCreate(Bundle savedInstanceState) 
	{
		super.onCreate(savedInstanceState);
		setContentView(R.layout.activity_chirp);

		connectButton = (Button) findViewById(R.id.connectbutton);
		connectButton.setOnClickListener(this);

		sensorView = (SensorView) findViewById(R.id.sensorView1);
		sensorView.setOnClickListener(this); 
		
		fwdButton = (Button) findViewById(R.id.forwardbutton);
		fwdButton.setOnClickListener(this);
		revButton = (Button) findViewById(R.id.reversebutton);
		revButton.setOnClickListener(this);
		rightButton = (Button) findViewById(R.id.rightbutton);
		rightButton.setOnClickListener(this);
		leftButton = (Button) findViewById(R.id.leftbutton);
		leftButton.setOnClickListener(this);
		rightfwdButton = (Button) findViewById(R.id.rightbuttonfwd);
		rightfwdButton.setOnClickListener(this);
		leftfwdButton = (Button) findViewById(R.id.leftbuttonfwd);
		leftfwdButton.setOnClickListener(this);
		stopButton = (Button) findViewById(R.id.stopbutton);
		stopButton.setOnClickListener(this);

		statusText = (TextView) findViewById(R.id.statustext);
		statusText.setOnClickListener(this);
		
		
	}

	@Override
	public boolean onCreateOptionsMenu(Menu menu) 
	{
		// Inflate the menu; this adds items to the action bar if it is present.
		getMenuInflater().inflate(R.menu.activity_chirp, menu);
		return true;
	}

	@Override
	public void onClick(View v) 
	{
		//Clicking the status text clears it.
		if(v == statusText)
		{
			statusText.setText("");
		}
		
		//connect to the bluetooth receiver.
		if(v == connectButton && !connected)
		{
			connectBT();
		}
		else if(v == connectButton && connected)//disconnect from the bot.
		{
			try
			{
				stopWorker = true;
				btOutputStream.close();
				btInputStream.close();
				btSocket.close();
				connectButton.setText("Connect");
				connected = false;
				System.out.println("Disconnected from the device.");
				statusText.setText("Disconnected from the device.");
			} 
			catch (IOException e) 
			{
				statusText.setText("Unable to disconnect.");
				e.printStackTrace();
			}
		}
		else if(btOutputStream != null)// all other buttons trigger commands to be sendt to the bot.
		{
			try 
			{
				stopKit = true;
				if(kitThread != null) kitThread.interrupt();
				if(v == fwdButton || v == sensorView)
				{
					btOutputStream.write("d0100".getBytes());//set forward LED

					btOutputStream.write("L1f40".getBytes());//Set left speed
					btOutputStream.write("R1f40".getBytes());//Set right speed
					btOutputStream.write("G0000".getBytes());//Start the motors
				}
				else if(v == revButton)
				{
					btOutputStream.write("d0001".getBytes());//set reverse LED

					btOutputStream.write("Lf448".getBytes());
					btOutputStream.write("Rf448".getBytes());
					btOutputStream.write("G0000".getBytes());
				}
				else if(v == stopButton)
				{
					btOutputStream.write("H0000".getBytes());//set speed = 0
					btOutputStream.write("00000".getBytes());// turn off power to the motors.
					
					btOutputStream.write("d0000".getBytes());
					if(kitThread == null || !kitThread.isAlive()) startKit();//start the "spinning" LED animation.
				}
				else if(v == rightButton)
				{
					btOutputStream.write("d0400".getBytes());//set right LED
					
					btOutputStream.write("Rf448".getBytes());
					btOutputStream.write("L0bb8".getBytes());
					btOutputStream.write("G0000".getBytes());
				}
				else if(v == rightfwdButton)
				{
					btOutputStream.write("d0200".getBytes());//set right forward LED
					
					btOutputStream.write("R07d0".getBytes());
					btOutputStream.write("L0bb8".getBytes());
					btOutputStream.write("G0000".getBytes());
				}
				else if(v == leftButton)
				{
					btOutputStream.write("d0004".getBytes());//set left LED
					
					btOutputStream.write("R0bb8".getBytes());
					btOutputStream.write("Lf448".getBytes());
					btOutputStream.write("G0000".getBytes());
				}
				else if(v == leftfwdButton)
				{
					btOutputStream.write("d0008".getBytes());//set left forward LED
					
					btOutputStream.write("R0bb8".getBytes());
					btOutputStream.write("L07d0".getBytes());
					btOutputStream.write("G0000".getBytes());
				}
			} 
			catch (IOException e) 
			{
				statusText.setText("Error sending to the device.");
				System.out.println("Error sending to the device.");
				e.printStackTrace();
			}
		}
		else//not connected
		{
			System.out.println("Not connected to device.");
			statusText.setText("Not connected to device.");
		}

	}
	
	/*
	 * Runs an animation on the LEDs
	 */
	private void startKit() 
	{
		stopKit = false;
		kitThread = new Thread(new Runnable() 
		{
			
			@Override
			public void run() 
			{
				System.out.println("start kit");
				while(!Thread.currentThread().isInterrupted() && !stopKit)
				{
					try 
					{
						btOutputStream.write("d0100".getBytes());
						Thread.sleep(100);
						btOutputStream.write("d0200".getBytes());
						Thread.sleep(100);
						btOutputStream.write("d0400".getBytes());
						Thread.sleep(100);
						btOutputStream.write("d0800".getBytes());
						Thread.sleep(100);
						btOutputStream.write("d0001".getBytes());
						Thread.sleep(100);
						btOutputStream.write("d0002".getBytes());
						Thread.sleep(100);
						btOutputStream.write("d0004".getBytes());
						Thread.sleep(100);
						btOutputStream.write("d0008".getBytes());
						Thread.sleep(100);
					} 
					catch (IOException e) 
					{
						break;
					} 
					catch (InterruptedException e) 
					{
						break;
					}
				}
				System.out.println("stop kit");
				stopKit = true;
			}
		});
		kitThread.start();
		
	}

	//Connects to the firefly bt adapter. (they have to be paired in advance)
	private void connectBT()
	{
		//Get the BT adapter
		btAdapter = BluetoothAdapter.getDefaultAdapter();
		if(btAdapter == null) 
		{
			statusText.setText("No bluetooth on this device");
			return;
		}

		//enable BT
		if(!btAdapter.isEnabled())
		{
			Intent enableBluetooth = new Intent(BluetoothAdapter.ACTION_REQUEST_ENABLE);
			startActivityForResult(enableBluetooth, 0);
			return;
		}

		//Find the device:
		Set<BluetoothDevice> pairedDevices = btAdapter.getBondedDevices();
		if(pairedDevices.size() > 0)
		{
			System.out.println("Found BT devices:");
			for(BluetoothDevice device : pairedDevices)
			{
				System.out.println(device.getName());
				if(device.getName().equals("FireFly-854B")) //Note, you will need to change this to match the name of your device
				{

					System.out.println("Found the device: "+device.getName());
					statusText.setText("Found the device: "+device.getName());
					btDevice = btAdapter.getRemoteDevice(device.getAddress());
					break;
				}
			}

		}

		//Connect to the device:
		try 
		{
			UUID uuid = UUID.fromString("00001101-0000-1000-8000-00805F9B34FB"); //Standard SerialPortService ID

			try 
			{
				Method m = btDevice.getClass().getMethod("createRfcommSocket", new Class[] {int.class});
				btSocket = (BluetoothSocket) m.invoke(btDevice, 1);
			} 
			catch (NullPointerException e) 
			{
				btSocket = btDevice.createRfcommSocketToServiceRecord(uuid);
			}

			btAdapter.cancelDiscovery();

			btSocket.connect();
			btOutputStream 	= btSocket.getOutputStream();
			btInputStream 	= btSocket.getInputStream();

			//Set up the inputstream handler:
			System.out.println("lets start the new inputstream worker..");
			stopWorker = false;
			workerThread = new Thread(new Runnable()
			{
				private String newData;

				public void run()
				{
					try {
						
						byte[] buffer = new byte[1024];
						int bufferindex = 0;
						int bytesAvailable;
						Runnable runner = new Runnable() 
						{
							
							@Override
							public void run() 
							{
								//System.out.println("adding text; "+sdh.newData(newData.trim()));
								if(sdh.newData(newData))
								{
									sensorView.updateLines(sdh.getLineDist());
								}
								
								//statusText.setText(newData);
							}
						};
						
						System.out.println("worker started.");
						while(!Thread.currentThread().isInterrupted() && !stopWorker)
						{
							//fetch the input:
							bytesAvailable = btInputStream.available();
							
							if(bytesAvailable > 0)
							{
								//data available
								//System.out.println("got data from device..");
								btInputStream.read(buffer, bufferindex, bytesAvailable);
								bufferindex += bytesAvailable;
								for (byte b : buffer) 
								{
									if(b == '\n')//got new line:
									{
										//System.out.println("got newline from device.");
										newData = new String(buffer,0,bufferindex+1);
										//send to event handler:
										inputHandler.post(runner);
										//reset buffer:
										buffer = new byte[1024];
										bufferindex = 0;
									}
								}
							}
							try 
							{
								Thread.sleep(1);
							} 
							catch (InterruptedException e) {}
						}
						
						
					} 
					catch (IOException e) 
					{
						e.printStackTrace();
					}
				}
			});
			workerThread.start();

		} 
		catch (IOException e) 
		{
			statusText.setText("Failed to connect to the device.");
			System.out.println("Failed to connect to the device.");
			e.printStackTrace();
			return;
		} 
		catch (NoSuchMethodException e) {
			e.printStackTrace();
		} catch (IllegalArgumentException e) {
			e.printStackTrace();
		} catch (IllegalAccessException e) {
			e.printStackTrace();
		} catch (InvocationTargetException e) {
			e.printStackTrace();
		}

		statusText.setText("Connected to the device.");
		System.out.println("Connected to the device.");
		connectButton.setText("Disconnect");
		connected = true;
	}
}
