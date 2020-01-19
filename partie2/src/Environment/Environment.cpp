#include "Environment.hpp"
#include <SFML/Graphics.hpp>
#include <Animal/Animal.hpp>
#include <Application.hpp>

//ajoute animale
void Environment::addAnimal(Animal* nouveau) 
{	
	faune.push_back(nouveau);
}

//ajoute cible 
void Environment::addTarget(const Vec2d& target) 
{	
	ressources.push_back(target);
}

void Environment::update(sf::Time dt) 
{													
	for (auto& animal: faune){
		animal -> Animal::update(dt);
	} 
}

//draw animaux et ressources 	
void Environment::draw(sf::RenderTarget& targetWindow) const
{
	for (auto& animal: faune){
		animal -> Animal::draw(targetWindow);
	}
	
	for (auto cible: ressources) {
		targetWindow.draw(buildCircle(cible, 5.0, sf::Color(255, 0, 0)));																					
	}
}

//vide l'environnement
void Environment::clean() 
{	
	for (auto* animal: faune) {			
		delete animal;
		animal=nullptr; 
	}
	faune.clear();
	ressources.clear();

}

Environment::~Environment()
{
	clean(); 
}

//retourne cibles vues par un animal 
std::vector<Vec2d> Environment::getTargetsInSightForAnimal(Animal* animal) 
{	
	std::vector<Vec2d> ciblesVues;
	
	for(auto& cible: ressources) {
		if (animal -> isTargetInSight(cible)) {			
			ciblesVues.push_back(cible); 				
		}
	}
	return ciblesVues;
}
