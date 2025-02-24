package exoengins;

/**
 * Class Thermique
 */

public class Thermique extends Propulsion { // Save as "Thermique.java"
	static double perte = 64 ;
	double masse ;
	int carburant ;

	public Thermique(double masse, int carburant) {
		this.masse = masse ;
		this.carburant = carburant ;
	}
	
	public double consommerEnergie() {
		return (perte + masse * Carburant.getConso(carburant));
	}
	
	public double getMasse() {
		return masse ;
	}
	
	public void setMasse(double newMasse) {
		this.masse = newMasse ;
	}
	
	public static double getPerte() {
		return perte ;
	}

	public static void setPerte(double newPerte) {
		perte = newPerte ;
	} 
	
	public int getCarburant() {
		return carburant ;
	}
	
	public void setCarburant(int carburant) {
		this.carburant = carburant ;
	}
	
	public String toString() {
		return "Thermique [masse=" + masse + ", carburant=" + Carburant.nom(carburant) + "]";
	}
	
	
}
