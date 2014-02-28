
#ifndef FLEISSNERTEST_HPP
#define	FLEISSNERTEST_HPP

#include <gtest/gtest.h>
#include "../src/Fleissner.hpp"

class FleissnerTest : public ::testing::Test
{
protected:
   Fleissner *F;

   virtual void SetUp()
   {
      F = new Fleissner({std::make_pair(0u, 1u), std::make_pair(0u, 3u), std::make_pair(0u, 5u),
         std::make_pair(1u, 4u),
         std::make_pair(2u, 2u),
         std::make_pair(3u, 1u), std::make_pair(3u, 4u),
         std::make_pair(4u, 5u),
         std::make_pair(5u, 3u)
      }, 6u, true);
   }

   virtual void TearDown()
   {
      delete F;
   }
};

TEST_F(FleissnerTest, encode)
{
   const ClassicalType clear_text = "JECHIFFRECETEXTEAVECUNCACHETOURNANT";

   EXPECT_EQ("TJEEOCCUCRHENUITNAEFNCFXTATAERCAHEEV", F->encode(clear_text));
}

TEST_F(FleissnerTest, decode)
{
   const ClassicalType clear_text = "JECHIFFRECETEXTEAVECUNCACHETOURNANTA";

   EXPECT_EQ(clear_text, F->decode("TJEEOCCUCRHENUITNAEFNCFXTATAERCAHEEV"));
}

#endif