#include "chronicler.h"

static Chronicler::SharedWidgets shared_singleton = Chronicler::SharedWidgets();

namespace Chronicler
{

    SharedWidgets &shared()
    {
        return shared_singleton;
    }

}
