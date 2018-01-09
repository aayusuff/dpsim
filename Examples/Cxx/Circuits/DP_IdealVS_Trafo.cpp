/** Reference Circuits
 *
 * @author Markus Mirz <mmirz@eonerc.rwth-aachen.de>
 * @copyright 2017, Institute for Automation of Complex Power Systems, EONERC
 *
 * DPsim
 *
 * This program is free software: you can redistribute it and/or modify
 * it under the terms of the GNU General Public License as published by
 * the Free Software Foundation, either version 3 of the License, or
 * any later version.
 *
 * This program is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 * GNU General Public License for more details.
 *
 * You should have received a copy of the GNU General Public License
 * along with this program.  If not, see <http://www.gnu.org/licenses/>.
 *********************************************************************************/

#include "Simulation.h"
#include "Utilities.h"

using namespace DPsim;

int main(int argc, char* argv[])
{
	// Define simulation scenario
	Real timeStep = 0.001;
	Real omega = 2.0*M_PI*50.0;
	Real finalTime = 0.2;
	std::ostringstream fileName;
	fileName << "IdealVS_Trafo" << timeStep;
	Components::Base::List circElements;
	circElements.push_back(std::make_shared<Components::DP::VoltageSourceIdeal>("v_1", -1, 0, Complex(100, 0), LogLevel::DEBUG, false));
	circElements.push_back(std::make_shared<Components::DP::Transformer>("trafo_1", 0, 1, 10, 0, 0, 0.001, LogLevel::DEBUG, false));
	circElements.push_back(std::make_shared<Components::DP::Resistor>("r_1", 1, -1, 1, LogLevel::DEBUG, false));

	// Define log names
	Logger log("Logs/" + fileName.str() + ".log");
	Logger leftVectorLog("Logs/LeftVector_" + fileName.str() + ".csv");
	Logger rightVectorLog("Logs/RightVector_" + fileName.str() + ".csv");

	// Set up simulation and start main simulation loop
	Simulation newSim(circElements, omega, timeStep, finalTime, log);

	std::cout << "Start simulation." << std::endl;

	while (newSim.step(leftVectorLog, rightVectorLog)) {
		newSim.increaseByTimeStep();
	}

	std::cout << "Simulation finished." << std::endl;
	return 0;
}