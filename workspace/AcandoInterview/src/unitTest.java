import static org.junit.Assert.*;

import org.junit.After;
import org.junit.AfterClass;
import org.junit.Before;
import org.junit.BeforeClass;
import org.junit.Test;


public class unitTest {

	@BeforeClass
	public static void first(){
		System.out.println("first");
	}
	
	@Before
	public void init() {
		System.out.println("before");
	}
	
	@Test
	public void test2(){
		assertFalse(false);
		assertTrue("hello", true);
		System.out.println("test1");
	}
	
	@Test(expected = NullPointerException.class)
	public void name() {
		throw new NullPointerException();
	}
	
	@After
	public void after(){
		System.out.println("after");
	}

	@AfterClass
	public static void last(){
		System.out.println("last");
	}
	
}
