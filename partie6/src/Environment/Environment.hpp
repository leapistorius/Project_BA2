#ifndef ENVIRONMENT_H
#define ENVIRONMENT_H

#include <list>
#include <vector>
#include <iostream>
#include <SFML/Graphics.hpp>
#include <Utility/Vec2d.hpp>
#include <Utility/Utility.hpp>
#include <SFML/Graphics.hpp>
#include <Environment/FoodGenerator.hpp>
#include <Environment/OrganicEntity.hpp>
#include <Interface/Drawable.hpp>
#include <Interface/Updatable.hpp>
#include <unordered_map>

class Animal;
class Wave; 
class Application;
class NeuronalScorpion; 

class Environment : public Drawable, public Updatable {
	
	private:

		std::list<OrganicEntity*> faune;
		std::list<FoodGenerator*> food;
		std::list<Wave*> waves; 
		std::list<CircularCollider*> obstacles; 

	public: 
	
		void addEntity(OrganicEntity* nouveau);				//methode peuplant l'environnement d'organic entities
		void addWave (Wave* nouveau);
		void addObstacle (CircularCollider* nouveau);  
		void addGenerator(FoodGenerator* nouveau);
	
		void update(sf::Time dt);							//methode faisant évoluer la faune à chaque pas de temps
		void draw(sf::RenderTarget& targetWindow) const;	//methode pour dessinez les ressources et la faune
		void clean();										//methode pour vider l'environnement de tout animal/cible/obstacle

		std::vector<OrganicEntity*> getEntitiesInSightForAnimal(Animal* animal) const;		//retourne les cibles vues par un animal donne
		std::vector<CircularCollider*> getObstacles() const; 
		double getIntensityForSensor(NeuronalScorpion* scorpion ,size_t i); 
		std::unordered_map<std::string,double> fetchData(std::string title);

		Environment() = default; 	 						//constructeur par default
		Environment(Environment const& autre) = delete;		//suppression du destructeur de copie
		~Environment();
};

#endif
