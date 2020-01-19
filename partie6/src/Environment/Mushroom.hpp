#ifndef MUSHROOM_H
#define MUSHROOM_H

#include <iostream> 
#include <Environment/Food.hpp> 


class Mushroom : public Food {
	
	public : 
	
		//constructeur/destructeur
		Mushroom(const Vec2d& position_);
		~Mushroom () = default;
		
		//getters 
		sf::Texture& getTexture() const override;
		double getSize() const override; 
		double getEnergieInitial() const override; 
		
		double getEnergie() const override; 
 
	
}; 


#endif
