#include "atomdb/schema.h"
#include <assert.h>

namespace atomdb {

bool ParseItem(const std::string& data, Item* item) {
    bool ok = true;
    char* value = const_cast<char*>(data.data());
    size_t data_len = data.length();

    item->data = value;
    item->meta = reinterpret_cast<ItemMeta*>(value + (data_len - sizeof(ItemMeta)));
    item->attrs = reinterpret_cast<Attr*>(value + item->meta->attr_offset);

    ItemMeta* meta = item->meta;
    if (meta->attr_offset + meta->attr_count * sizeof(Attr) + sizeof(ItemMeta) != data_len) {
        ok = false;
    }

    return ok;
}

ItemBuilder::ItemBuilder() {

}

ItemBuilder::~ItemBuilder() {

}

bool ItemBuilder::AddAttrValue(const std::string& name, AttrType type, const char* value, size_t value_len) {
    bool ok = name.length() < kAttrNameMaxLength;
    if (ok) {
        AttrValue attr;
        attr.type = type;
        attr.length = value_len;
        snprintf(attr.name, kAttrNameMaxLength, "%s", name.c_str());
        attr.value = value;
        attrs_.push_back(attr);
    }

    return ok;
}

bool item_attr_cmp(const AttrValue& i, const AttrValue& j) {
    return strcmp(i.name, j.name);
}

bool ItemBuilder::Serialize(std::string* output) {
    bool ok = true;
    std::sort(attrs_.begin(), attrs_.end(), item_attr_cmp);

    uint32_t offset = 0;
    for (std::vector<AttrValue>::iterator iter = attrs_.begin(); iter != attrs_.end(); ++iter) {
        output->append(iter->value, iter->length);
        iter->offset = offset;
        offset += iter->length;
    }

    for (std::vector<AttrValue>::iterator iter = attrs_.begin(); iter != attrs_.end(); ++iter) {
        output->append(reinterpret_cast<char*>(&(*iter)), sizeof(Attr));
    }

    ItemMeta meta;
    meta.attr_offset = offset;
    meta.attr_count = attrs_.size();
    output->append(reinterpret_cast<char*>(&meta), sizeof(ItemMeta));
    return ok;
}

}//namespace atomdb
