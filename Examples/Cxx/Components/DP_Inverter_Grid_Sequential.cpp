/**
 * @author Markus Mirz <mmirz@eonerc.rwth-aachen.de>
 * @copyright 2017-2018, Institute for Automation of Complex Power Systems, EONERC
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

#include <DPsim.h>

using namespace DPsim;
using namespace CPS::DP;
using namespace CPS::DP::Ph1;

int main(int argc, char* argv[]) {
	CommandLineArgs args(argc, argv);
	std::cout << "Simulate sequence " << Int(args.options["seq"]) << std::endl;

	// Define simulation scenario
	Real timeStep = 0.000001;
	Real finalTime = 0.05;
	String simName = "DP_Inverter_Grid_Sequential_s" + std::to_string(Int(args.options["seq"]));
	Logger::setLogDir("logs/"+simName);

	// Set system frequencies
	Matrix frequencies(5,1);
	frequencies << 50, 19850, 19950, 20050, 20150;
	//Matrix frequencies(9,1);
	//frequencies << 50, 19850, 19950, 20050, 20150, 39750, 39950, 40050, 40250;

	// Nodes
	auto n1 = Node::make("n1");
	auto n2 = Node::make("n2");
	auto n3 = Node::make("n3");
	auto n4 = Node::make("n4");
	auto n5 = Node::make("n5");

	Logger::Level level = Logger::Level::off;

	// Components
	auto inv = Inverter::make("inv", level);
	inv->setParameters(2, 3, 360, 0.87);
	auto r1 = Resistor::make("r1", level);
	r1->setParameters(0.1);
	auto l1 = Inductor::make("l1", level);
	l1->setParameters(600e-6);
	auto r2 = Resistor::make("r2", level);
	Real r2g = 0.1+0.001;
	r2->setParameters(r2g);
	auto l2 = Inductor::make("l2", level);
	Real l2g = 150e-6+0.001/(2.*PI*50.);
	l2->setParameters(l2g);
	auto c1 = Capacitor::make("c1", level);
	c1->setParameters(10e-6);
	auto rc = Capacitor::make("rc", level);
	rc->setParameters(1e-6);
	auto grid = VoltageSource::make("grid", level);
	grid->setParameters(Complex(0, -311.1270));

	// Topology
	inv->connect({ n1 });
	r1->connect({ n1, n2 });
	l1->connect({ n2, n3 });
	c1->connect({ Node::GND, n3 });
	rc->connect({ Node::GND, n3 });
	r2->connect({ n3, n4 });
	l2->connect({ n4, n5 });
	grid->connect({ Node::GND, n5 });

	// Define system topology
	auto sys = SystemTopology(50, frequencies,
		SystemNodeList{ n1, n2, n3, n4, n5 },
		SystemComponentList{ inv, r1, l1, r2, l2, c1, rc, grid });

	Simulation sim(simName, level);
	sim.setSystem(sys);
	sim.setTimeStep(timeStep);
	sim.setFinalTime(finalTime);
	sim.doHarmonicParallelization(false);

	sim.run();
	sim.logStepTimes(simName + "_step_times");
}
