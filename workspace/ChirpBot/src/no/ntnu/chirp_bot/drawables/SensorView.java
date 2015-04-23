package no.ntnu.chirp_bot.drawables;

import android.content.Context;
import android.graphics.Canvas;
import android.graphics.Color;
import android.graphics.Paint;
import android.util.AttributeSet;
import android.view.View;

public class SensorView extends View {

	private Paint gridPaint;
	private double cDia;
	private double cRad;
	private double height;
	private double width;
	private int[] lineDist = {  150, 150, 150, 150, 150, 150, 150, 150};
	private Paint linePaint;
	private int i;

	public SensorView(Context context) 
	{
		super(context);
		init();
	}
	
	public SensorView(Context context, AttributeSet attr) 
	{
		super(context, attr);
		init();
	}
	
	public void init()
	{
		gridPaint = new Paint(Paint.ANTI_ALIAS_FLAG);
        gridPaint.setStyle(Paint.Style.STROKE);
        gridPaint.setColor(Color.GRAY);
        
        linePaint = new Paint(Paint.ANTI_ALIAS_FLAG);
        linePaint.setStyle(Paint.Style.STROKE);
        linePaint.setColor(Color.GRAY);
	}

	@Override
	protected void onLayout(boolean changed, int l, int t, int r, int b) 
	{
		//
	}
	
	public void updateLines(double[] newLineDist) 
	{
		for (i = 0; i < newLineDist.length; i++) 
		{
			lineDist[i] = (int) (newLineDist[i]*-1*cRad);
		}
		this.invalidate();
	}

	@Override
    protected void onSizeChanged(int w, int h, int oldw, int oldh) 
	{
        super.onSizeChanged(w, h, oldw, oldh);
        
        cDia = Math.min(w, h);
        cRad = cDia/2;
        
        height = h;
        width = w;
        
        for (int i = 0; i < lineDist.length; i++)
        {
			lineDist[i] = -1;
		}
	}
	
	@Override
	protected void onDraw(Canvas canvas) 
	{
		super.onDraw(canvas);

		
		/*
		 * line((int)width/2, (int)height/2, (int)width/2, (int)((int)(height/2)-(int)(cRad)-lineDist[0]));

  line((int)width/2, (int)height/2, (int)(width/2+ (cRad+lineDist[1])/Math.sqrt(2)), (int)(height/2- (cRad+lineDist[1])/Math.sqrt(2)));

  line((int)width/2, (int)height/2, (int)(width/2)+(int)(cDia/2)+lineDist[2], (int)height/2);

  line((int)width/2, (int)height/2, (int)(width/2+ (cRad+lineDist[3])/Math.sqrt(2)), (int)(height/2+ (cRad+lineDist[3])/Math.sqrt(2)));

  line((int)width/2, (int)height/2, (int)width/2, (int)(height/2)+(int)(cDia/2)+lineDist[4]);

  line((int)width/2, (int)height/2, (int)(width/2- (cRad+lineDist[5])/Math.sqrt(2)), (int)(height/2+ (cRad+lineDist[5])/Math.sqrt(2)));

  line((int)width/2, (int)height/2, (int)(width/2)-(int)(cDia/2)-lineDist[6], (int)height/2);

  line((int)width/2, (int)height/2, (int)(width/2- (cRad+lineDist[7])/Math.sqrt(2)), (int)(height/2- (cRad+lineDist[7])/Math.sqrt(2)));

		 */
		canvas.drawCircle((float)width/2, (float)height/2, (float)cRad, gridPaint);
		
		/*
		//UP
		canvas.drawLine((int)width/2, (int)height/2, (int)width/2, (int)((height/2)-(cRad-lineDist[0])), gridPaint);
		//UP RIGHT
		canvas.drawLine((int)width/2, (int)height/2, (int)(width/2+ (cRad+lineDist[1])/Math.sqrt(2)), (int)(height/2- (cRad+lineDist[1])/Math.sqrt(2)), gridPaint);
		//RIGHT
		canvas.drawLine((int)width/2, (int)height/2, (int)(width/2)+(int)(cDia/2)+lineDist[2], (int)height/2, gridPaint);
		//DOWN RIGHT
		canvas.drawLine((int)width/2, (int)height/2, (int)(width/2+ (cRad+lineDist[3])/Math.sqrt(2)), (int)(height/2+ (cRad+lineDist[3])/Math.sqrt(2)), gridPaint);
		//DOWN
		canvas.drawLine((int)width/2, (int)height/2, (int)width/2, (int)(height/2)+(int)(cDia/2)+lineDist[4], gridPaint);
		//DOWN LEFT
		canvas.drawLine((int)width/2, (int)height/2, (int)(width/2- (cRad+lineDist[5])/Math.sqrt(2)), (int)(height/2+ (cRad+lineDist[5])/Math.sqrt(2)), gridPaint);
		//LEFT
		canvas.drawLine((int)width/2, (int)height/2, (int)(width/2)-(int)(cDia/2)-lineDist[6], (int)height/2, gridPaint);
		//LEFT UP
		canvas.drawLine((int)width/2, (int)height/2, (int)(width/2- (cRad+lineDist[7])/Math.sqrt(2)), (int)(height/2- (cRad+lineDist[7])/Math.sqrt(2)), gridPaint);
		*/
		
		canvas.drawLine((int)width/2, (int)((height/2)-(cRad+lineDist[0])), (int)(width/2+ (cRad+lineDist[1])/Math.sqrt(2)), (int)(height/2- (cRad+lineDist[1])/Math.sqrt(2)), gridPaint);
		canvas.drawLine((int)(width/2)+(int)(cDia/2)+lineDist[2], (int)height/2,  (int)(width/2+ (cRad+lineDist[3])/Math.sqrt(2)), (int)(height/2+ (cRad+lineDist[3])/Math.sqrt(2)), gridPaint);
		canvas.drawLine((int)width/2, (int)(height/2)+(int)(cDia/2)+lineDist[4], (int)(width/2- (cRad+lineDist[5])/Math.sqrt(2)), (int)(height/2+ (cRad+lineDist[5])/Math.sqrt(2)), gridPaint);
		canvas.drawLine((int)(width/2)-(int)(cDia/2)-lineDist[6], (int)height/2,  (int)(width/2- (cRad+lineDist[7])/Math.sqrt(2)), (int)(height/2- (cRad+lineDist[7])/Math.sqrt(2)), gridPaint);
		
		canvas.drawLine((int)(width/2+ (cRad+lineDist[1])/Math.sqrt(2)), (int)(height/2- (cRad+lineDist[1])/Math.sqrt(2)), (int)(width/2)+(int)(cDia/2)+lineDist[2], (int)height/2, gridPaint);
		canvas.drawLine((int)(width/2+ (cRad+lineDist[3])/Math.sqrt(2)), (int)(height/2+ (cRad+lineDist[3])/Math.sqrt(2)), (int)width/2, (int)(height/2)+(int)(cDia/2)+lineDist[4], gridPaint);
		canvas.drawLine((int)(width/2- (cRad+lineDist[5])/Math.sqrt(2)), (int)(height/2+ (cRad+lineDist[5])/Math.sqrt(2)), (int)(width/2)-(int)(cDia/2)-lineDist[6], (int)height/2, gridPaint);
		canvas.drawLine((int)(width/2- (cRad+lineDist[7])/Math.sqrt(2)), (int)(height/2- (cRad+lineDist[7])/Math.sqrt(2)), (int)width/2, (int)((height/2)-(cRad+lineDist[0])),  gridPaint);
	}

	
	
	
}
