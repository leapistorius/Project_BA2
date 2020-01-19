#include "ChasingAutomaton.hpp"
#include <Application.hpp>
#include <Utility/Constants.hpp>

//Getters et setters
double ChasingAutomaton::getStandardMaxSpeed() const
{
    return CHASING_AUTOMATON_MAX_SPEED;
}

double ChasingAutomaton::getMass() const
{
    return CHASING_AUTOMATON_MASS;
}

void ChasingAutomaton::setTargetPosition (const Vec2d& target)
{
    positionCible = target;
    clamping();
}

Vec2d ChasingAutomaton::getSpeedVector() const
{
    return direction*normeVitesse;
}

//Methodes draw
void ChasingAutomaton::draw(sf::RenderTarget& targetWindow) const
{
    drawSprite(targetWindow);
    drawCible(targetWindow);
}

void ChasingAutomaton::drawSprite(sf::RenderTarget& targetWindow) const
{
    sf::Texture& texture = getAppTexture(GHOST_TEXTURE);
    auto image_to_draw(buildSprite(getPosition(), getRadius()*2 ,texture));
    targetWindow.draw(image_to_draw);
}

void ChasingAutomaton::drawCible(sf::RenderTarget& targetWindow) const
{
    targetWindow.draw(buildCircle(positionCible, 5.0, sf::Color(255, 0, 0)));
}

//Constructeurs
ChasingAutomaton::ChasingAutomaton(const Vec2d& position_, double r, double speed)
    : CircularCollider(position_, r), normeVitesse(speed)
{
    positionCible.x = 0.0;
    positionCible.y = 0.0;
    direction.x = 1.0;
    direction.y = 0.0;
}

ChasingAutomaton::ChasingAutomaton(const Vec2d& position_)
    :ChasingAutomaton(position_, CHASING_AUTOMATON_RADIUS, 0.0)
{}

void ChasingAutomaton:: update(sf::Time dt)
{
    Vec2d force(forceAttraction());
    Vec2d acceleration(force/getMass());
    Vec2d newVitesse;
    newVitesse = (direction * normeVitesse) + (acceleration * dt.asSeconds());
    Vec2d newDirection(newVitesse.normalised());

    double maxSpeed(getStandardMaxSpeed());

    if (newVitesse.length()>maxSpeed) {
        newVitesse = newDirection * maxSpeed;
    }

    Vec2d newPosition(getPosition() + (newVitesse * dt.asSeconds()));
    direction=newDirection;
    normeVitesse=newVitesse.length();
    setPosition(newPosition);
}


Vec2d ChasingAutomaton::forceAttraction () const
{
    if (*this>positionCible) {
        Vec2d force(0.0, 0.0);
        return force;
    } else {
        double deceleration (0.6); //deceleration moyenne
        Vec2d toTarget(positionCible-getPosition());

        double speed(std::min(toTarget.length()/deceleration,getStandardMaxSpeed()));
        Vec2d v_target(toTarget.normalised() * speed);

        return v_target - (direction * normeVitesse);
    }

}
