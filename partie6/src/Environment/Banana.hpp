#ifndef BANANA_H
#define BANANA_H

#include <iostream> 
#include <Environment/OrganicEntity.hpp> 
#include <Utility/Utility.hpp>

class Banana : public OrganicEntity {
	
	private: 
	
		sf::Time compteurVie; 
	
	public : 
	
		//constructeur/destructeur
		Banana(const Vec2d& position_);
		~Banana () = default;
		
		//getters 
		sf::Texture& getTexture() const override;
		double getSize() const override; 
		double getEnergieInitial() const override;
		sf::Time getLongetivity() const override; 
		sf::Time getTempsEnceinte() const override;
		double getPerteEnergie() const override;
		double getEnergieConsumption() const override;
		double getVitesse() const override; 
		
		void update(sf::Time dt) override;
		void draw(sf::RenderTarget& targetWindow) const override;
		
		void meet() override;
		void eating(double& energie) override;  
		
		//methodes eatable
		bool eatable(OrganicEntity const* entity) const override;
		bool eatableBy(Gerbil const* gerbil) const override;
		bool eatableBy(Food const* food) const override;
		bool eatableBy(Scorpion const* scorpion) const override;
		bool eatableBy(Banana const* banana) const override; 
		
		//methodes matable
		bool matable(OrganicEntity const* other) const override;
		bool canMate(Scorpion const* scorpion) const override;
		bool canMate(Gerbil const* gerbil) const override;
		bool canMate(Food const* food) const override;
		bool canMate(Banana const* banana) const override;
		
		//methodes objets 
		bool upgradable (OrganicEntity const* other) const override; 
		bool canUpgrade(Scorpion const* scorpion) const override;
		bool canUpgrade(Gerbil const* gerbil) const override;
		bool canUpgrade(Food const* food) const override;
		bool canUpgrade(Banana const* banana) const override; 

}; 


#endif



