package com.idi.ntnu.chirpremote;

import android.app.Activity;
import android.content.pm.ActivityInfo;
import android.os.Bundle;
import android.os.Handler;
import android.view.KeyEvent;
import android.view.Menu;
import android.view.MenuInflater;
import android.view.MenuItem;
import android.view.MotionEvent;
import android.view.View;
import android.view.Window;
import android.view.WindowManager;
import android.widget.TextView;

import com.idi.ntnu.chirpremote.Bluetooth.BluetoothCommunicator;
import com.idi.ntnu.chirpremote.Bluetooth.BluetoothException;
import com.idi.ntnu.chirpremote.TouchSurface.Input;

/**
 * The main activity of the application.
 * @author Magnus Hertzberg Ulstein and Erik Samuelsson
 *
 */
public class MainActivity extends Activity implements BluetoothCommunicator {
	/**
	 * this is an edit * Whether or not the system UI should be auto-hidden
	 * after {@link #AUTO_HIDE_DELAY_MILLIS} milliseconds.
	 */
	private static final boolean AUTO_HIDE = false;

	/**
	 * If {@link #AUTO_HIDE} is set, the number of milliseconds to wait after
	 * user interaction before hiding the system UI.
	 */
	private static final int AUTO_HIDE_DELAY_MILLIS = 3000;

	/**
	 * If set, will toggle the system UI visibility upon interaction. Otherwise,
	 * will show the system UI visibility upon interaction.
	 */
	private Bluetooth bt;
	private Commander cmdr;
	private TouchSurface gui;
	private TextView status;
	private HighscoreList highscoreList;
	private Controller controller;
	private Settings settings;
			
		
	@Override
	protected void onCreate(Bundle savedInstanceState) {
		super.onCreate(savedInstanceState);
		bt = new Bluetooth(this, this);
		cmdr = new Commander(bt);
		
		requestWindowFeature(Window.FEATURE_NO_TITLE);
		getWindow().setFlags(WindowManager.LayoutParams.FLAG_FULLSCREEN,
                WindowManager.LayoutParams.FLAG_FULLSCREEN);
		setRequestedOrientation(ActivityInfo.SCREEN_ORIENTATION_NOSENSOR);
		
		getWindow().addFlags(WindowManager.LayoutParams.FLAG_KEEP_SCREEN_ON);

		setContentView(R.layout.layout);
		
		status = (TextView) findViewById(R.id.status_text);
		
		gui = (TouchSurface)findViewById(R.id.touchSurface);
		gui.setNotificationFrequency(10.0f);
		gui.setVibrate(true);
		gui.setInput(Input.TOUCH);
		
		highscoreList = (HighscoreList) findViewById(R.id.highscoreList);

		settings = new Settings(this, highscoreList);
		controller = new Controller(this, highscoreList, cmdr, settings);
		controller.setView(Controller.touch);

		PersistantStorage.load(this, highscoreList, settings);
		print("Start up complete.");

	}

	private void print(String string) {
		status.setText(string);
	}

	@Override
	protected void onPostCreate(Bundle savedInstanceState) {
		super.onPostCreate(savedInstanceState);

		// Trigger the initial hide() shortly after the activity has been
		// created, to briefly hint to the user that UI controls
		// are available.
		delayedHide(100);
	}

	/**
	 * Touch listener to use for in-layout UI controls to delay hiding the
	 * system UI. This is to prevent the jarring behavior of controls going away
	 * while interacting with activity UI.
	 */
	View.OnTouchListener mDelayHideTouchListener = new View.OnTouchListener() {
		@Override
		public boolean onTouch(View view, MotionEvent motionEvent) {
			if (AUTO_HIDE) {
				delayedHide(AUTO_HIDE_DELAY_MILLIS);
			}
			return false;
		}
	};

	Handler mHideHandler = new Handler();
	Runnable mHideRunnable = new Runnable() {
		@Override
		public void run() {

		}
	};

	@Override
	protected void onStop() {
		super.onStop();
		PersistantStorage.store(this, highscoreList, settings);
	}

	@Override
	protected void onDestroy() {
		try {
			bt.disconnect();
		} catch (BluetoothException e) {}
		super.onDestroy();
	}

	/**
	 * Schedules a call to hide() in [delay] milliseconds, canceling any
	 * previously scheduled calls.
	 */
	private void delayedHide(int delayMillis) {
		mHideHandler.removeCallbacks(mHideRunnable);
		mHideHandler.postDelayed(mHideRunnable, delayMillis);
	}

	byte receiveByte;
	@Override
	public void receiveBluetoothData(byte data) {
		receiveByte = data;
		runOnUiThread(new Runnable() {
			
			@Override
			public void run() {
				controller.setLightIntensity(receiveByte);				
			}
		});

	}
	
	@Override
	public boolean onCreateOptionsMenu(Menu menu) {
		
	    MenuInflater inflater = getMenuInflater();
	    inflater.inflate(R.menu.options_menu, menu);
	    return true;
	}
	

	@Override
	public boolean onOptionsItemSelected(MenuItem item) {
	    // Handle item selection
	    switch (item.getItemId()) {
	        case R.id.connect:
	        	if(controller.getView() != Controller.touch) {
	        		controller.setView(Controller.touch);
	        	}
	        	
	        	if(bt.isConnected()) {
        			try {
        				gui.disableInput();
        				gui.removeListener(cmdr);
        				controller.stop();
						bt.disconnect();
						print("Disconnected");
	            		item.setTitle("Connect");
					} catch (BluetoothException e) {
						print(e.getMessage());
					}
	            } else {
	            	try {
						bt.connect();
						gui.addListener(cmdr);
						gui.enableInput();
						print("Connected");
						item.setTitle("Disconnect");
					} catch (BluetoothException e) {
						print(e.getMessage());
					}
	            }
	            
	            return true;
	            
	        case R.id.highscore:
	        	if(controller.getView() == Controller.touch) {
	        		gui.disableInput();
	        	}
	        	controller.setView(Controller.score);
        		return true;
	        
	        case R.id.settings:
	        	if(controller.getView() == Controller.touch) {
	        		gui.disableInput();
	        	}
	        	controller.setView(Controller.settings);
	        	return true;
	        default:
	            return super.onOptionsItemSelected(item);
	    }
	}

	@Override
	public boolean onKeyDown(int keyCode, KeyEvent event) {
		if(keyCode == KeyEvent.KEYCODE_BACK) {
			String currentView = controller.getView();
			if(currentView == Controller.settings || currentView == Controller.score) {
				controller.setView(Controller.touch);
				if(bt.isConnected()) {
					gui.enableInput();
				}
				return true;
			}
		}
		return super.onKeyDown(keyCode, event);
	}
}
