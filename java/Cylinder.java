package tp4exosYaip5;

public class CylinderC {
   private Circle base;   // Base circle, an instance of Circle class
   private double height;
   
   // Constructor with default color, radius and height
   public CylinderC() {
      base = new Circle(); // Call the constructor to construct the Circle
      height = 1.0; 
   }
   public CylinderC(double height) {
      base=new Circle();        // call superclass no-arg constructor Circle()
      this.height = height;
   }
   // Constructor with default color, but given radius, height
   public CylinderC(double radius, double height) {
      base = new Circle(radius);  // call superclass constructor Circle(r)
      this.height = height;
   }
   
   // A public method for retrieving the height
   public double getHeight() {
      return height; 
   }
  
   // A public method for computing the volume of cylinder
   //  use superclass method getArea() to get the base area
   public double getVolume() {
      return this.base.getArea()*height; 
   }
   @Override
   public String toString() {      // in Cylinder class
      return "Cylinder: subclass of " + this.base.toString()  // use Circle's toString()
             + " height=" + height;
	   }
}
