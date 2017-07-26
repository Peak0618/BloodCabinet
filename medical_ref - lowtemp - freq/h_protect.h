#ifndef _PROTECT_H_
#define _PROTECT_H_
//------------------------------------------------------------------------------
//�ⲿ��������
extern void protect_high_TUTD_delaytime(void);
extern void protect_low_TUTD_delaytime(void);
extern void protect_high_THW_delaytime(void);
extern void protect_air_alarm_boot_delaytime(void);
extern void protect_air_higt_delaytime(void);
extern void protect_air_low_delaytime(void);
extern void protect_TE_boot_delaytime();
extern void protect_TE_interval_delaytime();
extern void protect_higt_TE_delaytime();








extern void prot2_power_off_deal(void);      //�ϵ籣�������������ѭ�������е���

extern void port2_power_fault_deal(void);    //��Դ����ϴ����������ѭ�������е���

extern void prot2_bat_discnnt_delaytime(void);    //���δ���ӱ�����ʱ������100ms��ʱ�����е���

extern void prot2_bat_space_delaytime(void);      //���δ���Ӽ������ʱ������100ms��ʱ�����е���

extern void prot2_bat_low_deal(void);   //��ص����ͱ��������������ѭ�������е���

extern void prot2_bat_low_delaytime(void);   //��ص������ӳٳ�����1min��ʱ�����е���

extern void port2_door_delaytime(void);      //�ſ��ر�����ʱ������1s��ʱ�����е���

extern void port2_T_fault_deal(void);        //���������ϴ����������ѭ�������е���

extern void prot2_TC_first_delaytime(void);  //���������״α�����ʱ������1min��ʱ�����е���

extern void prot2_cond_dirty_delaytime(void);//�������ౣ����ʱ������1s��ʱ�����е���

extern void prot2_volt_deal(void);      //��ѹ���걨�����������������ѭ�������е���

extern void warning_light_deal(void);      //�����ƴ����������ѭ�������е���

extern void alarm_buzz_deal(void);      //���������������������ѭ�������е���

extern void alarm_buzz_off_delaytime(void);       //����������ȡ����ʱ������1min��ʱ�����е���

extern void bat_T_hight_protect(void);               //����¶ȸ��жϳ���


//------------------------------------------------------------------------------
//�ⲿ��־����   
extern flag_type flg_alarm1;     //�����ı���
       #define  bflg_alarm_high_TUTD                flg_alarm1.bits.bit0      //�����¸��±���
       #define  bflg_alarm_low_TUTD                 flg_alarm1.bits.bit1      //�����µ��±���
       #define  bflg_alarm_high_AIR                 flg_alarm1.bits.bit2      //�������±���
       #define  bflg_alarm_low_AIR                  flg_alarm1.bits.bit3      //�������±���
       #define  bflg_TA_fault                       flg_alarm1.bits.bit4      //�����������ϱ�־  //peak ��PT100 ��Һ�����Ľ��������еõ�
       #define  bflg_TD_fault                       flg_alarm1.bits.bit5      //�´���������
       #define  bflg_TU_fault                       flg_alarm1.bits.bit6      //�ϴ���������
       #define  bflg_alarm_power_off                flg_alarm1.bits.bit7      //�ϵ籨����־         
       #define  bflg_alarm_power_fault              flg_alarm1.bits.bit8      //��Դ����ϱ�����־   
       #define  bflg_alarm_open_door                flg_alarm1.bits.bit9      //���ű�����־       
       #define  bflg_alarm_too_higt_THW             flg_alarm1.bits.bit10     //���¹�����5�濪���������� 
       #define  bflg_alarm_cond_dirty               flg_alarm1.bits.bit11     //�������౨����־     
       //�������������λ�ˣ���Ϊʹ���� flg_alarm4.uword
       //--xxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxx


extern flag_type flg_alarm2;     //�Ƿ����ı���
       #define  bflg_alarm_higt_THW                 flg_alarm2.bits.bit0      //���¹��߱��� 
       #define  bflg_alarm_higt_TE                  flg_alarm2.bits.bit1      //��˪���������±���
       #define  bflg_TE_fault                       flg_alarm2.bits.bit2      //�Ƚ������¶ȴ��������ϱ�־
       #define  bflg_THW_fault                      flg_alarm2.bits.bit3      //���´��������ϱ�־
       #define  bflg_humidity_fault                 flg_alarm2.bits.bit4      //ʪ�ȴ��������� 
       #define  bflg_TC_fault                       flg_alarm2.bits.bit5      //�������¶ȴ��������ϱ�־
       #define  bflg_alarm_bat_discnnt              flg_alarm2.bits.bit6      //���δ���ӱ�����־   
       #define  bflg_alarm_bat_fed                  flg_alarm2.bits.bit7      //��ؽӷ�������־    ����
       #define  bflg_alarm_bat_low                  flg_alarm2.bits.bit8      //��ص����ͱ�����־  ����
       #define  bflg_sterilize_fault                flg_alarm2.bits.bit9      //ɱ��ģ�����
       //�������������λ�ˣ���Ϊʹ���� flg_alarm5.uword
       //--xxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxx

extern flag_type flg_prot1;                                      
       #define   bflg_alarm_THW_high            flg_prot1.bits.bit0   //���¹��߱�����־    ����
       #define   bflg_alarm_volt                flg_prot1.bits.bit1   //��ѹ���걨����־  
       #define   bflg_alarm_bat1_T_hight        flg_prot1.bits.bit2   //���1�¶ȹ���       
       #define   bflg_alarm_bat2_T_hight        flg_prot1.bits.bit3   //���2�¶ȹ���
       #define   bflg_air_alarm_boot_delaytime  flg_prot1.bits.bit4   //��������������ʱ
       #define   bflg_TE_boot_delaytime         flg_prot1.bits.bit5   //��˪�������ж��ϵ���ʱ
       #define   bflg_TE_interval_delaytime     flg_prot1.bits.bit6   //��˪�������ж����
       #define   bflg_alarm_ALH                 flg_prot1.bits.bit7   //���±�����־          
       #define   bflg_alarm_ALL                 flg_prot1.bits.bit8   //���±�����־         
       #define   bflg_alarm_T_fault             flg_prot1.bits.bit9   //���������ϱ�����־: �������¡��������Ƚ�����4�ִ���������һ���й���������й���
	   #define   bflg_alarm_light               flg_prot1.bits.bit10  //��������
//------------------------------------------------------------------------------
extern flag_type flg_prot2;          
       #define   bflg_alarm_lamp                 flg_prot2.bits.bit0    //�����Ʊ�־
       #define   bflg_alarm_buzz                 flg_prot2.bits.bit1    //������������־
       #define   bflg_alarm_buzz_output          flg_prot2.bits.bit2    //�����������־
       #define   bflg_alarm_output               flg_prot2.bits.bit3    //Զ�̱��������־
       #define   bflg_prot2_door1_pin            flg_prot2.bits.bit4    //�ſ���1����״̬��־
       #define   bflg_prot2_door2_pin            flg_prot2.bits.bit5    //�ſ���2����״̬��־
       #define   bflg_prot2_bat_fed_pin          flg_prot2.bits.bit6    //﮵�ط��巴��־
       #define   bflg_prot2_TC_first_delaytime   flg_prot2.bits.bit7    //���������״α�����ʱ��־
       #define   bflg_alarm_buzz_off_delaytime   flg_prot2.bits.bit8    //����������ȡ����ʱ��־  =1��ʾ�Ѱ���"����ȡ��"����30min�ڲ���
       #define   bflg_prot2_THW_high_disp        flg_prot2.bits.bit9    //���¹����¶���ʾ��־
       #define   bflg_prot2_bat_low_delaytime    flg_prot2.bits.bit10   //��ص�������ʱ��־    //peak ��ʼ�ϵ�Ե�س��24Сʱ��
       #define   bflg_prot2_bat_space_delaytime  flg_prot2.bits.bit11   //���δ���Ӽ������ʱ��־
       #define   bflg_prot2_bat_space_reach      flg_prot2.bits.bit12   //���δ���Ӽ������ʱ����־


//------------------------------------------------------------------------------
//macro
//gus_trip_code1
#define   H_T_CODE       0x0001   //bit0  ���±���
#define   L_T_CODE       0x0002   //bit1  ���±���
#define   VAC_OFF_CODE   0x0004   //bit2  �ϵ籨��
#define   DC_F_CODE      0x0008   //bit3  ��Դ����ϱ���
#define   BAT_NO_CODE    0x0010   //bit4  ���δ����
#define   BAT_FED_CODE   0x0020   //bit5  ��ز巴����
#define   BAT_L_CODE     0x0040   //bit6  ��ص����ͱ���
#define   TA_S_CODE      0x0080   //bit7  ���ƴ��������ϱ��� S = sensor
#define   TU_S_CODE      0x0100   //bit8  �ϴ��������ϱ���
#define   TD_S_CODE      0x0200   //bit9  �´��������ϱ���
#define   THW_S_CODE     0x0400   //bit10 ���´��������� -------Һ����Ҫ��ӣ���Ϊ��ͨѶ������
#define   TC_S_CODE      0x0800   //bit11 ��������������
#define   TE_S_CODE      0x1000   //bit12 ��˪(�Ƚ���)����������
#define   TC_COND_CODE   0x2000   //bit13 �������౨��
#define   THW_H_CODE     0x4000   //bit14 ���¹��߱���
#define   AIR_H_CODE     0x8000   //bit15 �������±���
              
//gus_trip_code2
#define   AIR_L_CODE     0x0001   //bit0  �������±���
#define   TE_H_CODE      0x0002   //bit1  ��˪���������±���
#define   OZONE_F_CODE   0x0004   //bit2  ����ģ�鱨�� -------Һ����Ҫ��ӣ���Ϊ��ͨѶ������
#define   DOOR_OP_CODE   0x0008   //bit3  ���ű���

//------------------------------------------------------------------------------
#define   THW_FAULT_CODE      1
#define   TF_FAULT_CODE       2
#define   TC_FAULT_CODE       3
#define   TD_FAULT_CODE       4
#define   TE_FAULT_CODE       5
#define   TA_FAULT_CODE       6
//------------------------------------------------------------------------------
extern int16_t   gss_prot2_TC_first_delaytimer;   //���������״α�����ʱ��ʱ��

extern int16_t   gss_prot2_bat_space_delaytimer;  //���δ���Ӽ������ʱ��ʱ��

extern int16_t   gss_prot2_bat_low_delaytimer;    //��ص�������ʱ��ʱ��

extern int16_t   gss_alarm_buzz_off_delaytimer;   //����������ȡ����ʱ��ʱ��

extern uint16_t  gus_trip_code1;   //���ϴ���1
extern uint16_t  gus_trip_code2;   //���ϴ���2

extern int16_t gss_TE_boot_delaytime;



//------------------------------------------------------------------------------
#endif
