#ifndef BASIS_XFORM_IDENTITY_H
#define BASIS_XFORM_IDENTITY_H

#include "blade/product/product.h"
#include "brigand/algorithms/remove.hpp"
#include "brigand/functions/logical/not.hpp"
#include "brigand/sequences/list.hpp"

namespace space {
    namespace basis {
        namespace xform {
            struct Identity {
                template <class Metric, class A, class B, class Op>
                struct Apply {
                    using P = brigand::list<blade::Product<Metric, A, B>>;
                    using R = brigand::remove_if<P, brigand::bind<brigand::not_, Op>>;
                    using type = R;
                };
            };
        }
    }
}

#endif
