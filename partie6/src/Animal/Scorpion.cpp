#include "Scorpion.hpp"
#include <Application.hpp>
#include <Random/Uniform.hpp>

size_t Scorpion::compteurScorpion(0);

//CONSTRUCTEURS
Scorpion::Scorpion(const Vec2d& positionInitiale, double energie, bool sexe)
    : Animal(positionInitiale, getSize(), energie, sexe)
{
	compteurScorpion += 1; 
}

Scorpion::Scorpion(const Vec2d& positionInitiale)
    :Scorpion(positionInitiale, getEnergieInitial(), uniform(0, 1)==0)
{}

Scorpion::~Scorpion()
{
	compteurScorpion -= 1;
}

//GETTERS
double Scorpion::getStandardMaxSpeed() const
{
    return getAppConfig().scorpion_max_speed;
}

double Scorpion::getMass() const
{
    return getAppConfig().scorpion_mass;
}

double Scorpion::getViewRange()const
{
    return getAppConfig().scorpion_view_range;
}

double Scorpion::getViewDistance()const
{
    return getAppConfig().scorpion_view_distance;
}

double Scorpion::getRandomWalkRadius() const
{
    return getAppConfig().scorpion_random_walk_radius;
}

double Scorpion::getRandomWalkDistance() const
{
    return getAppConfig().scorpion_random_walk_distance;
}

double Scorpion::getRandomWalkJitter() const
{
    return getAppConfig().scorpion_random_walk_jitter;
}

double Scorpion::getEnergieInitial() const
{
    return getAppConfig().scorpion_energy_initial;
}

double Scorpion::getSize() const
{
    return getAppConfig().scorpion_size;
}

sf::Texture& Scorpion::getTexture() const
{
    if (getCompteurNaissance()>sf::Time::Zero) {
		return getAppTexture(getAppConfig().fireball_texture);
	}
	else {
		return getAppTexture(getAppConfig().scorpion_texture);
	}
}

sf::Time Scorpion::getLongetivity() const
{
    return getAppConfig().scorpion_longevity;
}

double Scorpion::getEnergyLossMatingFemale () const 
{
	return getAppConfig().scorpion_energy_loss_female_per_child; 
}

double Scorpion::getEnergyLossMatingMale () const
{
	return getAppConfig().scorpion_energy_loss_mating_male; 
}

size_t Scorpion::getMinBabies () const
{
	return getAppConfig().scorpion_min_children; 
} 

size_t Scorpion::getMaxBabies () const
{
	return getAppConfig().scorpion_max_children; 
}

double Scorpion::getGestationTime () const
{
	return getAppConfig().scorpion_gestation_time; 
}

size_t Scorpion::getCompteurScorpion ()
{
	return compteurScorpion; 
}

//AUTRES METHODES
void Scorpion::givingBirth() const
{
	Vec2d position(this->getPosition());
	Scorpion* baby = new Scorpion(position);
	getAppEnv().addEntity(baby);
}

void Scorpion::meet() 
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

//METHODES EATABLE
bool Scorpion::eatable(OrganicEntity const* entity) const
{
    return entity->eatableBy(this);
}

bool Scorpion::eatableBy(Gerbil const* gerbil) const
{
    return false;
}

bool Scorpion::eatableBy(Food const* food) const
{
    return false;
}

bool Scorpion::eatableBy(Scorpion const* scorpion) const
{
    return false;
}

bool Scorpion::eatableBy(Banana const* banana) const
{
    return false;
}


double Scorpion::getPerteEnergie() const
{
    return getAppConfig().scorpion_energy_loss_factor;
}

//METHODES MATABLE
bool Scorpion::matable(OrganicEntity const* other) const
{
	return other->canMate(this);
}

bool Scorpion::canMate(Scorpion const* scorpion) const
{
	bool conditionSexe(scorpion -> getSexe());
	bool conditionTempsVie(this->tempsEnVie.asSeconds() >= getAppConfig().scorpion_min_age_mating); //this suffisamment age
	bool conditionEnergyF(this->getEnergie() >= getAppConfig().scorpion_energy_min_mating_female); //this a assez d'energie pour se reproduire
	bool conditionEnergyM(this->getEnergie() >= getAppConfig().scorpion_energy_min_mating_male);
	
	if (scorpion->getState()!=Etat::MATING) {
		
		//si this est femelle
		if 	(this->getSexe()) {
			return ((not(conditionSexe)) and (conditionEnergyF) and (conditionTempsVie));
		}
		
		//si this est male
		else {
			return ((conditionSexe) and (scorpion->getState() != Etat::GIVING_BIRTH) and (conditionEnergyM) and (conditionTempsVie));
		}
	} return false;
}

bool Scorpion::canMate(Gerbil const* gerbil) const
{
	return false;
}	

bool Scorpion::canMate(Food const* food) const
{
	return false;
}

bool Scorpion::canMate(Banana const* banana) const
{
	return false;
}

//METHODES OBJETS
bool Scorpion::upgradable(OrganicEntity const* other) const
{
	return other->canUpgrade(this);
}

bool Scorpion::canUpgrade(Scorpion const* scorpion) const
{
	return false;
}

bool Scorpion::canUpgrade(Gerbil const* gerbil) const
{
	return false;
}

bool Scorpion::canUpgrade(Food const* food) const
{
	return false;
}

bool Scorpion::canUpgrade(Banana const* banana) const
{
	return false;
}

