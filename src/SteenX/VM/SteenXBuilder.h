#ifndef STEENX_VM_STEENXBUILDER_H_
#define STEENX_VM_STEENXBUILDER_H_

#include <string>

#include "RaceHog/Module/SearchSpaceBuilderModule.h"
#include "SteenX/Program/Program.h"
#include "Support/Stream/UserStream.h"

namespace racehog {

  /// The search space builder, based on the modified Steensgaard's Analysis.

  class SteenXBuilder final
    : public SearchSpaceBuilderModule {

  public:

    SteenXBuilder(UserStream&        stream,
                  const std::string& filename,
                  bool               is_interactive = false)
      : stream(stream),
        filename(filename),
        is_interactive(is_interactive) {}

    void
    run(void) override;

  private:

    UserStream&        stream;
    const std::string& filename;
    bool               is_interactive;

    /**
     * Program has to be outside of SteenXBuilder::run as other modules depend
     * on its data.
     */

    // TODO:  Fix this dependency;  Program ultimately has to live within
    // SteenXBuilder::run.

    Program prog;
  };
}

#endif // STEENX_VM_STEENXBUILDER_H_
