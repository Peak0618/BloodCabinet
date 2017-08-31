/*************************************************************************************************************
overview:20170315
    ���ļ�Ϊ������Χ�ĸ��أ� 1��ǿ�ƻ�˪       --ֻ�ǿ�ͣѹ�� 
                             2������ȿ���     --CN29������
                             3���ڷ������     --CN29
                             4��������������� --CN30
                             5�������ƿ���     --CN17
                             6��Զ�̱��������--CN28
*************************************************************************************************************/
#include "r_cg_macrodriver.h"
#include "h_type_define.h"
#include "h_ad.h"
//#include "h_key_disp.h"
#include "m_key_operation.h"
#include "m_compressor.h"
#include "m_peripheral_control.h"
#include "h_protect.h"
#include "h_com.h"

//----------------------------------------------------------------------------------------------------------
//��������
void defrosting_control(void);
void defrosting_delaytime(void);
void defrosting_interval_delaytime(void);
void door_heat_control(void);
void door_heat_RL_out(void);
void inside_fan_control(void);
void fan_RL_out(void);
void led_control(void);
void liquid_led_control(void);
void led_on_delaytime(void);
void led_RL_out(void);
void remote_alarm_lamp(void);
void remote_alarm_test_delaytime(void);


//�궨��
#define   DOOR_IN_PIN                P5.2       //�ſ���1�����   H_open L_close
#define   INSIDE_FAN_RL_OUT_PIN      P7.3       //�ڲ���������    
#define   CONDENSATE_FAN_RL_OUT_PIN  P13.0      //������������
#define   HEAT_RL_OUT_PIN            P4.2       //�����(����)��� RL6
#define   LED_RL_OUT_PIN             P14.6      //LED�����

#define   DOOR_OPEN                  1          
#define   DOOR_CLOSE                 0     
//��־����
flag_type flg_peripheral,flg_peripheral2;

//����
uint8_t  guc_defrosting_time;                  //��˪ʱ���ʱ
uint16_t guc_defrosting_interval_time;         //��˪���ʱ���ʱ
uint8_t  guc_led_on_delaytime;                 //led������ʱ
uint8_t  guc_humidity;                         //ʪ��
uint8_t  guc_remote_alarm_test_delaytime;      //���¡����Ա���������Զ�̱�����ʱxs������



//��˪����
/**********************************************************************************************************************************************
��������: ��˪״̬�Ľ����˳�__��˪ֻ��ѹ��ͣ��

����λ��:  ��ѭ��-------------------------------------ok
***********************************************************************************************************************************************/
void defrosting_control(void)
{
    if(bflg_TE_fault == 1)
    {
        bflg_defrosting_status = 0;
    }
    else
    {
        if(bflg_defrosting_status == 1)
        {
            if(((gss_TU >= 56) || (gss_TD >= 56)) ||
               (gss_TA >= (gss_factory_parm_value[DISP_FAC_dFH]*10))|| 
               (bflg_defrosting_time == 0))             
            {
                bflg_defrosting_status = 0;
            }
        }
        else
        {
            if(bflg_defrosting_interval_time == 0)
            {
                if(gss_TE <=  (gss_factory_parm_value[DISP_FAC_dFL]*10))
                {
                    bflg_defrosting_status = 1;
                    
                    bflg_defrosting_time = 1;
                    guc_defrosting_time = 0;
                    bflg_defrosting_interval_time = 1;
                    guc_defrosting_interval_time = 0;
                }
            }
        }
    }
}
/**********************************************************************************************************************************************
��������: ��˪ʱ����ʱ

����λ��:  1min��ʱ��-------------------------------------ok
***********************************************************************************************************************************************/
void defrosting_delaytime(void)
{
    if(bflg_defrosting_time == 1)
    {
        guc_defrosting_time++;
        if(guc_defrosting_time >= 20)    //20min
        {
            guc_defrosting_time = 0;
            bflg_defrosting_time = 0;
        }        
    }
}
/**********************************************************************************************************************************************
��������: ��˪���ʱ����ʱ��ǿ�ƻ�˪�����ڵ�12Сʱ��������һ��

����λ��:  1min��ʱ��-------------------------------------ok
***********************************************************************************************************************************************/
void defrosting_interval_delaytime(void)
{
    if(bflg_defrosting_interval_time == 1)
    {
        guc_defrosting_interval_time++;
        if(guc_defrosting_interval_time >= 720)   //12h
        {
            guc_defrosting_interval_time = 0;
            bflg_defrosting_interval_time = 0;
        }
    }  
}

//�����(�������˿)����
/**********************************************************************************************************************************************
��������: ����ȵĿ���  humidityʪ��  humiture��ʪ��  temperature

����λ��: ��ѭ�� -------------------------------------ok
***********************************************************************************************************************************************/
void door_heat_control(void)
{
    if(bflg_HT_fault == 1)   //��ʪ�ȴ���������
    {
        bflg_heat_runing = 1;
    }
    else
    {
        if(bflg_heat_runing == 1)
        {
            if(guc_humidity <= gss_factory_parm_value[DISP_FAC_HT2])    
            {
                bflg_heat_runing = 0;
            }
        }
        else
        {
            if(guc_humidity >= gss_factory_parm_value[DISP_FAC_HT1])    
            {
                bflg_heat_runing = 1;
            } 
        }
    }
}
/**********************************************************************************************************************************************
��������:  �����˿�ܽŵ����

����λ��: ��ѭ�� -------------------------------------ok
***********************************************************************************************************************************************/
void door_heat_RL_out(void)
{
    if(bflg_heat_runing == 1)
    {
        HEAT_RL_OUT_PIN = 1;
    }
    else
    {
        HEAT_RL_OUT_PIN = 0;
    }
}


//�������
/**********************************************************************************************************************************************
��������: �ڷ������


����λ��: ��ѭ����----------------------------------------------ok
***********************************************************************************************************************************************/
void inside_fan_control(void)
{
    if(bflg_prot2_door1_pin == DOOR_OPEN)  //����
    {
        bflg_inside_fan_runing = 0;
    }
    else
    {
        if(bflg_alarm_higt_TE == 1)
        {
            bflg_inside_fan_runing = 0;
        }
        else
        {
            bflg_inside_fan_runing = 1;
        }
    }
 }



/**********************************************************************************************************************************************
��������: ������: �ڷ�� + �������(��ѹ����ͣ)


����λ��: ��ѭ��----------------------------------------------ok
***********************************************************************************************************************************************/
void fan_RL_out(void)
{
    //�ڷ��
    if(bflg_inside_fan_runing == 1)
    {
        INSIDE_FAN_RL_OUT_PIN = 1;
    }
    else
    {
        INSIDE_FAN_RL_OUT_PIN = 0;
    }

    //--------------------------------------
    
    //�������
    if(bflg_compressor_runing == 1)
    {
        CONDENSATE_FAN_RL_OUT_PIN = 1;
    }
    else
    {
        CONDENSATE_FAN_RL_OUT_PIN= 0;
    }
}


//LED�����ƿ���
/**********************************************************************************************************************************************
��������: LED�ƵĿ���: ����(���ȼ����) + �����ſ���

����λ��:  ��ѭ��(������ʱ��)----------------------------------------------ok
***********************************************************************************************************************************************/
void led_control(void)
{
    if(bflg_com_type == 0) //LED��
    {
        if(gss_factory_parm_value[DISP_FAC_LC] == 1)   //Ĭ�ϲ�����
        {
            if(bflg_led_on == 1)                        //����
            {
                if(guc_key_val == KEY_LAMP)
                {
                    bflg_led_on = 0;                  //�ص�
                    
                    bflg_led_on_delaytime = 0;       //�忪����ʱ��־
                    guc_led_on_delaytime = 0;
                    bflg_key_open_led = 0;           //�尴�����ĵƱ�־
                    bflg_key_close_led = 1;          //�����ص�
                }
                else
                {
                    if(DOOR_IN_PIN == DOOR_CLOSE)
                    {
                        if(gss_user_manage_parm_value[DISP_USER_MANAGE_Lt] == -1)   //�ر���ʱ�صƹ���
                        {
                            if(bflg_key_open_led == 0)  //���ǰ������ĵ�
                            {
                                bflg_led_on = 0;
                            }
                        }
                        else
                        {
                            if(bflg_led_on_delaytime == 0)
                            {
                                bflg_led_on = 0;
                                bflg_key_open_led= 0;
                            }
                        }
                    }
                }
            }
            else
            {
                if(guc_key_val == KEY_LAMP)
                {
                    bflg_led_on = 1;            //����
                    
                    bflg_led_on_delaytime = 1;  //������ʱ��־
                    guc_led_on_delaytime = 0;
                    bflg_key_open_led = 1;      //�������ĵƱ�־
                    bflg_key_close_led = 0;     //�尴���صƱ�־
                }
                else
                {
                    if(DOOR_IN_PIN == DOOR_OPEN)
                    {
                        if(bflg_key_close_led != 1)
                        {
                            bflg_led_on = 1;
                        }
                    }
                    else
                    {
                        bflg_key_close_led = 0;
                    }
                }
            }
        }
        else
        {
            if(DOOR_IN_PIN == DOOR_OPEN)
            {
                bflg_led_on = 1;
            }
            else
            {
                bflg_led_on = 0;
            }
        }
    }
    /*else                   //Һ����
    {
        if(gss_liquid_led_button == 1) 
        {
            bflg_led_on = 1;                  //����
        }
        else
        {
            bflg_led_on = 0;                  //�ص�
        }
    } */
}
/**********************************************************************************************************************************************
��������: Һ�������ƿ��ƣ�����LED�У���LED������Ҫ��ʱ��⣬�����Һ��������ʱ����ʱ

����λ��:     ��ѭ��---------------------------------------------no
***********************************************************************************************************************************************/
void liquid_led_control(void)
{
    if(bflg_com_type == 1) //Һ����
    {
        if(gss_liquid_led_button == 1) 
        {
            bflg_led_on = 1;                  //����
        }
        else
        {
            bflg_led_on = 0;                  //�ص�
        }
    } 
}

/**********************************************************************************************************************************************
��������: LED����������ʱ

����λ��:  1min----------------------------------------------ok
***********************************************************************************************************************************************/
void led_on_delaytime(void)
{
    if(gss_user_manage_parm_value[DISP_USER_MANAGE_Lt] != -1)
    {
        if(bflg_led_on_delaytime == 1)
        {
            guc_led_on_delaytime++;
            if(guc_led_on_delaytime >= gss_user_manage_parm_value[DISP_USER_MANAGE_Lt])  //min
            {
                guc_led_on_delaytime = 0;
                bflg_led_on_delaytime = 0;
            }
        }
    }
}

/**********************************************************************************************************************************************
��������: LED�������

����λ��:  ��ѭ��----------------------------------------------ok
***********************************************************************************************************************************************/
void led_RL_out(void)
{
    if(bflg_led_on == 1)
    {
        LED_RL_OUT_PIN = 1;
    }
    else
    {
        LED_RL_OUT_PIN = 0;
    }
}
/**********************************************************************************************************************************************
��������: Զ�̱�����

����λ��:  ��ѭ��----------------------------------------------ok
***********************************************************************************************************************************************/
void remote_alarm_lamp(void)
{
    if(bflg_remote_alarm_test == 0)
    {
        if ((bflg_alarm_high_TUTD == 1)  ||  //����
            (bflg_alarm_low_TUTD == 1)   ||  //����
            (bflg_alarm_high_AIR == 1)   ||  //��������
            (bflg_alarm_low_AIR == 1)    ||  //��������
            (bflg_TA_fault == 1)         ||  //���ƴ���������
            (bflg_TU_fault == 1)         ||  //�ϴ���������
            (bflg_TD_fault == 1)         ||  //�´���������
            (bflg_alarm_power_off == 1)  ||  //�ϵ籨��
            (bflg_alarm_power_fault == 1)||  //��Դ�����
            (bflg_alarm_open_door == 1))     //���ű���
        {
            bflg_alarm_output = 1; //ͨѶ����״̬
            RL_ALM_OUT_PIN = 0;    //��  ������ôҪ�󱨾�ʱ = 0��
        }
        else
        {
            bflg_alarm_output = 0;
            RL_ALM_OUT_PIN = 1;    //��
        }
    }
    else
    {
        RL_ALM_OUT_PIN = 0;    
    }

}

/**************************************************************************************************************************************************************
�������ܣ�����"���Ա���"����ʱԶ�̱�������ʱ3ʱ�ָ�����


����λ�ã�1s----------------------------------------ok
****************************************************************************************************************************************************************/
void remote_alarm_test_delaytime(void)
{
    if(bflg_remote_alarm_test == 1)
    {
        guc_remote_alarm_test_delaytime++;
        if(guc_remote_alarm_test_delaytime >= 3)
        {
            guc_remote_alarm_test_delaytime = 0;
            bflg_remote_alarm_test = 0;
        }
    }
}









/**********************************END OF THE FILE*************************************************/
