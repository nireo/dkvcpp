#include <iostream>
#include <string>
#include <cassert>
#include "leveldb/db.h"
#include "server.hpp"

using namespace std;

int main() {
    Server s;
    s.init_database_connection("/tmp/database");

    string key = "name";
    string value = "test";

    // write
    leveldb::Status status;
    status = s.put(key, value);
    assert(status.ok());

    // read
    status = s.get(key, &value);
    assert(status.ok());

    cout << value << endl;

    // delete
    status = s.del(key);
    assert(status.ok());

    status = s.get(key, &value);
    if (!status.ok()) {
        cerr << key << "    " << status.ToString() << endl;
    } else {
        cout << key << "===" << value << endl;
    }

    return 0;
}