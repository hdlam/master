package com.idi.ntnu.chirpremote;

import android.content.Context;
import android.content.SharedPreferences;
import android.graphics.Color;
import android.graphics.Typeface;
import android.util.AttributeSet;
import android.widget.TextView;

/**
 * Handles the high scores of the game and the high score table.
 * @author Erik Samuelsson
 *
 */
public class HighscoreList extends TextView{
	
	private static final int listLength = 10;
	private static final int usernameLengthLimit = 9;
	
	private String[] usernames = new String[listLength];
	private float[] scores = new float[listLength];
	
	private int scoreCount;
	
	// the keys used for retrieving and storing information in the persistent storage.
	private static final String 
		scoreCountKey = "sck",
		usernameKey = "unk",
		scoreKey = "sk";
	
	public HighscoreList(Context context, AttributeSet attrs, int defStyle) {
		super(context, attrs, defStyle);
		init();
	}

	public HighscoreList(Context context, AttributeSet attrs) {
		super(context, attrs);
		init();
	}

	public HighscoreList(Context context) {
		super(context);
		init();
	}
	
	/**
	 * Initializes the list at construction.
	 */
	private void init() {
		setBackgroundColor(Color.BLACK);
		
		setTextSize(28);
		setTypeface(Typeface.MONOSPACE);
		setTextColor(Color.LTGRAY);
		
		printScores();
		
	}
	
	/**
	 * Retrieves the high scores from persistent storage.
	 * @param settings The settings object allowing access to persistent storage.
	 */
	public void loadScores(SharedPreferences settings) {
		scoreCount = settings.getInt(scoreCountKey, 0);
		
		for(int i=0; i<scoreCount; i++) {
			usernames[i] = settings.getString(usernameKey + i, null);
			scores[i] = settings.getFloat(scoreKey + i, -1);
		}
		
		printScores();

	}
	
	/**
	 * Stores the high scores in persistent storage.
	 * @param settings The settings object allowing access to persistent storage.
	 */
	public void storeScores(SharedPreferences settings) {
		SharedPreferences.Editor editor = settings.edit();
		
		editor.putInt(scoreCountKey, scoreCount);
		for(int i=0; i<scoreCount; i++) {
			editor.putString(usernameKey + i, usernames[i]);
			editor.putFloat(scoreKey + i, scores[i]);
		}
		
		editor.commit();
	}

	/**
	 * Adds a score to the list. If the score is not good enough to fit in the list, it is simply discarded.
	 * @param name The user name of the player.
	 * @param seconds The inverse score in number of seconds (lower is better).
	 * @return Whether the score was good enough to be added to the list.
	 */
	public boolean addScore(String name, float seconds) {		
		int place;
		for(place = 0; place < scoreCount; place++) {
			if(seconds <= scores[place]) {
				break;
			}
		}
		
		if(place >= listLength) {
			return false;
		}
		
		for(int i=Math.min(scoreCount - 1, listLength - 2); i>=place; i--) {
			usernames[i+1] = usernames[i];
			scores[i+1] = scores[i];
		}
		
		usernames[place] = name;
		scores[place] = seconds;
		
		if(scoreCount < listLength) {
			scoreCount++;
		}
		
		printScores();
		
		return true;	
	}
	
	/**
	 * Whether a score will be good enough to earn a place in the list.
	 * @param score The score in number of seconds.
	 * @return Whether the score would be added.
	 */
	public boolean scoreWillQualify(int score) {
		return listLength > scoreCount || score < scores[listLength];
	}
	
	/**
	 * Resets the high score list by removing all scores.
	 */
	public void resetScores() {
		scoreCount = 0;
		printScores();
	}
	
	/**
	 * Updates the GUI to reflect the current contents of the list.
	 */
	private void printScores() {		
		StringBuilder builder = new StringBuilder();
		
		for(int i=0; i<scoreCount; i++) {
			if(i+1 < 10) {
				builder.append(' ');
			}
			builder.append(i+1);
			
			builder.append(". ");
			builder.append(usernames[i]);
			for(int j=usernames[i].length(); j < usernameLengthLimit; j++) {
				builder.append(' ');
			}
			
			int add = 1;
			if(scores[i] < 10) {
				add++;
			}
			while(add-- > 0) {
				builder.append(' ');
			}
			if(scores[i] < 100) {
				String rawScore = Float.toString(scores[i]);
				String scoreString;
				if(rawScore.contains(".")) {
					scoreString = rawScore.substring(0,rawScore.indexOf(".")+2);	
				} else {
					scoreString = rawScore + ".0";
				}
				builder.append(scoreString+"s");
			} else {
				builder.append("BAD!");
			}
			
			builder.append("\n");
		}
		for(int i=scoreCount; i<listLength; i++) {
			if(i+1 < 10) {
				builder.append(' ');
			}
			builder.append(i+1);
			builder.append(".\n");
		}
		
		builder.deleteCharAt(builder.length()-1);
		setText(builder.toString());
	}
	
}
