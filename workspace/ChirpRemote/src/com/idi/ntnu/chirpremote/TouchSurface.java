package com.idi.ntnu.chirpremote;

import java.util.LinkedList;

import android.content.Context;
import android.graphics.Bitmap;
import android.graphics.BitmapFactory;
import android.graphics.Canvas;
import android.graphics.Color;
import android.graphics.DashPathEffect;
import android.graphics.Paint;
import android.graphics.PointF;
import android.graphics.RadialGradient;
import android.graphics.Rect;
import android.graphics.RectF;
import android.graphics.Shader.TileMode;
import android.hardware.Sensor;
import android.hardware.SensorEvent;
import android.hardware.SensorEventListener;
import android.hardware.SensorManager;
import android.os.Vibrator;
import android.util.AttributeSet;
import android.view.MotionEvent;
import android.view.SurfaceHolder;
import android.view.SurfaceView;

/**
 * A surface displaying the user interface for controlling the robot.
 * @author Erik Samuelsson
 *
 */
public class TouchSurface extends SurfaceView implements SurfaceHolder.Callback, SensorEventListener{

	private SurfaceHolder holder;
	
	// paints
	private final Paint paintThickBright = new Paint();
	private final Paint paintThinDark = new Paint();
	private final Paint paintThinDarkDash = new Paint();
	private final Paint paintBackgroundGradient = new Paint();
	private final Paint paintText = new Paint();
	private final int textSize = 50;
	private PointF touchPoint = new PointF();
	private int pointerId;
	
	private Bitmap icon;
	private Rect iconRectSrc;
	private RectF iconRectDst;
	
	private float circleRadius;
	private float radius;
	private PointF center = new PointF();
	private float magnitude = 0.0f;
	private float angle;
	
	
	private long lastNotification;
	private long notificationPeriod = 1000;
	private Vibrator vibrator;
	private boolean vibrateOn = false;
	private SensorManager sensorManager;
	
	private float time;
	
	// added to support a 2.3.3 device where surfaceChanged is called after onDraw
	private boolean surfaceChangedHasBeenCalled = false;		
	
	private float xRot, yRot;
	final float[] mValuesMagnet      = new float[3];
    final float[] mValuesAccel       = new float[3];
    final float[] mValuesOrientation = new float[3];
    final float[] mRotationMatrix    = new float[9];
		
	public static enum Input {
		NO_INPUT,
		TOUCH,
		TILT
	}
	
	private Input activeInput = Input.NO_INPUT;
	private boolean isInputActive = false;
	
	private LinkedList<TouchListener> listeners = new LinkedList<TouchListener>();
	
	/**
	 * The interface used by objects wishing to receive touch information.
	 * @author Erik Samuelsson
	 *
	 */
	public interface TouchListener {
		/**
		 * Is called when the touch point of the touch surface is changed.
		 * @param magnitude The magnitude of motion. A magnitude of 0 means no speed while 1 means full speed.
		 * @param angleInRadians The direction of motion. Straight ahead is 0, full right is +PI and full right is -PI.
		 */
		public void event(double magnitude, double angleInRadians);
	}
	
	public TouchSurface(Context context, AttributeSet attrs, int defStyle) {
		super(context, attrs, defStyle);
		init(context);
	}



	public TouchSurface(Context context, AttributeSet attrs) {
		super(context, attrs);
		init(context);
	}


	
	public TouchSurface(Context context) {
		super(context);
		init(context);
	}
	
	/**
	 * Sets the gradient of the touch surface's circle.
	 * @param paint The paint used to draw the background.
	 * @param radius The radius of the gradient.
	 */
	private void setGradient(Paint paint, float radius) {
		int color = 0xff30b0ff;
		RadialGradient gradient = new RadialGradient(center.x, center.y, radius, color, Color.BLACK, TileMode.CLAMP);
		
        paint.setShader(gradient);
	}
		
	/**
	 * Initiates the touch surface after construction.
	 * @param context
	 */
	private void init(Context context) {
		
		vibrator = (Vibrator) context.getSystemService(Context.VIBRATOR_SERVICE);
		sensorManager = (SensorManager) context.getSystemService(Context.SENSOR_SERVICE);
		
		icon = BitmapFactory.decodeResource(getResources(), R.drawable.chirpbot2);
		iconRectSrc = new Rect(0, 0, icon.getWidth(), icon.getHeight()); 
		iconRectDst = new RectF(0, 0, 0, 0);


		holder = getHolder();
        setBackgroundColor(Color.BLACK);
        
        paintBackgroundGradient.setAntiAlias(true);
        paintBackgroundGradient.setDither(true);
        
        paintThickBright.setColor(Color.LTGRAY);
        paintThickBright.setStyle(Paint.Style.STROKE);
        paintThickBright.setStrokeWidth(6);
        paintThickBright.setAntiAlias(true);
        paintThinDark.setColor(Color.GRAY);
        paintThinDark.setStyle(Paint.Style.STROKE);
        paintThinDark.setStrokeWidth(3);
        paintThinDark.setAntiAlias(true);
        paintThinDarkDash.setColor(Color.GRAY);
        paintThinDarkDash.setStyle(Paint.Style.STROKE);
        paintThinDarkDash.setStrokeWidth(3);
        paintThinDarkDash.setAntiAlias(true);
        
        paintText.setColor(Color.GRAY);
        paintText.setTextSize(textSize);
        paintText.setAntiAlias(true);
        
        paintThinDarkDash.setPathEffect(new DashPathEffect(new float[] {10,10}, 5));
        
        holder.addCallback(this);
                
    }
	
	/**
	 * Activate the tilt input.
	 */
	public void activateTilt() {
		Thread thread = new Thread(new Runnable() {
        	
			@Override
			public void run() {
				while(isInputActive && activeInput == Input.TILT) {
					SensorManager.getRotationMatrix(mRotationMatrix, null, mValuesAccel, mValuesMagnet);
					SensorManager.getOrientation(mRotationMatrix, mValuesOrientation);
					xRot = mValuesOrientation[2];
					yRot = -mValuesOrientation[1];
					
					
					float x = center.x + radius * xRot * 2;
					float y = center.y + radius * yRot * 3;
					
					if(true) {
						// weighted smoothing
						float w1 = 0.2f, w2 = 1 - w1;
						x = w1 * x + w2 * touchPoint.x;
						y = w1 * y + w2 * touchPoint.y;
						
					}
					setTouchPoint(x,y);
					
					postInvalidate();
					
					try {
						Thread.sleep(20);
					} catch (InterruptedException e) {}
				}
			}
		});
		thread.start();
	}
	
	/**
	 * Enables the surface. When enabled, the surface will accept input and send events.
	 */
	public void enableInput() {
		isInputActive = true;
		invalidate();
		if(vibrateOn) {
			vibrator.vibrate(50);
		}
		if(activeInput == Input.TILT) {
			activateTilt();
		}
	}
	
	/**
	 * Disables input. When disabled, the surface will not accept input and not send events.
	 */
	public void disableInput() {
		isInputActive = false;
		setTouchPoint(center.x, center.y);
		notifyListenersForce();
		invalidate();
		if(vibrateOn) {
			vibrator.vibrate(50);
		}
	}

	/**
	 * Activates or deactivates the vibration functionality of the surface.
	 * @param on true if vibrate is on, false otherwise.
	 */
	public void setVibrate(boolean on) {
		vibrateOn = on;
	}
	
	/**
	 * Whether vibration is enabled.
	 * @return
	 */
	public boolean isVibrateOn() {
		return vibrateOn;
	}
	
	/**
	 * Whether input is active.
	 * @return
	 */
	public boolean isInputActive() {
		return isInputActive;
	}
	
	/**
	 * Sets the input method.
	 * @param inputType Either NO_INPUT, TOUCH or TILT
	 */
	public void setInput(Input inputType) {
		if(inputType == activeInput) {
			return;
		}
					
		switch(activeInput) {
		case NO_INPUT:
			break;
		case TOUCH:
			break;
		case TILT:
			sensorManager.unregisterListener(this);
		}
		
		activeInput = inputType;
		
		switch(inputType) {
		case NO_INPUT:
			break;
		case TOUCH:
			setTouchPoint(center.x, center.y);
			break;
		case TILT:
			sensorManager.registerListener(this, sensorManager.getDefaultSensor(Sensor.TYPE_ACCELEROMETER),SensorManager.SENSOR_DELAY_GAME);
			sensorManager.registerListener(this, sensorManager.getDefaultSensor(Sensor.TYPE_MAGNETIC_FIELD),SensorManager.SENSOR_DELAY_GAME);
			activateTilt();
			break;
		}
		
	}

	@Override
	protected void onDraw(Canvas canvas) {
		super.onDraw(canvas);
		
		if(!surfaceChangedHasBeenCalled) return;
		boolean showCrosshairs = isInputActive;
		boolean showIcon = isInputActive;

		float fingerRadius = (0.75f + (1-magnitude)/2) * circleRadius;
		
		float iconWidthFromCenter, iconHeightFromCenter;
		iconWidthFromCenter = iconRectSrc.right * (0.75f + (1-magnitude)/2) / 4;
		iconHeightFromCenter = iconRectSrc.bottom * (0.75f + (1-magnitude)/2) / 4;
		
		iconRectDst.set(touchPoint.x-iconWidthFromCenter, touchPoint.y-iconHeightFromCenter, 
				touchPoint.x+iconWidthFromCenter, touchPoint.y+iconHeightFromCenter);
		
		
		
		// draw gradient
		setGradient(paintBackgroundGradient, radius*1.2f);
		canvas.drawCircle(center.x, center.y, radius*1.2f, paintBackgroundGradient);
		
		
		// draw background
		// icon
		if(showIcon) {
			for(float factor = 0.1f; factor < 1.0; factor += 0.1f) {
				float x = center.x + (touchPoint.x - center.x) * factor;
				float y = center.y + (touchPoint.y - center.y) * factor;
				canvas.drawCircle(x, y, fingerRadius * magnitude * factor, paintThinDark);
			}			
		}

		// draw foreground
		if(showCrosshairs) {
			// crosshairs circle
			canvas.drawCircle(center.x, center.y, radius, paintThickBright);
			// crosshairs lines
			float radius2 = radius * 1.05f;
			float xOffset = (float) (radius2 * Math.sin(Math.PI/4));
			float yOffset = (float) (radius2 * Math.cos(Math.PI/4));
			canvas.drawLine(center.x-xOffset, center.y-yOffset, center.x+xOffset, center.y+yOffset,paintThickBright);
			canvas.drawLine(center.x+xOffset, center.y-yOffset, center.x-xOffset, center.y+yOffset,paintThickBright);
			
			// arrows
			float width = radius * 1 / 5, begin = radius * 3 / 5, end = radius * 4 / 5;
			// down
			canvas.drawLine(center.x - width, center.y + begin, center.x, center.y + end, paintThickBright);
			canvas.drawLine(center.x + width, center.y + begin, center.x, center.y + end, paintThickBright);
			// up
			canvas.drawLine(center.x - width, center.y - begin, center.x, center.y - end, paintThickBright);
			canvas.drawLine(center.x + width, center.y - begin, center.x, center.y - end, paintThickBright);
			// left
			canvas.drawLine(center.x - begin, center.y - width, center.x - end, center.y, paintThickBright);
			canvas.drawLine(center.x - begin, center.y + width, center.x - end, center.y, paintThickBright);
			// right
			canvas.drawLine(center.x + begin, center.y - width, center.x + end, center.y, paintThickBright);
			canvas.drawLine(center.x + begin, center.y + width, center.x + end, center.y, paintThickBright);


		}
		
		// draw timer
		
		String rawTime = Float.toString(time);
		String timeString;
		if(rawTime.contains(".")) {
			timeString = rawTime.substring(0,rawTime.indexOf(".")+2) + "s";	
		} else {
			timeString = rawTime + ".0";
		}
		canvas.drawText(timeString, center.x-radius, center.y-radius, paintText);
		
		if(showIcon) {
			// draw icon
			canvas.drawBitmap(icon, iconRectSrc, iconRectDst, paintText);	
		}
	}
	
	/**
	 * Sets the value of the displayed timer.
	 * @param time The time in seconds.
	 */
	public void setTime(float time) {
		this.time = time;
		postInvalidate();
	}

	@Override
	public boolean onTouchEvent(MotionEvent event) {

		if(!isInputActive || activeInput != Input.TOUCH) {
			return true;
		}
		
		int action = event.getActionMasked();
		
		int index = event.getActionIndex();
		int id = event.getPointerId(index);
		float x = event.getX(index), y = event.getY(index);
		
		
		switch (action) {
		case MotionEvent.ACTION_UP:
			setTouchPoint(center.x, center.y);
			vibrator.vibrate(25);
			notifyListenersForce();
			break;
		case MotionEvent.ACTION_POINTER_UP:
			if(id == pointerId) {
				for(int i=0; i < event.getPointerCount(); i++) {
					int newId = event.getPointerId(i);
					if(newId != pointerId) {
						pointerId = newId;
						setTouchPoint(event.getX(i), event.getY(i));
						break;
					}
				}
			}
			break;
		case MotionEvent.ACTION_DOWN:
			setTouchPoint(x,y);
			pointerId = id;
			vibrator.vibrate(25);
			break;
		case MotionEvent.ACTION_MOVE:
			if(id == pointerId) {
				setTouchPoint(x,y);
			}
			break;
		default:
			break;
		}
		
		invalidate();

		
		return true;
	}
	
	/**
	 * Changes the location of the registered touch point. The coordinates are used to
	 * calculate the magnitude and direction of touch. Listeners are notified.
	 * @param x
	 * @param y
	 */
	private void setTouchPoint(float x, float y) {
		float relx = center.x - x, rely = center.y - y;
		if(radius != 0) {
			magnitude = (float) Math.sqrt(Math.pow(relx,2) + Math.pow(rely,2)) / radius;
		} else {
			magnitude = 0.0f;
		}
		angle = (float) -Math.atan2(relx, rely);

		if(magnitude > 1.0f) {
			relx /= magnitude;
			rely /= magnitude;
			magnitude = 1.0f;
		}
		
		touchPoint.set(center.x - relx, center.y - rely);
		notifyListeners();
		
	}
	
	@Override
	public void surfaceChanged(SurfaceHolder holder, int format, int width, int height) {
		surfaceChangedHasBeenCalled=true;
		center.set(getWidth()/ (float) 2, getHeight()/ (float)2);
		radius = (float) Math.min(center.x, center.y) * 0.95f;
		circleRadius = radius / 3;
		setTouchPoint(center.x, center.y);
		// added to support a 2.3.3 device where surfaceChanged is called after onDraw
		invalidate();
	}

	@Override
	public void surfaceCreated(SurfaceHolder arg0) {
		
	}

	@Override
	public void surfaceDestroyed(SurfaceHolder arg0) {
		
	}
	
	
	// listeners
	
	/**
	 * Adds a listener that will receive touch events.
	 * @param listener
	 */
	public void addListener(TouchListener listener) {
		listeners.add(listener);
	}
	
	/**
	 * Removes a previously registered listener.
	 * @param listener
	 */
	public void removeListener(TouchListener listener) {
		listeners.remove(listener);
	}
	
	/**
	 * Sets the frequency at which touch events will be sent to the listeners.
	 * Regular touch events will not be sent more often than this, but an event will always be sent
	 * when input is disabled or the touch input is released.
	 * @param frequency in number of events / second
	 */
	public void setNotificationFrequency(float frequency) {
		notificationPeriod = (long) (1000f / frequency);
	}
	
	/**
	 * Notifies the listeners if the time since the last notification is longer than the notification period.
	 */
	private void notifyListeners() {
		long now = System.currentTimeMillis();
		if(now - lastNotification > notificationPeriod) {
			notifyListenersForce();
		}
		
	}
	
	/**
	 * Notifies the listeners.
	 */
	private void notifyListenersForce() {
		long now = System.currentTimeMillis();
		lastNotification = now;
		for(TouchListener listener: listeners) {
			listener.event(magnitude, angle);
		}
	}



	// sensors
	
	@Override
	public void onAccuracyChanged(Sensor sensor, int accuracy) {
		
	}
	
	@Override
	public void onSensorChanged(SensorEvent event) {
	    switch (event.sensor.getType()) {
	    case Sensor.TYPE_ACCELEROMETER:
	    	System.arraycopy(event.values, 0, mValuesAccel, 0, 3);
	    	break;
	    case Sensor.TYPE_MAGNETIC_FIELD:
	    	System.arraycopy(event.values, 0, mValuesMagnet, 0, 3);
	    	break;
	    		    	
	    }
	    
		
	}

}
