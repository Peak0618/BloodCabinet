#ifndef _M_KEY_OPERATION_H_
#define _M_KEY_OPERATION_H_

//�ⲿ����
extern void mode_selection(void);
extern void key_locked_delaytime(void);   
extern void key_set_long_delaytime(void);
extern void key_set_up_long_delaytime(void);
extern void key_up_down_long_delaytime(void);
extern void key_set_down_long_delaytime(void);
extern void operation_parm_read(void);
extern void IC_unlock_deal(void);
extern void IC_unlock_delaytime(void);
extern void key_password_long_valid_delaytime(void);
extern void key_RTC_time_parm_sent_usb_delaytime(void); 



//����ֵ
#define   KEY_NONE              0x00     //�ް�������
#define   KEY_SET               0x08     //�趨
#define   KEY_UP                0x10     //�ϵ�
#define   KEY_DOWN              0x01     //�µ�
#define   KEY_BUZ               0x02     //��������
#define   KEY_LAMP              0x04     //��
#define   KEY_SET_AND_UP        0x18     //�趨+�ϵ�  ����
#define   KEY_UP_AND_DOWN       0x11     //�ϵ�+�µ�  ��ѯ
#define   KEY_SET_AND_DOWN      0x09     // IC����



//����_��
extern uint8_t   guc_disp_mode;
#define   DISP_LOCKED                  0   //����ģʽ
#define   DISP_USER_SET                1   //�û��趨��
#define   DISP_TIME_SET                2   //usbʱ������
#define   DISP_USER_MANAGE_PASSWORD    3   //�û����������
#define   DISP_USER_MANAGE             4   //�û������
#define   DISP_FACTORY_PASSWORD        5   //��������ģʽ
#define   DISP_FACTORY                 6   //����ģʽ
#define   DISP_INQUIRE                 7   //��ѯģʽ
#define   DISP_IC_PASSWORD             8   //IC������ģʽ
#define   DISP_IC_REGISTER             9   //IC��ע��ģʽ

extern int8_t   gsc_disp_parm_type;
//��֧_�û��趨�����
#define   DISP_USER_SET_TS                  0   //�����¶��趨ֵ         magnify 10 ʹ��ʱ����Ŵ�10
#define   DISP_USER_SET_ALH                 1   //���±���ֵ           magnify 10
#define   DISP_USER_SET_ALL                 2   //���±���ֵ           magnify 10
#define   DISP_USER_SET_T1                  3   //usbȡ������ 
#define   DISP_USER_SET_T2                  4   //usbʱ��
#define   DISP_USER_SET_P6                  5   //usb����ģʽ
#define   DISP_USER_SET_SS                  6   //�û����������趨
#define   DISP_USER_SET_SUM                 7   //�û��趨������ܸ���

//��֧_�û���������
#define   DISP_USER_MANAGE_IC               0  //ic��ע��/�û�����������޸�
#define   DISP_USER_MANAGE_CL1              1  //ic��ע��--�������Ϊ0����Ϊ����Ĭ��ʱΪ0����IC���봦���׻�
#define   DISP_USER_MANAGE_Lt               2  //������������ʱ�ر�ʱ��orȡ���ر���ʱ
#define   DISP_USER_MANAGE_dA               3  //���ŷ�����������ʱ
#define   DISP_USER_MANAGE_FA               4  //����������
#define   DISP_USER_MANAGE_S1               5  //ɱ��ģʽ
#define   DISP_USER_MANAGE_S2               6  //���ɱ��ʱ��
#define   DISP_USER_MANAGE_TT               7  //�¶ȵ�λ
#define   DISP_USER_MANAGE_SUM              8  //������������

//��֧_��������
#define  DISP_FAC_CL2                       0   //usb������������
#define  DISP_FAC_UL                        1   //usb������������ѡ��
#define  DISP_FAC_r0                        2   //�����²�                magnify 10
#define  DISP_FAC_b0F                       3   //��Ƶѹ��ת�ٿ����²�           use need magnify 10
#define  DISP_FAC_br1                       4   //��Ƶѹ��ת�ٱ���1
#define  DISP_FAC_br2                       5   //��Ƶѹ��ת�ٱ���2
#define  DISP_FAC_br3                       6   //��Ƶѹ��ת�ٱ���3
#define  DISP_FAC_br4                       7   //��Ƶѹ��ת�ٱ���4
#define  DISP_FAC_Ct1                       8   //�ϴ�����У׼ֵ             magnify 10
#define  DISP_FAC_Ct2                       9   //�´�����У׼ֵ             magnify 10
#define  DISP_FAC_Ct3                       10  //���ƴ�����У׼ֵ            magnify 10
#define  DISP_FAC_Ct4                       11  //��˪(�Ƚ���)������У׼ֵ            magnify 10
#define  DISP_FAC_Ct5                       12  //������������У׼ֵ           magnify 10 
#define  DISP_FAC_Ct6                       13  //���´�����У׼ֵ            magnify 10
#define  DISP_FAC_Ct7                       14  //ʪ�ȴ�����У׼ֵ
#define  DISP_FAC_CU                        15  //�е��ѹУ׼
#define  DISP_FAC_Ad                        16  //���������ӳ�
#define  DISP_FAC_AAH                       17  //�������±���ֵ                use need magnify 10��������ܲ�����ʾ�ͷ��㰴��++����ʹ����Щ����ʱ��Ҫ�Ŵ�10����Ϊ�¶ȶ��Ŵ���10��
#define  DISP_FAC_AAL                       18  //�������±���ֵ                use need magnify 10
#define  DISP_FAC_r1                        19  //�������౨���뻷���²�            use need magnify 10
#define  DISP_FAC_AHH                       20  //���¹��߱���                 use need magnify 10
#define  DISP_FAC_T3                        21  //ѹ�������ͣ��ʱ��     
#define  DISP_FAC_Cd                        22  //��ʼ�ϵ�ѹ��������ʱ
#define  DISP_FAC_Fd                        23  //������ȡ�������������ʱ��              use need magnify 10
#define  DISP_FAC_LC                        24  //���������ƿ��Ʒ�ʽ������
#define  DISP_FAC_CL3                       25  //ic��ע������/�û�����������ʼ��
#define  DISP_FAC_CL4                       26  //�ָ�����ֵ
#define  DISP_FAC_dFL                       27  //���뻯˪�¶�                 use need magnify 10
#define  DISP_FAC_dFH                       28  //�˳���˪�¶�                 use need magnify 10
#define  DISP_FAC_HT1                       29  //�ż�������ʪ��
#define  DISP_FAC_HT2                       30  //�ż���ֹͣʪ��
#define  DISP_FAC_PS                        31  //����--����
#define  DISP_FAC_PCH                       32  //ѪҺ��ҩƷ������л�
#define  DISP_FAC_SUM                       33  //�������������

//��֧_��ѯ����
extern int8_t  gsc_disp_inq_mode;
#define  DISP_INQ_d1                        0   //�ϴ�����
#define  DISP_INQ_d2                        1   //�´�����
#define  DISP_INQ_d3                        2   //���ƴ�����
#define  DISP_INQ_d4                        3   //��˪������
#define  DISP_INQ_d5                        4   //����������
#define  DISP_INQ_d6                        5   //���´�����
#define  DISP_INQ_d7                        6   //����ʪ��
#define  DISP_INQ_d8                        7   //�е��ѹ
#define  DISP_INQ_d9                        8   //��ص���



//�ⲿ��־
extern flag_type bflg_key_operation;
       #define   bflg_key_set_long_valid          bflg_key_operation.bits.bit0   //����set����Ч
       #define   bflg_key_set_long_delaytime      bflg_key_operation.bits.bit1   //����set����ʱ
       #define   bflg_key_set_up_long_valid       bflg_key_operation.bits.bit2   //����set+up����Ч
       #define   bflg_key_set_up_long_delaytime   bflg_key_operation.bits.bit3   //����set+up����ʱ
       #define   bflg_key_up_down_long_valid      bflg_key_operation.bits.bit4   //����up+down����Ч
       #define   bflg_key_up_down_long_delaytime  bflg_key_operation.bits.bit5   //����up+down����ʱ
       #define   bflg_key_set_down_long_valid     bflg_key_operation.bits.bit6   //����set+down����Ч
       #define   bflg_key_set_down_long_delaytime bflg_key_operation.bits.bit7   //����set+down����ʱ
       #define   bflg_parm_dis_form               bflg_key_operation.bits.bit8   //��ʾ 0:������ʶ or 1:����ֵ
       #define   bflg_key_inq_val                 bflg_key_operation.bits.bit9   //��ѯ��������ֵ
       
extern flag_type bflg_key_operation_save;
       #define   bflg_parm_save                         bflg_key_operation_save.bits.bit0   //�Ƿ�Բ������� 1δ���� 0����
       #define   bflg_usb_set_time_delaytime            bflg_key_operation_save.bits.bit1   //usb����ʱ����ʱ
       #define   bflg_usb_set_time                      bflg_key_operation_save.bits.bit2   //��ʾ������ʱ�䶯����usb�޸�ʱ��
       #define   bflg_key_password_long_valid           bflg_key_operation_save.bits.bit3   //ICע�ᡢ����������3�����볤��xs�󰴼�10�Ĳ�������
       #define   bflg_key_password_long_valid_delaytime bflg_key_operation_save.bits.bit4   //��������ʱ������ʱ

//��������
extern uint8_t   guc_key_val;                      //������ֵ
extern uint8_t   guc_TUTD_dis_type;                //ѡ����ʾ���� �����»�������ƽ��ֵ
extern uint8_t   guc_key_set_long_delaytimer;      //����set����ʱ
extern uint8_t   guc_key_set_up_long_delaytimer;   //����set+up����ʱ
extern uint8_t   guc_key_up_down_long_delaytimer;  //����up+down����ʱ
extern uint8_t   guc_key_set_down_long_delaytimer; //����set+down����ʱ
extern uint8_t   guc_key_locked_delaytimer;        //�����޲���������ʱ
extern uint8_t   guc_disp_mode;                    //�ܵ�UIģʽ

extern int16_t   gss_user_manage_password_val;    //�û���������ֵ
extern int16_t   gss_IC_password_val;             //ic������ֵ
extern int8_t    gsc_factory_password_val;         //��������ֵ
extern int8_t    gsc_disp_parm_type;               //user�趨��Ĳ�ͬ����
extern int8_t    gsc_disp_inq_mode;                //��ѯģʽ�в�ͬ����

extern int16_t   gss_key_temp_value;               //����ʱ����ʱֵ
//
extern int16_t   gss_TS_old_value;                 //�����¶ȵľ�ֵ         
extern int16_t   gss_ALH_old_value;                //���ø��±����ľ�ֵ
extern int16_t   gss_ALL_old_value;                //���õ��±����ľ�ֵ
extern int8_t    guc_user_write_time_value; 

extern int16_t   gss_user_set_parm_value[DISP_USER_SET_SUM];       
extern int16_t   gss_user_manage_parm_value[DISP_USER_MANAGE_SUM]; 
extern int16_t   gss_factory_parm_value[DISP_FAC_SUM]; 













#endif
/*********************************************END OF THE FILE********************************************************/
