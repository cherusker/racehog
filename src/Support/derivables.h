#ifndef RACEHOG_SUPPORT_DERIVABLES_H_
#define RACEHOG_SUPPORT_DERIVABLES_H_

#include <algorithm>
#include <deque>
#include <iterator>
#include <utility> // std::swap, std::forward

#include <boost/core/noncopyable.hpp>

#include "Support/Logging.h"

// TODO:  Carefully examine error handling and exceptions.

namespace racehog {
  
  /// A container thats stores derived types of a base type efficiently.
  /**
   * Data will be allocated in chunks and objects will be emplaced one after
   * another.
   * This should achieve good spatial locality and minimise the amount of
   * (de)allocations compared to a standard container of pointers (e.g.
   * `boost::ptr_vector`).
   *
   * Items will be deconstructed and memory will be freed automatically when
   * derivables get destroyed.
   * Note that there is no way to free memory (partly) upfront or deconstruct
   * items early.
   *
   * A general note about performance:
   *
   *   - Insertion is `O(1)` but there is some overhead due to background
   *     administration.
   *
   *   - Lookup is in `O(1)` and really fast!  It is basically a lookup by
   *     `std::deque::[]` and a simple pointer dereference.
   *
   * \tparam BaseT Base type that has to have a virtual destructor to avoid
   *         leaking memory.
   *
   * \tparam DerivedTypes List of all derived types that are going to be fed
   *         to this container.
   *         A complete list is required to calculate proper alignments and
   *         chunk sizes.
   */

  template<class    BaseType,
           class... DerivedTypes>
  class derivables final
    : private boost::noncopyable {

  public:

    typedef BaseType          value_type;
    typedef std::size_t       size_type; ///< Public/external size type.
    typedef std::ptrdiff_t    difference_type;
    typedef value_type&       reference;
    typedef const value_type& const_reference;
    typedef value_type*       pointer;
    typedef const value_type* const_pointer;

  private:

    typedef derivables<BaseType, DerivedTypes...> container_type;
    typedef container_type*                       container_pointer;
    typedef const container_type*                 const_container_pointer;

    // TODO:  Upgrade to RandomAccessIterator?

    template<class _container_pointer,
             class _pointer,
             class _reference>
    class _iterator
      : public std::iterator<std::random_access_iterator_tag,
                             value_type,      ///< value_type
                             difference_type, ///< difference_type
                             _pointer,        ///< pointer
                             _reference> {    ///< reference

    public:

      typedef _iterator<_container_pointer,
                        _pointer,
                        _reference> iterator_type;

      typedef iterator_type*       iterator_pointer;
      typedef iterator_type&       iterator_reference;
      typedef const iterator_type& const_iterator_reference;

      _iterator(void)
        : ptr(nullptr),
          id(0) {}

      _iterator(const_iterator_reference other)
        : ptr(other.ptr),
          id(other.id) {
        LOG(INFO) << "Copying derivables::iterator.";
      }

      _iterator(iterator_reference& other)
        : ptr(std::move(other.ptr)),
          id(std::move(other.id)) {
        LOG(INFO) << "Moving derivables::iterator.";
      }

      _iterator(_container_pointer ptr,
                size_type          id)
        : ptr(ptr),
          id(id) {}

      iterator_reference
      operator=(const_iterator_reference other) {
        ptr = other.ptr;
        id  = other.id;
        return *this;
      }

      bool
      operator==(const_iterator_reference other) const {
        return ptr == other.ptr &&
               id  == other.id  ? true : false;
      }

      bool
      operator!=(const_iterator_reference other) const {
        return ptr != other.ptr ||
               id  != other.id  ? true : false;
      }

      bool
      operator<(const_iterator_reference other) const {
        return ptr == other.ptr &&
               id  <  other.id  ? true : false;
      }

      bool
      operator>(const_iterator_reference other) const {
        return other < *this;
      }

      bool
      operator<=(const_iterator_reference other) const {
        return *this > other == true ? false : true;
      }

      bool
      operator>=(const_iterator_reference other) const {
        return *this < other == true ? false : true;
      }

      _reference
      operator*(void) const {
        return (*ptr)[id];
      }

      _pointer
      operator->(void) const {
        return &(*ptr)[id];
      }

      iterator_reference
      operator++() {
        ++id;
        return *this;
      }

      iterator_type
      operator++(int) {
        _iterator tmp = *this;
        ++*this;
        return tmp;
      }

      iterator_reference
      operator--() {
        --id;
        return *this;
      }

      iterator_type
      operator--(int) {
        // TODO:  `iterator_type` instead of `_iterator`?
        _iterator tmp = *this;
        --*this;
        return tmp;
      }

      iterator_reference
      operator+=(difference_type n) {
        id += n;
        return *this;
      }

      iterator_type
      operator+(difference_type n) const {
        iterator_type tmp = *this;
        return tmp += n;
      }

      iterator_reference
      operator-=(difference_type n) {
        id -= n;
        return *this;
      }

      iterator_type
      operator-(difference_type n) const {
        iterator_type tmp = *this;
        return tmp -= n;
      }

      difference_type
      operator-(const_iterator_reference other) {
        return id - other.id;
      }

      _reference
      operator[](difference_type n) const {
        return *(*this + n);
      }

      friend void
      swap(iterator_reference lhs,
           iterator_reference rhs) {
        (void) std::swap(lhs.ptr, rhs.ptr);
        (void) std::swap(lhs.id,  rhs.id);
      }

    private:

      _container_pointer ptr;
      size_type          id;
    };

  public:
    
    typedef _iterator<container_pointer,
                      pointer,
                      reference> iterator;

    typedef _iterator<const_container_pointer,
                      const_pointer,
                      const_reference> const_iterator;

    ~derivables(void) {
      (void) clear();
    }

    /// No bounds check;  know what you request.

    reference
    operator[](size_type id) {
      return *reinterpret_cast<pointer>(pointer_at(id));
    }

    /// No bounds check;  know what you request.

    const_reference
    operator[](size_type id) const {
      return *reinterpret_cast<const_pointer>(pointer_at(id));
    }

    reference
    front(void) {
      return this->operator[](0);
    }

    const_reference
    front(void) const {
      return this->operator[](0);
    }

    reference
    back(void) {
      return this->operator[](size() - 1);
    }

    const_reference
    back(void) const {
      return this->operator[](size() - 1);
    }

    iterator
    begin(void) {
      return iterator(this, 0);
    }

    const_iterator
    begin(void) const {
      return const_iterator(this, 0);
    }

    const_iterator
    cbegin(void) const {
      return const_iterator(this, 0);
    }

    iterator
    end(void) {
      return iterator(this, size());
    }

    const_iterator
    end(void) const {
      return const_iterator(this, size());
    }

    const_iterator
    cend(void) const {
      return const_iterator(this, size());
    }

    bool
    empty(void) const {
      return size() == 0;
    }

    size_type
    size(void) const {
      return num_elems;
    }

    void
    clear(void) {
      for (auto ptr : chunks) {
        auto it  = ptr;
        auto end = it + raw_chunk_size;
        while (num_elems > 0 && it != end) {
          --num_elems;
          ((pointer) it)->~value_type();
          it += raw_elem_size;
        }
        (void) delete[] ptr;
      }
      (void) chunks.clear();
      raw_chunk_ptr = nullptr;
      raw_chunk_end = nullptr;
    }

    /**
     * \tparam T must be `BaseType` or any type in `DerivedTypes`.
     */

    template<class    T,
             class... Args>
    T&
    emplace_at(size_type id,
               Args&&... args) {
      static_assert(sizeof(T) <= raw_elem_size, "Type does not fit.");
      // TODO:  Handle `pointer_at(id) == nullptr`.
      return *::new(pointer_at(id)) T(std::forward<Args>(args)...);
    }

    /**
     * \tparam T must be `BaseType` or any type in `DerivedTypes`.
     */

    template<class    T,
             class... Args>
    T&
    emplace_back(Args&&... args) {
      static_assert(sizeof(T) <= raw_elem_size, "Type does not fit.");
      (void) alloc();
      // TODO:  Can `new` throw an exception + how do we catch that?
      T* ptr = ::new(raw_chunk_ptr) T(std::forward<Args>(args)...);
      raw_chunk_ptr += raw_elem_size;
      ++num_elems;
      return *ptr;
    }

    /// Sets the size to \p size.
    /**
     * Currently only enlarges the container;  does nothing if \p size `<=`
     * derivables::size.
     * Added elements will _not_ be initialised in any way:
     * derivables::emplace_at have to be called for all added cells before
     * accesing them.
     */

    void
    resize(size_type new_size) {
      // TODO:  This logic can be improved + the loop can be reduced!
      //   -> allocate in chunks!
      while (num_elems < new_size) {
        (void) alloc();
        raw_chunk_ptr += raw_elem_size;
        ++num_elems;
      }
    }

  private:

    typedef char                  raw_value_type;
    typedef std::size_t           raw_size_type; ///< Internal size type.
    typedef raw_value_type*       raw_pointer;
    typedef const raw_value_type* const_raw_pointer;

    typedef std::vector<raw_pointer> raw_pointers;

    /// Size of a single element.

    static constexpr raw_size_type raw_elem_size =
      std::max({ sizeof(value_type), sizeof(DerivedTypes)... });

    /// One chunk holds exactly 16 elements.

    // TODO:  Provide a minimum of 4096 bytes.
    // TODO:  Discuss object alignment (always multiple of 8?) ...

    static constexpr size_type     num_elems_per_chunk = 16;
    static constexpr raw_size_type raw_chunk_size =
      raw_elem_size * num_elems_per_chunk;

    /// Memory is cheap, execution is expensive.
    /**
     * `size` could be calculated on every call to derivables::size but I
     * rather spend a few buffer bytes for that.
     */

    size_type num_elems = 0;

    raw_pointers chunks;
    raw_pointer  raw_chunk_ptr = nullptr;
    raw_pointer  raw_chunk_end = nullptr;

    /// Allocate more memory if necessary.
    /**
     * derivables::raw_chunk_ptr always points to a valid memory location
     * after calling derivables::alloc.
     */

    void
    alloc(void) {
      if (raw_chunk_ptr == raw_chunk_end) {
        raw_chunk_ptr = new char[raw_chunk_size];
        raw_chunk_end = raw_chunk_ptr + raw_chunk_size;
        (void) chunks.push_back(raw_chunk_ptr);
      }
    }

    /// No bounds check;  know what you request.

    raw_pointer
    pointer_at(size_type id) {
      // TODO:  Investigate release code of that expression.
      return chunks[id / num_elems_per_chunk] +
        (id % num_elems_per_chunk) * raw_elem_size;
    }

    const_raw_pointer
    pointer_at(size_type id) const {
      // TODO:  Investigate release code of that expression.
      return chunks[id / num_elems_per_chunk] +
        (id % num_elems_per_chunk) * raw_elem_size;
    }
  };
}

#endif // RACEHOG_SUPPORT_DERIVABLES_H_
