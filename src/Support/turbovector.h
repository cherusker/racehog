#ifndef RACEHOG_SUPPORT_TURBOVECTOR_H_
#define RACEHOG_SUPPORT_TURBOVECTOR_H_

#include <cstdlib> // malloc, free
#include <cstring> // memcpy, memcmp

#include <boost/core/noncopyable.hpp>

#include "Support/Logging.h"

// TODO:  IRL benchmarking of `std::vector` and `turbovector` to see if it is
//   worth maintaining `turbovector` or rather switch over to `std::vector`?

namespace racehog {
  
  /// A supercharged version of `std::vector`;  carefully read the usage.
  /**
   * Internally, \ref turbovector keeps a memory blob, grows that blob
   * automatically when needed, and provides pointers to iterate over it (just
   * like `std::vector`).
   * The main difference to `std::vector` are turbovector::clear and
   * turbovector::pop_back, which never overwrite (reset or destruct) memory
   * and, therefore, operate in `O(1)`.
   * Due to that, however, \ref turbovector can only be used with data types
   * that do not rely on destructors or reset memory.
   *
   * Furthermore, reallocation and copy construction happens via `memcpy` at all
   * times and require all stored objects to be TriviallyCopyable
   * (https://en.cppreference.com/w/cpp/named_req/TriviallyCopyable).
   *
   * The constructor of \ref turbovector has slightly different behaviour to
   * what is known from `std::vector`:
   *
   *   - Immediately allocating memory in _every_ constructor removes a few
   *     extra checks along the way.
   *
   *   - Allocating but _not_ initialising \ref turbovector makes the
   *     constructor run in `O(1)` instead of `O(n)`, given `std::malloc` runs
   *     in `O(1)`.
   *     Granted, for most instances of \ref turbovector this not measurable :)
   *
   *   - The allocated size (capacity) of \ref turbovector will always be 2^x
   *     bytes.
   *     This restriction should most likely always result in cache friendly
   *     solutions and it mitigates "bad" usage behaviour (when misusing
   *     \ref turbovector::resize or \ref turbovector::resize_to_fit).
   *
   * Note that some functions are missing (compared to `std::vector`);  partly
   * because the make no sense with \ref turbovector and partly because they are
   * currently not needed in _RaceHog_.
   */

  template<class T>
  class turbovector final
    : private boost::noncopyable {

  public:

    typedef T                 value_type;
    typedef std::size_t       size_type;
    typedef std::ptrdiff_t    difference_type;
    typedef value_type&       reference;
    typedef const value_type& const_reference;
    typedef value_type*       pointer;
    typedef const value_type* const_pointer;
    typedef pointer           iterator;
    typedef const_pointer     const_iterator;

    typedef turbovector<value_type> self;
    typedef self&                   self_reference;
    typedef const self&             const_self_reference;

    /// Always starts with allocated but **uninitialised** memory.
    /**
     * See \ref turbovector for details.
     */

    turbovector(size_type cap) {
      size_type c = min_cap;
      while (c < cap)
        c += c; // We always want to hit 2^x.
      b   = alloc(c);
      e   = b;
      eof = b + c;
    }

    /// Fast copy through `memcpy`.
    /**
     * See \ref turbovector for details.
     */

    turbovector(const turbovector& other) {
      LOG(INFO) << "Copying turbovector.";
      const size_type s = other.size();
      const size_type c = other.capacity();
      b   = alloc(c);
      e   = b + s;
      eof = b + c;
      (void) std::memcpy(b, other.b, s * sizeof(value_type));
    }

    turbovector(turbovector&& other) {
      LOG(INFO) << "Moving turbovector.";
      b         = other.b;
      e         = other.e;
      eof       = other.eof;
      other.b   = nullptr;
      other.e   = nullptr;
      other.eof = nullptr;
    }

    ~turbovector(void) {
      (void) dealloc(b);
    }

    /// https://en.cppreference.com/w/cpp/container/vector/operator_cmp
    /**
     * Comparison via `std::memcmp`.
     */

    bool
    operator==(const_self_reference other) const {
      size_type s = size();
      if (s != other.size())
        return false;
      return std::memcmp(b, other.b, s * sizeof(value_type)) == 0 ?
        true : false;
    }

    /// https://en.cppreference.com/w/cpp/container/vector/operator_at

    reference
    operator[](size_type pos) {
      return *(b + pos);
    }
		
    /// https://en.cppreference.com/w/cpp/container/vector/operator_at

    const_reference
    operator[](size_type pos) const {
      return *(b + pos);
    }

    /// https://en.cppreference.com/w/cpp/container/vector/front

    reference
    front(void) {
      return *b;
    }
		
    /// https://en.cppreference.com/w/cpp/container/vector/front

    const_reference
    front(void) const {
      return *b;
    }

    /// https://en.cppreference.com/w/cpp/container/vector/back

    reference
    back(void) {
      return *(e - 1);
    }
		
    /// https://en.cppreference.com/w/cpp/container/vector/back

    const_reference
    back(void) const {
      return *(e - 1);
    }

    /// https://en.cppreference.com/w/cpp/container/vector/data

    pointer
    data(void) {
      return b;
    }

    /// https://en.cppreference.com/w/cpp/container/vector/data

    const_pointer
    data(void) const {
      return b;
    }

    /// https://en.cppreference.com/w/cpp/container/vector/begin

    iterator
    begin(void) {
      return b;
    }

    /// https://en.cppreference.com/w/cpp/container/vector/begin

    const_iterator
    begin(void) const {
      return b;
    }

    /// https://en.cppreference.com/w/cpp/container/vector/begin

    const_iterator
    cbegin(void) const {
      return b;
    }

    /// https://en.cppreference.com/w/cpp/container/vector/end

    iterator
    end(void) {
      return e;
    }

    /// https://en.cppreference.com/w/cpp/container/vector/end

    const_iterator
    end(void) const {
      return e;
    }

    /// https://en.cppreference.com/w/cpp/container/vector/end

    const_iterator
    cend(void) const {
      return e;
    }

    /// https://en.cppreference.com/w/cpp/container/vector/empty
    /**
     * Always use turbovector::empty instead of turbovector::size when checking
     * for emptiness;  should be somewhat more efficient.
     */

    bool
    empty(void) const {
      return b == e ? true : false;
    }

    /// https://en.cppreference.com/w/cpp/container/vector/size

    size_type
    size(void) const {
      return e - b;
    }

    /// https://en.cppreference.com/w/cpp/container/vector/reserve
    /**
     * Note that \p new_cap will always be rounded up to the next 2^x.
     */

    void
    reserve(size_type new_cap) {
      if (capacity() < new_cap)
        (void) expand(new_cap);
    }

    /// https://en.cppreference.com/w/cpp/container/vector/capacity

    size_type
    capacity(void) const {
      return eof - b;
    }

    /// https://en.cppreference.com/w/cpp/container/vector/clear
    /**
     * Beware:  turbovector::clear does not touch cleared elements in any way.
     * See discussion in class description for details.
     */

    void
    clear(void) {
      e = b;
    }

    /// https://en.cppreference.com/w/cpp/container/vector/push_back

    void
    push_back(const value_type& value) {
      if (e == eof)
        (void) expand(size() + 1);
      (void) ::new(e++) value_type(value);
    }

    /// https://en.cppreference.com/w/cpp/container/vector/push_back

    void
    push_back(value_type&& value) {
      if (e == eof)
        (void) expand(size() + 1);
      (void) ::new(e++) value_type(std::move(value));
    }

    /// https://en.cppreference.com/w/cpp/container/vector/pop_back
    /**
     * Does not touch popped element (specifically does _not_ call the
     * destructor).
     */

    void
    pop_back(void) {
      --e;
    }

  private:

    // No less than two;  IF a vector is used somewhere, there should be a
    //   reason for that and that reason is usually to store a couple of
    //   elements.

    static constexpr size_type min_cap = 2;

    pointer b;
    pointer e;
    pointer eof;

    /// Allocate space for \p cap items.

    pointer
    alloc(size_type cap) {
      return static_cast<pointer>(std::malloc(cap * sizeof(value_type)));
    }

    void
    dealloc(pointer p) {
      (void) std::free(p);
    }

    /// Expects only to be called if turbovector::capacity `<` \p new_cap.
    /**
     * Will blindly increase the current capacity before checking it against
     * \p new_cap.
     * This saves a few comparisons.
     */

    void
    expand(size_type new_cap) {
      const size_type s = size();
      size_type       c = capacity();
      do {
        c += c; // We always want to hit 2^x.
      } while (c < new_cap);
      pointer b2 = alloc(c);
      (void) std::memcpy(b2, b, s * sizeof(value_type));
      (void) dealloc(b);
      b   = b2;
      e   = b2 + s;
      eof = b2 + c;
    }
  };
}

#endif // RACEHOG_SUPPORT_TURBOVECTOR_H_
