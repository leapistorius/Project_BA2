#ifndef ANIMAL_H
#define ANIMAL_H

#include <iostream>
#include <list>
#include <Utility/Vec2d.hpp>
#include <Utility/Utility.hpp>
#include <Environment/OrganicEntity.hpp>

enum Etat {FOOD_IN_SIGHT, FEEDING, RUNNING_AWAY, MATE_IN_SIGHT, MATING, GIVING_BIRTH, WANDERING};
enum Deceleration {fort, moyen, faible}; 

class Animal : public OrganicEntity {
	
	public:

		//constructeur/destructeur
		Animal(const Vec2d& position_, double taille, double energie, bool sexe);
		virtual ~Animal() = default; 

		//Getters
		virtual double getStandardMaxSpeed() const = 0; 
		virtual double getMass() const = 0; 
		virtual double getViewRange()const = 0;  
		virtual double getViewDistance()const = 0;  
		double getRotation()const;
		Vec2d getSpeedVector() const;
		bool getSexe() const;
		virtual sf::Texture& getTexture() const override = 0;
		virtual double getSize() const override  = 0;
		double getMaxSpeed() const; 
		Etat getState() const;
		virtual double getRandomWalkRadius() const = 0; 
		virtual double getRandomWalkDistance() const = 0; 
		virtual double getRandomWalkJitter() const = 0;
		virtual double getGestationTime () const = 0; 
		sf::Time getTempsEnceinte() const override;
		size_t getBabies () const; 
		double getEnergieConsumption() const override; 
		double getVitesse () const override; 
		
		//Setter
		void setState(Etat etat);
		void setBabies (size_t babies);
		void setMate(OrganicEntity* partenaire); 
		
		//Methodes draw
		void draw(sf::RenderTarget& targetWindow) const override; 
 
		//Methodes update 
		void update(sf::Time dt) override; 
		
		//Perception d'une cible 			 
		bool isTargetInSight(const Vec2d& cible) const; 	
		
		//RandomWalk
		Vec2d randomWalk();
		Vec2d convertToGlobalCoord(const Vec2d& local) const; 

		void eating (double& energie) override; 
		
		
	protected: 
	
		void setRotation(double angle);
		sf::Time tempsPause;
		sf::Time tempsEnceinte;  
	
	private: 
		
		//Attributs 
		Vec2d direction; 
		double normeVitesse; 
		bool femelle; 				//retourne vraie si femelle
		Etat state; 
		size_t nbBabies;
		
		//entites en vue
		Vec2d currentTarget;											//position de la cible virtuelle pour randomwalk
		OrganicEntity* cible;
		OrganicEntity* mate; 
		std::list<OrganicEntity*> enemies; 
					
		//Methodes
		void drawSprite(sf::RenderTarget& targetWindow) const;
		void drawVision(sf::RenderTarget& targetWindow) const;			//affiche le champ de vision de l'animal
		void drawRandomWalk(sf::RenderTarget& targetWindow) const;		//affichage de la cible virtuelle si en mode randomwalk()
		void drawDebug(sf::RenderTarget& targetWindow) const; 			//affichage en mode debugging 
		
		Vec2d forceAttraction (const Vec2d& cible) const;				//calcul force d'attraction exercee par cible reelle/virtuelle
		Vec2d forceFuite () const;										//calcul force fuite exercee par predateur sur animal
		void miseAJourDeplacement(Vec2d const& force, sf::Time dt);		//fixe nouvelles direction, normeVitesse et position 
		Vec2d getMovedCurrentTarget() const;
		Etat updateState(sf::Time dt);									//methode update modifiant le state de l'animal
		void analyzeEnvironnement(); 									//cherche nourriture, partenaire et ennemies
		virtual void givingBirth () const = 0;
		double distanceEnemy();
		void cleanEnemies();  
	
}; 


#endif


