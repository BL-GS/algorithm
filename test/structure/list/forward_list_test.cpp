/*
 * @author: BL-GS 
 * @date:   2023/5/6
 */

#include <gtest/gtest.h>

#include <structure/list/forward_list.h>

using namespace algorithm;

TEST(ForwardListTest, ForwardListTestPushFront) {
	structure::ForwardList<uint32_t> list;
	for (uint32_t i = 0; i < 160; ++i) {
		list.push_front(i);
	}

	for (uint32_t idx = 160; unsigned int & iter : list) {
		--idx;
		EXPECT_EQ(iter, idx);
	}
}

TEST(ForwardListTest, ForwardListTestIterator) {
	structure::ForwardList<uint32_t> list;
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
}

TEST(ForwardListTest, ForwardListTestInsert) {
	structure::ForwardList<uint32_t> list;
	for (uint32_t i = 0; i < 160; ++i) {
		list.push_back(i);
	}
	EXPECT_EQ(list.size(), 160);
	for (auto iter = list.begin(); iter != list.end(); ++iter) {
		if (*iter % 2 == 0) {
			iter = list.erase_next(iter);
			iter = list.insert_next(iter, *iter + 1);
			EXPECT_EQ(*iter % 2, 1);
		}
	}
	EXPECT_EQ(list.size(), 160);
	for (uint32_t idx = 0; unsigned int & iter : list) {
		EXPECT_EQ(iter, idx);
		++idx;
	}
}