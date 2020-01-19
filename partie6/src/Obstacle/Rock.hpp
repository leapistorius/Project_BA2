#ifndef ROCK_H
#define ROCK_H

#include <iostream>
#include <Obstacle/CircularCollider.hpp>

class Rock : public CircularCollider {
	
	public:
		Rock(Vec2d emplacement); 
		~Rock() = default; 
		
		sf::Texture& getTexture() const; 
		void draw(sf::RenderTarget& targetWindow) const override; 
		
	private: 
		double orientation; 
}; 


#endif

