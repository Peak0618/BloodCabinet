#ifndef _M_COMPRESSOR_H_
#define _M_COMPRESSOR_H_

//�ⲿ����
extern void compressor_first_power_delaytime(void);
extern void compressor_start_stop_control(void);
extern void compressor_continue_run_delaytime(void);
extern void compressor_RL_out(void);
extern void coompressor_stop_delaytime(void);
extern void coompressor_force_stop_delaytime(void); 
extern void compressor_freq_ctrl_deal(void);        
extern void compressor_freq_start_delaytime(void);
extern void compressor_init_freq_ctrl(void);
extern void comp2_update_delaytime(void);
extern void compressor_up_down_stop(void);




//------------------------------------------------------------------------------------------
//�ⲿ��־
extern flag_type bflg_compressor;         
       #define   bflg_compressor_runing                    bflg_compressor.bits.bit0  
       #define   bflg_compressor_start_delaytime           bflg_compressor.bits.bit1 //��Ƶѹ����ʼת��������ʱ
       #define   bflg_compressor_full_speed                bflg_compressor.bits.bit2
       #define   bflg_compressor_update_delaytime          bflg_compressor.bits.bit3
       #define   bflg_compressor_first_power_delaytime     bflg_compressor.bits.bit4 //�����ϵ���ʱ
       #define   bflg_defrosting_status                    bflg_compressor.bits.bit5 //��˪״̬
       #define   bflg_compressor_stop_delaytime            bflg_compressor.bits.bit6 //ѹ�����ͣ��ʱ��
       #define   bflg_compressor_continue                  bflg_compressor.bits.bit7 //ѹ����������4h��־
       #define   bflg_compressor_force_stop_delaytime      bflg_compressor.bits.bit8 //ѹ��ǿ��ͣ��ʱ��
       #define   bflg_compressor_continue_delaytime        bflg_compressor.bits.bit9 //ѹ���������м�ʱ
       #define   bflg_up_down_stop                         bflg_compressor.bits.bit10 //����������ͣ��


//------------------------------------------------------------------------------------------
//�ⲿ��������
extern int16_t   gss_para[28];
#define   COMP2_START_FREQ         0  //2000
#define   COMP2_START_TIME         1  //60
#define   COMP2_UPDATE_TIME        2  //80
#define   COMP2_SET_TEMP1          3  //-27
#define   COMP2_FREQ_DELT_K1       4  //50
#define   COMP2_FREQ_DELT_MIN      5  //-20
#define   COMP2_FREQ_DELT_MAX      6  //20
#define   COMP2_FREQ_MIN           7  //2000
#define   COMP2_FREQ_MAX           8  //4500
#define   COMP2_SET_TEMP2          9  //-25
#define   COMP2_FREQ_DELT_K2       10 //50
#define   COMP21_START_FREQ        11 //1200
#define   COMP21_START_TIME        12 //60
#define   COMP21_UPDATE_TIME       13 //80
#define   COMP21_FREQ_DELT_K       14 //50
#define   COMP21_FREQ_DELT_MIN     15 //-2000
#define   COMP21_FREQ_DELT_MAX     16 //2000
#define   COMP21_FREQ_MIN          17 //1600
#define   COMP21_FREQ_MAX          18 //4300
#define   COMP21_FREQ_DELT_TEMP    19 //0
#define   COMP_START_DELT_TEMP     20 //10
#define   COMP_STOP_DELT_TEMP      21 //-60
#define   PT100_DELT_TEMP          22 //0
#define   COMP_STOP_DELAYTIME      23 //480
#define   COMP_DOOR_TDELT          24 //50
#define   COMP_DOOR_FREQ           25 //3500
#define   COMP_RUN_DELAYTIME       26 //600
#define   COMP21_FREQ_SET          27 //1600

extern uint16_t gus_compressor_freq_cnt;               //��ʱ���ļ�������.����Ƶ��f=1/T
extern uint8_t  guc_compressor_first_power_delaytime;  //��ʼ�ϵ���ʱ


#endif
/**********************************END OF THE FILE*************************************************/
