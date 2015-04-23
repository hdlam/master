package no.ntnu.idi.chirp.multicast;

import java.io.BufferedReader;
import java.io.IOException;
import java.io.InputStreamReader;
import java.net.DatagramPacket;
import java.net.DatagramSocket;
import java.net.InetAddress;
import java.net.MulticastSocket;
import java.net.SocketTimeoutException;
import java.util.ArrayList;

public class Multicast
{
	
	int port = 52346;
	// Which address
	String group = "localhost";
	ArrayList<MulticastListener> listeners = new ArrayList<MulticastListener>();
	Reciever reciever = null;
	
	public Multicast()
	{
	}
	
	public Multicast(String group,int port)
	{
		this.port = port;
		this.group = group;
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
				System.out.println("Received data from: " + pack.getAddress().toString() +
						    ":" + pack.getPort() + " with length: " +
						    pack.getLength());
				System.out.write(pack.getData(),0,pack.getLength());
				System.out.println();
				
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
	
	public static void main(String[] args)
	{
		BufferedReader inFromUser = new BufferedReader(new InputStreamReader(System.in)); 
		
		Multicast mc = new Multicast();
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
