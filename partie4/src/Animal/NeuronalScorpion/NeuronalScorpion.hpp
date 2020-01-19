#ifndef NEURONAL_SCORPION_H
#define NEURONAL_SCORPION_H

#include <iostream> 
#include <array>
#include <Animal/NeuronalScorpion/Sensor.hpp> 
#include <Utility/Vec2d.hpp>
#include <Animal/Scorpion.hpp>

class NeuronalScorpion: public Scorpion 
{
	public: 
	
		enum class Etat {IDLE, WANDERING, TARGET_IN_SIGHT, MOVING};
	
		NeuronalScorpion(const Vec2d& positionInitiale, double energie, bool sexe);
		NeuronalScorpion(const Vec2d& positionInitiale);
		~NeuronalScorpion() = default; 
		
		Vec2d getPositionOfSensor(size_t i) const;
		std::array<std::pair<double,Sensor*>, 8> getSensors() const;
		void setState(NeuronalScorpion::Etat etat);
		
		bool verifyCompteur();	//verifie si un compteur a depasse le temps limite d'activite
		void reinitialisation();
		
		void update(sf::Time dt) override; 
		void draw(sf::RenderTarget& targetWindow) const override; 
		void drawDebug (sf::RenderTarget& targetWindow) const override; 
	
	protected: 
		
		NeuronalScorpion::Etat updateState(sf::Time dt);
		void updateDirection();				
		void updateHorloge(sf::Time dt);
		void updateCompteur(sf::Time dt);
	
	private: 
	
		void calculForce(sf::Time dt);
		void initialiseSensors();
		void analyzeEnvironnement() override;  
		Vec2d forceAttraction (const Vec2d& target) const override; 
		
		std::array<std::pair<double,Sensor*>, 8> sensors;
		Vec2d directionCible; 
		Etat state;
		sf::Time horloge;
		sf::Time compteurTemps; 	//pour eviter de depasser temps limite d'activation des senseurs 
};

#endif

