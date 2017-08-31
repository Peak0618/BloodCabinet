/*******************************************************************************************************************************************
overview:
        1、上下温高、低温报警
        2、环温报警
        3、空气高、低温报警
        4、化霜传感器(热交换)高温报警
        5、220断电报警
        6、12v故障
        7、电池未连接
        8、电池电量低
        9、开门报警
        10、冷凝器脏报警
        11、
********************************************************************************************************************************************/
#ifndef _PROTECT_C_
#define _PROTECT_C_
//------------------------------------------------------------------------------
//包含文件声明
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
//函数声明
void protect_high_TUTD_delaytime(void);
void protect_low_TUTD_delaytime(void);
void protect_high_THW_delaytime(void);
void protect_air_alarm_boot_delaytime(void);
void protect_air_higt_delaytime(void);
void protect_air_low_delaytime(void);
void protect_TE_boot_delaytime();
void protect_TE_interval_delaytime();
void protect_higt_TE_delaytime();







void prot2_power_off_deal(void);   //断电保护处理程序，在主循环程序中调用

void port2_power_fault_deal(void); //电源板故障处理程序，在主循环程序中调用

void prot2_bat_discnnt_delaytime(void); //电池未连接保护延时程序，在100ms定时程序中调用

void prot2_bat_space_delaytime(void);   //电池未连接检测间隔延时程序，在100ms定时程序中调用

void prot2_bat_low_deal(void);     //电池电量低保护处理程序，在主循环程序中调用

void prot2_bat_low_delaytime(void);//电池电量低延迟程序，在1min定时程序中调用

void port2_door_delaytime(void);   //门开关保护延时程序，在1s定时程序中调用

void port2_T_fault_deal(void);     //传感器故障处理程序，在主循环程序中调用

void prot2_TC_first_delaytime(void);    //冷凝器脏首次保护延时程序，在1min定时程序中调用

void prot2_cond_dirty_delaytime(void);  //冷凝器脏保护延时程序，在1s定时程序中调用

void prot2_volt_deal(void);   //电压超标报警保护处理程序，在主循环程序中调用

void warning_light_deal(void);   //报警灯处理程序，在主循环程序中调用

void alarm_buzz_deal(void);   //蜂鸣器报警处理程序，在主循环程序中调用

void alarm_buzz_off_delaytime(void);    //蜂鸣器报警取消延时程序，在1min定时程序中调用

void bat_T_hight_protect(void);             //电池温度过高判断



//------------------------------------------------------------------------------
//变量定义
flag_type flg_prot1,flg_prot2,flg_alarm1,flg_alarm2;

//peak add
flag_type   memory;

//------------------------------------------------------------------------------

int16_t   gss_prot2_bat_discnnt_delaytimer;  //电池未连接保护延时计时器
int16_t   gss_prot2_bat_space_delaytimer;    //电池未连接检测间隔延时计时器
int16_t   gss_prot2_bat_low_delaytimer;      //电池电量低延时计时器

int16_t   gss_prot2_door_delaytimer;    //门开关保护延时计时器

int16_t   gss_prot2_TC_first_delaytimer;//冷凝器脏首次保护延时计时器
int16_t   gss_prot2_cond_dirty_delaytimer;   //冷凝器脏延时计时器

int16_t   gss_alarm_buzz_off_delaytimer;     //蜂鸣器报警取消延时计时器

uint16_t  gus_trip_code1;     //故障代码1
uint16_t  gus_trip_code2;     //故障代码2

int16_t   gss_TUTD_high_delaytimer;        //上下温高温报警延时
int16_t   gss_TUTD_low_delaytimer;         //上下温低温报警延时
int16_t   gss_THW_high_delaytimer;         //环温过高报警
int16_t   gss_air_alarm_boot_delaytime;    //空气报警开关延时
int16_t   gss_AIR_high_delaytimer;         //空气高温报警延时
int16_t   gss_AIR_low_delaytimer;          //空气低温报警延时
int16_t   gss_TE_boot_delaytime;           //化霜传感器高温判定上电延时
int16_t   gss_TE_interval_delaytime;       //化霜传感器高温判定间隔
int16_t   gss_TE_higt_delaytimer;          //化霜传感器高温报警
int16_t   gss_THW_too_high_delaytimer;     //环温过高于5℃开启蜂鸣报警


/**********************************************************************************************************************************
函数功能：高温报警:上下温任一路大于等于高温设定温度1min
                   
函数位置：1s----------------------------------------ok      
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
函数功能：低温报警:上下温任一路小于等于低温设定温度1min
                   
函数位置：1s----------------------------------------ok
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
函数功能：环温过高报警:环温>= AHH
                   
函数位置：1s----------------------------------------ok
**********************************************************************************************************************************/
void protect_high_THW_delaytime(void)
{
    if(bflg_alarm_higt_THW == 0)
    {
        if(gss_THW >= gss_factory_parm_value[DISP_FAC_AHH]*10)    //只闪烁
        {
            gss_THW_high_delaytimer++;
            if (gss_THW_high_delaytimer >= 30)   //30s
            {
                gss_THW_high_delaytimer = 0;
                bflg_alarm_higt_THW = 1;         //只是报警灯闪烁
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
    //如果环温传感器温度≥（AHH+5℃），则同时启动蜂鸣器报警
    if(bflg_alarm_too_higt_THW == 0)
    {
        if(gss_THW >= gss_factory_parm_value[DISP_FAC_AHH]*10 +50)   //蜂鸣
        {
            gss_THW_too_high_delaytimer++;
            if (gss_THW_too_high_delaytimer >= 30)   //30s
            {
                gss_THW_too_high_delaytimer = 0;
                bflg_alarm_too_higt_THW = 1;         //只蜂鸣报警
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
函数功能：空气报警延时
                   
函数位置：1s---------------------------------------ok
**********************************************************************************************************************************/
void protect_air_alarm_boot_delaytime()
{
    if(gss_factory_parm_value[DISP_FAC_Ad] != -1)     //未关闭
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
        gss_air_alarm_boot_delaytime = 0;        //关闭重启时重新计时
        bflg_air_alarm_boot_delaytime = 0;
    }
}



/**********************************************************************************************************************************
函数功能：空气高温报警
                   
函数位置：1s----------------------------------------ok
**********************************************************************************************************************************/
void protect_air_higt_delaytime(void)
{
    if((gss_factory_parm_value[DISP_FAC_Ad] != -1) && (bflg_air_alarm_boot_delaytime == 1)) //开启&延时到
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
        gss_AIR_high_delaytimer = 0;              //关闭时即无报警
        bflg_alarm_high_AIR = 0;
        gus_trip_code1 &= ~AIR_H_CODE;
    }
}

/**********************************************************************************************************************************
函数功能：空气低温报警
                   
函数位置：1s----------------------------------------ok
**********************************************************************************************************************************/
void protect_air_low_delaytime(void)
{
    if((gss_factory_parm_value[DISP_FAC_Ad] != -1) && (bflg_air_alarm_boot_delaytime == 1)) //开启&延时到
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
函数功能：化霜传感器(即热交换传感器)高温判定启动延时
                   
函数位置：1min----------------------------------------ok
**********************************************************************************************************************************/
void protect_TE_boot_delaytime(void)
{
    if(bflg_TE_boot_delaytime == 1)      //系统初始化时要设为1
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
函数功能：化霜传感器(即热交换传感器)高温判定的时间间隔
                   
函数位置：1min----------------------------------------ok
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
函数功能：化霜传感器（即热交换传感器）高温报警
                   
函数位置：1s----------------------------------------ok
**********************************************************************************************************************************/
void protect_higt_TE_delaytime(void)
{
    if(bflg_TE_boot_delaytime == 0)           //启动延时60min结束
    {
        if(bflg_alarm_higt_TE == 0)           //无故障
        {
            if(bflg_prot2_door1_pin == 1)     //开门
            {
                bflg_TE_interval_delaytime = 1;  //20min间隔
                gss_TE_interval_delaytime = 0;
            }
            else
            {
                if(bflg_TE_interval_delaytime == 0)
                {
                    if(gss_TE > 200)              //20℃
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
        else //有故障
        {
            if(gss_TE <= 180)              //18℃
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






//旧

/***************************************************************************************************************************
函数功能 : 220v断电报警的成立和恢复


函数位置：主循环-------------------------------ok
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
函数功能: 电源板(12v)故障的成立和恢复
          ————达到报警值，显示板也不进行报警指示了；

函数位置:   主循环-----------------------------ok
************************************************************************************************************************************/
void port2_power_fault_deal(void)     
{
    if (guc_12V < ram_para2[num_power_off_12V])  //9v
    {
        /*if (bflg_alarm_power_off == 0)           //未断强电  --原     
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
函数功能: 电池未连接故障的成立和恢复

函数位置：100ms--------------------------------------------------ok
*********************************************************************************************************************************************/
void prot2_bat_discnnt_delaytime(void)   
{
    if (bflg_bat_charging == 0)              //非充电状态
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
    else   //充电状态
    {
        if (bflg_alarm_bat_discnnt == 0)      
        {
            //if (guc_bat_Vout_instant < 90)//ram_para2[num_port2_bat_discnnt_V1]) //peak 功能书为 9v
            if (guc_bat_Vout_instant < 90)       
            {
                bflg_alarm_bat_discnnt = 1;
                gus_trip_code1 |= BAT_NO_CODE;
            }
            if(bflg_bat_type == LEAD_ACID_BAT)      //铅酸标准，锂电待+      
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
function: 电池未连接检测间隔延时程序----未用

函数位置：100ms-----------------------------ok
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
函数功能: 电池电量低故障的成立和恢复

恢复条件:1、电池未连接，2、电压>阀值  (3、在充电时不能检测已被去掉 )

函数位置：主循环-----------------------------------ok
******************************************************************************************************************************/
void prot2_bat_low_deal(void)       
{    
    if ((bflg_prot2_bat_low_delaytime == 0) && (bflg_alarm_bat_discnnt == 0))   //充电24小时后，且电池已连接
	{  
        if (bflg_bat_type == 0)    //如果是铅酸电池
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
        else   //如果是锂电池
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
函数功能: 初始上电充电24小时后再对电池电量低进行检测
         
函数位置： 1min-------------------ok         
********************************************************************************************************************************/
void prot2_bat_low_delaytime(void) //电池电量低延迟程序，在1min定时程序中调用
{
    if (bflg_prot2_bat_low_delaytime == 1)
    {
        gss_prot2_bat_low_delaytimer++;
        if (gss_prot2_bat_low_delaytimer >= (ram_para2[num_prot2_bat_low_delaytime] * 60)) //24小时
        {
            gss_prot2_bat_low_delaytimer = 0;
            bflg_prot2_bat_low_delaytime = 0;
        }
    }
}

/*****************************************************************************************************************************
function: 开门报警的成立和恢复，持续检测开门5min则报开门报警

函数位置： 1s---------------------------------ok
*****************************************************************************************************************************/
void port2_door_delaytime(void)    
{
    if ((bflg_prot2_door1_pin == 1))     //开门
    {
        if (bflg_alarm_open_door == 0)
        {
            gss_prot2_door_delaytimer++;
            //if (gss_prot2_door_delaytimer >= (ram_para[num_prot2_door_delaytime] * 60))  //5分钟
            if (gss_prot2_door_delaytimer >= (gss_user_manage_parm_value[DISP_USER_MANAGE_dA] * 60))        //5分钟 用可调的gss_user_parm_value[DISP_USER_dA]替换之前的固定值
            {
                gss_prot2_door_delaytimer = 0;
                
                bflg_alarm_open_door = 1; //开门报警
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
函数功能: 环温、热交换、冷凝、pt100传感器故障都统一称为 bflg_alarm_T_fault 故障;

函数位置：主循环---------------------------------ok
*******************************************************************************************************************************/
void port2_T_fault_deal(void)       
{
    if (bflg_THW_fault == 1)    // 环温报警
    {
        bflg_alarm_T_fault = 1;
    }
    else if (bflg_TE_fault == 1) // 热交换      
    {
        bflg_alarm_T_fault = 1;
    }
    else if (bflg_TC_fault == 1) // 冷凝器
    {
        bflg_alarm_T_fault = 1;
    }
    else if (bflg_TA_fault == 1) // 主传感器pt100
    {
        bflg_alarm_T_fault = 1;
    }
    else
    {
        bflg_alarm_T_fault = 0;
    }
}


/************************************************************************************************************************
函数功能: 上电2h后再进行判断冷凝器脏报警，冷凝器脏首次保护延时；

函数位置：1min--------------------------------------ok
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
函数功能: 冷凝器脏报警的成立和恢复

函数位置：1s-----------------------------------------------ok
********************************************************************************************************************************/
void prot2_cond_dirty_delaytime(void) 
{
    if (bflg_prot2_TC_first_delaytime == 0)          //初始上电已延时2小时
    {
        if (bflg_alarm_cond_dirty == 0)              
        {
            if ((gss_TC - gss_THW) >=  gss_factory_parm_value[DISP_FAC_r1]*10)   //13℃
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
        else                                          //有报警    
        {
            if ((gss_TC - gss_THW) < (gss_factory_parm_value[DISP_FAC_r1]*10 - 30))    //10℃   
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
函数功能: (交流电)电压超标报警的成立和恢复
           ————达到报警值，显示板也不进行报警指示了；

函数位置：主循环----------------------------实际未用--------------------ok
******************************************************************************************************************************/
void prot2_volt_deal(void) 
{
   /* if (bflg_comp2_volt_type == 0) //如果是220V电源
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
    else                          //如果是115V电源
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
函数位置：显示板上报警灯控制（所有报警时报警灯都闪烁）
          报警类型：蜂鸣的报警+非蜂鸣的报警 （即flg_alarm1 + flg_alarm2）
          
函数位置：主循环----------------------------------------------ok    
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
函数功能: 蜂鸣器报警的处理，根据报警的类型确定是滴滴响，还是长响；

函数位置；主循环－－－－－－－－－－－ok
******************************************************************************************************************************/
void alarm_buzz_deal(void)     
{ 
    flag_type memory_temp;      
    
    if(bflg_com_type == 0)           //LED屏
    {
        if (bflg_test_mode == 0)         //非测试模式     
        {
            memory_temp.uword = memory.uword;  //memory_temp存储上一次的故障 
                                               //flg_alarm 最新的故障，用来判断蜂鸣器滴滴响  
            memory.uword= flg_alarm1.uword;     //memory 将flg_alarm最新的故障赋给它值，用来和上次的存储的故障值 memory_temp对比，判断是否有新的故障变化，若按下'蜂鸣取消'键，则可以清30内不响
        
           //--------------------------------------------------------------------------------
           
           if((flg_alarm1.uword  == 0)||                                        //没有报警
              (bflg_alarm_buzz_off_delaytime == 1))//||                         //有报警，但按下"蜂鸣取消"
              //((flg_alarm.uword == 0x0008 )&&(bflg_fac_bS_buz_valid == 1))) //只有电池未连接报警，且设置电池未连接屏蔽参数bS = 1)
            {                                                                   
                bflg_buzz_loop_tick_set = 0;                                   //以上3种情况不滴滴          
        
                if(memory_temp.uword != memory.uword)                          //有新的报警时，清按"蜂鸣取消"30min不响，又开始响
                {
                    bflg_alarm_buzz_off_delaytime = 0;
                    gss_alarm_buzz_off_delaytimer = 0;
                }
            }           
            else
            {
                bflg_buzz_loop_tick_set = 1;                                    //其他情况滴滴响
            }
        
            //------------------------------------------------------------------------------------
            //电池插反
            if (bflg_alarm_bat_fed == 1)               //peak 电池接反报警标志 6
            {
                bflg_buzz_always_sound_set = 1;        //peak 蜂鸣器长响
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
函数位置:滴滴响时，按下buzz键，停止30min响的延时；            
            
函数位置: 1min定时程序中调用      -----------ok        
********************************************************************************/
void alarm_buzz_off_delaytime(void)     
{
    if (bflg_alarm_buzz_off_delaytime == 1)
    {
        gss_alarm_buzz_off_delaytimer++;
        //if (gss_alarm_buzz_off_delaytimer >= ram_para[num_alarm_buzz_off_delaytime])     //30分钟
        if (gss_alarm_buzz_off_delaytimer >= gss_factory_parm_value[DISP_FAC_Fd]*10)     //可调                          //改为可调
        {
            gss_alarm_buzz_off_delaytimer = 0;
            bflg_alarm_buzz_off_delaytime = 0;
        }
    }
}


/**********************************************************************************
功能描述 : 电池温度过高保护 ,cn22和cn23两个接电池的口都一样

            当电池温度≥65℃（EEP）时，停止电池充放电； 
            当电池温度＜55℃（EEP）时，恢复电池充放电。

备注:  cn22 cn23两个口都作了温度过高的保护，虽然铅酸电池只有正负没有温度传感器，
       但是在没有接时候ad采集为1024，此时温读为0，不会过高；

函数位置: 主循环中-----------------------------------------------ok
**********************************************************************************/
void bat_T_hight_protect(void)
{
    //bat1_CN22
    if (gss_Tbat1 >= ram_para2[num_bat2_discharge_off_T])       //65℃   电池1 、2都用这个标准
    {
        bflg_alarm_bat1_T_hight = 1;
    }
    else if (gss_Tbat1 <= ram_para2[num_bat2_discharge_on_T])   //55℃
    {
        bflg_alarm_bat1_T_hight = 0;
    }

    //bat2_CN23
    //--------------------------------------------------------------------
    if (gss_Tbat2 >= ram_para2[num_bat2_discharge_off_T])       //65℃
    {
        bflg_alarm_bat2_T_hight = 1;
    }
    else if (gss_Tbat2 <= ram_para2[num_bat2_discharge_on_T])   //55℃
    {
        bflg_alarm_bat2_T_hight = 0;
    }
}

void errow_code(void)           //仅参考--未用
{
    bflg_alarm_high_TUTD = 0;  //高温---buzz
    bflg_alarm_low_TUTD = 0;   //低温---buzz
    bflg_alarm_higt_THW = 0;   //环温过高
    bflg_alarm_high_AIR = 0;   //空气高温---buzz
    bflg_alarm_low_AIR = 0;    //空气低温---buzz
    bflg_alarm_higt_TE = 0;    //化霜(热交换)传感器高温

    bflg_TA_fault = 0;         //控制传感器故障---buzz
    bflg_TE_fault = 0;         //化霜传感器故障

    //环温、湿度是1个
    bflg_THW_fault = 0;        //环温传感器故障
    bflg_humidity_fault = 0;   //湿度传感器故障

    bflg_TC_fault = 0;         //冷凝传感器故障
    bflg_TU_fault = 0;         //上传感器故障---buzz
    bflg_TD_fault = 0;         //下传感器故障---buzz


    bflg_alarm_bat_discnnt = 0;//电池未连接
    bflg_alarm_bat_fed = 0;    //电池插反
    bflg_alarm_bat_low = 0;    //电池电量低

    bflg_alarm_power_off = 0;  //断电报警---buzz
    bflg_alarm_power_fault = 0;//电源板故障---buzz

    bflg_alarm_open_door = 0;  //开门报警---buzz
    bflg_alarm_cond_dirty = 0; //冷凝器脏---buzz

    bflg_sterilize_fault = 0;  //杀菌模块故障

    //属于环温过高
    bflg_alarm_too_higt_THW = 0; //环温过高于5℃---buzz
}


//------------------------------------------------------------------------------
#endif
