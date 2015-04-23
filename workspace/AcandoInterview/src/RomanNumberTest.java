import static org.junit.Assert.*;

import org.junit.BeforeClass;
import org.junit.Test;


public class RomanNumberTest {
	private static RomanConverter converter;
	
	@BeforeClass
	public static void init(){
		converter = new RomanConverter();
	}
	
	@Test
	public void test() {
		assertEquals("Tester I = 1", 1, converter.convert("I"));
		assertEquals("Tester I = 1", 2, converter.convert("II"));
		assertEquals("Tester III = 3", 3, converter.convert("III"));
		assertEquals("Tester IV = 4", 4, converter.convert("IV"));
		assertEquals("Tester IV = 6", 6, converter.convert("VI"));
		assertEquals("Tester IX = 9", 9, converter.convert("IX"));
		
		
	}
	
	

	

}
