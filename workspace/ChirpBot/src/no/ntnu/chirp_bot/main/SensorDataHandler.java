package no.ntnu.chirp_bot.main;



public class SensorDataHandler 
{
	private String serialData;
	private String[] irStrs = {  "", "", "", "", "", "", "", "" };
	int lineMax = 150;
	int readCount = 0;
	boolean read = false;
	private double[] lineDist  = {  150, 150, 150, 150, 150, 150, 150, 150};
	private int dataindex;
	private int doneindex;
	private int i;

	public boolean newData(String data)
	{
		serialData = data;

		//Got the data..
		dataindex = serialData.indexOf("data:");
		doneindex = serialData.indexOf("done");
		if ( dataindex != -1 && doneindex != -1 && dataindex < doneindex)
		{

			try
			{
				String dataStr = serialData.substring(dataindex+5, doneindex).replaceAll("\n", "");
				irStrs = dataStr.split(",");
			}
			catch(Exception e)
			{
				e.printStackTrace();
				System.out.println("ERRORORORORO!!!\n"+serialData);
			}



			serialData = "";



			i = 0;
			// int highDist = 0;
			for (String str : irStrs)
			{

				if (i > 7)
				{
					System.out.println("ERROR.. GOT 9 SENSORS");
					break;
				}

				str = str.trim();

				if (isNumeric(str) && !str.equals(""))
				{  
					lineDist[i] = map(Integer.parseInt(str), 0, 200, 0, 1);//lineMax-(int)map(Integer.parseInt(str), 0, 150, 0, lineMax);
					// if (lineDist[i] > highDist) highDist = lineDist[i];
					//println("["+i+"] "+lineDist[i]);
				}
				i++;
			}
			return true;
		}
		//System.out.println( dataindex+" != -1 && "+doneindex+" != -1 && "+dataindex+" < "+doneindex+"\n"+data);
		return false;
	}

	public double[] getLineDist() 
	{
		return lineDist;
	}

	public static double map(double val, double orgStart,double orgEnd,double newStart,double newEnd)
	{
		double pc = (val-orgStart)/(orgEnd-orgStart);
		return ((newEnd-newStart)*pc)+newStart;
	}

	public static boolean isNumeric(String str)
	{
		for (char c : str.toCharArray())
		{
			if (!Character.isDigit(c)) return false;
		}
		return true;
	}
}
