/* 
 *** Try Kernel v2  A/DC デバイスドライバ
*/

#include <trykernel.h>
#include "dev_adc.h"
#include "adc_sysdep.h"

/*----------------------------------------------------------------------
 * デバイス制御データ
*/
typedef struct {
    ID      semid;      // 排他制御用のセマフォID
    ID      flgid;      // ハンドラからの通知用イベントフラグID
    UW      init;       // 初期化フラグ
    UINT    omode;      // オープンモード
    UW      data;       // リードデータ
} T_ADC_LLDCB;
static T_ADC_LLDCB	ll_devcb;

/*----------------------------------------------------------------------
 * 割り込みハンドラ
 */
void adc_inthdr( UINT intno);
T_DINT dint_adc = {
    .intatr = TA_HLNG,
    .inthdr = adc_inthdr,
};

void adc_inthdr( UINT intno)
{
    ll_devcb.data = in_w(ADC_FIFO)&0x0FFF;
    tk_set_flg(ll_devcb.flgid, 1);
	ClearInt(intno);
}

/*----------------------------------------------------------------------
 * デバイス初期化
 */
static void adc_init(void)
{
    set_w(RESETS_RESET, RESETS_RESET_ADC);
    clr_w(RESETS_RESET, RESETS_RESET_ADC);
    while((in_w(RESETS_RESET_DONE)&(RESETS_RESET_ADC))==0);

/* アナログ入力端子初期化 */
    /* P26 : ADC0 */
    out_w(GPIO_CTRL(26),GPIO_CTRL_FUNCSEL_NULL);
    clr_w(GPIO(26), GPIO_IE|GPIO_PUE|GPIO_PDE);

    /* P27 : ADC1 */
    out_w(GPIO_CTRL(27),GPIO_CTRL_FUNCSEL_NULL);
    clr_w(GPIO(27), GPIO_IE|GPIO_PUE|GPIO_PDE);

    /* P28 : ADC2 */
    out_w(GPIO_CTRL(28),GPIO_CTRL_FUNCSEL_NULL);
    clr_w(GPIO(28), GPIO_IE|GPIO_PUE|GPIO_PDE);

    /* P29 : ADC3 */
//   out_w(GPIO_CTRL(29),GPIO_CTRL_FUNCSEL_NULL);
//   clr_w(GPIO(29), GPIO_IE|GPIO_PUE|GPIO_PDE);

    /* A/DC有効 */
    set_w(ADC_CS, ADC_CS_EN);
    while(!(in_w(ADC_CS)&ADC_CS_READY));
}

/*----------------------------------------------------------------------
 * デバイスオープン関数
 */
ER dev_adc_open(UW unit, UINT omode)
{
    T_CSEM  csem = {
        .isemcnt    = 1,
        .maxsem     = 1,
    };
    T_CFLG  cflg = {
        .flgatr     = TA_TFIFO | TA_WMUL,
        .iflgptn    = 0,
    };
    ER  err;

    if(omode & TD_WRITE) return E_PAR;  // オープンモードが書込みは不可
    if(ll_devcb.init) return E_OK;      // 初期化済み

    err = tk_cre_sem(&csem);        // 排他制御用のセマフォ生成
    if(err < E_OK) return err;
    ll_devcb.semid = (ID)err;

    err = tk_cre_flg(&cflg);        // 割り込みハンドラからの通知用イベントフラグ生成
    if(err < E_OK) return err;
    ll_devcb.flgid = (ID)err;

    tk_def_int(INTNO_ADC, &dint_adc);   // 割り込みハンドラの登録

    ll_devcb.omode = omode;
    ll_devcb.init = TRUE;

    adc_init();                                         // A/DC初期化
    out_w(ADC_DIV, ADC_DIV_INI);                        // クロック設定
    out_w(ADC_FCS, 1<<ADC_FCS_THRESH_POS | ADC_FCS_EN); // FIFO設定

    return E_OK;
}

/*----------------------------------------------------------------------
 * デバイスリード関数
 */
ER dev_adc_read(UW unit, UW ch, UW *buf, SZ size, SZ *asize)
{
    UINT    flgptn;
    ER      err;

    if(ch<0 || ch>(ADC_CH_NUM-1)) return E_PAR;
    if(size != 1) return E_PAR;

    err = tk_wai_sem( ll_devcb.semid, 1, TMO_FEVR);             // 排他の開始
    if(err < E_OK) return err;

    while((in_w(ADC_CS)&ADC_CS_READY)==0);                      // ADCのレディ待ち

    ClearInt(INTNO_ADC);
    EnableInt(INTNO_ADC, INTPRI_ADC);                           // 割り込み有効
    out_w(ADC_INTE, 1);
    out_w(ADC_CS, ch<<ADC_CS_AINSEL_POS|ADC_CS_EN);
    set_w(ADC_CS, ADC_CS_STRAT_ONCE);                           // AD変換の開始

    tk_wai_flg(ll_devcb.flgid, 1, TWF_ANDW | TWF_CLR, &flgptn, TMO_FEVR); // 変換完了待ち

    DisableInt(INTNO_ADC);                                      // 割り込み無効
    *buf = ll_devcb.data;                                       // 変換値の取得
    if(asize != NULL) *asize = size;

    tk_sig_sem( ll_devcb.semid, 1);                             // 排他の終了
    return E_OK;
}
