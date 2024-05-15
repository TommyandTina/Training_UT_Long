typedef union lfs       lfs_t;              /* LFS request identifier       */


struct  lfs_devgetdone
{
    int             code;                   /* Command code                 */
    int             type;                   /* Blocking/nonblocking         */
    lfs_t**         ptr;                    /* Ptr to return finished lfs_t */
};

/*
 *  Request union
 */
union   lfs
{
    struct  lfs_devgetdone      r_devgetdone;
};

