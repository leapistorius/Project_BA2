#include "Environment.hpp"
#include <Animal/Animal.hpp>
#include <Environment/Wave.hpp>
#include <Application.hpp>
#include <algorithm>
#include <Animal/NeuronalScorpion/NeuronalScorpion.hpp>
#include <Animal/Scorpion.hpp> 
#include <Animal/Gerbil.hpp> 
#include <Environment/Food.hpp> 

void Environment::addEntity(OrganicEntity* nouveau)
{
    faune.push_back(nouveau);
}

void Environment::addWave (Wave* nouveau)
{
	waves.push_back(nouveau); 
}

void Environment::addObstacle (CircularCollider* nouveau)
{
	obstacles.push_back(nouveau); 
} 

void Environment::addGenerator(FoodGenerator* nouveau)
{
    food.push_back(nouveau);
}

void Environment::update(sf::Time dt)
{
	//liste faune 
    for (auto& entity: faune) {
            entity -> update(dt);		//mise a jour de la position et donc du dessin
    }
    
    for (auto& entity: faune) {
		if (entity != nullptr) {
			if (entity->isDead()) {		//controle si entite est morte
                delete entity;
                entity = nullptr;
            }
		}
	}

    faune.erase(std::remove(faune.begin(), faune.end(), nullptr), faune.end());		//efface tous les nullptr (donc efface entites mortes)

	//liste food generator 
    for (auto& entity: food) {
        if (entity != nullptr) {
            entity -> update(dt);
        }
    }
    
    //liste waves 
    for (auto& wave: waves) {
            wave -> update(dt);
    }
    
    for (auto& wave: waves) {
		if (wave != nullptr) {
			if (wave->getIntensity() < getAppConfig().wave_intensity_threshold) {		//controle si wave intensity est trop faible
                delete wave;
                wave = nullptr;
            }
		}
	}
    waves.erase(std::remove(waves.begin(), waves.end(), nullptr), waves.end());
}

void Environment::draw(sf::RenderTarget& targetWindow) const
{
    //dessine la faune
    for (auto& entity: faune) {
        if (entity != nullptr) {
            entity -> draw(targetWindow);
        }
    }
    
    //dessine les ondes
    for (auto& wave: waves) {
        if (wave != nullptr) {
            wave -> draw(targetWindow);
        }
    }
    
    //dessine les obstacles
    for (auto& obstacle: obstacles) {
        if (obstacle != nullptr) {
            obstacle -> draw(targetWindow);
        }
    }
}

void Environment::clean()
{
    //efface la faune
    for (auto& entity: faune) {
		if(entity!=nullptr) {
			//entity -> diminueCompteur();	//diminue le compteur du taype de l'entite specifique si elle meurt
			delete entity;
			entity=nullptr;			
		}
    }
    faune.clear();
 
	//efface les ondes
	for (auto& wave: waves) {
		delete wave;
        wave=nullptr;
    }
    waves.clear();
   
   //efface les obstacles 
   for (auto& obstacle: obstacles) {
		delete obstacle;
        obstacle=nullptr;
    }
    obstacles.clear();
}

Environment::~Environment()
{
    clean();
}

std::vector<OrganicEntity*> Environment::getEntitiesInSightForAnimal(Animal* animal) const 
{
    std::vector<OrganicEntity*> seenEntities;

    if(not(faune.empty())) {
        for(auto& entity: faune) {
            if ((entity != nullptr) and (animal -> isTargetInSight(entity -> getPosition()))) {
                seenEntities.push_back(entity);
            }
        }
    }
    return seenEntities;
}

std::vector<CircularCollider*> Environment::getObstacles() const 
{
    std::vector<CircularCollider*> colliders;

    if(not(obstacles.empty())) {
        for(auto& obstacle: obstacles) {
            if (obstacle != nullptr) {
                colliders.push_back(obstacle);
            }
        }
    }
    return colliders;
}

double Environment::getIntensityForSensor(NeuronalScorpion* scorpion, size_t i) 
{
	double intensity(0); 
	
	for (auto& onde: waves){
		
		double rayon (onde->getRadius()); 
		Vec2d positionSensor (scorpion->getPositionOfSensor(i)); 
		bool conditionDistMin ((onde->distanceTo(positionSensor) - getAppConfig().wave_on_wave_marging)<= rayon) ; 
		bool conditionDistMax ((onde->distanceTo(positionSensor) + getAppConfig().wave_on_wave_marging)>= rayon); 
		
		double arcAngle ((positionSensor - onde->getPosition()).angle()); 
		std::vector<Arcs*> ensembleOndes(onde->getEnsembleArcs());
		
		for (auto& curve: ensembleOndes) {
			bool conditionArcMin (arcAngle>=curve->first);  
			bool conditionArcMax (arcAngle<=curve->second);
			if (conditionArcMin and conditionArcMax and conditionDistMin and conditionDistMax) {
				//incrementer l'intensite pour chaque onde qui touche les senseurs 
				intensity += onde->getIntensity();  
			}
		}
	}
	return intensity; 
}

std::unordered_map<std::string,double> Environment::fetchData(std::string title)
{
	std::unordered_map<std::string,double> newData;
	
	if (title == "General") {
		
		//retourne un graphe de pairs de titres et de son nombre d'especes 
		newData.emplace("Scorpions",Scorpion::getCompteurScorpion());
		newData.emplace("Gerbils",Gerbil::getCompteurGerbil());
		newData.emplace("Food sources",Food::getCompteurFood());
		return newData;
	
	} else if (title == "Waves"){
		
		size_t compteurWave(0);
		
		//compte le nombre d'ondes
		if(not(waves.empty())) {
			for(auto& onde: waves) {
				if (onde!=nullptr) ++compteurWave;
			}	
		}
		
		newData.emplace("Waves",compteurWave);
		return newData;
	}
}
