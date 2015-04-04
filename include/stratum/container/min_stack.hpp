#ifndef STRATUM_CONTAINER_MIN_STACK
#define STRATUM_CONTAINER_MIN_STACK

#include <deque>
#include <tuple>
#include <functional>
#include <utility>


NAMESPACE_BEGIN(stratum,container)


template< typename T
        , typename COMPARATOR = std::less<T> >
class min_stack
{
public:
    typedef std::tuple<T, size_t, size_t>  index_type;
    typedef std::deque<index_type>  container_type
    typedef T  value_type;
    typedef container_type::size_type  size_type;
    typedef container_type::reference  reference;
    typedef container_type::const_reference  const_reference;

    explicit min_stack(COMPARATOR c = COMPARATOR())
    : storage_(), compare_(c)
    {
    } // min_stack()
    
    
    void push(const T& elem)
    {
        if( empty() )
        {
            storage_.push_back( std::make_tuple(elem, 0, 0) );
        }
        else
        {
            size_t smallestIndex = std::get<1>( storage_.back() );
            size_t largestIndex = std::get<2>( storage_.back() );

            if ( compare_(elem, min()) )
                smallestIndex = storage_.size();
                
            if ( compare_(max(), elem) )
                largestIndex = storage_.size();

            storage_.push_back( std::make_tuple(elem, smallestIndex, largestIndex) );
        }
    } // push()
    
    void pop()
    {
        storage_.pop_back();
    } // pop()
    
    const T& top() const
    {
        return std::get<0>( storage_.back() );
    } // top()

    
    bool   empty() const { return storage_.empty(); }
    size_t size() const { return storage_.size(); }


    const T& min() const
    {
        size_t smallestIndex = std::get<1>( storage_.back() );
        return std::get<0>( storage_[smallestIndex] );
    } // min()
    
    const T& max() const
    {
        size_t largestIndex = std::get<2>( storage_.back() );
        return std::get<0>( storage_[largestIndex] );
    } // max()
    
    
private:
    container_type  storage_;
    COMPARATOR      compare_;
    
}; // class min_stack


NAMESPACE_END(stratum,container)


#endif // STRATUM_CONTAINER_MIN_STACK


/* Returning the top element looks at the back of the deque. */
template <typename T, typename Comparator>
const T& MinStack<T, Comparator>::top() const {
  return mStack.back().first;
}

/* Returning the min element looks at the element in the deque that is the
 * smallest so far.  It's held at the index at the top of the stack. */
template <typename T, typename Comparator>
const T& MinStack<T, Comparator>::min() const {
  return mStack[mStack.back().second].first;
}

/* Inserting a new element adds it to the stack and updates the minimum element
 * if the new element is smaller.
 */
template <typename T, typename Comparator>
void MinStack<T, Comparator>::push(const T& elem) {
  /* If the stack is empty, add the new element and mark that it's the smallest
   * so far.
   */
  if (empty()) {
    mStack.push_back(std::make_pair(elem, 0));
  }
  else {
    /* Otherwise, find the index of the smallest element and insert the new
     * element annotated with that index.
     */
    size_t smallestIndex = mStack.back().second;

    /* If this new element is smaller, the smallest element will now be at the
     * back of the list.
     */
    if (mComp(elem, min()))
      smallestIndex = mStack.size();

    /* Add the element in. */
    mStack.push_back(std::make_pair(elem, smallestIndex));
  }
}

/* Popping an element off the stack just removes the top pair from the
 * deque.
 */
template <typename T, typename Comparator>
void MinStack<T, Comparator>::pop() {
  mStack.pop_back();
}