#include "makemeglow/glow.h"

#include <gtest/gtest.h>
 
TEST(FooTest, SampleTest) { 
    ASSERT_EQ(42, glow::foo());
}
 
int main(int argc, char **argv) {
    testing::InitGoogleTest(&argc, argv);
    return RUN_ALL_TESTS();
}