#ifndef _KEY_DISP_H_
#define _KEY_DISP_H_
//------------------------------------------------------------------------------
//function
extern void read_key_deal(void);   //����ֵ�����������ѭ�������е���

extern void led_disp1_deal(void);  //�����1��ʾ���������100ms��ʱ�����е���
extern void led_lamp_disp(void);

extern void led_disp2_deal(void);  //�����2��ʾ���������100ms��ʱ�����е���

extern void led_disp3_deal(void);  //�����3��ʾ���������100ms��ʱ�����е���

extern void read_key_delaytime(void);   //������ʱ������10ms��ʱ�����е���
//peak add
extern void  key_buzz_cancel(void);
extern void bat_con_check_again(void);     //  ����ȡ�������󣬵��ٴμ�⵽��أ��ָ��������ƣ� 
extern void led_loop_delaytime(void);                 //LED��˸��־
extern void led_disp1_flicker_delaytime(void);   //�趨ʱ������˸
extern void view_error_code_delaytime(void);      //��ʾ�������ʱ�� 
extern void key_factory_AT_test(void);
extern void  key_factory_init(void);                  //�ָ�����ֵ
extern void system_self_test(void);                   //��ʼ�ϵ�ʱ���Լ�
extern void led_disp_alternate_delaytime(void);
extern void led_disp_black_screen_delaytime(void);


//flag
extern flag_type flg_disp;         
          #define   bflg_key_locked          flg_disp.bits.bit0  //����������־
          #define   bflg_key_long_valid      flg_disp.bits.bit1  //��������Ч��־
          #define   bflg_key_long_delaytime  flg_disp.bits.bit2  //��������ʱ��־
          #define   bflg_disp1_val           flg_disp.bits.bit3  //��ֵ��ʾ��Ч��־
          #define   bflg_disp_minus          flg_disp.bits.bit4  //��ʾ������־
          #define   bflg_read_key_delaytime  flg_disp.bits.bit5  //����ֵ��ʱ��־
          #define   bflg_eep_init            flg_disp.bits.bit6  //EEP��ʼ����־       
          //peak  add
		  #define   bflg_key_long_buz_delaytime     flg_disp.bits.bit7   //���� "����ȡ��" ������ʱ��־ -�ڵ��δ����ʱ����buzz����ʱ
		  #define   bflg_key_long_buz_valid         flg_disp.bits.bit8   // ���� "����ȡ��" ������Ч��־
		  #define   flag_view_error_code            flg_disp.bits.bit9   // ��ʾ����������Ч��־
		  #define   bflg_led_loop_set               flg_disp.bits.bit10  // led ��˸�ı�־
		  #define   bflg_error_code_delaytime       flg_disp.bits.bit11  // ���������ʾʱ�䳤��
		  #define   bflg_key_long_buz_valid_ok      flg_disp.bits.bit12  // ���� 5s ִ�б��,�Ƿ������־
		  #define   bflg_IC_register_state          flg_disp.bits.bit13  //�Ƿ����ע��״̬
          #define   bflg_IC_erasure                 flg_disp.bits.bit14  //ע�����е�IC������û�з�ע���������ʾ���ж�BUZ����5s����Զ�ע��������ֻ�Ǵﵽ5s��ʾ�¶�
          #define   bflg_IC_erasure_delaytime       flg_disp.bits.bit15
          
extern flag_type flg_disp1;   
        #define   bflg_disp1_flicker               flg_disp1.bits.bit0  //��˸���
        #define   bflg_set2_val                    flg_disp1.bits.bit1  //��user2ģʽ�еı�־
        #define   bflg_factory_val                 flg_disp1.bits.bit2  //��factory����ģʽ���Ƿ�set��־
        #define   bflg_IC_pass_CL1                 flg_disp1.bits.bit3  //���ICע���Ĳ�����־
        #define   bflg_factory_AT_test             flg_disp1.bits.bit4  //��factory����ģʽ��ATָ��Ҫִ�еı�־��
        #define   bflg_factory_CL4                 flg_disp1.bits.bit5  //��factory����ģʽ��CL4ָ��Ҫִ�еı�־--�ָ���������
        #define   bflg_IC_erasure_order            flg_disp1.bits.bit6  //ָ��CL1��CL3��ע�������Ѿ�ע���IC��ָ���־
        #define   bflg_key_long_down_valid         flg_disp1.bits.bit7  //����״̬ʱ����down���Ƿ�ﵽ5s�ı�־
        #define   bflg_key_long_down_delaytime     flg_disp1.bits.bit8  //����״̬����down������ʼ��ʱ��־��
        #define   bflg_debug_val                   flg_disp1.bits.bit9  //��������ģʽ����ʾ����ֵ���ǲ���
        #define   bflg_prepare_deal                flg_disp1.bits.bit10 //����set����������Ҫ�ɿ�һ��set���ſ��Լ���������         
        #define   bflg_set1_val                    flg_disp1.bits.bit11 //��user1ģʽ�еı�־

extern flag_type flg_error_code_mark;   //��������Ѷ����
         #define   bflg_error_high_TUTD_mark        flg_error_code_mark.bits.bit0   // 1
		 #define   bflg_error_low_TUTD_mark         flg_error_code_mark.bits.bit1   // 2
         #define   bflg_error_higt_THW_mark         flg_error_code_mark.bits.bit2   // 3
		 #define   bflg_error_high_AIR_mark         flg_error_code_mark.bits.bit3   // 4
		 #define   bflg_error_low_AIR_mark          flg_error_code_mark.bits.bit4   // 5
		 #define   bflg_error_higt_TE_mark          flg_error_code_mark.bits.bit5   // 6
		 #define   bflg_error_TA_fault_mark         flg_error_code_mark.bits.bit6   // 7
		 #define   bflg_error_TE_fault_mark         flg_error_code_mark.bits.bit7   // 8
		 #define   bflg_error_THW_humidity_mark     flg_error_code_mark.bits.bit8   // 9
		 #define   bflg_error_TC_fault_mark         flg_error_code_mark.bits.bit9   // 10 
		 #define   bflg_error_TU_fault_mark         flg_error_code_mark.bits.bit10  // 11    
         #define   bflg_error_TD_fault_mark         flg_error_code_mark.bits.bit11  // 12  
         #define   bflg_error_bat_discnnt_mark      flg_error_code_mark.bits.bit12  // 13 
		 #define   bflg_error_bat_fed_mark          flg_error_code_mark.bits.bit13  // 14
         #define   bflg_error_bat_low_mark          flg_error_code_mark.bits.bit14  // 15
         //*****************can't add,use'd .uword****************************************
         
extern flag_type flg_error_code_mark1;    
        #define   bflg_error_power_off_mark         flg_error_code_mark1.bits.bit0   // 16
        #define   bflg_error_power_fault_mark       flg_error_code_mark1.bits.bit1   // 17
        #define   bflg_error_open_door_mark         flg_error_code_mark1.bits.bit2   // 18
        #define   bflg_error_cond_dirty_mark        flg_error_code_mark1.bits.bit3   // 19
        #define   bflg_error_sterilize_fault_mark   flg_error_code_mark1.bits.bit4   // 20
        //*****************can't add,use'd .uword****************************************


        
extern flag_type   flg_save2;
        #define   bflg_self_test                    flg_save2.bits.bit0   //ϵͳ�Լ��־
        #define   bflg_self_test_flicker            flg_save2.bits.bit1   //ϵͳ�Լ���˸��־
        #define   bflg_self_test_sound              flg_save2.bits.bit2   //ϵͳ�Լ���һ��
        #define   bflg_disp_alternate               flg_save2.bits.bit3   //���������¶Ƚ�����ʾ
        #define   bflg_disp_black_screen            flg_save2.bits.bit4   //�ϵ�ʱ�������¶Ƚ���

//macro
#define   LED1      guc_led_buffer[0]
#define   LED2      guc_led_buffer[1]
#define   LED3      guc_led_buffer[2]
#define   LED4      guc_led_buffer[3]
#define   LED5      guc_led_buffer[4]
#define   LED6      guc_led_buffer[5]
#define   LED7      guc_led_buffer[6]
#define   LED8      guc_led_buffer[7]   //lamp
#define   LED9      guc_led_buffer[8]
#define   LED10     guc_led_buffer[9]
#define   LED11     guc_led_buffer[10]
#define   LED12     guc_led_buffer[11]  //lamp

//array
extern int16_t   gss_disp_see_buffer[6];
extern uint8_t   guc_led_buffer[12];     

//variare
extern int16_t   gss_read_key_delaytimer; 
extern uint8_t   guc_key_val;            
extern int8_t    guc_IC_password_cnt;          

//------------------------------------------------------------------------------
#endif
