#include "Texture.h"


Texture::Texture(unsigned int id, unsigned int activesamplerLoc) 
	: textureID{id}
	, activeSamplerLocation{activesamplerLoc}
{
}

unsigned int Texture::getID() const
{
	return textureID;
}

unsigned int Texture::getActiveSampler()const 
{
	return activeSamplerLocation;
}