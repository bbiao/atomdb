#include "atomdb/schema.h"
#include "gtest/gtest.h"

namespace atomdb {

class SchemaTest : public testing::Test {

};

TEST_F(SchemaTest, BuildItemNormal) {
    ItemBuilder builder;
    int i = 32;
    bool ok = builder.AddAttr("id", N, reinterpret_cast<const char*>(&i), sizeof(i));
    EXPECT_TRUE(ok);
    ok = builder.AddAttr("name", S, "zhangbiao", 10);
    EXPECT_TRUE(ok);
    ok = builder.AddAttr("from", S, "baidu", 6);
    EXPECT_TRUE(ok);

    int j = 0;
    ok = builder.AddAttr("a", N, reinterpret_cast<const char*>(&j), sizeof(j));
    EXPECT_TRUE(ok);

    std::string output;
    ok = builder.Serialize(&output);
    EXPECT_TRUE(ok);
    // sizeof(Attr) = 44, sizeof(ItemMeta) = 8, 4 + 10 + 6 + 4 + 44 * 4 + 8 = 208
    EXPECT_EQ(208u, output.length());

    Item item;
    ok = ParseItem(output, &item);
    EXPECT_TRUE(ok);
    EXPECT_EQ(4u, item.meta->attr_count);
    EXPECT_EQ(24u, item.meta->attr_offset);
    for (uint16_t a = 0; a < item.meta->attr_count; ++a) {
        Attr* attr = item.attrs + a;
        printf("%s %d %u %u\n", attr->name, attr->type, attr->offset, attr->length);
    }
}

TEST_F(SchemaTest, BuildEmptyItem) {
    ItemBuilder builder;
    std::string output;
    bool ok = builder.Serialize(&output);
    EXPECT_TRUE(ok);
    EXPECT_EQ(sizeof(ItemMeta), output.length());
    Item item;
    ok = ParseItem(output, &item);
    EXPECT_TRUE(ok);
    EXPECT_EQ(0u, item.meta->attr_count);
    EXPECT_EQ(0u, item.meta->attr_offset);
}

TEST_F(SchemaTest, AddAttrTwice) {
    ItemBuilder builder;
    bool ok = builder.AddAttr("a", S, "bbiao", 6);
    EXPECT_TRUE(ok);
    ok = builder.AddAttr("a", S, "bbiao", 6);
    EXPECT_FALSE(ok);
}

}

int main(int argc, char** argv) {
    testing::InitGoogleTest(&argc, argv);
    return RUN_ALL_TESTS();
}
