typedef intptr_t        ngx_int_t;
typedef uintptr_t       ngx_uint_t;
typedef intptr_t        ngx_flag_t;

typedef struct {
    size_t      len;
    u_char     *data;
} ngx_str_t;


typedef struct {
    ngx_str_t   key;
    ngx_str_t   value;
} ngx_keyval_t;


typedef struct {
    unsigned    len:28;

    unsigned    valid:1;
    unsigned    no_cacheable:1;
    unsigned    not_found:1;
    unsigned    escape:1;

    u_char     *data;
} ngx_variable_value_t;
