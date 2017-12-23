/// \file
/// Unit test for Sort.

#include "ila/ast/sort.h"
#include "test_util.h"
#include "gtest/gtest.h"
#include <iostream>
#include <string>

namespace ila {

TEST(Sort, Boolean) {
  Sort bool_sort;
  EXPECT_TRUE(bool_sort.is_ast());
  EXPECT_TRUE(bool_sort.is_bool());

  EXPECT_EQ(0, bool_sort.bit_width());
  EXPECT_EQ(0, bool_sort.addr_width());
  EXPECT_EQ(0, bool_sort.data_width());
  EXPECT_TRUE(NULL == bool_sort.range());
  EXPECT_EQ(0, bool_sort.num_arg());
  EXPECT_DEATH(bool_sort.arg(0), ".*");

  std::string msg;
  GET_STDOUT_MSG(std::cout << bool_sort, msg);
  EXPECT_EQ("Boolean", msg);
}

TEST(Sort, Bitvector) {
  Sort bv_sort(8);
  EXPECT_TRUE(bv_sort.is_ast());
  EXPECT_TRUE(bv_sort.is_bv());

  EXPECT_EQ(8, bv_sort.bit_width());
  EXPECT_EQ(0, bv_sort.addr_width());
  EXPECT_EQ(0, bv_sort.data_width());
  EXPECT_TRUE(NULL == bv_sort.range());
  EXPECT_EQ(0, bv_sort.num_arg());
  EXPECT_DEATH(bv_sort.arg(1), ".*");

  std::string msg;
  GET_STDOUT_MSG(std::cout << bv_sort, msg);
  EXPECT_EQ("Bv(8)", msg);
}

TEST(Sort, Memory) {
  Sort mem_sort(2, 32);
  EXPECT_TRUE(mem_sort.is_ast());
  EXPECT_TRUE(mem_sort.is_mem());

  EXPECT_EQ(0, mem_sort.bit_width());
  EXPECT_EQ(2, mem_sort.addr_width());
  EXPECT_EQ(32, mem_sort.data_width());
  EXPECT_TRUE(NULL == mem_sort.range());
  EXPECT_EQ(0, mem_sort.num_arg());
  EXPECT_DEATH(mem_sort.arg(0), ".*");

  std::string msg;
  GET_STDOUT_MSG(std::cout << mem_sort, msg);
  EXPECT_EQ("Mem(2, 32)", msg);
}

TEST(Sort, Application) {
  Sort bool_sort;
  Sort bv_sort(8);
  Sort mem_sort(2, 32);

  SortPtr range = std::make_shared<Sort>(bv_sort);
  SortPtrVec args;
  args.push_back(std::make_shared<Sort>(bool_sort));
  args.push_back(std::make_shared<Sort>(bv_sort));
  args.push_back(std::make_shared<Sort>(mem_sort));

  Sort app_sort(range, args);
  EXPECT_TRUE(app_sort.is_ast());
  EXPECT_TRUE(app_sort.is_app());

  EXPECT_EQ(0, app_sort.bit_width());
  EXPECT_EQ(0, app_sort.addr_width());
  EXPECT_EQ(0, app_sort.data_width());
  EXPECT_FALSE(NULL == app_sort.range());
  EXPECT_TRUE(app_sort.range()->is_bv());
  EXPECT_TRUE(app_sort.range()->is_sort());
  EXPECT_EQ(*app_sort.range(), bv_sort);
  EXPECT_EQ(3, app_sort.num_arg());
  EXPECT_EQ(*app_sort.arg(0), bool_sort);
  EXPECT_EQ(*app_sort.arg(1), bv_sort);
  EXPECT_EQ(*app_sort.arg(2), mem_sort);
  EXPECT_DEATH(app_sort.arg(3), ".*");

  std::string msg;
  GET_STDOUT_MSG(std::cout << app_sort, msg);
  EXPECT_EQ("App(Bv(8))(Boolean, Bv(8), Mem(2, 32))", msg);
}

} // namespace ila

