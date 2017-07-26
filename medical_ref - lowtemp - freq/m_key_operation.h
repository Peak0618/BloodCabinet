#ifndef _M_KEY_OPERATION_H_
#define _M_KEY_OPERATION_H_

//外部函数
extern void mode_selection(void);
extern void key_locked_delaytime(void);   
extern void key_set_long_delaytime(void);
extern void key_set_up_long_delaytime(void);
extern void key_up_down_long_delaytime(void);
extern void key_set_down_long_delaytime(void);
extern void operation_parm_read(void);
extern void IC_unlock_deal(void);
extern void IC_unlock_delaytime(void);
extern void key_password_long_valid_delaytime(void);
extern void key_RTC_time_parm_sent_usb_delaytime(void); 



//按键值
#define   KEY_NONE              0x00     //无按键按下
#define   KEY_SET               0x08     //设定
#define   KEY_UP                0x10     //上调
#define   KEY_DOWN              0x01     //下调
#define   KEY_BUZ               0x02     //报警测试
#define   KEY_LAMP              0x04     //灯
#define   KEY_SET_AND_UP        0x18     //设定+上调  工厂
#define   KEY_UP_AND_DOWN       0x11     //上调+下调  查询
#define   KEY_SET_AND_DOWN      0x09     // IC密码



//主干_层
extern uint8_t   guc_disp_mode;
#define   DISP_LOCKED                  0   //锁定模式
#define   DISP_USER_SET                1   //用户设定层
#define   DISP_TIME_SET                2   //usb时间设置
#define   DISP_USER_MANAGE_PASSWORD    3   //用户管理层密码
#define   DISP_USER_MANAGE             4   //用户管理层
#define   DISP_FACTORY_PASSWORD        5   //工厂密码模式
#define   DISP_FACTORY                 6   //工厂模式
#define   DISP_INQUIRE                 7   //查询模式
#define   DISP_IC_PASSWORD             8   //IC卡密码模式
#define   DISP_IC_REGISTER             9   //IC卡注册模式

extern int8_t   gsc_disp_parm_type;
//分支_用户设定层参数
#define   DISP_USER_SET_TS                  0   //箱内温度设定值         magnify 10 使用时不需放大10
#define   DISP_USER_SET_ALH                 1   //高温报警值           magnify 10
#define   DISP_USER_SET_ALL                 2   //低温报警值           magnify 10
#define   DISP_USER_SET_T1                  3   //usb取数周期 
#define   DISP_USER_SET_T2                  4   //usb时间
#define   DISP_USER_SET_P6                  5   //usb导数模式
#define   DISP_USER_SET_SS                  6   //用户管理层参数设定
#define   DISP_USER_SET_SUM                 7   //用户设定层参数总个数

//分支_用户管理层参数
#define   DISP_USER_MANAGE_IC               0  //ic卡注册/用户管理层密码修改
#define   DISP_USER_MANAGE_CL1              1  //ic卡注销--这个不能为0，因为参数默认时为0，在IC密码处理易混
#define   DISP_USER_MANAGE_Lt               2  //箱内照明灯延时关闭时间or取消关闭延时
#define   DISP_USER_MANAGE_dA               3  //开门蜂鸣器报警延时
#define   DISP_USER_MANAGE_FA               4  //蜂鸣器音量
#define   DISP_USER_MANAGE_S1               5  //杀菌模式
#define   DISP_USER_MANAGE_S2               6  //最大杀菌时间
#define   DISP_USER_MANAGE_TT               7  //温度单位
#define   DISP_USER_MANAGE_SUM              8  //管理层参数总数

//分支_工厂参数
#define  DISP_FAC_CL2                       0   //usb板出厂数据清空
#define  DISP_FAC_UL                        1   //usb导出数据语言选择
#define  DISP_FAC_r0                        2   //控制温差                magnify 10
#define  DISP_FAC_b0F                       3   //变频压机转速控制温差           use need magnify 10
#define  DISP_FAC_br1                       4   //变频压机转速变量1
#define  DISP_FAC_br2                       5   //变频压机转速变量2
#define  DISP_FAC_br3                       6   //变频压机转速变量3
#define  DISP_FAC_br4                       7   //变频压机转速变量4
#define  DISP_FAC_Ct1                       8   //上传感器校准值             magnify 10
#define  DISP_FAC_Ct2                       9   //下传感器校准值             magnify 10
#define  DISP_FAC_Ct3                       10  //控制传感器校准值            magnify 10
#define  DISP_FAC_Ct4                       11  //化霜(热交换)传感器校准值            magnify 10
#define  DISP_FAC_Ct5                       12  //冷凝器传感器校准值           magnify 10 
#define  DISP_FAC_Ct6                       13  //环温传感器校准值            magnify 10
#define  DISP_FAC_Ct7                       14  //湿度传感器校准值
#define  DISP_FAC_CU                        15  //市电电压校准
#define  DISP_FAC_Ad                        16  //空气报警延迟
#define  DISP_FAC_AAH                       17  //空气高温报警值                use need magnify 10（因数码管不能显示和方便按键++），使用这些参数时需要放大10，因为温度都放大了10倍
#define  DISP_FAC_AAL                       18  //空气低温报警值                use need magnify 10
#define  DISP_FAC_r1                        19  //冷凝器脏报警与环温温差            use need magnify 10
#define  DISP_FAC_AHH                       20  //环温过高报警                 use need magnify 10
#define  DISP_FAC_T3                        21  //压缩机最短停机时间     
#define  DISP_FAC_Cd                        22  //初始上电压机开启延时
#define  DISP_FAC_Fd                        23  //蜂鸣器取消后再启动间隔时间              use need magnify 10
#define  DISP_FAC_LC                        24  //箱内照明灯控制方式哪种门
#define  DISP_FAC_CL3                       25  //ic卡注册密码/用户管理层密码初始化
#define  DISP_FAC_CL4                       26  //恢复出厂值
#define  DISP_FAC_dFL                       27  //进入化霜温度                 use need magnify 10
#define  DISP_FAC_dFH                       28  //退出化霜温度                 use need magnify 10
#define  DISP_FAC_HT1                       29  //门加热启动湿度
#define  DISP_FAC_HT2                       30  //门加热停止湿度
#define  DISP_FAC_PS                        31  //密码--工厂
#define  DISP_FAC_PCH                       32  //血液柜药品柜程序切换
#define  DISP_FAC_SUM                       33  //工厂层参数总数

//分支_查询参数
extern int8_t  gsc_disp_inq_mode;
#define  DISP_INQ_d1                        0   //上传感器
#define  DISP_INQ_d2                        1   //下传感器
#define  DISP_INQ_d3                        2   //控制传感器
#define  DISP_INQ_d4                        3   //化霜传感器
#define  DISP_INQ_d5                        4   //冷凝传感器
#define  DISP_INQ_d6                        5   //环温传感器
#define  DISP_INQ_d7                        6   //环境湿度
#define  DISP_INQ_d8                        7   //市电电压
#define  DISP_INQ_d9                        8   //电池电量



//外部标志
extern flag_type bflg_key_operation;
       #define   bflg_key_set_long_valid          bflg_key_operation.bits.bit0   //长按set键有效
       #define   bflg_key_set_long_delaytime      bflg_key_operation.bits.bit1   //长按set键计时
       #define   bflg_key_set_up_long_valid       bflg_key_operation.bits.bit2   //长按set+up键有效
       #define   bflg_key_set_up_long_delaytime   bflg_key_operation.bits.bit3   //长按set+up键计时
       #define   bflg_key_up_down_long_valid      bflg_key_operation.bits.bit4   //长按up+down键有效
       #define   bflg_key_up_down_long_delaytime  bflg_key_operation.bits.bit5   //长按up+down键计时
       #define   bflg_key_set_down_long_valid     bflg_key_operation.bits.bit6   //长按set+down键有效
       #define   bflg_key_set_down_long_delaytime bflg_key_operation.bits.bit7   //长按set+down键计时
       #define   bflg_parm_dis_form               bflg_key_operation.bits.bit8   //显示 0:参数标识 or 1:参数值
       #define   bflg_key_inq_val                 bflg_key_operation.bits.bit9   //查询层进入参数值
       
extern flag_type bflg_key_operation_save;
       #define   bflg_parm_save                         bflg_key_operation_save.bits.bit0   //是否对参数保存 1未保存 0保存
       #define   bflg_usb_set_time_delaytime            bflg_key_operation_save.bits.bit1   //usb设置时间延时
       #define   bflg_usb_set_time                      bflg_key_operation_save.bits.bit2   //表示有设置时间动作让usb修改时间
       #define   bflg_key_password_long_valid           bflg_key_operation_save.bits.bit3   //IC注册、解锁、工厂3类密码长按xs后按加10的步进增加
       #define   bflg_key_password_long_valid_delaytime bflg_key_operation_save.bits.bit4   //输入密码时长按延时

//变量定义
extern uint8_t   guc_key_val;                      //按键键值
extern uint8_t   guc_TUTD_dis_type;                //选择显示上温 或下温或上下温平均值
extern uint8_t   guc_key_set_long_delaytimer;      //长按set键计时
extern uint8_t   guc_key_set_up_long_delaytimer;   //长按set+up键计时
extern uint8_t   guc_key_up_down_long_delaytimer;  //长按up+down键计时
extern uint8_t   guc_key_set_down_long_delaytimer; //长按set+down键计时
extern uint8_t   guc_key_locked_delaytimer;        //按键无操作锁定延时
extern uint8_t   guc_disp_mode;                    //总的UI模式

extern int16_t   gss_user_manage_password_val;    //用户管理密码值
extern int16_t   gss_IC_password_val;             //ic卡密码值
extern int8_t    gsc_factory_password_val;         //工厂密码值
extern int8_t    gsc_disp_parm_type;               //user设定层的不同参数
extern int8_t    gsc_disp_inq_mode;                //查询模式中不同参数

extern int16_t   gss_key_temp_value;               //操作时的临时值
//
extern int16_t   gss_TS_old_value;                 //设置温度的旧值         
extern int16_t   gss_ALH_old_value;                //设置高温报警的旧值
extern int16_t   gss_ALL_old_value;                //设置低温报警的旧值
extern int8_t    guc_user_write_time_value; 

extern int16_t   gss_user_set_parm_value[DISP_USER_SET_SUM];       
extern int16_t   gss_user_manage_parm_value[DISP_USER_MANAGE_SUM]; 
extern int16_t   gss_factory_parm_value[DISP_FAC_SUM]; 













#endif
/*********************************************END OF THE FILE********************************************************/
