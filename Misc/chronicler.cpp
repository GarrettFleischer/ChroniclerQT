#include "chronicler.h"

static Chronicler::SharedInstances shared_singleton = Chronicler::SharedInstances();

namespace Chronicler
{

    SharedInstances &shared()
    {
        return shared_singleton;
    }

}
