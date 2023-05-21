/*
* @author: BL-GS
* @date:   2023/5/6
*/


#include <random>
#include <vector>
#include <gtest/gtest.h>

#include <algorithm/sort/bubble_sort.h>

TEST(BubbleSortTest, BubbleSortRandomTest) {
	std::vector<uint32_t> vec;
	std::default_random_engine rander;

	for (uint32_t i = 0; i < 200; ++i) {
		vec.push_back(rander());
	}

	std::vector<uint32_t> test_vec{vec};
	std::sort(vec.begin(), vec.end());

	algorithm::sort::bubble_sort(test_vec.begin(), test_vec.end());

	for (uint32_t i = 0; i < 200; ++i) {
		EXPECT_EQ(test_vec[i], vec[i]);
	}
}

TEST(BubbleSortTest, BubbleSortSmallTest) {
	std::vector<uint32_t> vec;
	std::default_random_engine rander;

	{
		std::vector<uint32_t> test_vec{vec};
		algorithm::sort::bubble_sort(test_vec.begin(), test_vec.end());
		EXPECT_EQ(test_vec.size(), 0);
	}

	{
		vec.push_back(0);
		std::vector<uint32_t> test_vec{vec};
		algorithm::sort::bubble_sort(test_vec.begin(), test_vec.end());

		EXPECT_EQ(test_vec.size(), 1);
		EXPECT_EQ(test_vec[0], vec[0]);
	}
}