import java.io.BufferedReader;
import java.io.FileReader;
import java.io.FileWriter;
import java.io.IOException;
 
public class BufferedReaderExample {
 
	public static void main(String[] args) {
		String file = "simulScenario1_";
		
		try 
		{
			BufferedReader br1 = new BufferedReader(new FileReader(file+"1.csv"));
			BufferedReader br2 = new BufferedReader(new FileReader(file+"2.csv"));
			BufferedReader br3 = new BufferedReader(new FileReader(file+"3.csv"));
			BufferedReader br4 = new BufferedReader(new FileReader(file+"4.csv"));
			BufferedReader br5 = new BufferedReader(new FileReader(file+"5.csv"));
//			BufferedReader br6 = new BufferedReader(new FileReader(file+"06.csv"));
//			BufferedReader br7 = new BufferedReader(new FileReader(file+"07.csv"));
//			BufferedReader br8 = new BufferedReader(new FileReader(file+"08.csv"));
//			BufferedReader br9 = new BufferedReader(new FileReader(file+"09.csv"));
//			BufferedReader br10 = new BufferedReader(new FileReader(file+"10.csv"));
			FileWriter fw = new FileWriter(file + "avgTotal.csv");
			String sCurrentLine;
			String[] s1 = new String[7];
			String[] s2 = new String[7];
			String[] s3 = new String[7];
			String[] s4 = new String[7];
			String[] s5 = new String[7];
//			String[] s6 = new String[7];
//			String[] s7 = new String[7];
//			String[] s8 = new String[7];
//			String[] s9 = new String[7];
//			String[] s10 = new String[7];
			Double[] output = new Double[7];
			while ((sCurrentLine = br1.readLine()) != null) {
				s1 = sCurrentLine.split(",");
				s2 = br2.readLine().split(",");
				s3 = br3.readLine().split(",");
				s4 = br4.readLine().split(",");
				s5 = br5.readLine().split(",");
//				s6 = br6.readLine().split(",");
//				s7 = br7.readLine().split(",");
//				s8 = br8.readLine().split(",");
//				s9 = br9.readLine().split(",");
//				s10 = br10.readLine().split(",");
				
				for (int i = 0; i < output.length; i++) {
					output[i] = (Double.parseDouble(s1[i])+
							Double.parseDouble(s2[i])+
							Double.parseDouble(s3[i])+
							Double.parseDouble(s4[i])+
							Double.parseDouble(s5[i])
//							Double.parseDouble(s6[i])+
//							Double.parseDouble(s7[i])+
//							Double.parseDouble(s8[i])+
//							Double.parseDouble(s9[i])+
//							Double.parseDouble(s10[i])
							)/5.0
							;
				}
				fw.write(output[0] + "," + output[1] +","+ output[2] + "," + output[3] + "," + output[4] + "," + output[5] + "," + output[6] + "\n");
				
			}
			
			br1.close();
			br2.close();
			br3.close();
			br4.close();
			br5.close();
//			br6.close();
//			br7.close();
//			br8.close();
//			br9.close();
//			br10.close();
			fw.close();
 
		} catch (IOException e) {
			e.printStackTrace();
		}
		System.out.println("complete");
	}
}
 
	