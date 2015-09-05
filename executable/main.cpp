#include <iostream>
#include <vector>
#include <string>
#include <array>
#include <regex>
#include "optionparser.h"
#include "Optimizer.hpp"
#include "BMAOptimizer.hpp"
#include "TravelingSalesman.hpp"
#include "mQAP.hpp"
#include "QAP.hpp"


void printError(const char* msg1, const option::Option& opt, const char* msg2)
{
	fprintf(stderr, "ERROR: %s", msg1);
	fwrite(opt.name, opt.namelen, 1, stderr);
	fprintf(stderr, "%s", msg2);
}

option::ArgStatus floatingPoint(const option::Option& option, bool msg)
{
	bool invalid = true;
	try
	{
		if (option.arg)
		{
			std::stof(option.arg);
		}

		invalid = false;
	}
	catch (std::invalid_argument e)
	{
		
	}
	if (!invalid)
	{
		return option::ARG_OK;
	}

	if (msg)
	{
		printError("Option '", option, "' requires a numeric argument\n");
	}
	return option::ARG_ILLEGAL;
}

option::ArgStatus unsignedInteger(const option::Option& option, bool msg)
{
	bool invalid = true;
	try
	{
		if (option.arg)
		{
			std::stoul(option.arg);
		}

		invalid = false;
	}
	catch (std::invalid_argument e)
	{
		
	}
	if (!invalid)
	{
		return option::ARG_OK;
	}

	if (msg)
	{
		printError("Option '", option, "' requires a numeric argument\n");
	}
	return option::ARG_ILLEGAL;
}

option::ArgStatus required(const option::Option& option, bool msg)
{
	if (option.arg)
	{
		return option::ARG_OK;
	}
	else if (msg)
	{
		printError("Option '", option, "' requires a string argument\n");
	}
	return option::ARG_ILLEGAL;
}

enum  optionIndex {
	UNKNOWN, HELP, MAXT, MINT, NUMSTEPS, FAST_MAXT, FAST_MINT, FAST_NUMSTEPS, PARETO_MAXT,
	PARETO_MINT, PARETO_EQUALMULT, NUMEVALUATIONS, POPULATION, TEST, OUTPUT, SEED,
	SHORT_IMPROVEMENT, LONG_IMPROVEMENT,
};

const option::Descriptor usage[] =
{
	{ UNKNOWN,		0, "", "",				option::Arg::None,	"USAGE: keyboardlayout [options]\n\nOptions:" },
	{ HELP,			0, "", "help",			option::Arg::None,	"  --help  \tPrint usage and exit." },
	{ MAXT,			0, "", "max_t",			floatingPoint,		"  --max_t  \tThe maximum temperature" },
	{ MINT,			0, "", "min_t",			floatingPoint,		"  --min_t  \tThe minimum temperature" },
	{ NUMSTEPS,		0, "", "steps",			unsignedInteger,	"  --steps  \tThe number of temperature steps" },
	{ FAST_MAXT,    0, "", "fast_max_t",	floatingPoint,		"  --fast_max_t  \tThe fast cooling maximum temperature" },
	{ FAST_MINT,    0, "", "fast_min_t",	floatingPoint,		"  --fast_min_t  \tThe fast cooling minimum temperature" },
	{ FAST_NUMSTEPS,0, "", "fast_steps",	unsignedInteger,	"  --fast_steps  \tThe number of fast cooling temperature steps" },
	{ PARETO_MAXT,  0, "", "pareto_max_t",	floatingPoint,		"  --pareto_max_t  \tThe pareto cooling maximum temperature" },
	{ PARETO_MINT,  0, "", "pareto_min_t",	floatingPoint,		"  --pareto_min_t  \tThe pareto cooling minimum temperature" },
	{ PARETO_EQUALMULT,0, "", "pareto_equal_multiplier", floatingPoint, "  --pareto_equal_multiplier  \tThe pareto cooling energy multiplier for two equally good solutions" },
	{ NUMEVALUATIONS,0, "", "evaluations",	unsignedInteger,	"  --evaluations  \tThe maximum number of evaluations" },
	{ POPULATION,	0, "", "population",	unsignedInteger,	"  --population  \tThe population size"},
	{ TEST,			0, "", "test",			required,			"  --test  \tThe name of the test to execute" },
	{ OUTPUT,		0, "", "output",		required,			"  --output  \tThe name of the output file" },
	{ SEED,			0, "", "seed",			unsignedInteger,	"  --seed  \tThe random seed" },
	{ SHORT_IMPROVEMENT, 0, "", "short_improvement", unsignedInteger,	"  --short_improvement  \tShort improvement iterations for BMA" },
	{ LONG_IMPROVEMENT,	0, "", "long_improvement", unsignedInteger,	"  --long_improvement  \tLong improvement iterations for BMA" },
	{ 0,0,0,0,0,0 }
};

int burma14(float minT, float maxT, int numSteps, float fast_minT, float fast_maxT, int fast_numSteps, int numEvaluations, unsigned int seed)
{
	std::array<double, 14> latitudes = {
		16.47,
		16.47,
		20.09,
		22.39,
		25.23,
		22.00,
		20.47,
		17.20,
		16.30,
		14.05,
		16.53,
		21.52,
		19.41,
		20.09
	};
	std::array<double, 14> longitudes = {
		96.10,
		94.44,
		92.54,
		93.37,
		97.24,
		96.05,
		97.02,
		96.29,
		97.38,
		98.12,
		97.38,
		95.59,
		97.13,
		94.55
	};
	Optimizer<13, 1> o(seed);
	o.populationSize(1);
	o.initialTemperature(maxT, minT, numSteps);
	o.fastCoolingTemperature(fast_maxT, fast_minT, fast_numSteps);
	TravelingSalesman<14> salesman(latitudes, longitudes);
	auto objectives = { salesman };
	auto& solutions = o.optimize(std::begin(objectives), std::end(objectives), numEvaluations);
	auto result = solutions.getResult()[0].m_keyboard;
	int resultValue = static_cast<int>(-std::round(salesman.evaluate(result)));
	return resultValue;
}

template<size_t NumLocations, size_t NumObjectives>
int mqap_helper(const std::string filename, float minT, float maxT, int numSteps, float fast_minT, float fast_maxT, int fast_numSteps, float pareto_minT, float pareto_maxT, float pareto_equalMultiplier,
	unsigned int numEvaluations, unsigned int population, unsigned int seed, const std::string outputFile)
{
	std::vector<mQAP<NumLocations>> objectives;
	for (size_t i = 0; i < NumObjectives; i++)
	{
		mQAP<NumLocations> objective(filename, i);
		objectives.push_back(objective);
	}
	Optimizer<NumLocations, NumObjectives, 32> o(seed);
	o.populationSize(population);
	o.initialTemperature(maxT, minT, numSteps);
	o.fastCoolingTemperature(fast_maxT, fast_minT, fast_numSteps);
	o.paretoTemperature(pareto_maxT, pareto_minT, pareto_equalMultiplier);
	auto& solutions = o.optimize(std::begin(objectives), std::end(objectives), numEvaluations);
	auto result = solutions.getResult();
	std::ofstream f(outputFile, std::ios::out | std::ios::trunc);
	f << "#" << std::endl;
	for (auto&& r : result)
	{
		for (auto&& o : r.m_solution)
		{
			f << std::setprecision(16) << -o << " ";
		}
		f << std::endl;
	}
	f << "#" << std::endl;
	return 0;
}

int mqap(const std::string filename, float minT, float maxT, int numSteps, float fast_minT, float fast_maxT, int fast_numSteps, float pareto_minT, float pareto_maxT, float pareto_equalMultiplier,
	unsigned int numEvaluations, unsigned int population, unsigned int seed, const std::string outputFile)
{
	auto regex = std::regex("KC(.*)-(.)fl");
	std::smatch match;
	std::regex_search(filename, match, regex);
	int numLocations = std::stoi(match.str(1));
	int numObjectives = std::stoi(match.str(2));
	if (numLocations == 10)
	{
		if (numObjectives == 2)
		{
			return mqap_helper<10, 2>(filename, minT, maxT, numSteps, fast_minT, fast_maxT, fast_numSteps, pareto_minT, pareto_maxT, pareto_equalMultiplier, numEvaluations, population, seed, outputFile);
		}
		else if (numObjectives == 3)
		{
			return mqap_helper<10, 3>(filename, minT, maxT, numSteps, fast_minT, fast_maxT, fast_numSteps, pareto_minT, pareto_maxT, pareto_equalMultiplier, numEvaluations, population, seed, outputFile);
		}
	}
	else if (numLocations == 20)
	{
		if (numObjectives == 2)
		{
			return mqap_helper<20, 2>(filename, minT, maxT, numSteps, fast_minT, fast_maxT, fast_numSteps, pareto_minT, pareto_maxT, pareto_equalMultiplier, numEvaluations, population, seed, outputFile);
		}
		else if (numObjectives == 3)
		{
			return mqap_helper<20, 3>(filename, minT, maxT, numSteps, fast_minT, fast_maxT, fast_numSteps, pareto_minT, pareto_maxT, pareto_equalMultiplier, numEvaluations, population, seed, outputFile);
		}
	}
	else if (numLocations == 30)
	{
		if (numObjectives == 2)
		{
			return mqap_helper<30, 2>(filename, minT, maxT, numSteps, fast_minT, fast_maxT, fast_numSteps, pareto_minT, pareto_maxT, pareto_equalMultiplier, numEvaluations, population, seed, outputFile);
		}
		else if (numObjectives == 3)
		{
			return mqap_helper<30, 3>(filename, minT, maxT, numSteps, fast_minT, fast_maxT, fast_numSteps, pareto_minT, pareto_maxT, pareto_equalMultiplier, numEvaluations, population, seed, outputFile);
		}
	}
	return 0;
}

int qap_bma(const std::string filename, size_t population, size_t shortDepth, size_t longDepth, size_t evaluations, unsigned int seed)
{
	QAP<12> objective(filename);
	Keyboard<12> keyboard;
	BMAOptimizer<12, 1> o(seed);
	o.populationSize(population);
	o.improvementDepth(shortDepth, longDepth);
	auto objectives = { objective };
	auto& solutions = o.optimize(std::begin(objectives), std::end(objectives), evaluations);
	auto result = solutions.getResult()[0].m_keyboard;
	int resultValue = static_cast<int>(-std::round(objective.evaluate(result)));
	return resultValue;
}

template<typename T, bool IsSigned = std::is_signed<T>::value, size_t NumBytes = sizeof(T)>
struct GetArgumentHelper
{
};

template<typename T>
struct GetArgumentHelper<T, true, 4>
{
	T operator()(const char* arg) const
	{
		return std::stol(arg);
	}
};

template<typename T>
struct GetArgumentHelper<T, false, 4>
{
	T operator()(const char* arg) const
	{
		return std::stoul(arg);
	}
};

template<typename T>
struct GetArgumentHelper<T, false, 8>
{
	T operator()(const char* arg) const
	{
		return std::stoull(arg);
	}
};

template<typename T>
struct GetArgumentHelper<T, true, 8>
{
	T operator()(const char* arg) const
	{
		return std::stoll(arg);
	}
};

template<>
struct GetArgumentHelper<float, true, 4>
{
	float operator()(const char* arg) const
	{
		return std::stof(arg);
	}
};

template<>
struct GetArgumentHelper<double, true, 8>
{
	double operator()(const char* arg) const
	{
		return std::stod(arg);
	}
};

template<typename T>
T getArgument(const std::vector<option::Option>& options, size_t index)
{
	if (!options[index])
	{
		throw std::invalid_argument(usage[index].longopt);
	}
	return GetArgumentHelper<T>()(options[index].arg);
}

int main(int argc, char* argv[])
{
	argc -= (argc > 0); argv += (argc > 0); // skip program name argv[0] if present
	option::Stats stats(usage, argc, argv);

	std::vector<option::Option> options(stats.options_max);
	std::vector<option::Option> buffer(stats.buffer_max);
	option::Parser parse(usage, argc, argv, options.data(), buffer.data());

	if (parse.error())
	{
		option::printUsage(std::cout, usage);
		return 1;
	}

	if (options[HELP] || argc == 0)
	{
		option::printUsage(std::cout, usage);
		return 1;
	}

	try
	{
		unsigned int evaluations = getArgument<int>(options, NUMEVALUATIONS);
		unsigned int seed = getArgument<unsigned int>(options, SEED);
		if (options[TEST])
		{
			std::string test = options[TEST].arg;
			std::transform(test.begin(), test.end(), test.begin(), tolower);
			bool isBurma = test == "burma14";
			bool ismqap = test.find("mqap") != -1;
			bool isqap = !ismqap && test.find("qap") != -1;
			if (isBurma || ismqap)
			{
				float minT = getArgument<float>(options, MINT);
				float maxT = getArgument<float>(options, MAXT);
				int steps = getArgument<int>(options, NUMSTEPS);
				float fast_minT = getArgument<float>(options, FAST_MINT);
				float fast_maxT = getArgument<float>(options, FAST_MAXT);
				int fast_steps = getArgument<int>(options, FAST_NUMSTEPS);
				float pareto_minT = getArgument<float>(options, PARETO_MINT);
				float pareto_maxT = getArgument<float>(options, PARETO_MAXT);
				float pareto_equalMultiplier = getArgument<float>(options, PARETO_EQUALMULT);
				if (isBurma)
				{
					auto res = burma14(minT, maxT, steps, fast_minT, fast_maxT, fast_steps, evaluations, seed);
					std::cout << res << std::endl;
				}
				else
				{
					if (!options[OUTPUT])
					{
						std::cout << "An output file is required" << std::endl;
						return 1;
					}
					unsigned int population = getArgument<unsigned int>(options, POPULATION);
					auto res = mqap(test, minT, maxT, steps, fast_minT, fast_maxT, fast_steps, pareto_minT, pareto_maxT, pareto_equalMultiplier, evaluations, population, seed, options[OUTPUT].arg);
					std::cout << res << std::endl;
				}
			}
			else if (test.find("qap") != -1)
			{
				size_t population = getArgument<size_t>(options, POPULATION);
				size_t shortDepth = getArgument<size_t>(options, SHORT_IMPROVEMENT);
				size_t longDepth = getArgument<size_t>(options, LONG_IMPROVEMENT);
				auto res = qap_bma(test, population, shortDepth, longDepth, evaluations, seed);
				std::cout << res << std::endl;

			}
		}
	}
	catch (std::invalid_argument e)
	{
			std::cout << "Option '" << e.what() << "' is required" << std::endl;
			option::printUsage(std::cout, usage);
			return 1;
	}
}