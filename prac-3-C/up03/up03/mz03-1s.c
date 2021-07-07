int
bitcount(STYPE value)
{
    int v_len = 0;
    UTYPE u_val = (UTYPE)value;
    for (;;) {
        if (u_val & 1) {
            v_len += 1;
        }
        u_val >>= 1;
        if (u_val == 0){
            return v_len;
        }
    }
}
