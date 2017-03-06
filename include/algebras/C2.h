#ifndef SPACE_ALGEBRAS_C2_H
#define SPACE_ALGEBRAS_C2_H

#include "conformal.h"

namespace space {
    namespace algebras {
        template <class Value_>
        struct C2 : public Conformal<Value_, 2> {
            using Base = Conformal<Value_, 2>;
            using E1 = typename Base::template BladeMultivector<0>;
            using E2 = typename Base::template BladeMultivector<1>;
        };
    }
}

#endif
