// ****************************************************************************
//
// Copyright (C) 2023 Vailant Group. All rights reserved.
//
// Information contained herein is subject to change without notice.
// Vaillant Group retains ownership and all other rights in the software and
// each component thereof.
// Any reproduction of the software or components thereof without the prior
// written permission of Vaillant Group is prohibited.
//
// ***************************************************************************

#include "gtest/gtest.h"

namespace neext {
namespace bsw {
namespace persistency {

int main(int argc, char **argv) {
    ::testing::InitGoogleTest(&argc, argv);
    return RUN_ALL_TESTS();
}

}  // namespace persistency
}  // namespace bsw
}  // namespace neext
