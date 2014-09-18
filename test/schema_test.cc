#include "atomdb/schema.h"
#include "gtest/gtest.h"

namespace atomdb {

class SchemaTest : public testing::Test {

};

TEST_F(SchemaTest, BuildItemNormal) {
    ItemBuilder builder;
    int i = 32;
    bool ok = builder.AddAttrValue("id", N, reinterpret_cast<const char*>(&i), sizeof(i));
    EXPECT_TRUE(ok);
    ok = builder.AddAttrValue("name", S, "zhangbiao", 10);
    EXPECT_TRUE(ok);
    ok = builder.AddAttrValue("from", S, "baidu", 6);
    EXPECT_TRUE(ok);

    std::string output;
    ok = builder.Serialize(&output);
    EXPECT_TRUE(ok);
    printf("%s\n", output.c_str());
}

}

int main(int argc, char** argv) {
    testing::InitGoogleTest(&argc, argv);
    return RUN_ALL_TESTS();
}
