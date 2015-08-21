#ifndef __WEIGHTCALCULATIONS_H__
#define __WEIGHTCALCULATIONS_H__

namespace Algorithm
{
	/**
	Functions to be passed as parameters
	*/
	class WeightCalculations
	{
	public:
		WeightCalculations();
		~WeightCalculations();

		static void CellWeight(float confidence, float sideConfidence, float stepsToGoal);
	private:

	};
		
}

#endif