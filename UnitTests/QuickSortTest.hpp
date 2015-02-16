
#ifndef QUICKSORTTEST_HPP
#define	QUICKSORTTEST_HPP

#include "Test.hpp"
#include "../src/File.hpp"
#include "../src/Sort.hpp"

namespace UnitTests
{
    class QuickSortTest : public Test
    {
    protected:
       QuickSort *DataSet1, *DataSet2, *DataSet3;

       void setUp() override
       {
          DataSet1 = new QuickSort({3, 9, 8, 4, 6, 10, 2, 5, 7, 1}, QuickSortPivotChoice::First);
          
          const File<uint32_t> SampleFile100("/home/gabriel/Documents/CryptoGL_DEV/UnitTests/QuickSort100Values.txt");
          const UInt32Vector Values100 = SampleFile100.load();
          DataSet2 = new QuickSort(Values100, QuickSortPivotChoice::First);   
          
          const File<uint32_t> SampleFile1000("/home/gabriel/Documents/CryptoGL_DEV/UnitTests/QuickSort1000Values.txt");
          const UInt32Vector Values1000 = SampleFile1000.load();
          DataSet3 = new QuickSort(Values1000, QuickSortPivotChoice::First);
       }

       void tearDown() override
       {
          delete DataSet1;
          delete DataSet2;
          delete DataSet3;
       }
    };

        
    TEST(CountComparisonSize10FirstPivot, QuickSortTest) 
    {
        compare(25, DataSet1->sort(0, 9));
    }
    
    TEST(CountComparisonSize10LastPivot, QuickSortTest) 
    {
        DataSet1->setPivotChoice(QuickSortPivotChoice::Last);
        compare(29, DataSet1->sort(0, 9));
    }
    
    TEST(CountComparisonSize10MedianPivot, QuickSortTest) 
    {
        DataSet1->setPivotChoice(QuickSortPivotChoice::Median);
        compare(21, DataSet1->sort(0, 9));
    }
    
    /* Tests with a vector of size 100. */
    TEST(CountComparisonSize100FirstPivot, QuickSortTest) 
    {
        compare(615, DataSet2->sort(0, 99));
    }
    
    TEST(CountComparisonSize100LastPivot, QuickSortTest) 
    {
        DataSet2->setPivotChoice(QuickSortPivotChoice::Last);
        compare(587, DataSet2->sort(0, 99));
    }
    
    TEST(CountComparisonSize100MedianPivot, QuickSortTest) 
    {
        DataSet2->setPivotChoice(QuickSortPivotChoice::Median);
        compare(518, DataSet2->sort(0, 99));
    }
    
    /* Tests with a vector of size 1000. */
    TEST(CountComparisonSize1000FirstPivot, QuickSortTest) 
    {
        compare(10297, DataSet3->sort(0, 999));
    }
    
    TEST(CountComparisonSize1000LastPivot, QuickSortTest) 
    {
        DataSet3->setPivotChoice(QuickSortPivotChoice::Last);
        compare(10184, DataSet3->sort(0, 999));
    }
    
    TEST(CountComparisonSize1000MedianPivot, QuickSortTest) 
    {
        DataSet3->setPivotChoice(QuickSortPivotChoice::Median);
        compare(8921, DataSet3->sort(0, 999));
    }
}

#endif