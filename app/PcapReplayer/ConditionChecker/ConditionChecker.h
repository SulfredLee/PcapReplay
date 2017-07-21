#pragma once

class ConditionChecker
{
public:
	ConditionChecker();
	~ConditionChecker();

	virtual void ConditionChecker_DataLine(const unsigned char * data, unsigned int MsgLen, const double& arrivalTime) = 0;
};