/*
* @author: BL-GS
* @date:   2023/5/6
*/


#include <random>
#include <vector>
#include <gtest/gtest.h>

#include <structure/array/small_vector.h>

using namespace algorithm;

TEST(SmallVectorTest, SmallVectorTestPushFront) {
   structure::SmallVector<uint32_t> vec;
   for (uint32_t i = 0; i < 160; ++i) {
	   vec.push_back(i);
   }

   EXPECT_EQ(vec.size(), 160);

   for (uint32_t idx = 0; uint32_t &iter : vec) {
	   EXPECT_EQ(iter, idx);
	   ++idx;
   }
}

TEST(SmallVectorTest, SmallVectorTestOperator) {
   structure::SmallVector<uint32_t> vec;
   for (uint32_t i = 0; i < 160; ++i) {
	   vec.push_back(i);
   }

   EXPECT_EQ(vec.size(), 160);

   for (uint32_t i = 0; i < 160; ++i) {
	   EXPECT_EQ(vec[i], i);
   }
}

TEST(SmallVectorTest, SmallVectorTestIterator) {
   structure::SmallVector<uint32_t> vec;
   for (uint32_t i = 0; i < 160; ++i) {
	   vec.push_back(i);
   }
   EXPECT_EQ(vec.size(), 160);
   for (uint32_t idx = 0; uint32_t &iter: vec) {
	   EXPECT_EQ(iter, idx);
	   ++idx;
   }
   for (uint32_t idx = 0; const uint32_t &iter: vec) {
	   EXPECT_EQ(iter, idx);
	   ++idx;
   }

   uint32_t idx = vec.size();
   for (auto iter = vec.rbegin(); iter != vec.rend(); ++iter) {
	   --idx;
	   EXPECT_EQ(*iter, idx);
   }
}

TEST(SmallVectorTest, SmallVectorTestInsert) {
   structure::SmallVector<uint32_t> vec;
   for (uint32_t i = 0; i < 160; ++i) {
	   vec.push_back(i);
   }
   EXPECT_EQ(vec.size(), 160);

   for (auto iter = vec.begin(); iter != vec.end(); ++iter) {
	   if (*iter % 2 == 0) {
		   iter = vec.erase(iter);
		   EXPECT_EQ(*iter % 2, 1);
		   iter = vec.insert(iter, *iter - 1);
		   EXPECT_EQ(*iter % 2, 0);
	   }
   }
   EXPECT_EQ(vec.size(), 160);
   for (uint32_t idx = 0; uint32_t &iter: vec) {
	   EXPECT_EQ(iter, idx);
	   ++idx;
   }
}

TEST(SmallVectorTest, SmallVectorRandomTest) {
   std::vector<uint32_t> test_vec;
   structure::SmallVector<uint32_t> vec;
   for (uint32_t i = 0; i < 250; ++i) {
	   test_vec.push_back(i);
	   vec.push_back(i);
   }

   EXPECT_EQ(test_vec.size(), vec.size());
   for (uint32_t k = 0; k < test_vec.size(); ++k) {
	   EXPECT_EQ(test_vec[k], vec[k]);
   }

   std::default_random_engine rander;

   for (uint32_t i = 0; i < 100; ++i) {
	   EXPECT_EQ(test_vec.size(), vec.size());

	   switch (size_t rand_num = rander(); rander() % 3) {
		   case 1:
			   test_vec.insert(test_vec.begin() + (rand_num % test_vec.size()), rand_num);
			   vec.insert(rand_num % vec.size(), rand_num);
			   break ;

		   case 2:
			   test_vec.erase(test_vec.begin() + (rand_num % test_vec.size()));
			   vec.erase(rand_num % vec.size());
			   break ;

		   default:
			   test_vec.push_back(i);
			   vec.push_back(i);
			   break ;
	   }
   }

   EXPECT_EQ(test_vec.size(), vec.size());
   for (uint32_t i = 0; i < test_vec.size(); ++i) {
	   EXPECT_EQ(test_vec[i], vec[i]);
   }

   for (uint32_t i = 0; i < 100; ++i) {
	   EXPECT_EQ(test_vec.size(), vec.size());

	   switch (size_t rand_num = rander(); rander() % 3) {
		   case 1:
			   test_vec.insert(test_vec.begin() + (rand_num % test_vec.size()), rand_num);
			   vec.insert(rand_num % vec.size(), rand_num);
			   break ;

		   case 2:
			   test_vec.erase(test_vec.begin() + (rand_num % test_vec.size()));
			   vec.erase(rand_num % vec.size());
			   break ;

		   default:
			   test_vec.push_back(i);
			   vec.push_back(i);
			   break ;
	   }
   }

   EXPECT_EQ(test_vec.size(), vec.size());
   for (uint32_t i = 0; i < test_vec.size(); ++i) {
	   EXPECT_EQ(test_vec[i], vec[i]);
   }
}