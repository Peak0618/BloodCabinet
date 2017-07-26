/**************************************************************************************************** 
overview:20170315
         此文件是对压缩机的控制；1、压机的启停
                                 2、定频的控制
                                 3、变频的转速控制   
*****************************************************************************************************/
//------------------------------------------------------------------------------------------
//包含的文件
#include "r_cg_macrodriver.h"
#include "r_cg_cgc.h"
#include "r_cg_port.h"

#include "h_type_define.h"
#include "h_ad.h"
#include "h_key_disp.h"
#include "m_compressor.h"
#include "h_rom_para2.h"
#include "m_key_operation.h"
#include "h_protect.h"
//------------------------------------------------------------------------------------------
//函数声明
void compressor_first_power_delaytime(void);
void compressor_start_stop_control(void);
void compressor_continue_run_delaytime(void);
void compressor_RL_out(void);
void coompressor_stop_delaytime(void);
void coompressor_force_stop_delaytime(void); 
void compressor_freq_ctrl_deal(void);   
void compressor_freq_start_delaytime(void);
void compressor_init_freq_ctrl(void);
void comp2_update_delaytime(void);
void compressor_up_down_stop(void);



//------------------------------------------------------------------------------------------
//宏定义
#define   COMPRESSOR_RL_OUT_PIN     P11.1      //压缩机1继电器输出口 原来的门控制口    
      

//------------------------------------------------------------------------------------------
//标志定义
flag_type bflg_compressor;


//------------------------------------------------------------------------------------------
//变量定义 
int16_t  gss_compressor_freq;                    //压机转速
int16_t  gss_compressor_start_delaytimer;        //变频启动时延时
int16_t  gss_compressor_update_delaytimer;
int16_t  gss_compressor_delt_freq;
uint16_t gus_compressor_freq_cnt;               //定时器的计数周期.决定频率f=1/T
uint8_t  guc_compressor_first_power_delaytime;  //初始上电延时
uint16_t guc_compressor_fault_runing_time;      //主传感器错误开停循环时间
uint8_t  guc_compressor_stop_delaytime;         //压机停机时间  放到分定时
uint8_t  guc_compressor_force_stop_delaytime;   //压机强制停机时间 放到分定时
uint8_t  guc_compressor_continue_delaytime;     //压机连续运行时间计时

int16_t  gss_compressor_TS;                     //箱内温度设定值
int16_t  gss_factory_parameter_r0;              //工厂参数中的控制温差

int16_t   gss_para[28];



/**********************************************************************************************************************************************
函数功能: 初次上电压机开机延时(默认1min)工厂参数

函数位置: 1min定时器----------------------------------ok
***********************************************************************************************************************************************/
void compressor_first_power_delaytime(void)
{
    if(bflg_compressor_first_power_delaytime == 1)
    {
        guc_compressor_first_power_delaytime++;
        if(guc_compressor_first_power_delaytime >= gss_factory_parm_value[DISP_FAC_Cd]) //min         
        {
            bflg_compressor_first_power_delaytime = 0;
            guc_compressor_first_power_delaytime = 0;
        }
    }
}

/**********************************************************************************************************************************************
函数功能: 压机启停控制

函数位置: 1s定时器----------------------------------ok
***********************************************************************************************************************************************/
void compressor_start_stop_control(void)
{
    if(bflg_TA_fault == 1)
    {
        guc_compressor_fault_runing_time++;
        if(guc_compressor_fault_runing_time >= 300)  //5min*60=300
        {
            guc_compressor_fault_runing_time = 0;
            
            if(bflg_compressor_runing == 0)
            {
                bflg_compressor_runing = 1;
            }
            else
            {
                bflg_compressor_runing = 0;
            }
        }
    }
    else
    {
        if(bflg_compressor_runing == 1)               //开机
        {
            if(bflg_defrosting_status == 1)           //强制化霜状态
            {
                bflg_compressor_runing = 0;           
                bflg_compressor_stop_delaytime = 1;
                guc_compressor_stop_delaytime = 0;
            }
            else if(bflg_compressor_continue == 1)    //连续运行4h
            {
                bflg_compressor_continue = 0;
                bflg_compressor_runing = 0;
                bflg_compressor_force_stop_delaytime = 1;
                guc_compressor_force_stop_delaytime = 0;
            }
            else if(bflg_up_down_stop == 1)           //因为上下温停机
            {
                bflg_up_down_stop = 0;
                
                bflg_compressor_runing = 0;           
                bflg_compressor_stop_delaytime = 1;
                guc_compressor_stop_delaytime = 0;
            }
            else if(gss_TA <= (gss_user_set_parm_value[DISP_USER_SET_TS] - gss_factory_parm_value[DISP_FAC_r0])) //正常开停点
            {
                bflg_compressor_runing = 0;
                bflg_compressor_stop_delaytime = 1;
                guc_compressor_stop_delaytime = 0;
            }
        }
        else
        {
            if((bflg_defrosting_status == 0) && (gss_TU >=20) && (gss_TD >= 20))  //未化霜状态且见“流程图”
            {
                if(gss_TA >= (gss_user_set_parm_value[DISP_USER_SET_TS] + gss_factory_parm_value[DISP_FAC_r0]))
                {
                    if((bflg_compressor_stop_delaytime == 0)&&(bflg_compressor_force_stop_delaytime == 0))
                    {
                        bflg_compressor_runing = 1;
                        bflg_compressor_continue_delaytime = 1;
                        guc_compressor_continue_delaytime = 0;
                    }
                }
            }
        }
    }
}
/**********************************************************************************************************************************************
函数功能；压机连续运行4h强制停机
         

函数位置:  1min定时器---------------------------------ok
***********************************************************************************************************************************************/
void compressor_continue_run_delaytime(void)
{
    if(bflg_compressor_runing == 1)
    {
        if(bflg_compressor_continue_delaytime == 1)
        {
            guc_compressor_continue_delaytime++;
            if(guc_compressor_continue_delaytime > 240)  //60min * 4h = 240
            {
                guc_compressor_continue_delaytime = 0;
                bflg_compressor_continue_delaytime = 0;
        
                bflg_compressor_continue = 1;
            }
        }
    }
    else
    {
        guc_compressor_continue_delaytime = 0;
        bflg_compressor_continue_delaytime = 0;
        bflg_compressor_continue = 0;
    }
}

/**********************************************************************************************************************************************
函数功能: 定频压机实际端口输出
         

函数位置:  主循环---------------------------------ok
***********************************************************************************************************************************************/
void compressor_RL_out(void)
{
    if(bflg_compressor_runing == 1)
    {
        COMPRESSOR_RL_OUT_PIN = 1;
    }
    else
    {
        COMPRESSOR_RL_OUT_PIN = 0;
    }
}

/**********************************************************************************************************************************************
函数功能: 压机最短停机时间
         

函数位置:  1min定时中---------------------------------ok
***********************************************************************************************************************************************/
void coompressor_stop_delaytime(void) 
{
    if(bflg_compressor_stop_delaytime == 1)
    {
        guc_compressor_stop_delaytime++;
        if(guc_compressor_stop_delaytime >= gss_factory_parm_value[DISP_FAC_T3])   
        {
            guc_compressor_stop_delaytime = 0;
            bflg_compressor_stop_delaytime = 0;
        }
    }
}
/**********************************************************************************************************************************************
函数功能: 压机强制停机时间
         

函数位置:  1min定时中---------------------------------ok
***********************************************************************************************************************************************/
void coompressor_force_stop_delaytime(void) 
{
    if(bflg_compressor_force_stop_delaytime == 1)
    {
        guc_compressor_force_stop_delaytime++;
        if(guc_compressor_force_stop_delaytime >= 5)  //5min
        {
            guc_compressor_force_stop_delaytime = 0;
            bflg_compressor_force_stop_delaytime = 0;
        }
    }
}

/**********************************************************************************************************************************************
函数功能: 变频压机转速控制。高温压缩机、低温压缩机的启动、停止条件与定频机方式一样。
         

函数位置:  1s定时中--------------------------------ok
***********************************************************************************************************************************************/
/*
void compressor_freq_ctrl_deal(void)            
{
    int32_t lss_tmp;
    
    //----------------------------------                            //低温压机的控制
    if (bflg_compressor_runing == 0)                                //低温压机未允许启动   
    {
        if (gss_compressor_freq != 0)
        {
            gss_compressor_freq = 0;
        }
    }
    else                                                                //低温压机允许启动
    {
        if (gss_compressor_freq == 0)                                    //低温压机还未启动
        {
            gss_compressor_freq = gss_para[COMP21_START_FREQ];           //起始转速
            
            bflg_compressor_start_delaytime = 1;                          //刚启动时已2000rpm运行1分钟
            gss_compressor_start_delaytimer = 0;
        }
        else                                                         //低温压机已经启动
        {
            if (bflg_compressor_start_delaytime == 0)                //刚启动时已2000rpm运行1分钟结束
            {
                if((gss_TA - gss_user_set_parm_value[DISP_USER_SET_TS]) > 50)                  //s1  5℃
                {

                    gss_compressor_freq = gss_para[COMP21_FREQ_MAX];     //peak 最大转速
                    bflg_compressor_full_speed = 1;                                //全速运行标志
                }
                else 
                {
                    if(bflg_compressor_full_speed == 1)                        //在全速运行
                    {   
                        if((gss_TA - gss_user_set_parm_value[DISP_USER_SET_TS] - 5 ) <= 0)      // s2 0.5℃ 达到退出全速的条件
                        {
                            bflg_compressor_full_speed = 0;
                            gss_compressor_freq = gss_para[COMP21_FREQ_SET];      //以设定的转速转、、、
                        }                   
                    }
                    else
                    {
                        if (bflg_compressor_update_delaytime == 0)           //每80s 进行判断更新一次转速
                        {
                            bflg_compressor_update_delaytime = 1; 
                            gss_compressor_update_delaytimer = 0;
                            
                            lss_tmp = gss_TA;                                //计算公式
                            lss_tmp -= gss_user_set_parm_value[DISP_USER_SET_TS];
                            lss_tmp += (gss_para[COMP21_FREQ_DELT_TEMP] * 10);//公式中是 减-------?
                            lss_tmp *= gss_para[COMP21_FREQ_DELT_K];
                            lss_tmp /= 10;                                   //因为和PT100相关，之前被放大10倍，所以此处/10,之前高温压机的不用，是因为用的热交换器的温度
                            
                            if(lss_tmp < gss_para[COMP21_FREQ_DELT_MIN])     //范围 -20 ~ 20
                            {
                                lss_tmp = gss_para[COMP21_FREQ_DELT_MIN];
                            }
                            
                            if (lss_tmp > gss_para[COMP21_FREQ_DELT_MAX])     
                            {
                                lss_tmp = gss_para[COMP21_FREQ_DELT_MAX];
                            }
                            
                            gss_compressor_delt_freq = (int16_t) (lss_tmp);       
                            gss_compressor_freq += gss_compressor_delt_freq;           //求得最后的结论
                            
                            if (gss_compressor_freq >= gss_para[COMP21_FREQ_MAX])      //转速范围 2000 ~ 4500
                            {
                                gss_compressor_freq = gss_para[COMP21_FREQ_MAX];
                            }
                            if (gss_compressor_freq <= gss_para[COMP21_FREQ_MIN])
                            {
                                gss_compressor_freq = gss_para[COMP21_FREQ_MIN];
                            }
                            //--------------------------------------
                        }
                    }
                }
            }
        }
    }
    //----------------------------------
    if (gss_compressor_freq == 0)
    {
    	  gus_compressor_freq_cnt = (60000 - 1);                               //cnt越大，周期越大，频率越小；
    }
    else
    {
    	  gus_compressor_freq_cnt = (uint16_t) (60000000 / gss_compressor_freq);   // 3改为---> 6
    }
}
*/

/**********************************************************************************************************************************************
函数功能: 变频压机转速控制-----新的变频方式
         

函数位置:  1s定时中--------------------------------ok
***********************************************************************************************************************************************/
void compressor_freq_ctrl_deal(void)  
{
    if (bflg_compressor_runing == 0)                                  
    {
        if (gss_compressor_freq != 0)
        {
            gss_compressor_freq = 0;
        }
    }
    else
    {
        if (gss_compressor_freq == 0)                                    //低温压机还未启动
        {
            gss_compressor_freq = gss_para[COMP21_START_FREQ];           //起始转速
            
            bflg_compressor_start_delaytime = 1;                         //刚启动时已1200rpm运行1分钟
            gss_compressor_start_delaytimer = 0;
        }
        else
        {
            if (bflg_compressor_start_delaytime == 0)                    //刚启动已2000rpm运行1min
            {
                if(bflg_compressor_update_delaytime == 0)
                {
                    bflg_compressor_update_delaytime = 1; 
                    gss_compressor_update_delaytimer = 0;
                    
                    if((bflg_THW_fault == 1) || (bflg_TA_fault == 1))     //环温/pt100故障都跑这档
                    {
                        gss_compressor_freq = 10*gss_factory_parm_value[DISP_FAC_br3];
                    }
                    else
                    {
                        if( gss_TA - gss_user_set_parm_value[DISP_USER_SET_TS] > gss_factory_parm_value[DISP_FAC_b0F]*10)
                        {
                            gss_compressor_freq = gss_para[COMP21_FREQ_MAX];           //max
                        }
                        else
                        {
                            if(gss_THW <= 160)
    //16℃
                            {
                                gss_compressor_freq = 10*gss_factory_parm_value[DISP_FAC_br1];
                            }
                            else if((gss_THW > 160)&&(gss_THW <= 250))
                            {
                                gss_compressor_freq = 10*gss_factory_parm_value[DISP_FAC_br2];
                            }
                            else if((gss_THW > 250)&&(gss_THW <= 320))
                            {
                                gss_compressor_freq = 10*gss_factory_parm_value[DISP_FAC_br3];
                            }
                            else if((gss_THW > 320))
                            {
                                gss_compressor_freq = 10*gss_factory_parm_value[DISP_FAC_br4];
                            }                    
                        }
                    }
                }
            }
        }
    }        //用的定时器是：1_3
    if (gss_compressor_freq == 0)               //6000对应最小输出频率为33hz,因底层设置15ms方波输出
    {
        gus_compressor_freq_cnt = (40000 - 1);  //60000 --->40000                             //cnt越大，周期越大，频率越小；
    }
    else
    {
        gus_compressor_freq_cnt = (uint16_t) (30000000 / gss_compressor_freq);   // 3改为---> 6-----又3了
    }                    
}




/**********************************************************************************************************************************************
函数功能: 变频压机以起始转速启动60s延时


函数位置:  1s定时------------------------------ok
***********************************************************************************************************************************************/
void compressor_freq_start_delaytime(void)
{
    if (bflg_compressor_start_delaytime == 1)
    {
        gss_compressor_start_delaytimer++;
        if (gss_compressor_start_delaytimer >= 60) //60s
        {
            gss_compressor_start_delaytimer = 0;
            bflg_compressor_start_delaytime = 0;
        }
    }
}

/**********************************************************************************************************************************************
函数功能: 变频控制时使用的一些参数


函数位置:  初始化中------------------------------ok
***********************************************************************************************************************************************/
void compressor_init_freq_ctrl(void)
{                                                                                  
    gss_para[COMP2_START_FREQ]      = 2000;     // P.00   
    gss_para[COMP2_START_TIME]      = 60;       // P.01   
    gss_para[COMP2_UPDATE_TIME]     = 80;       // P.02   
    gss_para[COMP2_SET_TEMP1]       = -27;      // P.03   
    gss_para[COMP2_FREQ_DELT_K1]    = 50;       // P.04   
    gss_para[COMP2_FREQ_DELT_MIN]   =  -20;     // P.05  
    gss_para[COMP2_FREQ_DELT_MAX]   = 20;       // P.06   
    gss_para[COMP2_FREQ_MIN]        = 2000;     // P.07   
    gss_para[COMP2_FREQ_MAX]        = 4500;     // P.08   
    gss_para[COMP2_SET_TEMP2]       = -25;      // P.09   
    gss_para[COMP2_FREQ_DELT_K2]    = 50;       // P.10   
    
    gss_para[COMP21_START_FREQ]     = 1200;     // P.11   
    gss_para[COMP21_START_TIME]     =  60;      // P.12  
    gss_para[COMP21_UPDATE_TIME]    = 80;       // P.13   
    gss_para[COMP21_FREQ_DELT_K]    = 50;       // P.14   
    gss_para[COMP21_FREQ_DELT_MIN]  = -2000;    // P.15   
    gss_para[COMP21_FREQ_DELT_MAX]  = 2000;     // P.16   
    gss_para[COMP21_FREQ_MIN]       = 1600;     // P.17   
    gss_para[COMP21_FREQ_MAX]       = 4300;     // P.18   
    gss_para[COMP21_FREQ_DELT_TEMP] = 0;        // P.19   
    gss_para[COMP_START_DELT_TEMP]  = 10;       // P.20   
    gss_para[COMP_STOP_DELT_TEMP]   = -60;      // P.21   
    gss_para[PT100_DELT_TEMP]       = 0;        // P.22   
    gss_para[COMP_STOP_DELAYTIME]   = 480;      // P.23    
    gss_para[COMP_DOOR_TDELT]       = 50;       // P.24   
    gss_para[COMP_DOOR_FREQ]        = 3500;     // P.25   
    gss_para[COMP_RUN_DELAYTIME]    = 600;      // P.26   
    gss_para[COMP21_FREQ_SET]       = 1600;     // P.27   

   /* 这些值存E2时                                                                      //默认值
    gss_para[COMP2_START_FREQ]      = ram_para[num_COMP2_START_FREQ];       // P.00   2000
    gss_para[COMP2_START_TIME]      = ram_para[num_COMP2_START_TIME];       // P.01   60
    gss_para[COMP2_UPDATE_TIME]     = ram_para[num_COMP2_UPDATE_TIME];      // P.02   80
    gss_para[COMP2_SET_TEMP1]       = ram_para[num_COMP2_SET_TEMP1];        // P.03   -27
    gss_para[COMP2_FREQ_DELT_K1]    = ram_para[num_COMP2_FREQ_DELT_K1];     // P.04   50
    gss_para[COMP2_FREQ_DELT_MIN]   = ram_para[num_COMP2_FREQ_DELT_MIN];    // P.05   -20
    gss_para[COMP2_FREQ_DELT_MAX]   = ram_para[num_COMP2_FREQ_DELT_MAX];    // P.06   20
    gss_para[COMP2_FREQ_MIN]        = ram_para[num_COMP2_FREQ_MIN];         // P.07   2000
    gss_para[COMP2_FREQ_MAX]        = ram_para[num_COMP2_FREQ_MAX];         // P.08   4500
    gss_para[COMP2_SET_TEMP2]       = ram_para[num_COMP2_SET_TEMP2];        // P.09   -25
    gss_para[COMP2_FREQ_DELT_K2]    = ram_para[num_COMP2_FREQ_DELT_K2];     // P.10   50
    
    gss_para[COMP21_START_FREQ]     = ram_para[num_COMP21_START_FREQ];      // P.11   1600
    gss_para[COMP21_START_TIME]     = ram_para[num_COMP21_START_TIME];      // P.12   60
    gss_para[COMP21_UPDATE_TIME]    = ram_para[num_COMP21_UPDATE_TIME];     // P.13   80
    gss_para[COMP21_FREQ_DELT_K]    = ram_para[num_COMP21_FREQ_DELT_K];     // P.14   50
    gss_para[COMP21_FREQ_DELT_MIN]  = ram_para[num_COMP21_FREQ_DELT_MIN];   // P.15   -2000
    gss_para[COMP21_FREQ_DELT_MAX]  = ram_para[num_COMP21_FREQ_DELT_MAX];   // P.16   2000
    gss_para[COMP21_FREQ_MIN]       = ram_para[num_COMP21_FREQ_MIN];        // P.17   1600
    gss_para[COMP21_FREQ_MAX]       = ram_para[num_COMP21_FREQ_MAX];        // P.18   4300
    gss_para[COMP21_FREQ_DELT_TEMP] = ram_para[num_COMP21_FREQ_DELT_TEMP];  // P.19   0
    gss_para[COMP_START_DELT_TEMP]  = ram_para[num_COMP_START_DELT_TEMP];   // P.20   10
    gss_para[COMP_STOP_DELT_TEMP]   = ram_para[num_COMP_STOP_DELT_TEMP];    // P.21   -60
    gss_para[PT100_DELT_TEMP]       = ram_para[num_PT100_DELT_TEMP];        // P.22   0
    gss_para[COMP_STOP_DELAYTIME]   = ram_para[num_COMP_STOP_DELAYTIME];    // P.23   480 
    gss_para[COMP_DOOR_TDELT]       = ram_para[num_COMP_DOOR_TDELT];        // P.24   50
    gss_para[COMP_DOOR_FREQ]        = ram_para[num_COMP_DOOR_FREQ];         // P.25   3500
    gss_para[COMP_RUN_DELAYTIME]    = ram_para[num_COMP_RUN_DELAYTIME];     // P.26   600
    gss_para[COMP21_FREQ_SET]       = ram_para[num_COMP21_FREQ_SET];        // P.27   1600
    */
}

/*********************************************************************************************************************************************************
函数功能：压机的转速每间隔80s进行一次更新

函数位置：1s定时器--------------------------------------ok
*********************************************************************************************************************************************************/
void comp2_update_delaytime(void)
{
    if (bflg_compressor_update_delaytime == 1)
    {
        gss_compressor_update_delaytimer++;
        if (gss_compressor_update_delaytimer >= 60)//60s
        {
            gss_compressor_update_delaytimer = 0;
            bflg_compressor_update_delaytime = 0;
        }
    }
}
/*********************************************************************************************************************************************************
函数功能：上下温无故障时，任一路<2℃，压机停机

函数位置：主--------------------------------------ok
*********************************************************************************************************************************************************/
void compressor_up_down_stop(void)
{
    if(bflg_compressor_runing == 1)
    {
        if(bflg_up_down_stop == 0)
        {
            if((bflg_TU_fault == 0)&&(bflg_TD_fault == 0))       //上下传感器无故障时
            {
                if((gss_TU < 20) || (gss_TD < 20))               //任意一路<2℃
                {
                    bflg_up_down_stop = 1;
                }
            }
        }
    }
    else
    {
        bflg_up_down_stop = 0;
    }
}


/**********************************END OF THE FILE*************************************************/
