#pragma once
class Converter
{
private:
	std::shared_ptr<Assimp::Importer> importer;

public:
	Converter();
	~Converter();
};

