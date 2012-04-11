package com.android.views;

import java.util.ArrayList;

import android.content.Context;
import android.graphics.Canvas;
import android.graphics.Paint;
import android.view.View;

public class DrawingCanvas extends View {
    public static class Line {
        public Line() {
            this(0, 0, 0, 0);
        }
        
        public Line(int startX, int startY, int stopX, int stopY) {
            this(startX, startY, stopX, stopY, new Paint());
        }
        
        public Line(int startX, int startY, int stopX, int stopY, Paint paint) {
            this.startX = startX;
            this.startY = startY;
            this.stopX = stopX;
            this.stopY = stopY;
            this.paint = paint;
        }
        
        public int startX;
        public int startY;
        public int stopX;
        public int stopY;
        public Paint paint;
    };
    
    private ArrayList<Line> lines = new ArrayList<Line>();
    
    public DrawingCanvas(Context context) {
        super(context);
    }
    
    public void addLine(Line line) {
        lines.add(line);
    }
    
    public void removeLines() {
        lines.clear();
    }

    @Override
    public void onDraw(Canvas canvas) {
        int width = canvas.getWidth();
        int height = canvas.getHeight();
        
        for (Line line : lines) {
            if (width > line.startX && height > line.startY && width > line.stopX && height > line.stopY) {
                canvas.drawLine(line.startX, line.startY, line.stopX, line.stopY, line.paint);
            }
        }
    }
}
