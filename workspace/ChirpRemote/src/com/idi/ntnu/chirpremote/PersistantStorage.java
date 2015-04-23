package com.idi.ntnu.chirpremote;

import android.app.Activity;

/**
 * Handles persistent storage in the application by calling all code responsible for this. 
 * @author Erik Samuelsson
 *
 */
public class PersistantStorage {
	
	public static final String PREFS_NAME = "a string";
	
	/**
	 * Stores the current settings and high scores in persistent storage.
	 * @param activity
	 * @param highscoreList
	 * @param settings
	 */
	public static void store(Activity activity, HighscoreList highscoreList, Settings settings) {
		highscoreList.storeScores(activity.getSharedPreferences(PREFS_NAME,0));
		settings.store(activity.getSharedPreferences(PREFS_NAME,0));
	}
	
	/**
	 * Loads settings and high scores from persistent storage.
	 * @param activity
	 * @param highscoreList
	 * @param settings
	 */
	public static void load(Activity activity, HighscoreList highscoreList, Settings settings) {
		highscoreList.loadScores(activity.getSharedPreferences(PREFS_NAME,0));
		settings.load(activity.getSharedPreferences(PREFS_NAME,0));

		
	}
}
