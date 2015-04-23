package com.idi.ntnu.chirpremote;

/**
 * Controls the flow of the game. 
 * This class uses the light intensity as seen by the robot to change state.
 * @author Magnus Hertzberg Ulstein
 *
 */
public class Game {
	private static int startThreshold;
	private static int endThreshold;
	private long startTime;
	private boolean gameStarted;
	private boolean leftStart;
	private float latestScore;
	
	public Game(){
		
	}
	
	/**
	 * Start a new game. Assumes the robot is located on the stared area.
	 */
	public void start(){
		gameStarted = true;
		leftStart = false;
	}
	
	/**
	 * Forces the currently running game to stop.
	 */
	public void stop() {
		gameStarted = false;
		leftStart = false;
	}
	
	/**
	 * Returns the value of the timer telling how long the game has taken.
	 * @return The number of seconds since the robot left the start area.
	 */
	public float getTime(){
		if (leftStart) {
			long time = System.currentTimeMillis() - startTime;
			float sec = time / 1000f;
			return sec;
		} else {
			return 0;
		}
	}
	
	/**
	 * Processes the current light intensity in order to determine if the state should be changed.
	 * @param lightIntensity The current light intensity as seen by the robot.
	 * @return Whether the game has ended.
	 */
	public boolean processInput(byte lightIntensity){
		if (!gameStarted){
			//Game has not started. victory is impossible
			return false;
		} else if (lightIntensity < endThreshold + 5){
			//We've won. yay.
			end();
			return true;
		} else if (!leftStart && lightIntensity > startThreshold + 5){
			leftStart = true;
			startTime = System.currentTimeMillis();
			return false;
		} else {
			return false;
		}
	}
	
	/**
	 * 
	 * @return Whether a game is running.
	 */
	public boolean isRunning() {
		return gameStarted;
	}
	
	/**
	 * Sets the intensity of the start area.
	 * @param thr The intensity of the start area.
	 */
	public static void setStartThreshold(int thr){
		startThreshold = thr + 5;
	}
	
	/**
	 * Sets the intensity of the end area.
	 * @param thr The intensity of the end area.
	 */
	public static void setEndThreshold(int thr){
		endThreshold = thr + 5;
	}
	
	/**
	 * Returns the end score of the game.
	 * @return The number of seconds between leaving the start area and reaching the end area.
	 */
	public float getScore(){
		return latestScore;
	}
	
	/**
	 * Should be called when the game has ended.
	 */
	private void end(){
		latestScore = getTime();
		gameStarted = false;
	}
}
