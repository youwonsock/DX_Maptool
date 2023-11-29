#include "pch.h"
#include "Converter.h"

Converter::Converter()
{
	importer = std::make_shared<Assimp::Importer>();
}

Converter::~Converter()
{
}
