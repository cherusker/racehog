#ifndef RACEHOG_SUPPORT_TRACKINGREFERENCE_H_
#define RACEHOG_SUPPORT_TRACKINGREFERENCE_H_

#include <boost/core/noncopyable.hpp>

#include "Support/Logging.h"

namespace racehog {

  /// A typed pointer to an object (main dish) with an associated id (side).
  /**
   * The general idea of TrackingReference is to enforce strong binding between a
   * referenced object and its (arbitrary) identifier within a system that the
   * object does not care about.
   * While there is no automatic linking (id and reference to the object have to
   * be set individually), TrackingReference heavily suggests parallel setting
   * (TrackingReference::set) and object-only retrieval
   * (TrackingReference::operator* and TrackingReference::operator->).
   *
   * Note:  it is possible to create partially constructed objects
   * (via TrackingReference::TrackingReference() and TrackingReference::setId).
   * Those TrackingReference objects have to be completed via
   * TrackingReference::set before they can be used.
   * The danger of partially constructed objects shall not be unterrated:
   * apart from providing meaningless ids, TrackingReference::operator* and
   * TrackingReference::operator-> could result in `nullptr` dereferencing.
   *
   * TrackingReference has a very small footprint;  nothing else is stored but
   * the id and a (typed) pointer.
   *
   * \tparam IdT        The id type.
   * \tparam ReferenceT The referenced type.
   */

  template<class IdType,
           class ReferenceType>
  class TrackingReference
    : private boost::noncopyable {

    typedef TrackingReference<IdType, ReferenceType> Self;

  public:

    typedef IdType        Id;
    typedef ReferenceType Reference;

    Reference&
    operator*(void) {
      return *ref;
    }

    const Reference&
    operator*(void) const {
      return *ref;
    }

    Reference*
    operator->(void) {
      return ref;
    }

    const Reference*
    operator->(void) const {
      return ref;
    }

    Id
    getId(void) const {
      return id;
    }

    /// Partially set this object (the reference will be set to `nullptr`).
    /**
     * See class description for details.
     */

    void
    setId(Id id) {
      this->id = id;
      ref      = nullptr;
    }

    void
    set(Id         id,
        Reference* ref) {
      this->id  = id;
      this->ref = ref;
    }

    Reference*
    getRaw(void) {
      return ref;
    }

    const Reference*
    getRaw(void) const {
      return ref;
    }

  protected:

    TrackingReference(void) = default;

    TrackingReference(Id         id,
                      Reference* ref)
      : id(id),
        ref(ref) {}

    TrackingReference(const Self& other)
      : id(other.id),
        ref(other.ref) {
      LOG(INFO) << "Copying TrackingReference.";
    }

    TrackingReference(Self&& other)
      : id(other.id),
        ref(other.ref) {
      LOG(INFO) << "Moving TrackingReference.";
      other.ref = nullptr;
    }

    TrackingReference&
    operator=(const Self& other) {
      // Explicitely deleted by `boost::noncopyable`.
      id  = other.id;
      ref = other.ref;
      return *this;
    }

  private:

    Id         id  = 0;
    Reference* ref = nullptr;
  };
}

#endif // RACEHOG_SUPPORT_TRACKINGREFERENCE_H_
