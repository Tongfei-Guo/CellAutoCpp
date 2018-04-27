#ifndef CLASSICMODEL_H_
#define CLASSICMODEL_H_

#include "Model.h"
#include "Cell.h"
#include "CAWorld.h"
#include "CATypes.h"
#include "CAFunctions.h"


CAWorld cyclicmodel(unsigned width, unsigned height, unsigned timestampsize = 1, bool visualizationswitch = false);
CAWorld gameoflifemodel(unsigned width, unsigned height,unsigned timestampsize = 1, bool visualizationswitch = false);


struct ising_param
{
	float beta;
	float J;
    float mu;
    float H ;
};

//recommended param: param.beta = 10, parm.J = 0.2, param.mu = 0.005, param.H = 1
CAWorld isingmodel(unsigned width, unsigned height, ising_param param, unsigned timestampsize = 1, bool visualizationswitch = false);


int lavamodel(unsigned width, unsigned height,unsigned timestampsize = 1, bool visualizationswitch = false);

#endif /* CLASSICMODEL_H_ */
