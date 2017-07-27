#ifndef _COM_C_
#define _COM_C_
//------------------------------------------------------------------------------
//包含文件声明
#include "r_cg_macrodriver.h"
#include "r_cg_cgc.h"
#include "r_cg_port.h"
#include "r_cg_intc.h"
#include "r_cg_serial.h"
#include "r_cg_adc.h"
#include "r_cg_timer.h"
#include "r_cg_wdt.h"
#include "r_cg_userdefine.h"

#include "h_type_define.h"
#include "h_com.h"
#include "h_ad.h"
#include "h_key_disp.h"
#include "h_rom_para2.h"
#include "h_protect.h"
#include "h_bat_ctrl.h"

//peak 
#include "h_main.h"
#include "m_key_operation.h"
#include "m_peripheral_control.h"
#include "h_protect.h"
#include "m_compressor.h"
#include "M_RTC.h"
//------------------------------------------------------------------------------
//函数声明
void com_init(void);//通讯初始化程序，在初始化程序中调用

void com_tx_init(void);  //通讯发送初始化程序，在主循环程序中调用

void com_rx_init(void);  //通讯接收初始化程序，在主循环程序中调用

void com_rx_data_deal(void);  //通讯接收数据处理程序，在主循环程序中调用

void com_tx_delaytime(void);  //通讯发送延时程序，在1ms定时程序中调用

void com_rx_delaytime(void);  //通讯接收延时程序，在1ms定时程序中调用

void com_rx_end_delaytime(void);   //通讯接收完成延时程序，在1ms定时程序中调用

void com_fault_delaytime(void);    //通讯故障延时程序，在1s定时程序中调用

void com_tx_int(void);   //通讯发送中断程序

void com_rx_int(uint8_t rx_data);  //通讯接收中断程序

void com_485_bus_switch(void);

unsigned int CRC16(unsigned char *puchmsg, unsigned int usDataLen);    //进行CRC校验的程序

void com_tx_usb_rank(void);

void com_usb_delay_time(void);  

//------------------------------------------------------------------------------
//标志定义
flag_type flg_com;
flag_type flg_read_write,flg_sent_usb_control,flg_sent_usb_event,flg_sent_usb_event_1,
          flg_usb_event_temp,flg_usb_event_temp_1,flg_usb_event_attribute;
//------------------------------------------------------------------------------
//变量定义
uint8_t   guc_com_tx_buffer[120];        //通讯发送缓存器
uint8_t   guc_com_rx_buffer[80];         //通讯接收缓存器

int16_t   gss_com_tx_delaytimer;        //通讯发送延时计时器
int16_t   gss_com_rx_delaytimer;        //通讯接收延时计时器

int16_t   gss_com_rx_end_delaytimer;    //通讯接收完标志
int16_t   gss_com_fault_delaytimer;     //通讯故障延时计时器

uint8_t   guc_com_tx_point;   //通讯发送索引
uint8_t   guc_com_rx_point;   //通讯接收索引

uint8_t   guc_com_type_cnt;   //通讯类型计数器

//peak add
uint8_t  guc_IC_access_state = 0;    // 接收的IC卡的指令

uint16_t guc_flash_add;              // 接收的IC卡注册已用的地址，用来显示有多少张卡注册
uint16_t guc_temp_flash_add;         // 接收的IC卡注册临时保存的地址，用来和现在的地址对比是否有新的IC卡注册，锁屏时间好重重新
uint8_t  guc_IC_erasure_tx_ok_count; // 注销IC卡指令，在bflg_com_tx_ok中设置发送多次计数,防止丢包

uint8_t   guc_com_bus_add;            //总的从机地址
uint16_t  guc_dis_bus_delaytime;      //显示板占据总线延时
uint16_t  guc_expand_bus_delaytime;   //拓展板占据总线延时

int16_t gss_liquid_led_button;              //液晶屏的开灯按钮  ：1开 0关
int16_t gss_liquid_ozone_disinfect_button;  //液晶屏的启动杀菌按钮：1开 0关

uint8_t guc_led_comm_cnt;                   //和led板通讯x次才转换到扩展板，因为led要求实时
uint8_t gss_usb_delaytime;                  //初次上电延时xs后在发送usb事件
uint8_t guc_U_disk_state;                   //显示板发送过来的U盘状态
uint8_t guc_usb_state;                      //显示板发送过来的usb板的状态


#define   LED_DIS_ADD         0     //LED从机地址
#define   LIQUID_DIS_ADD      1     //液晶从机地址
#define   EXPAND_ADD          2     //拓展板从机地址

#define   WRITE_STATE_CNT     50

#define   READ_USER_CNT       30
#define   READ_ADMIN_CNT      30




//------------------------------------------------------------------------------
//程序内容
//------------------------------------------------------------------------------
/*******************************************************************************
function:  初始化时(仅一次)，为了刚运行时打开发送中断，发送数据；
    

*******************************************************************************/
void com_init(void) //通讯初始化程序，在初始化程序中调用
{
    bflg_com_tx_delaytime = 1;     //peak 延时时间到，打开发送中断，发送数据；
    gss_com_tx_delaytimer = 3000;
    
    bflg_com_rx_delaytime = 0;
    gss_com_rx_delaytimer = 0;
    
    bflg_read_write_data = 1; //首次写数据
}
//------------------------------------------------------------------------------
/*******************************************************************************
function: 在每一次发送数据时，对其初始化；
         LED: 发送显示板上所有要显示的内容(txt)

         初始化内容:
           1、发送的查询指令(固定)
           2、发送的设定数据---->|a、 "工作状态"、"报警"、"故障指示"、"电压"、"环温"
                                 |b、"温度设定值" "高温报警设定值" "低温报警设定值"

           a---根据主控板本身的数据采集来判断；
           
           b---是显示板按键设置好的值； 过程如下
               显示板按键设置后--->发送给主板-->主板再将此值发送给显示板显示；  
               
********************************************************************************/
void com_tx_init(void)   //通讯发送初始化程序，在主循环程序中调用
{
    int8_t i;
    word_type gss_tmp;
    word_type com_crc;
    //----------------------------------------------------------LED屏-----------------------------------
    if (bflg_com_type == 0)   //如果是LED显示屏通讯
    {
        if(guc_com_bus_add == LED_DIS_ADD)
        {
            if (bflg_read_write_data == 0)  //如果是读数据  //peak 读按键值
            {
                guc_com_tx_buffer[0] = 0x00;   //00  //设备地址
                guc_com_tx_buffer[1] = 0x03;     //读命令
                
                guc_com_tx_buffer[2] = 0x00;
                guc_com_tx_buffer[3] = 0x07;     //起始地址7
                
                guc_com_tx_buffer[4] = 0x00;
                guc_com_tx_buffer[5] = 2;      //2  //地址个数
                //----------------------------------------------
                com_crc.uword = CRC16(guc_com_tx_buffer, 6);
                guc_com_tx_buffer[6] = com_crc.ubyte.low;
                guc_com_tx_buffer[7] = com_crc.ubyte.high;
            }
            else   //如果是写数据   //peak 设定数码管、LED显示内容
            {
                guc_com_tx_buffer[0] = 0x00;     //设备地址
                guc_com_tx_buffer[1] = 0x10;     //写命令
                
                guc_com_tx_buffer[2] = 0x00;
                guc_com_tx_buffer[3] = 0x00;     //起始地址0
                
                guc_com_tx_buffer[4] = 0x00;
                guc_com_tx_buffer[5] = 22;    //7    //地址个数  peak 要写几个寄存器，每个寄存器占2个字节；7*2 = 14 个字节；
                
                guc_com_tx_buffer[6] = 44;    //14   //字节数    peak [7]--[20] = 14 具体的内容
        
                for (i = 0; i < 12; i++)         // peak [7]~[18] = 12 ;
                {
                    guc_com_tx_buffer[i + 7] = guc_led_buffer[i];    //peak 数码管寄存器
                }           
                
                //----------------------------------------------
                if(bflg_IC_erasure_order == 1)  //如有注销指令,注销IC卡指令_在bflg_com_tx_ok中设置发送多次计数,防止丢包
                {               
                    guc_com_tx_buffer[19] = 1;
                }
                else
                {
                    guc_com_tx_buffer[19] = 0;
                }
                
                if (bflg_alarm_lamp == 0)         //peak 显示板上的三位报警灯 
                {
                    //guc_com_tx_buffer[19] = 0;
                    guc_com_tx_buffer[20] = 0x00;
                }
                else
                {
                    //guc_com_tx_buffer[19] = 0;
                    guc_com_tx_buffer[20] = 0x07;     //peak 显示板上面的 "报警" 的三个灯 
                }
                
                //--------------------------------------------------------------------------------------------
                //usb使用的数据帧
                gss_tmp.sword = gss_user_set_parm_value[DISP_USER_SET_TS]*10;     //设置温度
                guc_com_tx_buffer[21] = gss_tmp.ubyte.low;
                guc_com_tx_buffer[22] = gss_tmp.ubyte.high;
                gss_tmp.sword = gss_TU*10;                                        //上显示温度
                guc_com_tx_buffer[23] = gss_tmp.ubyte.low;
                guc_com_tx_buffer[24] = gss_tmp.ubyte.high;
                gss_tmp.sword = gss_TD*10;                                        //下显示温度
                guc_com_tx_buffer[25] = gss_tmp.ubyte.low;
                guc_com_tx_buffer[26] = gss_tmp.ubyte.high;
                gss_tmp.sword = gss_TA*10;                                        //箱内温度
                guc_com_tx_buffer[27] = gss_tmp.ubyte.low;
                guc_com_tx_buffer[28] = gss_tmp.ubyte.high;
                gss_tmp.sword = gss_TE*10;                                        //除霜(热交换)温度
                guc_com_tx_buffer[29] = gss_tmp.ubyte.low;
                guc_com_tx_buffer[30] = gss_tmp.ubyte.high;    
                //                                                                //预留
                guc_com_tx_buffer[31] = 0;
                guc_com_tx_buffer[32] = 0;

                //--------------------------------------------------------------------------------------------
                //事件帧
                if(bflg_sent_usb_ALH == 1)
                {
                    bflg_sent_usb_ALH = 0;
                    guc_com_tx_buffer[20] |= 0x80;  //[20]的第7位代表是否有事件帧的标志

                    guc_com_tx_buffer[33] = 0x00;                      //事件类代码  
                    gss_tmp.sword = gss_TU*10;
                    guc_com_tx_buffer[34] = gss_tmp.ubyte.low;         //事件值1 (温湿度、电压值需*100,因为TA已放大10,小端)
                    guc_com_tx_buffer[35] = gss_tmp.ubyte.high;   
                    gss_tmp.sword = gss_TD*10;
                    guc_com_tx_buffer[36] = gss_tmp.ubyte.low;         //事件值2 需*100
                    guc_com_tx_buffer[37] = gss_tmp.ubyte.high;
                    guc_com_tx_buffer[38] = 0x00;
                }
                else if(bflg_sent_usb_ALL == 1)
                {
                    bflg_sent_usb_ALL = 0;
                    guc_com_tx_buffer[20] |= 0x80;   

                    guc_com_tx_buffer[33] = 0x01;                      
                    gss_tmp.sword = gss_TU*10;
                    guc_com_tx_buffer[34] = gss_tmp.ubyte.low;         
                    guc_com_tx_buffer[35] = gss_tmp.ubyte.high;   
                    gss_tmp.sword = gss_TD*10;
                    guc_com_tx_buffer[36] = gss_tmp.ubyte.low;         
                    guc_com_tx_buffer[37] = gss_tmp.ubyte.high;
                    guc_com_tx_buffer[38] = 0x00;
                }
                else if(bflg_sent_usb_THW == 1)
                {
                    bflg_sent_usb_THW = 0;
                    guc_com_tx_buffer[20] |= 0x80;   

                    guc_com_tx_buffer[33] = 0x02;                      
                    gss_tmp.sword = gss_THW*10;
                    guc_com_tx_buffer[34] = gss_tmp.ubyte.low;         
                    guc_com_tx_buffer[35] = gss_tmp.ubyte.high;   
                    guc_com_tx_buffer[36] = 0x00;         
                    guc_com_tx_buffer[37] = 0x00;
                    guc_com_tx_buffer[38] = 0x00;
                }
                else if(bflg_sent_usb_ALHS == 1)
                {
                    bflg_sent_usb_ALHS = 0;
                    guc_com_tx_buffer[20] |= 0x80;   

                    guc_com_tx_buffer[33] = 0x03;                      
                    gss_tmp.sword = gss_user_set_parm_value[DISP_USER_SET_ALH]*10;
                    guc_com_tx_buffer[34] = gss_tmp.ubyte.low;         
                    guc_com_tx_buffer[35] = gss_tmp.ubyte.high; 
                    gss_tmp.sword = gss_ALH_old_value;
                    guc_com_tx_buffer[36] = gss_tmp.ubyte.low;        
                    guc_com_tx_buffer[37] = gss_tmp.ubyte.high; 
                    guc_com_tx_buffer[38] = 0x00;
                }
                else if(bflg_sent_usb_ALLS == 1)
                {
                    bflg_sent_usb_ALLS = 0;
                    guc_com_tx_buffer[20] |= 0x80;   

                    guc_com_tx_buffer[33] = 0x04;                      
                    gss_tmp.sword = gss_user_set_parm_value[DISP_USER_SET_ALL]*10;
                    guc_com_tx_buffer[34] = gss_tmp.ubyte.low;         
                    guc_com_tx_buffer[35] = gss_tmp.ubyte.high; 
                    gss_tmp.sword = gss_ALL_old_value;
                    guc_com_tx_buffer[36] = gss_tmp.ubyte.low;        
                    guc_com_tx_buffer[37] = gss_tmp.ubyte.high; 
                    guc_com_tx_buffer[38] = 0x00;
                }
                else if(bflg_sent_usb_TS == 1)
                {
                    bflg_sent_usb_TS = 0;
                    guc_com_tx_buffer[20] |= 0x80;   

                    guc_com_tx_buffer[33] = 0x05;                      
                    gss_tmp.sword = gss_user_set_parm_value[DISP_USER_SET_TS]*10;
                    guc_com_tx_buffer[34] = gss_tmp.ubyte.low;         
                    guc_com_tx_buffer[35] = gss_tmp.ubyte.high; 
                    gss_tmp.sword = gss_TS_old_value;
                    guc_com_tx_buffer[36] = gss_tmp.ubyte.low;        
                    guc_com_tx_buffer[37] = gss_tmp.ubyte.high; 
                    guc_com_tx_buffer[38] = 0x00;
                }
                else if(bflg_sent_usb_DOR == 1)
                {
                    bflg_sent_usb_DOR = 0;
                    guc_com_tx_buffer[20] |= 0x80; 

                    guc_com_tx_buffer[33] = 0x06;    
                    gss_tmp.sword = gss_TA*10;
                    guc_com_tx_buffer[34] = gss_tmp.ubyte.low;         
                    guc_com_tx_buffer[35] = gss_tmp.ubyte.high;  
                    guc_com_tx_buffer[36] = 0x00;    
                    guc_com_tx_buffer[37] = 0x00;
                    if(bflg_usb_DOR_open == 1)    //开门
                    {
                        guc_com_tx_buffer[38] = 0x01;    
                    }
                    else
                    {
                        guc_com_tx_buffer[38] = 0x02;    
                    }
                }
                else if(bflg_sent_usb_POW == 1)
                {
                    bflg_sent_usb_POW = 0;
                    guc_com_tx_buffer[20] |= 0x80; 

                    guc_com_tx_buffer[33] = 0x07;    
                    guc_com_tx_buffer[34] = 0x00;    
                    guc_com_tx_buffer[35] = 0x00;    
                    guc_com_tx_buffer[36] = 0x00;    
                    guc_com_tx_buffer[37] = 0x00;
                    if(bflg_usb_POW_on == 1)
                    {
                        guc_com_tx_buffer[38] = 0x01;    
                    }
                    else
                    {
                        guc_com_tx_buffer[38] = 0x02;    
                    }
                }
                else if(bflg_sent_usb_BAT == 1)
                {
                    bflg_sent_usb_BAT = 0;
                    guc_com_tx_buffer[20] |= 0x80; 

                    guc_com_tx_buffer[33] = 0x08;    
                    gss_tmp.sword = guc_bat_Vout*10;
                    guc_com_tx_buffer[34] = gss_tmp.ubyte.low;         
                    guc_com_tx_buffer[35] = gss_tmp.ubyte.high;  
                    guc_com_tx_buffer[36] = 0x00;   
                    guc_com_tx_buffer[37] = 0x00;
                    if(bflg_alarm_bat_discnnt == 1)
                    {
                        guc_com_tx_buffer[38] = 0x01;   
                    }
                    else if(bflg_alarm_bat_fed == 1)
                    {
                        guc_com_tx_buffer[38] = 0x02;   
                    }
                    else if(bflg_alarm_bat_low == 1)
                    {
                        guc_com_tx_buffer[38] = 0x03;   
                    }
                }
                else if(bflg_sent_usb_FAT_TA == 1)
                {
                    bflg_sent_usb_FAT_TA = 0;
                    guc_com_tx_buffer[20] |= 0x80; 

                    guc_com_tx_buffer[33] = 0x09;  
                    guc_com_tx_buffer[34] = 0x10;   
                    guc_com_tx_buffer[35] = 0x00;    
                    guc_com_tx_buffer[36] = 0x00;   
                    guc_com_tx_buffer[37] = 0x00;
                    guc_com_tx_buffer[38] = 0x00;  
                }
                else if(bflg_sent_usb_FAT_TE == 1)
                {
                    bflg_sent_usb_FAT_TE = 0;
                    guc_com_tx_buffer[20] |= 0x80; 
                
                    guc_com_tx_buffer[33] = 0x09;  
                    guc_com_tx_buffer[34] = 0x11;   
                    guc_com_tx_buffer[35] = 0x00;    
                    guc_com_tx_buffer[36] = 0x00;   
                    guc_com_tx_buffer[37] = 0x00;
                    guc_com_tx_buffer[38] = 0x00;  
                }
                else if(bflg_sent_usb_FAT_THW == 1)
                {
                    bflg_sent_usb_FAT_THW = 0;
                    guc_com_tx_buffer[20] |= 0x80; 

                    guc_com_tx_buffer[33] = 0x09;  
                    guc_com_tx_buffer[34] = 0x12;   
                    guc_com_tx_buffer[35] = 0x00;    
                    guc_com_tx_buffer[36] = 0x00;   
                    guc_com_tx_buffer[37] = 0x00;
                    guc_com_tx_buffer[38] = 0x00;  
                }
                else if(bflg_sent_usb_FAT_TC == 1)
                {
                    bflg_sent_usb_FAT_TC = 0;
                    guc_com_tx_buffer[20] |= 0x80; 
                
                    guc_com_tx_buffer[33] = 0x09;  
                    guc_com_tx_buffer[34] = 0x13;   
                    guc_com_tx_buffer[35] = 0x00;    
                    guc_com_tx_buffer[36] = 0x00;   
                    guc_com_tx_buffer[37] = 0x00;
                    guc_com_tx_buffer[38] = 0x00;  
                }
                else if(bflg_sent_usb_FAT_TU == 1)
                {
                    bflg_sent_usb_FAT_TU = 0;
                    guc_com_tx_buffer[20] |= 0x80; 
                
                    guc_com_tx_buffer[33] = 0x09;  
                    guc_com_tx_buffer[34] = 0x14;   
                    guc_com_tx_buffer[35] = 0x00;    
                    guc_com_tx_buffer[36] = 0x00;   
                    guc_com_tx_buffer[37] = 0x00;
                    guc_com_tx_buffer[38] = 0x00;  
                }
                else if(bflg_sent_usb_FAT_TD == 1)
                {
                    bflg_sent_usb_FAT_TD = 0;
                    guc_com_tx_buffer[20] |= 0x80; 
                
                    guc_com_tx_buffer[33] = 0x09;  
                    guc_com_tx_buffer[34] = 0x15;   
                    guc_com_tx_buffer[35] = 0x00;    
                    guc_com_tx_buffer[36] = 0x00;   
                    guc_com_tx_buffer[37] = 0x00;
                    guc_com_tx_buffer[38] = 0x00;  
                }
                else if(bflg_sent_usb_IC == 1)
                {
                    bflg_sent_usb_IC = 0;
                    guc_com_tx_buffer[20] |= 0x80; 
                    
                    guc_com_tx_buffer[33] = 0x0A; 
                    gss_tmp.sword = (guc_flash_add>>2);   //guc_flash_add>>2 = 除以4，将地址转换为张数
                    //guc_com_tx_buffer[34] = (uint8_t)(guc_flash_add>>2);       //guc_flash_add>>2 = 除以4，将地址转换为张数
                    //guc_com_tx_buffer[35] = (uint8_t)((guc_flash_add>>2)>>8);  
                    guc_com_tx_buffer[34] = gss_tmp.ubyte.low;         
                    guc_com_tx_buffer[35] = gss_tmp.ubyte.high;                      
                    guc_com_tx_buffer[36] = 0x00;   
                    guc_com_tx_buffer[37] = 0x00;
                    guc_com_tx_buffer[38] = 0x00;    
                }
                else if(bflg_sent_usb_TC == 1)
                {
                    bflg_sent_usb_TC = 0;
                    guc_com_tx_buffer[20] |= 0x80; 

                    guc_com_tx_buffer[33] = 0x0B;    
                    gss_tmp.sword = (gss_TC * 10);   
                    guc_com_tx_buffer[34] = gss_tmp.ubyte.low;         
                    guc_com_tx_buffer[35] = gss_tmp.ubyte.high;  
                    gss_tmp.sword = (gss_THW * 10);   
                    guc_com_tx_buffer[36] = gss_tmp.ubyte.low;         
                    guc_com_tx_buffer[37] = gss_tmp.ubyte.high;  
                    guc_com_tx_buffer[38] = 0x00;    
                }
                else if(bflg_sent_usb_POW_OFF == 1)
                {
                    bflg_sent_usb_POW_OFF = 0;
                    guc_com_tx_buffer[20] |= 0x80; 
                
                    guc_com_tx_buffer[33] = 0x0C;    
                    gss_tmp.sword = (gss_TA * 10);   
                    guc_com_tx_buffer[34] = gss_tmp.ubyte.low;         
                    guc_com_tx_buffer[35] = gss_tmp.ubyte.high;    
                    guc_com_tx_buffer[36] = 0x00;    
                    guc_com_tx_buffer[37] = 0x00;
                    guc_com_tx_buffer[38] = 0x00;                 
                }
                else if(bflg_sent_usb_DOOR_OPEN == 1)
                {
                    bflg_sent_usb_DOOR_OPEN = 0;
                    guc_com_tx_buffer[20] |= 0x80; 
                
                    guc_com_tx_buffer[33] = 0x0D;    
                    gss_tmp.sword = (gss_TA * 10);   
                    guc_com_tx_buffer[34] = gss_tmp.ubyte.low;         
                    guc_com_tx_buffer[35] = gss_tmp.ubyte.high;   
                    guc_com_tx_buffer[36] = 0x00;    
                    guc_com_tx_buffer[37] = 0x00;
                    guc_com_tx_buffer[38] = 0x00;  
                }
                else if(bflg_sent_usb_AIR_H == 1)
                {
                    bflg_sent_usb_AIR_H = 0;
                    guc_com_tx_buffer[20] |= 0x80; 
                
                    guc_com_tx_buffer[33] = 0x0E;    
                    gss_tmp.sword = (gss_TA * 10);   
                    guc_com_tx_buffer[34] = gss_tmp.ubyte.low;         
                    guc_com_tx_buffer[35] = gss_tmp.ubyte.high;   
                    guc_com_tx_buffer[36] = 0x00;    
                    guc_com_tx_buffer[37] = 0x00;
                    guc_com_tx_buffer[38] = 0x00;  
                }
                else if(bflg_sent_usb_AIR_L == 1)
                {
                    bflg_sent_usb_AIR_L = 0;
                    guc_com_tx_buffer[20] |= 0x80; 
                
                    guc_com_tx_buffer[33] = 0x0F;    
                    gss_tmp.sword = (gss_TA * 10);   
                    guc_com_tx_buffer[34] = gss_tmp.ubyte.low;         
                    guc_com_tx_buffer[35] = gss_tmp.ubyte.high;   
                    guc_com_tx_buffer[36] = 0x00;    
                    guc_com_tx_buffer[37] = 0x00;
                    guc_com_tx_buffer[38] = 0x00;  
                }
                else if(bflg_sent_usb_TE_H == 1)
                {
                    bflg_sent_usb_TE_H = 0;
                    guc_com_tx_buffer[20] |= 0x80; 
                
                    guc_com_tx_buffer[33] = 0x10;    
                    gss_tmp.sword = (gss_TE * 10);   
                    guc_com_tx_buffer[34] = gss_tmp.ubyte.low;         
                    guc_com_tx_buffer[35] = gss_tmp.ubyte.high;   
                    guc_com_tx_buffer[36] = 0x00;    
                    guc_com_tx_buffer[37] = 0x00;
                    guc_com_tx_buffer[38] = 0x00;  
                }
                else if(bflg_sent_usb_ozone_FAT == 1)
                {
                    bflg_sent_usb_ozone_FAT = 0;
                    guc_com_tx_buffer[20] |= 0x80; 

                    guc_com_tx_buffer[33] = 0x11;    
                    guc_com_tx_buffer[34] = 0x00;         
                    guc_com_tx_buffer[35] = 0x00;   
                    guc_com_tx_buffer[36] = 0x00;    
                    guc_com_tx_buffer[37] = 0x00;
                    guc_com_tx_buffer[38] = 0x00;  
                }                
                else 
                {
                    guc_com_tx_buffer[20] &= 0x7f; 
                    
                    guc_com_tx_buffer[33] = 0x00;    
                    guc_com_tx_buffer[34] = 0x00;   
                    guc_com_tx_buffer[35] = 0x00;    
                    guc_com_tx_buffer[36] = 0x00;    
                    guc_com_tx_buffer[37] = 0x00;
                    guc_com_tx_buffer[38] = 0x00;    
                }

                //--------------------------------------------------------------------------------------------
                //时间帧
                if(bflg_usb_set_time == 1)           //有修改设置时间/常规的也是一直发送时间
                {
                    bflg_usb_set_time = 0;
                    guc_com_tx_buffer[20] |= 0x20;   //[20]的第5位代表是否有修改设置时间的标志
                }
                guc_com_tx_buffer[39] = guc_RTC_r_year;    //年
                guc_com_tx_buffer[40] = guc_RTC_r_month;   //月
                guc_com_tx_buffer[41] = guc_RTC_r_day;     //日
                guc_com_tx_buffer[42] = guc_RTC_r_hour;    //时
                guc_com_tx_buffer[43] = guc_RTC_r_minute;  //分
                guc_com_tx_buffer[44] = guc_RTC_r_second;  //秒
                //--------------------------------------------------------------------------------------------
                //控制帧    
                if(bflg_sent_usb_CL2 == 1)          //清除usb数据
                {
                    bflg_sent_usb_CL2 = 0;
                    guc_com_tx_buffer[20] |= 0x40;   //[20]的第6位代表是否有控制帧的标志
                    
                    guc_com_tx_buffer[45] = 0x01;    //控制代码
                    guc_com_tx_buffer[46] = 0x32;    //值1
                    guc_com_tx_buffer[47] = 0x33;    //值2
                    guc_com_tx_buffer[48] = 0x34;    //值3
                }
                else if(bflg_sent_usb_T1 == 1)       //usb记录频率
                {
                    bflg_sent_usb_T1 = 0;
                    guc_com_tx_buffer[20] |= 0x40; 
                    
                    guc_com_tx_buffer[45] = 0x02;   
                    guc_com_tx_buffer[46] = (uint8_t)gss_user_set_parm_value[DISP_USER_SET_T1];  
                    guc_com_tx_buffer[47] = 0x00;    
                    guc_com_tx_buffer[48] = 0x00;    
                }
                else if(bflg_sent_usb_P6 == 1)       //usb 导数模式
                {
                    bflg_sent_usb_P6 = 0;
                    guc_com_tx_buffer[20] |= 0x40;
                    
                    guc_com_tx_buffer[45] = 0x03;   
                    guc_com_tx_buffer[46] = (uint8_t)gss_user_set_parm_value[DISP_USER_SET_P6];
                    guc_com_tx_buffer[47] = 0x00;    
                    guc_com_tx_buffer[48] = 0x00;    
                }
                else if(bflg_sent_usb_UL == 1)     //usb语言类型
                {
                    bflg_sent_usb_UL = 0;
                    guc_com_tx_buffer[20] |= 0x40;
                    
                    guc_com_tx_buffer[45] = 0x05;   
                    guc_com_tx_buffer[46] = (uint8_t)gss_factory_parm_value[DISP_FAC_UL];
                    guc_com_tx_buffer[47] = 0x00;    
                    guc_com_tx_buffer[48] = 0x00; 
                }
                else
                {
                    guc_com_tx_buffer[20] &= 0xbf;
                    
                    guc_com_tx_buffer[45] = 0x00;    
                    guc_com_tx_buffer[46] = 0x00;    
                    guc_com_tx_buffer[47] = 0x00;    
                    guc_com_tx_buffer[48] = 0x00;    
                }
                //--------------------------------------------------------------------------------------------
                //机型帧
                guc_com_tx_buffer[49] = 0x02;    //机型识别码 血液为 0x02
                guc_com_tx_buffer[50] = 0x00;    //预留
                
                //--------------------------------------------------------------------------------------
                com_crc.uword = CRC16(guc_com_tx_buffer, 51);  //21
                guc_com_tx_buffer[51] = com_crc.ubyte.low;     //21
                guc_com_tx_buffer[52] = com_crc.ubyte.high;    //22
            }
        }
        else if(guc_com_bus_add == EXPAND_ADD)
        {
            if (bflg_read_write_data == 0)       //如果是读数据  
            {
                guc_com_tx_buffer[0] = 0x02;     //设备地址
                guc_com_tx_buffer[1] = 0x03;     //读命令
                
                guc_com_tx_buffer[2] = 0x00;
                guc_com_tx_buffer[3] = 0x07;     //起始地址7
                
                guc_com_tx_buffer[4] = 0x00;
                guc_com_tx_buffer[5] = 3;        //地址个数
                //----------------------------------------------
                com_crc.uword = CRC16(guc_com_tx_buffer, 6);
                guc_com_tx_buffer[6] = com_crc.ubyte.low;
                guc_com_tx_buffer[7] = com_crc.ubyte.high;
            }
            else    
            {
                guc_com_tx_buffer[0] = 0x02;     //设备地址
                guc_com_tx_buffer[1] = 0x10;     //写命令
                
                guc_com_tx_buffer[2] = 0x00;
                guc_com_tx_buffer[3] = 0x00;     //起始地址0
                
                guc_com_tx_buffer[4] = 0x00;
                guc_com_tx_buffer[5] = 7;        //地址个数  peak 要写几个寄存器，每个寄存器占2个字节；7*2 = 14 个字节；
                
                guc_com_tx_buffer[6] = 14;       //发送的数据字节数

                //upgrade--> gss_tmp.ubyte.low
                guc_com_tx_buffer[7] |= (uint8_t)(gss_factory_parm_value[DISP_FAC_Ct6]>>8);            //环温校准H-放大10
                guc_com_tx_buffer[8] |= (uint8_t)gss_factory_parm_value[DISP_FAC_Ct6];                 //L
                guc_com_tx_buffer[9] |= (uint8_t)(gss_factory_parm_value[DISP_FAC_Ct7]>>8)；            //湿度校准
                guc_com_tx_buffer[10]|= (uint8_t)gss_factory_parm_value[DISP_FAC_Ct7];                 //L
                guc_com_tx_buffer[11]|= (uint8_t)(gss_user_manage_parm_value[DISP_USER_MANAGE_S2]>>8); //最大杀菌时间
                guc_com_tx_buffer[12]|= (uint8_t)gss_user_manage_parm_value[DISP_USER_MANAGE_S2];      //L
                guc_com_tx_buffer[13] = (uint8_t)gss_user_manage_parm_value[DISP_USER_MANAGE_S1];      //杀菌模式
                if(bflg_expand_unlock == 1)     //开锁
                {
                    guc_com_tx_buffer[14] = 0x01;     
                }
                else
                {
                    guc_com_tx_buffer[14] = 0x00;    
                }
                if(DOOR1_IN_PIN == 1)   //开门
                {
                    guc_com_tx_buffer[15] = 0x01; 
                }
                else
                {
                    guc_com_tx_buffer[15] = 0x00; 
                }
                guc_com_tx_buffer[16] = 0x00; //预留
                guc_com_tx_buffer[17] = 0x00;
                guc_com_tx_buffer[18] = 0x00;
                guc_com_tx_buffer[19] = 0x00;
                guc_com_tx_buffer[20] = 0x00;
                
                //----------------------------------------------
                com_crc.uword = CRC16(guc_com_tx_buffer, 21);
                guc_com_tx_buffer[21] = com_crc.ubyte.low;
                guc_com_tx_buffer[22] = com_crc.ubyte.high;
            }
        }
    }

//---------------------------------------------------------------------液晶屏------------------
    else  //如果是液晶屏通讯
    {
        if(guc_com_bus_add == LIQUID_DIS_ADD)
        {
            if (bflg_read_write_data == 0)        //读用户信息
            {
                if (bflg_read_user_admin == 0)   //如果是读用户信息
                {
                    guc_com_tx_buffer[0] = 0x01;      //设备地址
                    guc_com_tx_buffer[1] = 0x03;      //读命令
                    
                    guc_com_tx_buffer[2] = 0xA4;
                    guc_com_tx_buffer[3] = 0x1F;      //起始地址8100
                    
                    guc_com_tx_buffer[4] = READ_USER_CNT;
                    guc_com_tx_buffer[5] = 0X00;       //地址个数
                    //------------------------------------------
                    com_crc.uword = CRC16(guc_com_tx_buffer, 6);
                    guc_com_tx_buffer[6] = com_crc.ubyte.low;
                    guc_com_tx_buffer[7] = com_crc.ubyte.high;
                }
                else    //读管理员数据
                {
                    guc_com_tx_buffer[0] = 0x01;      //设备地址
                    guc_com_tx_buffer[1] = 0x03;      //读命令
                    
                    guc_com_tx_buffer[2] = 0xD6;
                    guc_com_tx_buffer[3] = 0x1F;      //起始地址8150
                    
                    guc_com_tx_buffer[4] = READ_ADMIN_CNT;
                    guc_com_tx_buffer[5] = 0X00;       //地址个数
                    //------------------------------------------
                    com_crc.uword = CRC16(guc_com_tx_buffer, 6);
                    guc_com_tx_buffer[6] = com_crc.ubyte.low;
                    guc_com_tx_buffer[7] = com_crc.ubyte.high;
                }
            }
            else   //如果是写数据
            {
                guc_com_tx_buffer[0] = 0x01;     //设备地址
                guc_com_tx_buffer[1] = 0x10;     //写命令
                
                guc_com_tx_buffer[2] = 0x40;
                guc_com_tx_buffer[3] = 0x1F;     //起始地址10000
                
                guc_com_tx_buffer[4] = WRITE_STATE_CNT;
                guc_com_tx_buffer[5] = 0x00;    //地址个数
                
                guc_com_tx_buffer[6] = (WRITE_STATE_CNT * 2);   //字节数
                //----------------------------------------------
                {
                    //
                    //debug
                    //gss_THW = -1997;
                    
                    gss_tmp.sword = gss_THW;     //环境温度
                    guc_com_tx_buffer[7] = gss_tmp.ubyte.low;
                    guc_com_tx_buffer[8] = gss_tmp.ubyte.high;
                    //------------------------------------------
                    gss_tmp.sword = guc_humidity; //环境湿度
                    guc_com_tx_buffer[9] = gss_tmp.ubyte.low;
                    guc_com_tx_buffer[10] = gss_tmp.ubyte.high;
                    //------------------------------------------
                    gss_tmp.sword = gss_TA;      //PT100
                    guc_com_tx_buffer[11] = gss_tmp.ubyte.low;
                    guc_com_tx_buffer[12] = gss_tmp.ubyte.high;
                    //------------------------------------------
                    gss_tmp.sword = gss_TE;     //热交换器(化霜传感器)温度
                    guc_com_tx_buffer[13] = gss_tmp.ubyte.low;
                    guc_com_tx_buffer[14] = gss_tmp.ubyte.high;
                    //------------------------------------------
                    gss_tmp.sword = gss_TC;    //冷凝器温度
                    guc_com_tx_buffer[15] = gss_tmp.ubyte.low;
                    guc_com_tx_buffer[16] = gss_tmp.ubyte.high;
                    //------------------------------------------
                    gss_tmp.sword = gss_TU;   //上传感器温度
                    guc_com_tx_buffer[17] = gss_tmp.ubyte.low;
                    guc_com_tx_buffer[18] = gss_tmp.ubyte.high;
                    //------------------------------------------
                    gss_tmp.sword = gss_TD;  //下传感器温度
                    guc_com_tx_buffer[19] = gss_tmp.ubyte.low;
                    guc_com_tx_buffer[20] = gss_tmp.ubyte.high;
                    //------------------------------------------
                    gss_tmp.sword = gss_VAC; //市电电源电压
                    guc_com_tx_buffer[21] = gss_tmp.ubyte.low;
                    guc_com_tx_buffer[22] = gss_tmp.ubyte.high;
                    //------------------------------------------
                    gss_tmp.sword = guc_bat_Vout; //电池电量
                    guc_com_tx_buffer[23] = gss_tmp.ubyte.low;
                    guc_com_tx_buffer[24] = gss_tmp.ubyte.high;
            
                    //------------------------------------------
                    if (bflg_compressor_runing == 0)    //压缩机状态
                    {
                        guc_com_tx_buffer[25] = 0;
                        guc_com_tx_buffer[26] = 0;
                    }
                    else
                    {
                        guc_com_tx_buffer[25] = 1;
                        guc_com_tx_buffer[26] = 0;
                    }
                    //------------------------------------------
                    if (bflg_inside_fan_runing == 0)    //内风机状态
                    {
                        guc_com_tx_buffer[27] = 0;
                        guc_com_tx_buffer[28] = 0;
                    }
                    else
                    {
                        guc_com_tx_buffer[27] = 1;
                        guc_com_tx_buffer[28] = 0;
                    }
                    //------------------------------------------
                    if (bflg_compressor_runing == 0)       //外风机状态(即冷凝风机，状态同压机)
                    {
                        guc_com_tx_buffer[29] = 0;
                        guc_com_tx_buffer[30] = 0;
                    }
                    else
                    {
                        guc_com_tx_buffer[29] = 1;
                        guc_com_tx_buffer[30] = 0;
                    }
                    //------------------------------------------
                    if (bflg_heat_runing == 0)       //门体加热丝状态
                    {
                        guc_com_tx_buffer[31] = 0;
                        guc_com_tx_buffer[32] = 0;
                    }
                    else
                    {
                        guc_com_tx_buffer[31] = 1;
                        guc_com_tx_buffer[32] = 0;
                    }
                    //------------------------------------------
                    if (bflg_led_on == 0)           //照明灯状态
                    {
                        guc_com_tx_buffer[33] = 0;
                        guc_com_tx_buffer[34] = 0;
                    }
                    else
                    {
                        guc_com_tx_buffer[33] = 1;
                        guc_com_tx_buffer[34] = 0;
                    }
                    //------------------------------------------
                    if (bflg_sterilization_state == 0)     //臭氧杀菌状态
                    {
                        guc_com_tx_buffer[35] = 0;
                        guc_com_tx_buffer[36] = 0;
                    }
                    else
                    {
                        guc_com_tx_buffer[35] = 1;
                        guc_com_tx_buffer[36] = 0;
                    }
                    //------------------------------------------
                    if (bflg_beam_heat_runing == 0)   //中梁加热丝状态
                    {
                        guc_com_tx_buffer[37] = 0;
                        guc_com_tx_buffer[38] = 0;
                    }
                    else
                    {
                        guc_com_tx_buffer[37] = 1;
                        guc_com_tx_buffer[38] = 0;
                    }
                    //------------------------------------------
                    if (bflg_water_box_heat_runing == 0)   //接水盒加热丝状态
                    {
                        guc_com_tx_buffer[39] = 1;         //李帆要求一直开
                        guc_com_tx_buffer[40] = 0;
                    }
                    else
                    {
                        guc_com_tx_buffer[39] = 1;
                        guc_com_tx_buffer[40] = 0;
                    }
                    //------------------------------------------
                    if (bflg_locd_runing == 0)      //电磁锁状态-实际还未处理
                    {
                        guc_com_tx_buffer[41] = 0;
                        guc_com_tx_buffer[42] = 0;
                    }
                    else
                    {
                        guc_com_tx_buffer[41] = 1;
                        guc_com_tx_buffer[42] = 0;
                    }
                    //------------------------------------------
                    if (bflg_prot2_door1_pin == 0)    //门开关状态
                    {
                        guc_com_tx_buffer[43] = 0;
                        guc_com_tx_buffer[44] = 0;
                    }
                    else
                    {
                        guc_com_tx_buffer[43] = 1;
                        guc_com_tx_buffer[44] = 0;
                    }
                    //------------------------------------------
                    gss_tmp.sword = gus_trip_code1;   //报警状态信息1
                    guc_com_tx_buffer[45] = gss_tmp.ubyte.low;
                    guc_com_tx_buffer[46] = gss_tmp.ubyte.high;
                    //------------------------------------------
                    gss_tmp.sword = gus_trip_code2;   //报警状态信息2
                    guc_com_tx_buffer[47] = gss_tmp.ubyte.low;
                    guc_com_tx_buffer[48] = gss_tmp.ubyte.high;
            
                    //预留
                    guc_com_tx_buffer[49] = 0;
                    guc_com_tx_buffer[50] = 0;
                    guc_com_tx_buffer[51] = 0;
                    guc_com_tx_buffer[52] = 0;
                    guc_com_tx_buffer[53] = 0;
                    guc_com_tx_buffer[54] = 0;  
                    guc_com_tx_buffer[55] = 0;
                    guc_com_tx_buffer[56] = 0;
                    guc_com_tx_buffer[57] = 0;
                    guc_com_tx_buffer[58] = 0;
                    guc_com_tx_buffer[59] = 0;
                    guc_com_tx_buffer[60] = 0; 
                    guc_com_tx_buffer[61] = 0;
                    guc_com_tx_buffer[62] = 0;
                    guc_com_tx_buffer[63] = 0;
                    guc_com_tx_buffer[64] = 0;
                    guc_com_tx_buffer[65] = 0;
                    guc_com_tx_buffer[66] = 0;     
                    guc_com_tx_buffer[67] = 0;
                    guc_com_tx_buffer[68] = 0;  
                    guc_com_tx_buffer[69] = 0;
                    guc_com_tx_buffer[70] = 0;
                    guc_com_tx_buffer[71] = 0;
                    guc_com_tx_buffer[72] = 0;
                    guc_com_tx_buffer[73] = 0;
                    guc_com_tx_buffer[74] = 0; 
                    guc_com_tx_buffer[75] = 0;
                    guc_com_tx_buffer[76] = 0;
                    guc_com_tx_buffer[77] = 0;
                    guc_com_tx_buffer[78] = 0;
                    guc_com_tx_buffer[79] = 0;
                    guc_com_tx_buffer[80] = 0; 
                    //预留
                    guc_com_tx_buffer[81] = 0;
                    guc_com_tx_buffer[82] = 0;
                    //------------------------------------------
                    //预留
                    guc_com_tx_buffer[83] = 0;
                    guc_com_tx_buffer[84] = 0;
                    //------------------------------------------
                    //预留
                    guc_com_tx_buffer[85] = 0;
                    guc_com_tx_buffer[86] = 0;
                    //------------------------------------------
                    //预留
                    guc_com_tx_buffer[87] = 0;
                    guc_com_tx_buffer[88] = 0;
                    //------------------------------------------
                    //预留
                    guc_com_tx_buffer[89] = 0;
                    guc_com_tx_buffer[90] = 0;
                    //------------------------------------------
                    //预留
                    guc_com_tx_buffer[91] = 0;
                    guc_com_tx_buffer[92] = 0;
                    //------------------------------------------
                    //预留
                    guc_com_tx_buffer[93] = 0;
                    guc_com_tx_buffer[94] = 0;
                    //------------------------------------------
                    //预留
                    guc_com_tx_buffer[95] = 0;
                    guc_com_tx_buffer[96] = 0;
                    //------------------------------------------
                    //预留
                    guc_com_tx_buffer[97] = 0;
                    guc_com_tx_buffer[98] = 0;
                    //------------------------------------------
                    //预留
                    guc_com_tx_buffer[99] = 0;
                    guc_com_tx_buffer[100] = 0;
                    //------------------------------------------
                    //预留
                    guc_com_tx_buffer[101] = 0;
                    guc_com_tx_buffer[102] = 0;
                    //------------------------------------------
                    //预留
                    guc_com_tx_buffer[103] = 0;
                    guc_com_tx_buffer[104] = 0;
                    //------------------------------------------
                    //预留
                    guc_com_tx_buffer[105] = 0;
                    guc_com_tx_buffer[106] = 0;
                }
                //----------------------------------------------
                com_crc.uword = CRC16(guc_com_tx_buffer, ((WRITE_STATE_CNT * 2) + 7));   //107
                guc_com_tx_buffer[(WRITE_STATE_CNT * 2) + 7] = com_crc.ubyte.low;
                guc_com_tx_buffer[(WRITE_STATE_CNT * 2) + 7 + 1] = com_crc.ubyte.high;
            }
        }
        else if(guc_com_bus_add == EXPAND_ADD)
        {
            if (bflg_read_write_data == 0)       //如果是读数据  
            {
                guc_com_tx_buffer[0] = 0x02;     //设备地址
                guc_com_tx_buffer[1] = 0x03;     //读命令
                
                guc_com_tx_buffer[2] = 0x00;
                guc_com_tx_buffer[3] = 0x07;     //起始地址7
                
                guc_com_tx_buffer[4] = 0x00;
                guc_com_tx_buffer[5] = 3;        //地址个数
                //----------------------------------------------
                com_crc.uword = CRC16(guc_com_tx_buffer, 6);
                guc_com_tx_buffer[6] = com_crc.ubyte.low;
                guc_com_tx_buffer[7] = com_crc.ubyte.high;
            }
            else    
            {
                guc_com_tx_buffer[0] = 0x02;     //设备地址
                guc_com_tx_buffer[1] = 0x10;     //写命令
                
                guc_com_tx_buffer[2] = 0x00;
                guc_com_tx_buffer[3] = 0x00;     //起始地址0
                
                guc_com_tx_buffer[4] = 0x00;
                guc_com_tx_buffer[5] = 7;        //地址个数  peak 要写几个寄存器，每个寄存器占2个字节；7*2 = 14 个字节；
                
                guc_com_tx_buffer[6] = 14;       //发送的数据字节数
                
                guc_com_tx_buffer[7] |= (uint8_t)(gss_factory_parm_value[DISP_FAC_Ct6]>>8);  //环温校准H-放大10
                guc_com_tx_buffer[8] |= (uint8_t)gss_factory_parm_value[DISP_FAC_Ct6];       //L
                guc_com_tx_buffer[9] |= (uint8_t)(gss_factory_parm_value[DISP_FAC_Ct7]>>8)；  //湿度校准
                guc_com_tx_buffer[10]|= (uint8_t)gss_factory_parm_value[DISP_FAC_Ct7];       //L
                guc_com_tx_buffer[11]|= (uint8_t)(gss_user_manage_parm_value[DISP_USER_MANAGE_S2]>>8); //最大杀菌时间--移液晶处理
                guc_com_tx_buffer[12]|= (uint8_t)gss_user_manage_parm_value[DISP_USER_MANAGE_S2];      //L
                guc_com_tx_buffer[13] = (uint8_t)gss_user_manage_parm_value[DISP_USER_MANAGE_S1];      //杀菌模式
                guc_com_tx_buffer[14] = 0x00;    //预留
                guc_com_tx_buffer[15] = 0x00;
                guc_com_tx_buffer[16] = 0x00;
                guc_com_tx_buffer[17] = 0x00;
                guc_com_tx_buffer[18] = 0x00;
                guc_com_tx_buffer[19] = 0x00;
                guc_com_tx_buffer[20] = 0x00;
                
                //----------------------------------------------
                com_crc.uword = CRC16(guc_com_tx_buffer, 21);
                guc_com_tx_buffer[21] = com_crc.ubyte.low;
                guc_com_tx_buffer[22] = com_crc.ubyte.high;
            }
        }

    }
    //------------------------------------------------------
    guc_com_tx_point = 0;
    bflg_com_tx_busy = 1;
    TXD0 = guc_com_tx_buffer[guc_com_tx_point++];
}
//------------------------------------------------------------------------------
/******************************************************************************
function:  在每次接收数据时，对其初始化；
           接收时，将要接收到缓存清空，准备接收新数据；

******************************************************************************/
void com_rx_init(void)   //通讯接收初始化程序，在主循环程序中调用
{
    uint8_t i;
    
    for (i = 0; i < 65; i++)       //20
    {
        guc_com_rx_buffer[i] = 0;  //清接收缓存器
    }
    
    bflg_com_rx_busy = 0;
    guc_com_rx_point = 0;
}
//------------------------------------------------------------------------------
/*******************************************************************************
function : 接收成功后，对数据进行处理 ；

           bflg_read_write_data--- 主机发送给显示板按照:发送查询指令、发送设定值
      这两个操作循环发送的方式；

        因为主板需要不停的判断此时按下的按键，比如增加"高温报警设定"后要改变判断的阀值了；
            显示板也需要知道此时主板有哪些故障，来进行显示；
        所以读/写要循环执行；
********************************************************************************/
void com_rx_data_deal(void)   //通讯接收数据处理程序，在主循环程序中调用
{
    uint8_t i,msb_byte = 0,lsb_byte = 0;
    uint16_t lus_tmp,lus_THW_tmp;
    word_type com_crc;
    word_type gss_tmp;
    //------------------------------------------------------
    if (bflg_com_type == 0)   //如果是LED显示屏通讯
    {
        if(guc_com_bus_add == LED_DIS_ADD)
        {
            if ((guc_com_rx_buffer[1] == 0x03))  //如果是读数据  //peak 读按键的值
            {
                com_crc.uword = CRC16(guc_com_rx_buffer, 7);
                if ((com_crc.ubyte.low == guc_com_rx_buffer[7])
                 && (com_crc.ubyte.high == guc_com_rx_buffer[8]))
                {
                    bflg_com_type_ok = 1;   //通讯类型判断ok
                    bflg_bus_rx_busying = 0;
                    
                    bflg_com_tx_delaytime = 1;         
                    gss_com_tx_delaytimer = 15;       //原10 
                    
                    bflg_com_fault = 0;               //清通讯故障标志
                    gss_com_fault_delaytimer = 0;     //清通讯故障计时器
                    //------------------------------------------
                    //数据处理
                    bflg_read_write_data = 1;         //变为写数据

                    guc_led_comm_cnt++;
                    if(guc_led_comm_cnt >= 20)  
                    {
                        guc_led_comm_cnt = 0;
                        guc_com_bus_add = EXPAND_ADD;     //切换到扩展板
                    }
                    
                    if (guc_key_val != guc_com_rx_buffer[3])
                    {
                        bflg_read_key_delaytime = 0; //peak 若键值有改变，则不需要等待一段时间后在判断键值，而是直接处理 read_key_deal()
                        gss_read_key_delaytimer = 0;
                    }
                    guc_key_val = guc_com_rx_buffer[3];    
            
                    //注册状态时显示的IC已用注册的地址，显示需转换为卡片的张数 = xx/4;
                    guc_flash_add = 0;
                    msb_byte = guc_com_rx_buffer[4];
                    lsb_byte = guc_com_rx_buffer[5];
                    guc_flash_add = ((guc_flash_add | msb_byte)<<8) | lsb_byte;
                     
                    //peak add 开锁指令
                    if ((guc_com_rx_buffer[6] & 0x80) != 0)
                    {
                          guc_IC_access_state = 1;
                    }
                    else
                    {
                          guc_IC_access_state = 0;
                    }
                    //--------------------------------------------------------
                    //U盘状态
                    guc_U_disk_state = (uint8_t)(guc_com_rx_buffer[6]&0x03);
                    guc_usb_state    = (uint8_t)(guc_com_rx_buffer[6]&0x04);
                    //------------------------------------------------------
                    //测试版本显示板确认
                    if ((guc_com_rx_buffer[6] & 0x40) != 0)
                    {
                          bflg_test_type = 1;
                    }
                    else
                    {
                          bflg_test_type = 0;
                    }
                }
            }
            else if (guc_com_rx_buffer[1] == 0x10)    //如果是写数据  //peak 设定数码管、LED显示  打算IC卡在此接收
            {
                com_crc.uword = CRC16(guc_com_rx_buffer, 6);
                if ((com_crc.ubyte.low == guc_com_rx_buffer[6])        //peak 如果CRC校验正确
                 && (com_crc.ubyte.high == guc_com_rx_buffer[7]))
                {
                    bflg_com_type_ok = 1;           //通讯类型判断ok      //peak ??
                    bflg_bus_rx_busying = 0;
                    
                    bflg_com_tx_delaytime = 1;      //置发送延时标志      //peak 之后时间到将再次允许发送
                    gss_com_tx_delaytimer = 10;     //发送延时计时器赋值
                    
                    bflg_com_fault = 0;             //清通讯故障标志      //peak ?? 未用
                    gss_com_fault_delaytimer = 0;   //清通讯故障计时器
                    //------------------------------------------
                    //数据处理
                    bflg_read_write_data = 0;       //变为读数据          //peak                     
                }
            }
        }
        else if(guc_com_bus_add == EXPAND_ADD)
        {
            if ((guc_com_rx_buffer[1] == 0x03))  //如果是读数据  
            {
                com_crc.uword = CRC16(guc_com_rx_buffer, 9);
                if ((com_crc.ubyte.low == guc_com_rx_buffer[9])
                 && (com_crc.ubyte.high == guc_com_rx_buffer[10]))
                {
                    bflg_com_type_ok = 1;   //通讯类型判断ok
                    bflg_bus_rx_busying = 0;
                    
                    bflg_com_tx_delaytime = 1;         
                    gss_com_tx_delaytimer = 15;        
                    
                    bflg_com_fault = 0;               //清通讯故障标志
                    gss_com_fault_delaytimer = 0;     //清通讯故障计时器
                    //------------------------------------------
                    //数据处理
                    bflg_read_write_data = 1;         //变为写数据

                    guc_com_bus_add = LED_DIS_ADD;    //切换到显示板
                    
                    lus_THW_tmp = 0;
                    lus_THW_tmp |= guc_com_rx_buffer[3];  //环温H
                    lus_THW_tmp = lus_THW_tmp<<8;
                    lus_THW_tmp |= guc_com_rx_buffer[4];
                    gss_THW = (int16_t)lus_THW_tmp + gss_factory_parm_value[DISP_FAC_Ct6];
		    
                    //guc_humidity = 0x00;                           //湿度H
                    guc_humidity = guc_com_rx_buffer[6];             //L

                    if((guc_com_rx_buffer[7]&0x01) == 0x01)
                    {
                        bflg_sterilization_state = 1;                //杀菌状态
                    }
                    else
                    {
                        bflg_sterilization_state = 0;
                    }
                    if((guc_com_rx_buffer[7]&0x02) == 0x02)
                    {
                        bflg_THW_fault = 1;                         //环温传感器故障
                        gus_trip_code1 |= THW_S_CODE;
                    }
                    else
                    {
                        bflg_THW_fault = 0;
                        gus_trip_code1 &= ~THW_S_CODE;
                    }
                    if((guc_com_rx_buffer[7]&0x04) == 0x04)
                    {
                        bflg_humidity_fault = 1;                   //湿度故障
                    }
                    else
                    {
                        bflg_humidity_fault = 0;
                    }
                    if((guc_com_rx_buffer[7]&0x08) == 0x08)
                    {
                        bflg_sterilize_fault = 1;                   //臭氧杀菌模块故障
                        gus_trip_code2 |= OZONE_F_CODE;                        
                    }
                    else
                    {
                        bflg_sterilize_fault = 0;
                        gus_trip_code2 &= ~OZONE_F_CODE;                        
                    }
                    //guc_com_rx_buffer[8];                         //预留
                }
            }
            else if (guc_com_rx_buffer[1] == 0x10)    //如果是写数据  //peak 设定数码管、LED显示  打算IC卡在此接收
            {
                com_crc.uword = CRC16(guc_com_rx_buffer, 6);
                if ((com_crc.ubyte.low == guc_com_rx_buffer[6])        //peak 如果CRC校验正确
                 && (com_crc.ubyte.high == guc_com_rx_buffer[7]))
                {
                    bflg_com_type_ok = 1;           //通讯类型判断ok      //peak ??
                    bflg_bus_rx_busying = 0;
                    
                    bflg_com_tx_delaytime = 1;      //置发送延时标志      //peak 之后时间到将再次允许发送
                    gss_com_tx_delaytimer = 10;     //发送延时计时器赋值
                    
                    bflg_com_fault = 0;             //清通讯故障标志      //peak ?? 未用
                    gss_com_fault_delaytimer = 0;   //清通讯故障计时器
                    //------------------------------------------
                    //数据处理
                    bflg_read_write_data = 0;       //变为读数据          //peak 
                }
            }
        }       
    }
    else  //如果是液晶屏通讯
    {
        if(guc_com_bus_add == LIQUID_DIS_ADD)    
        {
            if ((guc_com_rx_buffer[1] == 0x03))  //如果是读数据
            {
                if (bflg_read_user_admin == 0)   //如果是读用户信息
                {
                    com_crc.uword = CRC16(guc_com_rx_buffer, ((READ_USER_CNT * 2) + 3));
                    if ((com_crc.ubyte.low == guc_com_rx_buffer[(READ_USER_CNT * 2) + 3])
                     && (com_crc.ubyte.high == guc_com_rx_buffer[(READ_USER_CNT * 2) + 4]))
                    {
                        bflg_com_type_ok = 1;              //通讯类型判断ok
                        bflg_bus_rx_busying = 0;
                        
                        bflg_com_tx_delaytime = 1;         //置发送延时标志
                        gss_com_tx_delaytimer = 15;        //发送延时计时器赋值
                        
                        bflg_com_fault = 0;                //清通讯故障标志
                        gss_com_fault_delaytimer = 0;      //清通讯故障计时器
                        //----------------------------------------------------
                        //数据处理
                        //bflg_read_write_data = 1;        //变为写数据
                        bflg_read_user_admin = 1;          //变为读管理员信息
                        
                        //--------------------------------------
                        gss_tmp.ubyte.low = guc_com_rx_buffer[3];
                        gss_tmp.ubyte.high = guc_com_rx_buffer[4];
                        gss_user_set_parm_value[DISP_USER_SET_TS] = gss_tmp.sword;           //得到设定温度ok
                        //ram_para[num_user_set_TS] = gss_user_set_parm_value[DISP_USER_SET_TS];  
                        //--------------------------------------
                        gss_tmp.ubyte.low = guc_com_rx_buffer[5];
                        gss_tmp.ubyte.high = guc_com_rx_buffer[6];
                        gss_user_set_parm_value[DISP_USER_SET_ALH] = gss_tmp.sword;          //得到高温报警温度ok
                        //ram_para[num_user_set_ALH] = gss_user_set_parm_value[DISP_USER_SET_ALH];
                        //--------------------------------------
                        gss_tmp.ubyte.low = guc_com_rx_buffer[7];
                        gss_tmp.ubyte.high = guc_com_rx_buffer[8];
                        gss_user_set_parm_value[DISP_USER_SET_ALL] = gss_tmp.sword;          //得到低温报警温度ok
                        //ram_para[num_user_set_ALL] = gss_user_set_parm_value[DISP_USER_SET_ALL];
                        //--------------------------------------
                        gss_tmp.ubyte.low = guc_com_rx_buffer[9];
                        gss_tmp.ubyte.high = guc_com_rx_buffer[10];
                        gss_liquid_led_button = gss_tmp.sword;                           //箱内照明灯控制按钮ok
                        //--------------------------------------
                        gss_tmp.ubyte.low = guc_com_rx_buffer[11];
                        gss_tmp.ubyte.high = guc_com_rx_buffer[12];
                        gss_liquid_ozone_disinfect_button = gss_tmp.sword;               //臭氧杀菌启动按钮ok
                        //--------------------------------------
                        gss_tmp.ubyte.low = guc_com_rx_buffer[13];
                        gss_tmp.ubyte.high = guc_com_rx_buffer[14];
                        gss_user_manage_parm_value[DISP_USER_MANAGE_Lt] = gss_tmp.sword; //箱内照明灯延时关闭时间ok
                        //--------------------------------------
                        gss_tmp.ubyte.low = guc_com_rx_buffer[15];
                        gss_tmp.ubyte.high = guc_com_rx_buffer[16];
                        gss_user_manage_parm_value[DISP_USER_MANAGE_dA] = gss_tmp.sword; //开门蜂鸣器报警延时时间ok--处理移液晶屏上
                        //--------------------------------------
                        gss_tmp.ubyte.low = guc_com_rx_buffer[17];                 
                        gss_tmp.ubyte.high = guc_com_rx_buffer[18];
                        gss_user_manage_parm_value[DISP_USER_MANAGE_FA] = gss_tmp.sword; //蜂鸣器音量ok--处理移液晶屏上
                        //--------------------------------------
                        gss_tmp.ubyte.low = guc_com_rx_buffer[19];
                        gss_tmp.ubyte.high = guc_com_rx_buffer[20];
                        gss_user_manage_parm_value[DISP_USER_MANAGE_S1] = gss_tmp.sword; //杀菌模式ok
                        //--------------------------------------
                        gss_tmp.ubyte.low = guc_com_rx_buffer[21];
                        gss_tmp.ubyte.high = guc_com_rx_buffer[22];
                        gss_user_manage_parm_value[DISP_USER_MANAGE_S2] = gss_tmp.sword; //最大杀菌时间ok
                        //--------------------------------------
                        gss_tmp.ubyte.low = guc_com_rx_buffer[23];
                        gss_tmp.ubyte.high = guc_com_rx_buffer[24];
                        gss_user_manage_parm_value[DISP_USER_MANAGE_TT] = gss_tmp.sword; //温度单位ok--处理移液晶屏上
                        //--------------------------------------
                        gss_tmp.ubyte.low = guc_com_rx_buffer[25];
                        gss_tmp.ubyte.high = guc_com_rx_buffer[26];
                        ram_para[num_comp2_backup_TS] = gss_tmp.sword;         //年--待+
                        //--------------------------------------
                        gss_tmp.ubyte.low = guc_com_rx_buffer[27];
                        gss_tmp.ubyte.high = guc_com_rx_buffer[28];
                        ram_para[num_comp2_backup_TS] = gss_tmp.sword;         //月--待+
                        //--------------------------------------
                        gss_tmp.ubyte.low = guc_com_rx_buffer[29];
                        gss_tmp.ubyte.high = guc_com_rx_buffer[30];
                        ram_para[num_comp2_backup_TS] = gss_tmp.sword;         //日--待+
                        //--------------------------------------
                        gss_tmp.ubyte.low = guc_com_rx_buffer[31];
                        gss_tmp.ubyte.high = guc_com_rx_buffer[32];
                        ram_para[num_comp2_backup_TS] = gss_tmp.sword;         //时--待+
                        //--------------------------------------
                        gss_tmp.ubyte.low = guc_com_rx_buffer[33];
                        gss_tmp.ubyte.high = guc_com_rx_buffer[34];
                        ram_para[num_comp2_backup_TS] = gss_tmp.sword;         //分--待+
                        //--------------------------------------
                        gss_tmp.ubyte.low = guc_com_rx_buffer[35];
                        gss_tmp.ubyte.high = guc_com_rx_buffer[36];
                        ram_para[num_comp2_backup_TS] = gss_tmp.sword;         //秒--待                   
                    }
                }
                else    //如果是读管理员信息
                {
                    com_crc.uword = CRC16(guc_com_rx_buffer, ((READ_ADMIN_CNT * 2) + 3));
                    if ((com_crc.ubyte.low == guc_com_rx_buffer[(READ_ADMIN_CNT * 2) + 3])
                     && (com_crc.ubyte.high == guc_com_rx_buffer[(READ_ADMIN_CNT * 2) + 4]))
                    {
                        bflg_com_type_ok = 1;              //通讯类型判断ok
                        bflg_bus_rx_busying = 0;
                        
                        bflg_com_tx_delaytime = 1;         //置发送延时标志
                        gss_com_tx_delaytimer = 15;        //发送延时计时器赋值
                        
                        bflg_com_fault = 0;                //清通讯故障标志
                        gss_com_fault_delaytimer = 0;      //清通讯故障计时器
                        //--------------------------------------------------------
                        //数据处理
                        bflg_read_write_data = 1;          //变为写数据
                        bflg_read_user_admin = 0;          //变为读用户信息

                        guc_com_bus_add = EXPAND_ADD;      //切换扩展板地址
                        
                        //--------------------------------------
                        gss_tmp.ubyte.low = guc_com_rx_buffer[3];
                        gss_tmp.ubyte.high = guc_com_rx_buffer[4];
                        gss_factory_parm_value[DISP_FAC_r0] = gss_tmp.sword;     //控制温差ok
                        //--------------------------------------
                        gss_tmp.ubyte.low = guc_com_rx_buffer[5];
                        gss_tmp.ubyte.high = guc_com_rx_buffer[6];
                        gss_factory_parm_value[DISP_FAC_Ct1] = gss_tmp.sword;    //上传感器校准值ok
                        //--------------------------------------
                        gss_tmp.ubyte.low = guc_com_rx_buffer[7];
                        gss_tmp.ubyte.high = guc_com_rx_buffer[8];
                        gss_factory_parm_value[DISP_FAC_Ct2] = gss_tmp.sword;    //下传感器校准值ok
                        //--------------------------------------
                        gss_tmp.ubyte.low = guc_com_rx_buffer[9];
                        gss_tmp.ubyte.high = guc_com_rx_buffer[10];
                        gss_factory_parm_value[DISP_FAC_Ct3] = gss_tmp.sword;    //控制传感器校准值ok
                        //--------------------------------------
                        gss_tmp.ubyte.low = guc_com_rx_buffer[11];
                        gss_tmp.ubyte.high = guc_com_rx_buffer[12];
                        gss_factory_parm_value[DISP_FAC_Ct4] = gss_tmp.sword;    //化霜传感器校准值ok
                        //--------------------------------------
                        gss_tmp.ubyte.low = guc_com_rx_buffer[13];
                        gss_tmp.ubyte.high = guc_com_rx_buffer[14];
                        gss_factory_parm_value[DISP_FAC_Ct5] = gss_tmp.sword;    //冷凝传感器校准值ok
                        //--------------------------------------
                        gss_tmp.ubyte.low = guc_com_rx_buffer[15];
                        gss_tmp.ubyte.high = guc_com_rx_buffer[16];
                        gss_factory_parm_value[DISP_FAC_Ct6] = gss_tmp.sword;    //环温传感器校准值ok
                        //--------------------------------------
                        gss_tmp.ubyte.low = guc_com_rx_buffer[17];
                        gss_tmp.ubyte.high = guc_com_rx_buffer[18];
                        gss_factory_parm_value[DISP_FAC_Ct7] = gss_tmp.sword;    //湿度传感器校准值ok
                        //--------------------------------------
                        gss_tmp.ubyte.low = guc_com_rx_buffer[19];
                        gss_tmp.ubyte.high = guc_com_rx_buffer[20];
                        gss_factory_parm_value[DISP_FAC_CU] = gss_tmp.sword;     //市电电压校准值ok
                        //--------------------------------------
                        gss_tmp.ubyte.low = guc_com_rx_buffer[21];
                        gss_tmp.ubyte.high = guc_com_rx_buffer[22];
                        gss_factory_parm_value[DISP_FAC_Ad] = gss_tmp.sword;     //空气报警延时ok
                        //-------------------------------------- 
                        gss_tmp.ubyte.low = guc_com_rx_buffer[23];
                        gss_tmp.ubyte.high = guc_com_rx_buffer[24];
                        gss_factory_parm_value[DISP_FAC_AAH] = gss_tmp.sword;    //空气高温报警值ok
                        //--------------------------------------
                        gss_tmp.ubyte.low = guc_com_rx_buffer[25];
                        gss_tmp.ubyte.high = guc_com_rx_buffer[26];
                        gss_factory_parm_value[DISP_FAC_AAL] = gss_tmp.sword;    //空气低温报警值ok
                        //--------------------------------------
                        gss_tmp.ubyte.low = guc_com_rx_buffer[27];
                        gss_tmp.ubyte.high = guc_com_rx_buffer[28];
                        gss_factory_parm_value[DISP_FAC_r1] = gss_tmp.sword;     //冷凝器脏报警与环温温差ok                   
                        //--------------------------------------
                        gss_tmp.ubyte.low = guc_com_rx_buffer[29];
                        gss_tmp.ubyte.high = guc_com_rx_buffer[30];
                        gss_factory_parm_value[DISP_FAC_AHH] = gss_tmp.sword;    //环温过高报警值ok
                        //--------------------------------------
                        gss_tmp.ubyte.low = guc_com_rx_buffer[31];
                        gss_tmp.ubyte.high = guc_com_rx_buffer[32];
                        gss_factory_parm_value[DISP_FAC_T3] = gss_tmp.sword;     //压缩机最短停机时间ok
                        //--------------------------------------
                        gss_tmp.ubyte.low = guc_com_rx_buffer[33];
                        gss_tmp.ubyte.high = guc_com_rx_buffer[34];
                        gss_factory_parm_value[DISP_FAC_Cd] = gss_tmp.sword;     //初始上电压机开启延时ok
                        //--------------------------------------
                        gss_tmp.ubyte.low = guc_com_rx_buffer[35];
                        gss_tmp.ubyte.high = guc_com_rx_buffer[36];
                        gss_factory_parm_value[DISP_FAC_Fd] = gss_tmp.sword;     //蜂鸣器取消后再启动间隔延时ok
                        //--------------------------------------
                        gss_tmp.ubyte.low = guc_com_rx_buffer[37];
                        gss_tmp.ubyte.high = guc_com_rx_buffer[38];
                        gss_factory_parm_value[DISP_FAC_LC] = gss_tmp.sword;     //箱内照明灯控制方式ok
                        //--------------------------------------
                        gss_tmp.ubyte.low = guc_com_rx_buffer[39];
                        gss_tmp.ubyte.high = guc_com_rx_buffer[40];
                        gss_factory_parm_value[DISP_FAC_dFL] = gss_tmp.sword;    //进入化霜温度ok
                        //--------------------------------------
                        gss_tmp.ubyte.low = guc_com_rx_buffer[41];
                        gss_tmp.ubyte.high = guc_com_rx_buffer[42];
                        gss_factory_parm_value[DISP_FAC_dFH] = gss_tmp.sword;    //退出化霜温度ok
                        //--------------------------------------
                        gss_tmp.ubyte.low = guc_com_rx_buffer[43];
                        gss_tmp.ubyte.high = guc_com_rx_buffer[44];
                        gss_factory_parm_value[DISP_FAC_HT1] = gss_tmp.sword;    //门加热启动湿度ok
                        //--------------------------------------
                        gss_tmp.ubyte.low = guc_com_rx_buffer[45];
                        gss_tmp.ubyte.high = guc_com_rx_buffer[46];
                        gss_factory_parm_value[DISP_FAC_HT2] = gss_tmp.sword;    //门加热停止湿度ok
                        //--------------------------------------
                        gss_tmp.ubyte.low = guc_com_rx_buffer[47];
                        gss_tmp.ubyte.high = guc_com_rx_buffer[48];
                        gss_factory_parm_value[DISP_FAC_PCH] = gss_tmp.sword;    //血液柜/药品柜程序切换ok                 
                    }
                }
            }
            else if (guc_com_rx_buffer[1] == 0x10)    //如果是写数据
            {
                com_crc.uword = CRC16(guc_com_rx_buffer, 6);
                if ((com_crc.ubyte.low == guc_com_rx_buffer[6])
                 && (com_crc.ubyte.high == guc_com_rx_buffer[7]))
                {
                    bflg_com_type_ok = 1;             //通讯类型判断ok
                    bflg_bus_rx_busying = 0;
                    
                    bflg_com_tx_delaytime = 1;        //置发送延时标志
                    gss_com_tx_delaytimer = 15;       //发送延时计时器赋值
                    
                    bflg_com_fault = 0;               //清通讯故障标志
                    gss_com_fault_delaytimer = 0;     //清通讯故障计时器
                    //----------------------------------------------------
                    //数据处理
                    bflg_read_write_data = 0;         //变为读数据
                }
            }
        }
        else if(guc_com_bus_add == EXPAND_ADD)
        {
            if ((guc_com_rx_buffer[1] == 0x03))  //如果是读数据  
            {
                com_crc.uword = CRC16(guc_com_rx_buffer, 9);
                if ((com_crc.ubyte.low == guc_com_rx_buffer[9])
                 && (com_crc.ubyte.high == guc_com_rx_buffer[10]))
                {
                    bflg_com_type_ok = 1;   //通讯类型判断ok
                    bflg_bus_rx_busying = 0;
                    
                    bflg_com_tx_delaytime = 1;         
                    gss_com_tx_delaytimer = 15;        
                    
                    bflg_com_fault = 0;               //清通讯故障标志
                    gss_com_fault_delaytimer = 0;     //清通讯故障计时器
                    //------------------------------------------
                    //数据处理
                    bflg_read_write_data = 1;         //变为写数据

                    guc_com_bus_add = LIQUID_DIS_ADD;    //切换到液晶屏
                    
                    lus_THW_tmp = 0;
                    lus_THW_tmp |= guc_com_rx_buffer[3];  //环温H
                    lus_THW_tmp = lus_THW_tmp<<8;
                    lus_THW_tmp |= guc_com_rx_buffer[4];
                    gss_THW = (int16_t)lus_THW_tmp + gss_factory_parm_value[DISP_FAC_Ct6];
		    
                    //guc_humidity = 0x00;                           //湿度H
                    guc_humidity = guc_com_rx_buffer[6];             //L

                    if((guc_com_rx_buffer[7]&0x01) == 0x01)
                    {
                        bflg_sterilization_state = 1;                //杀菌状态
                    }
                    else
                    {
                        bflg_sterilization_state = 0;
                    }
                    if((guc_com_rx_buffer[7]&0x02) == 0x02)
                    {
                        bflg_THW_fault = 1;                         //环温传感器故障
                        gus_trip_code1 |= THW_S_CODE;
                    }
                    else
                    {
                        bflg_THW_fault = 0;
                        gus_trip_code1 &= ~THW_S_CODE;
                    }
                    if((guc_com_rx_buffer[7]&0x04) == 0x04)
                    {
                        bflg_humidity_fault = 1;                   //湿度故障
                    }
                    else
                    {
                        bflg_humidity_fault = 0;
                    }
                    if((guc_com_rx_buffer[7]&0x08) == 0x08)
                    {
                        bflg_sterilize_fault = 1;                   //臭氧杀菌模块故障
                        gus_trip_code2 |= OZONE_F_CODE;                        
                    }
                    else
                    {
                        bflg_sterilize_fault = 0;
                        gus_trip_code2 &= ~OZONE_F_CODE;                        
                    }
                    //guc_com_rx_buffer[8];                         //预留
                }
            }
            else if (guc_com_rx_buffer[1] == 0x10)    //如果是写数据  //peak 设定数码管、LED显示  打算IC卡在此接收
            {
                com_crc.uword = CRC16(guc_com_rx_buffer, 6);
                if ((com_crc.ubyte.low == guc_com_rx_buffer[6])        //peak 如果CRC校验正确
                 && (com_crc.ubyte.high == guc_com_rx_buffer[7]))
                {
                    bflg_com_type_ok = 1;           //通讯类型判断ok      //peak ??
                    bflg_bus_rx_busying = 0;
                    
                    bflg_com_tx_delaytime = 1;      //置发送延时标志      //peak 之后时间到将再次允许发送
                    gss_com_tx_delaytimer = 15;     //发送延时计时器赋值
                    
                    bflg_com_fault = 0;             //清通讯故障标志      //peak ?? 未用
                    gss_com_fault_delaytimer = 0;   //清通讯故障计时器
                    //------------------------------------------------------------
                    //数据处理
                    bflg_read_write_data = 0;       //变为读数据           
                }
            }
        }
    }
}
//------------------------------------------------------------------------------
/*******************************************************************************
function:  bflg_com_rx_delaytime 置1后，延时gss_com_rx_delaytimer 设定的时间后，
           可以打开中断允许接收；
*******************************************************************************/
void com_rx_delaytime(void)     //通讯接收延时程序，在1ms定时程序中调用
{
    if (bflg_com_rx_delaytime == 1)
    {
        gss_com_rx_delaytimer--;
        if (gss_com_rx_delaytimer <= 0)
        {
            gss_com_rx_delaytimer = 0;
            bflg_com_rx_delaytime = 0;
            
            bflg_com_allow_rx = 1;
        }
    }
}
//------------------------------------------------------------------------------
/*******************************************************************************
function:  bflg_com_tx_delaytime 此标志置1后，延时gss_com_tx_delaytimer 设定的时间
           后，打开发送中断，允许发送；
          
*******************************************************************************/
void com_tx_delaytime(void)     //通讯发送延时程序，在1ms定时程序中调用
{
    if (bflg_com_tx_delaytime == 1)
    {
        gss_com_tx_delaytimer--;
        if (gss_com_tx_delaytimer <= 0)
        {
            gss_com_tx_delaytimer = 0;
            bflg_com_tx_delaytime = 0;
            
            bflg_com_allow_tx = 1;
        }
    }
}
//------------------------------------------------------------------------------
/*******************************************************************************
function: 接收数据时，多长时间后，仍未正确接收到数据，或是没有接收完，
          则停止数据的接收，也不对接收的数据进行任何的处理；
          
*******************************************************************************/
void com_rx_end_delaytime(void) //通讯接收完成延时程序，在1ms定时程序中调用
{
    /*if (bflg_com_rx_busy == 1)  //如果当前接收忙
    {
        gss_com_rx_end_delaytimer++;
        if (gss_com_rx_end_delaytimer >= 50)
        {
            gss_com_rx_end_delaytimer = 0;
            bflg_com_rx_busy = 0;
            
            bflg_com_rx_end = 1;
            
            bflg_com_tx_delaytime = 1;
            gss_com_tx_delaytimer = 10;
        }
    }*/
}
//------------------------------------------------------------------------------
/******************************************************************************
function: 没有对发生故障时的情况下进行相应的处理；
            

函数位置:1s定时器中-------------------------ok
******************************************************************************/
void com_fault_delaytime(void)     //通讯故障延时程序，在1s定时程序中调用
{
    if (bflg_com_fault == 0)                //peak 这个判断了并没有起作用
    {
        gss_com_fault_delaytimer++;
        if (gss_com_fault_delaytimer >= 30)  //30
        {
            gss_com_fault_delaytimer = 0;
            bflg_com_fault = 1;
        }
    }
    //---------------------------------------------------------------------
    //通讯类型再判断
    if (bflg_com_type_ok == 0)               //通讯正确后 = 1；不正确则为0；
    {
        guc_com_type_cnt++;                  //peak 开始通讯时，发送5次数据
        if (guc_com_type_cnt >= 5)
        {   
            guc_com_type_cnt = 0;            //peak+
            
            bflg_com_type = ~bflg_com_type;  

            if(bflg_com_type == 0)
            {
                guc_com_bus_add = LED_DIS_ADD;
            }
            else
            {
                guc_com_bus_add = LIQUID_DIS_ADD;
            }
        }
    }
}
//------------------------------------------------------------------------------
/******************************************************************************
function :中断中处理；
          发送要发送的内容----> |1、发送读显示板数据的指令  (8个字节)
                                |2、发送向显示板写的数据    (23个字节)
    
******************************************************************************/
void com_tx_int(void)    //通讯发送中断程序
{
    if (bflg_com_tx_busy == 1)
    {
        if (bflg_com_type == 0)    //如果是LED显示屏通讯
        {
            if(guc_com_bus_add == LED_DIS_ADD)
            {
                if (bflg_read_write_data == 0)   //如果是读数据
                {
                    if (guc_com_tx_point < 8)
                    {
                        TXD0 = guc_com_tx_buffer[guc_com_tx_point++];     //发送数据
                    }
                    else
                    {
                        guc_com_tx_point = 0;    //清发送字节索引
                        bflg_com_tx_busy = 0;    //清发送忙标志
                        bflg_com_tx_ok = 1;
                        
                        bflg_com_rx_delaytime = 1;    //置com通讯接收延时标志
                        gss_com_rx_delaytimer = 5;    //com通讯接收延时计时器赋值

                        /*if(bflg_com_type_ok == 0)
                        {
                            bflg_com_tx_delaytime = 1;    //放到485切换那-----调试液晶去掉注释
                            gss_com_tx_delaytimer = 100;
                        }*/
                    }
                }
                else    //如果是写数据
                {
                    if (guc_com_tx_point < 53)  //23
                    {
                        TXD0 = guc_com_tx_buffer[guc_com_tx_point++];     //发送数据
                    }
                    else
                    {
                        guc_com_tx_point = 0;    //清发送字节索引  
                        bflg_com_tx_busy = 0;    //清发送忙标志    
                        bflg_com_tx_ok = 1;
                        
                        bflg_com_rx_delaytime = 1;    //置通讯接收延时标志
                        gss_com_rx_delaytimer = 5;    //通讯接收延时计时器赋值
                        
                        if(bflg_com_type_ok == 0)
                        {
                            bflg_com_tx_delaytime = 1;    //未确定显示板用，因确认显示板是用写指令所以只在此处
                            gss_com_tx_delaytimer = 100;
                        }
                    }
                }
            }
            else if(guc_com_bus_add == EXPAND_ADD)
            {
                if (bflg_read_write_data == 0)   //如果是读数据
                {
                    if (guc_com_tx_point < 8)
                    {
                        TXD0 = guc_com_tx_buffer[guc_com_tx_point++];     //发送数据
                    }
                    else
                    {
                        guc_com_tx_point = 0;    //清发送字节索引
                        bflg_com_tx_busy = 0;    //清发送忙标志
                        bflg_com_tx_ok = 1;
                        
                        bflg_com_rx_delaytime = 1;    //置com通讯接收延时标志
                        gss_com_rx_delaytimer = 5;    //com通讯接收延时计时器赋值

                        //bflg_com_tx_delaytime = 1;
                        //gss_com_tx_delaytimer = 100;
                    }
                }
                else    //如果是写数据
                {
                    if (guc_com_tx_point < 23)
                    {
                        TXD0 = guc_com_tx_buffer[guc_com_tx_point++];     //发送数据
                    }
                    else
                    {
                        guc_com_tx_point = 0;    //清发送字节索引
                        bflg_com_tx_busy = 0;    //清发送忙标志
                        bflg_com_tx_ok = 1;
                        
                        bflg_com_rx_delaytime = 1;    //置通讯接收延时标志
                        gss_com_rx_delaytimer = 5;    //通讯接收延时计时器赋值
                        
                        //bflg_com_tx_delaytime = 1;
                        //gss_com_tx_delaytimer = 100;
                    }
                }
            }
        }
        else   //如果是液晶屏通讯
        {
            if(guc_com_bus_add == LIQUID_DIS_ADD)
            {
                if (bflg_read_write_data == 0)   //如果是读数据
                {
                    if (guc_com_tx_point < 8)
                    {
                        TXD0 = guc_com_tx_buffer[guc_com_tx_point++];     //发送数据
                    }
                    else
                    {
                        guc_com_tx_point = 0;    //清发送字节索引
                        bflg_com_tx_busy = 0;    //清发送忙标志
                        bflg_com_tx_ok = 1;
                        
                        bflg_com_rx_delaytime = 1;    //置com通讯接收延时标志
                        gss_com_rx_delaytimer = 5;    //com通讯接收延时计时器赋值
                        
                        //bflg_com_tx_delaytime = 1;
                        //gss_com_tx_delaytimer = 1000;
                    }
                }
                else    //如果是写数据
                {
                    if (guc_com_tx_point < ((WRITE_STATE_CNT * 2) + 9))   //109
                    {
                        TXD0 = guc_com_tx_buffer[guc_com_tx_point++];     //发送数据
                    }
                    else
                    {
                        guc_com_tx_point = 0;     //清发送字节索引
                        bflg_com_tx_busy = 0;     //清发送忙标志
                        bflg_com_tx_ok = 1;
                        
                        bflg_com_rx_delaytime = 1;     //置通讯接收延时标志
                        gss_com_rx_delaytimer = 5;     //通讯接收延时计时器赋值
                        
                        if(bflg_com_type_ok == 0)
                        {
                            bflg_com_tx_delaytime = 1;    
                            gss_com_tx_delaytimer = 1000;
                        }
                        //debug 通讯指示灯
                        TRIP_LAMP_PIN = ~TRIP_LAMP_PIN;
                    }
                }
            }
            else if(guc_com_bus_add == EXPAND_ADD)
            {
                if (bflg_read_write_data == 0)   //如果是读数据
                {
                    if (guc_com_tx_point < 8)
                    {
                        TXD0 = guc_com_tx_buffer[guc_com_tx_point++];     //发送数据
                    }
                    else
                    {
                        guc_com_tx_point = 0;    //清发送字节索引
                        bflg_com_tx_busy = 0;    //清发送忙标志
                        bflg_com_tx_ok = 1;
                        
                        bflg_com_rx_delaytime = 1;    //置com通讯接收延时标志
                        gss_com_rx_delaytimer = 5;    //com通讯接收延时计时器赋值

                        //bflg_com_tx_delaytime = 1;
                        //gss_com_tx_delaytimer = 100;
                    }
                }
                else    //如果是写数据
                {
                    if (guc_com_tx_point < 23)
                    {
                        TXD0 = guc_com_tx_buffer[guc_com_tx_point++];     //发送数据
                    }
                    else
                    {
                        guc_com_tx_point = 0;    //清发送字节索引
                        bflg_com_tx_busy = 0;    //清发送忙标志
                        bflg_com_tx_ok = 1;
                        
                        bflg_com_rx_delaytime = 1;    //置通讯接收延时标志
                        gss_com_rx_delaytimer = 5;    //通讯接收延时计时器赋值
                        
                        //bflg_com_tx_delaytime = 1;
                        //gss_com_tx_delaytimer = 100;
                    }
                }
            }
        }
    }
}
//------------------------------------------------------------------------------
/******************************************************************************
function: 中断接收的数据

          *****LED****
          如果收到的是发送完查询命令后返回的值:
          共收到9个字节。功能书中可以无限扩展的，但是此程序发送查询设定为固定的     
      查询2个寄存器，所以返回时也是固定的4个字节(一个寄存器返回2个字节);
      
       ----------------------------------------   
          如果收到的是发送完设定命令后返回的值:
          共收到8个字节，功能书中就是8个；

*******************************************************************************/
void com_rx_int(uint8_t rx_data)   //通讯接收中断程序
{
    uint8_t luc_com_rx_byte;  //接收字节临时缓冲区
    
    gss_com_rx_end_delaytimer = 0; //清接收完延时计时器
    //--------------------------------------------------
    luc_com_rx_byte = rx_data;
    
    if (bflg_com_type == 0)   //如果是LED显示屏通讯
    {
        if ((bflg_com_rx_busy == 0) && ((luc_com_rx_byte == 0x00)||(luc_com_rx_byte == 0x02)))     //如果无接收忙，且从地址正确
        {
            bflg_com_rx_busy = 1;
            
            guc_com_rx_buffer[0] = luc_com_rx_byte;
            guc_com_rx_point = 1;
        }      
        else if (bflg_com_rx_busy == 1)      //如果接收忙
        {
            guc_com_rx_buffer[guc_com_rx_point++] = luc_com_rx_byte;
            if(guc_com_rx_buffer[0] == 0x00)   //接收LED显示板
            {
                if ((guc_com_rx_buffer[1] == 0x03) && (guc_com_rx_point >= 9))       //如果是查询命令，且接收数据完毕
                {
                    guc_com_rx_point = 0;   //清接收字节索引
                    bflg_com_rx_busy = 0;   //清接收忙标志
                    bflg_com_rx_ok = 1;     //置接收成功标志
                }
                else if ((guc_com_rx_buffer[1] == 0x10) && (guc_com_rx_point >= 8))  //如果是多个设定命令，且接收到字节数数据帧
                {
                    guc_com_rx_point = 0;    
                    bflg_com_rx_busy = 0;    
                    bflg_com_rx_ok = 1;      
                }
                else if(guc_com_rx_point > 9)
                {
                    guc_com_rx_point = 0;    
                    bflg_com_rx_busy = 0;   
                    bflg_com_rx_ok = 1;    //同接收完的处理
                }                
            }
            else if(guc_com_rx_buffer[0] == 0x02)   //接收拓展板
            {
                if ((guc_com_rx_buffer[1] == 0x03) && (guc_com_rx_point >= 11))       //如果是查询命令，且接收数据完毕
                {
                    guc_com_rx_point = 0;   //清接收字节索引
                    bflg_com_rx_busy = 0;   //清接收忙标志
                    bflg_com_rx_ok = 1;     //置接收成功标志
                }
                else if ((guc_com_rx_buffer[1] == 0x10) && (guc_com_rx_point >= 8))  //如果是多个设定命令，且接收到字节数数据帧
                {
                    guc_com_rx_point = 0;    
                    bflg_com_rx_busy = 0;    
                    bflg_com_rx_ok = 1;      
                }
                else if(guc_com_rx_point > 11)
                {
                    guc_com_rx_point = 0;    
                    bflg_com_rx_busy = 0;   
                    bflg_com_rx_ok = 1;       //同接收完的处理
                }                 
            }
        }
    }
    else  //如果是液晶屏通讯
    {
        if ((bflg_com_rx_busy == 0) && ((luc_com_rx_byte == 0x01)||(luc_com_rx_byte == 0x02)))     //如果无接收忙，且从地址正确
        {
            bflg_com_rx_busy = 1;
            
            guc_com_rx_buffer[0] = luc_com_rx_byte;
            guc_com_rx_point = 1;
        }
        else if (bflg_com_rx_busy == 1)      //如果接收忙
        {
            guc_com_rx_buffer[guc_com_rx_point++] = luc_com_rx_byte;
            
            if(guc_com_rx_buffer[0] == 0x01)          //液晶地址
            {
                if (guc_com_rx_buffer[1] == 0x10)     //如果是多个设定命令
                {
                    if (guc_com_rx_point >= 8)   //接收到字节数数据帧
                    {
                        guc_com_rx_point = 0;    //清接收字节索引
                        bflg_com_rx_busy = 0;    //清接收忙标志
                        bflg_com_rx_ok = 1;      //置接收成功标志
                    }
                }
                else if (guc_com_rx_buffer[1] == 0x03)     //如果是读信息
                {
                    if (bflg_read_user_admin == 0)    //如果是读用户信息
                    {
                        if (guc_com_rx_point >= ((READ_USER_CNT * 2) + 5))      //65
                        {
                            guc_com_rx_point = 0;     //清接收字节索引
                            bflg_com_rx_busy = 0;     //清接收忙标志
                            bflg_com_rx_ok = 1;       //置接收成功标志
                        }
                    }
                    else     //如果是读管理员信息
                    {
                        if (guc_com_rx_point >= ((READ_ADMIN_CNT * 2) + 5))     //65
                        {
                            guc_com_rx_point = 0;     //清接收字节索引
                            bflg_com_rx_busy = 0;     //清接收忙标志
                            bflg_com_rx_ok = 1;       //置接收成功标志
                        }
                    }
                }
                else if(guc_com_rx_point > 65)
                {
                    guc_com_rx_point = 0;    
                    bflg_com_rx_busy = 0;   
                    bflg_com_rx_ok = 1;    //同接收完的处理
                }
            }
            else if(guc_com_rx_buffer[0] == 0x02)     //扩展板地址
            {
                if ((guc_com_rx_buffer[1] == 0x03) && (guc_com_rx_point >= 11))       //如果是查询命令，且接收数据完毕
                {
                    guc_com_rx_point = 0;   //清接收字节索引
                    bflg_com_rx_busy = 0;   //清接收忙标志
                    bflg_com_rx_ok = 1;     //置接收成功标志
                }
                else if ((guc_com_rx_buffer[1] == 0x10) && (guc_com_rx_point >= 8))  //如果是多个设定命令，且接收到字节数数据帧
                {
                    guc_com_rx_point = 0;    
                    bflg_com_rx_busy = 0;    
                    bflg_com_rx_ok = 1;      
                }
                else if(guc_com_rx_point > 11)
                {
                    guc_com_rx_point = 0;    
                    bflg_com_rx_busy = 0;   
                    bflg_com_rx_ok = 1;       //同接收完的处理
                }                 
            }  
        }
    }
}

/*********************************************************************************************************************************************
函数功能:两个从机的地址的切换(在未成功接收到其中一个从机数据时，自动切换到另一个从机)
         1、没有接收到数据，超时变地址(如未接扩展板)
         2、收到地址不对(第一个字节)，超时变地址；
         3、收到数据CRC错误，超时变地址；
         
         
函数位置：1ms定时器--------------------------------------------------ok
**********************************************************************************************************************************************/
void com_485_bus_switch(void)
{ 
    if(bflg_com_type_ok == 1)                        //未判定是LED还是液晶屏不进行和扩展板的地址切换
    {
        if(bflg_bus_rx_busying == 1)                 //总线在接收状态
        {
            if(bflg_com_type == 0)                   //和LED屏连接
            {
                if(guc_com_bus_add == LED_DIS_ADD)   
                {
                    guc_dis_bus_delaytime++;
                    if(guc_dis_bus_delaytime >= 50)    //最多收led屏9个数据
                    {
                        guc_dis_bus_delaytime = 0;
                        
                        R_UART0_Stop();
                        bflg_com_rx_ok = 0;            //即使现在收完数据也作废
                        
                        guc_led_comm_cnt = 0;
                        
                        guc_com_bus_add = EXPAND_ADD;
                        bflg_read_write_data = 1;      //------------每次换地址从头开始
                        bflg_bus_rx_busying = 0;

                        bflg_com_tx_delaytime = 1;     //别开停频繁   
                        gss_com_tx_delaytimer = 5; 
                    }
                }
                else 
                {
                    guc_dis_bus_delaytime = 0;
                }
                //---------------------------------------------------------------------
                if(guc_com_bus_add == EXPAND_ADD)         //拓展板占据总线 
                {
                    guc_expand_bus_delaytime++;
                    if(guc_expand_bus_delaytime >= 50)   //最多收扩展板11个数据
                    {   
                        guc_expand_bus_delaytime = 0;
                        
                        R_UART0_Stop();
                        bflg_com_rx_ok = 0;        
                        
                        guc_com_bus_add = LED_DIS_ADD;
                        bflg_read_write_data = 1;     
                        bflg_bus_rx_busying = 0;
                        
                        bflg_com_tx_delaytime = 1;       
                        gss_com_tx_delaytimer = 5; 
                    }
                }
                else
                {
                    guc_expand_bus_delaytime = 0;
                }
            }
            else                                     //和液晶屏连接
            {
                if(guc_com_bus_add == LIQUID_DIS_ADD)   
                {
                    guc_dis_bus_delaytime++;
                    if(guc_dis_bus_delaytime >= 500)  //最多接收液晶屏65个数据
                    {
                        guc_dis_bus_delaytime = 0;
                        
                        R_UART0_Stop();
                        bflg_com_rx_ok = 0;            //即使现在收完数据也作废
                        
                        guc_com_bus_add = EXPAND_ADD;
                        bflg_read_write_data = 1;      //------------每次换地址从头开始
                        bflg_read_user_admin = 0;
                        bflg_bus_rx_busying = 0;
                        
                        bflg_com_tx_delaytime = 1;     //别开停频繁   
                        gss_com_tx_delaytimer = 5;   
                    }
                }
                else 
                {
                    guc_dis_bus_delaytime = 0;
                }
                //---------------------------------------------------------------
                if(guc_com_bus_add == EXPAND_ADD)        //拓展板占据总线 
                {
                    guc_expand_bus_delaytime++;
                    if(guc_expand_bus_delaytime >= 100)   //最多收扩展板11个数据
                    {   
                        guc_expand_bus_delaytime = 0;
                        
                        R_UART0_Stop();
                        bflg_com_rx_ok = 0;           
                        
                        guc_com_bus_add = LIQUID_DIS_ADD;
                        bflg_read_write_data = 1;         
                        bflg_read_user_admin = 0;
                        bflg_bus_rx_busying = 0;
                        
                        bflg_com_tx_delaytime = 1;         
                        gss_com_tx_delaytimer = 5;   
                    }
                }
                else
                {
                    guc_expand_bus_delaytime = 0;
                }
            }
        }
        else
        {
            guc_dis_bus_delaytime = 0;
            guc_expand_bus_delaytime = 0;
        }
    }
}


//------------------------------------------------------------------------------
unsigned int CRC16(unsigned char *puchmsg, unsigned int usDataLen)    //进行CRC校验的程序
{
    unsigned int uchCRC;
    unsigned int uIndex_x, uIndex_y;
    
    uchCRC = 0xFFFF;
    
    if (usDataLen > 0)
    {
        for (uIndex_x = 0; uIndex_x <= (usDataLen - 1); uIndex_x++)
        {
            uchCRC = uchCRC ^ (unsigned int) (*puchmsg);
            puchmsg++;
            
            for (uIndex_y = 0; uIndex_y <= 7; uIndex_y++)
            {
                if ((uchCRC & 0x0001) != 0)
                {
                    uchCRC = (uchCRC >> 1) ^ 0xA001;
                }
                else
                {
                    uchCRC = uchCRC >> 1;
                }
            }
        }
    }
    else
    {
        uchCRC = 0;
    }
    
    return uchCRC;
}

//------------------------------------------------------------------------------------------------
//peak add
/*********************************************************************************************************************************************
函数功能: 判断是否有报警事件发送，不像设置类的，通过手动操作就知道是否发送

函数位置: bflg_com_allow_tx == 1 中 ----------------------------ok
**********************************************************************************************************************************************/
void com_tx_usb_rank(void)
{   
    //---------------------------------------------------------------------------高温报警 1
    if(bflg_alarm_ALH != bflg_usb_ALH_temp)      //高温报警     
    {  
        bflg_usb_ALH_temp = bflg_alarm_ALH;

        if(bflg_alarm_ALH == 1)           //要求只在报警时发送
        {
            bflg_sent_usb_ALH = 1;        //发送初始化此事件
        }
    }
    //---------------------------------------------------------------------------低温报警 2
    else if(bflg_alarm_ALL != bflg_usb_ALL_temp)  //低温报警
    {
        bflg_usb_ALL_temp  = bflg_alarm_ALL;

        if(bflg_alarm_ALL == 1)
        {
            bflg_sent_usb_ALL = 1;
        }
    }
    //---------------------------------------------------------------------------环温超标报警 3
    else if(bflg_alarm_THW_high != bflg_usb_THW_temp) //环温超标报警
    {
        bflg_usb_THW_temp  = bflg_alarm_THW_high;

        if(bflg_alarm_THW_high == 1)
        {
            bflg_sent_usb_THW = 1;
        }
    }
    //---------------------------------------------------------------------------开关门 8
    else if(bflg_prot2_door1_pin != bflg_usb_DOR_temp)  //开关门状态
    {
        bflg_usb_DOR_temp = bflg_prot2_door1_pin;
        bflg_sent_usb_DOR = 1;

        if(bflg_prot2_door1_pin == 1)
        {
            bflg_usb_DOR_open = 1;
        }
        else
        {
            bflg_usb_DOR_open = 0;
        }
    }
    //---------------------------------------------------------------------------上电断电 9
    else if(bflg_alarm_power_off != bflg_usb_POW_temp)  //上/断电状态
    {
        bflg_usb_POW_temp = bflg_alarm_power_off;
        bflg_sent_usb_POW = 1;

        if(bflg_alarm_power_off == 0)
        {
            bflg_usb_POW_on = 1;     //上电
        }
        else
        {
            bflg_usb_POW_on = 0;
        }
    }
    //---------------------------------------------------------------------------电池报警 10
    else if(bflg_alarm_bat_discnnt != bflg_usb_BAT_discnnt_temp) 
    {
        bflg_usb_BAT_discnnt_temp = bflg_alarm_bat_discnnt;//电池未连接
        if(bflg_alarm_bat_discnnt == 1)
        {
            bflg_sent_usb_BAT = 1;
        }
    }
    else if(bflg_alarm_bat_fed != bflg_usb_BAT_fed_temp)
    {
        bflg_usb_BAT_fed_temp = bflg_alarm_bat_fed;       //电池插反
        if(bflg_alarm_bat_fed == 1)
        {
            bflg_sent_usb_BAT = 1;
        }
    }
    else if(bflg_alarm_bat_low != bflg_usb_BAT_low_temp)
    {
        bflg_usb_BAT_low_temp = bflg_alarm_bat_low;       //电池电量低
        if(bflg_alarm_bat_low == 1)
        {
            bflg_sent_usb_BAT = 1;
        }
    }
    //---------------------------------------------------------------------------传感器故障 12
    else if(bflg_THW_fault != bflg_usb_THW_fault_temp)//环温传感器故障
    {
        bflg_usb_THW_fault_temp = bflg_THW_fault;
        if(bflg_THW_fault == 1)
        {
            bflg_sent_usb_FAT_THW = 1;
            //bflg_sent_usb_FAT = 1;
        }
    }
    /*else if(bflg_TC_2_fault != bflg_usb_TC_2_fault_temp) //热交换器
    {
        bflg_usb_TC_2_fault_temp = bflg_TC_2_fault;
        if(bflg_TC_2_fault == 1)
        {
             bflg_sent_usb_FAT_TC = 1;   //bflg_sent_usb_FAT_TE = 1; //双系统的热交换变为冷凝
            //bflg_sent_usb_FAT = 1;
        }
    }*/
    else if(bflg_TC_fault != bflg_usb_TC_fault_temp) //冷凝器传感器故障
    {
        bflg_usb_TC_fault_temp = bflg_TC_fault;
        if(bflg_TC_fault == 1)
        {
            bflg_sent_usb_FAT_TC = 1;
            //bflg_sent_usb_FAT = 1;
        }
    }
    else if(bflg_TA_fault != bflg_usb_TA_fault_temp) //主传感器故障
    {
        bflg_usb_TA_fault_temp = bflg_TA_fault;
        if(bflg_TA_fault == 1)
        {
            bflg_sent_usb_FAT_TA = 1;
            //bflg_sent_usb_FAT = 1;
        }
    }
    //---------------------------------------------------------------------------冷凝器脏报警16
    else if(bflg_alarm_cond_dirty != bflg_usb_TC_temp)    //冷凝器脏       
    {
        bflg_usb_TC_temp = bflg_alarm_cond_dirty;
        if(bflg_alarm_cond_dirty == 1)
        {
            bflg_sent_usb_TC = 1;
        }
    }
    //---------------------------------------------------------------------------断电报警
    else if(bflg_alarm_power_off != bflg_usb_power_off_temp) //断电报警
    {
        bflg_usb_power_off_temp = bflg_alarm_power_off;
        if(bflg_alarm_power_off == 1)
        {
            bflg_sent_usb_POW_OFF = 1;
        }
    }
    //---------------------------------------------------------------------------开门报警
    else if(bflg_alarm_open_door != bflg_usb_open_door_temp)  //开门报警
    {
        bflg_usb_open_door_temp = bflg_alarm_open_door;
        if(bflg_alarm_open_door == 1)
        {
            bflg_sent_usb_DOOR_OPEN = 1;
        }
    }
    //---------------------------------------------------------------------------空气高温报警
    
    else if(bflg_alarm_high_AIR != bflg_usb_high_AIR_temp)  //空气高温报警
    {
        bflg_usb_high_AIR_temp = bflg_alarm_high_AIR;
        if(bflg_alarm_high_AIR ==1)
        {
            bflg_sent_usb_AIR_H = 1;
        }
    }
    //---------------------------------------------------------------------------空气低温报警
    else if(bflg_alarm_low_AIR != bflg_usb_low_AIR_temp)  //空气低温报警
    {
        bflg_usb_low_AIR_temp = bflg_alarm_low_AIR;
        if(bflg_alarm_low_AIR ==1)
        {
            bflg_sent_usb_AIR_L = 1;
        }
    }    
    //---------------------------------------------------------------------------化霜传感器高温报警
    else if(bflg_alarm_higt_TE != bflg_usb_higt_TE_temp)  //化霜传感器高温报警
    {
        bflg_usb_higt_TE_temp = bflg_alarm_higt_TE;
        if(bflg_alarm_higt_TE ==1)
        {
            bflg_sent_usb_TE_H = 1;
        }
    }    
    //---------------------------------------------------------------------------臭氧模块故障
    else if(bflg_sterilize_fault != bflg_usb_sterilize_fault_temp)  //臭氧模块故障
    {
        bflg_usb_sterilize_fault_temp = bflg_sterilize_fault;
        if(bflg_sterilize_fault ==1)
        {
            bflg_sent_usb_ozone_FAT = 1;
        }        
    }    
}
/**********************************************************************************************************************
函数功能:  初始上电时，主板延时一段时间再发送usb事件，等待usb板的时间同步完成，
           否则记录这些事件的时间 会错误；
           
函数位置:  1s延时函数中 ------------------------------------------ok
**********************************************************************************************************************/
void com_usb_delay_time(void)  
{
    if(bflg_usb_delaytime == 1)
    {
        gss_usb_delaytime++;
        if(gss_usb_delaytime >= 25)
        {
            gss_usb_delaytime = 0;
            bflg_usb_delaytime = 0;
        }
    }
}
#endif
/*********************************************END OF THE FILE*************************************************************************/
