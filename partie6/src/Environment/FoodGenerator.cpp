#include "FoodGenerator.hpp"
#include <Random/Normal.hpp>
#include <Application.hpp>
#include <Utility/Vec2d.hpp>
#include <Environment/Mushroom.hpp> 

FoodGenerator::FoodGenerator()
:timeFood(sf::Time::Zero), timeObjet(sf::Time::Zero), compteurFood (0)
{}

void FoodGenerator::update(sf::Time dt)
{
    timeFood += dt;
    if (timeFood > sf::seconds(getAppConfig().food_generator_delta)) {
        timeFood = sf::Time::Zero;
        getAppEnv().addEntity(addFood());
        compteurFood += 1; 
    } 
    timeObjet += dt; 
    if (timeObjet > sf::seconds(getAppConfig().object_generator_delta)){
		timeObjet = sf::Time::Zero; 
		getAppEnv().addEntity(addBanana()); 
		
	}
}

double FoodGenerator::getEnvironmentSize() const
{
    return getAppConfig().simulation_world_size;
}

Food* FoodGenerator::addFood() const
{
    double valeurAleatoire1 (normal(getEnvironmentSize()/2 , getEnvironmentSize()/4*getEnvironmentSize()/4));
    double valeurAleatoire2 (normal(getEnvironmentSize()/2 , getEnvironmentSize()/4*getEnvironmentSize()/4));
    Vec2d positionFood (valeurAleatoire1, valeurAleatoire2);
    if (compteurFood%2 == 0){
		return new Food (positionFood);
	} else {
		return new Mushroom (positionFood); 
	}
}

Banana* FoodGenerator::addBanana() const 
{
    double valeurAleatoire1 (normal(getEnvironmentSize()/2 , getEnvironmentSize()/4*getEnvironmentSize()/4));
    double valeurAleatoire2 (normal(getEnvironmentSize()/2 , getEnvironmentSize()/4*getEnvironmentSize()/4));
    Vec2d positionBanana (valeurAleatoire1, valeurAleatoire2);	
    return new Banana (positionBanana); 
}
