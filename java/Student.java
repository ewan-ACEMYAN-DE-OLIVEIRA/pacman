package ExercicesTP2;

/**
 * Class Student
 */

public class Student extends Person {  // Save as "Student.java"
   private String program ;
   private int year ;
   private double fee ;
   
   public Student (String name, String address, String program, int year, double fee) {
	   super(name, address);
	   this.program = program ;
	   this.year = year ; 
	   this.fee =fee ; 
   }
   
   public String getProgram() {
	   return program ;
   }
   
   public void setProgram(String program) {
	   this.program = program ;
   }
   
   public int getYear() {
	   return year ;
   }
   
   public void setYear(int year) {
	   this.year = year ;
   }
   
   public double getFee() {
	   return fee ;
   }
   
   public void setFee(int year) {
	   this.fee = fee ;
   }
   
   public String toString() {
	   return "Student[Person[name=" + this.getName() + " address=" + this.getAddress() + " program=" + program + " year=" + year + "fee=" + fee + "]";
   }
}
