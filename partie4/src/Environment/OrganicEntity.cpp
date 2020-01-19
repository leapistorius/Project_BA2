#include "OrganicEntity.hpp"
#include <Application.hpp>

OrganicEntity::OrganicEntity(const Vec2d& position_, double taille, double energie)
    : CircularCollider(position_, taille/2), niveauEnergie(energie), tempsEnVie(sf::Time::Zero)
{}

//getters
sf::Time OrganicEntity::getLongetivity() const
{
    return sf::seconds(10);		//longueur de vie par defaut modifiable si souhaitable (pour l'instant valeur "infinie")
}

double OrganicEntity::getEnergie() const
{
    return niveauEnergie;
}

//setter
void OrganicEntity::setEnergie(double energie)
{
    niveauEnergie = energie;
}

void OrganicEntity::update(sf::Time dt)
{
	updateTime(dt);
	updateEnergie(dt); 
}

void OrganicEntity::updateTime(sf::Time dt)
{
	tempsEnVie += dt;
}

void OrganicEntity::updateEnergie (sf::Time dt)
{	
	double energie (this->getEnergie());
	double perteEnergie (this->getEnergieConsumption() + this->getVitesse() * this->getPerteEnergie() * dt.asSeconds()); 
	energie-=perteEnergie;
	
	this -> eating(energie);
	
	setEnergie(energie);
}

bool OrganicEntity::isDead()
{
    return ((niveauEnergie<getAppConfig().animal_min_energy) or (tempsEnVie > this->getLongetivity()));
}


