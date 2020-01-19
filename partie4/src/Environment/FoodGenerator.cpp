#include "FoodGenerator.hpp"
#include <Random/Normal.hpp>
#include <Application.hpp>
#include <Utility/Vec2d.hpp>

FoodGenerator::FoodGenerator()
:time(sf::Time::Zero)
{}

void FoodGenerator::update(sf::Time dt)
{
    time += dt;
    if (time > sf::seconds(getAppConfig().food_generator_delta)) {
        time = sf::Time::Zero;
        getAppEnv().addEntity(addFood());
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
    return new Food (positionFood);
}
