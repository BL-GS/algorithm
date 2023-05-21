/*
* @author: BL-GS
* @date:   2023/5/6
*/


#include <random>
#include <vector>
#include <gtest/gtest.h>

#include <algorithm/string/search.h>

TEST(StringAlgorithmTest, StringAlgorithmRandomSimpleFindTest) {
	std::vector<uint32_t> vec;
	std::default_random_engine rander;

	for (uint32_t i = 0; i < 200; ++i) {
		size_t base_length    = std::max(rander(), 128UL) % 4096UL;
		size_t pattern_length = rander() % 16 + 1;

		std::string base_str; base_str.resize(base_length);
		std::string pattern_str; pattern_str.resize(pattern_length);

		for (auto &c: base_str) {
			c = rander();
		}
		for (auto &c: pattern_str) {
			c = rander();
		}

		auto iter = algorithm::string::simple_find(pattern_str.begin(), pattern_str.end(),
		                                           base_str.begin(), base_str.end());
		auto res_pos = base_str.find(pattern_str);

		if (res_pos == base_str.npos) {
			EXPECT_EQ(iter, base_str.end());
		}
		else {
			EXPECT_EQ(iter - base_str.begin(), res_pos);
		}
	}
}

TEST(StringAlgorithmTest, StringAlgorithmRandomKMPFindTest) {
	std::vector<uint32_t> vec;
	std::default_random_engine rander;

	for (uint32_t i = 0; i < 200; ++i) {
		size_t base_length    = std::max(rander(), 128UL) % 4096UL;
		size_t pattern_length = rander() % 16 + 1;

		std::string base_str; base_str.resize(base_length);
		std::string pattern_str; pattern_str.resize(pattern_length);

		for (auto &c: base_str) {
			c = rander() % ('z' - 'a') + 'a';
		}
		for (auto &c: pattern_str) {
			c = rander() % ('z' - 'a') + 'a';
		}

		auto iter = algorithm::string::kmp_find(pattern_str.begin(), pattern_str.end(),
		                                        base_str.begin(), base_str.end(),
		                                        'a', 'z');
		auto res_pos = base_str.find(pattern_str);

		if (res_pos == std::string::npos) {
			EXPECT_EQ(iter, base_str.end());
		}
		else {
			EXPECT_EQ(iter - base_str.begin(), res_pos);
			if (iter - base_str.begin(), res_pos) {
				algorithm::string::kmp_find(pattern_str.begin(), pattern_str.end(),
				                            base_str.begin(), base_str.end(),
				                            'a', 'z');
			}
		}
	}
}


TEST(StringAlgorithmTest, StringAlgorithmRandomBoyerMooreFindTest) {
	std::vector<uint32_t> vec;
	std::default_random_engine rander;

	for (uint32_t i = 0; i < 200; ++i) {
		size_t base_length    = std::max(rander(), 128UL) % 4096UL;
		size_t pattern_length = rander() % 16 + 1;

		std::string base_str; base_str.resize(base_length);
		std::string pattern_str; pattern_str.resize(pattern_length);

		for (auto &c: base_str) {
			c = rander();
		}
		for (auto &c: pattern_str) {
			c = rander();
		}

		auto iter = algorithm::string::boyer_moore_find(pattern_str.begin(), pattern_str.end(),
		                                        base_str.begin(), base_str.end());
		auto res_pos = base_str.find(pattern_str);

		if (res_pos == std::string::npos) {
			EXPECT_EQ(iter, base_str.end());
		}
		else {
			EXPECT_EQ(iter - base_str.begin(), res_pos);
		}
	}
}