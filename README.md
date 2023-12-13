# DABSE-project

## Setup

1. install pbc library: https://github.com/jianda1013/pbc
2. install openssl library
3. make sure all the `gmp.h`, `openssl`, `pbc` are in `/usr/local/include`
4. make sure all the `libcrypto.a`, `libgmp.a`, `libpbc.a`, `libssl.a` are in `/usr/local/lib`

## Example usage

```
cmake .
make
./main
```

## Reference

https://www.sciencedirect.com/science/article/pii/S2214212621000454
