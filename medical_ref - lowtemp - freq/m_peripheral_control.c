/*************************************************************************************************************
overview:20170315
    此文件为控制外围的负载； 1、强制化霜       --只是开停压机 
                             2、电加热控制     --CN29即门体
                             3、内风机控制     --CN29
                             4、冷凝器风机控制 --CN30
                             5、照明灯控制     --CN17
                             6、远程报警灯输出--CN28
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
//函数声明
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


//宏定义
#define   DOOR_IN_PIN                P5.2       //门开关1输入口   H_open L_close
#define   INSIDE_FAN_RL_OUT_PIN      P7.3       //内部风机输出口    
#define   CONDENSATE_FAN_RL_OUT_PIN  P13.0      //冷凝风机输出口
#define   HEAT_RL_OUT_PIN            P4.2       //电加热(门体)输出 RL6
#define   LED_RL_OUT_PIN             P14.6      //LED输出口

#define   DOOR_OPEN                  1          
#define   DOOR_CLOSE                 0     
//标志定义
flag_type flg_peripheral,flg_peripheral2;

//变量
uint8_t  guc_defrosting_time;                  //化霜时间计时
uint16_t guc_defrosting_interval_time;         //化霜间隔时间计时
uint8_t  guc_led_on_delaytime;                 //led开灯延时
uint8_t  guc_humidity;                         //湿度
uint8_t  guc_remote_alarm_test_delaytime;      //按下“测试报警”按键远程报警延时xs后正常



//化霜控制
/**********************************************************************************************************************************************
函数功能: 化霜状态的进入退出__化霜只是压机停机

函数位置:  主循环-------------------------------------ok
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
函数功能: 化霜时间延时

函数位置:  1min定时器-------------------------------------ok
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
函数功能: 化霜间隔时间延时，强制化霜在相邻的12小时内最多进行一次

函数位置:  1min定时器-------------------------------------ok
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

//电加热(门体加热丝)控制
/**********************************************************************************************************************************************
函数功能: 电加热的控制  humidity湿度  humiture温湿度  temperature

函数位置: 主循环 -------------------------------------ok
***********************************************************************************************************************************************/
void door_heat_control(void)
{
    if(bflg_HT_fault == 1)   //温湿度传感器故障
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
函数功能:  电加热丝管脚的输出

函数位置: 主循环 -------------------------------------ok
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


//风机控制
/**********************************************************************************************************************************************
函数功能: 内风机控制


函数位置: 主循环中----------------------------------------------ok
***********************************************************************************************************************************************/
void inside_fan_control(void)
{
    if(bflg_prot2_door1_pin == DOOR_OPEN)  //开门
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
函数功能: 风机输出: 内风机 + 冷凝风机(随压机开停)


函数位置: 主循环----------------------------------------------ok
***********************************************************************************************************************************************/
void fan_RL_out(void)
{
    //内风机
    if(bflg_inside_fan_runing == 1)
    {
        INSIDE_FAN_RL_OUT_PIN = 1;
    }
    else
    {
        INSIDE_FAN_RL_OUT_PIN = 0;
    }

    //--------------------------------------
    
    //冷凝风机
    if(bflg_compressor_runing == 1)
    {
        CONDENSATE_FAN_RL_OUT_PIN = 1;
    }
    else
    {
        CONDENSATE_FAN_RL_OUT_PIN= 0;
    }
}


//LED照明灯控制
/**********************************************************************************************************************************************
函数功能: LED灯的控制: 按键(优先级最高) + 开关门控制

函数位置:  主循环(按键延时中)----------------------------------------------ok
***********************************************************************************************************************************************/
void led_control(void)
{
    if(bflg_com_type == 0) //LED屏
    {
        if(gss_factory_parm_value[DISP_FAC_LC] == 1)   //默认玻璃门
        {
            if(bflg_led_on == 1)                        //灯亮
            {
                if(guc_key_val == KEY_LAMP)
                {
                    bflg_led_on = 0;                  //关灯
                    
                    bflg_led_on_delaytime = 0;       //清开灯延时标志
                    guc_led_on_delaytime = 0;
                    bflg_key_open_led = 0;           //清按键开的灯标志
                    bflg_key_close_led = 1;          //按键关灯
                }
                else
                {
                    if(DOOR_IN_PIN == DOOR_CLOSE)
                    {
                        if(gss_user_manage_parm_value[DISP_USER_MANAGE_Lt] == -1)   //关闭延时关灯功能
                        {
                            if(bflg_key_open_led == 0)  //不是按键开的灯
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
                    bflg_led_on = 1;            //开灯
                    
                    bflg_led_on_delaytime = 1;  //开灯延时标志
                    guc_led_on_delaytime = 0;
                    bflg_key_open_led = 1;      //按键开的灯标志
                    bflg_key_close_led = 0;     //清按键关灯标志
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
    /*else                   //液晶屏
    {
        if(gss_liquid_led_button == 1) 
        {
            bflg_led_on = 1;                  //开灯
        }
        else
        {
            bflg_led_on = 0;                  //关灯
        }
    } */
}
/**********************************************************************************************************************************************
函数功能: 液晶屏控制开灯，若在LED中，因LED按键需要延时检测，会造成液晶屏开灯时有延时

函数位置:     主循环---------------------------------------------no
***********************************************************************************************************************************************/
void liquid_led_control(void)
{
    if(bflg_com_type == 1) //液晶屏
    {
        if(gss_liquid_led_button == 1) 
        {
            bflg_led_on = 1;                  //开灯
        }
        else
        {
            bflg_led_on = 0;                  //关灯
        }
    } 
}

/**********************************************************************************************************************************************
函数功能: LED按键开灯延时

函数位置:  1min----------------------------------------------ok
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
函数功能: LED灯输出口

函数位置:  主循环----------------------------------------------ok
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
函数功能: 远程报警灯

函数位置:  主循环----------------------------------------------ok
***********************************************************************************************************************************************/
void remote_alarm_lamp(void)
{
    if(bflg_remote_alarm_test == 0)
    {
        if ((bflg_alarm_high_TUTD == 1)  ||  //高温
            (bflg_alarm_low_TUTD == 1)   ||  //低温
            (bflg_alarm_high_AIR == 1)   ||  //空气高温
            (bflg_alarm_low_AIR == 1)    ||  //空气低温
            (bflg_TA_fault == 1)         ||  //控制传感器故障
            (bflg_TU_fault == 1)         ||  //上传感器故障
            (bflg_TD_fault == 1)         ||  //下传感器故障
            (bflg_alarm_power_off == 1)  ||  //断电报警
            (bflg_alarm_power_fault == 1)||  //电源板故障
            (bflg_alarm_open_door == 1))     //开门报警
        {
            bflg_alarm_output = 1; //通讯传输状态
            RL_ALM_OUT_PIN = 0;    //开  海尔这么要求报警时 = 0；
        }
        else
        {
            bflg_alarm_output = 0;
            RL_ALM_OUT_PIN = 1;    //关
        }
    }
    else
    {
        RL_ALM_OUT_PIN = 0;    
    }

}

/**************************************************************************************************************************************************************
函数功能：按下"测试报警"按键时远程报警灯延时3时恢复正常


函数位置：1s----------------------------------------ok
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
