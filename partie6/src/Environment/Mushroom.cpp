#include "Mushroom.hpp"
#include <Application.hpp>
#include <Environment/OrganicEntity.hpp>

Mushroom::Mushroom(const Vec2d& position_)
: Food (position_, getSize(), getEnergieInitial())
{}

sf::Texture& Mushroom::getTexture() const
{
	return getAppTexture(getAppConfig().mushroom_texture); 
}

double Mushroom::getEnergieInitial() const
{
    return getAppConfig().mushroom_energy; //une energie positive pour que reste sur l'environment
}

double Mushroom::getSize() const
{
    return getAppConfig().mushroom_size;
}

double Mushroom::getEnergie() const //pour enlever energie negative lorsqu'elle est mange quand Animal::eating()
{
	return getAppConfig().mushroom_lossEnergy; 
}
