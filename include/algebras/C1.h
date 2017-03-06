#ifndef SPACE_ALGEBRAS_C1_H
#define SPACE_ALGEBRAS_C1_H

#include "conformal.h"

namespace space {
    namespace algebras {
        template <class Value_>
        struct C1 : public Conformal<Value_, 1> {
            using Base = Conformal<Value_, 1>;
            using E1 = typename Base::template BladeMultivector<0>;
        };
    }
}

#endif
