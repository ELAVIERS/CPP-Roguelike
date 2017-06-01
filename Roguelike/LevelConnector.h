#pragma once
#include "Entity.h"

class LevelConnector : public Entity {
public:
	LevelConnector();
	~LevelConnector();

	unsigned char connectionID;
	unsigned char direction;

	virtual void GetProperties(PropertySet&) const;
	virtual void SetProperties(const PropertySet&);
};