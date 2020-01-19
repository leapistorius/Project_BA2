#include "Animal.hpp"
#include <Application.hpp>
#include <Utility/Constants.hpp>
#include <Environment/Environment.hpp>
#include <SFML/Graphics.hpp>
#include <Utility/Arc.hpp>
#include <Random/Uniform.hpp>
#include <cmath>
#include <vector>

//Getters et setters 
double Animal::getStandardMaxSpeed() const
{
	return ANIMAL_MAX_SPEED; 
}

double Animal::getMass() const
{
	return ANIMAL_MASS; 
}

double Animal::getViewRange() const 
{
	return ANIMAL_VIEW_RANGE; 
}

double Animal::getViewDistance() const
{
	return ANIMAL_VIEW_DISTANCE; 
}

double Animal::getRotation() const 
{
	return direction.angle();
}
	
void Animal::setRotation(double angle)
{
	direction.x = cos(angle);
	direction.y = sin(angle);
}

void Animal::setTargetPosition (const Vec2d& target)
{
	positionCible = target; 
	clamping();
}

Vec2d Animal::getSpeedVector() const
{
	return direction*normeVitesse; 
}

double Animal::getRandomWalkRadius() const
{
	return ANIMAL_RANDOM_WALK_RADIUS; 
}

double Animal::getRandomWalkDistance() const
{
	return ANIMAL_RANDOM_WALK_DISTANCE; 
}

double Animal::getRandomWalkJitter() const
{
	return ANIMAL_RANDOM_WALK_JITTER; 
}

void Animal::setDeceleration (int d_) 
{
	d=d_;
}

//Methodes draw
void Animal::draw(sf::RenderTarget& targetWindow) const 
{
    drawSprite(targetWindow);
    drawVision(targetWindow);
    drawRandomWalk(targetWindow); 
} 

void Animal::drawSprite(sf::RenderTarget& targetWindow) const 
{
	sf::Texture& texture = getAppTexture(ANIMAL_TEXTURE);
    auto image_to_draw(buildSprite(getPosition(), getRadius()*2 ,texture, getRotation()/DEG_TO_RAD));
    targetWindow.draw(image_to_draw);
}

void Animal::drawVision(sf::RenderTarget& targetWindow) const 
{
    sf::Color color = sf::Color::Black;
    color.a = 16; // light, transparent grey

    double minAngle((-getViewRange()/2.0)/DEG_TO_RAD);
    double maxAngle((getViewRange()/2.0)/DEG_TO_RAD);
    
    Arc arc(buildArc(minAngle, maxAngle, getViewDistance(), getPosition(), color, getRotation()/DEG_TO_RAD));
    targetWindow.draw(arc);	
}

void Animal::drawRandomWalk(sf::RenderTarget& targetWindow) const
{
	//draw arc du cercle en jaune
	sf::Color color(255, 150, 0); 
	Vec2d distanceCircle(getRandomWalkDistance(), 0.0);
	targetWindow.draw(buildAnnulus(convertToGlobalCoord(distanceCircle), getRandomWalkRadius(), color, 2)); 

	//draw la cible virtuelle
	targetWindow.draw(buildCircle(convertToGlobalCoord(getMovedCurrentTarget()), 5.0, sf::Color(0, 0, 255))); 
} 

//Constructeurs Animal 
Animal::Animal(const Vec2d& position_, double r, double speed)		
: CircularCollider(position_, r), normeVitesse(speed)
{	positionCible.x = 0.0; 
	positionCible.y = 0.0; 
	direction.x = 1.0;
	direction.y = 0.0;
	currentTarget.x = 1.0; 
	currentTarget.y = 0.0; 
}
 
Animal::Animal(const Vec2d& position_)
: Animal(position_, ANIMAL_RADIUS, 0.0)
{}


//update de la classe Animal 
void Animal:: update(sf::Time dt)
{ 	
	//1. chercher les cibles dans champ de vision et bon environnement 
	std::vector<Vec2d> cibles (getAppEnv().Environment::getTargetsInSightForAnimal(this));	
	
	//2. choisir une cible au hasard
	if (not(cibles.empty())) {
		int randomIndex = rand() % cibles.size();			
		Vec2d cible(cibles[randomIndex]); 		
		
	//3. si au moins une, la stocker comme cible -> calculer force + mettre a jour deplacement
		Vec2d force(forceAttraction(cible));
		miseAJourDeplacement(force, dt);	
		
	} else {
	//4. randomwalk et 5. mise a jour du deplacement
		Vec2d force(convertToGlobalCoord(randomWalk())-getPosition());
		miseAJourDeplacement(force, dt);
	} 
}	

void Animal::miseAJourDeplacement(Vec2d const& force, sf::Time dt) 
{	
	Vec2d acceleration(force/getMass());												
		Vec2d newVitesse ( (direction * normeVitesse) + (acceleration * dt.asSeconds()) );				
		Vec2d newDirection(newVitesse.normalised());								
			
		double maxSpeed(getStandardMaxSpeed());
		if (newVitesse.length()>maxSpeed) {
			newVitesse = newDirection * maxSpeed;
		}

		Vec2d newPosition(getPosition() + (newVitesse * dt.asSeconds()) );
		
		Vec2d nul(0.0, 0.0);
		if (direction!=nul) {			//verifie que direction soit non-nulle 
			direction=newDirection;
			normeVitesse=newVitesse.length();
			setPosition(newPosition);
		}	
}

	
Vec2d Animal::forceAttraction (const Vec2d& cible) const 
{
	if (isPointInside(cible)) {	
		Vec2d force(0.0, 0.0);
		return force;				
	} else {	 
		
		double decelere;
		switch(d) {
			case fort: 
				decelere = 0.9; 
				break; 
			case moyen: 
				decelere = 0.6; 
				break; 
			case faible: 
				decelere = 0.3; 
				break; 
		}
			
		Vec2d toTarget(cible-getPosition());						
		
		double speed(std::min(toTarget.length()/decelere,getStandardMaxSpeed()));
		Vec2d v_target(toTarget.normalised() * speed);		
		
		return v_target - (direction * normeVitesse);
	}
	
}

//Perception d'une cible
bool Animal::isTargetInSight(const Vec2d& target) const
{
	Vec2d distance(target-getPosition());
	
	if (isEqual(distance.lengthSquared(), 0.0)) {
		return true;
	} else if ((distance.lengthSquared()<=pow(getViewDistance(),2)) and (direction.dot(distance.normalised())>=cos((getViewRange() + 0.001)/2))) {
		return true;
	} else {
		return false;	
	}
}

Vec2d Animal::getMovedCurrentTarget() const
{
	Vec2d distanceCircle (getRandomWalkDistance(), 0.0); 
	Vec2d movedCurrentTarget (currentTarget + distanceCircle); 
	
	return movedCurrentTarget; 
}

//Deplacement aleatoire 
Vec2d Animal::randomWalk()
{
	//generer une cible vituelle 
	Vec2d randomVec (uniform(-1.0, 1.0), uniform(-1.0, 1.0)); 
	
	currentTarget += randomVec * getRandomWalkJitter(); 
	currentTarget = currentTarget.normalised() * getRandomWalkRadius(); 
	
	Vec2d distanceCircle (getRandomWalkDistance(), 0.0); 
	Vec2d movedCurrentTarget (currentTarget + distanceCircle); 
	
	return movedCurrentTarget; 
	 	
}

Vec2d Animal::convertToGlobalCoord(const Vec2d& local) const
{
	 // create a transformation matrix
       sf::Transform matTransform;

       // first, translate
       matTransform.translate(getPosition()); 

       // then rotate
       matTransform.rotate(direction.angle()/DEG_TO_RAD); 

       // now transform the point
       Vec2d global = matTransform.transformPoint(local);
       
    return global; 
       
}
 
