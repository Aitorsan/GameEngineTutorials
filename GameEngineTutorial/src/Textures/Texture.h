#pragma once
class Texture
{
	unsigned int textureID;
	unsigned int activeSamplerLocation;
public:
	Texture(unsigned int textureid, unsigned int activesamplerID);
	unsigned int getID()const ;
	unsigned int getActiveSampler()const;
	
};

