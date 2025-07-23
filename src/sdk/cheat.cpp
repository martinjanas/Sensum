#include "cheat.h"

namespace cheat
{
    Console& console()
    {
        static Console instance;
        return instance;
    }

    Interfaces& interfaces()
    {
        static Interfaces instance;
        return instance;
    }
}
