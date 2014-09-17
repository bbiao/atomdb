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

}//namespace atomdb
