#ifndef SPACE_ALGEBRAS_C3_H
#define SPACE_ALGEBRAS_C3_H

#include "conformal.h"

namespace space {
    namespace algebras {
        template <class Value_>
        struct C3 : public Conformal<Value_, 2> {
            using Base = Conformal<Value_, 2>;
            using E1 = typename Base::template BladeMultivector<0>;
            using E2 = typename Base::template BladeMultivector<1>;
            using E3 = typename Base::template BladeMultivector<2>;
        };
    }
}

#endif
