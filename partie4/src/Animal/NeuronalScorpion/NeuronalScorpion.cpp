#include <cmath>
#include "NeuronalScorpion.hpp"
#include <Random/Uniform.hpp>
#include <Application.hpp>

NeuronalScorpion::NeuronalScorpion(const Vec2d& positionInitiale, double energie, bool sexe)
: Scorpion(positionInitiale, energie, sexe), directionCible(0, 0), state(Etat::WANDERING), horloge(sf::Time::Zero), compteurTemps(sf::Time::Zero)
{
	initialiseSensors(); 
}

NeuronalScorpion::NeuronalScorpion(const Vec2d& positionInitiale)
: NeuronalScorpion(positionInitiale, getEnergieInitial(), uniform(0, 1)==0)
{}

Vec2d NeuronalScorpion::getPositionOfSensor(size_t i) const
{
	return convertToGlobalCoord(sensors[i].second->getLocationSensor()); 
}

std::array<std::pair<double,Sensor*>, 8> NeuronalScorpion::getSensors() const
{
	return sensors;
}

void NeuronalScorpion::setState(NeuronalScorpion::Etat etat)
{
	state = etat; 
}

void NeuronalScorpion::update(sf::Time dt)
{	
	bool isActif(false);
	for(size_t i(0); i<sensors.size(); ++i) {		
		sensors[i].second->update(dt);
		if (sensors[i].second->getState()==ACTIF) {
			isActif = true;
		}
	}
	
	//si les senseurs sont actifs ou ont deja etes actifs sans que le compteurs soit reinitialise a 0
	if (isActif or compteurTemps>sf::Time::Zero) {
		updateCompteur(dt);
	}
	
	if (verifyCompteur()) {
		compteurTemps = sf::Time::Zero; 		//reinitialise compteur 
		updateDirection();	
		for(auto& sensor: sensors) {	
			sensor.second->reInitialisation();	//reinitialise chaque sensor
		}
	}		
	
	calculForce(dt);
}

NeuronalScorpion::Etat NeuronalScorpion::updateState(sf::Time dt)
{
	analyzeEnvironnement(); 
	bool conditionTemps (horloge <= sf::Time::Zero); 
	
	//si une cible en vue
	if (getCible() != nullptr) {			
		setState(Etat::TARGET_IN_SIGHT); 
	} 
	
	//si dans etat IDLE
	else if (state==Etat::IDLE){
		double scoreEstimation (directionCible.length()); 
		if (scoreEstimation > getAppConfig().scorpion_minimal_score_for_action){
			horloge = sf::Time::Zero; 	
			setState(Etat::MOVING);
		} else if (conditionTemps){
			setState(Etat::WANDERING);
		}
	} 
	
	//si dans Etat MOVING
	else if (state == Etat::MOVING){
		if (conditionTemps){
			horloge = sf::seconds(getAppConfig().scorpion_idle_time);
			setState(Etat::IDLE);
			this->reinitialisation();
		} 
	} 
	
	//si dans Etat WANDERING
	else if (state == Etat::WANDERING){
		for(auto& sensor: sensors) {
			if (sensor.second->getState() == ACTIF){
				setState(Etat::IDLE);
				break;
			}
		}
		
	//si dans Etat TARGET_IN_SIGHTS	
	} else if (state == Etat::TARGET_IN_SIGHT){
		setState(Etat::IDLE);
	}
	
	return state; 
}

void NeuronalScorpion::updateDirection()
{	 
	Vec2d tmpDirection(directionCible);
	
	for(auto& sensor: sensors) {
		double tmpSensor(sensor.second->getScore()*getAppConfig().scorpion_sensor_radius);
		double tmpAngle(sensor.first + this->getRotation());
		tmpDirection.x += tmpSensor*cos(tmpAngle);
		tmpDirection.y += tmpSensor*sin(tmpAngle);
	}
	
	directionCible = convertToGlobalCoord(tmpDirection);
}

void NeuronalScorpion::updateHorloge(sf::Time dt)
{
	horloge -= dt; 
}

void NeuronalScorpion::updateCompteur(sf::Time dt)
{
	compteurTemps += dt;	
}

void NeuronalScorpion::calculForce(sf::Time dt) 
{
	Vec2d force;
	Vec2d null(0.0, 0.0); 
	switch(updateState(dt)){
		
		case Etat::IDLE: 
				
				if (horloge == sf::Time::Zero){	//seulement si changement d'etat
					horloge = sf::seconds(getAppConfig().scorpion_idle_time);
				} else {
					updateHorloge(dt); 
				}
			
				force = null;
				break;
				
		case Etat::WANDERING: force = convertToGlobalCoord(randomWalk())-getPosition();
				break;
				
		case Etat::TARGET_IN_SIGHT: force = forceAttraction(getCible()->getPosition());
				break;
				
		case Etat::MOVING: 
				
				if (horloge == sf::Time::Zero){		
					horloge = sf::seconds(getAppConfig().scorpion_moving_time);
				} else { 
					updateHorloge(dt); 
				}
				
				//verification angle rotation directionCible 
				double calculAngle (abs(getRotation() - directionCible.angle())); 
				if (calculAngle > getAppConfig().scorpion_rotation_angle_precision) {
					setRotation(directionCible.angle()); //rotation vers cible 
					force = null;
					setVitesse(0.01); //ralentissement 
				} else {
					force.x = getAppConfig().simulation_world_size * 0.1; //cible virtuelle a une distance d'un 10e de taille du monde 
					force.y = 0;
					convertToGlobalCoord(force);
				}
				break;
	}
	
	miseAJourDeplacement(force, dt);
}

void NeuronalScorpion::reinitialisation() 
{
	directionCible.x = 0.0;
	directionCible.y = 0.0; 
	
	//reinitialisation de chaque sensor
	for(auto& sensor: sensors) {
		sensor.second->reInitialisation();
	}
}

void NeuronalScorpion::draw(sf::RenderTarget& targetWindow) const
{
    if(isDebugOn()){
		drawDebug(targetWindow);	
	}
	
	Animal::drawSprite(targetWindow); 
}

void NeuronalScorpion::drawDebug (sf::RenderTarget& targetWindow) const 
{
	std::string un_texte;  
	switch(state){
		case Etat::IDLE: un_texte += " Idle";  
			break; 
		case Etat::WANDERING: un_texte += " Wandering"; 
			break; 
		case Etat::TARGET_IN_SIGHT: un_texte += " Target in Sight";
			break; 
		case Etat::MOVING:	un_texte += " Moving"; 
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
	
	targetWindow.draw(text);	
	
	for (size_t i(0); i<sensors.size(); ++i){
		sf::Color couleur; 
		if (sensors[i].second->getState() == ACTIF  and sensors[i].second->getInhibitor()>0.2){
			couleur = sf::Color::Magenta; 
		} else if (sensors[i].second->getState() == PAS_ACTIF and sensors[i].second->getInhibitor()>0.2){
			couleur = sf::Color::Blue; 
		} else if (sensors[i].second->getInhibitor() < 0.2 and sensors[i].second->getState()== ACTIF){
			couleur = sf::Color::Red; 
		} else {
			couleur = sf::Color::Green; 
		} 
		targetWindow.draw(buildCircle(getPositionOfSensor(i),  getRadius()/3, couleur));
	}
	
	targetWindow.draw(buildLine(getPosition(), getPosition() + directionCible, sf::Color::Black, 2));
}


void NeuronalScorpion::initialiseSensors()
{
	std::array<int, 8> angleSensor = {18, 54, 90, 140, -140, -90, -54, -18}; 
	double rayon (getAppConfig().scorpion_sensor_radius); 
	
	for (size_t i(0); i<sensors.size(); ++i){
		sensors[i].first = angleSensor[i];
		sensors[i].second = new Sensor(rayon*cos(angleSensor[i]), rayon*sin(angleSensor[i]), this, i);
	}	
}

bool NeuronalScorpion::verifyCompteur() 
{
	return (compteurTemps.asSeconds()>getAppConfig().sensor_activation_duration); 
}

Vec2d NeuronalScorpion::forceAttraction (const Vec2d& target) const 
{
	if (*this>target) {	
		Vec2d force(0.0, 0.0);
		return force;				
	} else {	
	
		Deceleration d = Deceleration::moyen; 	//parametrable selon les desirs
		double decelere;
		switch(d) {
			case Deceleration::fort: 
				decelere = 0.9; 
				break; 
			case Deceleration::moyen: 
				decelere = 0.6; 
				break; 
			case Deceleration::faible: 
				decelere = 0.3; 
				break; 
		}
			
		Vec2d toTarget(target-getPosition());						
		
		double speed(std::min(toTarget.length()/decelere,getMaxSpeed()));
		Vec2d v_target(toTarget.normalised() * speed);		
		
		return v_target - (getDirection() * getVitesse());
	}	
}

void NeuronalScorpion::analyzeEnvironnement() 
{
	setCible(nullptr);
	
	std::vector<OrganicEntity*> cibles (getAppEnv().getEntitiesInSightForAnimal(this)); 	//trouve toutes cibles dans champ de vision de l'animal
	
	if (not(cibles.empty())) {	

		double tmpDistTarget(getAppConfig().simulation_world_size);
		
		for (auto& entity: cibles){																	
			if (entity!=nullptr and eatable(entity) and (distanceTo(*entity) < tmpDistTarget)){
				tmpDistTarget = distanceTo(*entity);
				setCible(entity); 
			}
		}
	}
}



