#include "IdealVoltageSource.h"

using namespace DPsim;

IdealVoltageSource::IdealVoltageSource(std::string name, int src, int dest, Real voltage, Real phase) : BaseComponent(src, dest) {
	this->mName = name;
	this->mVoltageDiffr = voltage*cos(phase);
	this->mVoltageDiffi = voltage*sin(phase);
}

void IdealVoltageSource::applySystemMatrixStamp(DPSMatrix& g, int compOffset, Real om, Real dt) {
	

	if (mNode1 >= 0) {
		g(compOffset-1, mNode1) = -1;
		g(mNode1, compOffset-1) = -1;
		g(2*compOffset-1, mNode1+compOffset) = -1;
		g(mNode1 + compOffset, 2 * compOffset-1) = -1;

	}

	if (mNode2 >= 0) {
		g(compOffset-1, mNode2) = 1;
		g(mNode2, compOffset-1) = 1;
		g(2 * compOffset -1, mNode2 + compOffset) = 1;
		g(mNode2 + compOffset, 2 * compOffset-1) = 1;
	}
	Logger log;
	
	log.Log() << g << std::endl;
	log.WriteLogToFile("test.log");


}

void IdealVoltageSource::applyRightSideVectorStamp(DPSMatrix& j, int compOffset, Real om, Real dt) {
	// Apply matrix stamp for equivalent current source

		j(compOffset - 1, 0) = j(compOffset - 1, 0) + mVoltageDiffr;
		j(2 * compOffset - 1, 0) = j(2 * compOffset - 1, 0) + mVoltageDiffi;


		Logger log2;

		log2.Log() << j << std::endl;
		log2.WriteLogToFile("test2.log");

}

void IdealVoltageSource::step(DPSMatrix& g, DPSMatrix& j, int compOffset, Real om, Real dt, Real t) {
	// Apply matrix stamp for equivalent current source

	j(compOffset - 1, 0) = j(compOffset - 1, 0) + mVoltageDiffr;
	j(2 * compOffset - 1, 0) = j(2 * compOffset - 1, 0) + mVoltageDiffi;

}