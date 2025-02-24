package exoengins;

/**
 * Class Electrique
 */

public class Electrique extends Propulsion { // Save as "Electrique.java"
	static double baseConso = 3.14 ;
	static double perte = 62.0 ;
	double masse ;
	
	public Electrique(double masse) {
		this.masse = masse ;
	}
	
	public double consommerEnergie() {
		return baseConso + masse * perte ;
	}
	
	public double getMasse() {
		return masse ;
	}
	
	public void setMasse(double newMasse) {
		this.masse = newMasse ;
	}
	
	public static double getBaseConso() {
		return baseConso ;
	}

	public static void setBaseConso(double newBaseConso) {
		baseConso = newBaseConso ;
	} 
	
	public static void setPerte(double newPerte) {
		perte = newPerte ;
		
	}
	
	public static double getPerte() {
		return perte ;
	}
	
	public String toString() {
		return "Electrique[masse=" + masse + "]";
	}
	
	
}
