# dkvcpp
A distributed key-value storage built on top of leveldb. The distribution of keys is based on the FNV-hash of a key.

All of the servers are defined in the `config.hpp` file.

## dependencies

This service uses leveldb and httplib for the service to work.