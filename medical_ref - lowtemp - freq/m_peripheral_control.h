#ifndef _M_PERIPHERAL_CONTROL_H_
#define _M_PERIPHERAL_CONTROL_H_


//函数声明
extern void defrosting_control(void);
extern void defrosting_delaytime(void);
extern void defrosting_interval_delaytime(void);
extern void door_heat_control(void);
extern void door_heat_RL_out(void);
extern void inside_fan_control(void);
extern void fan_RL_out(void);
extern void led_control(void);
extern void liquid_led_control(void);
extern void led_on_delaytime(void);
extern void led_RL_out(void);
extern void remote_alarm_lamp(void);
extern void remote_alarm_test_delaytime(void);


//flag
extern flag_type flg_peripheral;
       #define   bflg_defrosting_time                    flg_peripheral.bits.bit0  //除霜时间
       #define   bflg_defrosting_interval_time           flg_peripheral.bits.bit1  //除霜间隔
       #define   bflg_HT_fault                           flg_peripheral.bits.bit2  //温湿度传感器故障
       #define   bflg_heat_runing                        flg_peripheral.bits.bit3  //电加热丝运行
       #define   bflg_inside_fan_runing                  flg_peripheral.bits.bit4  //内部风机运转
       #define   bflg_led_on                             flg_peripheral.bits.bit5  //led灯亮  
       #define   bflg_led_on_delaytime                   flg_peripheral.bits.bit6  //按键开灯延时
       #define   bflg_key_open_led                       flg_peripheral.bits.bit7  //是按键开的灯
       #define   bflg_key_close_led                      flg_peripheral.bits.bit8  //是按键关的灯
       #define   bflg_sterilization_state                flg_peripheral.bits.bit9  //杀菌状态
       #define   bflg_beam_heat_runing                   flg_peripheral.bits.bit10 //中梁加热丝状态-----关，目前没有
       #define   bflg_water_box_heat_runing              flg_peripheral.bits.bit11 //接水盒加热丝状态---一直开
       #define   bflg_locd_runing                        flg_peripheral.bits.bit12 //电磁锁状态

extern flag_type flg_peripheral2;
       #define   bflg_remote_alarm_test                 flg_peripheral2.bits.bit1   //按"测试报警"时远程报警灯测试


//variare
extern uint8_t  guc_humidity;                         //湿度
       
       

//macro
#define   RL_ALM_OUT_PIN             P6.5       //远程报警继电器输出

//----------------------------------------------------------
//继电器输出口宏定义
#define   RL_COMP1_OUT_PIN    P7.4      //压缩机1继电器输出口     
#define   RL_COMP2_OUT_PIN    P7.5      //压缩机2继电器输出口      

#define   RL_FAN1_OUT_PIN     P7.3      //风机1继电器输出口           RL7  bflg_outfan20_running
#define   RL_FAN2_OUT_PIN     P4.2      //风机2继电器输出口         

#define   RL_HTRMXG_OUT_PIN   P4.1      //毛细管加热丝继电器输出口  

#define   RL_HTRGK_OUT_PIN    P13.0     //柜口加热丝继电器输出口    
 
//#define   RL_ALM_OUT_PIN      P6.5      //远程报警继电器输出口      =1 是打开报警灯  

#define   RL_DOWN_OUT_PIN     P15.6     //降压继电器输出口          
#define   RL_UP_OUT_PIN       P10.0     //升压继电器输出口          
//----------------------------------------------------------
//12V输出口宏定义
//#define   LOCK_OUT_PIN        P11.0   //电磁锁输出口     

#define   HTRMT_OUT_PIN       P11.1     //门体加热丝输出口   
#define   HTRMXG_OUT_PIN      P14.6     //毛细管加热丝输出口 
#define   HTRPHK_OUT_PIN      P14.7     //平衡口加热丝输出口 
//----------------------------------------------------------
#define   BUZZ_OUT_PIN        P3.1      //蜂鸣器输出口       
//----------------------------------------------------------
#define   TFT_OUT_PIN         P10.1     //液晶屏电源输出口   
  
//----------------------------------------------------------
//开关输入口宏定义
#define   DOOR1_IN_PIN        P5.2      //门开关1输入口  

#define   BAT_FED_IN_PIN      P0.5      //电池插反输入口  
//----------------------------------------------------------
//拨码开关输入口宏定义
//拨码开关输入口宏定义
#define   SW11_IN_PIN         P5.1      //拨码开关输入口
#define   SW12_IN_PIN         P5.0      //拨码开关输入口
#define   SW13_IN_PIN         P3.0      //拨码开关输入口
#define   SW14_IN_PIN         P8.7      //拨码开关输入口
#define   SW15_IN_PIN         P8.6      //拨码开关输入口
#define   SW16_IN_PIN         P8.5      //拨码开关输入口
#define   SW17_IN_PIN         P8.4      //拨码开关输入口
#define   SW18_IN_PIN         P8.3      //拨码开关输入口

#define   SW21_IN_PIN         P0.6      //拨码开关输入口
#define   SW22_IN_PIN         P7.0      //拨码开关输入口
#define   SW23_IN_PIN         P7.1      //拨码开关输入口
#define   SW24_IN_PIN         P7.2      //拨码开关输入口







#endif 
