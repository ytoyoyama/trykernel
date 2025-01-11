#ifndef GPIO_H
#define GPIO_H
/* 
 *** Try Kernel v2  GPIO制御
*/
void gpio_enable_input(UINT no);                // GPIO入力有効
void gpio_enable_output(UINT no, UINT ival);    // GPIO出力有効
void gpio_enable_pullup(UINT no);               // GPIO Pull-Up 有効
void gpio_enable_pulldown(UINT no);             // GPIO Pull-down 有効
void gpio_set_intmode(UINT no, UW mode);        // GPIO割込みモード設定

#endif  /* GPIO_H */