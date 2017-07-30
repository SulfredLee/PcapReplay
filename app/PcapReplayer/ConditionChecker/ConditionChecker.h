#ifndef CONDITIONCHECKER_H
#define CONDITIONCHECKER_H
class ConditionChecker
{
public:
	ConditionChecker();
	~ConditionChecker();

	virtual void ConditionChecker_DataLine(const unsigned char * data, unsigned int MsgLen, const double& arrivalTime) = 0;
};
#endif
