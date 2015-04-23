package com.idi.ntnu.chirpremote;

import android.app.Activity;
import android.content.SharedPreferences;
import android.content.SharedPreferences.Editor;
import android.view.View;
import android.view.View.OnClickListener;
import android.widget.Button;
import android.widget.CheckBox;
import android.widget.TableLayout;
import android.widget.TextView;

import com.idi.ntnu.chirpremote.TouchSurface.Input;

/**
 * Handles the settings view and all its functionality
 * @author Erik Samuelsson
 *
 */
public class Settings implements OnClickListener {
	
	private byte minLightIntensity, maxLightIntensity, currentLightIntensity;
	
	private TableLayout settingsLayout;
	
	private static final String minLightIntensityKey = "minLightIntensity";
	private static final String maxLightIntensityKey = "maxLightIntensity";
	
	// light
	private TextView currentLightText, minLightText, maxLightText;
	private Button minLightButton, maxLightButton;
	
	// highscore
	private Button resetButton;
	private HighscoreList highscoreList;
	
	private TouchSurface gui;
	private CheckBox vibrateCheckBox;
	private CheckBox touchCheckBox;
	private CheckBox tiltCheckBox;
	
	/**
	 * Constructs the object and hides it.
	 * @param activity
	 * @param highscoreList
	 */
	public Settings(Activity activity, HighscoreList highscoreList) {
		settingsLayout = (TableLayout) activity.findViewById(R.id.settingsLayout);
		currentLightText = (TextView) activity.findViewById(R.id.currenLightText);
		minLightText = (TextView) activity.findViewById(R.id.minLightText);
		maxLightText = (TextView) activity.findViewById(R.id.maxLightText);		
		minLightButton = (Button) activity.findViewById(R.id.minLightButton);
		maxLightButton = (Button) activity.findViewById(R.id.maxLightButton);
		
		resetButton = (Button) activity.findViewById(R.id.resetButton);
		this.highscoreList = highscoreList;
		
		gui = (TouchSurface) activity.findViewById(R.id.touchSurface);
		vibrateCheckBox = (CheckBox) activity.findViewById(R.id.vibrateCheckBox);
		touchCheckBox = (CheckBox) activity.findViewById(R.id.touchCheckBox);
		tiltCheckBox = (CheckBox) activity.findViewById(R.id.tiltCheckBox);



		
		minLightButton.setOnClickListener(this);
		maxLightButton.setOnClickListener(this);
		resetButton.setOnClickListener(this);
		
		vibrateCheckBox.setOnClickListener(this);
		touchCheckBox.setOnClickListener(this);
		tiltCheckBox.setOnClickListener(this);
		
		hide();

	}
	
	/**
	 * Loads the setting configuration found in persistent storage.
	 * @param settings
	 */
	public void load(SharedPreferences settings) {
		setMinLightIntensity((byte) settings.getInt(minLightIntensityKey, 0));
		setMaxLightIntensity((byte) settings.getInt(maxLightIntensityKey, 0));
	}
	
	/**
	 * Stores the current settings in persistent storage.
	 * @param settings
	 */
	public void store(SharedPreferences settings) {
		Editor editor = settings.edit();
		editor.putInt(minLightIntensityKey, minLightIntensity);
		editor.putInt(maxLightIntensityKey, maxLightIntensity);
		editor.commit();
	}
	
	/**
	 * Shows this view.
	 */
	public void show() {
		settingsLayout.setVisibility(View.VISIBLE);
	}
	
	/**
	 * Hides this view.
	 */
	public void hide() {
		settingsLayout.setVisibility(View.GONE);
	}

	@Override
	public void onClick(View v) {
		
		switch(v.getId()) {
		case R.id.minLightButton:
			setMinLightIntensity(currentLightIntensity);
			break;
		case R.id.maxLightButton:
			setMaxLightIntensity(currentLightIntensity);
			break;
		case R.id.resetButton:
			highscoreList.resetScores();
			break;
		case R.id.vibrateCheckBox:
			gui.setVibrate(vibrateCheckBox.isChecked());
			break;
		case R.id.touchCheckBox:
			gui.setInput(Input.TOUCH);
			touchCheckBox.setChecked(true);
			tiltCheckBox.setChecked(false);
			break;
		case R.id.tiltCheckBox:
			gui.setInput(Input.TILT);
			touchCheckBox.setChecked(false);
			tiltCheckBox.setChecked(true);
			break;
		}
		
	}

	/**
	 * Returns the configured light intensity of the end area.
	 * @return The light intensity.
	 */
	public byte getMinLightIntensity() {
		return minLightIntensity;
	}

	/**
	 * Configures the light intensity of the start area.
	 * @param minLightIntensity
	 */
	private void setMinLightIntensity(byte minLightIntensity) {
		this.minLightIntensity = minLightIntensity;
		minLightText.setText(""+minLightIntensity);

	}

	/**
	 * Returns the configured light intensity of the start area.
	 * @return
	 */
	public byte getMaxLightIntensity() {
		return maxLightIntensity;
	}

	/**
	 * Configures the light intensity of the start area.
	 * @param maxLightIntensity
	 */
	private void setMaxLightIntensity(byte maxLightIntensity) {
		this.maxLightIntensity = maxLightIntensity;
		maxLightText.setText(""+maxLightIntensity);
	}

	/**
	 * Sets the current light intensity.
	 * @param currentLightIntensity
	 */
	public void setCurrentLightIntensity(byte currentLightIntensity) {
		this.currentLightIntensity = currentLightIntensity;
		currentLightText.setText(""+currentLightIntensity);
	}

	
}
