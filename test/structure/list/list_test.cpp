/*
 * @author: BL-GS 
 * @date:   2023/5/6
 */


#include <gtest/gtest.h>

#include <structure/list/list.h>

using namespace algorithm;

TEST(ListTest, ListTestPushFront) {
	structure::List<uint32_t> list;
	for (uint32_t i = 0; i < 160; ++i) {
		list.push_front(i);
	}
	EXPECT_EQ(list.size(), 160);
	for (uint32_t idx = 160; unsigned int & iter : list) {
		--idx;
		EXPECT_EQ(iter, idx);
	}
}

TEST(ListTest, ListTestPushBack) {
	structure::List<uint32_t> list;
	for (uint32_t i = 0; i < 160; ++i) {
		list.push_back(i);
	}
	EXPECT_EQ(list.size(), 160);
	for (uint32_t idx = 0; unsigned int & iter : list) {
		EXPECT_EQ(iter, idx);
		++idx;
	}
}

TEST(ListTest, ListTestIterator) {
	structure::List<uint32_t> list;
	for (uint32_t i = 0; i < 160; ++i) {
		list.push_back(i);
	}
	EXPECT_EQ(list.size(), 160);
	for (uint32_t idx = 0; uint32_t & iter : list) {
		EXPECT_EQ(iter, idx);
		++idx;
	}
	for (uint32_t idx = 0; const uint32_t & iter : list) {
		EXPECT_EQ(iter, idx);
		++idx;
	}
	uint32_t idx = list.size();
	for (auto iter = list.rbegin(); iter != list.rend(); ++iter) {
		--idx;
		EXPECT_EQ(*iter, idx);
	}
}

TEST(ListTest, ListTestInsert) {
	structure::List<uint32_t> list;
	for (uint32_t i = 0; i < 160; ++i) {
		list.push_back(i);
	}
	EXPECT_EQ(list.size(), 160);
	for (auto iter = list.begin(); iter != list.end(); ++iter) {
		if (*iter % 2 == 0) {
			iter = list.erase(iter);
			EXPECT_EQ(*iter % 2, 1);
			iter = list.insert(iter, *iter - 1);
			EXPECT_EQ(*iter % 2, 0);
		}
	}
	EXPECT_EQ(list.size(), 160);
	for (uint32_t idx = 0; unsigned int & iter : list) {
		EXPECT_EQ(iter, idx);
		++idx;
	}
}