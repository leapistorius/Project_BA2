#include "Environment.hpp"
#include <Animal/Animal.hpp>
#include <algorithm>


void Environment::addEntity(OrganicEntity* nouveau)
{
    faune.push_back(nouveau);
}

void Environment::addGenerator(FoodGenerator* nouveau)
{
    food.push_back(nouveau);
}

void Environment::update(sf::Time dt)
{
    for (auto& entity: faune) {
            entity -> update(dt);		//mise a jour de la position et donc du dessin
    }
    
    for (auto& entity: faune) {
		if (entity != nullptr) {
			if (entity->isDead()) {		//controle si entite est morte
                delete entity;
                entity = nullptr;
            }
		}
	}
	
    faune.erase(std::remove(faune.begin(), faune.end(), nullptr), faune.end());		//efface tous les nullptr (donc efface entites mortes)

    for (auto& entity: food) {
        if (entity != nullptr) {
            entity -> update(dt);
        }
    }
}

void Environment::draw(sf::RenderTarget& targetWindow) const
{
    for (auto& entity: faune) {
        if (entity != nullptr) {
            entity -> draw(targetWindow);
        }
    }
}

void Environment::clean()
{
    for (auto& entity: faune) {
        delete entity;
        entity=nullptr;
    }

    faune.clear();
}

Environment::~Environment()
{
    clean();
}


std::vector<OrganicEntity*> Environment::getEntitiesInSightForAnimal(Animal* animal)
{
    std::vector<OrganicEntity*> seenEntities;

    if(not(faune.empty())) {
        for(auto& entity: faune) {
            if ((entity != nullptr) and (animal -> isTargetInSight(entity -> getPosition()))) {
                seenEntities.push_back(entity);
            }
        }
    }
    return seenEntities;
}


