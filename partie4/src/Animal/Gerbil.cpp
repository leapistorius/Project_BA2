#include "Gerbil.hpp"
#include <Application.hpp>
#include <Random/Uniform.hpp>

//constructeurs
Gerbil::Gerbil(const Vec2d& positionInitiale, double energie, bool sexe)
    : Animal(positionInitiale, getSize(), energie, sexe)
{}

Gerbil::Gerbil(const Vec2d& positionInitiale)
    :Gerbil(positionInitiale, getEnergieInitial(), uniform<int>(0, 1) == 0)
{}

//getters
double Gerbil::getStandardMaxSpeed() const
{
    return getAppConfig().gerbil_max_speed;
}

double Gerbil::getMass() const
{
    return getAppConfig().gerbil_mass;
}

double Gerbil::getViewRange()const
{
    return getAppConfig().gerbil_view_range;
}

double Gerbil::getViewDistance()const
{
    return getAppConfig().gerbil_view_distance;
}

double Gerbil::getRandomWalkRadius() const
{
    return getAppConfig().gerbil_random_walk_radius;
}

double Gerbil::getRandomWalkDistance() const
{
    return getAppConfig().gerbil_random_walk_distance;
}

double Gerbil::getRandomWalkJitter() const
{
    return getAppConfig().gerbil_random_walk_jitter;
}

double Gerbil::getEnergieInitial() const
{
    return getAppConfig().gerbil_energy_initial;
}

double Gerbil::getSize() const
{
    return getAppConfig().gerbil_size;
}

sf::Texture& Gerbil::getTexture() const
{
    if (this->getSexe()) {
        return getAppTexture(getAppConfig().gerbil_texture_female);
    } else {
        return getAppTexture(getAppConfig().gerbil_texture_male);
    }
}

sf::Time Gerbil::getLongetivity() const
{
    return getAppConfig().gerbil_longevity;
}

double Gerbil::getEnergyLossMatingFemale() const 
{
	return getAppConfig().gerbil_energy_loss_female_per_child; 
}

double Gerbil::getEnergyLossMatingMale() const
{
	return getAppConfig().gerbil_energy_loss_mating_male; 
}

size_t Gerbil::getMinBabies() const
{
	return getAppConfig().gerbil_min_children; 
} 

size_t Gerbil::getMaxBabies() const
{
	return getAppConfig().gerbil_max_children; 
}

double Gerbil::getGestationTime() const
{
	return getAppConfig().gerbil_gestation_time; 
}

//autres methodes
void Gerbil::givingBirth() const
{
	Vec2d position(this->getPosition());
	Gerbil* baby = new Gerbil(position);
	getAppEnv().addEntity(baby);
}

void Gerbil::meet() 
{	
	setState(Etat::MATING);
	tempsPause = sf::seconds(getAppConfig().animal_mating_time);	
	
	//femelles
	if (getSexe()==true){ 		
		setBabies (uniform(getMinBabies(), getMaxBabies())); 
		setEnergie(getEnergie() - getEnergyLossMatingFemale()*getBabies());
		tempsEnceinte = sf::seconds(getGestationTime());
		setMate(nullptr);
	}
		
	//males 
	else {
		setEnergie(getEnergieInitial() - getEnergyLossMatingMale());
		setMate(nullptr);
	}
}

//methodes eatable
bool Gerbil::eatable(OrganicEntity const* entity) const
{
    return entity->eatableBy(this);
}

bool Gerbil::eatableBy(Gerbil const* gerbil) const
{
    return false;
}

bool Gerbil::eatableBy(Food const* food) const
{
    return false;
}

bool Gerbil::eatableBy(Scorpion const* scorpion) const
{
    return true;
}

double Gerbil::getPerteEnergie() const
{
    return getAppConfig().gerbil_energy_loss_factor;

}

//methodes matable
bool Gerbil::matable(OrganicEntity const* other) const
{
	return other->canMate(this);
}

bool Gerbil::canMate(Scorpion const* scorpion) const
{
	return false;
}

bool Gerbil::canMate(Gerbil const* gerbil) const
{
	bool conditionSexe(gerbil -> getSexe());
	bool conditionTempsVie(this->tempsEnVie.asSeconds() >= getAppConfig().gerbil_min_age_mating); //this suffisamment age
	bool conditionEnergyF(this->getEnergie() >= getAppConfig().gerbil_energy_min_mating_female); //this a assez d'energie pour se reproduire
	bool conditionEnergyM(this->getEnergie() >= getAppConfig().gerbil_energy_min_mating_male);
	
	if (gerbil->getState()!= Etat::MATING) {	//threesomes...
		
		//si this est femelle
		if 	(this->getSexe()) {
			return ((not(conditionSexe)) and (conditionEnergyF) and (conditionTempsVie));
		}
		
		//si this est male
		else {
			return ((conditionSexe) and (gerbil->getState() != Etat::GIVING_BIRTH) and (conditionEnergyM) and (conditionTempsVie));	
		}
		
	} return false;
}

bool Gerbil::canMate(Food const* food) const
{
	return false;
}

 
