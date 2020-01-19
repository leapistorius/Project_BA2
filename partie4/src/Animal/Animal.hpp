#ifndef ANIMAL_H
#define ANIMAL_H

#include <iostream>
#include <list>
#include <Utility/Vec2d.hpp>
#include <Utility/Utility.hpp>
#include <Environment/OrganicEntity.hpp>


class Animal : public OrganicEntity {
	
	public:
		enum class Deceleration {fort, moyen, faible}; 
		enum class Etat {FOOD_IN_SIGHT, FEEDING, RUNNING_AWAY, MATE_IN_SIGHT, MATING, GIVING_BIRTH, WANDERING};
		
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
		double getVitesse() const;
		OrganicEntity* getCible () const; 
		Vec2d getDirection () const; 
		
		//Setter
		void setState(Animal::Etat etat);
		void setBabies (size_t babies);
		void setMate(OrganicEntity* partenaire); 
		void setVitesse (double vitesse); 
		void setDirection (Vec2d d); 
		void setCible (OrganicEntity* entity); 
		
		//Methode draw
		void draw(sf::RenderTarget& targetWindow) const override; 
 
		//Methodes update 
		void update(sf::Time dt) override; 
		void updateEnergie (sf::Time dt);	
		
		//Perception d'une cible 			 
		bool isTargetInSight(const Vec2d& cible) const; 	
		
		//RandomWalk
		Vec2d randomWalk(); 
		Vec2d convertToGlobalCoord(const Vec2d& local) const; 

		
	protected: 
	
		void setRotation(double angle);
		sf::Time tempsPause;
		sf::Time tempsEnceinte; 
		void miseAJourDeplacement(Vec2d const& force, sf::Time dt);		//fixe nouvelles direction, normeVitesse et position 
	
	protected:
	
	void drawSprite(sf::RenderTarget& targetWindow) const;
	
	private: 
		
		//Attributs 
		Vec2d direction; 
		double normeVitesse; 
		bool femelle; 				//retourne vraie si femelle
		Etat state; 
		size_t nbBabies;
		
		//entites en vue
		Vec2d currentTarget;		//position de la cible virtuelle pour randomwalk
		OrganicEntity* cible;
		OrganicEntity* mate; 
		std::list<OrganicEntity*> enemies; 
					
		//Methodes draw
		void drawVision(sf::RenderTarget& targetWindow) const;			//affiche le champ de vision de l'animal
		void drawRandomWalk(sf::RenderTarget& targetWindow) const;		//affichage de la cible virtuelle si en mode randomwalk()
		virtual void drawDebug(sf::RenderTarget& targetWindow) const; 			//Affichages en mode debbuging 
		
		//Autres methodes
		virtual Vec2d forceAttraction (const Vec2d& cible) const;				//calcul force d'attraction exercee par cible reelle/virtuelle
		Vec2d forceFuite () const;										//calcul force fuite exercee par predateur sur animal
		Vec2d getMovedCurrentTarget() const;
		Etat updateState(sf::Time dt);							//methode update modifiant le state de l'animal
		virtual void analyzeEnvironnement(); 									//cherche nourriture, partenaire et ennemies
		void eating(double& energie) override;
		virtual void givingBirth () const = 0;
		double distanceEnemy();
		void cleanEnemies();  
	
}; 


#endif


