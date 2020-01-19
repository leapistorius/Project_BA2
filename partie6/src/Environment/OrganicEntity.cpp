#include "OrganicEntity.hpp"
#include <Application.hpp>

//CONSTRUCTEUR
OrganicEntity::OrganicEntity(const Vec2d& position_, double taille, double energie)
    : CircularCollider(position_, taille/2), tempsEnVie(sf::Time::Zero), niveauEnergie(energie)
{}

//GETTERS
sf::Time OrganicEntity::getLongetivity() const
{
    return sf::seconds(10);		//longueur de vie par defaut modifiable si souhaitable (pour l'instant valeur "infinie")
}

double OrganicEntity::getEnergie() const
{
    return niveauEnergie;
}

//SETTER
void OrganicEntity::setEnergie(double energie)
{
    niveauEnergie = energie;
}

//UPDATE
void OrganicEntity::update(sf::Time dt)
{
	updateTime(dt);
	updateEnergie(dt); 
}

void OrganicEntity::updateTime(sf::Time dt)
{
	tempsEnVie += dt;
}

void OrganicEntity::updateEnergie(sf::Time dt)
{	
	double energie (this->getEnergie());	//initialise a l'energie de l'entite
	double perteEnergie (this->getEnergieConsumption() + this->getVitesse() * this->getPerteEnergie() * dt.asSeconds()); 
	
	energie-=perteEnergie; //diminue l'energie de l'entite a chaque pas de temps
	
	this -> eating(energie);	//augmente l'energie du predateur si en FEEDING
	setEnergie(energie);
}

//SI MORT
bool OrganicEntity::isDead()
{
    return ((niveauEnergie<getAppConfig().animal_min_energy) or (tempsEnVie > this->getLongetivity()));
}


