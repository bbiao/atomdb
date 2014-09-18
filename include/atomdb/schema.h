#ifndef __ATOMDB_SCHEMA_H
#define __ATOMDB_SCHEMA_H

#include <stdint.h>
#include <string>
#include <vector>

namespace atomdb {

enum AttrType {
    N,  //number
    NS, //number set
    S,  //string
    SS, //string set
    B,  //binary
    BS, //binary set
};

const uint32_t kAttrNameMaxLength = 32;

struct Attr {
    AttrType type;
    uint32_t offset;
    uint32_t length;
    char name[kAttrNameMaxLength];
};

struct ItemMeta {
    uint32_t attr_offset;
    uint32_t attr_count;
};

struct Item {
    const char* data;
    Attr* attrs;
    ItemMeta* meta;
};

bool ParseItem(const std::string& data, Item* item);

class ItemBuilder {
public:
    ItemBuilder();
    virtual ~ItemBuilder();

    bool AddAttr(const std::string& name, AttrType type, const char* value, size_t value_len);
    bool Serialize(std::string* output);
private:
    std::vector<Attr> attrs_;
    std::string buffer_;
    uint32_t offset_;
};

}//namespace atomdb
#endif//__ATOMDB_SCHEMA_H
