#ifndef CHASING_AUTOMATON_H
#define CHASING_AUTOMATON_H

#include <iostream>
#include <Utility/Vec2d.hpp>
#include <Obstacle/CircularCollider.hpp>
#include <Utility/Utility.hpp>

class ChasingAutomaton : public CircularCollider
{

public:

    //Getters et setters
    double getStandardMaxSpeed() const;
    double getMass() const;
    void setTargetPosition (const Vec2d& target);
    Vec2d getSpeedVector() const;

    //Methode update
    void update(sf::Time dt);

    //Methode draw
    void draw(sf::RenderTarget& targetWindow) const;

    //Constructeurs
    ChasingAutomaton(const Vec2d& position_, double r, double speed);
    ChasingAutomaton(const Vec2d& position_);
    //Destructeur
    ~ChasingAutomaton() = default;

private:

    //Attributs
    Vec2d direction;
    double normeVitesse;
    Vec2d positionCible;

    //Methodes
    Vec2d forceAttraction () const;
    void drawSprite(sf::RenderTarget& targetWindow) const;
    void drawCible(sf::RenderTarget& targetWindow) const;

};


#endif
