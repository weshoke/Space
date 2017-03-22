#ifndef SPACE_ALGEBRAS_E4_H
#define SPACE_ALGEBRAS_E4_H

#include "euclidean.h"

namespace space {
    namespace algebras {
        template <class Value>
        struct E4 : public Euclidean<Value, 4> {
            using Base = Euclidean<Value, 4>;
            using Vec = typename Base::Vec;
        };
    }
}

#endif
