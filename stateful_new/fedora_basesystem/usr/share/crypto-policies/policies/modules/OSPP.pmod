# Restrict FIPS policy for the Common Criteria OSPP profile.

# SSH (upper limit)
# Ciphers: aes128-ctr, aes256-ctr, aes128-cbc, aes256-cbc, aes128-gcm@openssh.com, aes256-gcm@openssh.com
# PubkeyAcceptedKeyTypes: rsa-sha2-256, rsa‑sha2‑512, ecdsa-sha2-nistp256, ecdsa-sha2-nistp384, ecdsa-sha2-nistp521
# MACs: hmac-sha2-256, hmac-sha2-512, implicit for aes128-gcm@openssh.com, aes256-gcm@openssh.com
# KexAlgorithms: ecdh-sha2-nistp256, ecdh-sha2-nistp384, ecdh-sha2-nistp521, diffie-hellman-group14-sha256, diffie-hellman-group16-sha512, diffie-hellman-group18-sha512

# TLS ciphers (suggested minimal set for openssl)
# * TLS_RSA_WITH_AES_128_CBC_SHA     - excluded by FIPS, uses RSA key exchange
# * TLS_RSA_WITH_AES_256_CBC_SHA     - excluded by FIPS, uses RSA key exchange
# * TLS_RSA_WITH_AES_128_CBC_SHA256  - excluded by FIPS, uses RSA key exchange
# * TLS_RSA_WITH_AES_256_CBC_SHA256  - excluded by FIPS, uses RSA key exchange
# * TLS_RSA_WITH_AES_128_GCM_SHA256  - excluded by FIPS, uses RSA key exchange
# * TLS_RSA_WITH_AES_256_GCM_SHA384  - excluded by FIPS, uses RSA key exchange
# * TLS_DHE_RSA_WITH_AES_128_CBC_SHA256
# * TLS_DHE_RSA_WITH_AES_256_CBC_SHA256
# * TLS_DHE_RSA_WITH_AES_128_GCM_SHA256
# * TLS_DHE_RSA_WITH_AES_256_GCM_SHA384
# * TLS_ECDHE_ECDSA_WITH_AES_128_CBC_SHA256
# * TLS_ECDHE_ECDSA_WITH_AES_128_GCM_SHA256
# * TLS_ECDHE_ECDSA_WITH_AES_256_CBC_SHA384  - disabled in openssl itself
# * TLS_ECDHE_ECDSA_WITH_AES_256_GCM_SHA384
# * TLS_ECDHE_RSA_WITH_AES_128_CBC_SHA256
# * TLS_ECDHE_RSA_WITH_AES_128_GCM_SHA256
# * TLS_ECDHE_RSA_WITH_AES_256_CBC_SHA384  - disabled in openssl itself
# * TLS_ECDHE_RSA_WITH_AES_256_GCM_SHA384
# Supported Groups Extension in ClientHello: secp256r1, secp384r1, secp521r1

mac = -HMAC-SHA1  # see above, both SSH and TLS ended up not using it

hash = -SHA2-224 -SHA3-*

sign = -*-SHA2-224

cipher = -AES-*-CCM
cipher@!{ssh,tls} = -AES-*-CTR

ssh_certs = 0
ssh_etm = 0

protocol@TLS = -TLS1.3

arbitrary_dh_groups = 0
