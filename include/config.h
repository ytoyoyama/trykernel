#ifndef CONFIG_H
#define CONFIG_H
/* 
 *** Try Kernel v2  コンフィギュレーション定義
*/

#define USE_PICO_W      0       /* 1: Pico W  0: Pico */

#define CNF_MAX_TSKID   32      /* 最大タスク数 */
#define	CNF_MAX_TSKPRI  16      /* 最大タスク優先度 */

#define CNF_MAX_FLGID   8       /* 最大イベントフラグ数 */
#define CNF_MAX_SEMID   8       /* 最大セマフォ数 */
#define	CNF_MAX_MBFID	8		/* 最大セマフォ数 */

#endif  /* CONFIG_H */