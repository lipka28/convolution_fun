#pragma once
#include <memory>

#include "png_image.hpp"


using std::make_unique;
using std::make_shared;
using std::unique_ptr;
using std::shared_ptr;

using sp_Image = shared_ptr<Png_image>;

enum error {
    SUCCES = 0,
    FAIL
};
