#include "TexturedModel.h"


TexturedModel::TexturedModel(const RawModel& model, const Texture& texture)
	: rawModel{model}
	, texture{texture}
{
}

const RawModel& TexturedModel::getRawModel()const
{
	return rawModel;
}
const Texture& TexturedModel::getModelTexture() const
{
	return texture;
}