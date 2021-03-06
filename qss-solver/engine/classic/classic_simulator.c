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

#include <functional>
#include <stdlib.h>

#include <classic/classic_simulator.h>
#include <common/integrator.h>
// #include <common/random.h>
#include <common/model.h>

CLC_simulator
CLC_Simulator ()
{
  CLC_simulator p = (CLC_simulator) checkedMalloc (sizeof(*p));
  p->data = NULL;
  p->model = NULL;
  p->output = NULL;
  p->settings = NULL;
  p->simulationLog = NULL;
  p->stats = NULL;
  p->initializeDataStructs = NULL;
  return (p);
}

void
CLC_freeSimulator (CLC_simulator simulator)
{
#ifdef DEBUG
  SD_freeSimulationLog (simulator->simulationLog);
#endif
  SD_freeOutput (simulator->output, simulator->data->states,
		 simulator->data->discretes);
  CLC_freeData (simulator->data);
  CLC_freeModel (simulator->model);
  SD_freeSimulationSettings (simulator->settings);
  SD_freeStatistics (simulator->stats);
  free (simulator);
}

void
CLC_simulate (SIM_simulator simulate)
{
  // Random ();
  CLC_simulator simulator = (CLC_simulator) simulate->state->sim;
  // ((CLC_initializeDataStructs) simulator->initializeDataStructs) (simulator);
  (*simulator->initializeDataStructs) ((void *) simulator);
  INT_integrator integrator = INT_Integrator (simulate);
#ifdef DEBUG
  SD_output output = simulator->output;
  simulator->simulationLog = SD_SimulationLog (output->name);
#endif
  INT_initialize (integrator, simulate);
#ifdef DEBUG
  SD_simulationSettings settings = simulator->settings;
  SD_simulationLog simulationLog = simulator->simulationLog;
  if (settings->debug > 3)
    {
      SD_print (simulationLog, "\nBegin Simulation:");
      SD_print (simulationLog, "\n-----------------");
    }
#endif
  INT_integrate (integrator, simulate);
  INT_freeIntegrator (integrator);
}

void
CLC_simulatorEnd (SIM_simulator simulate)
{
  CLC_simulator simulator = (CLC_simulator) simulate->state->sim;
  CLC_freeSimulator (simulator);
}

void
CLC_initSimulator (SIM_simulator simulator, InitializeDataStructs *ids)
{
  simulator->state->sim = (void*) CLC_Simulator ();
  ((CLC_simulator) simulator->state->sim)->settings =
      simulator->state->settings;
  ((CLC_simulator) simulator->state->sim)->stats = SD_Statistics();
  ((CLC_simulator) simulator->state->sim)->initializeDataStructs = ids;
  simulator->ops->simulate = CLC_simulate;
  simulator->ops->freeSimulator = CLC_simulatorEnd;
}
