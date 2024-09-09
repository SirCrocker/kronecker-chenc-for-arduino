# kronecker-for-arduino
Kronecker Channel Encoding for use in Arduino projects. Currently only Rank-One detector is implemented.
The implementations uses TMPD-4 Scheme 2 

The implementation assumes that:
- Manchester encoding will be used (subset of BPSK).
- The channel is a Binary Symmetric Channel.
- Only digital data is sent and received (0s and 1s).
- The packets smallest unit is bytes (8 bits).
- Since the modulation with Manchester is a biyective function, the encoding is applied in the soon-to-be-sent bytes, not on the manchester modulated ones.

Check (link to the paper) for more information.

A good readme is coming soon.