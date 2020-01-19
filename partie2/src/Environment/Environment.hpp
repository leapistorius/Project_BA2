#ifndef ENVIRONMENT_H
#define ENVIRONMENT_H

#include <iostream>
#include <list>
#include <vector>
#include <Utility/Vec2d.hpp>
#include <Utility/Utility.hpp>

class Animal;

class Environment {
	
	private:
	
		std::list<Animal*> faune;
		std::list<Vec2d> ressources;	

	public:

		void addAnimal(Animal* nouveau);
		void addTarget(const Vec2d& target);
		void update(sf::Time dt);
		void draw(sf::RenderTarget& targetWindow) const;
		void clean();
		
		std::vector<Vec2d> getTargetsInSightForAnimal(Animal* animal);
		
		//Constructeurs/destructeurs
		Environment() = default; 	 				
		~Environment(); 
		Environment(Environment const& autre) = delete;	//suppression du destructeur de copie
		
};

#endif
