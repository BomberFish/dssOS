# AD-SUPPORT subpolicy is intended to be used in Active Directory
# environments where either accounts or trusted domain objects were not yet
# migrated to AES or future encryption types. Active Directory implicitly
# requires RC4 and MD5 (arcfour-hmac-md5) in Kerberos by default.

cipher@kerberos = RC4-128+
hash@kerberos = MD5+
