# As per FIPS 140-3 IG Annex D.Q, EMS is mandatory in TLS 1.2 since 2023-05-16.
# This subpolicy disables this mandatory EMS enforcement of the FIPS policy.
# Doing so violates FIPS requirements, do not use in FIPS-compliant setups.

__ems = RELAX
