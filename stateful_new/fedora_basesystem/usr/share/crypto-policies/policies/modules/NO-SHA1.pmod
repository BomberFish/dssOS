# This is an example subpolicy dropping the SHA1 hash and signature support

hash = -SHA1
sign = -*-SHA1
sha1_in_certs = 0

# https://fedoraproject.org/wiki/Changes/StrongCryptoSettings3Preview1
# SHA-1 signatures are blocked in OpenSSL in FUTURE only
__openssl_block_sha1_signatures = 1
