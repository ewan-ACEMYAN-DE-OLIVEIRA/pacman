package ExerciceTP2;
/*
 * Classe MyPoint
 */
public class MyPoint {
	private int x;
	private int y;

	public MyPoint() {
		this.x=0;
		this.y=0;
	}

	public MyPoint(int x, int y) {
		this.x=x;
		this.y=y;
	}
	
	public int getX() {
		return x;
	}
	
	public int getY() {
		return y;
	}
	
	public void setX(int newX) {
		this.x=newX;
	}
	
	public void setY(int newY) {
		this.y=newY;
	}
	
	public int[] getXY() {
		return new int[] {x,y};
	}
	
	public void setXY(int x, int y) {
		this.x=x;
		this.y=y;
	}
	
	public double distance(int x, int y) {
		int dx=this.x-x;
		int dy=this.y-y;
		return Math.sqrt(dx*dx+dy*dy);
	}
	
	public double distance(MyPoint another) {
		return distance (another.x,another.y);
	}
	
	public String toString() {
		return "("+x+","+y+")";
	}

}
