#ifndef _KEY_DISP_C_
#define _KEY_DISP_C_
//------------------------------------------------------------------------------
//�����ļ�����
#include "r_cg_macrodriver.h"
#include "r_cg_cgc.h"
#include "r_cg_port.h"

#include "h_type_define.h"
#include "h_key_disp.h"
#include "h_ad.h"
#include "h_rom_para2.h"
#include "h_protect.h"
#include "h_bat_ctrl.h"

//peak add
#include "M_buzz.h"
#include "r_cg_timer.h"
#include "h_com.h"
#include "h_main.h"
#include "m_compressor.h"
#include "m_key_operation.h"
#include "m_peripheral_control.h"
#include "m_test.h"
//------------------------------------------------------------------------------
//��������
void key_IC_register_deal(void); //��ע��״̬�£��ٰ����е�һЩ�������״̬ת��
void led_lamp_disp(void);

void led_disp1_deal(void);       //�����1��ʾ���������100ms��ʱ�����е���

void led_disp2_deal(void);       //�����2��ʾ���������100ms��ʱ�����е���

void led_disp3_deal(void);       //�����3��ʾ���������100ms��ʱ�����е���

void led1_disp_val(int16_t lss_disp_val);    //�����1��ʾ��ֵ����

void led2_disp_val(int16_t lss_disp_val);    //�����2��ʾ��ֵ����

void led3_disp_val(int16_t lss_disp_val);    //�����3��ʾ��ֵ����




void read_key_delaytime(void);              //������ʱ������10ms��ʱ�����е���

//peak add
void key_buzz_cancel(void);
void bat_con_check_again(void);    
void view_error_code(void);           
void view_error_code_delaytime(void);
void led_loop_delaytime(void);      
void led_disp1_flicker_delaytime(void);  
void key_IC_password_deal(void);
void key_factory_AT_test(void);
void key_factory_init(void);          
void system_self_test(void);           
void led_disp_alternate_delaytime(void);
void led_disp_black_screen_delaytime(void);

//------------------------------------------------------------------------------
//flag
flag_type flg_disp,flg_disp1,flg_error_code_mark,flg_error_code_mark1,flg_save2;

//variate 
int16_t    gss_user_CL1_flicker_cnt;   //ע��IC��ʱCL1��˸�Ĵ���

//------------------------------------------------------------------------------
//led��ʾ����궨��
//---------------------
//|        a          |
//|       ----        |
//|    f | g  | b     |
//|       ----        |
//|    e |    | c     |
//|       ---- .      |
//|        d    dp    |
//---------------------
//----------------------------------------------------------
//    dp   |  g   |  f   |  e   |  d   |  c   |  b   |  a
//   bit7  | bit6 | bit5 | bit4 | bit3 | bit2 | bit1 | bit0
//----------------------------------------------------------
#define   data_all_off   0x00    //������
#define   data_dot       0x80    //��
#define   data_hyphen    0x40    //����

#define   data_0         0x3F    //����0
#define   data_1         0x06
#define   data_2         0x5B
#define   data_3         0x4F
#define   data_4         0x66
#define   data_5         0x6D
#define   data_6         0x7D
#define   data_7         0x07
#define   data_8         0x7F
#define   data_9         0x6F

#define   data_A         0x77
#define   data_b         0x7C
#define   data_C         0x39
#define   data_c         0x58
#define   data_d         0x5E
#define   data_E         0x79
#define   data_F         0x71
#define   data_G         0x3D
#define   data_H         0x76
#define   data_I         0x06    //peak �¼ӵĴ�дI,������1��ֵһ��
#define   data_i         0x04
#define   data_J         0x0E
#define   data_L         0x38
#define   data_N         0x37
#define   data_n         0x54
#define   data_o         0x5C
#define   data_P         0x73
#define   data_q         0x67
#define   data_r         0x50
#define   data_S         0x6D
#define   data_T         0x31
#define   data_t         0x78
#define   data_U         0x3E
#define   data_u         0x1C
#define   data_y         0x6E
//peak add
#define  data_E_dp       0XF9    // ��ʾ E �� �㣻
#define  data_line       0X40    // ֻ��ʾ g ������������� 



//----------------------------------------------------------
//��ʾ����궨��
uint8_t   guc_led_buffer[12]; //����ܼĴ���  //peak LDE1~LED12 ��Ӧ [0] ~ [11]
//----------------------------------------------------------
const uint8_t guc_nmb_table[]=
{
  data_0  // 0
, data_1  // 1
, data_2  // 2
, data_3  // 3
, data_4  // 4
, data_5  // 5
, data_6  // 6
, data_7  // 7
, data_8  // 8
, data_9  // 9
};


//-----------------------------------------------------------------------------
uint8_t   guc_disp_rate;           //��ʾ���ʼĴ���
int16_t   gss_disp_see_buffer[6];  //��ʾ��ѯ��Ϣ�Ĵ���
int16_t   gss_read_key_delaytimer; //����ֵ��ʱ��ʱ��



int16_t    gss_user2_CL1_flicker_cnt;      //ע��IC��ʱCL1��˸�Ĵ���
int16_t    gss_factory_AT_test_delaytime;  //����ATָ��ʱȫ����ʱ��
int16_t    gss_factory_CL3_flicker_cnt;    //��ʼIC����ʱCL3��˸�Ĵ���
int16_t    gss_factory_CL4_flicker_cnt;    //�ָ���������CL4��˸�Ĵ���


//peak add
int16_t   gss_error_code_delaytime;       //����������ʾ��ʱʱ��
int16_t   gss_led_loop_delaytime;         //LED����˸Ƶ���趨
int16_t   guc_IC_register_delaytime;      //�жϳ�����ϼ��ĵ�ʱ���Ƿ�ﵽ����ע��״̬
//int16_t   gss_IC_password_val;            //ICע��ģʽ������ֵ
int8_t    guc_IC_erasure_delaytime;       //ICע��ʱ���ж�
int8_t    gss_disp1_flicker_delaytime;    //"�����¶��趨" "���±����趨" "���±����趨" ��˸��ʱ
int8_t    guc_IC_password_cnt;            //����ICע��ģʽʱ������������������
int8_t    guc_password_cnt;               //���빤��ģʽʱ������������������

uint8_t   gss_self_test_delaytime;        //ϵͳ�Լ�ʱ����˸��ʱ
uint8_t   gss_self_test_cnt;              //ϵͳ�Լ�ʱ����˸��������

int16_t   gss_disp_alternate_delaytime;   //���������¶Ƚ�����ʾʱ����
int16_t   gss_disp_black_screen_delaytime;//�ϵ�ʱ�������¶Ƚ�����ʾ���



/**********************************************************************************************************************************************
��������: ��ʾ��������ܵ���ʾ��
          LED1 bit0:����      LED1 bit1:����         LED1 bit2:��Դ       LED1 bit3:���� 
          LED1 bit4:����      LED1 bit5:��������       LED1 bit6:����       LED1 bit7:NFC

          LED2~LED4����Ӧ3λ�����
          
����λ��:  10ms��ʱ��---------------------------------ok
***********************************************************************************************************************************************/
void led_disp1_deal(void)     
{   
    if (bflg_test_mode == 1)               //����ǲ���ģʽ
    {
		R_TAU0_Channel3_Stop();
        if(bflg_test_mode_RTC == 0)        //δ����buzz��
        {
	        if (bflg_test_ok == 0)         //���δ�������
	        {
	            guc_disp_rate = dot_0d001;
                LED1 = data_1;
                LED2 = data_0;
                LED3 = data_0;
                LED4 = data_0;

	            //led1_disp_val(SOFT_VERSION);
	        }
	        else                            //����������
	        {
	            if (guc_test_mode_code == 0)    
	            {
	                LED1 = data_P;
	                LED2 = data_A;
	                LED3 = data_S;
	                LED4 = data_S;
	            }
	            else                            //����в��Թ���
	            {
	                LED1 = data_all_off;
	                LED2 = data_E;
	                LED3 = guc_nmb_table[guc_test_mode_code / 10];
	                LED4 = guc_nmb_table[guc_test_mode_code % 10];  
	            }
	        }
        }
        else
        {
			LED1 = data_2;  //��ʱ�ٵģ�һֱRTC�����
			LED2 = data_0;
			LED3 = data_1;
			LED4 = data_7;
			//LED3 = guc_nmb_table[guc_RTC_r_year / 10];   //��+
			//LED4 = guc_nmb_table[guc_RTC_r_year % 10]; 
        }

        //�����2-567
        led2_disp_val(gss_TA / 10);
        //�����3 -91011
        led3_disp_val(gss_VAC);   
    } 
    else if (guc_disp_mode == DISP_LOCKED)     //���������ģʽ
    {
        if (guc_key_val == KEY_BUZ)   
        {
            bflg_remote_alarm_test = 1;
            guc_buzz_sound_cnt = 3;     //�ע��
        }
        //----------------------------------------------------------
        if (flag_view_error_code == 0) 
        {
            if((flg_alarm1.uword != 0) || (flg_alarm2.uword != 0))
            {
                if (guc_key_val == KEY_BUZ)                       				      
                {
                    flag_view_error_code = 1;	
                }
            }
            //-------------------------------------------------------
            if(bflg_disp_black_screen == 1)
            {
                LED2 = data_all_off;
                LED3 = data_all_off;
                LED4 = data_all_off; 
            }
            else if(bflg_disp_alternate == 1)  
            {
                if(bflg_TA_fault == 1)
                {
                    LED2 = data_E_dp;
                    LED3 = data_1;
                    LED4 = data_0;  
                }
                else if(bflg_TU_fault == 1)
                {
                    LED2 = data_E_dp;
                    LED3 = data_1;
                    LED4 = data_4; 
                }
                else if(bflg_TD_fault == 1)
                {
                    LED2 = data_E_dp;
                    LED3 = data_1;
                    LED4 = data_5; 
                }
            }
            else
            {
                guc_disp_rate = dot_0d1;                 //��С����   

                if(guc_TUTD_dis_type == 0)               //��ʾ�����µ�ƽ��ֵ 
                {
                    led1_disp_val((gss_TU+gss_TD) >> 1); 
                }
                else if( guc_TUTD_dis_type == 1)         //��ʾ����
                {
                    led1_disp_val(gss_TU);                   
                }
                else if(guc_TUTD_dis_type == 2)       
                {
                    led1_disp_val(gss_TD);          
                }
            }
        }
        else
        {
            view_error_code();      
        }
    }
    else if(guc_disp_mode == DISP_USER_SET)
    {
        if(bflg_parm_dis_form == 0)
        {
            if(gsc_disp_parm_type == DISP_USER_SET_TS)
            {
                LED2 = data_all_off;
                LED3 = data_T;
                LED4 = data_S;  
            }
            else if(gsc_disp_parm_type == DISP_USER_SET_ALH)
            {
                LED2 = data_A;
                LED3 = data_L;
                LED4 = data_H; 
            }
            else if(gsc_disp_parm_type == DISP_USER_SET_ALL)
            {
                LED2 = data_A;
                LED3 = data_L;
                LED4 = data_L; 
            }
            else if(gsc_disp_parm_type == DISP_USER_SET_T1)
            {
                LED2 = data_all_off;
                LED3 = data_T;
                LED4 = data_1; 
            }   
            else if(gsc_disp_parm_type == DISP_USER_SET_T2)
            {
                LED2 = data_all_off;
                LED3 = data_T;
                LED4 = data_2; 
            }  
            else if(gsc_disp_parm_type == DISP_USER_SET_P6)
            {
                LED2 = data_all_off;
                LED3 = data_P;
                LED4 = data_6; 
            }  
            else if(gsc_disp_parm_type == DISP_USER_SET_SS)
            {
                LED2 = data_all_off;
                LED3 = data_S;
                LED4 = data_S; 
            }              
        }
        else
        {
            if(gsc_disp_parm_type == DISP_USER_SET_T2)
            {
                LED2 = data_all_off;
                LED3 = data_P;
                LED4 = guc_nmb_table[gss_user_set_parm_value[DISP_USER_SET_T2]+1];
            }
            else
            {
                if(bflg_disp1_flicker == 1)         
                {
                    if(bflg_parm_save == 1)                  
                    {
                        if((gsc_disp_parm_type == DISP_USER_SET_TS) || 
                           (gsc_disp_parm_type == DISP_USER_SET_ALH)||
                           (gsc_disp_parm_type == DISP_USER_SET_ALL))
                        {
                            guc_disp_rate = dot_0d1;
                            led1_disp_val(gss_key_temp_value);
                        }
                        else
                        {
                            guc_disp_rate = not_dot;
                            led1_disp_val(gss_key_temp_value);
                        }
                    }
                    else
                    {
                        if((gsc_disp_parm_type == DISP_USER_SET_TS) || 
                           (gsc_disp_parm_type == DISP_USER_SET_ALH)||
                           (gsc_disp_parm_type == DISP_USER_SET_ALL))
                        {
                            guc_disp_rate = dot_0d1;
                            led1_disp_val(gss_user_set_parm_value[gsc_disp_parm_type]);
                        }
                        else
                        {
                            guc_disp_rate = not_dot;
                            led1_disp_val(gss_user_set_parm_value[gsc_disp_parm_type]);
                        }
                    }
                }
                else
                {
                    LED2 = data_all_off;
                    LED3 = data_all_off;
                    LED4 = data_all_off;
                }
            }
        }  
    }
    else if(guc_disp_mode == DISP_TIME_SET)
    {
        if(bflg_disp1_flicker == 1)  
        {
            guc_disp_rate = not_dot;
            led1_disp_val(guc_user_write_time_value);
        }
        else
        {
            LED1 = data_all_off;
            LED2 = data_all_off;
            LED3 = data_all_off;
            LED4 = data_all_off;
        }
    }
    else if(guc_disp_mode == DISP_USER_MANAGE_PASSWORD)
    {
        guc_disp_rate = not_dot;
        led1_disp_val(gss_user_manage_password_val);
        if(gss_user_manage_password_val < 99)
        {
            LED2 = data_0;
        }

        if(gss_user_manage_password_val < 9)
        {
            LED3 = data_0;
        }
    }
    else if(guc_disp_mode == DISP_USER_MANAGE)
    {
        if(bflg_parm_dis_form == 0)
        {
            if(gsc_disp_parm_type == DISP_USER_MANAGE_IC)
            {
                LED2 = data_all_off;
                LED3 = data_I;
                LED4 = data_C;  
            }
            else if(gsc_disp_parm_type == DISP_USER_MANAGE_CL1)
            {
                LED2 = data_C;
                LED3 = data_L;
                LED4 = data_1; 
            }
            else if(gsc_disp_parm_type == DISP_USER_MANAGE_Lt)
            {
                LED2 = data_all_off;
                LED3 = data_L;
                LED4 = data_t; 
            }
            else if(gsc_disp_parm_type == DISP_USER_MANAGE_dA)
            {
                LED2 = data_all_off;
                LED3 = data_d;
                LED4 = data_A; 
            }   
            else if(gsc_disp_parm_type == DISP_USER_MANAGE_FA)
            {
                LED2 = data_all_off;
                LED3 = data_F;
                LED4 = data_A; 
            }  
            else if(gsc_disp_parm_type == DISP_USER_MANAGE_S1)
            {
                LED2 = data_all_off;
                LED3 = data_S;
                LED4 = data_1; 
            }  
            else if(gsc_disp_parm_type == DISP_USER_MANAGE_S2)
            {
                LED2 = data_all_off;
                LED3 = data_S;
                LED4 = data_2; 
            } 
            else if(gsc_disp_parm_type == DISP_USER_MANAGE_TT)
            {
                LED2 = data_all_off;
                LED3 = data_T;
                LED4 = data_T; 
            }  
        }
        else
        {
            if(bflg_disp1_flicker == 1)         //��˸
            {
                if(bflg_parm_save == 1)                  
                {
                    guc_disp_rate = not_dot;
                    led1_disp_val(gss_key_temp_value);
                }
                else
                {
                    guc_disp_rate = not_dot;
                    led1_disp_val(gss_user_manage_parm_value[gsc_disp_parm_type]);
                }
            }
            else
            {
                LED2 = data_all_off;
                LED3 = data_all_off;
                LED4 = data_all_off;
            }
        }  
    }
    else if(guc_disp_mode == DISP_FACTORY_PASSWORD)
    {
        guc_disp_rate = not_dot;
        led1_disp_val(gsc_factory_password_val);      
        if(gsc_factory_password_val < 10)
        {
            LED3 = data_0;
        }
    }
    else if(guc_disp_mode == DISP_FACTORY)
    {
        if(bflg_parm_dis_form == 0)
        {
            if(gsc_disp_parm_type == DISP_FAC_CL2)
            {
                LED2 = data_C;
                LED3 = data_L;
                LED4 = data_2;  
            }
            else if(gsc_disp_parm_type == DISP_FAC_UL)
            {
                LED2 = data_all_off;
                LED3 = data_U;
                LED4 = data_L; 
            }            
            else if(gsc_disp_parm_type == DISP_FAC_r0)
            {
                LED2 = data_all_off;
                LED3 = data_r;
                LED4 = data_0; 
            }
            else if(gsc_disp_parm_type == DISP_FAC_b0F)
            {
                LED2 = data_b;
                LED3 = data_0;
                LED4 = data_F; 
            }
            else if(gsc_disp_parm_type == DISP_FAC_br1)
            {
                LED2 = data_b;
                LED3 = data_r;
                LED4 = data_1; 
            }
            else if(gsc_disp_parm_type == DISP_FAC_br2)
            {
                LED2 = data_b;
                LED3 = data_r;
                LED4 = data_2; 
            }
            else if(gsc_disp_parm_type == DISP_FAC_br3)
            {
                LED2 = data_b;
                LED3 = data_r;
                LED4 = data_3; 
            }
            else if(gsc_disp_parm_type == DISP_FAC_br4)
            {
                LED2 = data_b;
                LED3 = data_r;
                LED4 = data_4; 
            }            
            else if(gsc_disp_parm_type == DISP_FAC_Ct1)
            {
                LED2 = data_C;
                LED3 = data_t;
                LED4 = data_1; 
            }
            else if(gsc_disp_parm_type == DISP_FAC_Ct2)
            {
                LED2 = data_C;
                LED3 = data_t;
                LED4 = data_2; 
            }   
            else if(gsc_disp_parm_type == DISP_FAC_Ct3)
            {
                LED2 = data_C;
                LED3 = data_t;
                LED4 = data_3; 
            }  
            else if(gsc_disp_parm_type == DISP_FAC_Ct4)
            {
                LED2 = data_C;
                LED3 = data_t;
                LED4 = data_4; 
            }  
            else if(gsc_disp_parm_type == DISP_FAC_Ct5)
            {
                LED2 = data_C;
                LED3 = data_t;
                LED4 = data_5; 
            } 
            else if(gsc_disp_parm_type == DISP_FAC_Ct6)
            {
                LED2 = data_C;
                LED3 = data_t;
                LED4 = data_6; 
            } 
            else if(gsc_disp_parm_type == DISP_FAC_Ct7)
            {
                LED2 = data_C;
                LED3 = data_t;
                LED4 = data_7;  
            }
            else if(gsc_disp_parm_type == DISP_FAC_CU)
            {
                LED2 = data_all_off;
                LED3 = data_C;
                LED4 = data_U; 
            }
            else if(gsc_disp_parm_type == DISP_FAC_Ad)
            {
                LED2 = data_all_off;
                LED3 = data_A;
                LED4 = data_d; //�޴�D
            }
            else if(gsc_disp_parm_type == DISP_FAC_AAH)
            {
                LED2 = data_A;
                LED3 = data_A;
                LED4 = data_H; 
            }   
            else if(gsc_disp_parm_type == DISP_FAC_AAL)
            {
                LED2 = data_A;
                LED3 = data_A;
                LED4 = data_L; 
            }  
            else if(gsc_disp_parm_type == DISP_FAC_r1)
            {
                LED2 = data_all_off;
                LED3 = data_r;
                LED4 = data_1; 
            }  
            else if(gsc_disp_parm_type == DISP_FAC_AHH)
            {
                LED2 = data_A;
                LED3 = data_H;
                LED4 = data_H; 
            } 
            else if(gsc_disp_parm_type == DISP_FAC_T3)
            {
                LED2 = data_all_off;
                LED3 = data_T;
                LED4 = data_3; 
            }  
            else if(gsc_disp_parm_type == DISP_FAC_Cd)
            {
                LED2 = data_all_off;
                LED3 = data_C;
                LED4 = data_d;  
            }
            else if(gsc_disp_parm_type == DISP_FAC_Fd)
            {
                LED2 = data_all_off;
                LED3 = data_F;
                LED4 = data_d; 
            }
            else if(gsc_disp_parm_type == DISP_FAC_LC)
            {
                LED2 = data_all_off;
                LED3 = data_L;
                LED4 = data_C; 
            }
            else if(gsc_disp_parm_type == DISP_FAC_CL3)
            {
                LED2 = data_C;
                LED3 = data_L;
                LED4 = data_3; 
            }   
            else if(gsc_disp_parm_type == DISP_FAC_CL4)
            {
                LED2 = data_C;
                LED3 = data_L;
                LED4 = data_4; 
            }  
            else if(gsc_disp_parm_type == DISP_FAC_dFL)
            {
                LED2 = data_d;
                LED3 = data_F;
                LED4 = data_L; 
            }  
            else if(gsc_disp_parm_type == DISP_FAC_dFH)
            {
                LED2 = data_d;
                LED3 = data_F;
                LED4 = data_H; 
            } 
            else if(gsc_disp_parm_type == DISP_FAC_HT1)
            {
                LED2 = data_H;
                LED3 = data_T;
                LED4 = data_1; 
            } 
            else if(gsc_disp_parm_type == DISP_FAC_HT2)
            {
                LED2 = data_H;
                LED3 = data_T;
                LED4 = data_2; 
            }  
            else if(gsc_disp_parm_type == DISP_FAC_PS)
            {
                LED2 = data_all_off;
                LED3 = data_P;
                LED4 = data_S; 
            } 
            else if(gsc_disp_parm_type == DISP_FAC_PCH)
            {
                LED2 = data_P;
                LED3 = data_C;
                LED4 = data_H; 
            }  
        }
        else
        {
            if(bflg_disp1_flicker == 1)         //��˸
            {
                if(bflg_parm_save == 1)                  
                {
                    if((gsc_disp_parm_type == DISP_FAC_r0) ||(gsc_disp_parm_type == DISP_FAC_Ct1)||  
                       (gsc_disp_parm_type == DISP_FAC_Ct2)||(gsc_disp_parm_type == DISP_FAC_Ct3)||
                       (gsc_disp_parm_type == DISP_FAC_Ct4)||(gsc_disp_parm_type == DISP_FAC_Ct5)||
                       (gsc_disp_parm_type == DISP_FAC_Ct6))
                    {
                        guc_disp_rate = dot_0d1;               //��С�����
                        led1_disp_val(gss_key_temp_value);
                    }
                    else if(gsc_disp_parm_type == DISP_FAC_Fd) //10min,20min,30min�輶��*10
                    {
                        guc_disp_rate = not_dot;
                        led1_disp_val(gss_key_temp_value*10); 
                    }
                    else
                    {
                        guc_disp_rate = not_dot;
                        led1_disp_val(gss_key_temp_value);
                    }
                }
                else
                {
                    if((gsc_disp_parm_type == DISP_FAC_r0) ||(gsc_disp_parm_type == DISP_FAC_Ct1)||
                       (gsc_disp_parm_type == DISP_FAC_Ct2)||(gsc_disp_parm_type == DISP_FAC_Ct3)||
                       (gsc_disp_parm_type == DISP_FAC_Ct4)||(gsc_disp_parm_type == DISP_FAC_Ct5)||
                       (gsc_disp_parm_type == DISP_FAC_Ct6))
                    {
                        guc_disp_rate = dot_0d1;
                        led1_disp_val(gss_factory_parm_value[gsc_disp_parm_type]);
                    }
                    else if(gsc_disp_parm_type == DISP_FAC_Fd) 
                    {
                        guc_disp_rate = not_dot;
                        led1_disp_val(gss_factory_parm_value[gsc_disp_parm_type]*10);
                    }
                    else
                    {
                        guc_disp_rate = not_dot;
                        led1_disp_val(gss_factory_parm_value[gsc_disp_parm_type]);
                    }
                }
            }
            else
            {
                LED2 = data_all_off;
                LED3 = data_all_off;
                LED4 = data_all_off;
            }
        }  
    }
    else if(guc_disp_mode == DISP_INQUIRE)
    {
        if(bflg_key_inq_val == 0) //��ʾd1-d9
        {                
            LED2 = data_all_off;
            LED3 = data_d;
            LED4 = guc_nmb_table[gsc_disp_inq_mode+1];  
        }
        else                     //��ʾֵ
        {
            if(gsc_disp_inq_mode == DISP_INQ_d1)
            {
                guc_disp_rate = dot_0d1;
                led1_disp_val(gss_TU);          
            }
            else if(gsc_disp_inq_mode == DISP_INQ_d2)
            {
                guc_disp_rate = dot_0d1;
                led1_disp_val(gss_TD);          
            }
            else if(gsc_disp_inq_mode == DISP_INQ_d3)
            {
                guc_disp_rate = dot_0d1;
                led1_disp_val(gss_TA);          
            }
            else if(gsc_disp_inq_mode == DISP_INQ_d4)
            {
                guc_disp_rate = dot_0d1;
                led1_disp_val(gss_TE);          
            }
            else if(gsc_disp_inq_mode == DISP_INQ_d5)
            {
                guc_disp_rate = dot_0d1;
                led1_disp_val(gss_TC);          
            }
            else if(gsc_disp_inq_mode == DISP_INQ_d6)
            {
                guc_disp_rate = dot_0d1;
                led1_disp_val(gss_THW);          
            }
            else if(gsc_disp_inq_mode == DISP_INQ_d7)
            {
                guc_disp_rate = not_dot;
                led1_disp_val(guc_humidity);
            }
            else if(gsc_disp_inq_mode == DISP_INQ_d8)
            {
                guc_disp_rate = not_dot;
                led1_disp_val(gss_VAC);          
            }
            else if(gsc_disp_inq_mode == DISP_INQ_d9)
            {
                guc_disp_rate = dot_0d1;
                led1_disp_val(guc_bat_Vout);          
            }
        }
    }
    else if(guc_disp_mode == DISP_IC_PASSWORD)  //����ICע��������
    {
        if(bflg_IC_pass_CL1 == 1)
        {
            if(bflg_disp1_flicker == 1)                       //����������ȷ��CL1��˸3��            
            {   
                LED2 = data_C;
                LED3 = data_L;
                LED4 = data_1;
              
                gss_user_CL1_flicker_cnt++;
                if(gss_user_CL1_flicker_cnt >= 150)            //�ӽ���˸3��
                {
                    gss_user_CL1_flicker_cnt = 0;
                    bflg_buzz_one_tick_set = 1;   
                    bflg_IC_pass_CL1 = 0;
                    
                    guc_disp_mode = DISP_USER_MANAGE;           //���ص��û������CL1
                    gsc_disp_parm_type = DISP_USER_MANAGE_CL1;
                    gss_key_temp_value = 0;
                    bflg_parm_dis_form = 0;
                    bflg_parm_save = 0;
                }
            }
            else
            {
                LED2 = data_all_off;
                LED3 = data_all_off;
                LED4 = data_all_off;
            }
        }
        else
        {
            guc_disp_rate = not_dot;
            led1_disp_val(gss_IC_password_val);
            if(gss_IC_password_val/100 == 0)         //Ϊ����ʾ 000
            {
                LED2 = data_0;
            }
            if(gss_IC_password_val/10 == 0)                
            {
                LED3 = data_0;
            }
        }
    }
    else if(guc_disp_mode == DISP_IC_REGISTER)  //ICע���
    {
        LED2 = data_line;
        LED3 = guc_nmb_table[(guc_flash_add/4)/10];    //��ʾICע�����õĵ�ַ��
        LED4 = guc_nmb_table[(guc_flash_add/4)%10];    
    }

    //-------------------------------------------------------------------
    //U��״̬��ָʾ��
    if(guc_U_disk_state == 0x00)       //�����Ѱγ�_��
    {
        LED4 &= (~data_dot); 
    }
    else if(guc_U_disk_state == 0x01)  //æ_��
    {
        if(bflg_disp1_flicker == 1) 
        {
            LED4 |= data_dot; 
        }
        else
        {
            LED4 &= (~data_dot); 
        }
    }
    else if(guc_U_disk_state == 0x02)  //��д���_����
    {
        LED4 |= data_dot; 
    }
    /*else if(guc_U_disk_state == 0x03) //Ԥ��_��дʧ��_�Զ���������2������
    {
        if(bflg_disp1_flicker == 1) 
        {
            LED3 |= data_dot; 
            LED4 |= data_dot; 
        }
        else
        {
            LED3 &= (~data_dot); 
            LED4 &= (~data_dot); 
        }
    }*/
    else
    {
        LED3 &= (~data_dot); 
        LED4 &= (~data_dot); 
    }
    /*if((guc_usb_state & 0x04) == 0x04) //Ԥ��_usb�����_�Զ���usb�廵������3������
    {
        if(bflg_disp1_flicker == 1) 
        {
            LED2 |= data_dot; 
        }
        else
        {
            LED2 &= (~data_dot); 
        }
    }
    else
    {
        LED2 &= (~data_dot); 
    }*/
}
/******************************************************************************************************************************************
�������ܣ���ʾ���ϵ�led�Ƶ���ʾ

            LED1 bit0:����      LED1 bit1:����         LED1 bit2:��Դ       LED1 bit3:���� 
            LED1 bit4:����      LED1 bit5:��������       LED1 bit6:����       LED1 bit7:NFC

            LED2~LED4����Ӧ3λ�����

����λ�ã�10ms----------------------------------------ok
*******************************************************************************************************************************************/
void led_lamp_disp(void)
{
    if(bflg_test_mode == 0)
    {
        //---------------------------------------------������ָʾ��led1��led2
        if(guc_TUTD_dis_type == 0)
        {
            LED1 |= 0x03;
        }
        else if(guc_TUTD_dis_type == 1)
        {
            LED1 |= 0x01;                        
            LED1 &= ~0x02;
        }
        else if(guc_TUTD_dis_type == 2)
        {
            LED1 &= ~0x01;                       
            LED1 |= 0x02;
        }
        //---------------------------------------------  ��Դָʾ�� led3
        if(bflg_alarm_power_off == 1)
        {
            if(bflg_disp1_flicker == 1)
            {
                LED1 |= 0X04;
            }
            else
            {
                LED1 &= ~0x04; 
            }
        }
        else
        {
            LED1 |= 0X04;  //��Ӵ���
        }
        //-----------------------------------------------����ָʾ��led4
        if(bflg_alarm_light == 1)
        {
            if(bflg_disp1_flicker == 1)
            {
                LED1 |= 0x08;
            }
            else
            {
                LED1 &= ~0x08; 
            }
        }
        else
        {
            LED1 &= ~0x08; 
        }
        //-----------------------------------------------����ָʾ��led5
        if(bflg_alarm_open_door == 1)  //���ű���ʱ��
        {
            if(bflg_disp1_flicker == 1)
            {
                LED1 |= 0X10;
            }
            else
            {
                LED1 &= ~0x10; 
            }
        }
        else
        {
            if(bflg_prot2_door1_pin == 1) //����
            {
                LED1 |= 0X10;
            }
            else
            {
                LED1 &= ~0x10; 
            }
        }
        //-----------------------------------------------��������ָʾ��led6
        if(bflg_alarm_cond_dirty == 1)
        {
            if(bflg_disp1_flicker == 1)
            {
                LED1 |= 0X20;
            }
            else
            {
                LED1 &= ~0x20; 
            }
        }
        else
        {
            LED1 &= ~0x20; 
        }
        //-----------------------------------------------����ָʾ��led7
        //��+
        
        //-----------------------------------------------NFCָʾ��led8
        //��+
    }
}

/*******************************************************************************
function: ��
          �м��LEDҪ��ʾ������
          
*******************************************************************************/
void led_disp2_deal(void)     //�����2��ʾ���������100ms��ʱ�����е���
{
    if (bflg_test_mode == 1)         //����ǲ���ģʽ
    {
        guc_disp_rate = not_dot;
        led2_disp_val(gss_TA / 10);
    }
    else
    {
        guc_disp_rate = not_dot;
        //debug
        //led2_disp_val(gss_VAC);              //peak ѡ���ѹ��ʾ
        //led2_disp_val(guc_bat_Vout);
        //led2_disp_val(gss_adg_Temp[0]);
        //led2_disp_val(gss_comp2_freq / 10);    // ����ѹ��Ƶ��
    }      
}

//------------------------------------------------------------------------------
/*******************************************************************************
function: ��
-------------------��ʾ����-----------------------
 �ж������� ���£�35�棨 EEP�� ���� 30 �루 EEP����
 �����ʩ�� ��������˸��
 �ָ������� ���¡�34�棨 EEP�� ���� 30 �루 EEP����
		  ����ʱ�����ʾ���£�35�棬 ����ʾ 35�棩
---------------------------------------------------

 bflg_prot2_THW_high_disp ��protect��ѡ�� 0/1 (���ڵĻ���/�趨��)
 
*******************************************************************************/
void led_disp3_deal(void)        //�����3��ʾ���������100ms��ʱ�����е���
{
    guc_disp_rate = not_dot;
    //debug
    /*
    if (bflg_prot2_THW_high_disp == 0)   //peak ѡ������ʾ
    {
    	  //led3_disp_val(gss_THW);      //����

    	  //peak add    	  
    	  //led3_disp_val(gss_TC);       //������
    	  //led3_disp_val(gss_TE);       //�Ƚ�����
    	  led3_disp_val(gss_Tbat1);      //����¶�
    }
    else
    {
    	  led3_disp_val(ram_para[num_prot2_THW_T]);  //peak 35��
    }
    */
    //led3_disp_val(guc_bat_Vin);
    //led3_disp_val(gss_bat_I);
    //led3_disp_val(gss_adg_Temp[1]);
    //led3_disp_val(gss_bat_pwm_value);
    //led3_disp_val(gss_adg_Temp2);
    //led3_disp_val(gss_THW);      //����
    //led3_disp_val(gss_comp21_freq / 10);    //��ѹ��ת��
}
//------------------------------------------------------------------------------
/*******************************************************************************
 ������������4λ�����:
 ��λ����ܵ�1~4 λ�ķֱ���ʾ:
*******************************************************************************/
void led1_disp_val(int16_t lss_disp_val)     //�����1��ʾ��ֵ����
{
    uint16_t lus_disp_abs_val;
   
    
    if (lss_disp_val < 0)
    {
        bflg_disp_minus = 1;
        lus_disp_abs_val = (~lss_disp_val) + 1;
    }
    else
    {
        bflg_disp_minus = 0;
        lus_disp_abs_val = lss_disp_val;
    }
    //----------------------------------------------------------------
    if ((lus_disp_abs_val / 1000) != 0)   //ȡ����λ��ֵ
    {
        lus_disp_abs_val /= 10;           //����4λ��ʱ������С10������Щ�������ڹ������ϱ�ע
    }                                     //����ֵ����4λʱ��������ȷ����

    //----------------------------------------------------------------
    if ((lus_disp_abs_val / 100) != 0)
    {
        LED2 = guc_nmb_table[lus_disp_abs_val / 100];
    }
    else
    {
        LED2 = data_all_off;
    }
    //------------------------------------------------------
    lus_disp_abs_val %= 100;
    if ((lus_disp_abs_val / 10) != 0)
    {
        LED3 = guc_nmb_table[lus_disp_abs_val / 10];
        //------------------------------
        if (guc_disp_rate == dot_0d1)
        {
            LED3 |= data_dot;
        }
        //------------------------------
        if (bflg_disp_minus == 1)
        {
            if (LED2 == data_all_off)
            {
                LED2 = data_hyphen;
                bflg_disp_minus = 0;
            }
        }
    }
    else
    {
        if (guc_disp_rate == dot_0d1)
        {
            LED3 = data_0 | data_dot;
        }
        else
        {
            if (LED2 == data_all_off)
            {
                LED3 = data_all_off;
            }
            else
            {
                LED3 = data_0;
            }
        }
    }
    //------------------------------------------------------
    lus_disp_abs_val %= 10;
    if (lus_disp_abs_val != 0)
    {
        LED4 = guc_nmb_table[lus_disp_abs_val];
        if (bflg_disp_minus == 1)
        {
            if (LED3 == data_all_off)
            {
                LED3 = data_hyphen;
                bflg_disp_minus = 0;
            }
            else if (LED2 == data_all_off)
            {
                LED2 = data_hyphen;
                bflg_disp_minus = 0;
            }
        }
    }
    else
    {
        LED4 = data_0;
    }
}

/*{
    uint16_t lus_disp_abs_val;
    
    if (lss_disp_val < 0)
    {
        bflg_disp_minus = 1;      //��ʾ������־   
        lus_disp_abs_val = (~lss_disp_val) + 1;    // peak �������Ĳ���ת��Ϊ������Ҫ�Ĳ������ŵ�ԭ��
    }
    else
    {
        bflg_disp_minus = 0;
        lus_disp_abs_val = lss_disp_val;
    }
    //------------------------------------------------------ LED1
    if ((lus_disp_abs_val / 1000) != 0)                  //peak ȡ�̣�ȡ����λ��ֵ
    {
        LED1 = guc_nmb_table[lus_disp_abs_val / 1000];   //peak ��ֵ����ʾ
    }
    else
    {
        LED1 = data_all_off;                             //peak ûֵ�Ͳ���
    }
    //------------------------------------------------------ LED2   
    lus_disp_abs_val %= 1000;                            // peak ȡ�࣬ʣ�µ���ֵ
    if ((lus_disp_abs_val / 100) != 0)
    {
        LED2 = guc_nmb_table[lus_disp_abs_val / 100];
        //------------------------------
        if (guc_disp_rate == dot_0d01)                   //peak �ж�LED2��λС�����Ƿ�
        {
            LED2 |= data_dot;
        }
        //------------------------------
        if (bflg_disp_minus == 1)                        //peak �ж�ǰһλ�Ƿ��и���
        {
            if (LED1 == data_all_off)
            {
                LED1 = data_hyphen;
                bflg_disp_minus = 0;
            }
        }
    }
    else
    {
        if (guc_disp_rate == dot_0d01)                  //peak �ж�Ϊ0ʱ�Ƿ�С����
        {
            LED2 = data_0 | data_dot;
        }
        else
        {
            if (LED1 == data_all_off)                   //peak ǰһλû��ֵ����λΪ0û���壬Ҳ�ص�
            {
                LED2 = data_all_off;
            }
            else
            {
                LED2 = data_0;
            }
        }
    }
    //------------------------------------------------------ LED3
    lus_disp_abs_val %= 100;
    if ((lus_disp_abs_val / 10) != 0)
    {
        LED3 = guc_nmb_table[lus_disp_abs_val / 10];
        //------------------------------
        if (guc_disp_rate == dot_0d1)
        {
            LED3 |= data_dot;               //peak LED3��С����Ҳ��ʾ��
        }
        //------------------------------
        if (bflg_disp_minus == 1)
        {
            if (LED2 == data_all_off)
            {
                LED2 = data_hyphen;
                bflg_disp_minus = 0;
            }
            else if (LED1 == data_all_off)
            {
                LED1 = data_hyphen;
                bflg_disp_minus = 0;
            }
        }
    }
    else
    {
        if (guc_disp_rate == dot_0d1)
        {
            LED3 = data_0 | data_dot;
        }
        else
        {
            if (LED2 == data_all_off)
            {
                LED3 = data_all_off;
            }
            else
            {
                LED3 = data_0;
            }
        }
    }
    //------------------------------------------------------ LED4
    lus_disp_abs_val %= 10;
    if (lus_disp_abs_val != 0)
    {
        LED4 = guc_nmb_table[lus_disp_abs_val];
        if (bflg_disp_minus == 1)
        {
            if (LED3 == data_all_off)
            {
                LED3 = data_hyphen;
                bflg_disp_minus = 0;
            }
            else if (LED2 == data_all_off)
            {
                LED2 = data_hyphen;
                bflg_disp_minus = 0;
            }
            else if (LED1 == data_all_off)
            {
                LED1 = data_hyphen;
                bflg_disp_minus = 0;
            }
        }
    }
    else
    {
        LED4 = data_0;
    }
}*/
//------------------------------------------------------------------------------
/*******************************************************************************
 �м���������Ϊ3λ�����
 
*******************************************************************************/
void led2_disp_val(int16_t lss_disp_val) //�����2��ʾ��ֵ����
{
    uint16_t lus_disp_abs_val;
    
    if (lss_disp_val < 0)
    {
        bflg_disp_minus = 1;
        lus_disp_abs_val = (~lss_disp_val) + 1;
    }
    else
    {
        bflg_disp_minus = 0;
        lus_disp_abs_val = lss_disp_val;
    }
    //------------------------------------------------------
    if ((lus_disp_abs_val / 100) != 0)
    {
        LED5 = guc_nmb_table[lus_disp_abs_val / 100];
    }
    else
    {
        LED5 = data_all_off;
    }
    //------------------------------------------------------
    lus_disp_abs_val %= 100;
    if ((lus_disp_abs_val / 10) != 0)
    {
        LED6 = guc_nmb_table[lus_disp_abs_val / 10];
        //------------------------------
        if (guc_disp_rate == dot_0d1)
        {
            LED6 |= data_dot;
        }
        //------------------------------
        if (bflg_disp_minus == 1)
        {
            if (LED5 == data_all_off)
            {
                LED5 = data_hyphen;
                bflg_disp_minus = 0;
            }
        }
    }
    else
    {
        if (guc_disp_rate == dot_0d1)
        {
            LED6 = data_0 | data_dot;
        }
        else
        {
            if (LED5 == data_all_off)
            {
                LED6 = data_all_off;
            }
            else
            {
                LED6 = data_0;
            }
        }
    }
    //------------------------------------------------------
    lus_disp_abs_val %= 10;
    if (lus_disp_abs_val != 0)
    {
        LED7 = guc_nmb_table[lus_disp_abs_val];
        if (bflg_disp_minus == 1)
        {
            if (LED6 == data_all_off)
            {
                LED6 = data_hyphen;
                bflg_disp_minus = 0;
            }
            else if (LED5 == data_all_off)
            {
                LED5 = data_hyphen;
                bflg_disp_minus = 0;
            }
        }
    }
    else
    {
        LED7 = data_0;
    }
}
//------------------------------------------------------------------------------
 /*******************************************************************************
 ��������������3λ�����  
*******************************************************************************/
void led3_disp_val(int16_t lss_disp_val) //�����3��ʾ��ֵ����
{
    uint16_t lus_disp_abs_val;
    
    if (lss_disp_val < 0)
    {
        bflg_disp_minus = 1;
        lus_disp_abs_val = (~lss_disp_val) + 1;
    }
    else
    {
        bflg_disp_minus = 0;
        lus_disp_abs_val = lss_disp_val;
    }
    //------------------------------------------------------
    if ((lus_disp_abs_val / 100) != 0)
    {
        LED9 = guc_nmb_table[lus_disp_abs_val / 100];
    }
    else
    {
        LED9 = data_all_off;
    }
    //------------------------------------------------------
    lus_disp_abs_val %= 100;
    if ((lus_disp_abs_val / 10) != 0)
    {
        LED10 = guc_nmb_table[lus_disp_abs_val / 10];
        //------------------------------
        if (guc_disp_rate == dot_0d1)
        {
            LED10 |= data_dot;
        }
        //------------------------------
        if (bflg_disp_minus == 1)
        {
            if (LED9 == data_all_off)
            {
                LED9 = data_hyphen;
                bflg_disp_minus = 0;
            }
        }
    }
    else
    {
        if (guc_disp_rate == dot_0d1)
        {
            LED10 = data_0 | data_dot;
        }
        else
        {
            if (LED9 == data_all_off)
            {
                LED10 = data_all_off;
            }
            else
            {
                LED10 = data_0;
            }
        }
    }
    //------------------------------------------------------
    lus_disp_abs_val %= 10;
    if (lus_disp_abs_val != 0)
    {
        LED11 = guc_nmb_table[lus_disp_abs_val];
        if (bflg_disp_minus == 1)
        {
            if (LED10 == data_all_off)
            {
                LED10 = data_hyphen;
                bflg_disp_minus = 0;
            }
            else if (LED9 == data_all_off)
            {
                LED9 = data_hyphen;
                bflg_disp_minus = 0;
            }
        }
    }
    else
    {
        LED11 = data_0;
    }
}


//------------------------------------------------------------------------------
/*******************************************************************************
function:  ��������ÿ���೤ʱ���ȡһ�ΰ���ֵ����������� read_key_deal()
********************************************************************************/
void read_key_delaytime(void) //������ʱ������10ms��ʱ�����е���
{
    if (bflg_read_key_delaytime == 1)
    {
        gss_read_key_delaytimer++;
        if (gss_read_key_delaytimer >= 100)  // 100*10 = 1000ms = 1s ��Ϊ1s
        {
            gss_read_key_delaytimer = 0;
            bflg_read_key_delaytime = 0;
        }
    }
}

//------------------------------------------------------------------------------
/*    |   The following code is added on my own
	  |
	  |
	  |
	  |    
	 \|/                                                                     */

/*===========================================================================
��������: 1���б���ʱ���� buzz ����30min�ڲ��ٱ���
          2�����δ����ʱ������buzz 5s,���ڵ����һ�β巴֮ǰ���ᱨ����
        ��ԭ����������ȡ���ģ�

����λ��: ��ѭ�� ----------------ok
===========================================================================*/
void  key_buzz_cancel(void)
{
    if (guc_key_val == KEY_SET)  
    {
        if (bflg_buzz_loop_tick_set == 1)         // �����ʱ�з��������������ǰ�����"����ȡ��"�����������ʱ30 min �����죬
        {                                         // 30 min ��ָ��������ƣ�
           if (bflg_alarm_buzz_off_delaytime == 0)//��������Ҫ����30min��������ټ�ʱ
            {
                bflg_alarm_buzz_off_delaytime = 1;
                gss_alarm_buzz_off_delaytimer = 0;
            }       
        }
    }           
}

/********************************************************************************
function:            
          ���δ���ӣ�����5s������һ�μ�⵽������ʱ���ڲ��죬
          
          ���ٴμ�⵽��أ��ָ��������ƣ�
����λ��: ��ѭ��-------------------ok          
********************************************************************************/
void bat_con_check_again(void)    
{
	if (bflg_key_long_buz_valid == 1)        //�е��δ���ӣ���������bS����δ1��
	{
		if (bflg_alarm_bat_discnnt == 0)     //����ּ�⵽���
		{    
		     //debug ע��
			 bflg_key_long_buz_valid = 0;    //�ָ���������
			 //gss_factory_bS = 0;           //��ʱע��
		}
	}
}

/******************************************************************************
function: ��ʾ��δ����ʱ����һ�� ������ȡ�������ɲ�ѯ���ϴ�������:

����λ�ã� �� led_disp1_deal(void) ��,��10ms��ʱ����--------ok
******************************************************************************/
void view_error_code(void)    
{ 
   /*
    bflg_alarm_high_TUTD = 0;  //1
    bflg_alarm_low_TUTD = 0;   //2
    bflg_alarm_higt_THW = 0;   //3
    bflg_alarm_high_AIR = 0;   //4
    bflg_alarm_low_AIR = 0;    //5
    bflg_alarm_higt_TE = 0;    //6
    bflg_TA_fault = 0;         //7
    bflg_TE_fault = 0;         //8

    //���¡�ʪ����1��
    bflg_THW_fault = 0;        //9
    bflg_humidity_fault = 0;   //ͬ��
    
    bflg_TC_fault = 0;         //10
    bflg_TU_fault = 0;         //11
    bflg_TD_fault = 0;         //12
    bflg_alarm_bat_discnnt = 0;//13
    bflg_alarm_bat_fed = 0;    //14
    bflg_alarm_bat_low = 0;    //15
    bflg_alarm_power_off = 0;  //16
    bflg_alarm_power_fault = 0;//17
    bflg_alarm_open_door = 0;  //18
    bflg_alarm_cond_dirty = 0; //19
    bflg_sterilize_fault = 0;  //20
    //���ڻ��¹���
    bflg_alarm_too_higt_THW = 0; //��*/
    
    if ((bflg_alarm_high_TUTD == 1) && (bflg_error_high_TUTD_mark ==0))        //1
    {
        bflg_error_code_delaytime = 1;

        LED2 = data_E_dp;
        LED3 = data_0;
        LED4 = data_0;
    } 	
    else if ((bflg_alarm_low_TUTD == 1) && (bflg_error_low_TUTD_mark ==0))  //2 
    {
        bflg_error_code_delaytime = 1;
	 
		LED2 = data_E_dp;
		LED3 = data_0;
		LED4 = data_1;
    }
    else if ((bflg_alarm_higt_THW == 1) && (bflg_error_higt_THW_mark ==0))    //3 
    {
        bflg_error_code_delaytime = 1;
	
		LED2 = data_E_dp;
		LED3 = data_0;
		LED4 = data_2;
    }
    else if ((bflg_alarm_high_AIR == 1) && (bflg_error_high_AIR_mark ==0))  //4
    {
        bflg_error_code_delaytime = 1;
	 
		LED2 = data_E_dp;
		LED3 = data_0;
		LED4 = data_3;
    }
    else if ((bflg_alarm_low_AIR == 1) && (bflg_error_low_AIR_mark ==0))  //5
    {
        bflg_error_code_delaytime = 1;
	
		LED2 = data_E_dp;
		LED3 = data_0;
		LED4 = data_4;
    }
    else if ((bflg_alarm_higt_TE == 1) && (bflg_error_higt_TE_mark ==0))     //6
    {
        bflg_error_code_delaytime = 1;
	
		LED2 = data_E_dp;
		LED3 = data_0;
		LED4 = data_5;
    }
    else if ((bflg_TA_fault == 1) && (bflg_error_TA_fault_mark ==0))      //7
    {
        bflg_error_code_delaytime = 1;
	
		LED2 = data_E_dp;
		LED3 = data_1;
		LED4 = data_0;
    }
    else if ((bflg_TE_fault == 1) && (bflg_error_TE_fault_mark ==0))     //8
    {
        bflg_error_code_delaytime = 1;
	
		LED2 = data_E_dp;
		LED3 = data_1;
		LED4 = data_1;
    }    
    else if (((bflg_THW_fault == 1)||(bflg_humidity_fault==1)) && (bflg_error_THW_humidity_mark ==0))    //9
    {
        bflg_error_code_delaytime = 1;
	
		LED2 = data_E_dp;
		LED3 = data_1;
		LED4 = data_2;
    }
    else if ((bflg_TC_fault == 1) && (bflg_error_TC_fault_mark ==0))    //10
    {
        bflg_error_code_delaytime = 1;
	
		LED2 = data_E_dp;
		LED3 = data_1;
		LED4 = data_3;
    }
    else if ((bflg_TU_fault == 1) && (bflg_error_TU_fault_mark ==0))     //11
    {
        bflg_error_code_delaytime = 1;
	
		LED2 = data_E_dp;
		LED3 = data_1;
		LED4 = data_4;
    }
   else if ((bflg_TD_fault == 1) && (bflg_error_TD_fault_mark ==0))     //12
    {
        bflg_error_code_delaytime = 1;
	
		LED2 = data_E_dp;
		LED3 = data_1;
		LED4 = data_5;
    }
    else if ((bflg_alarm_bat_discnnt == 1) && (bflg_error_bat_discnnt_mark ==0))     //13
    {
        bflg_error_code_delaytime = 1;
	
		LED2 = data_E_dp;
		LED3 = data_2;
		LED4 = data_0;
    }
    else if ((bflg_alarm_bat_fed == 1) && (bflg_error_bat_fed_mark ==0))   //14
    {
        bflg_error_code_delaytime = 1;
	
		LED2 = data_E_dp;
		LED3 = data_2;
		LED4 = data_1;
    }
    else if ((bflg_alarm_bat_low == 1) && (bflg_error_bat_low_mark ==0))   //15
    {
        bflg_error_code_delaytime = 1;
	 
		LED2 = data_E_dp;
		LED3 = data_2;
		LED4 = data_2;
    }
    else if ((bflg_alarm_power_off == 1) && (bflg_error_power_off_mark ==0))   //16
    {
        bflg_error_code_delaytime = 1;
	 
		LED2 = data_E_dp;
		LED3 = data_3;
		LED4 = data_0;
    }
    else if ((bflg_alarm_power_fault == 1) && (bflg_error_power_fault_mark ==0))   //17
    {
        bflg_error_code_delaytime = 1;
	 
		LED2 = data_E_dp;
		LED3 = data_3;
		LED4 = data_1;
    }
    else if ((bflg_alarm_open_door == 1) && (bflg_error_open_door_mark ==0))   //18
    {
        bflg_error_code_delaytime = 1;
	 
		LED2 = data_E_dp;
		LED3 = data_4;
		LED4 = data_0;
    }
    else if ((bflg_alarm_cond_dirty == 1) && (bflg_error_cond_dirty_mark ==0))   //19
    {
        bflg_error_code_delaytime = 1;
	 
		LED2 = data_E_dp;
		LED3 = data_5;
		LED4 = data_0;
    }
    else if ((bflg_sterilize_fault == 1) && (bflg_error_sterilize_fault_mark ==0))   //20
    {
        bflg_error_code_delaytime = 1;
	 
		LED2 = data_E_dp;
		LED3 = data_6;
		LED4 = data_0;
    }
	else
	{
        flag_view_error_code = 0;                    // ����ʾ�����¶���
	
        flg_error_code_mark.uword = 0;
        flg_error_code_mark1.uword = 0;
	}
}

/***********************************************************************************************
�������� :

����λ��: �� 100ms �е���  ------ok
***********************************************************************************************/
void view_error_code_delaytime (void)
{
    if (bflg_error_code_delaytime == 1) 
    {
        gss_error_code_delaytime ++;
        if (gss_error_code_delaytime >= 20)       // 100 * 20 = 2s
        {
            bflg_error_code_delaytime = 0;
            gss_error_code_delaytime = 0;

            if ((bflg_alarm_high_TUTD == 1) && (bflg_error_high_TUTD_mark ==0))        //1
            {
                bflg_error_high_TUTD_mark = 1;
            }   
            else if ((bflg_alarm_low_TUTD == 1) && (bflg_error_low_TUTD_mark ==0))     //2 
            {
                bflg_error_low_TUTD_mark = 1;
            }
            else if ((bflg_alarm_higt_THW == 1) && (bflg_error_higt_THW_mark ==0))     //3 
            {
                bflg_error_higt_THW_mark = 1;
            }
            else if ((bflg_alarm_high_AIR == 1) && (bflg_error_high_AIR_mark ==0))     //4
            {
                bflg_error_high_AIR_mark = 1;
            }
            else if ((bflg_alarm_low_AIR == 1) && (bflg_error_low_AIR_mark ==0))       //5
            {
            bflg_error_low_AIR_mark = 1;

            }
            else if ((bflg_alarm_higt_TE == 1) && (bflg_error_higt_TE_mark ==0))      //6
            {
                bflg_error_higt_TE_mark = 1;
            }
            else if ((bflg_TA_fault == 1) && (bflg_error_TA_fault_mark ==0))          //7
            {
                bflg_error_TA_fault_mark = 1;
            }
            else if ((bflg_TE_fault == 1) && (bflg_error_TE_fault_mark ==0))          //8
            {
                bflg_error_TE_fault_mark = 1;
            }    
            else if (((bflg_THW_fault == 1)||(bflg_humidity_fault==1)) && (bflg_error_THW_humidity_mark ==0))    //9
            {
                bflg_error_THW_humidity_mark = 1;
            }
            else if ((bflg_TC_fault == 1) && (bflg_error_TC_fault_mark ==0))         //10
            {
                bflg_error_TC_fault_mark = 1;
            }
            else if ((bflg_TU_fault == 1) && (bflg_error_TU_fault_mark ==0))         //11
            {
                bflg_error_TU_fault_mark = 1;
            }
            else if ((bflg_TD_fault == 1) && (bflg_error_TD_fault_mark ==0))         //12
            {
                bflg_error_TD_fault_mark = 1;
            }
            else if ((bflg_alarm_bat_discnnt == 1) && (bflg_error_bat_discnnt_mark ==0))  //13
            {
                bflg_error_bat_discnnt_mark = 1;
            }
            else if ((bflg_alarm_bat_fed == 1) && (bflg_error_bat_fed_mark ==0))          //14
            {
                bflg_error_bat_fed_mark = 1;
            }
            else if ((bflg_alarm_bat_low == 1) && (bflg_error_bat_low_mark ==0))         //15
            {
                bflg_error_bat_low_mark = 1;
            }
            else if ((bflg_alarm_power_off == 1) && (bflg_error_power_off_mark ==0))      //16
            {
                bflg_error_power_off_mark = 1;
            }
            else if ((bflg_alarm_power_fault == 1) && (bflg_error_power_fault_mark ==0))   //17
            {
                bflg_error_power_fault_mark = 1;
            }
            else if ((bflg_alarm_open_door == 1) && (bflg_error_open_door_mark ==0))       //18
            {
                bflg_error_open_door_mark = 1;
            }
            else if ((bflg_alarm_cond_dirty == 1) && (bflg_error_cond_dirty_mark ==0))     //19
            {
                bflg_error_cond_dirty_mark = 1;
            }
            else if ((bflg_sterilize_fault == 1) && (bflg_error_sterilize_fault_mark ==0))   //20
            {
                bflg_error_sterilize_fault_mark = 1;
            }
        }
    }
}

/*******************************************************************************************
��������: ָʾ����˸ʱ���־���趨,��һֱ������ѭ��

����λ��:100ms ��ʱ���� ---ok

У��: >=1  ----->  >1
*******************************************************************************************/
void led_loop_delaytime(void)    //--����
{
    gss_led_loop_delaytime++;
	if (gss_led_loop_delaytime > 1)      // 100ms * 1 = 100ms 
	{
		gss_led_loop_delaytime = 0;
	    bflg_led_loop_set = ~bflg_led_loop_set;
	}
}

/*******************************************************************************************
��������: ��"�����¶��趨"��"���±����趨"��"���±����趨" ģʽʱ����Ӧ����ֵҪ��˸

����λ��:    100ms ��ʱ����----------ok
*******************************************************************************************/
void led_disp1_flicker_delaytime(void)
{
    gss_disp1_flicker_delaytime++;
    if(gss_disp1_flicker_delaytime > 5)
    {       
        gss_disp1_flicker_delaytime = 0;
        bflg_disp1_flicker = ~bflg_disp1_flicker;      
    }
}



/******************************************************************************
function: ������ģʽ�£�����set+down 5s����ICע������ģʽ
          ��ICע������ģʽʱ�Ըú������д���������ȷ����ע��ģʽ��

          if(guc_disp1_mode == DISP1_IC_PASSWORD) 
          {
             key_IC_password_deal();
          }
          
����λ��:��  read_key_deal();     -----------------------OK    
******************************************************************************/
void key_IC_password_deal(void)  //�������봦�����
{
   /* if (guc_key_val == KEY_BUZ)
    {
        if(gsc_disp_user2_mode == DISP_USER2_CL1)    //�����user2��� ICע������ʱ�����
        {           
            guc_disp_mode = DISP_USER2;   
            //gsc_disp_user2_mode = DISP_USER2_dA;   //δ�����ٴν���˲�user2��ʾĬ�ϵĲ���
            bflg_set2_val = 0;                       //�������CL1 ָ���з��ص�
        }
        else
        {
            guc_disp_mode = DISP_LOCKED;   
            
            bflg_IC_register_state = 0;              //��� ����֤
        }

    }
    else if (guc_key_val == KEY_UP)
    {          
        gss_IC_password_val++;
        if (gss_IC_password_val > 999)
        {
            gss_IC_password_val = 0;
        }     
    }
    
    else if (guc_key_val == KEY_DOWN)
    {       
        gss_IC_password_val--;
        if (gss_IC_password_val < 0)
        {
            gss_IC_password_val = 999;
        }      
    }
    
    else if (guc_key_val == KEY_SET)
    {       
        guc_IC_password_cnt++;
        if(guc_IC_password_cnt >= 3)
        {   
            bflg_IC_register_state = 0;           //��ɽ���IC����״̬
            gss_IC_password_val = 0;
            guc_IC_password_cnt = 0;
            guc_disp_mode = DISP_LOCKED;
            gsc_disp_user1_mode = DISP_USER1_TS;    
            gsc_disp_user2_mode = DISP_USER2_dA; 
        }
        else
        {            
            if (gss_IC_password_val == gss_user2_IC)      // ����������Ƿ�����õ�һ��
            {        
                guc_IC_password_cnt = 0;
                
                if(gsc_disp_user2_mode == DISP_USER2_CL1) //�����user2��� ICע������
                {
                    bflg_IC_erasure_order = 1;            //ע������IC��
                    
                    bflg_IC_pass_CL1 = 1;                 //CL1��˸��ʾ�ı�־
                }
                else
                {              
                    guc_disp_mode  = DISP_IC_REG;
                }
            }
        }        
    }*/
}

/*******************************************************************************
��������:  ��ICע��ʱ�����������ܵ���ʾ����

����λ��:  read_key_deal() �е� ������ DISP1_IC_REG ״̬��ִ��----------OK
*******************************************************************************/
void key_IC_register_deal(void)
{/*
    if(guc_key_val == KEY_BUZ)                // ����SET�����˳������� 
    {
       bflg_IC_register_state = 0;            // �˳� ICע��״̬
       guc_temp_flash_add = 0;
       guc_flash_add = 0;
       
       guc_disp_mode = DISP_LOCKED;            //�ص��ϲ����ʾ״̬
    }*/
}

/*******************************************************************************
��������:  factory����ģʽ��ATָ��Ĳ���
           
����λ��:  10ms��ʱ��---------------------------------------ok
*******************************************************************************/
void key_factory_AT_test(void)
{
    LED1 = 0xff;
    LED2 = 0xff;
    LED3 = 0xff;
    LED4 = 0xff;
    LED5 = 0xff;
    LED6 = 0xff;
    LED7 = 0xff;
    LED8 = 0xff;
    LED9 = 0xff;
    LED10 = 0xff;
    LED11 = 0xff;
    LED12 = 0xff;    
    
    RL_ALM_OUT_PIN = 1;                       //Զ�̱����ƿ�
    bflg_alarm_lamp = 1;                      //3λ������
    
    gss_factory_AT_test_delaytime++;
    if(gss_factory_AT_test_delaytime >= 600)  //10ms * 600 = 6s ��ָ�����
    {
        gss_factory_AT_test_delaytime = 0;
        bflg_factory_AT_test = 0;

        LED1 = 0x00;
        LED2 = 0x00;
        LED3 = 0x00;
        LED4 = 0x00;
        LED5 = 0x00;
        LED6 = 0x00;
        LED7 = 0x00;
        LED8 = 0x00;
        LED9 = 0x00;
        LED10 = 0x00;
        LED11 = 0x00;
        LED12 = 0x00;  
    }
}

/*******************************************************************************
��������:  CL4����ָ���Ӧ�ָ�����ֵ����
           
����λ��: ��ѭ��---------------------------------------ok
*******************************************************************************/
void key_factory_init(void)
{
    if(bflg_factory_CL4 == 1)
    {
        bflg_factory_CL4 = 0;
        
        init_ram_para2();             // peak ��дE2���°���Ҫ��дһ�Σ�
        init_ram_para();
        
        eeprom2_read_deal();          //eeprom2��ȡ�������
        eeprom_read_deal();           //eeprom��ȡ�������
        /*
        gss_disp1_TS = ram_para[num_comp2_TS];
        gss_disp1_ALH = ram_para[num_comp2_ALH];
        gss_disp1_ALL = ram_para[num_comp2_ALL];
        //-------------------------------------------------
        gss_user2_dA  = ram_para[num_door_alarm_delaytime];
        gss_user2_T1  = ram_para[num_usb_read_delaytime];
        //gss_user2_T2  δ����
        gss_user2_P6  = ram_para[num_usb_P6];
        gss_user2_IC  = ram_para[num_IC_password];
        //gss_user2_CL1 
        //-------------------------------------------------
        //gss_factory_CL2        
        gss_factory_Ct = ram_para[num_pt100_calibration];     
        gss_factory_parm_value[DISP_FAC_CU] = ram_para[num_voltage_calibration];  
        gss_factory_CA = ram_para[num_THW_calibration];  
        gss_factory_parm_value[DISP_FAC_Fd] = ram_para[num_buzz_alarm_delaytime];  
        //gss_factory_AT  
        gss_factory_SS = ram_para[num_TE_fault_shield];  
        gss_factory_AA = ram_para[num_THW_hight_alarm];  
        gss_factory_Cd = ram_para[num_comp_startup_delaytime];  
        //gss_factory_CL3 
        //gss_factory_CL4  
        gss_factory_PS =  ram_para[num_password_val];  
        */
    }
}

/*******************************************************************************
��������:  ���ϵ�ʱ�Լ죬���е�����3s,��1��
           
����λ��:  10ms��ʱ��----------------------------------ok
*******************************************************************************/
void system_self_test(void)
{
    if(bflg_self_test_sound == 0)
    {
        bflg_self_test_sound = 1;
        guc_buzz_sound_cnt = 1;     //��1��
    }

    gss_self_test_delaytime++;
    if(gss_self_test_delaytime >= 100)    //10ms*100 = 1s
    {
        gss_self_test_delaytime = 0;
        bflg_self_test_flicker = ~bflg_self_test_flicker;

        gss_self_test_cnt++;
        if(gss_self_test_cnt > 4 )
        {
            gss_self_test_cnt = 0;
            bflg_self_test = 0;
        }
    }


    if(bflg_self_test_flicker == 0)
    {   //ȫ��
        LED1 = 0xff;
        LED2 = 0xff;
        LED3 = 0xff;
        LED4 = 0xff;
        LED5 = 0xff;
        LED6 = 0xff;
        LED7 = 0xff;
        LED8 = 0xff;
        LED9 = 0xff;
        LED10 = 0xff;
        LED11 = 0xff;
        LED12 = 0xff; 
        
        bflg_alarm_lamp = 1;
    }
    else
    {   //ȫ��
        LED1 = 0x00;
        LED2 = 0x00;
        LED3 = 0x00;
        LED4 = 0x00;
        LED5 = 0x00;
        LED6 = 0x00;
        LED7 = 0x00;
        LED8 = 0x00;
        LED9 = 0x00;
        LED10 = 0x00;
        LED11 = 0x00;
        LED12 = 0x00; 

        bflg_alarm_lamp = 0;
    }
}


/**************************************************************************************************************************************************************
�������ܣ�����TA��TU��TD����������ʱ������ܽ�����ʾ���������¶�


����λ�ã�1s----------------------------------------ok
****************************************************************************************************************************************************************/
void led_disp_alternate_delaytime(void)
{
    if(bflg_alarm_power_off == 0)
    {
        if((bflg_TA_fault == 1)||(bflg_TU_fault == 1)||(bflg_TD_fault == 1))
        {
            if(bflg_disp_alternate == 1)            
            {
                gss_disp_alternate_delaytime++;
                if(gss_disp_alternate_delaytime >= 2)  //�Դ��������ϴ���2s
                {
                    gss_disp_alternate_delaytime = 0;
                    bflg_disp_alternate = 0;
                }
            }
            else
            {
                gss_disp_alternate_delaytime++;
                if(gss_disp_alternate_delaytime >= 6) //���¶�6s
                {
                    gss_disp_alternate_delaytime = 0;
                    bflg_disp_alternate = 1;
                }
            }
        }
        else
        {
            gss_disp_alternate_delaytime = 0;
            bflg_disp_alternate = 0;
        }
    }
    else
    {
        gss_disp_alternate_delaytime = 0;
        bflg_disp_alternate = 0;
    }    
}
/**************************************************************************************************************************************************************
�������ܣ��ϵ�ʱ���������ʾ�¶Ⱥͺ�������


����λ�ã�1s----------------------------------------ok
****************************************************************************************************************************************************************/
void led_disp_black_screen_delaytime(void)
{
    if(bflg_alarm_power_off == 1)                    //�ϵ�ʱ��ʾ���������ȼ����ڴ���������
    {
        if(bflg_disp_black_screen == 1)
        {
            gss_disp_black_screen_delaytime++;
            if(gss_disp_black_screen_delaytime >= 30)  //����30s
            {
                gss_disp_black_screen_delaytime = 0;
                bflg_disp_black_screen = 0;
            }
        }
        else
        {
            gss_disp_black_screen_delaytime++;
            if(gss_disp_black_screen_delaytime >= 5)   //��ʾ�¶�5s
            {
                gss_disp_black_screen_delaytime = 0;
                bflg_disp_black_screen = 1;
            }
        }
    }
    else
    {
        gss_disp_black_screen_delaytime = 0;
        bflg_disp_black_screen = 0;
    }
}



#endif
