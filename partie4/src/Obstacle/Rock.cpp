#include "Rock.hpp"
#include <Utility/Constants.hpp>
#include <Random/Uniform.hpp>
#include <Application.hpp>

Rock::Rock(Vec2d emplacement)
: CircularCollider (emplacement, uniform(getAppConfig().simulation_world_size/50, 2*getAppConfig().simulation_world_size/50)), orientation (std::max(1.0, uniform(-PI,PI)))
{}

void Rock::draw(sf::RenderTarget& targetWindow) const
{	
    targetWindow.draw(buildSprite(getPosition(), getRadius(), getTexture(), orientation));
}

sf::Texture& Rock::getTexture() const
{
    return getAppTexture(getAppConfig().rock_texture);

}


