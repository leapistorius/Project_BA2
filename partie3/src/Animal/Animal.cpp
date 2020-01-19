#include "Animal.hpp"
#include <Application.hpp>
#include <SFML/Graphics.hpp>
#include <Utility/Arc.hpp>
#include <Random/Uniform.hpp>
#include <cmath>
#include <vector>
#include <string>

//Constructeur Animal 
Animal::Animal(const Vec2d& position_, double taille, double energie, bool sexe)
: OrganicEntity(position_, taille, energie), tempsPause(sf::Time::Zero), tempsEnceinte (sf::Time::Zero), normeVitesse(0.001), femelle(sexe), state(WANDERING), nbBabies (0), cible(nullptr), mate(nullptr)
{						 																						
	direction.x = 1.0;
	direction.y = 0.0;
	currentTarget.x = 1.0; 
	currentTarget.y = 0.0; 
}

//getters 
double Animal::getRotation() const 
{
	return direction.angle();
}

Vec2d Animal::getSpeedVector() const
{
	return direction*normeVitesse; 
}

bool Animal::getSexe() const 
{
	return femelle;
}

Etat Animal::getState() const
{
	return state;
}

sf::Time Animal::getTempsEnceinte() const 
{
	return tempsEnceinte;
}

size_t Animal::getBabies () const 
{
	return nbBabies; 
}

double Animal::getEnergieConsumption() const 
{
	return getAppConfig().animal_base_energy_consumption; 
}

double Animal::getVitesse () const 
{
	return normeVitesse; 
}

//setters
void Animal::setRotation(double angle)
{
	direction.x = cos(angle);
	direction.y = sin(angle);
}

void Animal::setState(Etat etat) 
{
	state = etat; 
}

void Animal::setBabies (size_t babies) 
{
	nbBabies = babies; 
}

void Animal::setMate(OrganicEntity* partenaire)
{
	mate = partenaire;
}

//METHODES DRAW
void Animal::draw(sf::RenderTarget& targetWindow) const
{
    if(isDebugOn()){
		drawVision(targetWindow);
		drawRandomWalk(targetWindow);
		drawDebug(targetWindow);	
	}
	
	drawSprite(targetWindow);
} 

void Animal::drawSprite(sf::RenderTarget& targetWindow) const  
{
    auto image_to_draw(buildSprite(getPosition(), getRadius()*2, getTexture(), getRotation()/DEG_TO_RAD));	
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

void Animal::drawDebug(sf::RenderTarget& targetWindow) const 
{	
	std::string sexe; 
	if (femelle==true){
		sexe = "Female "; 
	} else {
		sexe = "Male "; 
	}
	std::string un_texte(sexe + '\n' + to_nice_string(this->getEnergie()) + '\n');  
	switch(getState()){
		case FOOD_IN_SIGHT: un_texte += " Food in Sight";  
			break; 
		case FEEDING: un_texte += " Feeding"; 
			break; 
		case RUNNING_AWAY: un_texte += " Running Away";
			break; 
		case MATE_IN_SIGHT:	un_texte += " Mate in Sight"; 
			break; 
		case MATING: un_texte += " Mating"; 
			break; 
		case GIVING_BIRTH: un_texte += " Giving birth"; 
			break; 
		case WANDERING: un_texte += " Wandering";  
			break;  
	}
	
	Vec2d distanceText(getRandomWalkDistance(), 0.0);
	auto text = buildText(un_texte,									
					  convertToGlobalCoord(distanceText),
					  getAppFont(),
					  getAppConfig().default_debug_text_size,
					  sf::Color::Black,
					  getRotation() / DEG_TO_RAD + 90
					 );
	targetWindow.draw(buildCircle(getPosition(), 50, sf::Color::White)); 
	
	if (femelle==true and tempsEnceinte> sf::Time::Zero)
	{
		targetWindow.draw(buildAnnulus(getPosition(), 50, sf::Color::Magenta, 2, sf::Color::Transparent)); 
	}
	
	targetWindow.draw(text);	
}

//update
void Animal:: update(sf::Time dt)
{ 	
	OrganicEntity::update(dt);	//incrementation tempsEnVie, update l'energie
	Vec2d force; 
	Vec2d null(0.0, 0.0); 
	
	switch(updateState(dt)){
		case FOOD_IN_SIGHT: force = forceAttraction(cible->getPosition()); 
			break; 
		case FEEDING: force = null;
				normeVitesse = 0.01; //ralentit quand FEEDING
			break; 
		case RUNNING_AWAY: force = forceFuite(); //qd scorpion rapproche gerbil, elle fuit 
			break; 
		case MATE_IN_SIGHT:	force = forceAttraction(mate->getPosition());
			break; 
		case MATING: force = null;
				normeVitesse = 0.01; //ralentit quand MATING
			break; 
		case GIVING_BIRTH: force = null;
				normeVitesse = 0.01; //ralentit quand GIVINGBIRTH
			break; 
		case WANDERING: force = convertToGlobalCoord(randomWalk())-getPosition();
			break;  
	}
	
	miseAJourDeplacement(force, dt);

} 

Etat Animal::updateState(sf::Time dt)	
{	
	//si en pause (FEEDING, MATING, GIVINGBIRTH)
	if (tempsPause > sf::Time::Zero) {		
		tempsPause -= dt;
	} 
	
	else {
		
		//si femelle enceinte 
		if (tempsEnceinte > sf::Time::Zero){
			setState(WANDERING);
			tempsEnceinte -= dt; 
					
			//si femelle doit mettre bas
			if (tempsEnceinte<=sf::Time::Zero) {		//isEqual(tempsEnceinte.asSeconds(),0)
				tempsPause = sf::seconds(getAppConfig().animal_tempsPause);	
				setState(GIVING_BIRTH);
				givingBirth();
			}
		}
	
		analyzeEnvironnement();
		
		bool conditionRunningAway (not (enemies.empty()) and distanceEnemy()<= getAppConfig().animal_distMaxEnemy);  
		
		//si un enemy en vue (prime sur partenaire)
		if (conditionRunningAway) {
			setState(RUNNING_AWAY); //ajouter conditions pour que animal soit en "securite"
		}
			
		//si un partenaire en vue (prime sur cible)
		else if (mate != nullptr) {	
			if (this->isColliding(*mate)){	 //si les animaux sont MATING
				mate->meet();
				meet(); 
			} else { 
				setState(MATE_IN_SIGHT); 
			}
			
		//si une cible en vue
		} else if (cible != nullptr) {
			if (this->isColliding(*cible)){	 //si la cible est dans l'animal (*this>*cible)
					setState(FEEDING);
					tempsPause = sf::seconds(getAppConfig().animal_tempsPause); 
			} else {
				setState(FOOD_IN_SIGHT); 
			}
		} else {
			setState(WANDERING); 
		}
	}
	
	return state; 
}

void Animal::analyzeEnvironnement() 
{	
	cible = nullptr;
	mate = nullptr;
	
	std::vector<OrganicEntity*> cibles (getAppEnv().Environment::getEntitiesInSightForAnimal(this)); 	//trouve toutes cibles dans champ de vision de l'animal
	
	if (not(cibles.empty())) {	
	
		//tmp
		double tmpDistMate(getAppConfig().simulation_world_size);
		double tmpDistTarget(getAppConfig().simulation_world_size);
	
		
		for (auto& entity: cibles){	
			//conditions
			bool conditionMatable (matable(entity) and (distanceTo(*entity) < tmpDistMate)); 
			bool conditionEnceinte ((entity->getTempsEnceinte() <= sf::Time::Zero) and (this->getTempsEnceinte() <= sf::Time::Zero));
			
			//pour toutes les entites reelles																
			if (entity!=nullptr) { 
				
				//trouve enemies
				if (entity->eatable(this)){ 
					enemies.push_back(entity);
				}
					  
				//trouve le partenaire le plus proche
				else if (conditionMatable and conditionEnceinte){
					tmpDistMate = distanceTo(*entity);
					mate = entity; 
				} 
						
				//trouve la proie la plus proche
				else if (eatable(entity) and (distanceTo(*entity) < tmpDistTarget)){
					tmpDistTarget = distanceTo(*entity);
					cible = entity; 
				}
			}
		}
	}
}

void Animal::eating (double& energie) 
{	
	if (cible!= nullptr and getState() == FEEDING){
		energie += 0.7 * (cible -> getEnergie()); 		//prend 70% de l'energie de cible 
		cible -> setEnergie(0); 
		cible = nullptr;		
	}
}

double Animal::getMaxSpeed() const 
{	 
	if (this->getEnergie() < getAppConfig().animal_seuilEnergie) {
		return getStandardMaxSpeed()*0.5; //plus lentement
	} else {
		switch(getState()){
			case FOOD_IN_SIGHT: return getStandardMaxSpeed()*3;
			case RUNNING_AWAY: return getStandardMaxSpeed()*4;
			case MATE_IN_SIGHT: return getStandardMaxSpeed()*2;
			default: return getStandardMaxSpeed(); 
		}
	}
}

void Animal::miseAJourDeplacement(Vec2d const& force, sf::Time dt) {
	
	Vec2d acceleration(force/getMass());												
	Vec2d newVitesse ( (direction * normeVitesse) + (acceleration * dt.asSeconds()) );				
	Vec2d newDirection(newVitesse.normalised());								
			
	double maxSpeed(getMaxSpeed());
	if (newVitesse.length()>maxSpeed) {
		newVitesse = newDirection * maxSpeed;
	}

	Vec2d newPosition(getPosition() + (newVitesse * dt.asSeconds()) );
		
	Vec2d nul(0.0, 0.0);
	if (direction!=nul) {	//verifie que direction soit non-nulle 
		direction=newDirection;
		normeVitesse=newVitesse.length();
		setPosition(newPosition);
	}
	
	clamping();	
}

Vec2d Animal::forceAttraction (const Vec2d& target) const 
{
	if (*this>target) {	
		Vec2d force(0.0, 0.0);
		return force;				
	} else {	
	
		Deceleration d = moyen; 	//parametrable selon les desirs
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
			
		Vec2d toTarget(target-getPosition());						
		
		double speed(std::min(toTarget.length()/decelere,getMaxSpeed()));
		Vec2d v_target(toTarget.normalised() * speed);		
		
		return v_target - (direction * normeVitesse);
	}	
}

Vec2d Animal::forceFuite () const 
{	
	Vec2d force (0.0, 0.0);
	for (auto& enemy : enemies){
		if (enemy!=nullptr){
			Vec2d difference ((enemy->getPosition()) - (this->getPosition()));
			force += ((getAppConfig().animal_coefAmp1 *difference)/pow(difference.length(),getAppConfig().animal_coefAmp2));
		} 
	}
	
	return -force;
}

double Animal::distanceEnemy() 
{	
	double tmpDistEnemy (getAppConfig().simulation_world_size); 
	
	if (not (enemies.empty())){ 
		OrganicEntity* enemyPlusProche = nullptr;
		
		for (auto& entity: enemies){
			if (entity != nullptr){
				if (distanceTo(*entity) < tmpDistEnemy){
					tmpDistEnemy = distanceTo(*entity);
					enemyPlusProche = entity;
				} else entity=nullptr;
			}
		}
	}
	
	//laver tableau
	cleanEnemies();
	return tmpDistEnemy;
	
}

void Animal::cleanEnemies () 
{
	enemies.erase(std::remove(enemies.begin(), enemies.end(), nullptr), enemies.end());
}

bool Animal::isTargetInSight(const Vec2d& target) const
{
	Vec2d distance(target-getPosition());
	
	if (isEqual(distance.lengthSquared(), 0.0)) {
		return true;
	} else return ((distance.lengthSquared()<=pow(getViewDistance(),2)) and (direction.dot(distance.normalised())>=cos((getViewRange() + 0.001)/2)));
}

Vec2d Animal::getMovedCurrentTarget() const
{
	Vec2d distanceCircle (getRandomWalkDistance(), 0.0); 
	Vec2d movedCurrentTarget (currentTarget + distanceCircle); 
	
	return movedCurrentTarget; 
}
	
Vec2d Animal::randomWalk()
{
	Vec2d randomVec (uniform(-1.0, 1.0), uniform(-1.0, 1.0)); 		//generer une cible vituelle 
	
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
	matTransform.rotate(direction.angle()/DEG_TO_RAD); 		//avec angle du vecteur direction animal en degree 

	// now transform the point
	Vec2d global = matTransform.transformPoint(local);
       
    return global; 
}

