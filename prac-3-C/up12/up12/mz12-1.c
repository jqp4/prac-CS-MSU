STYPE
bit_reverse(STYPE value)
{
    int len = sizeof(STYPE) * 8 - 1;
    UTYPE v = (UTYPE)value;
    UTYPE u = 0;
    while (len--) {
        u = (u | (v & 1)) << 1;
        v >>= 1;
    }
    u |= v & 1;
    return u;
}
