#include "optimizer.hpp"
#include "BMAOptimizer.hpp"
#include "gtest/gtest.h"
#include "gmock/gmock.h"
#include <array>
#include "Objective.hpp"
#include "MakeArray.hpp"
#include "TravelingSalesman.hpp"

using namespace testing;


TEST(OptimizerTSPTests, TestEvaluator)
{
	std::array<double, 666> latitudes = {
		90.00, 71.17, 64.51, 61.13, 58.20, 49.16, 53.33, 51.03, 50.25, 52.07,
		49.53, 58.46, 43.39, 45.25, 45.31, 46.49, 44.39, 47.34, 47.36, 47.40,
		38.35, 37.48, 34.03, 32.43, 40.46, 33.27, 39.43, 35.05, 31.45, 46.47,
		44.59, 41.16, 39.07, 35.28, 32.47, 29.46, 43.02, 41.53, 38.39, 35.08,
		29.58, 42.20, 40.26, 39.06, 33.45, 42.21, 40.43, 39.57, 38.54, 30.20,
		25.46, 25.05, 28.38, 25.33, 25.40, 22.13, 22.09, 20.40, 19.24, 19.03,
		19.20, 16.51, 17.03, 17.59, 20.58, 17.30, 14.38, 13.42, 14.06, 12.09,
		9.56, 8.58, 23.08, 22.24, 20.01, 18.00, 18.32, 18.28, 18.28, 14.36,
		13.06, 10.39, 12.06, 4.56, 5.50, 6.48, 10.30, 10.40, 10.59, 6.15,
		4.36, 3.27, -0.56, -0.13, -1.40, -2.10, -3.46, -8.07, -12.03, -13.31,
		-16.24, -16.30, -17.48, -19.35, -23.39, -33.27, -36.50, -53.09, -51.42, -38.43,
		-38.00, -34.50, -34.36, -32.57, -31.24, -32.53, -26.49, -25.16, -30.04, -27.35,
		-25.25, -23.32, -22.54, -20.23, -19.55, -20.27, -15.35, -16.40, -15.47, -12.59,
		-8.03, -5.47, -3.43, -5.05, -2.31, -1.27, -3.08, -8.46, 14.55, 28.06,
		32.38, 31.38, 33.39, 34.02, 34.05, 35.48, 35.43, 36.47, 22.56, 36.22,
		36.48, 34.44, 32.54, 32.07, 31.12, 31.16, 29.58, 30.03, 24.05, 19.37,
		15.36, 13.11, 13.38, 15.20, 9.00, 11.36, 18.06, 14.40, 13.28, 11.51,
		16.46, 12.39, 10.23, 9.31, 8.30, 6.18, 5.19, 6.41, 5.33, 6.08,
		6.29, 12.22, 13.31, 12.00, 11.51, 12.07, 6.27, 6.27, 0.20, 3.45,
		3.52, 4.22, 0.23, -4.16, -4.18, 0.04, -5.54, 0.30, -3.23, -1.57,
		0.19, -1.17, 2.01, -4.03, -6.10, -6.48, -8.48, -12.44, -11.40, -12.49,
		-15.25, -20.09, -17.50, -15.47, -19.49, -25.58, -15.57, -37.15, -22.59, -22.34,
		-26.38, -24.45, -25.45, -26.15, -29.12, -29.55, -33.00, -33.58, -33.55, -23.21,
		-18.55, -12.16, -20.10, -4.38, 37.44, 38.43, 41.11, 37.23, 36.32, 36.43,
		37.13, 37.53, 38.21, 39.28, 41.23, 41.38, 40.24, 41.39, 43.15, 43.22,
		38.54, 39.34, 42.30, 44.50, 43.36, 43.18, 43.42, 43.42, 42.42, 45.50,
		45.26, 45.45, 45.10, 48.24, 48.05, 47.13, 47.23, 49.30, 48.52, 49.15,
		47.19, 48.41, 48.35, 49.36, 50.38, 50.50, 50.38, 51.03, 51.13, 51.26,
		51.55, 52.22, 52.05, 53.13, 50.23, 50.43, 50.50, 51.29, 51.27, 51.30,
		52.30, 53.25, 53.30, 53.23, 53.50, 54.59, 55.57, 55.53, 56.28, 57.10,
		60.09, 62.01, 51.54, 52.40, 53.20, 54.35, 55.00, 64.09, 64.11, 76.34,
		70.40, 68.26, 65.01, 61.30, 60.27, 60.10, 63.25, 60.23, 58.58, 59.55,
		57.43, 55.36, 58.25, 59.20, 57.38, 56.09, 55.24, 55.40, 53.04, 53.33,
		54.20, 54.05, 51.57, 52.24, 52.07, 52.31, 50.47, 50.44, 50.56, 51.12,
		51.17, 51.28, 51.28, 51.32, 51.31, 51.19, 50.58, 51.29, 51.19, 50.50,
		51.03, 49.14, 50.07, 49.25, 49.48, 49.27, 49.03, 48.46, 49.01, 48.08,
		46.12, 46.31, 46.57, 47.33, 47.23, 47.16, 47.48, 48.18, 48.13, 47.05,
		45.03, 45.28, 45.27, 45.27, 45.40, 44.25, 44.29, 43.46, 43.55, 39.20,
		41.54, 40.51, 41.27, 41.07, 40.28, 38.11, 37.30, 38.07, 35.54, 53.24,
		54.23, 53.08, 52.25, 51.46, 52.15, 53.09, 51.06, 50.16, 50.03, 51.15,
		49.45, 50.05, 49.50, 49.12, 48.09, 48.43, 47.30, 47.32, 46.05, 46.15,
		45.45, 46.47, 47.10, 45.48, 45.39, 44.26, 44.11, 46.03, 45.20, 45.48,
		43.31, 43.52, 44.50, 42.38, 41.59, 41.20, 42.41, 42.09, 43.13, 42.30,
		39.36, 40.38, 38.15, 37.58, 35.20, 35.10, 68.58, 64.34, 59.55, 59.25,
		56.57, 54.43, 54.41, 53.54, 49.50, 50.26, 46.28, 55.45, 56.20, 55.45,
		53.12, 51.40, 50.00, 48.27, 44.36, 47.14, 48.44, 46.21, 41.43, 40.11,
		40.23, 58.00, 56.51, 67.27, 69.20, 55.00, 55.02, 56.01, 49.50, 43.15,
		41.20, 39.40, 38.35, 43.48, 52.16, 47.55, 52.03, 62.13, 64.45, 53.01,
		59.34, 50.17, 50.35, 48.27, 46.58, 43.10, 41.01, 38.25, 39.56, 38.43,
		39.45, 39.55, 37.55, 37.01, 36.12, 34.44, 33.30, 33.53, 31.57, 32.50,
		32.04, 31.46, 24.28, 21.30, 21.27, 15.23, 14.48, 12.45, 14.32, 23.37,
		25.18, 25.17, 26.13, 24.38, 29.20, 30.30, 33.21, 35.28, 36.20, 38.05,
		37.16, 35.40, 34.19, 30.20, 32.40, 29.36, 30.17, 36.18, 34.20, 31.32,
		34.31, 33.36, 31.35, 31.25, 30.11, 30.12, 27.42, 25.22, 24.52, 30.19,
		28.40, 26.17, 26.55, 26.28, 25.20, 25.36, 22.32, 23.02, 21.09, 20.30,
		18.58, 17.23, 17.42, 15.21, 12.59, 13.05, 10.49, 9.56, 6.56, 27.43,
		27.28, 23.43, 22.20, 22.00, 16.47, 18.47, 19.52, 17.58, 21.02, 16.28,
		16.04, 10.45, 11.33, 13.45, 5.25, 3.10, 1.17, 3.35, -0.57, -2.55,
		-6.10, -6.54, -7.48, -7.15, -8.39, -10.10, -3.20, 1.33, 4.56, -0.30,
		-5.07, 1.29, -3.43, -5.40, 7.04, 10.18, 10.42, 14.35, 22.17, 22.38,
		25.03, 29.40, 36.03, 34.15, 30.39, 29.39, 25.05, 23.06, 26.06, 30.36,
		32.03, 31.14, 34.48, 36.06, 37.55, 39.08, 39.55, 38.53, 41.48, 45.45,
		39.01, 37.33, 35.06, 43.03, 39.43, 38.15, 35.42, 35.10, 36.34, 35.00,
		34.40, 34.24, 32.48, 31.36, 26.13, 13.28, -2.32, -4.12, -9.30, -12.28,
		-31.56, -34.55, -37.49, -42.53, -33.52, -27.28, -19.16, -23.42, -45.52, -43.32,
		-41.18, -36.52, -21.08, -14.16, -18.08, -22.16, -9.26, -0.32, 11.35, 21.19,
		1.52, -9.45, -17.32, -25.04, -27.07, -90.00,
	};
	std::array<double, 666> longitudes = {
		0.00, -156.47, -147.43, -149.53, -134.27, -123.07, -113.28, -114.05, -104.39, -106.38,
		-97.09, -94.10, -79.23, -75.42, -73.34, -71.14, -63.36, -52.43, -122.20, -117.23,
		-121.30, -122.24, -118.15, -117.09, -111.53, -112.05, -105.01, -106.40, -106.29, -92.06,
		-93.13, -95.57, -94.39, -97.32, -96.48, -95.22, -87.55, -87.38, -90.25, -90.03,
		-90.07, -83.03, -80.00, -84.31, -84.23, -71.04, -74.01, -75.07, -77.01, -81.40,
		-80.12, -77.21, -106.05, -103.26, -100.19, -97.51, -100.59, -103.20, -99.09, -98.12,
		-96.40, -99.55, -96.43, -92.55, -89.37, -88.12, -90.31, -89.12, -87.13, -86.17,
		-84.05, -79.32, -82.22, -79.58, -75.49, -76.48, -72.20, -69.54, -66.07, -61.05,
		-59.37, -61.31, -68.56, -52.20, -55.10, -58.10, -66.56, -71.37, -74.48, -75.35,
		-74.05, -76.31, -91.01, -78.30, -78.38, -79.50, -73.15, -79.02, -77.03, -71.59,
		-71.33, -68.09, -63.10, -65.45, -70.24, -70.40, -73.03, -70.55, -57.51, -62.17,
		-57.33, -56.12, -58.27, -60.40, -64.11, -68.49, -65.13, -57.40, -51.11, -48.34,
		-49.15, -46.37, -43.14, -43.30, -43.56, -54.37, -56.05, -49.16, -47.55, -38.31,
		-34.54, -35.13, -38.30, -42.49, -44.16, -48.29, -60.01, -63.54, -23.31, -15.24,
		-16.54, -8.00, -7.35, -6.51, -4.57, -5.45, -0.43, 3.03, 5.30, 6.37,
		10.11, 10.46, 13.11, 20.04, 29.54, 32.18, 32.33, 31.15, 32.53, 37.14,
		32.32, 30.13, 25.21, 38.53, 38.50, 43.09, -15.57, -17.26, -16.39, -15.35,
		-3.01, -8.00, -9.18, -13.43, -13.15, -10.47, -4.02, -1.35, -0.13, 1.13,
		2.37, -1.31, 2.07, 8.30, 13.10, 15.03, 3.24, 7.27, 6.44, 8.47,
		11.31, 18.35, 9.27, 15.17, 15.18, 18.16, 22.25, 25.12, 29.22, 30.04,
		32.25, 36.49, 45.20, 39.40, 39.11, 39.17, 13.14, 15.47, 27.28, 28.13,
		28.17, 28.36, 31.03, 35.00, 34.52, 32.35, -5.42, -12.30, 14.31, 17.06,
		15.10, 25.55, 28.10, 28.00, 26.07, 30.56, 27.55, 25.40, 18.22, 43.40,
		47.31, 49.17, 57.30, 55.27, -25.40, -9.08, -8.36, -5.59, -6.18, -4.25,
		-3.41, -4.46, -0.29, -0.22, 2.11, -0.53, -3.41, -4.43, -2.58, -8.23,
		1.26, 2.39, 1.31, -0.34, 1.26, 5.24, 7.15, 7.23, 9.27, 1.16,
		4.24, 4.51, 5.43, -4.29, -1.41, -1.33, 0.41, 0.08, 2.20, 4.02,
		5.01, 6.12, 7.45, 6.09, 5.34, 4.20, 3.04, 3.43, 4.25, 5.28,
		4.28, 4.54, 5.08, 6.33, -4.10, -1.54, -0.08, -3.13, -2.35, -0.10,
		-1.50, -2.55, -2.15, -1.30, -1.35, -1.35, -3.13, -4.15, -3.00, -2.04,
		-1.09, -6.46, -8.28, -8.38, -6.15, -5.55, -7.19, -21.51, -51.44, -68.47,
		23.42, 17.25, 25.28, 23.45, 22.17, 24.58, 10.25, 5.20, 5.45, 10.45,
		11.58, 13.00, 15.37, 18.03, 18.18, 10.13, 10.23, 12.35, 8.49, 9.59,
		10.08, 12.07, 7.37, 9.44, 11.38, 13.24, 6.05, 7.05, 6.59, 6.47,
		7.17, 7.01, 7.13, 7.13, 7.28, 9.29, 11.01, 11.58, 12.20, 12.55,
		13.44, 6.59, 8.40, 8.43, 9.56, 11.04, 8.24, 9.11, 12.06, 11.34,
		6.09, 6.38, 7.26, 7.35, 8.32, 11.24, 13.02, 14.18, 16.20, 15.27,
		7.40, 9.12, 11.00, 12.21, 13.46, 8.57, 11.20, 11.15, 12.28, 9.00,
		12.29, 14.17, 15.34, 16.52, 17.15, 15.33, 15.06, 13.21, 14.31, 14.32,
		18.40, 18.00, 16.55, 19.30, 21.00, 23.09, 17.00, 19.00, 19.58, 22.35,
		13.23, 14.26, 18.17, 16.37, 17.07, 21.15, 19.05, 21.38, 18.13, 20.09,
		21.13, 23.36, 27.35, 24.09, 25.37, 26.06, 28.39, 14.31, 14.27, 15.58,
		16.27, 18.25, 20.30, 18.07, 21.26, 19.50, 23.19, 24.45, 27.55, 27.28,
		19.56, 22.56, 21.44, 23.43, 25.09, 33.22, 33.05, 40.32, 30.15, 24.45,
		24.06, 20.30, 25.19, 27.34, 24.00, 30.31, 30.44, 37.35, 44.00, 49.08,
		50.09, 39.10, 36.15, 34.59, 33.32, 39.42, 44.25, 48.03, 44.49, 44.30,
		49.51, 56.15, 60.36, 63.58, 88.06, 73.24, 82.55, 92.50, 73.10, 76.57,
		69.18, 66.48, 68.48, 87.35, 104.20, 106.53, 113.30, 129.49, 177.29, 158.39,
		150.48, 127.32, 137.02, 135.06, 142.42, 131.56, 28.58, 27.09, 32.52, 35.30,
		37.02, 41.17, 40.14, 35.18, 37.10, 36.43, 36.18, 35.30, 35.56, 35.00,
		34.46, 35.14, 39.36, 39.12, 39.49, 44.12, 42.57, 45.12, 49.08, 58.35,
		55.18, 51.32, 50.35, 46.43, 47.59, 47.47, 44.25, 44.28, 43.08, 46.18,
		49.36, 51.26, 47.04, 48.16, 51.38, 52.32, 57.05, 59.36, 62.12, 65.30,
		69.12, 73.04, 74.18, 73.05, 71.29, 67.00, 68.52, 68.22, 67.03, 78.02,
		77.13, 73.02, 75.49, 80.21, 83.00, 85.07, 88.22, 72.37, 79.06, 85.50,
		72.50, 78.29, 83.18, 75.10, 77.35, 80.17, 78.41, 78.07, 79.51, 85.19,
		89.39, 90.25, 91.50, 96.05, 96.10, 98.59, 102.08, 102.36, 105.51, 107.36,
		108.13, 106.40, 104.55, 100.31, 100.20, 101.42, 103.51, 98.40, 100.21, 104.45,
		106.48, 107.36, 110.22, 112.45, 115.13, 123.35, 114.35, 110.20, 114.55, 117.09,
		119.24, 124.51, 128.12, 132.45, 125.36, 123.54, 122.34, 121.00, 114.09, 120.17,
		121.30, 91.09, 103.41, 108.52, 104.04, 106.34, 102.40, 113.16, 119.17, 114.17,
		118.47, 121.28, 113.39, 120.19, 112.30, 117.12, 116.25, 121.35, 123.27, 126.41,
		125.45, 126.58, 129.03, 141.21, 140.07, 140.53, 139.46, 136.55, 136.39, 135.45,
		135.30, 132.27, 129.55, 130.33, 127.40, 144.47, 140.42, 152.12, 147.10, 130.50,
		115.50, 138.35, 144.58, 147.19, 151.13, 153.02, 146.48, 133.53, 170.30, 172.38,
		174.47, 174.46, -175.12, -170.42, 178.25, 166.27, 159.57, 166.55, 165.23, -157.52,
		-157.20, -139.00, -149.34, -130.06, -109.22, 0.00,
	};

	TravelingSalesman<666> salesman(latitudes, longitudes);
	int distance = 0;
	for (int i = 0;i < 666; i++)
	{
		distance += salesman.calculateDistance(i, (i + 1) % 666);
	}
	EXPECT_EQ(423710, distance);
	Keyboard<665> keyboard;
	distance = -static_cast<int>(salesman.evaluate(keyboard));
	EXPECT_EQ(423710, distance);
}

TEST(OptimizerTSPTests, Burma14)
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
	Optimizer<13, 1> o;
	o.populationSize(1);
	o.initialTemperature(591.5637f, 121.418f, 528);
	o.fastCoolingTemperature(349.3616f, 0.5561f, 393);
	TravelingSalesman<14> salesman(latitudes, longitudes);
	auto objectives = { salesman };
	auto& solutions = o.optimize(std::begin(objectives), std::end(objectives), 20000);
	// The reverse direction is also a solution
	EXPECT_THAT(solutions.size(), AnyOf(1, 2));
	auto result = solutions.getResult()[0].m_keyboard;
	int resultValue = static_cast<int>(-std::round(salesman.evaluate(result)));
	EXPECT_EQ(3323, resultValue);
}

TEST(BMAOptimizerTSPTests, Burma14)
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
	BMAOptimizer<13, 1> o;
	o.populationSize(1);
	TravelingSalesman<14> salesman(latitudes, longitudes);
	auto objectives = { salesman };
	auto& solutions = o.optimize(std::begin(objectives), std::end(objectives), 20000);
	// The reverse direction is also a solution
	EXPECT_THAT(solutions.size(), AnyOf(1, 2));
	auto result = solutions.getResult()[0].m_keyboard;
	int resultValue = static_cast<int>(-std::round(salesman.evaluate(result)));
	EXPECT_EQ(3323, resultValue);
}
