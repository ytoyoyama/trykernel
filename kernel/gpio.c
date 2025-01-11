/* 
 *** Try Kernel v2  GPIO制御
*/
#include <trykernel.h>

/* GPIO入力有効 */
void gpio_enable_input(UINT no) {
    out_w(GPIO_OE_CLR, 1<<no);      // 出力無効
    set_w(GPIO(no), GPIO_OD);       // 出力無効
    set_w(GPIO(no), GPIO_IE);       // 入力有効
    out_w(GPIO_CTRL(no), 5);        // SIO機能選択
}

/* GPIO出力有効 */
void gpio_enable_output(UINT no, UINT ival) {
    out_w(GPIO_OE_CLR, (1<<no));    // 出力無効
    if(ival) {
        out_w(GPIO_OUT_SET, (1<<no));   // 出力クリア
    } else {
        out_w(GPIO_OUT_CLR, (1<<no));   // 出力クリア
    }
    out_w(GPIO_OE_SET, (1<<no));    // 出力有効
    clr_w(GPIO(no), GPIO_IE);		// 入力無効
    clr_w(GPIO(no), GPIO_OD);		// 出力有効
    out_w(GPIO_CTRL(no), 5);        // SIO機能選択
}

/* GPIO Pull-Up 有効*/
void gpio_enable_pullup(UINT no) {
    out_w(GPIO(no),(in_w(GPIO(no)) | GPIO_PUE) & ~GPIO_PDE);
}

/* GPIO Pull-down 有効 */
void gpio_enable_pulldown(UINT no) {
    out_w(GPIO(no),(in_w(GPIO(no)) | GPIO_PDE) & ~GPIO_PUE);
}

/* GPIO割込みモード設定 */
void gpio_set_intmode(UINT no, UW mode) {
    out_w(PROC0_INTE(no),
            (in_w(PROC0_INTE(no))&~(0x0f<<((no&0x07)*4)))|mode<<((no&0x07)*4));
}
