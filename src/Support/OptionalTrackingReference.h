#ifndef RACEHOG_SUPPORT_OPTIONALTRACKINGREFERENCE_H_
#define RACEHOG_SUPPORT_OPTIONALTRACKINGREFERENCE_H_

#include <boost/core/noncopyable.hpp>

#include "Support/TrackingReference.h"

namespace racehog {

  /// Optional TrackingReference.
  /**
   * All things of TrackingReference also apply to OptionalTrackingReference.
   * In addition to TrackingReference, OptionalTrackingReference tracks
   * whether some part of it has been set yet or not.
   * Note that OptionalTrackingReference::setId is enough to active the _set_
   * state.
   *
   * The current state can be accessed via OptionalTrackingReference::isSet and
   * directly modified via OptionalTrackingReference::unset.
   * Other than that, the _set_ state is modified implicitely by its member
   * functions;  see  descriptions of member function for details.
   *
   * As for the memory footprint:
   * in addition to TrackingReference, only a bool is required to track the
   * state (set/unset).
   *
   * \tparam IdT        The id type.
   * \tparam ReferenceT The referenced type.
   */

  template<class IdType,
           class ReferenceType>
  class OptionalTrackingReference
    : public TrackingReference<IdType, ReferenceType> {

  public:

    typedef TrackingReference<IdType, ReferenceType> Super;
    typedef typename Super::Id                       Id;
    typedef typename Super::Reference                Reference;

    bool
    isSet(void) const {
      return is_set;
    }

    /// `isSet() = true`.
    /**
     * More details at TrackingReference::setId.
     */

    void
    setId(Id id) {
      Super::setId(id);
      is_set = true;
    }

    /// `isSet() = true`.
    /**
     * More details at TrackingReference::set.
     */

    void
    set(Id         id,
        Reference* ref) {
      Super::set(id, ref);
      is_set = true;
    }

    /// `isSet() = false`.

    void
    unset(void) {
      // `setId` is used to set the reference to `nullptr`.
      Super::setId(0);
      is_set = false;
    }

  protected:

    /// `isSet() = false`.
    /**
     * More details at TrackingReference::TrackingReference().
     */

    OptionalTrackingReference(void) = default;

    /// `isSet() = true`.
    /**
     * More details at TrackingReference::TrackingReference(Id, Reference*).
     */

    OptionalTrackingReference(Id         id,
                              Reference* ref)
      : Super(id, ref),
        is_set(true) {}

  private:

    bool is_set = false;
  };
}

#endif // RACEHOG_SUPPORT_OPTIONALTRACKINGREFERENCE_H_
