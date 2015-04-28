package no.ntnu.idi.chirp;

import java.io.BufferedReader;
import java.io.IOException;
import java.io.InputStreamReader;
import java.net.*;
import java.util.ArrayList;
import java.util.Arrays;
import java.util.logging.Level;
import java.util.logging.Logger;

import org.newdawn.slick.AppGameContainer;
import org.newdawn.slick.SlickException;

import trafficsim.TrafficSim2;

public class Multicast
{
	
	int port = 52346;
	String group;
	ArrayList<MulticastListener> listeners = new ArrayList<MulticastListener>();
	Reciever reciever = null;
	volatile String lastMsg = "";
	
	public Multicast(String group)
	{
        this.group = group;
	}
	
	public Multicast(String group,int port)
	{
		this(group);
		this.port = port;
	}
	
	public void addListener(MulticastListener listener)
	{
		listeners.add(listener);
	}
	
	public void send(String data) throws IOException
	{
		// Create the socket but we don't bind it as we are only going to send data
		MulticastSocket s = new MulticastSocket();
		// Note that we don't have to join the multicast group if we are only
		// sending data and not receiving
		// Fill the buffer with some data
		byte buf[] = data.getBytes();
		// Create a DatagramPacket 
		DatagramPacket pack = new DatagramPacket(buf, buf.length,
							 InetAddress.getByName(group), port);
		// Do a send. Note that send takes a byte for the ttl and not an int.
		System.out.println("Sending..");
		s.send(pack);
		// And when we have finished sending data close the socket
		s.close();
	}

	public void openRecieve() throws IOException
	{
		Reciever rec = new Reciever();
		Thread reciever = new Thread(rec);
		this.reciever = rec;
		reciever.start();
	}
	
	public void closeRecieve()
	{
		if(this.reciever != null)
			this.reciever.stop();
	}
	
	public String getLastMsg() 
	{
		return lastMsg;
	}
	
	class Reciever implements Runnable
	{
		
		// Create a DatagramPacket and do a receive
		byte buf[] = new byte[1024];
		boolean running = true;
		
		public void stop()
		{
			running = false;
			System.out.println("closing reciever port..");
		}
		
		@Override
		public void run()
		{
			// Create the socket and bind it to port 'port'.
			DatagramSocket s = null;
			try
			{
				s = new DatagramSocket(port);
			
				// join the multicast group
				//s.joinGroup(InetAddress.getByName(group));
			}
			catch (IOException e)
			{
				e.printStackTrace();
			}
			// Now the socket is set up and we are ready to receive packets
			while(running)
			{
				DatagramPacket pack = null;
				//System.out.println("start recieve...");
				
				try
				{
					pack = new DatagramPacket(buf, buf.length, InetAddress.getByName(group), port);
					s.setSoTimeout(500); //timeout of 1sec
					s.receive(pack);
				}
				catch (SocketTimeoutException e)
				{
					//timeout
				}
				catch (NullPointerException e)
				{
					e.printStackTrace();
					break;
				}
				catch (IOException e)
				{
					e.printStackTrace();
					break;
				}
				if((pack.getAddress() == null) || pack.getLength() == buf.length) continue;
				// Finally, let us do something useful with the data we just received,
				// like print it on stdout :-)
//				System.out.println("Received data from: " + pack.getAddress().toString() +
//						    ":" + pack.getPort() + " with length: " +
//						    pack.getLength());
//                System.out.write(pack.getData(),0,pack.getLength());
                lastMsg = new String(pack.getData());
                for (MulticastListener l : listeners) {
					l.multiCastRecieved(new String(pack.getData()));
				}

			}
			// And when we have finished receiving data leave the multicast group and
			// close the socket
			try
			{
				//s.leaveGroup(InetAddress.getByName(group));
				s.close();
				System.out.println("port closed");
			}
			catch (NullPointerException e)
			{
				e.printStackTrace();
			}
			/*catch (IOException e)
			{
				e.printStackTrace();
			}*/
			
		}
		
	}
	
	public static void main(String[] args) throws UnknownHostException {
		BufferedReader inFromUser = new BufferedReader(new InputStreamReader(System.in));
		
		Multicast mc = new Multicast("127.0.0.1");//Inet4Address.getLocalHost().getHostAddress());
		TrafficSim2 sim = new TrafficSim2("simulator",mc);

		mc.addListener(sim);
		
		try
		{
			mc.openRecieve();
		}
		catch (IOException e1)
		{
			e1.printStackTrace();
		}
		
		try
		{
			AppGameContainer appgc;
			appgc = new AppGameContainer(sim);
			appgc.setDisplayMode(800, 800, false);
			appgc.start();
			
		}
		catch (SlickException ex)
		{
			Logger.getLogger(TrafficSim2.class.getName()).log(Level.SEVERE, null, ex);
		}
        
		try
		{
			mc.openRecieve();
		}
		catch (IOException e1)
		{
			e1.printStackTrace();
		}

		try
		{
			mc.send(inFromUser.readLine());
		}
		catch (IOException e)
		{
			e.printStackTrace();
		}
		mc.closeRecieve();
	}
}
