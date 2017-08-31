#ifndef _COM_H_
#define _COM_H_
//------------------------------------------------------------------------------
//外部函数声明
extern void com_init(void);   //通讯初始化程序，在初始化程序中调用

extern void com_rx_init(void);     //通讯接收初始化程序，在主循环程序中调用

extern void com_rx_data_deal(void);     //通讯接收数据处理程序，在主循环程序中调用

extern void com_tx_init(void);     //通讯发送数据处理程序，在主循环程序中调用

extern void com_rx_delaytime(void);     //通讯接收延时程序，在1ms定时程序中调用

extern void com_tx_delaytime(void);     //通讯发送延时程序，在1ms定时程序中调用

extern void com_rx_end_delaytime(void); //通讯接收结束延时程序，在1ms定时程序中调用

extern void com_fault_delaytime(void);  //通讯故障延时程序，在1s定时程序中调用

extern void com_tx_int(void); //通讯发送中断程序

extern void com_rx_int(uint8_t rx_data);//通讯接收中断程序

extern void com_485_bus_switch(void);

void com_tx_usb_rank(void);

void com_usb_delay_time(void);  


//------------------------------------------------------------------------------
//外部变量声明//
extern flag_type flg_com;

          #define   bflg_com_tx_delaytime    flg_com.bits.bit0   //通讯发送延时标志
          #define   bflg_com_rx_delaytime    flg_com.bits.bit1   //通讯接收延时标志
          #define   bflg_com_allow_tx        flg_com.bits.bit2   //通讯允许发送标志
          #define   bflg_com_allow_rx        flg_com.bits.bit3   //通讯允许接收标志
          #define   bflg_com_tx_busy         flg_com.bits.bit4   //通讯发送忙标志
          #define   bflg_com_rx_busy         flg_com.bits.bit5   //通讯接收忙标志
          #define   bflg_com_tx_ok           flg_com.bits.bit6   //通讯发送成功标志
          #define   bflg_com_rx_ok           flg_com.bits.bit7   //通讯接收成功标志
          #define   bflg_com_rx_end          flg_com.bits.bit8   //通讯接收结束标志
          
          #define   bflg_com_fault           flg_com.bits.bit9   //通讯故障标志
          
          #define   bflg_com_type            flg_com.bits.bit10  //通讯类型标志     //peak  0:LED  1:液晶
          #define   bflg_com_type_ok         flg_com.bits.bit11  //通讯类型判断ok标志

          //peak add
          #define   bflg_lock_delaytimer     flg_com.bits.bit12  //开锁的延时标志
          #define   bflg_bus_rx_busying      flg_com.bits.bit13  //总线正在处于接收状态，用于超时管理
//-----------------------------------------------------------------------------------------------------------------------------------------
extern flag_type flg_read_write;
          #define   bflg_read_write_data     flg_read_write.bits.bit0      //读写数据标志
          #define   bflg_read_user_admin     flg_read_write.bits.bit1      //读用户管理员标志
          
//-----------------------------------------------------------------------------------------------------------------------------------------
extern flag_type flg_sent_usb_control;      //发送usb的控制类
          #define   bflg_sent_usb_CL2        flg_sent_usb_control.bits.bit0  //发送给清除usb数据  主-->显-->usb
          #define   bflg_sent_usb_T1         flg_sent_usb_control.bits.bit1  //发送给设置usb记录频率
          #define   bflg_sent_usb_P6         flg_sent_usb_control.bits.bit2  //发送给设置usb导数模式
		  #define   bflg_sent_usb_UL         flg_sent_usb_control.bits.bit3  //发送给设置usb语言类型 
          
extern flag_type flg_sent_usb_event;        //发送usb的事件类
         #define   bflg_sent_usb_ALH         flg_sent_usb_event.bits.bit0    //高温报警
         #define   bflg_sent_usb_ALL         flg_sent_usb_event.bits.bit1    //低温报警
         #define   bflg_sent_usb_THW         flg_sent_usb_event.bits.bit2    //环温超标报警
         #define   bflg_sent_usb_ALHS        flg_sent_usb_event.bits.bit3    //高温报警设定
         #define   bflg_sent_usb_ALLS        flg_sent_usb_event.bits.bit4    //低温报警设定
         #define   bflg_sent_usb_TS          flg_sent_usb_event.bits.bit5    //箱内温度设定
         #define   bflg_sent_usb_DOR         flg_sent_usb_event.bits.bit6    //开门/关门
         #define   bflg_sent_usb_POW         flg_sent_usb_event.bits.bit7    //上电/断电 状态
         #define   bflg_sent_usb_BAT         flg_sent_usb_event.bits.bit8    //电池报警
         //....
         #define   bflg_sent_usb_FAT_TA      flg_sent_usb_event.bits.bit9    //主传感器故障
         #define   bflg_sent_usb_FAT_TE      flg_sent_usb_event.bits.bit10   //化霜(热交换)..
         #define   bflg_sent_usb_FAT_THW     flg_sent_usb_event.bits.bit11   //环温..
         #define   bflg_sent_usb_FAT_TC      flg_sent_usb_event.bits.bit12   //冷凝..
         #define   bflg_sent_usb_FAT_TU      flg_sent_usb_event.bits.bit13   //上温..
         #define   bflg_sent_usb_FAT_TD      flg_sent_usb_event.bits.bit14   //下温..
         #define   bflg_sent_usb_IC          flg_sent_usb_event.bits.bit15   //IC卡注销
                  
extern flag_type flg_sent_usb_event_1;        //发送usb的事件类
         #define   bflg_sent_usb_TC          flg_sent_usb_event_1.bits.bit0   //冷凝器脏报警
         #define   bflg_sent_usb_POW_OFF     flg_sent_usb_event_1.bits.bit1   //断电报警
         #define   bflg_sent_usb_DOOR_OPEN   flg_sent_usb_event_1.bits.bit2   //开门报警
         #define   bflg_sent_usb_AIR_H       flg_sent_usb_event_1.bits.bit3   //空气高温报警
         #define   bflg_sent_usb_AIR_L       flg_sent_usb_event_1.bits.bit4   //空气低温报警
         #define   bflg_sent_usb_TE_H        flg_sent_usb_event_1.bits.bit5   //化霜传感器高温报警
         #define   bflg_sent_usb_ozone_FAT   flg_sent_usb_event_1.bits.bit6   //臭氧模块故障
          
extern flag_type flg_usb_event_temp;
        #define   bflg_usb_ALH_temp          flg_usb_event_temp.bits.bit0    //高温报警
        #define   bflg_usb_ALL_temp          flg_usb_event_temp.bits.bit1    //低温报警
        #define   bflg_usb_THW_temp          flg_usb_event_temp.bits.bit2    //环温超标报警
        #define   bflg_usb_ALHS_temp         flg_usb_event_temp.bits.bit3    //高温报警设定
        #define   bflg_usb_ALLS_temp         flg_usb_event_temp.bits.bit4    //低温报警设定
        #define   bflg_usb_TS_temp           flg_usb_event_temp.bits.bit5    //箱内温度设定
        #define   bflg_usb_DOR_temp          flg_usb_event_temp.bits.bit6    //开门/关门
        #define   bflg_usb_POW_temp          flg_usb_event_temp.bits.bit7    //上电/断电 状态
        #define   bflg_usb_BAT_discnnt_temp  flg_usb_event_temp.bits.bit8    //电池未接报警
        #define   bflg_usb_BAT_fed_temp      flg_usb_event_temp.bits.bit9    //电池插反报警
        #define   bflg_usb_BAT_low_temp      flg_usb_event_temp.bits.bit10   //电池电量低报警
        #define   bflg_usb_TA_fault_temp     flg_usb_event_temp.bits.bit11   //主传感器故障
        #define   bflg_usb_TE_fault_temp     flg_usb_event_temp.bits.bit12   //化霜(热交换)..
        #define   bflg_usb_THW_fault_temp    flg_usb_event_temp.bits.bit13   //环温..
        #define   bflg_usb_TC_fault_temp     flg_usb_event_temp.bits.bit14   //冷凝..
        #define   bflg_usb_TU_fault_temp     flg_usb_event_temp.bits.bit15   //上温..
          
extern flag_type flg_usb_event_temp_1;
        #define   bflg_usb_TD_fault_temp         flg_usb_event_temp_1.bits.bit0   //下温..
        #define   bflg_usb_IC_temp               flg_usb_event_temp_1.bits.bit1   //IC卡注销
        #define   bflg_usb_TC_temp               flg_usb_event_temp_1.bits.bit2   //冷凝器脏报警
        #define   bflg_usb_power_off_temp        flg_usb_event_temp_1.bits.bit3   //断电报警
        #define   bflg_usb_open_door_temp        flg_usb_event_temp_1.bits.bit4   //开门报警
        #define   bflg_usb_high_AIR_temp         flg_usb_event_temp_1.bits.bit5   //空气高温报警
        #define   bflg_usb_low_AIR_temp          flg_usb_event_temp_1.bits.bit6   //空气低温报警
        #define   bflg_usb_higt_TE_temp          flg_usb_event_temp_1.bits.bit7   //化霜传感器高温报警
        #define   bflg_usb_sterilize_fault_temp  flg_usb_event_temp_1.bits.bit8   //臭氧模块故障

extern flag_type flg_usb_event_attribute;       
        #define   bflg_usb_DOR_open              flg_usb_event_attribute.bits.bit0    //门是开or关
        #define   bflg_usb_POW_on                flg_usb_event_attribute.bits.bit1    //上电or断电
        #define   bflg_usb_delaytime             flg_usb_event_attribute.bits.bit2    //发送给usb的故障事件延时后再发送--在海尔调试
                  

//------------------------------------------------------------------------------
//外部变量 
extern uint8_t  guc_IC_access_state;          // 开锁的状态
extern uint16_t guc_flash_add;                // 接收的IC卡注册已用的地址
extern uint16_t guc_temp_flash_add;           // 接收的IC卡注册临时保存的地址
extern uint8_t  guc_IC_erasure_tx_ok_count;   // 注销IC卡指令，在bflg_com_tx_ok中设置发送多次计数,防止丢包
extern int16_t  gss_liquid_led_button;        // 液晶屏的开灯指令
extern uint8_t  guc_com_bus_add;
extern uint8_t  gss_usb_delaytime;             //初次上电延时xs后在发送usb事件
extern uint8_t  guc_U_disk_state;              //显示板发送过来的U盘状态
extern uint8_t  guc_usb_state;                 //显示板发送过来的usb板的状态
extern int16_t  gss_liquid_fingerprint_unlocked;    //液晶屏的指纹开锁

//------------------------------------------------------------------------------
#endif
