#include <functional>

#include <qss-solver/engine/common/utils.h>
#include <qss-solver/engine/common/model.h>
#include <qss-solver/engine/common/data.h>
#include <qss-solver/engine/classic/classic_data.h>

#include <qss-solver/engine/common/engine.h>

#include "black_scholes_model.h"

BlackScholesModel::BlackScholesModel() {
  solver = SD_DOPRI;
  ft = 1.0;
  dqmin = 1e-12;
  dqrel = 1e-9;
}

BlackScholesModel::~BlackScholesModel() {}

void BlackScholesModel::settings(SD_simulationSettings settings) {
  settings->debug = SD_DBG_NoDebug;
  settings->parallel = FALSE;
  settings->hybrid = TRUE;
  settings->method = SD_DOPRI;
}

void BlackScholesModel::definition(double *x, double *d, double *alg, double t, double *dx) {
  int i = 0;
	modelData->scalarEvaluations++;
		alg[0] = 5.000000000000000000000000e-01*pow(__PAR_sigma,2.0)*pow(d[99],2.0)*(x[1]-2.0*x[0]+__PAR_u0)/__PAR_ds2+(__PAR_r-__PAR_d)*5.000000000000000000000000e-01*d[99]*(x[1]-__PAR_u0)/__PAR_ds-__PAR_r*x[0];
		dx[0] = d[0]*alg[0];
		alg[98] = 5.000000000000000000000000e-01*pow(__PAR_sigma,2.0)*pow(d[197],2.0)*(__PAR_uN1-2.0*x[98]+x[97])/__PAR_ds2+(__PAR_r-__PAR_d)*d[197]*5.000000000000000000000000e-01*(__PAR_uN1-x[97])/__PAR_ds-__PAR_r*x[98];
		dx[98] = d[98]*alg[98];
	for(i = 1; i <= 97; i++)
	{
		alg[(i) * 1] = 5.000000000000000000000000e-01*pow(__PAR_sigma,2.0)*pow(d[(i+99)],2.0)*(x[(i+1) * 1]-2.0*x[(i) * 1]+x[(i-1) * 1])/__PAR_ds2+(__PAR_r-__PAR_d)*d[(i+99)]*5.000000000000000000000000e-01*(x[(i+1) * 1]-x[(i-1) * 1])/__PAR_ds-__PAR_r*x[(i) * 1];
		dx[i] = d[(i)]*alg[(i) * 1];
	}
}

void BlackScholesModel::zeroCrossing(int i, double *x, double *d, double *alg, double t, double *zc) {
  int j0 = 0;
	modelData->zeroCrossings++;
	if(i >= 0 && i <= 98)
	{
		alg[0] = 5.000000000000000000000000e-01*pow(__PAR_sigma,2.0)*pow(d[99],2.0)*(x[1]-2.0*x[0]+__PAR_u0)/__PAR_ds2+(__PAR_r-__PAR_d)*5.000000000000000000000000e-01*d[99]*(x[1]-__PAR_u0)/__PAR_ds-__PAR_r*x[0];
		j0 = i;
	if (j0 >= 1 && j0 <= 97)
	{
		alg[(i) * 1] = 5.000000000000000000000000e-01*pow(__PAR_sigma,2.0)*pow(d[(j0+99)],2.0)*(x[(j0+1) * 1]-2.0*x[(j0) * 1]+x[(j0-1) * 1])/__PAR_ds2+(__PAR_r-__PAR_d)*d[(j0+99)]*5.000000000000000000000000e-01*(x[(j0+1) * 1]-x[(j0-1) * 1])/__PAR_ds-__PAR_r*x[(j0) * 1];
		}
		alg[98] = 5.000000000000000000000000e-01*pow(__PAR_sigma,2.0)*pow(d[197],2.0)*(__PAR_uN1-2.0*x[98]+x[97])/__PAR_ds2+(__PAR_r-__PAR_d)*d[197]*5.000000000000000000000000e-01*(__PAR_uN1-x[97])/__PAR_ds-__PAR_r*x[98];
		zc[0] = alg[(i) * 1]-(0.0);
	}
}

void BlackScholesModel::handlerPos(int i, double *x, double *d, double *alg, double t) {
  if(i >= 0 && i <= 98)
	{
		d[(i)] = 1.0;
	}
}

void BlackScholesModel::handlerNeg(int i, double *x, double *d, double *alg, double t) {
  if(i >= 0 && i <= 98)
	{
		d[(i)] = 0.0;
	}
}

void BlackScholesModel::output(int i, double *x, double *d, double *alg, double t, double *out) {
  int j = 0;
	j = i;
	if(j >=0 && j <= 98)
	{
		out[0] = x[(j) * 1];
	}
}

void BlackScholesModel::initializeDataStructs(void *simulator_) {
  CLC_simulator simulator = (CLC_simulator) simulator_;
	int i = 0;
	int i1;
	int i2;
	int outputs[99];
	int states[99];
	simulator->data = CLC_Data(99,198,99,0,99,solver, ft, dqmin, dqrel);
  modelData = simulator->data;

	// Allocate main data structures.
	__PAR_Smax = 100.0;
	__PAR_sigma = 4.000000000000000000000000e-01;
	__PAR_r = 0.09;
	__PAR_K = 20.0;
	__PAR_d = 0.1;
	__PAR_u0 = 0.0;
	__PAR_uN1 = __PAR_Smax-__PAR_K;
	__PAR_ds = __PAR_Smax/(99+1.0);
	__PAR_ds2 = pow(__PAR_ds,2.0);
	// Initialize model code.
	for(i1 = 0; i1 <= 98; i1++)
	{
	if((i1+1)*__PAR_ds>__PAR_K)
	{
		modelData->x[(i1) * 1] = (i1+1)*__PAR_ds-__PAR_K;
	}
	else
	{
		modelData->x[(i1) * 1] = 0.0;
	}
		modelData->d[(i1+99)] = (i1+1)*__PAR_ds;
	}
	if(5.000000000000000000000000e-01*pow(__PAR_sigma,2.0)*pow(modelData->d[(99)],2.0)*(modelData->x[1]-2.0*modelData->x[0]+__PAR_u0)/__PAR_ds2+(__PAR_r-__PAR_d)*5.000000000000000000000000e-01*modelData->d[(99)]*(modelData->x[1]-__PAR_u0)/__PAR_ds-__PAR_r*modelData->x[0]>=0.0)
	{
		modelData->d[(0)] = 1.0;
	}
	for(i2 = 1; i2 <= 97; i2++)
	{
	if(5.000000000000000000000000e-01*pow(__PAR_sigma,2.0)*pow(modelData->d[(i2+99)],2.0)*(modelData->x[(i2+1) * 1]-2.0*modelData->x[(i2) * 1]+modelData->x[(i2-1) * 1])/__PAR_ds2+(__PAR_r-__PAR_d)*modelData->d[(i2+99)]*5.000000000000000000000000e-01*(modelData->x[(i2+1) * 1]-modelData->x[(i2-1) * 1])/__PAR_ds-__PAR_r*modelData->x[(i2) * 1]>=0.0)
	{
		modelData->d[(i2)] = 1.0;
	}
	}
	if(5.000000000000000000000000e-01*pow(__PAR_sigma,2.0)*pow(modelData->d[(197)],2.0)*(__PAR_uN1-2.0*modelData->x[98]+modelData->x[97])/__PAR_ds2+(__PAR_r-__PAR_d)*modelData->d[(197)]*5.000000000000000000000000e-01*(__PAR_uN1-modelData->x[97])/__PAR_ds-__PAR_r*modelData->x[98]>=0.0)
	{
		modelData->d[(98)] = 1.0;
	}
	for(i = 0; i <= 98; i++)
	{
		modelData->event[i].direction = 0;
	}
		double period[1];
	period[0] = 0.00273973;

  using namespace std::placeholders;

  SD_eq bsmo = std::bind(&BlackScholesModel::output, this, _1, _2, _3, _4, _5, _6);

  simulator->output = SD_Output("bs",99,198,99,period,1,0,CI_Sampled,SD_Memory,bsmo);
	SD_output modelOutput = simulator->output;

	for(i = 0; i <= 98; i++)
	{
		modelOutput->nOS[i] = 1;
		modelOutput->nSO[i]++;
	}
	SD_allocOutputMatrix(modelOutput,99,198);
	cleanVector(states,0,99);

	cleanVector(outputs,0,99);

	for(i = 0; i <= 98; i++)
	{
		sprintf(modelOutput->variable[i].name,"u[%d]",i+1);
	}
		cleanVector(outputs,0,99);

	for(i = 0; i <= 98; i++)
	{
		modelOutput->SO[i][states[i]++] = i;
		modelOutput->OS[i][outputs[i]++] = i;
	}

  CLC_eq bsmf = std::bind(&BlackScholesModel::definition, this, _1, _2, _3, _4, _5);
  CLC_zc bsmzc = std::bind(&BlackScholesModel::zeroCrossing, this, _1, _2, _3, _4, _5, _6);
  CLC_hnd bsmhp = std::bind(&BlackScholesModel::handlerPos, this, _1, _2, _3, _4, _5),
          bsmhn = std::bind(&BlackScholesModel::handlerNeg, this, _1, _2, _3, _4, _5);

	simulator->model = CLC_Model(bsmf,bsmzc,bsmhp,bsmhn);
}

void BlackScholesModel::testrun() {
  using namespace std::placeholders;
  MOD_settings bsms = std::bind(&BlackScholesModel::settings, this, _1);
  InitializeDataStructs bsmids = std::bind(&BlackScholesModel::initializeDataStructs,
                                            this, _1);
  engine(bsms, bsmids);
}

/* C */

black_scholes_model new_black_scholes_model() {
  black_scholes_model m = reinterpret_cast<black_scholes_model>(new BlackScholesModel());
  return m;
}

void black_scholes_model_testrun (black_scholes_model m) {
  reinterpret_cast<BlackScholesModel*>(m)->testrun();
}
