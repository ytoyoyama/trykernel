#include <trykernel.h>

int usermain(void)
{
    tm_putstring("Start Application\n");

    tk_slp_tsk(TMO_FEVR);       // 初期タスクを待ち状態に
    return 0;
}