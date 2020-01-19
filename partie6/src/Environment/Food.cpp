#include "Food.hpp"
#include <Application.hpp>

size_t Food::compteurFood(0); 

//Constructeur
Food::Food(const Vec2d& position_)
: OrganicEntity(position_, getSize(), getEnergieInitial()) 
{
	compteurFood += 1; 
}

Food::Food(const Vec2d& position_, double size, double energie)
: OrganicEntity (position_, size, energie)
{
	compteurFood += 1; 
}

Food::~Food()
{
	compteurFood -= 1;
}

//getters
double Food::getEnergieInitial() const
{
    return getAppConfig().food_energy;
}

double Food::getSize() const
{
    return getAppConfig().food_size;
}

sf::Texture& Food::getTexture() const
{
    return getAppTexture(getAppConfig().food_texture);
}

sf::Time Food::getLongetivity()	const
{
    return OrganicEntity::getLongetivity();		//longueur de vie modifiable si souhaitable (pour l'instant valeur par defaut: "infinie")
}

void Food::draw(sf::RenderTarget& targetWindow) const
{
    auto image_to_draw(buildSprite(getPosition(), getRadius()*2 ,getTexture()));
    targetWindow.draw(image_to_draw);
}

sf::Time Food::getTempsEnceinte() const {}

double Food::getPerteEnergie() const 
{
	return 0.0;
} 

double Food::getEnergieConsumption() const
{
	return 0.0;
}

double Food::getVitesse() const
{
	return 0.0;
} 

size_t Food::getCompteurFood()
{
	return compteurFood; 
}

void Food:: update(sf::Time dt) {}

void Food::meet() {}

void Food::eating(double& energie) {}

//methodes eatable
bool Food::eatable(OrganicEntity const* entity) const
{
    return entity->eatableBy(this);
}

bool Food::eatableBy(Gerbil const* gerbil) const
{
    return true;
}

bool Food::eatableBy(Food const* food) const
{
    return false;
}

bool Food::eatableBy(Scorpion const* scoprion) const
{
    return false;
}

bool Food::eatableBy(Banana const* banana) const
{
    return false;
}

//methodes matable
bool Food::matable(OrganicEntity const* other) const
{
	return other->canMate(this);
}

bool Food::canMate(Scorpion const* scorpion) const
{
	return false;
}

bool Food::canMate(Gerbil const* gerbil) const
{
	return false;
}

bool Food::canMate(Food const* food) const
{
	return false;
}

bool Food::canMate(Banana const* banana) const
{
	return false;
}

//methodes objet 
bool Food::upgradable(OrganicEntity const* other) const
{
	return other->canUpgrade(this);
}

bool Food::canUpgrade(Scorpion const* scorpion) const
{
	return false;
}

bool Food::canUpgrade(Gerbil const* gerbil) const
{
	return false;
}

bool Food::canUpgrade(Food const* food) const
{
	return false;
}

bool Food::canUpgrade(Banana const* banana) const
{
	return false;
}


