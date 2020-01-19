#ifndef ANIMAL_H
#define ANIMAL_H

#include <iostream>
#include <Utility/Vec2d.hpp>
#include <Utility/Utility.hpp>
#include <Obstacle/CircularCollider.hpp>

enum Deceleration {fort , moyen, faible}; 

class Animal : public CircularCollider {
	
	public:

		//Getters
		double getStandardMaxSpeed() const; 
		double getMass() const; 
		double getViewRange()const;  
		double getViewDistance()const;  
		double getRotation()const;
		Vec2d getSpeedVector() const;
		double getRandomWalkRadius() const; 
		double getRandomWalkDistance() const; 
		double getRandomWalkJitter() const;
		
		//Setters 
		void setTargetPosition (const Vec2d& target); 
		void setDeceleration(int d_);
		
		//Methode update 
		void update(sf::Time dt);  
		
		//Methode draw
		void draw(sf::RenderTarget& targetWindow) const; 
		
		//constructeurs
		Animal(const Vec2d& position_, double r, double speed);			
		Animal(const Vec2d& position_); 
		
		//destructeur
		~Animal() = default; 
		 
		//Perception d'une cible 
		bool isTargetInSight(const Vec2d& cible) const; 
		
		//RandomWalk
		Vec2d randomWalk(); 
		Vec2d convertToGlobalCoord(const Vec2d& local) const; 
		
	private: 
		
		//Attributs
		Vec2d direction; 
		double normeVitesse; 
		Vec2d positionCible; 
		Vec2d currentTarget; 
		
		int d;
		
		//Methodes
		void drawSprite(sf::RenderTarget& targetWindow) const;
		void drawVision(sf::RenderTarget& targetWindow) const;
		void drawRandomWalk(sf::RenderTarget& targetWindow) const; 
		Vec2d forceAttraction (const Vec2d& cible) const;
		void miseAJourDeplacement(Vec2d const& force, sf::Time dt);
		Vec2d getMovedCurrentTarget() const; 
		
	protected: 

		void setRotation(double angle);
	
}; 


#endif


