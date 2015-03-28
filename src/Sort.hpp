#ifndef SORT_HPP
#define	SORT_HPP

#include "Vector.hpp"

namespace CryptoGL
{
    enum class QuickSortPivotChoice : uint8_t
    {
        First,
        Median,
        Last
    };
    
    class Sort 
    {
    public:
        Sort(const UInt32Vector &V) : V(V) {}
        virtual ~Sort() {}
        
        virtual int32_t sort(int32_t first, int32_t last) = 0;

    protected:
        UInt32Vector V;
    };
    
    
    class MergeSort : public Sort
    {
    public:
        MergeSort(const UInt32Vector &V) : Sort(V) {}
        
        int32_t sort(int32_t first, int32_t last) override
        {
            return 0;
        }
        
        uint32_t merge(const uint32_t first, const uint32_t last)
        {
            const uint32_t mid = (first + last) / 2;
            uint32_t i = first;
            uint32_t j = mid + 1;
            UInt32Vector Result(V.size()); 
            uint32_t k = 0;
            uint32_t inversion = 0;

            while (i <= mid && j <= last) 
            {
                if (V[i] <= V[j]) 
                {
                    Result[k++] = V[i++];
                } 
                else 
                {
                    Result[k++] = V[j++];
                    inversion += mid - i + 1;
                }
            }
            while (i <= mid)
                Result[k++] = V[i++];

            while (j <= last)
                Result[k++] = V[j++];

            for (uint32_t m = 0 ; m < last - first + 1; ++m)
                V[m + first] = Result[m];

            return inversion;
        }
        
        uint32_t countInversion(uint32_t lower, uint32_t upper)
        {
            if (lower >= upper) 
                return 0;

            const uint32_t mid = (lower + upper) / 2;
            const uint32_t x = countInversion(lower, mid);
            const uint32_t y = countInversion(mid + 1, upper);
            const uint32_t z = merge(lower, upper);

            return x + y + z;
        }
    };
    
    /*
     * QuickSort algorithm class.
     */
    class QuickSort : public Sort
    {
    private:
        QuickSortPivotChoice PivotChoice;
        int32_t comparison_count = 0;
        
    public:
        QuickSort(const UInt32Vector &V, QuickSortPivotChoice PivotChoice) 
            : Sort(V), PivotChoice(PivotChoice) {}
        
        int32_t sort(int32_t first, int32_t last) override
        {
            int32_t index_pivot = 0;
            if(first < last)
            {
                comparison_count += last - first;
                choosePivot(first, last);
                index_pivot = partition(first, last);
                sort(first, index_pivot - 1); 
                sort(index_pivot + 1, last);
            }
            
            return comparison_count;
        }
                
        int32_t partition(const int32_t first, const int32_t last)
        {
            const int32_t pivot = V[first];
            int32_t i = first + 1;
            for(int32_t j = first + 1; j <= last; ++j)
            {
                if(V[j] < pivot)
                {
                    V.swap(j, i);
                    i++;
                }
            }
            V.swap(first, i - 1);
            
            return i - 1;
        }
        
        /* Choose the pivot to partition the vector and return the index of the pivot in the vector. */
        void choosePivot(const int32_t first, const int32_t last)
        {
            switch(PivotChoice)
            {
                case QuickSortPivotChoice::First:
                    break;

                case QuickSortPivotChoice::Last:
                    V.swap(first, last);
                    break;

                case QuickSortPivotChoice::Median:
                    medianOf3(first, last);
                    break;
            }
        }
        
        void setPivotChoice(QuickSortPivotChoice Choice)
        {
            PivotChoice = Choice;
            comparison_count = 0;
        }
        
        void medianOf3(const int32_t first, const int32_t last) 
        {
            const int32_t middle = (first + last) / 2;          
            if (V[first] > V[middle])
              V.swap(first, middle);

            if (V[first] > V[last])
              V.swap(first, last);

            if (V[middle] > V[last])
              V.swap(middle, last);

            V.swap(middle, first);
        }


    };
}

#endif