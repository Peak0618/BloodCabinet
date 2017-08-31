/*******************************************************************************************************************************************
overview:
        1�������¸ߡ����±���
        2�����±���
        3�������ߡ����±���
        4����˪������(�Ƚ���)���±���
        5��220�ϵ籨��
        6��12v����
        7�����δ����
        8����ص�����
        9�����ű���
        10���������౨��
        11��
********************************************************************************************************************************************/
#ifndef _PROTECT_C_
#define _PROTECT_C_
//------------------------------------------------------------------------------
//�����ļ�����
#include "h_type_define.h"
#include "h_ad.h"
#include "h_rom_para2.h"
#include "h_key_disp.h"
#include "h_protect.h"
//#include "h_volt_ctrl.h"
#include "h_bat_ctrl.h"

//peak add
#include "M_buzz.h"
#include "h_main.h"
#include "m_key_operation.h"
#include "h_com.h"
//------------------------------------------------------------------------------
//��������
void protect_high_TUTD_delaytime(void);
void protect_low_TUTD_delaytime(void);
void protect_high_THW_delaytime(void);
void protect_air_alarm_boot_delaytime(void);
void protect_air_higt_delaytime(void);
void protect_air_low_delaytime(void);
void protect_TE_boot_delaytime();
void protect_TE_interval_delaytime();
void protect_higt_TE_delaytime();







void prot2_power_off_deal(void);   //�ϵ籣�������������ѭ�������е���

void port2_power_fault_deal(void); //��Դ����ϴ����������ѭ�������е���

void prot2_bat_discnnt_delaytime(void); //���δ���ӱ�����ʱ������100ms��ʱ�����е���

void prot2_bat_space_delaytime(void);   //���δ���Ӽ������ʱ������100ms��ʱ�����е���

void prot2_bat_low_deal(void);     //��ص����ͱ��������������ѭ�������е���

void prot2_bat_low_delaytime(void);//��ص������ӳٳ�����1min��ʱ�����е���

void port2_door_delaytime(void);   //�ſ��ر�����ʱ������1s��ʱ�����е���

void port2_T_fault_deal(void);     //���������ϴ����������ѭ�������е���

void prot2_TC_first_delaytime(void);    //���������״α�����ʱ������1min��ʱ�����е���

void prot2_cond_dirty_delaytime(void);  //�������ౣ����ʱ������1s��ʱ�����е���

void prot2_volt_deal(void);   //��ѹ���걨�����������������ѭ�������е���

void warning_light_deal(void);   //�����ƴ����������ѭ�������е���

void alarm_buzz_deal(void);   //���������������������ѭ�������е���

void alarm_buzz_off_delaytime(void);    //����������ȡ����ʱ������1min��ʱ�����е���

void bat_T_hight_protect(void);             //����¶ȹ����ж�



//------------------------------------------------------------------------------
//��������
flag_type flg_prot1,flg_prot2,flg_alarm1,flg_alarm2;

//peak add
flag_type   memory;

//------------------------------------------------------------------------------

int16_t   gss_prot2_bat_discnnt_delaytimer;  //���δ���ӱ�����ʱ��ʱ��
int16_t   gss_prot2_bat_space_delaytimer;    //���δ���Ӽ������ʱ��ʱ��
int16_t   gss_prot2_bat_low_delaytimer;      //��ص�������ʱ��ʱ��

int16_t   gss_prot2_door_delaytimer;    //�ſ��ر�����ʱ��ʱ��

int16_t   gss_prot2_TC_first_delaytimer;//���������״α�����ʱ��ʱ��
int16_t   gss_prot2_cond_dirty_delaytimer;   //����������ʱ��ʱ��

int16_t   gss_alarm_buzz_off_delaytimer;     //����������ȡ����ʱ��ʱ��

uint16_t  gus_trip_code1;     //���ϴ���1
uint16_t  gus_trip_code2;     //���ϴ���2

int16_t   gss_TUTD_high_delaytimer;        //�����¸��±�����ʱ
int16_t   gss_TUTD_low_delaytimer;         //�����µ��±�����ʱ
int16_t   gss_THW_high_delaytimer;         //���¹��߱���
int16_t   gss_air_alarm_boot_delaytime;    //��������������ʱ
int16_t   gss_AIR_high_delaytimer;         //�������±�����ʱ
int16_t   gss_AIR_low_delaytimer;          //�������±�����ʱ
int16_t   gss_TE_boot_delaytime;           //��˪�����������ж��ϵ���ʱ
int16_t   gss_TE_interval_delaytime;       //��˪�����������ж����
int16_t   gss_TE_higt_delaytimer;          //��˪���������±���
int16_t   gss_THW_too_high_delaytimer;     //���¹�����5�濪����������


/**********************************************************************************************************************************
�������ܣ����±���:��������һ·���ڵ��ڸ����趨�¶�1min
                   
����λ�ã�1s----------------------------------------ok      
**********************************************************************************************************************************/
void protect_high_TUTD_delaytime(void)
{
    if(bflg_alarm_high_TUTD == 0)
    {
        if((gss_TU >= gss_user_set_parm_value[DISP_USER_SET_ALH])||(gss_TD >= gss_user_set_parm_value[DISP_USER_SET_ALH]))
        {
            gss_TUTD_high_delaytimer++;
            if (gss_TUTD_high_delaytimer >= 60)   //60s
            {
                gss_TUTD_high_delaytimer = 0;
                bflg_alarm_high_TUTD = 1;
                gus_trip_code1 |= H_T_CODE;
            }
        }
        else
        {
            gss_TUTD_high_delaytimer = 0;
        }
    }
    else
    {
        if((gss_TU < gss_user_set_parm_value[DISP_USER_SET_ALH])&&(gss_TD < gss_user_set_parm_value[DISP_USER_SET_ALH]))
        {
            gss_TUTD_high_delaytimer++;
            if (gss_TUTD_high_delaytimer >= 60)   //60s
            {
                gss_TUTD_high_delaytimer = 0;
                bflg_alarm_high_TUTD = 0;
                gus_trip_code1 &= ~H_T_CODE;
            }
        }
        else
        {
            gss_TUTD_high_delaytimer = 0;
        }
    }
}

/**********************************************************************************************************************************
�������ܣ����±���:��������һ·С�ڵ��ڵ����趨�¶�1min
                   
����λ�ã�1s----------------------------------------ok
**********************************************************************************************************************************/
void protect_low_TUTD_delaytime(void)
{
    if(bflg_alarm_low_TUTD == 0)
    {
        if((gss_TU <= gss_user_set_parm_value[DISP_USER_SET_ALL])||(gss_TD <= gss_user_set_parm_value[DISP_USER_SET_ALL]))
        {
            gss_TUTD_low_delaytimer++;
            if (gss_TUTD_low_delaytimer >= 60)   //60s
            {
                gss_TUTD_low_delaytimer = 0;
                bflg_alarm_low_TUTD = 1;
                gus_trip_code1 |= L_T_CODE;
            }
        }
        else
        {
            gss_TUTD_low_delaytimer = 0;
        }
    }
    else
    {
        if((gss_TU > gss_user_set_parm_value[DISP_USER_SET_ALL])&&(gss_TD > gss_user_set_parm_value[DISP_USER_SET_ALL]))
        {
            gss_TUTD_low_delaytimer++;
            if (gss_TUTD_low_delaytimer >= 60)   //60s
            {
                gss_TUTD_low_delaytimer = 0;
                bflg_alarm_low_TUTD = 0;
                gus_trip_code1 &= ~L_T_CODE;
            }
        }
        else
        {
            gss_TUTD_low_delaytimer = 0;
        }
    }
}

/**********************************************************************************************************************************
�������ܣ����¹��߱���:����>= AHH
                   
����λ�ã�1s----------------------------------------ok
**********************************************************************************************************************************/
void protect_high_THW_delaytime(void)
{
    if(bflg_alarm_higt_THW == 0)
    {
        if(gss_THW >= gss_factory_parm_value[DISP_FAC_AHH]*10)    //ֻ��˸
        {
            gss_THW_high_delaytimer++;
            if (gss_THW_high_delaytimer >= 30)   //30s
            {
                gss_THW_high_delaytimer = 0;
                bflg_alarm_higt_THW = 1;         //ֻ�Ǳ�������˸
                gus_trip_code1 |= THW_H_CODE;
            }
        }
        else
        {
            gss_THW_high_delaytimer = 0;
        }
    }
    else
    {
        if(gss_THW < gss_factory_parm_value[DISP_FAC_AHH]*10 - 10)
        {
            gss_THW_high_delaytimer++;
            if (gss_THW_high_delaytimer >= 30)   //60s
            {
                gss_THW_high_delaytimer = 0;
                bflg_alarm_higt_THW = 0;
                gus_trip_code1 &= ~THW_H_CODE;
            }
        }
        else
        {
            gss_THW_high_delaytimer = 0;
        }
    }       
    //-------------------------------------------------------------------------
    //������´������¶ȡݣ�AHH+5�棩����ͬʱ��������������
    if(bflg_alarm_too_higt_THW == 0)
    {
        if(gss_THW >= gss_factory_parm_value[DISP_FAC_AHH]*10 +50)   //����
        {
            gss_THW_too_high_delaytimer++;
            if (gss_THW_too_high_delaytimer >= 30)   //30s
            {
                gss_THW_too_high_delaytimer = 0;
                bflg_alarm_too_higt_THW = 1;         //ֻ��������
            }
        }
        else
        {
            gss_THW_too_high_delaytimer = 0;
        }
    }
    else
    {
        if(gss_THW < gss_factory_parm_value[DISP_FAC_AHH]*10 - 10)
        {
            gss_THW_too_high_delaytimer++;
            if (gss_THW_too_high_delaytimer >= 30)   //60s
            {
                gss_THW_too_high_delaytimer = 0;
                bflg_alarm_too_higt_THW = 0;
            }
        }
        else
        {
            gss_THW_too_high_delaytimer = 0;
        }
    }  
}

/**********************************************************************************************************************************
�������ܣ�����������ʱ
                   
����λ�ã�1s---------------------------------------ok
**********************************************************************************************************************************/
void protect_air_alarm_boot_delaytime()
{
    if(gss_factory_parm_value[DISP_FAC_Ad] != -1)     //δ�ر�
    {
        if(bflg_air_alarm_boot_delaytime == 0)
        {
            gss_air_alarm_boot_delaytime++;
            if(gss_air_alarm_boot_delaytime >= gss_factory_parm_value[DISP_FAC_Ad])
            {
                gss_air_alarm_boot_delaytime = 0;
                bflg_air_alarm_boot_delaytime = 1;
            }
        }
    }
    else
    {
        gss_air_alarm_boot_delaytime = 0;        //�ر�����ʱ���¼�ʱ
        bflg_air_alarm_boot_delaytime = 0;
    }
}



/**********************************************************************************************************************************
�������ܣ��������±���
                   
����λ�ã�1s----------------------------------------ok
**********************************************************************************************************************************/
void protect_air_higt_delaytime(void)
{
    if((gss_factory_parm_value[DISP_FAC_Ad] != -1) && (bflg_air_alarm_boot_delaytime == 1)) //����&��ʱ��
    {
        if(bflg_alarm_high_AIR == 0)
        {
            if(gss_TA >= gss_factory_parm_value[DISP_FAC_AAH]*10)
            {
                gss_AIR_high_delaytimer++;
                if(gss_AIR_high_delaytimer >= 60)  //60s = 1min
                {
                    gss_AIR_high_delaytimer = 0;
                    bflg_alarm_high_AIR = 1;
                    gus_trip_code1 |= AIR_H_CODE;
                }
            }
            else
            {
                gss_AIR_high_delaytimer = 0;
            }
        }
        else
        {
            if(gss_TA < gss_factory_parm_value[DISP_FAC_AAH]*10 - 5)
            {
                gss_AIR_high_delaytimer++;
                if(gss_AIR_high_delaytimer >= 60)  //1min
                {
                    gss_AIR_high_delaytimer = 0;
                    bflg_alarm_high_AIR = 0;
                    gus_trip_code1 &= ~AIR_H_CODE;
                }
            }
            else
            {
                gss_AIR_high_delaytimer = 0;
            }
        }
    }
    else
    {
        gss_AIR_high_delaytimer = 0;              //�ر�ʱ���ޱ���
        bflg_alarm_high_AIR = 0;
        gus_trip_code1 &= ~AIR_H_CODE;
    }
}

/**********************************************************************************************************************************
�������ܣ��������±���
                   
����λ�ã�1s----------------------------------------ok
**********************************************************************************************************************************/
void protect_air_low_delaytime(void)
{
    if((gss_factory_parm_value[DISP_FAC_Ad] != -1) && (bflg_air_alarm_boot_delaytime == 1)) //����&��ʱ��
    {
        if(bflg_alarm_low_AIR == 0)
        {
            if(gss_TA <= gss_factory_parm_value[DISP_FAC_AAL]*10)
            {
                gss_AIR_low_delaytimer++;
                if(gss_AIR_low_delaytimer >= 60)  //60s = 1min
                {
                    gss_AIR_low_delaytimer = 0;
                    bflg_alarm_low_AIR = 1;
                    gus_trip_code2 |= AIR_L_CODE;
                }
            }
            else
            {
                gss_AIR_low_delaytimer = 0;
            }
        }
        else
        {
            if(gss_TA > gss_factory_parm_value[DISP_FAC_AAL]*10 + 5)
            {
                gss_AIR_low_delaytimer++;
                if(gss_AIR_low_delaytimer >= 60)  //1min
                {
                    gss_AIR_low_delaytimer = 0;
                    bflg_alarm_low_AIR = 0;
                    gus_trip_code2 &= ~AIR_L_CODE;
                    
                }
            }
            else
            {
                gss_AIR_low_delaytimer = 0;
            }
        }
    }
    else
    {
        gss_AIR_low_delaytimer = 0;
        bflg_alarm_low_AIR = 0;
        gus_trip_code2 &= ~AIR_L_CODE;
    }
}

/**********************************************************************************************************************************
�������ܣ���˪������(���Ƚ���������)�����ж�������ʱ
                   
����λ�ã�1min----------------------------------------ok
**********************************************************************************************************************************/
void protect_TE_boot_delaytime(void)
{
    if(bflg_TE_boot_delaytime == 1)      //ϵͳ��ʼ��ʱҪ��Ϊ1
    {
        gss_TE_boot_delaytime++;
        if(gss_TE_boot_delaytime >= 60)  //60min
        {
            gss_TE_boot_delaytime = 0;
            bflg_TE_boot_delaytime = 0;
        }
    }
}

/**********************************************************************************************************************************
�������ܣ���˪������(���Ƚ���������)�����ж���ʱ����
                   
����λ�ã�1min----------------------------------------ok
**********************************************************************************************************************************/
void protect_TE_interval_delaytime(void)
{
    if(bflg_TE_interval_delaytime == 1)    
    {
        gss_TE_interval_delaytime++;
        if(gss_TE_interval_delaytime >= 20)  //20min
        {
            gss_TE_interval_delaytime = 0;
            bflg_TE_interval_delaytime = 0;
        }
    }
}

/**********************************************************************************************************************************
�������ܣ���˪�����������Ƚ��������������±���
                   
����λ�ã�1s----------------------------------------ok
**********************************************************************************************************************************/
void protect_higt_TE_delaytime(void)
{
    if(bflg_TE_boot_delaytime == 0)           //������ʱ60min����
    {
        if(bflg_alarm_higt_TE == 0)           //�޹���
        {
            if(bflg_prot2_door1_pin == 1)     //����
            {
                bflg_TE_interval_delaytime = 1;  //20min���
                gss_TE_interval_delaytime = 0;
            }
            else
            {
                if(bflg_TE_interval_delaytime == 0)
                {
                    if(gss_TE > 200)              //20��
                    {
                        gss_TE_higt_delaytimer++;
                        if(gss_TE_higt_delaytimer >= 60)  //60s
                        {
                            gss_TE_higt_delaytimer = 0;
                            
                            bflg_alarm_higt_TE = 1;
                            gus_trip_code2 |= TE_H_CODE;
                        }
                    }
                    else
                    {
                        gss_TE_higt_delaytimer = 0;
                    
                        bflg_TE_interval_delaytime = 1;
                        gss_TE_interval_delaytime = 0;                    
                    }
                }
            }
        }
        else //�й���
        {
            if(gss_TE <= 180)              //18��
            {
                gss_TE_higt_delaytimer++;
                if(gss_TE_higt_delaytimer >= 60)  //60s
                {
                    gss_TE_higt_delaytimer = 0;
                    
                    bflg_alarm_higt_TE = 0;
                    gus_trip_code2 &= ~TE_H_CODE;
                    
                    bflg_TE_interval_delaytime = 1;
                    gss_TE_interval_delaytime = 0;
                }
            }
            else
            {
                gss_TE_higt_delaytimer = 0;                 
            }
        }
    }
}






//��

/***************************************************************************************************************************
�������� : 220v�ϵ籨���ĳ����ͻָ�


����λ�ã���ѭ��-------------------------------ok
***************************************************************************************************************************/
void prot2_power_off_deal(void)    
{
    if (gss_VAC <= ram_para2[num_power_fautl_VAC1])         //80V
    {
        bflg_alarm_power_off = 1;
        gus_trip_code1 |= VAC_OFF_CODE;
    }
    else if (gss_VAC >= ram_para2[num_power_fautl_VAC2])    //90V
    {
        bflg_alarm_power_off = 0;
        gus_trip_code1 &= ~VAC_OFF_CODE;
    }
}

/************************************************************************************************************************************
��������: ��Դ��(12v)���ϵĳ����ͻָ�
          ���������ﵽ����ֵ����ʾ��Ҳ�����б���ָʾ�ˣ�

����λ��:   ��ѭ��-----------------------------ok
************************************************************************************************************************************/
void port2_power_fault_deal(void)     
{
    if (guc_12V < ram_para2[num_power_off_12V])  //9v
    {
        /*if (bflg_alarm_power_off == 0)           //δ��ǿ��  --ԭ     
        {
            bflg_alarm_power_fault = 1;
            gus_trip_code1 |= 0x4000;
        }
        else
        {
            bflg_alarm_power_fault = 0;
            gus_trip_code1 &= ~0x4000;
        }*/
        
        bflg_alarm_power_fault = 1;
        gus_trip_code1 |= DC_F_CODE;
    }
    else if (guc_12V > ram_para2[num_power_on_12V]) //11v
    {
        bflg_alarm_power_fault = 0;
        gus_trip_code1 &= ~DC_F_CODE;
    }
}

/******************************************************************************************************************************
��������: ���δ���ӹ��ϵĳ����ͻָ�

����λ�ã�100ms--------------------------------------------------ok
*********************************************************************************************************************************************/
void prot2_bat_discnnt_delaytime(void)   
{
    if (bflg_bat_charging == 0)              //�ǳ��״̬
    {
        if (bflg_alarm_bat_discnnt == 0)     
        {
            if (guc_bat_Vout_instant < ram_para2[num_port2_bat_discnnt_V1])   //20 = 2v
            {
                bflg_alarm_bat_discnnt = 1;
                gus_trip_code1 |= BAT_NO_CODE;
            }
        }
        else   
        {
            if (guc_bat_Vout_instant > ram_para2[num_port2_bat_discnnt_V2])   //50 = 5V
            {
                bflg_alarm_bat_discnnt = 0;
                gus_trip_code1 &= ~BAT_NO_CODE;
            }
        }
    }
    else   //���״̬
    {
        if (bflg_alarm_bat_discnnt == 0)      
        {
            //if (guc_bat_Vout_instant < 90)//ram_para2[num_port2_bat_discnnt_V1]) //peak ������Ϊ 9v
            if (guc_bat_Vout_instant < 90)       
            {
                bflg_alarm_bat_discnnt = 1;
                gus_trip_code1 |= BAT_NO_CODE;
            }
            if(bflg_bat_type == LEAD_ACID_BAT)      //Ǧ���׼��﮵��+      
            {
                if (guc_bat_Vout_instant > 151)      
                {
                    bflg_alarm_bat_discnnt = 1;
                    gus_trip_code1 |= BAT_NO_CODE;
                }
            }
            
        }
        else    
        {
            if (guc_bat_Vout_instant > 100)//ram_para2[num_port2_bat_discnnt_V2])       // 10v
            {
                bflg_alarm_bat_discnnt = 0;
                gus_trip_code1 &= ~BAT_NO_CODE;
            }
        }
    }
}


/*****************************************************************************************************************************
function: ���δ���Ӽ������ʱ����----δ��

����λ�ã�100ms-----------------------------ok
******************************************************************************************************************************/
void prot2_bat_space_delaytime(void)
{
    if (bflg_prot2_bat_space_delaytime == 1)
    {
        gss_prot2_bat_space_delaytimer++;
        if (gss_prot2_bat_space_delaytimer >= 200) //5s
        {
            gss_prot2_bat_space_delaytimer = 0;
            bflg_prot2_bat_space_delaytime = 0;
            
            //bflg_prot2_bat_space_reach = 1;
        }
    }
    else
    {
    	  gss_prot2_bat_space_delaytimer++;
        if (gss_prot2_bat_space_delaytimer >= 50) //5s
        {
            gss_prot2_bat_space_delaytimer = 0;
            bflg_prot2_bat_space_delaytime = 1;
            
            //bflg_prot2_bat_space_reach = 1;
        }
    }
}

/******************************************************************************************************************************
��������: ��ص����͹��ϵĳ����ͻָ�

�ָ�����:1�����δ���ӣ�2����ѹ>��ֵ  (3���ڳ��ʱ���ܼ���ѱ�ȥ�� )

����λ�ã���ѭ��-----------------------------------ok
******************************************************************************************************************************/
void prot2_bat_low_deal(void)       
{    
    if ((bflg_prot2_bat_low_delaytime == 0) && (bflg_alarm_bat_discnnt == 0))   //���24Сʱ���ҵ��������
	{  
        if (bflg_bat_type == 0)    //�����Ǧ����
        {
            if (guc_bat_Vout < ram_para2[num_bat_low_V1])       //10.5V
            {
                bflg_alarm_bat_low = 1;
                gus_trip_code1 |= BAT_L_CODE;
            }
            else if (guc_bat_Vout > ram_para2[num_bat_low_V2])   //11.0V
            {
                bflg_alarm_bat_low = 0;
                gus_trip_code1 &= ~BAT_L_CODE;
            }
        }
        else   //�����﮵��
        {
            if (guc_bat_Vout < ram_para2[num_bat_low_V3])       //9.0V
            {
                bflg_alarm_bat_low = 1;
                gus_trip_code1 |= BAT_L_CODE;
            }
            else if (guc_bat_Vout > ram_para2[num_bat_low_V4])   //9.5V
            {
                bflg_alarm_bat_low = 0;
                gus_trip_code1 &= ~BAT_L_CODE;
            }
        }
    }
    else
    {
        bflg_alarm_bat_low = 0;
        gus_trip_code1 &= ~BAT_L_CODE;
    }
}

/********************************************************************************************************************************
��������: ��ʼ�ϵ���24Сʱ���ٶԵ�ص����ͽ��м��
         
����λ�ã� 1min-------------------ok         
********************************************************************************************************************************/
void prot2_bat_low_delaytime(void) //��ص������ӳٳ�����1min��ʱ�����е���
{
    if (bflg_prot2_bat_low_delaytime == 1)
    {
        gss_prot2_bat_low_delaytimer++;
        if (gss_prot2_bat_low_delaytimer >= (ram_para2[num_prot2_bat_low_delaytime] * 60)) //24Сʱ
        {
            gss_prot2_bat_low_delaytimer = 0;
            bflg_prot2_bat_low_delaytime = 0;
        }
    }
}

/*****************************************************************************************************************************
function: ���ű����ĳ����ͻָ���������⿪��5min�򱨿��ű���

����λ�ã� 1s---------------------------------ok
*****************************************************************************************************************************/
void port2_door_delaytime(void)    
{
    if ((bflg_prot2_door1_pin == 1))     //����
    {
        if (bflg_alarm_open_door == 0)
        {
            gss_prot2_door_delaytimer++;
            //if (gss_prot2_door_delaytimer >= (ram_para[num_prot2_door_delaytime] * 60))  //5����
            if (gss_prot2_door_delaytimer >= (gss_user_manage_parm_value[DISP_USER_MANAGE_dA] * 60))        //5���� �ÿɵ���gss_user_parm_value[DISP_USER_dA]�滻֮ǰ�Ĺ̶�ֵ
            {
                gss_prot2_door_delaytimer = 0;
                
                bflg_alarm_open_door = 1; //���ű���
                gus_trip_code2 |= DOOR_OP_CODE;
            }
        }
    }
    else
    {
        gss_prot2_door_delaytimer = 0;
        bflg_alarm_open_door = 0;
        gus_trip_code2 &= ~DOOR_OP_CODE;
    }
}

/******************************************************************************************************************************
��������: ���¡��Ƚ�����������pt100���������϶�ͳһ��Ϊ bflg_alarm_T_fault ����;

����λ�ã���ѭ��---------------------------------ok
*******************************************************************************************************************************/
void port2_T_fault_deal(void)       
{
    if (bflg_THW_fault == 1)    // ���±���
    {
        bflg_alarm_T_fault = 1;
    }
    else if (bflg_TE_fault == 1) // �Ƚ���      
    {
        bflg_alarm_T_fault = 1;
    }
    else if (bflg_TC_fault == 1) // ������
    {
        bflg_alarm_T_fault = 1;
    }
    else if (bflg_TA_fault == 1) // ��������pt100
    {
        bflg_alarm_T_fault = 1;
    }
    else
    {
        bflg_alarm_T_fault = 0;
    }
}


/************************************************************************************************************************
��������: �ϵ�2h���ٽ����ж��������౨�������������״α�����ʱ��

����λ�ã�1min--------------------------------------ok
************************************************************************************************************************/
void prot2_TC_first_delaytime(void)     
{
    if (bflg_prot2_TC_first_delaytime == 1)
    {
        gss_prot2_TC_first_delaytimer++;
        if (gss_prot2_TC_first_delaytimer >= ram_para2[num_prot2_TC_first_delaytime]) //120min = 2h    //120
        {
            gss_prot2_TC_first_delaytimer = 0;
            bflg_prot2_TC_first_delaytime = 0;
        }
    }
}

/********************************************************************************************************************************
��������: �������౨���ĳ����ͻָ�

����λ�ã�1s-----------------------------------------------ok
********************************************************************************************************************************/
void prot2_cond_dirty_delaytime(void) 
{
    if (bflg_prot2_TC_first_delaytime == 0)          //��ʼ�ϵ�����ʱ2Сʱ
    {
        if (bflg_alarm_cond_dirty == 0)              
        {
            if ((gss_TC - gss_THW) >=  gss_factory_parm_value[DISP_FAC_r1]*10)   //13��
            {                        
                gss_prot2_cond_dirty_delaytimer++;
                if (gss_prot2_cond_dirty_delaytimer >= ram_para2[num_prot2_cond_dirty_delaytime])   //300s = 5min
                {
                    gss_prot2_cond_dirty_delaytimer = 0;
                    
                    bflg_alarm_cond_dirty = 1;
                    gus_trip_code1 |= TC_COND_CODE;
                }
            }
            else                                    
            {
                gss_prot2_cond_dirty_delaytimer = 0;
            }
        }
        else                                          //�б���    
        {
            if ((gss_TC - gss_THW) < (gss_factory_parm_value[DISP_FAC_r1]*10 - 30))    //10��   
            {
                gss_prot2_cond_dirty_delaytimer++;
                if (gss_prot2_cond_dirty_delaytimer >= ram_para2[num_prot2_cond_dirty_delaytime])   //300s
                {
                    gss_prot2_cond_dirty_delaytimer = 0;
                    
                    bflg_alarm_cond_dirty = 0;
                    gus_trip_code1 &= ~TC_COND_CODE;
                }
            }
            else
            {
                gss_prot2_cond_dirty_delaytimer = 0;
            }
        }
    }
}

/*****************************************************************************************************************************
��������: (������)��ѹ���걨���ĳ����ͻָ�
           ���������ﵽ����ֵ����ʾ��Ҳ�����б���ָʾ�ˣ�

����λ�ã���ѭ��----------------------------ʵ��δ��--------------------ok
******************************************************************************************************************************/
void prot2_volt_deal(void) 
{
   /* if (bflg_comp2_volt_type == 0) //�����220V��Դ
    {
        if ((gss_VAC <= ram_para2[num_prot2_volt_V1]) || (gss_VAC >= ram_para2[num_prot2_volt_V2]))     //184V    253V
        {
            bflg_alarm_volt = 1;
            gus_trip_code1 |= 0x8000;
        }
        else if ((gss_VAC >= ram_para2[num_prot2_volt_V3]) && (gss_VAC <= ram_para2[num_prot2_volt_V4])) //194V   243V
        {
            bflg_alarm_volt = 0;
            gus_trip_code1 &= ~0x8000;
        }
    }
    else                          //�����115V��Դ
    {
        if ((gss_VAC <= ram_para2[num_prot2_volt_V5]) || (gss_VAC >= ram_para2[num_prot2_volt_V6]))        //93V   136V
        {
            bflg_alarm_volt = 1;
            gus_trip_code1 |= 0x8000;
        }
        else if ((gss_VAC >= ram_para2[num_prot2_volt_V7]) && (gss_VAC <= ram_para2[num_prot2_volt_V8]))   //98V   131V
        {
            bflg_alarm_volt = 0;
            gus_trip_code1 &= ~0x8000;
        }
    }*/
}

/*********************************************************************************************************************
����λ�ã���ʾ���ϱ����ƿ��ƣ����б���ʱ�����ƶ���˸��
          �������ͣ������ı���+�Ƿ����ı��� ����flg_alarm1 + flg_alarm2��
          
����λ�ã���ѭ��----------------------------------------------ok    
*********************************************************************************************************************/
void warning_light_deal(void)              
{
    if((flg_alarm1.uword != 0) || (flg_alarm2.uword != 0))
    {
        bflg_alarm_light = 1;
    }
    else
    {
        bflg_alarm_light = 0;
    }
}

/******************************************************************************************************************************
��������: �����������Ĵ������ݱ���������ȷ���ǵε��죬���ǳ��죻

����λ�ã���ѭ������������������������ok
******************************************************************************************************************************/
void alarm_buzz_deal(void)     
{ 
    flag_type memory_temp;      
    
    if(bflg_com_type == 0)           //LED��
    {
        if (bflg_test_mode == 0)         //�ǲ���ģʽ     
        {
            memory_temp.uword = memory.uword;  //memory_temp�洢��һ�εĹ��� 
                                               //flg_alarm ���µĹ��ϣ������жϷ������ε���  
            memory.uword= flg_alarm1.uword;     //memory ��flg_alarm���µĹ��ϸ�����ֵ���������ϴεĴ洢�Ĺ���ֵ memory_temp�Աȣ��ж��Ƿ����µĹ��ϱ仯��������'����ȡ��'�����������30�ڲ���
        
           //--------------------------------------------------------------------------------
           
           if((flg_alarm1.uword  == 0)||                                        //û�б���
              (bflg_alarm_buzz_off_delaytime == 1))//||                         //�б�����������"����ȡ��"
              //((flg_alarm.uword == 0x0008 )&&(bflg_fac_bS_buz_valid == 1))) //ֻ�е��δ���ӱ����������õ��δ�������β���bS = 1)
            {                                                                   
                bflg_buzz_loop_tick_set = 0;                                   //����3��������ε�          
        
                if(memory_temp.uword != memory.uword)                          //���µı���ʱ���尴"����ȡ��"30min���죬�ֿ�ʼ��
                {
                    bflg_alarm_buzz_off_delaytime = 0;
                    gss_alarm_buzz_off_delaytimer = 0;
                }
            }           
            else
            {
                bflg_buzz_loop_tick_set = 1;                                    //��������ε���
            }
        
            //------------------------------------------------------------------------------------
            //��ز巴
            if (bflg_alarm_bat_fed == 1)               //peak ��ؽӷ�������־ 6
            {
                bflg_buzz_always_sound_set = 1;        //peak ����������
            }
            else
            {
                bflg_buzz_always_sound_set = 0;
            }   
        }
    }
    else
    {
        bflg_buzz_loop_tick_set = 0;
    }
}

/*******************************************************************************************************************************
����λ��:�ε���ʱ������buzz����ֹͣ30min�����ʱ��            
            
����λ��: ��1min��ʱ�����е���      -----------ok        
********************************************************************************/
void alarm_buzz_off_delaytime(void)     
{
    if (bflg_alarm_buzz_off_delaytime == 1)
    {
        gss_alarm_buzz_off_delaytimer++;
        //if (gss_alarm_buzz_off_delaytimer >= ram_para[num_alarm_buzz_off_delaytime])     //30����
        if (gss_alarm_buzz_off_delaytimer >= gss_factory_parm_value[DISP_FAC_Fd]*10)     //�ɵ�                          //��Ϊ�ɵ�
        {
            gss_alarm_buzz_off_delaytimer = 0;
            bflg_alarm_buzz_off_delaytime = 0;
        }
    }
}


/**********************************************************************************
�������� : ����¶ȹ��߱��� ,cn22��cn23�����ӵ�صĿڶ�һ��

            ������¶ȡ�65�棨EEP��ʱ��ֹͣ��س�ŵ磻 
            ������¶ȣ�55�棨EEP��ʱ���ָ���س�ŵ硣

��ע:  cn22 cn23�����ڶ������¶ȹ��ߵı�������ȻǦ����ֻ������û���¶ȴ�������
       ������û�н�ʱ��ad�ɼ�Ϊ1024����ʱ�¶�Ϊ0��������ߣ�

����λ��: ��ѭ����-----------------------------------------------ok
**********************************************************************************/
void bat_T_hight_protect(void)
{
    //bat1_CN22
    if (gss_Tbat1 >= ram_para2[num_bat2_discharge_off_T])       //65��   ���1 ��2���������׼
    {
        bflg_alarm_bat1_T_hight = 1;
    }
    else if (gss_Tbat1 <= ram_para2[num_bat2_discharge_on_T])   //55��
    {
        bflg_alarm_bat1_T_hight = 0;
    }

    //bat2_CN23
    //--------------------------------------------------------------------
    if (gss_Tbat2 >= ram_para2[num_bat2_discharge_off_T])       //65��
    {
        bflg_alarm_bat2_T_hight = 1;
    }
    else if (gss_Tbat2 <= ram_para2[num_bat2_discharge_on_T])   //55��
    {
        bflg_alarm_bat2_T_hight = 0;
    }
}

void errow_code(void)           //���ο�--δ��
{
    bflg_alarm_high_TUTD = 0;  //����---buzz
    bflg_alarm_low_TUTD = 0;   //����---buzz
    bflg_alarm_higt_THW = 0;   //���¹���
    bflg_alarm_high_AIR = 0;   //��������---buzz
    bflg_alarm_low_AIR = 0;    //��������---buzz
    bflg_alarm_higt_TE = 0;    //��˪(�Ƚ���)����������

    bflg_TA_fault = 0;         //���ƴ���������---buzz
    bflg_TE_fault = 0;         //��˪����������

    //���¡�ʪ����1��
    bflg_THW_fault = 0;        //���´���������
    bflg_humidity_fault = 0;   //ʪ�ȴ���������

    bflg_TC_fault = 0;         //��������������
    bflg_TU_fault = 0;         //�ϴ���������---buzz
    bflg_TD_fault = 0;         //�´���������---buzz


    bflg_alarm_bat_discnnt = 0;//���δ����
    bflg_alarm_bat_fed = 0;    //��ز巴
    bflg_alarm_bat_low = 0;    //��ص�����

    bflg_alarm_power_off = 0;  //�ϵ籨��---buzz
    bflg_alarm_power_fault = 0;//��Դ�����---buzz

    bflg_alarm_open_door = 0;  //���ű���---buzz
    bflg_alarm_cond_dirty = 0; //��������---buzz

    bflg_sterilize_fault = 0;  //ɱ��ģ�����

    //���ڻ��¹���
    bflg_alarm_too_higt_THW = 0; //���¹�����5��---buzz
}


//------------------------------------------------------------------------------
#endif
