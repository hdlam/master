#ifndef IEXTENSTION_H
#define IEXTENSTION_H

class IExtension
{
public:
	virtual ~IExtension(void) {};
	virtual void init() = 0;
	virtual void update() = 0;
	virtual String name() = 0;
};

#endif
