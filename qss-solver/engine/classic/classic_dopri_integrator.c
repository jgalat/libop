/*****************************************************************************

 This file is part of QSS Solver.

 QSS Solver is free software: you can redistribute it and/or modify
 it under the terms of the GNU General Public License as published by
 the Free Software Foundation, either version 3 of the License, or
 (at your option) any later version.

 QSS Solver is distributed in the hope that it will be useful,
 but WITHOUT ANY WARRANTY; without even the implied warranty of
 MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 GNU General Public License for more details.

 You should have received a copy of the GNU General Public License
 along with QSS Solver.  If not, see <http://www.gnu.org/licenses/>.

 ******************************************************************************/

#include <classic/classic_dopri_integrator.h>

#include <stdio.h>
#include <stdlib.h>

#include "../common/data.h"
#include "../common/simulator.h"
#include "../common/utils.h"
#include "classic_data.h"
#include "classic_integrator.h"
#include "classic_simulator.h"
#include "dopri5/dopri5.h"

struct simData_DOPRI
{
  double **solution;
  double *solution_time;
  double *x;
  double *t;
  unsigned long *totalOutputSteps;
  double last_step;
  double step_size;
  double *temp_x;
  double final_time;
  int size;
  Dopri5 *dopri5;
};

void
DOPRI_events (int n, double x, double *y, double *g,
  void *integrator_data)
{
  CLC_integratorData id = (CLC_integratorData) integrator_data;
  CLC_data clcData = id->clcData;
  CLC_model clcModel = id->clcModel;
  double out;
  (*clcModel->events->zeroCrossing) (n, y, clcData->d, clcData->alg, x, &out);
  g[0] = out + clcData->event[n].zcSign * HIST;
}

void
DOPRI_model (unsigned n, double x, double *y, double *f,
  void *integrator_data)
{
  CLC_integratorData id = (CLC_integratorData) integrator_data;
  CLC_data clcData = id->clcData;
  CLC_model clcModel = id->clcModel;
  clcData->funEvaluations++;
  (*clcModel->f) (y, clcData->d, clcData->alg, x, f);
}

void
DOPRI_solout (long nr, double xold, double x, double* y, unsigned n, int* irtrn,
	      int *jroot, void *integrator_data)
{
  CLC_integratorData id = (CLC_integratorData) integrator_data;
  CLC_model clcModel = id->clcModel;
  CLC_data clcData = id->clcData;
  SD_output simOutput = id->simOutput;
  struct simData_DOPRI *simDataDopri = (struct simData_DOPRI*) id->simData;
  //const double _ft = clcData->ft;
#ifdef SYNC_RT
  /* Sync */
  waitUntil(x);
#endif
  if (irtrn[0] == 3)
    CLC_handle_event (clcData, clcModel, y, jroot, x, NULL);
  if (!id->is_sampled) {
      clcData->totalSteps++;
      CLC_save_step (simOutput, simDataDopri->solution,
		     simDataDopri->solution_time, x,
		     simDataDopri->totalOutputSteps[0], y, clcData->d,
		     clcData->alg);
      simDataDopri->totalOutputSteps[0]++;
  } else { // Do sample
      while (simDataDopri->last_step+simDataDopri->step_size<x) {
        // Skip last step
        if (fabs(simDataDopri->last_step+simDataDopri->step_size-simDataDopri->final_time)/simDataDopri->step_size < 1) {
          break;
        }
        clcData->totalSteps++;
        int i;
        for (i=0;i< simDataDopri->size; i++)
          simDataDopri->temp_x[i] = simDataDopri->dopri5->contd5(i,
            simDataDopri->last_step+simDataDopri->step_size);
        CLC_save_step (simOutput, simDataDopri->solution,
		       simDataDopri->solution_time, simDataDopri->last_step+simDataDopri->step_size,
		       simDataDopri->totalOutputSteps[0], simDataDopri->temp_x, clcData->d,
		       clcData->alg);
        simDataDopri->totalOutputSteps[0]++;
        simDataDopri->last_step+=simDataDopri->step_size;
      }
  }
}

void
DOPRI_integrate (SIM_simulator simulate)
{
  CLC_simulator simulator = (CLC_simulator) simulate->state->sim;

  CLC_data clcData = simulator->data;
  CLC_model clcModel = simulator->model;
  SD_output simOutput = simulator->output;

  struct simData_DOPRI simDataDopri;

  int i;
  unsigned long totalOutputSteps = 0;
  double t = clcData->it;
  const double _ft = clcData->ft;
  double dQRel = clcData->dQRel[0];
  double dQMin = clcData->dQMin[0];
  double *_x = clcData->x;
  double step_size = 1;
  int is_sampled = simOutput->commInterval != CI_Step;
  if (is_sampled)
    {
      step_size = simOutput->sampled->period[0];
    }
  const int num_steps = (is_sampled ? _ft / step_size + 1 : MAX_OUTPUT_POINTS);
  double **solution = (double**) malloc (sizeof(double*) * simOutput->outputs);
  double *solution_time = (double*) malloc (sizeof(double) * num_steps);
  int *jroot = (int*) malloc (sizeof(int) * clcData->events);
  double troot = 0;
  int size = clcData->states, res;
  double *x, *temp_x, rel_tol = dQRel, abs_tol = dQMin;
  x = (double*) checkedMalloc (sizeof(double) * clcData->states);
  temp_x = (double*) checkedMalloc (sizeof(double) * clcData->states);
  CLC_compute_outputs (simOutput, solution, num_steps);
  for (i = 0; i < clcData->states; i++)
    x[i] = _x[i];
  simDataDopri.solution = solution;
  simDataDopri.solution_time = solution_time;
  simDataDopri.x = x;
  simDataDopri.temp_x = temp_x;
  simDataDopri.totalOutputSteps = &totalOutputSteps;
  simDataDopri.step_size = step_size;
  simDataDopri.last_step = t;
  simDataDopri.final_time = _ft ;
  simDataDopri.size = clcData->states;
  getTime (simulator->stats->sTime);
  if (is_sampled) {
    CLC_save_step (simOutput, solution, solution_time, t, totalOutputSteps,
		     x, clcData->d, clcData->alg);
    totalOutputSteps++;
  }
#ifdef SYNC_RT
  setInitRealTime();
#endif

  CLC_integratorData integrator_data = CLC_IntegratorData(clcModel, clcData, simOutput,
                            (void*) &simDataDopri, is_sampled);

  Dopri5 *dopri5 = new Dopri5((void*) integrator_data);

  simDataDopri.dopri5 = dopri5;

  res = dopri5->run(size, DOPRI_model, t, x, _ft , &rel_tol, &abs_tol, 0,
		    DOPRI_solout, 2,
		    stdout,
		    1e-30, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 2000000000, 0, -1,
		    size,
		    NULL,
		    0, DOPRI_events, clcData->events, jroot, &troot);

  if (res < 0)
	{
#ifdef DEBUG
	  SD_print (simulator->simulationLog,
		    "DOPRI failed with res=%d at %g\n", res, t);
#endif
	  printf ("DOPRI failed with res=%d at %g\n", res, t);
	}
#ifdef SYNC_RT
  /* Sync */
  waitUntil(t);
#endif

  if (is_sampled)
	{
	  if (totalOutputSteps < (unsigned) num_steps)
	    {
	      CLC_save_step (simOutput, solution, solution_time, _ft,
			     totalOutputSteps, x, clcData->d, clcData->alg);
	      totalOutputSteps++;
	    }
	}
  getTime (simulator->stats->sTime);
  subTime (simulator->stats->sTime, simulator->stats->iTime);
#ifdef DEBUG
  if (simulator->settings->debug == 0 || simulator->settings->debug > 1)
    {
      SD_print (simulator->simulationLog, "Simulation time (DOPRI):");
      SD_print (simulator->simulationLog, "----------------");
      SD_print (simulator->simulationLog, "Miliseconds: %g",
		getTimeValue (simulator->stats->sTime));
      SD_print (simulator->simulationLog, "Scalar function evaluations: %d",
		clcData->scalarEvaluations);
      SD_print (simulator->simulationLog, "Individual Zero Crossings : %d",
		clcData->zeroCrossings);
      SD_print (simulator->simulationLog, "Function evaluations: %llu",
		clcData->funEvaluations);
      //printLog("Function evaluations (reported by DASSL): %d",totalStepsDASSL);
      //printLog("Zero crossing evaluations : %d", totalCrossingEvaluations );
      SD_print (simulator->simulationLog, "Output steps: %d", totalOutputSteps);
      SD_print (simulator->simulationLog, "Simulation steps: %d", dopri5->nstepRead ());
      SD_print (simulator->simulationLog, "Simulation steps (accepted) : %d",
		dopri5->naccptRead ());
      SD_print (simulator->simulationLog, "Simulation steps (rejected) : %d",
		dopri5->nrejctRead ());
      SD_print (simulator->simulationLog,
		"Function evaluations (reported by DOPRI): %d", dopri5->nfcnRead ());
      SD_print (simulator->simulationLog, "Events detected : %d",
		clcData->totalEvents);
    }
#endif
  //CLC_write_output (simOutput, solution, solution_time, totalOutputSteps);
  free (x);
  free (temp_x);
  free (solution_time);
  free (jroot);
  delete dopri5;
  SD_exportSolution(solution, totalOutputSteps, simOutput->outputs,
    simOutput->solution);
  for (i = 0; i < simOutput->outputs; i++)
    {
      free (solution[i]);
    }
  free (solution);
  free (integrator_data);
}
