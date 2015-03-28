#ifndef HEAP_HPP
#define	HEAP_HPP

#include "Types.hpp"
#include "exceptions/Exception.hpp"


namespace CryptoGL
{
    /* Main utilities of the heap (complete):
     * Find repeatedly maximums or minimums efficiently.
     * Running-time = O(1) */
    
    template <class Type>
    class Heap 
    {
        static constexpr uint8_t ROOT = 0;
        
    public:
        Heap() : height(0) {}
        explicit Heap(const Vector<Type> &V) : HeapVector(V)
        {
            const int32_t v_size = V.size();
            for(int32_t i = v_size - 1; i >= 0; --i)
            {
                siftDown(i);
            }
        }
        
        virtual ~Heap() {}
        
           
        /* Insert a Value in a Heap. Running time = O(log n) where n is number of elements in the heap. */
        void push(const Type &Value)
        {
            HeapVector.push_back(Value);
            siftUp(HeapVector.size() - 1);
        }
        
        /* Get the parent of the given index. */
        static uint32_t parentOf(const uint32_t index)
        {
            return (index - 1) / 2;
        }
        
        /* Get the left child of the given index. */
        static uint32_t leftChildOf(const uint32_t index)
        {
            return (2 * index) + 1;
        }
        
        /* Get the right child of the given index. */
        static uint32_t rightChildOf(const uint32_t index)
        {
            return (2 * index) + 2;
        }
        
        /* Check if the heap is empty. */
        bool isEmpty() const
        {
            return size() == 0;
        }
        
        /* Remove the minimum value from the Heap and return its value. */
        Type removeMinimumValue()
        {
            if(isEmpty())
            {
                throw Exception("Can not remove the minimum value because the heap is empty.");
            }
            
            HeapVector.swap(ROOT, size() - 1);
            const Type MinimumValue = HeapVector.back();
            HeapVector.pop_back();
            if(!isEmpty())
            {
                siftDown(ROOT);
            }
            
            return MinimumValue;
        }
        
        /* Get the size of the heap. */
        uint32_t size() const
        {
            return HeapVector.size();
        }
        
        /* Return the root of the heap which contains the minimum value. */
        Type getMinimumValue() const
        {
            return HeapVector[ROOT];
        }
                        
    private:
        /* Sift the value up to keep the heap property valid. */
        void siftUp(const uint32_t index)
        {
            // Cannot sift up the root.
            if(index == ROOT)
            {
                return;
            }
            
            const uint32_t parent_index = parentOf(index);
            if(HeapVector[parent_index] > HeapVector[index])
            {
                HeapVector.swap(parent_index, index);
                siftUp(parent_index);
            }
        }
        
        /* Sift the value down to keep the heap property valid. */
        void siftDown(const uint32_t index)
        {
            uint32_t left_child_index = leftChildOf(index);
            uint32_t right_child_index = rightChildOf(index);
            
            uint32_t minimum_index = index;
            if(right_child_index >= size())
            {
                if(left_child_index >= size())
                {
                    return;
                }
                
                minimum_index = left_child_index;
            }
            else
            {
                minimum_index = HeapVector[left_child_index] <= HeapVector[right_child_index] 
                        ? left_child_index : right_child_index;
            }

            if(HeapVector[index] > HeapVector[minimum_index])
            {
                HeapVector.swap(minimum_index, index);
                siftDown(minimum_index);
            }
        }
        
        Vector<Type> HeapVector;
        uint32_t height;
    };
}

#endif