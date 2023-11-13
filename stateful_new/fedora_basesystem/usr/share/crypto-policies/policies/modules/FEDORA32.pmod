# If this subpolicy is applied to the DEFAULT policy,
# the result provides the same policy as in the Fedora 32 DEFAULT policy

# DH params size: >= 1023
# SHA1 enabled in signatures
# TLS protocols: TLS >= 1.0 DTLS >= 1.0

group@SSH = FFDHE-1536+ FFDHE-1024+

hash = SHA1+

sign = ECDSA-SHA1+ RSA-PSS-SHA1+ RSA-SHA1+

protocol@TLS = TLS1.1+ TLS1.0+ DTLS1.0+

min_dh_size = 1023

sha1_in_certs = 1
