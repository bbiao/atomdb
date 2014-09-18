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

ItemBuilder::ItemBuilder(): offset_(0u) {

}

ItemBuilder::~ItemBuilder() {

}

bool ItemBuilder::AddAttr(const std::string& name,
                          AttrType type,
                          const char* value,
                          size_t value_len) {
    bool ok = name.length() < kAttrNameMaxLength;
    if (ok) {
        for (std::vector<Attr>::const_iterator iter = attrs_.begin();
                iter != attrs_.end(); ++iter) {
            if (name == iter->name) {
                return false;
            }
        }

        Attr attr;
        attr.type = type;
        attr.length = value_len;
        snprintf(attr.name, kAttrNameMaxLength, "%s", name.c_str());
        buffer_.append(value, value_len);
        attr.offset = offset_;
        offset_ += value_len;
        attrs_.push_back(attr);
    }

    return ok;
}

bool item_attr_cmp(const Attr& i, const Attr& j) {
    return strcmp(i.name, j.name);
}

bool ItemBuilder::Serialize(std::string* output) {
    bool ok = true;
    std::sort(attrs_.begin(), attrs_.end(), item_attr_cmp);

    output->assign(buffer_.data(), buffer_.length());
    for (std::vector<Attr>::iterator iter = attrs_.begin(); iter != attrs_.end(); ++iter) {
        output->append(reinterpret_cast<char*>(&(*iter)), sizeof(Attr));
    }

    ItemMeta meta;
    meta.attr_offset = offset_;
    meta.attr_count = attrs_.size();
    output->append(reinterpret_cast<char*>(&meta), sizeof(ItemMeta));
    return ok;
}

void ItemBuilder::Reset() {
    offset_ = 0;
    attrs_.clear();
    buffer_.clear();
}

}//namespace atomdb
