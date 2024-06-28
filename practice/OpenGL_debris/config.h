#ifndef __PH_CONFIG_H__
#define __PH_CONFIG_H__

#include "tgaimage.h"
#include <random>

namespace ph
{   
    const auto white  = TGAColor(255, 255, 255, 255);
    const auto black  = TGAColor(0,   0,   0,   0);
    const auto red    = TGAColor(255, 0,   0,   255);
    const auto green  = TGAColor(0,   255, 0,   255);
    const auto blue   = TGAColor(0,   0,   255, 255);

    TGAColor get_random_color() {
        return( TGAColor(
            std::rand() % 256,
            std::rand() % 256,
            std::rand() % 256,
            255
        ));
    }
} // namespace ph

#endif