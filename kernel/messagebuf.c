/* 
 *** Try Kernel v2  メッセージバッファ
*/

#include <trykernel.h>
#include <knldef.h>

MBFCB	mbfcb_tbl[CNF_MAX_MBFID];         /* メッセージバッファ管理ブロック(FLGCB) */

/* メッセージバッファの生成API */
ID tk_cre_mbf( const T_CMBF *pk_cmbf)
{
    ID      mbfid;
    UINT    intsts;

    /* パラメータチェック */
    if((pk_cmbf->bufsz <= 0) || (pk_cmbf->maxmsz <= 0) || (pk_cmbf->maxmsz >= 256)
            || (pk_cmbf->bufsz < pk_cmbf->maxmsz + sizeof(UB)) || (pk_cmbf->bufptr == NULL)) {
        return E_PAR;
    }

    DI(intsts);     // 割込み禁止
    for(mbfid = 0; mbfcb_tbl[mbfid].state != KS_NONEXIST; mbfid++);

    if(mbfid < CNF_MAX_FLGID) {
        /* メッセージバッファ管理情報の初期化 */
        mbfcb_tbl[mbfid].state  = KS_EXIST;
        mbfcb_tbl[mbfid].bufsz      = pk_cmbf->bufsz;
        mbfcb_tbl[mbfid].maxmsz     = pk_cmbf->maxmsz;
        mbfcb_tbl[mbfid].bufptr     = pk_cmbf->bufptr;

        mbfcb_tbl[mbfid].freesz = pk_cmbf->bufsz;;
        mbfcb_tbl[mbfid].buf_rp = mbfcb_tbl[mbfid].buf_wp = pk_cmbf->bufptr;
        mbfid++;
    } else {
        mbfid = E_LIMIT;
    }
    EI(intsts);      // 割込み許可
    return mbfid;
}

/* バッファへのメッセージの格納 */
static void store_msg( MBFCB *mbfcb,  const void *msg, INT msgsz)
{
    UB      *src, *dst, *limit;
    INT     i;

    src = (UB*)msg;
    dst = mbfcb->buf_wp;
    limit = mbfcb->bufptr + mbfcb->bufsz;

    *dst++ = (UB)msgsz;
    for( i = msgsz; i > 0; i--) {
        *dst++ = *src++;
        if(dst >= limit) dst = mbfcb->bufptr;
    }
    
    mbfcb->freesz -= msgsz + sizeof(UB);
    mbfcb->buf_wp = dst;
}

/* バッファからメッセージを取得 */
static INT retrieve_msg( MBFCB *mbfcb,  const void *msg)
{
    UB      *src, *dst, *limit;
    INT     msgsz;
    INT     i;

    src = mbfcb->buf_rp;
    dst = (UB*)msg;
    limit = mbfcb->bufptr + mbfcb->bufsz;

    msgsz =  *src++;
    for( i = msgsz; i > 0 ; i--) {
        *dst++ = *src++;
        if(src >= limit) src = mbfcb->bufptr;
    }

    mbfcb->freesz += msgsz + sizeof(UB);
    mbfcb->buf_rp = src;

    return msgsz;
}

/* メッセージバッファへの送信API */
ER tk_snd_mbf( ID mbfid, const void *msg, INT msgsz, TMO tmout)
{
    MBFCB   *mbfcb;
    TCB     *tcb;
    UINT    intsts;
    INT     rcvsz;
    ER      err     = E_OK;

    if(mbfid <= 0 || mbfid > CNF_MAX_MBFID) return E_ID;

    DI(intsts);     // 割込み禁止
    mbfcb = &mbfcb_tbl[--mbfid];

    if(mbfcb->state == KS_EXIST) {
        if(mbfcb->freesz > msgsz + sizeof(UB)) {    // バッファに空きがある
            store_msg( mbfcb, msg, msgsz);          //　バッファへメッセージを格納
            
            for( tcb = wait_queue; tcb != NULL; tcb = tcb->next) {
                if((tcb->waifct == TWFCT_MBFR) && (tcb->waiobj == mbfid)) {
                    rcvsz = retrieve_msg( mbfcb,  tcb->msg);
                    
                    tqueue_remove_entry( &wait_queue, tcb);                 // タスクをウェイトキューから外す
                    tcb->state	= TS_READY;
                    tcb->waifct	= TWFCT_NON;
                    *(tcb->waierr) = rcvsz;
                    tqueue_add_entry( &ready_queue[tcb->itskpri], tcb);     // タスクをレディキューへつなぐ
                    scheduler();                                            // スケジューラを実行
                    break;
                }
            }
        } else {                                    // バッファに空きが無いので送信待ち状態に移行
            tqueue_remove_top(&ready_queue[cur_task->itskpri]);     // タスクをレディキューから外す

            /* TCBの各種情報を変更する */
            cur_task->state     = TS_WAIT;      // タスクの状態を待ち状態に変更
            cur_task->waifct    = TWFCT_MBFS;   // 待ち要因を設定
            cur_task->waiobj    = mbfid;        // 待ちメッセージバッファIDを設定
            cur_task->waitim    = ((tmout == TMO_FEVR)? tmout: tmout + TIMER_PERIOD);    // 待ち時間を設定
            cur_task->msgsz     = msgsz;        // メッセージのサイズ
            cur_task->msg       = msg;          // メッセージ
            cur_task->waierr    = &err;

            tqueue_add_entry(&wait_queue, cur_task);                // タスクをウェイトキューに繋ぐ
            scheduler();                                            // スケジューラを実行
        }
    } else {
        err = E_NOEXS;
    }

    EI(intsts);      // 割込み許可
    return err;
}

/* メッセージバッファから受信API */
INT tk_rcv_mbf( ID mbfid, void *msg, TMO tmout)
{
    MBFCB   *mbfcb;
    TCB     *tcb;
    UINT    intsts;
    INT     msgsz   = 0;
    ER      err     = E_OK;

    if(mbfid <= 0 || mbfid > CNF_MAX_MBFID) return E_ID;

    DI(intsts);     // 割込み禁止
    mbfcb = &mbfcb_tbl[--mbfid];

    if(mbfcb->state == KS_EXIST) {
        if(mbfcb->buf_rp != mbfcb->buf_wp) {    // メッセージが格納されている
            msgsz = retrieve_msg( mbfcb,  msg);     // バッファからメッセージを取得

            for( tcb = wait_queue; tcb != NULL; tcb = tcb->next) {
                if((tcb->waifct == TWFCT_MBFS) && (tcb->waiobj == mbfid)) {
                    if(tcb->msgsz <= mbfcb->freesz) {
                        store_msg(mbfcb, tcb->msg, tcb->msgsz);

                        tqueue_remove_entry( &wait_queue, tcb);                 // タスクをウェイトキューから外す
                        tcb->state	= TS_READY;
                        tcb->waifct	= TWFCT_NON;
                        tqueue_add_entry( &ready_queue[tcb->itskpri], tcb);     // タスクをレディキューへつなぐ
                        scheduler();                                            // スケジューラを実行
                    }
                    break;
                }
            }
        } else {                                // メッセージが無いので受信待ち状態に移行
            tqueue_remove_top(&ready_queue[cur_task->itskpri]);     // タスクをレディキューから外す

            /* TCBの各種情報を変更する */
            cur_task->state     = TS_WAIT;      // タスクの状態を待ち状態に変更
            cur_task->waifct    = TWFCT_MBFR;   // 待ち要因を設定
            cur_task->waiobj    = mbfid;        // 待ちメッセージバッファIDを設定
            cur_task->waitim    = ((tmout == TMO_FEVR)? tmout: tmout + TIMER_PERIOD);    // 待ち時間を設定
            cur_task->msg       = msg;          // メッセージを格納する領域
            cur_task->waierr    = &err;

            tqueue_add_entry(&wait_queue, cur_task);                // タスクをウェイトキューに繋ぐ
            scheduler();                                            // スケジューラを実行
        }
    } else {
        err = E_NOEXS;
    }

    EI(intsts);      // 割込み許可
    return (msgsz? msgsz:(INT)err);
}
