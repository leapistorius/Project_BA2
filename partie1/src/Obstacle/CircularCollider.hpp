#ifndef CIRCULAR_COLLIDER_H
#define CIRCULAR_COLLIDER_H

#include <iostream>
#include <fstream>
#include <Utility/Vec2d.hpp>
#include <array> 
#include <limits>
#include <cfloat>

class CircularCollider {
	
	private:
	
		Vec2d position;		
		double rayon;
	
	
	protected:
	
		//methode pour assurer un monde torique accessible aux sous-classes
		void clamping();
		
		
	public:	
		//Constructeurs/destructeur			
		CircularCollider (Vec2d p, double r);					
		CircularCollider (CircularCollider const& autre);		
		~CircularCollider () = default;							 
	
		//getters/setters pour la position et le rayon
		const Vec2d& getPosition() const;					
		const double& getRadius() const;	
		void setPosition(const Vec2d& newPosition);
	
		//operateur= interne de copie 
		CircularCollider& operator=(const CircularCollider& objet);

		
		//GESTION DU DEPLACEMENT
		Vec2d directionTo(const Vec2d& to) const;	
		Vec2d directionTo (const CircularCollider& objet)const; 
		double distanceTo (const Vec2d& to)const; 
		double distanceTo (const CircularCollider& objet)const; 
		void move(const Vec2d& dx);
		CircularCollider& operator+=(const Vec2d& dx);  
		void checkShortestDistance (double& distance, const double& tmp_dist, Vec2d& direction, const Vec2d& tmp_vec) const;
		
		
		//GESTION DES COLLISIONS
		bool operator>(const CircularCollider& body)const;		
		bool operator|(const CircularCollider& body)const;			
		bool operator>(const Vec2d& body)const;
		
		bool isCircularColliderInside(const CircularCollider& other) const;	
		bool isColliding(const CircularCollider& other) const;				
		bool isPointInside(const Vec2d& point) const;
	
	
};
				
    std::ostream& operator<<(std::ostream& sortie, const CircularCollider& objet);  
        
#endif
