#ifndef _M_PERIPHERAL_CONTROL_H_
#define _M_PERIPHERAL_CONTROL_H_


//��������
extern void defrosting_control(void);
extern void defrosting_delaytime(void);
extern void defrosting_interval_delaytime(void);
extern void door_heat_control(void);
extern void door_heat_RL_out(void);
extern void inside_fan_control(void);
extern void fan_RL_out(void);
extern void led_control(void);
extern void liquid_led_control(void);
extern void led_on_delaytime(void);
extern void led_RL_out(void);
extern void remote_alarm_lamp(void);
extern void remote_alarm_test_delaytime(void);


//flag
extern flag_type flg_peripheral;
       #define   bflg_defrosting_time                    flg_peripheral.bits.bit0  //��˪ʱ��
       #define   bflg_defrosting_interval_time           flg_peripheral.bits.bit1  //��˪���
       #define   bflg_HT_fault                           flg_peripheral.bits.bit2  //��ʪ�ȴ���������
       #define   bflg_heat_runing                        flg_peripheral.bits.bit3  //�����˿����
       #define   bflg_inside_fan_runing                  flg_peripheral.bits.bit4  //�ڲ������ת
       #define   bflg_led_on                             flg_peripheral.bits.bit5  //led����  
       #define   bflg_led_on_delaytime                   flg_peripheral.bits.bit6  //����������ʱ
       #define   bflg_key_open_led                       flg_peripheral.bits.bit7  //�ǰ������ĵ�
       #define   bflg_key_close_led                      flg_peripheral.bits.bit8  //�ǰ����صĵ�
       #define   bflg_sterilization_state                flg_peripheral.bits.bit9  //ɱ��״̬
       #define   bflg_beam_heat_runing                   flg_peripheral.bits.bit10 //��������˿״̬-----�أ�Ŀǰû��
       #define   bflg_water_box_heat_runing              flg_peripheral.bits.bit11 //��ˮ�м���˿״̬---һֱ��
       #define   bflg_locd_runing                        flg_peripheral.bits.bit12 //�����״̬

extern flag_type flg_peripheral2;
       #define   bflg_remote_alarm_test                 flg_peripheral2.bits.bit1   //��"���Ա���"ʱԶ�̱����Ʋ���


//variare
extern uint8_t  guc_humidity;                         //ʪ��
       
       

//macro
#define   RL_ALM_OUT_PIN             P6.5       //Զ�̱����̵������

//----------------------------------------------------------
//�̵�������ں궨��
#define   RL_COMP1_OUT_PIN    P7.4      //ѹ����1�̵��������     
#define   RL_COMP2_OUT_PIN    P7.5      //ѹ����2�̵��������      

#define   RL_FAN1_OUT_PIN     P7.3      //���1�̵��������           RL7  bflg_outfan20_running
#define   RL_FAN2_OUT_PIN     P4.2      //���2�̵��������         

#define   RL_HTRMXG_OUT_PIN   P4.1      //ëϸ�ܼ���˿�̵��������  

#define   RL_HTRGK_OUT_PIN    P13.0     //��ڼ���˿�̵��������    
 
//#define   RL_ALM_OUT_PIN      P6.5      //Զ�̱����̵��������      =1 �Ǵ򿪱�����  

#define   RL_DOWN_OUT_PIN     P15.6     //��ѹ�̵��������          
#define   RL_UP_OUT_PIN       P10.0     //��ѹ�̵��������          
//----------------------------------------------------------
//12V����ں궨��
//#define   LOCK_OUT_PIN        P11.0   //����������     

#define   HTRMT_OUT_PIN       P11.1     //�������˿�����   
#define   HTRMXG_OUT_PIN      P14.6     //ëϸ�ܼ���˿����� 
#define   HTRPHK_OUT_PIN      P14.7     //ƽ��ڼ���˿����� 
//----------------------------------------------------------
#define   BUZZ_OUT_PIN        P3.1      //�����������       
//----------------------------------------------------------
#define   TFT_OUT_PIN         P10.1     //Һ������Դ�����   
  
//----------------------------------------------------------
//��������ں궨��
#define   DOOR1_IN_PIN        P5.2      //�ſ���1�����  

#define   BAT_FED_IN_PIN      P0.5      //��ز巴�����  
//----------------------------------------------------------
//���뿪������ں궨��
//���뿪������ں궨��
#define   SW11_IN_PIN         P5.1      //���뿪�������
#define   SW12_IN_PIN         P5.0      //���뿪�������
#define   SW13_IN_PIN         P3.0      //���뿪�������
#define   SW14_IN_PIN         P8.7      //���뿪�������
#define   SW15_IN_PIN         P8.6      //���뿪�������
#define   SW16_IN_PIN         P8.5      //���뿪�������
#define   SW17_IN_PIN         P8.4      //���뿪�������
#define   SW18_IN_PIN         P8.3      //���뿪�������

#define   SW21_IN_PIN         P0.6      //���뿪�������
#define   SW22_IN_PIN         P7.0      //���뿪�������
#define   SW23_IN_PIN         P7.1      //���뿪�������
#define   SW24_IN_PIN         P7.2      //���뿪�������







#endif 
