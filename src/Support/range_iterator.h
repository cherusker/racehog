#ifndef RACEHOG_SUPPORT_RANGEITERATOR_H_
#define RACEHOG_SUPPORT_RANGEITERATOR_H_

namespace racehog {
  
  /// Container for encapsulating a begin and end pointer.
  /**
   * Idea:  http://llvm.org/doxygen/classllvm_1_1iterator__range.html
   */

  template<class iterator>
  class range_iterator final
    : private boost::noncopyable {

  public:

    typedef range_iterator<iterator> self;

    template<class Container>
    range_iterator(Container&& c)
      : b(c.begin()),
        e(c.end()) {}

    range_iterator(iterator&& begin,
                   iterator&& end)
      : b(std::move(begin)),
        e(std::move(end)) {}

    range_iterator(const self& other)
      : b(other.b),
        e(other.e) {
      LOG(INFO) << "Copying range_iterator.";
    }

    range_iterator(self&& other)
      : b(std::move(other.b)),
        e(std::move(other.e)) {
      LOG(INFO) << "Moving range_iterator.";
    }

    iterator
    begin(void) {
      return b;
    }

    iterator
    end(void) {
      return e;
    }

  private:

    iterator b;
    iterator e;
  };
}

#endif // RACEHOG_SUPPORT_RANGEITERATOR_H_
