#ifndef SENSOR_H
#define SENSOR_H

#include <iostream>
#include <Interface/Updatable.hpp>
#include <Utility/Vec2d.hpp>

typedef enum {ACTIF, PAS_ACTIF} State; 
class NeuronalScorpion; 

class Sensor: public Updatable 
{
	public: 
	
		Sensor (Vec2d position, State etat, NeuronalScorpion* scorpion, size_t i);
		Sensor (double x, double y, NeuronalScorpion* scorpion, size_t i);  
		~Sensor()= default; 
		
		void update(sf::Time dt) override; 
		void updateState(); 
		void updateInhibitor(); 
		void updateScore (); 
		
		Vec2d getLocationSensor() const; 
		double getScore() const;
		State getState() const;
		double getInhibitor() const;
		void setInhibitor(double inhibition);
		void reInitialisation(); 
	
	
	private:
	 
		Vec2d location; 
		State state; 
		double score; 
		double inhibitor;
		NeuronalScorpion* scorpionHote;		//pointeur sur scorpion auquel le senseur appartient
		const size_t index;
};

#endif


