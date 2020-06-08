#pragma once
#include "../Textures/Texture.h"
#include "RawModel.h"

class TexturedModel
{
	const RawModel& rawModel;
	const Texture& texture;
public:
	TexturedModel(const RawModel& model, const Texture& texture);
	const RawModel& getRawModel() const;
	const Texture& getModelTexture() const;
};

