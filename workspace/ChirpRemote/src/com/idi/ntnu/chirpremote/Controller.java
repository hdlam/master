package com.idi.ntnu.chirpremote;

import android.app.Activity;
import android.view.View;
import android.view.View.OnClickListener;
import android.widget.Button;
import android.widget.TableLayout;
import android.widget.TextView;

import com.idi.ntnu.chirpremote.Bluetooth.BluetoothException;

/**
 * Controller handling the game and changing between views.
 * @author Erik Samuelsson
 *
 */
public class Controller implements OnClickListener {

	private Game game;
	private Settings settingsView;
	
	private Activity activity;
	private TextView statusText;
	private TableLayout highscoreLayout;
	private TextView nameText;
	private Button addButton, startButton;
	private HighscoreList highscoreList;
	private TouchSurface gui;
	private Commander commander;
	
	public static final String score = "Scoreboard", touch = "Remote", settings = "Settings";
	private String activeView = touch;
	
	public static final String PREFS_NAME = "a string";
	
	/**
	 * Constructor taking all the necessary components of the application.
	 * @param activity2 
	 * @param highscoreList
	 * @param commander 
	 * @param settings
	 */
	public Controller(Activity activity2, HighscoreList highscoreList, Commander commander, Settings settings) {
		this.activity = activity2;
		this.statusText = (TextView) activity.findViewById(R.id.status_text);
		this.highscoreLayout = (TableLayout) activity.findViewById(R.id.highscoreLayout);
		this.nameText = (TextView) activity.findViewById(R.id.usernameText);
		this.addButton = (Button) activity.findViewById(R.id.highscoreAddButton);
		this.startButton = (Button) activity.findViewById(R.id.highscoreStartButton);
		this.highscoreList = highscoreList;
		this.gui = (TouchSurface) activity.findViewById(R.id.touchSurface);
		this.commander = commander;
		this.settingsView = settings;
		
		addButton.setOnClickListener(this);
		startButton.setOnClickListener(this);
		
		game = new Game();
		disableEdit();

	}
	
	/**
	 * Start the timer. Should be called when the robot exits the start area.
	 */
	private void startTimer() {
		Thread thread = new Thread(new Runnable() {
			
			@Override
			public void run() {
				while(game.isRunning()) {
					gui.setTime(game.getTime());
					
					try {
						Thread.sleep(20);
					} catch (InterruptedException e) {}
				}
				
			}
		});
		
		thread.start();
	}
	
	/** 
	 * Ends the timer. Should be called when the robot enters the end area.
	 */
	private void endTimer() {
		gui.setTime(0);
	}
	
	/**
	 * Enables the functionality for adding a high score.
	 */
	private void enableEdit() {
		nameText.setEnabled(true);
		addButton.setEnabled(true);
	}
	
	/**
	 * Disables the functionality for adding a high score.
	 */
	private void disableEdit() {
		nameText.setEnabled(false);
		addButton.setEnabled(false);
	}
	
	/**
	 * Sets the view of the application to one of the defined views.
	 * @param view The view to be displayed.
	 */
	public void setView(String view) {
		if(view != activeView) {
			if(view == touch) {
				gui.setVisibility(View.VISIBLE);
				highscoreLayout.setVisibility(View.GONE);
				settingsView.hide();
			} else if(view == score) {
				gui.setVisibility(View.GONE);
				highscoreLayout.setVisibility(View.VISIBLE);
				settingsView.hide();
			} else if(view == settings){
				gui.setVisibility(View.GONE);
				highscoreLayout.setVisibility(View.GONE);
				settingsView.show();
			}
			activeView = view;
		}
	}
	
	/**
	 * 
	 * @return The view currently displayed.
	 */
	public String getView() {
		return activeView;
	}
	
	/**
	 * Starts a new game. The robot should be located on the start area.
	 */
	private void startGame() {
		Game.setStartThreshold(settingsView.getMaxLightIntensity());
		Game.setEndThreshold(settingsView.getMinLightIntensity());
		try {
			commander.sendTħreshold(settingsView.getMinLightIntensity(), settingsView.getMaxLightIntensity());
		} catch (BluetoothException e) {
			print(e.getMessage());
		}
		game.start();
		
		setView(touch);
		gui.enableInput();
		startButton.setEnabled(false);
		
		print("Game started");
		startTimer();
	}
	
	/**
	 * Ends the currently active game. The robot should be located on the end area.
	 */
	private void endGame() {
		enableEdit();
		startButton.setEnabled(true);
		endTimer();
		print("Game ended");
		try {
			commander.sendVictory();
		} catch (BluetoothException e) {
			print(e.getMessage());
		}
		setView(score);
	}
	
	/**
	 * Forces the current game to stop.
	 */
	public void stop() {
		if(game.isRunning()) {
			game.stop();
			startButton.setEnabled(true);
			endTimer();
			print("Game canceled");
		}
	}

	@Override
	public void onClick(View v) {
		
		switch(v.getId()) {
		case R.id.highscoreAddButton:
			String name = nameText.getText().toString();
			highscoreList.addScore(name, game.getScore());
			disableEdit();
			try {
				commander.sendGameEnd();
			} catch (BluetoothException e) {
				print(e.getMessage());
			}
			break;
		case R.id.highscoreStartButton:		
			startGame();
		}
		
	}

	/**
	 * Sets the current light intensity.
	 * @param data A signed byte representing the current light intensity as seen by the robot.
	 */
	public void setLightIntensity(byte data) {
		settingsView.setCurrentLightIntensity(data);
		StringBuilder b = new StringBuilder(8);
		for(int i=7; i >= 0; i--) {
			byte mask = (byte) Math.pow(2, i);
			if((data & mask) != 0) {
				b.append(1);
			} else {
				b.append(0);
			}
		}
		if(game.processInput(data)) {
			endGame();
		}
		
	}
	
	/**
	 * Displays a message in the status area.
	 * @param string The message to be displayed.
	 */
	private void print(String string) {
		statusText.setText(string);
	}
}
