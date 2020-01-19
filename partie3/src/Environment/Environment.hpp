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

class Animal;
class Application;


class Environment : public Drawable, public Updatable {
	
	private:

		std::list<OrganicEntity*> faune;
		std::list<FoodGenerator*> food;

	public:

		void addEntity(OrganicEntity* nouveau);							//methode peuplant l'environnement d'organic entities
		void addGenerator(FoodGenerator* nouveau);
		void update(sf::Time dt);
		void draw(sf::RenderTarget& targetWindow) const;
		void clean();													//methode pour vider l'environnement de tout animal/cible

		std::vector<OrganicEntity*> getEntitiesInSightForAnimal(Animal* animal);			//retourne les cibles vues par un animal donne

		Environment() = default; 	 									//constructeur par default
		Environment(Environment const& autre) = delete;					//suppression du destructeur de copie
		~Environment();
};

#endif
