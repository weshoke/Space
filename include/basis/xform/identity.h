#ifndef BASIS_XFORM_IDENTITY_H
#define BASIS_XFORM_IDENTITY_H

#include "blade/product/product.h"
#include "brigand/sequences/list.hpp"

namespace space {
    namespace basis {
        namespace xform {
            struct Identity {
                template <class Metric, class A, class B>
                struct Apply {
                    using type = brigand::list<blade::Product<Metric, A, B>>;
                };
            };
        }
    }
}

#endif
