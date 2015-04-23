package no.ntnu.idi.chirp.rxtx;

public interface RxtxListener {
	
	void dataReceived(float x, float y);
	void robotIsReady();

}
