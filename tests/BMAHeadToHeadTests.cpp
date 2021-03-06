#include "gtest/gtest.h"
#include "gmock/gmock.h"
#include "BMAOptimizer.hpp"
#include "BMAReference.hpp"
#include "BMAOptimizerPrev.hpp"
#include "QAP.hpp"
#include <time.h>
#include "boost/accumulators/accumulators.hpp"
#include "boost/accumulators/statistics/median.hpp"

using namespace testing;

TEST(BMAHeadToHeadTests, Tai30a)
{
	std::cout.imbue(std::locale("en-gb"));
	using namespace boost::accumulators;
	std::string filename = "../../tests/QAPData/tai30a.dat";
	QAP<30, float> objective(filename);
	size_t numBetter = 0;
	const size_t bestOf = 50;
	const int target = 1818146;
	size_t totalEvaluationsO = 0;
	size_t totalEvaluationsR = 0;
	accumulator_set<double, features<tag::median>> accO;
	accumulator_set<double, features<tag::median>> accR;
	for (size_t i = 0; i < bestOf; i++)
	{
		clock_t before = clock();
		Keyboard<30> keyboard;
		BMAOptimizer<30> o; 
		o.crossover(CrossoverType::PartiallyMatched);
		o.jumpMagnitude(0.33265849806193115f);
		o.improvementDepth(719);
		o.perturbType(PerturbType::Normal);
		o.minDirectedPertubation(0.06737968529264693f);
		o.populationSize(6);
		o.stagnation(322, 1.677511985917598f, 9.721076484938884f);
		o.tabuTenure(0.3088725545878326f, 1.8264393766550164f);
		o.mutation(12, 0.6967257822466718f, 13);
		o.tournamentPool(6);
		o.primarilyEvolution(false);
		o.target(-target);
		auto& solution = o.optimize(objective, 200000000);
		float t = static_cast<float>((clock() - before) / static_cast<double>(CLOCKS_PER_SEC));
		std::cout << "BMAOptimizer time " << t << std::endl;
		int resultValue = static_cast<int>(-std::round(std::get<0>(solution)));
		EXPECT_EQ(target, resultValue);

		BMAReference r;
		r.run(target, filename, "", 1);
		EXPECT_EQ(target, r.getLastResult());
		if (resultValue != r.getLastResult())
		{
			if (resultValue < r.getLastResult())
			{
				numBetter++;
			}
		}
		else if (o.getNumEvaluations() <= r.getNumEvaluations())
		{
			numBetter++;
		}
		std::cout << "Num evaluations " << o.getNumEvaluations() << " vs " << r.getNumEvaluations() << std::endl;
		totalEvaluationsO += o.getNumEvaluations();
		totalEvaluationsR += r.getNumEvaluations();
		accO(static_cast<double>(o.getNumEvaluations()));
		accR(static_cast<double>(r.getNumEvaluations()));
	}
	std::cout << "Total evaluations " << totalEvaluationsO << " vs " << totalEvaluationsR << std::endl;
	std::cout << "Result " << numBetter << " vs " << bestOf - numBetter << std::endl;
	std::cout << "Median " << static_cast<uint64_t>(median(accO)) << " vs " << static_cast<uint64_t>(median(accR)) << std::endl;
	EXPECT_GT(numBetter, bestOf - numBetter);
	EXPECT_LE(totalEvaluationsO, totalEvaluationsR);
}

TEST(BMAHeadToHeadTests, Tai30aToPrev)
{
	std::cout.imbue(std::locale("en-gb"));
	using namespace boost::accumulators;
	std::string filename = "../../tests/QAPData/tai30a.dat";
	QAP<30, float> objective(filename);
	size_t numBetter = 0;
	const size_t bestOf = 50;
	const int target = 1818146;
	size_t totalEvaluationsO = 0;
	size_t totalEvaluationsR = 0;
	accumulator_set<double, features<tag::median>> accO;
	accumulator_set<double, features<tag::median>> accR;
	for (size_t i = 0; i < bestOf; i++)
	{
		clock_t before = clock();
		Keyboard<30> keyboard;
		BMAOptimizer<30> o; 
		o.crossover(CrossoverType::PartiallyMatched);
		o.jumpMagnitude(0.33265849806193115f);
		o.improvementDepth(719);
		o.perturbType(PerturbType::Normal);
		o.minDirectedPertubation(0.06737968529264693f);
		o.populationSize(6);
		o.stagnation(322, 1.677511985917598f, 9.721076484938884f);
		o.tabuTenure(0.3088725545878326f, 1.8264393766550164f);
		o.mutation(12, 0.6967257822466718f, 13);
		o.tournamentPool(6);
		o.primarilyEvolution(false);
		o.target(-target);
		auto& solution = o.optimize(objective, 200000000);
		float t = static_cast<float>((clock() - before) / static_cast<double>(CLOCKS_PER_SEC));
		std::cout << "BMAOptimizer time " << t << std::endl;
		int resultValue = static_cast<int>(-std::round(std::get<0>(solution)));
		EXPECT_EQ(target, resultValue);

		before = clock();
		BMAOptimizerPrev<30> r;
		r.crossover(CrossoverType::PartiallyMatched);
		r.jumpMagnitude(0.33265849806193115f);
		r.improvementDepth(719);
		r.perturbType(PerturbType::Normal);
		r.minDirectedPertubation(0.06737968529264693f);
		r.populationSize(6);
		r.stagnation(322, 1.677511985917598f, 9.721076484938884f);
		r.tabuTenure(0.3088725545878326f, 1.8264393766550164f);
		r.mutation(12, 0.6967257822466718f, 13);
		r.tournamentPool(6);
		r.primarilyEvolution(false);
		r.target(-target);
		auto& solutionR = r.optimize(objective, 200000000);
		t = static_cast<float>((clock() - before) / static_cast<double>(CLOCKS_PER_SEC));
		std::cout << "BMAOptimizer reference time " << t << std::endl;
		int resultValueR = static_cast<int>(-std::round(std::get<0>(solutionR)));
		EXPECT_EQ(target, resultValueR);

		if (resultValue != resultValueR)
		{
			if (resultValue < resultValueR)
			{
				numBetter++;
			}
		}
		else if (o.getNumEvaluations() <= r.getNumEvaluations())
		{
			numBetter++;
		}
		std::cout << "Num evaluations " << o.getNumEvaluations() << " vs " << r.getNumEvaluations() << std::endl;
		totalEvaluationsO += o.getNumEvaluations();
		totalEvaluationsR += r.getNumEvaluations();
		accO(static_cast<double>(o.getNumEvaluations()));
		accR(static_cast<double>(r.getNumEvaluations()));
	}
	std::cout << "Total evaluations " << totalEvaluationsO << " vs " << totalEvaluationsR << std::endl;
	std::cout << "Result " << numBetter << " vs " << bestOf - numBetter << std::endl;
	std::cout << "Median " << static_cast<uint64_t>(median(accO)) << " vs " << static_cast<uint64_t>(median(accR)) << std::endl;
	EXPECT_GT(numBetter, bestOf - numBetter);
	EXPECT_LE(totalEvaluationsO, totalEvaluationsR);
}
