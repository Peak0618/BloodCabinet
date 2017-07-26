/**************************************************************************************************** 
overview:20170329
                �����Ĳ���:
                            1��ͨ������ѡ����벻ͬ��ģʽ
                            2��ͨ�������Բ������е���
*****************************************************************************************************/

//�������ļ�
#include "r_cg_macrodriver.h"
#include "r_cg_cgc.h"
#include "r_cg_port.h"
#include "h_type_define.h"
#include "m_key_operation.h"
#include "h_rom_para2.h"
#include "h_main.h"
#include "h_com.h"
#include "M_buzz.h"
#include "M_RTC.h"
#include "h_key_disp.h"
//��������
void mode_selection(void);
void key_locked_deal(void);
void key_locked_delaytime(void);   
void key_set_long_delaytime(void);
void key_set_up_long_delaytime(void);
void key_up_down_long_delaytime(void);
void key_set_down_long_delaytime(void);
void key_user_set_deal(void);
void key_user_manage_password_deal(void);
void key_user_manage_deal(void);
void key_factory_password_deal();
void key_factory_deal(void);
void key_RTC_time_parm_deal();
void key_RTC_time_parm_sent_usb_delaytime(void); 
void key_inquire_deal(void);
void key_ic_password_deal(void);
void key_ic_register_deal(void);
void operation_parm_read(void);
void IC_unlock_deal(void);
void IC_unlock_delaytime(void);  
void key_password_long_valid_delaytime(void);
void key_clear_password_long_falg(void);
void key_enter_mode_init(uint8_t mode_num,uint8_t parm_num,uint8_t dis_form);


//��־����
flag_type bflg_key_operation,bflg_key_operation_save;




//��


//��������
uint8_t   guc_key_val;                      //������ֵ
uint8_t   guc_TUTD_dis_type;                //ѡ����ʾ���� �����»�������ƽ��ֵ
uint8_t   guc_key_set_long_delaytimer;      //����set����ʱ
uint8_t   guc_key_set_up_long_delaytimer;   //����set+up����ʱ
uint8_t   guc_key_up_down_long_delaytimer;  //����up+down����ʱ
uint8_t   guc_key_set_down_long_delaytimer; //����set+down����ʱ
uint8_t   guc_key_locked_delaytimer;        //�����޲���������ʱ

int16_t   gss_user_manage_password_val;     //�û���������ֵ
int16_t   gss_IC_password_val;              //ic������ֵ
int8_t    gsc_factory_password_val;         //��������ֵ
int8_t    gsc_disp_inq_mode;                //��ѯģʽ�в�ͬ����

uint8_t   guc_disp_mode;                    //�ܵ�UIģʽ
int8_t    gsc_disp_parm_type;               //��ͬ�Ĳ�������,���еĲ����������
int16_t   gss_key_temp_value;               //��������ʱֵ,���еĲ����������
//
int16_t   gss_TS_old_value;                 //�����¶ȵľ�ֵ         
int16_t   gss_ALH_old_value;                //���ø��±����ľ�ֵ
int16_t   gss_ALL_old_value;                //���õ��±����ľ�ֵ
//
int8_t    guc_user_write_time_value;             //����RTCʱ�����ʱ����
uint8_t   gss_usb_set_time_delaytime;            //usb����ʱ����ʱ
int16_t   gss_key_password_long_valid_delaytime; //��������ʱ������ʱ����ɲ���10����
uint8_t   guc_lock_delaytimer;                   // IC�������Ĵ򿪵�ʱ��

int16_t   gss_user_set_parm_value[DISP_USER_SET_SUM];        //�û��趨����ֵ   
int16_t   gss_user_manage_parm_value[DISP_USER_MANAGE_SUM];  //�û��������ֵ   
int16_t   gss_factory_parm_value[DISP_FAC_SUM];              //��������ֵ  




//int16_t   gss_THW_calibration;              
/**********************************************************************************************************************************************
��������: ģʽѡ��,ͨ������ѡ����벻ͬ��ģʽ


����λ��: ��ѭ��(��ȡ������ʱ)----------------------------------------------ok
***********************************************************************************************************************************************/
void mode_selection(void)
{
    if (bflg_test_type == 1)                     //����ǲ��԰���ʾ��
    {
        if (guc_key_val == 0x20)                 //SET---�����µ����ü���ѪҺ�Ĳ�ͬ
        {
            bflg_test_mode = 1;                  //�ò���ģʽ��־
        } 
        else if(guc_key_val == 0x40)             //BUZZ��
        {
        	bflg_test_mode_RTC = 1;
        	//test_mode_RTC_circuit();
        }
    }
    else if(guc_disp_mode == DISP_LOCKED)               //����ʱ��������
    {
        key_locked_deal();     
    }
    else if(guc_disp_mode == DISP_USER_SET)             //�û����ò����
    {
        key_user_set_deal();
    }
    else if(guc_disp_mode == DISP_USER_MANAGE_PASSWORD) //�û����������
    {
    	key_user_manage_password_deal();
    }
    else if(guc_disp_mode == DISP_USER_MANAGE)          //�û���������
    {
    	key_user_manage_deal();
    }
    else if(guc_disp_mode == DISP_FACTORY_PASSWORD)     //��������
    {
        key_factory_password_deal();
    }
    else if(guc_disp_mode == DISP_FACTORY)              //��������
    {
        key_factory_deal();
    }
    else if(guc_disp_mode == DISP_INQUIRE)              //��ѯ�����
    {
        key_inquire_deal();
    }
    else if(guc_disp_mode == DISP_TIME_SET)             //RTCʱ������ģʽ
    {
        key_RTC_time_parm_deal();
    }      
    else if(guc_disp_mode == DISP_IC_PASSWORD)          //IC��������  
    {
        key_ic_password_deal();
    }
    else if(guc_disp_mode == DISP_IC_REGISTER)          //ICע������
    {
        key_ic_register_deal();
    }
}

/**********************************************************************************************************************************************
��������: ����ģʽ�°����Ĳ���


����λ��: mode_selection(����ѭ��)---------------------------------------------ok
***********************************************************************************************************************************************/
void key_locked_deal(void)
{
    //�����º�ƽ��ֵ��ʾ�л�
    if(guc_key_val == KEY_UP)             //��up�л����º�������
    {
        if(guc_TUTD_dis_type == 0)
        {
            guc_TUTD_dis_type = 1;
        }
        else
        {
            guc_TUTD_dis_type = 0;
        }
    }
    
    if(guc_key_val == KEY_DOWN)            //��down�л����º�������
    {
        if(guc_TUTD_dis_type == 0)
        {
            guc_TUTD_dis_type = 2;
        }
        else
        {
            guc_TUTD_dis_type = 0;
        }
    }
    //----------------------------------------------------------------------------------------
    if (guc_key_val == KEY_SET)          //������set�� ��5s����USER SET
    {
        if (bflg_key_set_long_valid == 0)           
        {
            if (bflg_key_set_long_delaytime == 0)    
            {
                bflg_key_set_long_delaytime = 1;     
                guc_key_set_long_delaytimer = 0;
            }
        }
        else                                     
        {
            bflg_key_set_long_valid = 0;
            
            key_enter_mode_init(DISP_USER_SET,0,0);
            gss_user_set_parm_value[DISP_USER_SET_T2] = 0; //����P1��ʼ
        }
    } 
    else
    {
        bflg_key_set_long_delaytime = 0;      //clear
        guc_key_set_long_delaytimer = 0;
    }
    //----------------------------------------------------------------------------------------
    if(guc_key_val == KEY_SET_AND_UP)  //���� ���趨+�ϵ��� 5s���빤������
    {
        if (bflg_key_set_up_long_valid == 0)           
        {
            if (bflg_key_set_up_long_delaytime == 0)    
            {
                bflg_key_set_up_long_delaytime = 1;     
                guc_key_set_up_long_delaytimer = 0;
            }
        }
        else                                     
        {
            bflg_key_set_up_long_valid = 0;
            
            key_enter_mode_init(DISP_FACTORY_PASSWORD,0,0);
            gsc_factory_password_val = 0;       
        }
    }
    else
    {
        bflg_key_set_up_long_delaytime = 0;     
        guc_key_set_up_long_delaytimer = 0;
    }
    //----------------------------------------------------------------------------------------
    if(guc_key_val == KEY_UP_AND_DOWN)  //���� ���ϵ�+�µ��� 5s�����ѯģʽ
    {
        if (bflg_key_up_down_long_valid == 0)           
        {
            if (bflg_key_up_down_long_delaytime == 0)    
            {
                bflg_key_up_down_long_delaytime = 1;     
                guc_key_up_down_long_delaytimer = 0;
            }
        }
        else                                     
        {
            bflg_key_up_down_long_valid = 0;
            
            key_enter_mode_init(DISP_INQUIRE,0,0);
            gsc_disp_inq_mode = 0;
            bflg_key_inq_val = 0;
        }
    }
    else
    {
        bflg_key_up_down_long_delaytime = 0;     
        guc_key_up_down_long_delaytimer = 0;
    }
    //----------------------------------------------------------------------------------------
    if(guc_key_val == KEY_SET_AND_DOWN)  //���� ������+�µ��� 5s����IC����ģʽ
    {
        if (bflg_key_set_down_long_valid == 0)           
        {
            if (bflg_key_set_down_long_delaytime == 0)    
            {
                bflg_key_set_down_long_delaytime = 1;     
                guc_key_set_down_long_delaytimer = 0;
            }
        }
        else                                     
        {
            bflg_key_set_down_long_valid = 0;
            
            bflg_IC_register_state = 1;         //��ʾ����ע��״̬

            key_enter_mode_init(DISP_IC_PASSWORD,0,0); //����ICע������״̬
            gss_IC_password_val = 0;                              
            guc_IC_password_cnt = 0;
        }
    }
    else
    {
        bflg_key_set_down_long_delaytime = 0;     
        guc_key_set_down_long_delaytimer = 0;
    }
}

/**********************************************************************************************************************************************
��������: 20s ����û�ж�����������״̬


����λ��:  100ms -----------------------------------------ok  
***********************************************************************************************************************************************/
void key_locked_delaytime(void)   
{
    if (guc_key_val == KEY_NONE)
    {
        guc_key_locked_delaytimer++;
        if (guc_key_locked_delaytimer >= 200)    //200*100ms=20s
        {
            guc_key_locked_delaytimer = 0;
            key_enter_mode_init(DISP_LOCKED,0,0);
        }
    }
    else
    {
        guc_key_locked_delaytimer = 0;
    }
}

/**********************************************************************************************************************************************
��������: ����ģʽ�³���set����ʱ


����λ��: 100ms��ʱ��----------------------------------------------ok
***********************************************************************************************************************************************/
void key_set_long_delaytime(void)
{
    if (bflg_key_set_long_delaytime == 1)
    {
        guc_key_set_long_delaytimer++;
        if (guc_key_set_long_delaytimer >= 50)  //50*100 = 5s
        {
            guc_key_set_long_delaytimer = 0;
            bflg_key_set_long_delaytime = 0;
            
            bflg_key_set_long_valid = 1;       
        }
    }
}
/**********************************************************************************************************************************************
��������: ����ģʽ�³���set+up����ʱ


����λ��: 100ms��ʱ��----------------------------------------------ok
***********************************************************************************************************************************************/
void key_set_up_long_delaytime(void)
{
    if (bflg_key_set_up_long_delaytime == 1)
    {
        guc_key_set_up_long_delaytimer++;
        if (guc_key_set_up_long_delaytimer >= 50)  //50*100 = 5s
        {
            guc_key_set_up_long_delaytimer = 0;
            bflg_key_set_up_long_delaytime = 0;
            
            bflg_key_set_up_long_valid = 1;       
        }
    }
}

/**********************************************************************************************************************************************
��������: ����ģʽ�³���up+down����ʱ


����λ��: 100ms��ʱ��----------------------------------------------ok
***********************************************************************************************************************************************/
void key_up_down_long_delaytime(void)
{
    if (bflg_key_up_down_long_delaytime == 1)
    {
        guc_key_up_down_long_delaytimer++;
        if (guc_key_up_down_long_delaytimer >= 50)  //50*100 = 5s
        {
            guc_key_up_down_long_delaytimer = 0;
            bflg_key_up_down_long_delaytime = 0;
            
            bflg_key_up_down_long_valid = 1;       
        }
    }
}

/**********************************************************************************************************************************************
��������: ����ģʽ�³���set+down����ʱ


����λ��: 100ms��ʱ��----------------------------------------------ok
***********************************************************************************************************************************************/
void key_set_down_long_delaytime(void)
{
    if (bflg_key_set_down_long_delaytime == 1)
    {
        guc_key_set_down_long_delaytimer++;
        if (guc_key_set_down_long_delaytimer >= 50)  //50*100 = 5s
        {
            guc_key_set_down_long_delaytimer = 0;
            bflg_key_set_down_long_delaytime = 0;
            
            bflg_key_set_down_long_valid = 1;       
        }
    }
}

/**********************************************************************************************************************************************
��������: �û��趨��ģʽ�°����Ĳ���

ʹ��ע�⣺����˲�ʱ��
         1��   guc_disp_mode = DISP_USER_SET;
         2��   gsc_disp_parm_type = 0;      Ĭ����ʾ�׸�����
         3��   gss_key_temp_value = 0;      ��ʱ����Ϊ0
         4��   bflg_parm_save = 0;          Ĭ�ϲ�����
         5��   bflg_parm_dis_form = 0��      Ĭ����ʾ��������

����:  �����в����Ĳ���ֵ����1�����Բ���ͳһ         
         
����λ��: mode_selection(����ѭ��)---------------------------------------------ok
***********************************************************************************************************************************************/
void key_user_set_deal(void)
{
    int16_t lss_disp1_TS_temp,lss_disp1_ALH_temp,lss_disp1_ALL_temp;
    
    lss_disp1_TS_temp  = gss_user_set_parm_value[DISP_USER_SET_TS];
    lss_disp1_ALH_temp = gss_user_set_parm_value[DISP_USER_SET_ALH];
    lss_disp1_ALL_temp = gss_user_set_parm_value[DISP_USER_SET_ALL];
    
    if(guc_key_val == KEY_BUZ)
    {
        guc_disp_mode = DISP_LOCKED;
        bflg_parm_dis_form = 0;
    }
    else
    {
        if(bflg_parm_dis_form == 0)
        {
            if(guc_key_val == KEY_SET)
            {
                
                if(gsc_disp_parm_type == DISP_USER_SET_SS) // special
                {
                    key_enter_mode_init(DISP_USER_MANAGE_PASSWORD,0,0);
                    gss_user_manage_password_val = 0;                 
                }
                else
                {
                    bflg_parm_dis_form = 1;
                }
            }
            else
            {
                if(guc_key_val == KEY_UP)
                {
                    gsc_disp_parm_type++;
                    if(gsc_disp_parm_type > DISP_USER_SET_SS)
                    {
                        gsc_disp_parm_type = DISP_USER_SET_TS;
                    }
                }
                else if(guc_key_val == KEY_DOWN)
                {
                    gsc_disp_parm_type--;
                    if(gsc_disp_parm_type < DISP_USER_SET_TS)
                    {
                        gsc_disp_parm_type = DISP_USER_SET_SS;
                    }
                }
            }
        }
        else
        {
            if(guc_key_val == KEY_SET)
            {
                bflg_parm_dis_form = 0;

                if(gsc_disp_parm_type == DISP_USER_SET_T2) //into usb time layer
                {
                    bflg_parm_save = 0;
                    gss_key_temp_value = 0;
                    
                    key_enter_mode_init(DISP_TIME_SET,0,0);                     //����ʱ�����ò�
                    
                    if(gss_user_set_parm_value[DISP_USER_SET_T2] == 0)        //P1 ��      //��ȡ��ʱ��ʱ��ֵ
                    {
                        guc_user_write_time_value = guc_RTC_r_year;
                    }
                    else if(gss_user_set_parm_value[DISP_USER_SET_T2] == 1)   //P2 ��
                    {
                        guc_user_write_time_value = guc_RTC_r_month;
                    }
                    else if(gss_user_set_parm_value[DISP_USER_SET_T2] == 2)    //P3 ��
                    {
                        guc_user_write_time_value = guc_RTC_r_day;
                    }
                    else if(gss_user_set_parm_value[DISP_USER_SET_T2] == 3)    //P4 ʱ
                    {
                        guc_user_write_time_value = guc_RTC_r_hour;
                    }
                    else if(gss_user_set_parm_value[DISP_USER_SET_T2] == 4)    //P5 ��
                    {
                        guc_user_write_time_value = guc_RTC_r_minute;
                    }
                }
                else
                {
                    if(bflg_parm_save == 1)    
                    {
                        bflg_parm_save = 0;
                        
                        if(gss_user_set_parm_value[gsc_disp_parm_type] != gss_key_temp_value) //save
                        {
                            gss_user_set_parm_value[gsc_disp_parm_type] = gss_key_temp_value;
                            gss_key_temp_value = 0;
                        
                            //save
                            if(gsc_disp_parm_type == DISP_USER_SET_TS)
                            {
                                gss_TS_old_value = lss_disp1_TS_temp;  //usb����ʱʹ��
                                bflg_sent_usb_TS = 1;
                                
                                //-------------------------------------------------------------------------------------------------------------------------------------
                                gss_user_set_parm_value[DISP_USER_SET_ALH] = gss_user_set_parm_value[DISP_USER_SET_TS] + (gss_user_set_parm_value[DISP_USER_SET_ALH] - lss_disp1_TS_temp);
                                if(gss_user_set_parm_value[DISP_USER_SET_ALH] >= 100) 
                                {
                                    gss_user_set_parm_value[DISP_USER_SET_ALH] = 100;
                                }
                                else if(gss_user_set_parm_value[DISP_USER_SET_ALH] <= (gss_user_set_parm_value[DISP_USER_SET_TS] + 10))
                                {
                                    gss_user_set_parm_value[DISP_USER_SET_ALH] = gss_user_set_parm_value[DISP_USER_SET_TS] + 10;
                                }

                                gss_user_set_parm_value[DISP_USER_SET_ALL] = gss_user_set_parm_value[DISP_USER_SET_TS] - (lss_disp1_TS_temp - gss_user_set_parm_value[DISP_USER_SET_ALL]);
                                if(gss_user_set_parm_value[DISP_USER_SET_ALL] >= (gss_user_set_parm_value[DISP_USER_SET_TS] - 10))
                                {
                                    gss_user_set_parm_value[DISP_USER_SET_ALL] = gss_user_set_parm_value[DISP_USER_SET_TS] - 10;
                                }
                                else if(gss_user_set_parm_value[DISP_USER_SET_ALL] <= 10)
                                {
                                    gss_user_set_parm_value[DISP_USER_SET_ALL] = 10;
                                }

                                ram_para[num_user_set_ALH] = gss_user_set_parm_value[gsc_disp_parm_type];                                                                    //!!!!!!!
                                eeprom_write(num_user_set_ALH, ram_para[num_user_set_ALH]);

                                ram_para[num_user_set_ALL] = gss_user_set_parm_value[gsc_disp_parm_type];                                                                    //!!!!!!!
                                eeprom_write(num_user_set_ALL, ram_para[num_user_set_ALL]);                                    
                                
                                ram_para[num_user_set_TS] = gss_user_set_parm_value[gsc_disp_parm_type];                                                                    //!!!!!!!
                                eeprom_write(num_user_set_TS, ram_para[num_user_set_TS]);
                            }
                            else if(gsc_disp_parm_type == DISP_USER_SET_ALH)
                            {
                                gss_ALH_old_value = lss_disp1_ALH_temp;  //usb����ʱʹ��
                                bflg_sent_usb_ALH = 1;
                                
                                ram_para[num_user_set_ALH] = gss_user_set_parm_value[gsc_disp_parm_type];                                                                    //!!!!!!!
                                eeprom_write(num_user_set_ALH, ram_para[num_user_set_ALH]);
                            }
                            else if(gsc_disp_parm_type == DISP_USER_SET_ALL)
                            {
                                gss_ALL_old_value = lss_disp1_ALL_temp;  //usb����ʱʹ��
                                bflg_sent_usb_ALL = 1;
                                
                                ram_para[num_user_set_ALL] = gss_user_set_parm_value[gsc_disp_parm_type];                                                                    //!!!!!!!
                                eeprom_write(num_user_set_ALL, ram_para[num_user_set_ALL]);
                            }
                            else if(gsc_disp_parm_type == DISP_USER_SET_T1)
                            {
                                ram_para[num_user_set_T1] = gss_user_set_parm_value[gsc_disp_parm_type];                                                                    //!!!!!!!
                                eeprom_write(num_user_set_T1, ram_para[num_user_set_T1]);

                                bflg_sent_usb_T1 = 1; 
                            }
                            else if(gsc_disp_parm_type == DISP_USER_SET_T2)
                            {
                                //NO SAVE
                            }                        
                            else if(gsc_disp_parm_type == DISP_USER_SET_P6)
                            {
                                ram_para[num_user_set_P6] = gss_user_set_parm_value[gsc_disp_parm_type];                                                                    //!!!!!!!
                                eeprom_write(num_user_set_P6, ram_para[num_user_set_P6]);
                                
                                bflg_sent_usb_P6 = 1;
                            }         
                            else if(gsc_disp_parm_type == DISP_USER_SET_SS)
                            {
                                //NO SAVE
                            }                        
                        }
                    }
                }
            
            }
            else if(guc_key_val == KEY_UP)
            {
                if(bflg_parm_save == 0)
                {
                    bflg_parm_save = 1;
                    gss_key_temp_value = gss_user_set_parm_value[gsc_disp_parm_type];
                }
                gss_key_temp_value++;         

                if(gsc_disp_parm_type == DISP_USER_SET_TS)
                {
                    if(gss_key_temp_value > 60)    //6��  magnify 10
                    {
                        gss_key_temp_value = 60;
                    }
                }
                else if(gsc_disp_parm_type == DISP_USER_SET_ALH)
                {
                    if(gss_key_temp_value > 100)    //10��
                    {
                        gss_key_temp_value = 100;
                    }
                }
                else if(gsc_disp_parm_type == DISP_USER_SET_ALL)
                {
                    if(gss_key_temp_value > (gss_user_set_parm_value[DISP_USER_SET_TS] - 10))    
                    {
                        gss_key_temp_value = gss_user_set_parm_value[DISP_USER_SET_TS] - 10;
                    }
                }
                else if(gsc_disp_parm_type == DISP_USER_SET_T1)
                {
                    if(gss_key_temp_value > 30)     //30min
                    {
                        gss_key_temp_value = 30;
                    }
                }
                else if(gsc_disp_parm_type == DISP_USER_SET_T2)
                {
                    gss_user_set_parm_value[DISP_USER_SET_T2]++;
                    if(gss_user_set_parm_value[DISP_USER_SET_T2] > 4)     //P1-P5
                    {
                        gss_user_set_parm_value[DISP_USER_SET_T2] = 0;
                    }
                }
                else if(gsc_disp_parm_type == DISP_USER_SET_P6)
                {
                    if(gss_key_temp_value > 12)     //12month
                    {
                        gss_key_temp_value = 12;
                    }
                }
                //NO  DISP_USER_SET_SS
            }
            else if(guc_key_val == KEY_DOWN)
            {
                if(bflg_parm_save == 0)
                {
                    bflg_parm_save = 1;
                    gss_key_temp_value = gss_user_set_parm_value[gsc_disp_parm_type];
                }
                gss_key_temp_value--;
                
                if(gsc_disp_parm_type == DISP_USER_SET_TS)
                {
                    if(gss_key_temp_value < 20)    //2��  magnify 10
                    {
                        gss_key_temp_value = 20;
                    }
                }
                else if(gsc_disp_parm_type == DISP_USER_SET_ALH)
                {
                    if(gss_key_temp_value < (gss_user_set_parm_value[DISP_USER_SET_TS] + 10))   
                    {
                        gss_key_temp_value = gss_user_set_parm_value[DISP_USER_SET_TS] + 10;
                    }
                }
                else if(gsc_disp_parm_type == DISP_USER_SET_ALL)
                {
                    if(gss_key_temp_value < 10)    //1��
                    {
                        gss_key_temp_value = 10;
                    }
                }
                else if(gsc_disp_parm_type == DISP_USER_SET_T1)
                {
                    if(gss_key_temp_value < 1)    
                    {
                        gss_key_temp_value = 1;  //1min
                    }
                }
                else if(gsc_disp_parm_type == DISP_USER_SET_T2)
                {
                    gss_user_set_parm_value[DISP_USER_SET_T2]--;
                    if(gss_user_set_parm_value[DISP_USER_SET_T2] < 0)     //P1-P5
                    {
                        gss_user_set_parm_value[DISP_USER_SET_T2] = 4;
                    }
                }
                else if(gsc_disp_parm_type == DISP_USER_SET_P6)
                {
                    if(gss_key_temp_value < 0)    
                    {
                        gss_key_temp_value = 0;  
                    }
                }
            }
        }
    }
}


/**********************************************************************************************************************************************
��������: ���û��趨������û������ʱ���������

ʹ��ע��: ����˲�Ӧ
          1��  guc_disp_mode = DISP_USER_MANAGE_PASSWORD;
          2��  gss_user_manage_password_val = 0;  

����λ��: mode_selection(����ѭ��)---------------------------------------------ok
***********************************************************************************************************************************************/
void key_user_manage_password_deal(void)
{  
    if (guc_key_val == KEY_BUZ)
    {
        guc_disp_mode = DISP_LOCKED;   
    }
    else if (guc_key_val == KEY_UP)
    {          
        gss_user_manage_password_val++;
        if (gss_user_manage_password_val > 999)
        {
            gss_user_manage_password_val = 0;
        }
    }
    else if (guc_key_val == KEY_DOWN)
    {    
        gss_user_manage_password_val--;
        if (gss_user_manage_password_val < 0)
        {
            gss_user_manage_password_val = 999;
        }
    }
    else if (guc_key_val == KEY_SET)
    {           
        if (gss_user_manage_password_val == gss_user_manage_parm_value[DISP_USER_MANAGE_IC])                //��ʱ��Ĭ��
        {
            key_enter_mode_init(DISP_USER_MANAGE,0,0);  
        }
    }
}


/**********************************************************************************************************************************************
��������: �û������İ�������

ʹ��ע�⣺����˲�ʱ��
         1��   guc_disp_mode = DISP_USER_MANAGE;
         2��   gsc_disp_parm_type = 0;     Ĭ����ʾ�׸�����
         3��   gss_key_temp_value = 0;     ��ʱ����Ϊ0
         4��   bflg_parm_save = 0;         Ĭ�ϲ�����
         5��   bflg_parm_dis_form = 0��     Ĭ����ʾ��������

����:  �����в����Ĳ���ֵ����1�����Բ���ͳһ    


����λ��: mode_selection(����ѭ��)----------------------------------------------ok
***********************************************************************************************************************************************/
void key_user_manage_deal(void)
{
    if(guc_key_val == KEY_BUZ)
    {
        guc_disp_mode = DISP_LOCKED;
    }
    else
    {
        if(bflg_parm_dis_form == 0)
        {
            if(guc_key_val == KEY_SET)
            {
                if(gsc_disp_parm_type == DISP_USER_MANAGE_CL1)   //special ����ICע��
                {                   
                    guc_disp_mode = DISP_IC_PASSWORD;      

                    gss_IC_password_val = 0;                       
                    guc_IC_password_cnt = 0;             
                }
                else
                {
                    bflg_parm_dis_form = 1;        
                }
            }
            else
            {
                if(guc_key_val == KEY_UP)
                {
                    gsc_disp_parm_type++;
                   
                    if(gsc_disp_parm_type > DISP_USER_MANAGE_TT)
                    {
                        gsc_disp_parm_type = DISP_USER_MANAGE_IC;
                    }
                }
                else if(guc_key_val == KEY_DOWN)
                {
                    gsc_disp_parm_type--;
                    if(gsc_disp_parm_type < DISP_USER_MANAGE_IC)
                    {
                        gsc_disp_parm_type = DISP_USER_MANAGE_TT;
                    }
                }
            }
        }
        else
        {
            if(guc_key_val == KEY_SET)
            {
                bflg_parm_dis_form = 0;
                
                if(bflg_parm_save == 1)    
                {
                    bflg_parm_save = 0;

                    if(gsc_disp_parm_type == DISP_USER_MANAGE_CL1)  //special_IC��ע��
                    {
                         //��+
                         //��������IC�����
                    }
                    else
                    {
                        if(gss_user_manage_parm_value[gsc_disp_parm_type] != gss_key_temp_value) //save
                        {
                            gss_user_manage_parm_value[gsc_disp_parm_type] = gss_key_temp_value;
                            gss_key_temp_value = 0;
                        
                            if(gsc_disp_parm_type == DISP_USER_MANAGE_IC)
                            {
                                ram_para[num_user_manage_IC] = gss_user_manage_parm_value[gsc_disp_parm_type];                                                                    //!!!!!!!
                                eeprom_write(num_user_manage_IC, ram_para[num_user_manage_IC]);
                            }
                            else if(gsc_disp_parm_type == DISP_USER_MANAGE_CL1)
                            {
                                //NO SAVE
                            }
                            else if(gsc_disp_parm_type == DISP_USER_MANAGE_Lt)
                            {
                                ram_para[num_user_manage_Lt] = gss_user_manage_parm_value[gsc_disp_parm_type];                                                                    //!!!!!!!
                                eeprom_write(num_user_manage_Lt, ram_para[num_user_manage_Lt]);
                            }
                            else if(gsc_disp_parm_type == DISP_USER_MANAGE_dA)
                            {
                                ram_para[num_user_manage_dA] = gss_user_manage_parm_value[gsc_disp_parm_type];                                                                    //!!!!!!!
                                eeprom_write(num_user_manage_dA, ram_para[num_user_manage_dA]);
                            }
                            else if(gsc_disp_parm_type == DISP_USER_MANAGE_FA)
                            {
                                ram_para[num_user_manage_FA] = gss_user_manage_parm_value[gsc_disp_parm_type];                                                                    //!!!!!!!
                                eeprom_write(num_user_manage_FA, ram_para[num_user_manage_FA]);
                            }
                            else if(gsc_disp_parm_type == DISP_USER_MANAGE_S1)
                            {
                                ram_para[num_user_manage_S1] = gss_user_manage_parm_value[gsc_disp_parm_type];                                                                    //!!!!!!!
                                eeprom_write(num_user_manage_S1, ram_para[num_user_manage_S1]);
                            }
                            else if(gsc_disp_parm_type == DISP_USER_MANAGE_S2)
                            {
                                ram_para[num_user_manage_S2] = gss_user_manage_parm_value[gsc_disp_parm_type];                                                                    //!!!!!!!
                                eeprom_write(num_user_manage_S2, ram_para[num_user_manage_S2]);
                            }
                            else if(gsc_disp_parm_type == DISP_USER_MANAGE_TT)
                            {
                                ram_para[num_user_manage_TT] = gss_user_manage_parm_value[gsc_disp_parm_type];                                                                    //!!!!!!!
                                eeprom_write(num_user_manage_TT, ram_para[num_user_manage_TT]);
                            }                        
                        }
                    }
                }
            }
            else if(guc_key_val == KEY_UP)
            {
                if(bflg_parm_save == 0)
                {
                    bflg_parm_save = 1;
                    gss_key_temp_value = gss_user_manage_parm_value[gsc_disp_parm_type];
                }
                gss_key_temp_value++;         

                if(gsc_disp_parm_type == DISP_USER_MANAGE_IC)
                {
                    if(gss_key_temp_value > 999)    
                    {
                        gss_key_temp_value = 999;
                    }
                }
                else if(gsc_disp_parm_type == DISP_USER_MANAGE_CL1)
                {
                    // wait
                }
                else if(gsc_disp_parm_type == DISP_USER_MANAGE_Lt)
                {
                    if(gss_key_temp_value > 60)    
                    {
                        gss_key_temp_value = 60;
                    }
                }
                else if(gsc_disp_parm_type == DISP_USER_MANAGE_dA)
                {
                    if(gss_key_temp_value > 30)     
                    {
                        gss_key_temp_value = 30;
                    }
                }
                else if(gsc_disp_parm_type == DISP_USER_MANAGE_FA)
                {
                    if(gss_key_temp_value > 3)     
                    {
                        gss_key_temp_value = 3;
                    }
                }
                else if(gsc_disp_parm_type == DISP_USER_MANAGE_S1)
                {
                    if(gss_key_temp_value > 1)     
                    {
                        gss_key_temp_value = 1;
                    }
                }
                else if(gsc_disp_parm_type == DISP_USER_MANAGE_S2)
                {
                    if(gss_key_temp_value > 999)     
                    {
                        gss_key_temp_value = 999;
                    }
                }  
                else if(gsc_disp_parm_type == DISP_USER_MANAGE_TT)
                {
                    if(gss_key_temp_value > 1)     
                    {
                        gss_key_temp_value = 1;
                    }
                }                      
            }
            else if(guc_key_val == KEY_DOWN)
            {
                if(bflg_parm_save == 0)
                {
                    bflg_parm_save = 1;
                    gss_key_temp_value = gss_user_manage_parm_value[gsc_disp_parm_type];
                }
                gss_key_temp_value--;         

                if(gsc_disp_parm_type == DISP_USER_MANAGE_IC)
                {
                    if(gss_key_temp_value < 0)    
                    {
                        gss_key_temp_value = 0;
                    }
                }
                else if(gsc_disp_parm_type == DISP_USER_MANAGE_CL1)
                {
                    // wait
                }
                else if(gsc_disp_parm_type == DISP_USER_MANAGE_Lt)
                {
                    if(gss_key_temp_value < -1)    
                    {
                        gss_key_temp_value = -1;
                    }
                }
                else if(gsc_disp_parm_type == DISP_USER_MANAGE_dA)
                {
                    if(gss_key_temp_value < 1)     
                    {
                        gss_key_temp_value = 1;
                    }
                }
                else if(gsc_disp_parm_type == DISP_USER_MANAGE_FA)
                {
                    if(gss_key_temp_value < 1)     
                    {
                        gss_key_temp_value = 1;
                    }
                }
                else if(gsc_disp_parm_type == DISP_USER_MANAGE_S1)
                {
                    if(gss_key_temp_value > 1)     
                    {
                        gss_key_temp_value = 1;
                    }
                }
                else if(gsc_disp_parm_type == DISP_USER_MANAGE_S2)
                {
                    if(gss_key_temp_value < 0)     
                    {
                        gss_key_temp_value = 0;
                    }
                }  
                else if(gsc_disp_parm_type == DISP_USER_MANAGE_TT)
                {
                    if(gss_key_temp_value < 0)     
                    {
                        gss_key_temp_value = 0;
                    }
                }                      
            }
        }
    }
}


/**********************************************************************************************************************************************
��������: ��������ģʽ�µĲ���


����λ��: mode_selection(����ѭ��)---------------------------------------------ok
***********************************************************************************************************************************************/
void key_factory_password_deal(void)
{  
    if (guc_key_val == KEY_BUZ)
    {
        guc_disp_mode = DISP_LOCKED;   
    }
    else if (guc_key_val == KEY_UP)
    {          
        gsc_factory_password_val++;
        if (gsc_factory_password_val > 99)
        {
            gsc_factory_password_val = 0;
        }
    }
    else if (guc_key_val == KEY_DOWN)
    {    
        gsc_factory_password_val--;
        if (gsc_factory_password_val < 0)
        {
            gsc_factory_password_val = 99;
        }
    }
    else if (guc_key_val == KEY_SET)
    {           
        if (gsc_factory_password_val == gss_factory_parm_value[DISP_FAC_PS])   
        {
            key_enter_mode_init(DISP_FACTORY,0,0);    
        }
    }
}


/**********************************************************************************************************************************************
��������: ����ģʽ�µİ�������


����λ��: mode_selection(����ѭ��)---------------------------------------------ok
***********************************************************************************************************************************************/
void key_factory_deal(void)
{
    if(guc_key_val == KEY_BUZ)
    {
        guc_disp_mode = DISP_LOCKED;
    }
    else
    {
        if(bflg_parm_dis_form == 0)
        {
            if(guc_key_val == KEY_SET)
            {
                bflg_parm_dis_form = 1;
            }
            else
            {
                if(guc_key_val == KEY_UP)
                {
                    gsc_disp_parm_type++;
                   
                    if(gsc_disp_parm_type > DISP_FAC_PCH)
                    {
                        gsc_disp_parm_type = DISP_FAC_CL2;
                    }
                }
                else if(guc_key_val == KEY_DOWN)
                {
                    gsc_disp_parm_type--;
                    if(gsc_disp_parm_type < DISP_FAC_CL2)
                    {
                        gsc_disp_parm_type = DISP_FAC_PCH;
                    }
                }
            }
        }
        else
        {
            if(guc_key_val == KEY_SET)
            {
                bflg_parm_dis_form = 0;
                
                if(bflg_parm_save == 1)    
                {
                    bflg_parm_save = 0;

                   
                    if((gsc_disp_parm_type == DISP_FAC_CL2) ||   //special
                        (gsc_disp_parm_type == DISP_FAC_CL3)||
                        (gsc_disp_parm_type == DISP_FAC_CL4))
                    {
                        if(gsc_disp_parm_type == DISP_FAC_CL2)
                        {
                            //��+
                            bflg_sent_usb_CL2 = 1;                  //�÷�����usbָ��
                            guc_buzz_sound_cnt = 1;                 //����1�� 
                        }
                        else if(gsc_disp_parm_type == DISP_FAC_CL3)
                        {
                            //��+
                        }
                        else if(gsc_disp_parm_type == DISP_FAC_CL4)
                        {
                            //��+
                        }
                    }   
                    else
                    {
                        if(gss_factory_parm_value[gsc_disp_parm_type] != gss_key_temp_value) //save
                        {
                            gss_factory_parm_value[gsc_disp_parm_type] = gss_key_temp_value;
                            gss_key_temp_value = 0;
                        
                            //save_wait
                            if(gsc_disp_parm_type == DISP_FAC_CL2)
                            {
                                //NO SAVE
                            }    
                            else if(gsc_disp_parm_type == DISP_FAC_UL)
                            {
                                ram_para[num_fac_UL] = gss_factory_parm_value[gsc_disp_parm_type];                                                                    //!!!!!!!
                                eeprom_write(num_fac_UL, ram_para[num_fac_UL]);

                                bflg_sent_usb_UL = 1;
                            } 
                            else if(gsc_disp_parm_type == DISP_FAC_r0)
                            {
                                ram_para[num_fac_r0] = gss_factory_parm_value[gsc_disp_parm_type];                                                                    //!!!!!!!
                                eeprom_write(num_fac_r0, ram_para[num_fac_r0]);
                            }  
                            else if(gsc_disp_parm_type == DISP_FAC_b0F)
                            {
                                ram_para[num_fac_b0F] = gss_factory_parm_value[gsc_disp_parm_type];                                                                    //!!!!!!!
                                eeprom_write(num_fac_b0F, ram_para[num_fac_b0F]);
                            }    
                            else if(gsc_disp_parm_type == DISP_FAC_br1)
                            {
                                ram_para[num_fac_br1] = gss_factory_parm_value[gsc_disp_parm_type];                                                                    //!!!!!!!
                                eeprom_write(num_fac_br1, ram_para[num_fac_br1]);
                            }
                            else if(gsc_disp_parm_type == DISP_FAC_br2)
                            {
                                ram_para[num_fac_br2] = gss_factory_parm_value[gsc_disp_parm_type];                                                                    //!!!!!!!
                                eeprom_write(num_fac_br2, ram_para[num_fac_br2]);
                            }    
                            else if(gsc_disp_parm_type == DISP_FAC_br3)
                            {
                                ram_para[num_fac_br3] = gss_factory_parm_value[gsc_disp_parm_type];                                                                    //!!!!!!!
                                eeprom_write(num_fac_br3, ram_para[num_fac_br3]);
                            }  
                            else if(gsc_disp_parm_type == DISP_FAC_br4)
                            {
                                ram_para[num_fac_br4] = gss_factory_parm_value[gsc_disp_parm_type];                                                                    //!!!!!!!
                                eeprom_write(num_fac_br4, ram_para[num_fac_br4]);
                            }    
                            else if(gsc_disp_parm_type == DISP_FAC_Ct1)
                            {
                                ram_para[num_fac_Ct1] = gss_factory_parm_value[gsc_disp_parm_type];                                                                    //!!!!!!!
                                eeprom_write(num_fac_Ct1, ram_para[num_fac_Ct1]);
                            }
                            else if(gsc_disp_parm_type == DISP_FAC_Ct2)
                            {
                                ram_para[num_fac_Ct2] = gss_factory_parm_value[gsc_disp_parm_type];                                                                    //!!!!!!!
                                eeprom_write(num_fac_Ct2, ram_para[num_fac_Ct2]);
                            }    
                            else if(gsc_disp_parm_type == DISP_FAC_Ct3)
                            {
                                ram_para[num_fac_Ct3] = gss_factory_parm_value[gsc_disp_parm_type];                                                                    //!!!!!!!
                                eeprom_write(num_fac_Ct3, ram_para[num_fac_Ct3]);
                            }  
                            else if(gsc_disp_parm_type == DISP_FAC_Ct4)
                            {
                                ram_para[num_fac_Ct4] = gss_factory_parm_value[gsc_disp_parm_type];                                                                    //!!!!!!!
                                eeprom_write(num_fac_Ct4, ram_para[num_fac_Ct4]);
                            }    
                            else if(gsc_disp_parm_type == DISP_FAC_Ct5)
                            {
                                ram_para[num_fac_Ct5] = gss_factory_parm_value[gsc_disp_parm_type];                                                                    //!!!!!!!
                                eeprom_write(num_fac_Ct5, ram_para[num_fac_Ct5]);
                            }
                            else if(gsc_disp_parm_type == DISP_FAC_Ct6)
                            {
                                ram_para[num_fac_Ct6] = gss_factory_parm_value[gsc_disp_parm_type];                                                                    //!!!!!!!
                                eeprom_write(num_fac_Ct6, ram_para[num_fac_Ct6]);
                            }    
                            else if(gsc_disp_parm_type == DISP_FAC_Ct7)
                            {
                                ram_para[num_fac_Ct7] = gss_factory_parm_value[gsc_disp_parm_type];                                                                    //!!!!!!!
                                eeprom_write(num_fac_Ct7, ram_para[num_fac_Ct7]);
                            }  
                            else if(gsc_disp_parm_type == DISP_FAC_CU)
                            {
                                ram_para[num_fac_CU] = gss_factory_parm_value[gsc_disp_parm_type];                                                                    //!!!!!!!
                                eeprom_write(num_fac_CU, ram_para[num_fac_CU]);
                            }    
                            else if(gsc_disp_parm_type == DISP_FAC_Ad)
                            {
                                ram_para[num_fac_Ad] = gss_factory_parm_value[gsc_disp_parm_type];                                                                    //!!!!!!!
                                eeprom_write(num_fac_Ad, ram_para[num_fac_Ad]);
                            }
                            else if(gsc_disp_parm_type == DISP_FAC_AAH)
                            {
                                ram_para[num_fac_AAH] = gss_factory_parm_value[gsc_disp_parm_type];                                                                    //!!!!!!!
                                eeprom_write(num_fac_AAH, ram_para[num_fac_AAH]);
                            }    
                            else if(gsc_disp_parm_type == DISP_FAC_AAL)
                            {
                                ram_para[num_fac_AAL] = gss_factory_parm_value[gsc_disp_parm_type];                                                                    //!!!!!!!
                                eeprom_write(num_fac_AAL, ram_para[num_fac_AAL]);
                            }  
                            else if(gsc_disp_parm_type == DISP_FAC_r1)
                            {
                                ram_para[num_fac_r1] = gss_factory_parm_value[gsc_disp_parm_type];                                                                    //!!!!!!!
                                eeprom_write(num_fac_r1, ram_para[num_fac_r1]);
                            }    
                            else if(gsc_disp_parm_type == DISP_FAC_AHH)
                            {
                                ram_para[num_fac_AHH] = gss_factory_parm_value[gsc_disp_parm_type];                                                                    //!!!!!!!
                                eeprom_write(num_fac_AHH, ram_para[num_fac_AHH]);
                            }
                            else if(gsc_disp_parm_type == DISP_FAC_T3)
                            {
                                ram_para[num_fac_T3] = gss_factory_parm_value[gsc_disp_parm_type];                                                                    //!!!!!!!
                                eeprom_write(num_fac_T3, ram_para[num_fac_T3]);
                            }    
                            else if(gsc_disp_parm_type == DISP_FAC_Cd)
                            {
                                ram_para[num_fac_Cd] = gss_factory_parm_value[gsc_disp_parm_type];                                                                    //!!!!!!!
                                eeprom_write(num_fac_Cd, ram_para[num_fac_Cd]);
                            }  
                            else if(gsc_disp_parm_type == DISP_FAC_Fd)
                            {
                                ram_para[num_fac_Fd] = gss_factory_parm_value[gsc_disp_parm_type];                                                                    //!!!!!!!
                                eeprom_write(num_fac_Fd, ram_para[num_fac_Fd]);
                            }    
                            else if(gsc_disp_parm_type == DISP_FAC_LC)
                            {
                                ram_para[num_fac_LC] = gss_factory_parm_value[gsc_disp_parm_type];                                                                    //!!!!!!!
                                eeprom_write(num_fac_LC, ram_para[num_fac_LC]);
                            }
                            else if(gsc_disp_parm_type == DISP_FAC_CL3)
                            {
                                //NO SAVE
                            }    
                            else if(gsc_disp_parm_type == DISP_FAC_CL4)
                            {
                                //NO SAVE
                            }  
                            else if(gsc_disp_parm_type == DISP_FAC_dFL)
                            {
                                ram_para[num_fac_dFL] = gss_factory_parm_value[gsc_disp_parm_type];                                                                    //!!!!!!!
                                eeprom_write(num_fac_dFL, ram_para[num_fac_dFL]);
                            }    
                            else if(gsc_disp_parm_type == DISP_FAC_dFH)
                            {
                                ram_para[num_fac_dFH] = gss_factory_parm_value[gsc_disp_parm_type];                                                                    //!!!!!!!
                                eeprom_write(num_fac_dFH, ram_para[num_fac_dFH]);
                            }
                            else if(gsc_disp_parm_type == DISP_FAC_HT1)
                            {
                                ram_para[num_fac_HT1] = gss_factory_parm_value[gsc_disp_parm_type];                                                                    //!!!!!!!
                                eeprom_write(num_fac_HT1, ram_para[num_fac_HT1]);
                            }    
                            else if(gsc_disp_parm_type == DISP_FAC_HT2)
                            {
                                ram_para[num_fac_HT2] = gss_factory_parm_value[gsc_disp_parm_type];                                                                    //!!!!!!!
                                eeprom_write(num_fac_HT2, ram_para[num_fac_HT2]);
                            }  
                            else if(gsc_disp_parm_type == DISP_FAC_PS)
                            {
                                ram_para[num_fac_PS] = gss_factory_parm_value[gsc_disp_parm_type];                                                                    //!!!!!!!
                                eeprom_write(num_fac_PS, ram_para[num_fac_PS]);
                            }    
                            else if(gsc_disp_parm_type == DISP_FAC_PCH)
                            {
                                ram_para[num_fac_PCH] = gss_factory_parm_value[gsc_disp_parm_type];                                                                    //!!!!!!!
                                eeprom_write(num_fac_PCH, ram_para[num_fac_PCH]);
                            }                        
                        }
                    }
                }
            }
            else if(guc_key_val == KEY_UP)
            {
                if(bflg_parm_save == 0)
                {
                    bflg_parm_save = 1;
                    gss_key_temp_value = gss_factory_parm_value[gsc_disp_parm_type];
                }
                gss_key_temp_value++;         

                if(gsc_disp_parm_type == DISP_FAC_CL2)
                {
                    if(gss_key_temp_value > 1)    
                    {
                        gss_key_temp_value = 0;
                    }
                }
                else if(gsc_disp_parm_type == DISP_FAC_UL)
                {
                    if(gss_key_temp_value > 1)    
                    {
                        gss_key_temp_value = 0;
                    }
                }                
                else if(gsc_disp_parm_type == DISP_FAC_r0)
                {
                    if(gss_key_temp_value > 50)    //magnify 10
                    {
                        gss_key_temp_value = 3;
                    }
                }
                else if(gsc_disp_parm_type == DISP_FAC_b0F)
                {
                    if(gss_key_temp_value > 10)    
                    {
                        gss_key_temp_value = 0;
                    }
                }
                else if(gsc_disp_parm_type == DISP_FAC_br1)
                {
                    if(gss_key_temp_value > 200)    
                    {
                        gss_key_temp_value = 120;
                    }
                }
                else if(gsc_disp_parm_type == DISP_FAC_br2)
                {
                    if(gss_key_temp_value > 200)    
                    {
                        gss_key_temp_value = 120;
                    }
                }
                else if(gsc_disp_parm_type == DISP_FAC_br3)
                {
                    if(gss_key_temp_value > 350)    
                    {
                        gss_key_temp_value = 120;
                    }
                }
                else if(gsc_disp_parm_type == DISP_FAC_br4)
                {
                    if(gss_key_temp_value > 430)    
                    {
                        gss_key_temp_value = 120;
                    }
                }                    
                else if(gsc_disp_parm_type == DISP_FAC_Ct1)
                {
                    if(gss_key_temp_value > 50)    //magnify 10
                    {
                        gss_key_temp_value = -50;
                    }
                }
                else if(gsc_disp_parm_type == DISP_FAC_Ct2)
                {
                    if(gss_key_temp_value > 50)     //magnify 10
                    {
                        gss_key_temp_value = -50;
                    }
                }
                else if(gsc_disp_parm_type == DISP_FAC_Ct3)
                {
                    if(gss_key_temp_value > 50)     //magnify 10
                    {
                        gss_key_temp_value = -50;
                    }
                }
                else if(gsc_disp_parm_type == DISP_FAC_Ct4)
                {
                    if(gss_key_temp_value > 50)     //magnify 10
                    {
                        gss_key_temp_value = -50;
                    }
                }
                else if(gsc_disp_parm_type == DISP_FAC_Ct5)
                {
                    if(gss_key_temp_value > 50)     //magnify 10
                    {
                        gss_key_temp_value = -50;
                    }
                }  
                else if(gsc_disp_parm_type == DISP_FAC_Ct6)
                {
                    if(gss_key_temp_value > 50)     //magnify 10
                    {
                        gss_key_temp_value = -50;
                    }
                }   
                else if(gsc_disp_parm_type == DISP_FAC_Ct7)
                {
                    if(gss_key_temp_value > 100)     
                    {
                        gss_key_temp_value = 0;
                    }
                } 
                else if(gsc_disp_parm_type == DISP_FAC_CU)
                {
                    if(gss_key_temp_value > 9)     
                    {
                        gss_key_temp_value = -9;
                    }
                } 
                else if(gsc_disp_parm_type == DISP_FAC_Ad)
                {
                    if(gss_key_temp_value > 99)     
                    {
                        gss_key_temp_value = -1;
                    }
                } 
                else if(gsc_disp_parm_type == DISP_FAC_AAH)
                {
                    if(gss_key_temp_value > 100)     
                    {
                        gss_key_temp_value = gss_factory_parm_value[DISP_FAC_AAL];
                    }
                } 
                else if(gsc_disp_parm_type == DISP_FAC_AAL)
                {
                    if(gss_key_temp_value > gss_factory_parm_value[DISP_FAC_AAH])     
                    {
                        gss_key_temp_value = -5;
                    }
                } 
                else if(gsc_disp_parm_type == DISP_FAC_r1)
                {
                    if(gss_key_temp_value > 30)     
                    {
                        gss_key_temp_value = 5;
                    }
                } 
                else if(gsc_disp_parm_type == DISP_FAC_AHH)
                {
                    if(gss_key_temp_value > 60)     
                    {
                        gss_key_temp_value = 30;
                    }
                } 
                else if(gsc_disp_parm_type == DISP_FAC_T3)
                {
                    if(gss_key_temp_value > 12)     
                    {
                        gss_key_temp_value = 0;
                    }
                } 
                else if(gsc_disp_parm_type == DISP_FAC_Cd)
                {
                    if(gss_key_temp_value > 15)     
                    {
                        gss_key_temp_value = 0;
                    }
                } 
                else if(gsc_disp_parm_type == DISP_FAC_Fd)
                {
                    if(gss_key_temp_value > 3)     
                    {
                        gss_key_temp_value = 1;
                    }
                } 
                else if(gsc_disp_parm_type == DISP_FAC_LC)
                {
                    if(gss_key_temp_value > 1)     
                    {
                        gss_key_temp_value = 0;
                    }
                } 
                else if(gsc_disp_parm_type == DISP_FAC_CL3)
                {
                    if(gss_key_temp_value > 1)     
                    {
                        gss_key_temp_value = 0;
                    }
                } 
                else if(gsc_disp_parm_type == DISP_FAC_CL4)
                {
                    if(gss_key_temp_value > 1)     
                    {
                        gss_key_temp_value = 0;
                    }
                } 
                else if(gsc_disp_parm_type == DISP_FAC_dFL)
                {
                    if(gss_key_temp_value > 0)     
                    {
                        gss_key_temp_value = -20;
                    }
                } 
                else if(gsc_disp_parm_type == DISP_FAC_dFH)
                {
                    if(gss_key_temp_value > 10)     
                    {
                        gss_key_temp_value = 0;
                    }
                } 
                else if(gsc_disp_parm_type == DISP_FAC_HT1)
                {
                    if(gss_key_temp_value > 100)     
                    {
                        gss_key_temp_value = 40;
                    }
                } 
                else if(gsc_disp_parm_type == DISP_FAC_HT2)
                {
                    if(gss_key_temp_value > (gss_factory_parm_value[DISP_FAC_HT1] -10))   
                    {
                        gss_key_temp_value = 0;
                    }
                } 
                else if(gsc_disp_parm_type == DISP_FAC_PS)
                {
                    if(gss_key_temp_value > 99)     
                    {
                        gss_key_temp_value = 0;
                    }
                } 
                else if(gsc_disp_parm_type == DISP_FAC_PCH)
                {
                    if(gss_key_temp_value > 1)     
                    {
                        gss_key_temp_value = 0;
                    }
                }                     
            }
            else if(guc_key_val == KEY_DOWN)
            {
                if(bflg_parm_save == 0)
                {
                    bflg_parm_save = 1;
                    gss_key_temp_value = gss_factory_parm_value[gsc_disp_parm_type];
                }
                gss_key_temp_value--;      //��Ҫ�ĳ����ģʽ   

                if(gsc_disp_parm_type == DISP_FAC_CL2)
                {
                    if(gss_key_temp_value < 0)    
                    {
                        gss_key_temp_value = 1;
                    }
                }
                else if(gsc_disp_parm_type == DISP_FAC_UL)
                {
                    if(gss_key_temp_value < 0)    
                    {
                        gss_key_temp_value = 1;
                    }
                }                
                else if(gsc_disp_parm_type == DISP_FAC_r0)
                {
                    if(gss_key_temp_value < 3)    
                    {
                        gss_key_temp_value = 50;
                    }
                }
                else if(gsc_disp_parm_type == DISP_FAC_b0F)
                {
                    if(gss_key_temp_value < 0)    
                    {
                        gss_key_temp_value = 10;
                    }
                }
                else if(gsc_disp_parm_type == DISP_FAC_br1)
                {
                    if(gss_key_temp_value < 120)    
                    {
                        gss_key_temp_value = 200;
                    }
                }
                else if(gsc_disp_parm_type == DISP_FAC_br2)
                {
                    if(gss_key_temp_value < 120)    
                    {
                        gss_key_temp_value = 200;
                    }
                }
                else if(gsc_disp_parm_type == DISP_FAC_br3)
                {
                    if(gss_key_temp_value < 120)    
                    {
                        gss_key_temp_value = 350;
                    }
                }
                else if(gsc_disp_parm_type == DISP_FAC_br4)
                {
                    if(gss_key_temp_value < 120)    
                    {
                        gss_key_temp_value = 430;
                    }
                }
                else if(gsc_disp_parm_type == DISP_FAC_Ct1)
                {
                    if(gss_key_temp_value < -50)    
                    {
                        gss_key_temp_value = 50;
                    }
                }
                else if(gsc_disp_parm_type == DISP_FAC_Ct2)
                {
                    if(gss_key_temp_value < -50)    
                    {
                        gss_key_temp_value = 50;
                    }
                }
                else if(gsc_disp_parm_type == DISP_FAC_Ct3)
                {
                    if(gss_key_temp_value < -50)    
                    {
                        gss_key_temp_value = 50;
                    }
                }
                else if(gsc_disp_parm_type == DISP_FAC_Ct4)
                {
                    if(gss_key_temp_value < -50)    
                    {
                        gss_key_temp_value = 50;
                    }
                }
                else if(gsc_disp_parm_type == DISP_FAC_Ct5)
                {
                    if(gss_key_temp_value < -50)    
                    {
                        gss_key_temp_value = 50;
                    }
                }  
                else if(gsc_disp_parm_type == DISP_FAC_Ct6)
                {
                    if(gss_key_temp_value < -50)    
                    {
                        gss_key_temp_value = 50;
                    }
                }   
                else if(gsc_disp_parm_type == DISP_FAC_Ct7)
                {
                    if(gss_key_temp_value < 0)     
                    {
                        gss_key_temp_value = 100;
                    }
                } 
                else if(gsc_disp_parm_type == DISP_FAC_CU)
                {
                    if(gss_key_temp_value < -9)     
                    {
                        gss_key_temp_value = 9;
                    }
                } 
                else if(gsc_disp_parm_type == DISP_FAC_Ad)
                {
                    if(gss_key_temp_value < -1)     
                    {
                        gss_key_temp_value = 99;
                    }
                } 
                else if(gsc_disp_parm_type == DISP_FAC_AAH)
                {
                    if(gss_key_temp_value < gss_factory_parm_value[DISP_FAC_AAL])     
                    {
                        gss_key_temp_value = 100;
                    }
                } 
                else if(gsc_disp_parm_type == DISP_FAC_AAL)
                {
                    if(gss_key_temp_value < -5)     
                    {
                        gss_key_temp_value =  gss_factory_parm_value[DISP_FAC_AAH];
                    }
                } 
                else if(gsc_disp_parm_type == DISP_FAC_r1)
                {
                    if(gss_key_temp_value < 5)     
                    {
                        gss_key_temp_value = 30;
                    }
                } 
                else if(gsc_disp_parm_type == DISP_FAC_AHH)
                {
                    if(gss_key_temp_value < 30)     
                    {
                        gss_key_temp_value = 60;
                    }
                } 
                else if(gsc_disp_parm_type == DISP_FAC_T3)
                {
                    if(gss_key_temp_value < 0)     
                    {
                        gss_key_temp_value = 12;
                    }
                } 
                else if(gsc_disp_parm_type == DISP_FAC_Cd)
                {
                    if(gss_key_temp_value < 0)     
                    {
                        gss_key_temp_value = 15;
                    }
                } 
                else if(gsc_disp_parm_type == DISP_FAC_Fd)
                {
                    if(gss_key_temp_value < 1)     
                    {
                        gss_key_temp_value = 3;
                    }
                } 
                else if(gsc_disp_parm_type == DISP_FAC_LC)
                {
                    if(gss_key_temp_value < 0)     
                    {
                        gss_key_temp_value = 1;
                    }
                } 
                else if(gsc_disp_parm_type == DISP_FAC_CL3)
                {
                    if(gss_key_temp_value < 0)     
                    {
                        gss_key_temp_value = 1;
                    }
                } 
                else if(gsc_disp_parm_type == DISP_FAC_CL4)
                {
                    if(gss_key_temp_value < 0)     
                    {
                        gss_key_temp_value = 1;
                    }
                } 
                else if(gsc_disp_parm_type == DISP_FAC_dFL)
                {
                    if(gss_key_temp_value < -20)     
                    {
                        gss_key_temp_value = 0;
                    }
                } 
                else if(gsc_disp_parm_type == DISP_FAC_dFH)
                {
                    if(gss_key_temp_value < 0)     
                    {
                        gss_key_temp_value = 10;
                    }
                } 
                else if(gsc_disp_parm_type == DISP_FAC_HT1)
                {
                    if(gss_key_temp_value < 40)     
                    {
                        gss_key_temp_value = 100;
                    }
                } 
                else if(gsc_disp_parm_type == DISP_FAC_HT2)
                {
                    if(gss_key_temp_value < 0)     
                    {
                        gss_key_temp_value = (gss_factory_parm_value[DISP_FAC_HT1] -10);
                    }
                } 
                else if(gsc_disp_parm_type == DISP_FAC_PS)
                {
                    if(gss_key_temp_value < 0)     
                    {
                        gss_key_temp_value = 99;
                    }
                } 
                else if(gsc_disp_parm_type == DISP_FAC_PCH)
                {
                    if(gss_key_temp_value < 0)     
                    {
                        gss_key_temp_value = 1;
                    }
                }                     
            }
        }
    }
}

/******************************************************************************************************************************
��������:  ���û�����������RTCʱ��ʱ��Ĳ���:P1-P5

����λ��: read_key_deal() ����ѭ��------------------------ok
******************************************************************************************************************************/
void key_RTC_time_parm_deal(void)  
{
    if (guc_key_val == KEY_BUZ)    //��USER�е�T2����������
    {        
        key_enter_mode_init(DISP_USER_SET,DISP_USER_SET_T2,1);
        //gss_user_parm_value[DISP_USER_T2]= 0;//��Ҫ��P1��ʼ
    }
    else if (guc_key_val == KEY_UP)             
    {
        guc_user_write_time_value++;
        
        if(gss_user_set_parm_value[DISP_USER_SET_T2] == 0)           //P1_��
        {
            if(guc_user_write_time_value > 99)
            {
                guc_user_write_time_value = 10;
            }
        }
        else if(gss_user_set_parm_value[DISP_USER_SET_T2] == 1)      //P2_��
        {
            if(guc_user_write_time_value > 12)
            {
                guc_user_write_time_value = 1;
            }
        }
        else if(gss_user_set_parm_value[DISP_USER_SET_T2] == 2)      //P3_��
        {
            if(guc_user_write_time_value > 31)
            {
                guc_user_write_time_value = 1;
            }
        }
        else if(gss_user_set_parm_value[DISP_USER_SET_T2] == 3)      //P4_ʱ
        {
            if(guc_user_write_time_value > 23)
            {
                guc_user_write_time_value = 0;
            }
        }
        else if(gss_user_set_parm_value[DISP_USER_SET_T2] == 4)      //P5_��
        {
            if(guc_user_write_time_value > 59)
            {
                guc_user_write_time_value = 0;
            }
        }
    }
    else if(guc_key_val == KEY_DOWN)
    {
        guc_user_write_time_value--;
        if(gss_user_set_parm_value[DISP_USER_SET_T2] == 0)           //P1_��
        {
            if(guc_user_write_time_value < 10)
            {
                guc_user_write_time_value = 99;
            }
        }
        else if(gss_user_set_parm_value[DISP_USER_SET_T2] == 1)      //P2_��
        {
            if(guc_user_write_time_value < 1)
            {
                guc_user_write_time_value = 12;
            }
        }
        else if(gss_user_set_parm_value[DISP_USER_SET_T2] == 2)      //P3_��
        {
            if(guc_user_write_time_value < 1)
            {
                guc_user_write_time_value = 31;
            }
        }
        else if(gss_user_set_parm_value[DISP_USER_SET_T2] == 3)      //P4_ʱ
        {
            if(guc_user_write_time_value < 0)
            {
                guc_user_write_time_value = 23;
            }
        }
        else if(gss_user_set_parm_value[DISP_USER_SET_T2] == 4)      //P5_��
        {
            if(guc_user_write_time_value < 0)
            {
                guc_user_write_time_value = 59;
            }
        }
    }
    else if(guc_key_val == KEY_SET)  //ȷ���޸�ʱ��
    {
        guc_disp_mode = DISP_USER_SET;
        gsc_disp_parm_type = DISP_USER_SET_T2;
        //gss_key_temp_value = 0;
        bflg_parm_dis_form = 1;
        bflg_parm_save = 0;
            
        if(gss_user_set_parm_value[DISP_USER_SET_T2] == 0)      //P1��
        {
            if(guc_user_write_time_value != guc_RTC_r_year)    
            {
                guc_RTC_ADDR   =  CMD_W_Y;
                guc_RTC_WDATA = func_timer_4302_deal(guc_user_write_time_value);  
                bflg_RTC_enable_write = 1;
                
                bflg_usb_set_time_delaytime = 1;
                gss_usb_set_time_delaytime = 0;
            }
        }
        else if(gss_user_set_parm_value[DISP_USER_SET_T2] == 1)      //P1��
        {
            if(guc_user_write_time_value != guc_RTC_r_month)    
            {
                guc_RTC_ADDR   =  CMD_W_MO;
                guc_RTC_WDATA = func_timer_4302_deal(guc_user_write_time_value);  
                bflg_RTC_enable_write = 1;
                
                bflg_usb_set_time_delaytime = 1;
                gss_usb_set_time_delaytime = 0;
            }
        }
        else if(gss_user_set_parm_value[DISP_USER_SET_T2] == 2)      //P1��
        {
            if(guc_user_write_time_value != guc_RTC_r_day)    
            {
                guc_RTC_ADDR   =  CMD_W_D;
                guc_RTC_WDATA = func_timer_4302_deal(guc_user_write_time_value);  
                bflg_RTC_enable_write = 1;
                
                bflg_usb_set_time_delaytime = 1;
                gss_usb_set_time_delaytime = 0;
            }
        }
        else if(gss_user_set_parm_value[DISP_USER_SET_T2] == 3)      //P1ʱ
        {
            if(guc_user_write_time_value != guc_RTC_r_hour)    
            {
                guc_RTC_ADDR   =  CMD_W_H;
                guc_RTC_WDATA = func_timer_4302_deal(guc_user_write_time_value);  
                bflg_RTC_enable_write = 1;
                
                bflg_usb_set_time_delaytime = 1;
                gss_usb_set_time_delaytime = 0;
            }
        }
        else if(gss_user_set_parm_value[DISP_USER_SET_T2] == 4)      //P1��
        {
            if(guc_user_write_time_value != guc_RTC_r_minute) 
            {
                guc_RTC_ADDR   =  CMD_W_M;
                guc_RTC_WDATA = func_timer_4302_deal(guc_user_write_time_value);  
                bflg_RTC_enable_write = 1;
                
                bflg_usb_set_time_delaytime = 1;
                gss_usb_set_time_delaytime = 0;
            }
        }
    }
}

/******************************************************************************************************************************
��������:  �޸�RTCʱ�Ӻ�����ʱ2s�ٷ����޸�usbʱ�䣬��ֹ���廹δ��ȡ���޸ĺ��ʱ��

����λ��:  1s��ʱ��---------------------------------------------------ok
******************************************************************************************************************************/
void key_RTC_time_parm_sent_usb_delaytime(void) 
{
    if(bflg_usb_set_time_delaytime == 1)
    {   
        gss_usb_set_time_delaytime++;
        if(gss_usb_set_time_delaytime >= 2)
        {
            gss_usb_set_time_delaytime = 0;
            bflg_usb_set_time_delaytime = 0;
            
            bflg_usb_set_time = 1;
        }
    }
}
/**********************************************************************************************************************************************
��������: ����ģʽ�µİ�������


����λ��: mode_selection(����ѭ��)----------------------------------------------ok
***********************************************************************************************************************************************/
void key_inquire_deal(void)
{
    if(guc_key_val == KEY_BUZ)
    {
        guc_disp_mode = DISP_LOCKED;
        bflg_key_inq_val = 0;
    }
    else
    {
        if(bflg_key_inq_val == 0)
        {
            if(guc_key_val == KEY_SET)
            {
                bflg_key_inq_val = 1;
            }
            else
            {
                if(guc_key_val == KEY_UP)
                {
                    gsc_disp_inq_mode++;
                    if(gsc_disp_inq_mode > DISP_INQ_d9)
                    {
                        gsc_disp_inq_mode = DISP_INQ_d1;
                    }
                }
                else if(guc_key_val == KEY_DOWN)
                {
                    gsc_disp_inq_mode--;
                    if(gsc_disp_inq_mode < DISP_INQ_d1)

                    {
                        gsc_disp_inq_mode = DISP_INQ_d9;
                    }
                }
            }
        }
        else
        {
            if(guc_key_val == KEY_SET)
            {
                bflg_key_inq_val = 0;
            }
        }
    }
}

/**********************************************************************************************************************************************
��������:IC����ģʽ�µİ�������


����λ��: mode_selection(����ѭ��)----------------------------------------------ok
***********************************************************************************************************************************************/
void key_ic_password_deal(void)
{
    if (guc_key_val == KEY_BUZ)
    {
        if(gsc_disp_parm_type == DISP_USER_MANAGE_CL1)     //��manage���CL1������
        {           
            guc_disp_mode = DISP_USER_MANAGE;    
            gss_key_temp_value = 0;
            bflg_parm_dis_form = 0;
            bflg_parm_save = 0;
        }
        else
        {
            guc_disp_mode = DISP_LOCKED;   
            
            bflg_IC_register_state = 0;                    //��� ����֤
        }
    }
    else if (guc_key_val == KEY_UP)
    {  
        if (bflg_key_password_long_valid == 0)
        {
            if(bflg_key_password_long_valid_delaytime == 0)
            {
                bflg_key_password_long_valid_delaytime = 1;
                gss_key_password_long_valid_delaytime = 0;
            }
            gss_IC_password_val++;
            if (gss_IC_password_val > 999)
            {
                gss_IC_password_val = 0;
            }             
        }
        else
        {
            gss_IC_password_val += 10;
            if (gss_IC_password_val > 999)
            {
                gss_IC_password_val = 0;
            } 
        }    
    }
    else if (guc_key_val == KEY_DOWN)
    {       
        if (bflg_key_password_long_valid == 0)
        {
            if(bflg_key_password_long_valid_delaytime == 0)
            {
                bflg_key_password_long_valid_delaytime = 1;
                gss_key_password_long_valid_delaytime = 0;
            }
            gss_IC_password_val--;
            if (gss_IC_password_val < 0)
            {
                gss_IC_password_val = 999;
            }             
        }
        else
        {
            gss_IC_password_val -= 10;
            if (gss_IC_password_val < 0)
            {
                gss_IC_password_val = 999;
            } 
        }     
    }    
    else if (guc_key_val == KEY_SET)
    {       
        key_clear_password_long_falg();   
        
        guc_IC_password_cnt++;

        if(guc_IC_password_cnt <= 3)
        {
            if (gss_IC_password_val == gss_user_manage_parm_value[DISP_USER_MANAGE_IC])      // ����������Ƿ�����õ�һ��
            {        
                guc_IC_password_cnt = 0;
                
                if(gsc_disp_parm_type == DISP_USER_MANAGE_CL1) //�����user2��� ICע������
                {
                    bflg_IC_erasure_order = 1;                 //ע������IC��
                    
                    bflg_IC_pass_CL1 = 1;                      //CL1��˸��ʾ�ı�־--��˸���ڻص�manage��

                    bflg_sent_usb_IC = 1;                      //usb�¼�
                }
                else
                {              
                    key_enter_mode_init(DISP_IC_REGISTER,0,0);
                }
            }
            else if(guc_IC_password_cnt >= 3)
            {
                bflg_IC_register_state = 0;           //��ɽ���IC����״̬
                gss_IC_password_val = 0;
                guc_IC_password_cnt = 0;
                guc_disp_mode = DISP_LOCKED;
                //gsc_disp_user1_mode = DISP_SET_TS;    
                //gsc_disp_user2_mode = DISP_USER_dA; 
            }
        }
    }
    else
    {
        key_clear_password_long_falg();         //��������ʱ��������10
    }
}

/**********************************************************************************************************************************************
��������:ICע��ģʽ�µİ�������


����λ��: mode_selection(����ѭ��)----------------------------------------------ok
***********************************************************************************************************************************************/
void key_ic_register_deal(void)
{
    if(guc_key_val == KEY_BUZ)                
    {
        bflg_IC_register_state = 0;         // �˳� ICע��״̬
        guc_temp_flash_add = 0;
        guc_flash_add = 0;

        guc_disp_mode = DISP_LOCKED;           
    }     
}


/**********************************************************************************************************************************************
��������: �û��趨�������û��������������������Ĭ��ֵ���Ժ�Ҫ��E2��ȡ��


����λ��: ��ʼ����---------------------------------------------OK
***********************************************************************************************************************************************/
void operation_parm_read(void)
{   
    //�û��趨����
    gss_user_set_parm_value[DISP_USER_SET_TS]  = ram_para[num_user_set_TS];  //magnify �õ�ʱ����*10��
    gss_user_set_parm_value[DISP_USER_SET_ALH] = ram_para[num_user_set_ALH]; //magnify
    gss_user_set_parm_value[DISP_USER_SET_ALL] = ram_para[num_user_set_ALL]; //magnify
    gss_user_set_parm_value[DISP_USER_SET_T1]  = ram_para[num_user_set_T1];
    //gss_user_set_parm_value[DISP_USER_SET_T2]  = ram_para[num_user_set_T2];
    gss_user_set_parm_value[DISP_USER_SET_P6]  = ram_para[num_user_set_P6];
    //gss_user_set_parm_value[DISP_USER_SET_SS]  = ram_para[num_user_set_SS];

    //�û��������
    gss_user_manage_parm_value[DISP_USER_MANAGE_IC]  = ram_para[num_user_manage_IC];    
    //gss_user_manage_parm_value[DISP_USER_MANAGE_CL1] = ram_para[num_user_manage_CL1];
    gss_user_manage_parm_value[DISP_USER_MANAGE_Lt]  = ram_para[num_user_manage_Lt];   
    gss_user_manage_parm_value[DISP_USER_MANAGE_dA]  = ram_para[num_user_manage_dA];  
    gss_user_manage_parm_value[DISP_USER_MANAGE_FA]  = ram_para[num_user_manage_FA];   
    gss_user_manage_parm_value[DISP_USER_MANAGE_S1]  = ram_para[num_user_manage_S1];
    gss_user_manage_parm_value[DISP_USER_MANAGE_S2]  = ram_para[num_user_manage_S2];  
    gss_user_manage_parm_value[DISP_USER_MANAGE_TT]  = ram_para[num_user_manage_TT];

    //��������
    //gss_factory_parm_value[DISP_FAC_CL2] = ram_para[num_fac_CL2];    
    gss_factory_parm_value[DISP_FAC_UL]  = ram_para[num_fac_UL];    
    gss_factory_parm_value[DISP_FAC_r0]  = ram_para[num_fac_r0];    //magnify     
    gss_factory_parm_value[DISP_FAC_b0F] = ram_para[num_fac_b0F];   //use need magnify 10
    gss_factory_parm_value[DISP_FAC_br1] = ram_para[num_fac_br1]; 
    gss_factory_parm_value[DISP_FAC_br2] = ram_para[num_fac_br2];
    gss_factory_parm_value[DISP_FAC_br3] = ram_para[num_fac_br3];
    gss_factory_parm_value[DISP_FAC_br4] = ram_para[num_fac_br4];
    gss_factory_parm_value[DISP_FAC_Ct1] = ram_para[num_fac_Ct1];    //magnify
    gss_factory_parm_value[DISP_FAC_Ct2] = ram_para[num_fac_Ct2];    //magnify
    gss_factory_parm_value[DISP_FAC_Ct3] = ram_para[num_fac_Ct3];    //magnify
    gss_factory_parm_value[DISP_FAC_Ct4] = ram_para[num_fac_Ct4];    //magnify
    gss_factory_parm_value[DISP_FAC_Ct5] = ram_para[num_fac_Ct5];    //magnify
    gss_factory_parm_value[DISP_FAC_Ct6] = ram_para[num_fac_Ct6];    //magnify
    gss_factory_parm_value[DISP_FAC_Ct7] = ram_para[num_fac_Ct7];
    gss_factory_parm_value[DISP_FAC_CU]  = ram_para[num_fac_CU];
    gss_factory_parm_value[DISP_FAC_Ad]  = ram_para[num_fac_Ad];
    gss_factory_parm_value[DISP_FAC_AAH] = ram_para[num_fac_AAH];   //use need magnify 10��������ܲ�����ʾ�ͷ��㰴��++����ʹ����Щ����ʱ��Ҫ�Ŵ�10����Ϊ�¶ȶ��Ŵ���10��
    gss_factory_parm_value[DISP_FAC_AAL] = ram_para[num_fac_AAL];   //use need magnify 10
    gss_factory_parm_value[DISP_FAC_r1]  = ram_para[num_fac_r1];    //use need magnify 10
    gss_factory_parm_value[DISP_FAC_AHH] = ram_para[num_fac_AHH];   //use need magnify 10
    gss_factory_parm_value[DISP_FAC_T3]  = ram_para[num_fac_T3];
    gss_factory_parm_value[DISP_FAC_Cd]  = ram_para[num_fac_Cd];
    gss_factory_parm_value[DISP_FAC_Fd]  = ram_para[num_fac_Fd];    //ʹ����*10����ֵ���������������
    gss_factory_parm_value[DISP_FAC_LC]  = ram_para[num_fac_LC];
    //gss_factory_parm_value[DISP_FAC_CL3] = ram_para[num_fac_CL3];
    //gss_factory_parm_value[DISP_FAC_CL4] = ram_para[num_fac_CL4];
    gss_factory_parm_value[DISP_FAC_dFL] = ram_para[num_fac_dFL];   //use need magnify 10
    gss_factory_parm_value[DISP_FAC_dFH] = ram_para[num_fac_dFH];   //use need magnify 10
    gss_factory_parm_value[DISP_FAC_HT1] = ram_para[num_fac_HT1];
    gss_factory_parm_value[DISP_FAC_HT2] = ram_para[num_fac_HT2];
    gss_factory_parm_value[DISP_FAC_PS]  = ram_para[num_fac_PS];
    gss_factory_parm_value[DISP_FAC_PCH] = ram_para[num_fac_PCH];
}


/********************************************************************************************************************
��������: ��IC��ע��״̬ʱ�����п����Ķ���

����λ��: ��������   ---------------------------ok
*********************************************************************************************************************/
void IC_unlock_deal(void)
{
    if(guc_disp_mode != DISP_IC_REGISTER)   //��IC��ע��״̬ʱ�����п����Ķ���
    {
        if(guc_IC_access_state == 1)
        {
            LOCK_OUT_PIN = 1;          //����
            guc_IC_access_state = 0;

            bflg_lock_delaytimer = 1;  //��������ʱ��־
            guc_lock_delaytimer = 0;
        }
    }
}

/********************************************************************************************************************
��������: IC��������ʱ

����λ��: 1s ��ʱ ------------------------------ok
*********************************************************************************************************************/
void IC_unlock_delaytime(void)  
{
    if(bflg_lock_delaytimer == 1)
    {
        guc_lock_delaytimer++;
        if(guc_lock_delaytimer >= 10)    // ����10s
        {
            bflg_lock_delaytimer = 0;
            guc_lock_delaytimer = 0;

            LOCK_OUT_PIN = 0;
        }
    }
}
/********************************************************************************************************************
��������: ICע��(IC)����������(PS1)����������(PS2) ������ʱ,Ȼ�󰴲���10����

          ÿ�����ǲ�ʱ��Ҫ��0��

����λ��: 1s ��ʱ -----------------------------------ok
*********************************************************************************************************************/
void key_password_long_valid_delaytime(void)
{
    if (bflg_key_password_long_valid_delaytime == 1)
    {
        gss_key_password_long_valid_delaytime++;
        if (gss_key_password_long_valid_delaytime >= 5)  
        {
            gss_key_password_long_valid_delaytime = 0;
            bflg_key_password_long_valid_delaytime = 0;
            
            bflg_key_password_long_valid = 1;   
        }
    }
}

/********************************************************************************************************************
��������: �����볤��ʱ�ı�־:1.��������ʱ 2.�޸�����ʱ ������ͬ������


����λ�ã����ļ��еĸ���������------------------------------------------ok
*********************************************************************************************************************/
void key_clear_password_long_falg(void)
{
    bflg_key_password_long_valid = 0;
    bflg_key_password_long_valid_delaytime = 0;
    gss_key_password_long_valid_delaytime = 0;  
    
}
/********************************************************************************************************************
��������:����ÿ����ͬģʽʱ�����ֱ�־��������0��ֻ�������Ĳ�ı��
         ֻ�ڽ��ս��뺯��ʱʹ�ã�
  
����λ�ã����ļ��еĸ���������------------------------------------------no
*********************************************************************************************************************/
void key_enter_mode_init(uint8_t mode_num,uint8_t parm_num,uint8_t dis_form)
{
    guc_disp_mode = mode_num;           //�ܵ�ģʽ�������
    gsc_disp_parm_type = parm_num;      //ÿһ��ģʽ�в�ͬ�Ĳ������,һ�㶼��0��ֻ�����ⲻ��
    gss_key_temp_value = 0;             //��ʱֵ
    bflg_parm_save = 0;                 //0δ�޸�      1�޸Ļ�δ����
    bflg_parm_dis_form = dis_form;      //0��ʾ��ʾ 1��ʾ��ֵ,һ����0�����ⲻ�� 
}
/*******************************************END OF THE FILE*************************************************************/
