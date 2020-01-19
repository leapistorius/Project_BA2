#include "Banana.hpp"
#include <Application.hpp>

Banana::Banana(const Vec2d& position_)
: OrganicEntity (position_, getSize(), getEnergieInitial()), compteurVie (sf::Time::Zero)
{}

sf::Texture& Banana::getTexture() const
{
	return getAppTexture(getAppConfig().banana_texture); 
}

double Banana::getEnergieInitial() const
{
    return getAppConfig().banana_energy; 
}

double Banana::getSize() const
{
    return getAppConfig().banana_size;
}

//voir quand animal touche il fait rotation de 360degrees 
void Banana::update (sf::Time dt) 
{
	compteurVie += dt; 
	if (compteurVie >= sf::seconds(10)){ //si dans l'environment pendant plus de 10s disparait
		setEnergie(0); 
	}
}

void Banana::draw (sf::RenderTarget& targetWindow) const
{
	auto image_to_draw(buildSprite(getPosition(), getSize(), getTexture()));	
    targetWindow.draw(image_to_draw);
}


sf::Time Banana::getLongetivity() const
{
    return OrganicEntity::getLongetivity();		//longueur de vie modifiable si souhaitable (pour l'instant valeur par defaut: "infinie")
}

sf::Time Banana::getTempsEnceinte() const {}

double Banana::getPerteEnergie() const 
{
	return 0.0;
} 

double Banana::getEnergieConsumption() const
{
	return 0.0;
}

double Banana::getVitesse() const
{
	return 0.0;
} 

void Banana::meet() {}

void Banana::eating(double& energie) {}

//methodes eatable
bool Banana::eatable(OrganicEntity const* entity) const
{
    return entity->eatableBy(this);
}

bool Banana::eatableBy(Gerbil const* gerbil) const
{
    return false;
}

bool Banana::eatableBy(Food const* food) const
{
    return false;
}

bool Banana::eatableBy(Scorpion const* scoprion) const
{
    return false;
}

bool Banana::eatableBy(Banana const* banana) const
{
    return false;
}

//methodes matable
bool Banana::matable(OrganicEntity const* other) const
{
	return other->canMate(this);
}

bool Banana::canMate(Scorpion const* scorpion) const
{
	return true;
}

bool Banana::canMate(Gerbil const* gerbil) const
{
	return true;
}

bool Banana::canMate(Food const* food) const
{
	return true;
}

bool Banana::canMate(Banana const* banana) const
{
	return false;
}


//methodes objet 
bool Banana::upgradable(OrganicEntity const* other) const 
{
	return other->canUpgrade(this);
}

bool Banana::canUpgrade(Scorpion const* scorpion) const //seulement les scorpions et les gerbils sont affectes a la banane 
{
	return true;
}

bool Banana::canUpgrade(Gerbil const* gerbil) const
{
	return true;
}

bool Banana::canUpgrade(Food const* food) const
{
	return false;
}

bool Banana::canUpgrade(Banana const* banana) const
{
	return false;
}

