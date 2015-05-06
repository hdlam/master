import java.io.FileWriter;
import java.io.IOException;
import java.nio.ByteBuffer;
import java.util.ArrayList;
import java.util.Arrays;


public class Test {
	
	public static void writeFile2() throws IOException {
		FileWriter fw = new FileWriter("out.txt");
	 
		for (int i = 0; i < 10; i++) {
			fw.write("something\n");
		}
	 
		fw.close();
	}
	public static void main(String[] args) {
		try {
			writeFile2();
		} catch (IOException e) {
			// TODO Auto-generated catch block
			e.printStackTrace();
		}
	}
}
