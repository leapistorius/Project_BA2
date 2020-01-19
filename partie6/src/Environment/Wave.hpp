#ifndef WAVE_H
#define WAVE_H

#include <iostream>
#include <vector>
#include <Utility/Vec2d.hpp>
#include <Obstacle/CircularCollider.hpp>
#include <Interface/Updatable.hpp>

typedef std::pair<double,double> Arcs; 

class Wave : public CircularCollider, public Updatable {
	
	public:
	
		Wave (Vec2d depart, double energie, double rayon, double constante, double vitesse); 
		~Wave() = default; 
		
		double getIntensity () const; 
		std::vector <Arcs*> getEnsembleArcs() const;
		void draw(sf::RenderTarget& targetWindow) const;
		
		//update 
		void update (sf::Time dt); 
		void updateTime (sf::Time dt); 
		void updateRayon ();  
		void updateEnergie (); 
		void updateIntensity (); 
		void updateArcs(); 		//divise onde en morceaux si collision avec obstacle
	
	private: 
	
		sf::Time temps; 
		double niveauEnergieInitial; 
		double rayonInitial; 
		double constOnde; 
		double vitessePropagation; 
		double energie; 
		double intensity; 
		std::vector <Arcs*> ensembleArcs; 
	
}; 


#endif
