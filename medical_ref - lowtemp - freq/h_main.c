/*****************************************************************************************************************************
overview:
        1��ѪҺ���������
*******************************************************************************************************************************/

#ifndef _MAIN_C_
#define _MAIN_C_

//---------new--------

//------------------------------------------------------------------------------
//�����ļ�����
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
#include "h_main.h"
#include "h_com.h"
#include "h_adg.h"
#include "h_ad.h"
#include "h_key_disp.h"
#include "h_rom_para2.h"
//#include "h_comp2.h"
#include "h_protect.h"
//#include "h_volt_ctrl.h"
#include "h_bat_ctrl.h"

//peak add
#include "M_buzz.h"
#include "m_compressor.h"
#include "m_peripheral_control.h"
#include "m_key_operation.h"
#include "m_test.h"
#include "M_RTC.h"

//------------------------------------------------------------------------------
//��������
void system_init(void);  //ϵͳ��ʼ������

void switch_init(void);  //���뿪�س�ʼ��

void control_init(void); //���Ƴ�ʼ������

void system_start(void); //ϵͳ��������

void main_loop(void);    //��ѭ������

void timer_op(void);     //��ʱ��������ѭ�������е���

void ad_convert_deal(void);   //adת�����������1ms��ʱ�����е���

void timer_int(void);    //1ms��ʱ�жϳ���

void init_ram_para2(void);

void init_ram_para(void);

void eeprom2_read_deal(void); //eeprom2��ȡ��������ڳ�ʼ�������е���

void eeprom_read_deal(void);  //eeprom��ȡ��������ڳ�ʼ�������е���

void delay_10us(uint16_t _10us);   //10us��ʱ������eeprom��д�����е���

void delay_1ms(uint16_t _1ms);     //1ms��ʱ������eeprom��д�����е���

void eeprom_write(uint16_t lus_addr, int16_t lss_eep_data);      //дeeprom����

int16_t eeprom_read(uint16_t lus_addr); //��eeprom����

void RL_output_delaytime(void);    //�̵��������ʱ������1ms��ʱ�����е���

void trip_lampblink_deal(void);    //���ϵ���˸������10ms��ʱ�����е���

void input_pin_delaytime(void);    //����������ʱ������10ms��ʱ�����е���


//------------------------------------------------------------------------------
//485ͨѶ����ģʽ�궨��
#define   COM_RX_MODE    (P1.0 = 0)     //����ģʽ
#define   COM_TX_MODE    (P1.0 = 1)     //����ģʽ

#define   NET_RX_MODE    (P0.4 = 0)     //����ģʽ
#define   NET_TX_MODE    (P0.4 = 1)     //����ģʽ






//------------------------------------------------------------------------------
//��������
flag_type flg_time;
//----------------------------------------------------------
uint8_t   guc_10ms_timer;     //10�����ʱ��
uint8_t   guc_100ms_timer;    //100ms��ʱ��
uint8_t   guc_1s_timer;       //1s��ʱ��
uint8_t   guc_1min_timer;     //1min��ʱ��

uint8_t   guc_RL_output_delaytimer;     //�̵��������ʱ��ʱ��

uint8_t   guc_trip_flash_cnt;      //���ϵ���˸������
uint8_t   guc_trip_flash_timer;    //���ϵ���˸��ʱ��

uint8_t   guc_trip_code; //���ϴ���

int16_t   gss_tmp;
//----------------------------------------------------------
uint8_t   guc_door1_pin_high_delaytimer;     //�ſ���1���Ÿ���ʱ��ʱ��
uint8_t   guc_door1_pin_low_delaytimer;      //�ſ���1���ŵ���ʱ��ʱ��

uint8_t   guc_door2_pin_high_delaytimer;     //�ſ���2���Ÿ���ʱ��ʱ��
uint8_t   guc_door2_pin_low_delaytimer;      //�ſ���2���ŵ���ʱ��ʱ��

uint8_t   guc_bat_fed_pin_high_delaytimer;   //��ط��巴���Ÿ���ʱ��ʱ��
uint8_t   guc_bat_fed_pin_low_delaytimer;    //��ط��巴���ŵ���ʱ��ʱ��
//----------------------------------------------------------


const int16_t gss_buzzer_register[10] = 
{
  900,    //1
  800,    //2
  900,    //3
  1000,   //4
  1100,   //5
  1200,   //6
  1300,   //7
  1400,   //8
  1500,   //9
  1600    //10
};
//------------------------------------------------------------------------------
//��������//
//------------------------------------------------------------------------------
void system_init(void)   //ϵͳ��ʼ������
{
    bflg_self_test = 1;       //��ʼ�ϵ�ʱϵͳ�Լ��־
    
    switch_init();            //���뿪�س�ʼ��
    //----------------------------------
    init_ram_para2();         // peak ��дE2���°���Ҫ��дһ�Σ�
    init_ram_para();
    
    eeprom2_read_deal();     //eeprom2��ȡ�������
    eeprom_read_deal();      //eeprom��ȡ�������
    
    operation_parm_read();     //�Ӷ�ȡ��E2�������л�ȡ��Ҫ��ֵ

    //----------------------------------
    R_ADC_Set_OperationOn();  //adת������
    //----------------------------------
    com_init();     //ͨѶ��ʼ������
    //----------------------------------
    adg_init();     //adģ���ʼ������
    //----------------------------------
    control_init(); //���Ƴ�ʼ������
    //----------------------------------
    RTC_init();
}

/*****************************************************************************************************************************************
�������ܣ����뿪�س�ʼ��

����λ�ã�ϵͳ��ʼ��(1)--------------------------------------------ok
*******************************************************************************************************************************************/
void switch_init(void)        
{
    if (SW21_IN_PIN == 1)          //��·Ĭ��Ϊ�ߵ�ƽ
    {
        //bflg_comp2_volt_type = 0;  //220V����
    }
    else
    {
        //bflg_comp2_volt_type = 1;  //115V����
    }
}

/*****************************************************************************************************************************************
�������ܣ�IO�ڳ�ʼ����������־��ʼ��

����λ�ã�ϵͳ��ʼ��(1)------------------------ok
*******************************************************************************************************************************************/
void control_init(void)  
{
    //------------------------------------------------------
    //12V��ʼ���ر�
    LOCK_OUT_PIN = 0;  //LOCK_OUT_PIN = 1; peak ԭ����
    HTRMT_OUT_PIN = 0;
    HTRMXG_OUT_PIN = 0;
    HTRPHK_OUT_PIN = 0;
    //------------------------------------------------------
    TRIP_LAMP_PIN = 0;   //����
    //------------------------------------------------------
    guc_100ms_timer = 100;
    guc_1s_timer = 100;
    guc_1min_timer = 60;
    //------------------------------------------------------
    guc_ad_calc_cnt = 0;
    bflg_ad_calc_ok = 0;
    //------------------------------------------------------
    //BAT_DISCHARGE_PIN = 1;
    
    bflg_bat_frist_charge = 1;//�״��ϵ���е�س��
    //------------------------------------------------------
    bflg_adg_reset = 1;  //adģ�鸴λ
    //------------------------------------------------------
    
    bflg_prot2_TC_first_delaytime = 1;
    gss_prot2_TC_first_delaytimer = 0;
    
    bflg_prot2_bat_low_delaytime = 1;
    gss_prot2_bat_low_delaytimer = 0;
    
    bflg_prot2_bat_space_delaytime = 1;
    gss_prot2_bat_space_delaytimer = 0;

    bflg_discharge_delaytime = 1;    //��ʼ�ϵ���ʱ3s�е���Ƿ�ŵ�
    gss_discharge_delaytime = 0;
    //----------------------------------
    //init_freq_ctrl();
    bflg_compressor_first_power_delaytime = 1;
    guc_compressor_first_power_delaytime = 0;
    compressor_init_freq_ctrl();

    bflg_TE_boot_delaytime = 1;
    gss_TE_boot_delaytime = 0;

    bflg_usb_delaytime = 1;
    gss_usb_delaytime = 0;
    //debug
    //bflg_com_type = 1;
    //guc_com_bus_add = 1;
    //bflg_com_type_ok = 1;
}
//------------------------------------------------------------------------------
void system_start(void)  //ϵͳ��������
{
    R_TAU0_Channel0_Start();   //1ms��ʱ����
    
    R_ADC_Start();             //ADת������
    
    R_TAU0_Channel2_Start();
    
    R_TAU0_Channel5_Start();
    
    R_TAU1_Channel2_Start();
    R_TAU1_Channel3_Start();
}
//------------------------------------------------------------------------------
void main_loop(void)    
{   
    //------------------------------------------------------
    timer_op();                    //��ʱ����
    //------------------------------------------------------
    if (bflg_allow_ad_calc == 1)   //�������ad����
    {
        bflg_allow_ad_calc = 0;
        ad_temp_calc();            //ad�¶ȼ������
    }
    //------------------------------------------------------
    if (bflg_com_type_ok == 1)
    {
        bflg_bat_type = bflg_com_type;  //�õ�������� peak��ͨ�����뿪����ѡ�������͵ģ�����ͨ��ͨѶѡ�������ں��棬��������ͨѶѡ��
    }

    //����ģʽ
    if (bflg_test_mode == 1)       //����ǲ���ģʽ--Ҫ��ͨѶ֮ǰ
    {
        test_mode_com_deal();      
        test_mode_output_deal();   
        test_mode_input_deal();    
    }
    else                          
    {
        compressor_RL_out();
        door_heat_RL_out();
        fan_RL_out();
        led_RL_out();    
        bat_RL_out();
    }
    
    //------------------------------------------------------
    //ͨѶ
    if (bflg_com_allow_tx == 1)    
    {       
        bflg_com_allow_tx = 0;
        //------------------------------
        if(bflg_usb_delaytime == 0)
        {
            com_tx_usb_rank(); //usb�����¼����ж�
        }        
        COM_TX_MODE;
        R_UART0_Start();
        com_tx_init();    
    }
    //----------------------------------
    if (bflg_com_tx_ok == 1)  //bflg_com_allow_rx ֮ǰ������ѭ������5ms
    {
        bflg_com_tx_ok = 0;
        guc_IC_erasure_tx_ok_count++;
        if(guc_IC_erasure_tx_ok_count >= 4)    //���͸����ذ�2��֮������0,��ֹ���͹�ȥ����û���յ���
        {
            guc_IC_erasure_tx_ok_count = 0;
            bflg_IC_erasure_order = 0;
        }
        //------------------------------
        R_UART0_Stop();
    }
    //----------------------------------   
    if (bflg_com_allow_rx == 1)      
    {
        bflg_bus_rx_busying = 1;  //����ʱ����ռ�ü�ʱ
        
        bflg_com_allow_rx = 0;
        //------------------------------
        com_rx_init();    
        COM_RX_MODE;
        R_UART0_Start();
    }
    //----------------------------------
    /*if (bflg_com_rx_end == 1)  //���ͨѶʱȥ��
    {
        bflg_com_rx_end = 0;
        //------------------------------
        R_UART0_Stop();
    }*/
    //----------------------------------
    if (bflg_com_rx_ok == 1)   
    {
        bflg_com_rx_ok = 0;
        //------------------------------
        R_UART0_Stop();
        com_rx_data_deal();    
    }
    

    

    
    //----------------------------------
    if (bflg_read_key_delaytime == 0)   //�������ֵʱ������
    {
        bflg_read_key_delaytime = 1;
        gss_read_key_delaytimer = 0;
        
        //read_key_deal();      //����ֵ�������
        key_buzz_cancel();      //����ȡ��30����
        
        mode_selection();
        led_control();
    }
    //----------------------------------
    if (bflg_ad_calc_ok == 1)
    {
        prot2_power_off_deal();    //�ϵ籣���������
        
        port2_power_fault_deal();  //��Դ����ϴ������
        
        prot2_bat_low_deal();      //��ص����ͱ����������
        
        port2_T_fault_deal();      //���������ϴ������
               
        bat_T_hight_protect();     //����¶ȹ����ж�peak
    }

    
    

    //----------------------------------
    //���
    if (bflg_test_mode == 1)  //����ǲ���ģʽ
    {
        RL_CON_PIN = 1;
        gss_bat_pwm_value = 50;  
        TDR01 = gss_bat_pwm_value;
    }
    else if ((bflg_alarm_bat_fed == 1) || (bflg_alarm_power_fault == 1) ||(bflg_alarm_bat_discnnt == 1)||
             (bflg_alarm_bat1_T_hight == 1) || (bflg_alarm_bat2_T_hight == 1))  //||(bflg_alarm_power_off == 1)
    {   
        bflg_bat_charging = 0;                     //��δ�ﵽ������������ͣ��粻��10s��
        gss_bat_pwm_value = 0;
        TDR01 = gss_bat_pwm_value;
    }
    //else if (bflg_comp2_power_on_delaytime == 0)   //�ϵ���ʱ�����ִ��
    //{
    else if (bflg_bat_charge_update == 1)           //ÿ10s����һ��,������
    {
        bflg_bat_charge_update = 0;
        bat_charge_deal();                     //��س�紦�����
        TDR01 = gss_bat_pwm_value;             
    }
    //}
    
    //----------------------------------
    bat_con_check_again();  //peak add
    buzz_sound_priority(); //��ͬ���������ȼ�
    
    //ѪҺ���
    defrosting_control();
    door_heat_control();
    inside_fan_control();
    bat_type_check();
    warning_light_deal();
    remote_alarm_lamp();
    alarm_buzz_deal();
    compressor_up_down_stop();
    
    //----------------------------------
    RTC_continuous_w();
    RTC_continuous_r();
    //----------------------------------
    IC_unlock_deal();
}

//------------------------------------------------------------------------------
void timer_op(void)      //��ʱ��������ѭ�������е���
{
    //------------------------------------------------------
    if (bflg_1ms_reach == 1)  //���1ms��ʱ��
    {
        bflg_1ms_reach = 0;
        guc_100ms_timer--;
        //--------------------------------------------------
        //1ms��ʱ���ó���
        //--------------------------------------------------
        ad_convert_deal();    //adת���������
        //--------------------------------------------------
        com_rx_delaytime();   //ͨѶ������ʱ����
        
        com_tx_delaytime();   //ͨѶ������ʱ����
        
        com_rx_end_delaytime();    //ͨѶ���������ʱ����
        //--------------------------------------------------
        RL_output_delaytime();     //�̵��������ʱ����
        
        com_485_bus_switch();          //485���ߵ�ַ�л�

        RTC_write_read_enable();        

    }
    //------------------------------------------------------
    if (bflg_10ms_reach == 1)
    {
        bflg_10ms_reach = 0;
        guc_1s_timer--;
        //--------------------------------------------------
        //10ms��ʱ���ó���
        trip_lampblink_deal();              //���ϵ���˸����
        
        if(bflg_self_test == 1)             //��ʼ�ϵ�ʱ�Լ�
        {   
            if(bflg_com_type_ok == 1)
            {              
                system_self_test();
            }
        }
        else if(bflg_factory_AT_test==1)    //�ڹ�����������ָ����
        {
            key_factory_AT_test();
        }
        else
        {
            led_disp1_deal();          //�����1��ʾ�������
            
            //led_disp2_deal();          //�����2��ʾ�������
            
            //led_disp3_deal();          //�����3��ʾ�������
            led_lamp_disp();
        }

        
        read_key_delaytime();          //������ʱ����
        
        input_pin_delaytime();         //����������ʱ����
        if(bflg_discharge_delaytime == 0)
        {
            bat_discharge_deal();      //��طŵ紦��
        }
        
	    buzz_loop_tick_delaytime();         //�ε�����ʱ����
    }
    //------------------------------------------------------
    if (guc_100ms_timer == 0)
    {
        guc_100ms_timer = 100;
        //--------------------------------------------------
        //100ms��ʱ���ó���
        //key_locked_delaytime();         //����������ʱ����
                
        adg_data_delaytime();           //adģ�����ݴ�����ʱ����
        
        prot2_bat_space_delaytime();    //���δ���Ӽ������ʱ����
        
        prot2_bat_discnnt_delaytime();  //���δ���ӱ�����ʱ����      //peak  ԭ����ע�͵���---

		//peak add
		led_loop_delaytime();               //led��˸��־
        led_disp1_flicker_delaytime();      //�趨ʱ������˸
        buzz_one_tick_delaytime();          //��һ����ʱ        
        buzz_sound_cnt();
        view_error_code_delaytime();        //���������ʾʱ��
        //--------------------------------------------------
        //ѪҺ���
        key_locked_delaytime();
        key_set_long_delaytime();
        key_set_up_long_delaytime();
        key_up_down_long_delaytime();
        key_set_down_long_delaytime();
        //-----------------------------------------------
        RTC_read_interval();
    }
    //------------------------------------------------------
    if (guc_1s_timer == 0)
    {
        guc_1s_timer = 100;
        guc_1min_timer--;
        //--------------------------------------------------
        //1s��ʱ���ó���
        //--------------------------------------------------
        test_mode_LED_deal();      //����ģʽ��ɫ�ƴ������
        //--------------------------------------------------
        com_fault_delaytime();     //ͨѶ������ʱ����

        //--------------------------------------------------
        port2_door_delaytime();    //�ſ��ر�����ʱ����
        //--------------------------------------------------
        prot2_cond_dirty_delaytime();   //�������ౣ����ʱ����               
        
        //--------------------------------------------------
        bat_charge_delaytime();    //��س����ʱ����
		
        //ѪҺ���
        if(bflg_compressor_first_power_delaytime == 0) //�ϵ���ʱ����
        {
            compressor_start_stop_control();
        }
        compressor_freq_start_delaytime();
        comp2_update_delaytime();

        compressor_freq_ctrl_deal();
        TDR13 = gus_compressor_freq_cnt;


        protect_high_TUTD_delaytime();
        protect_low_TUTD_delaytime();
        protect_high_THW_delaytime();
        protect_air_alarm_boot_delaytime();
        protect_air_higt_delaytime();
        protect_air_low_delaytime();
        protect_higt_TE_delaytime();
        bat_discharge_delaytime();
        led_disp_alternate_delaytime();
        led_disp_black_screen_delaytime();
        remote_alarm_test_delaytime();
        com_usb_delay_time();
        key_RTC_time_parm_sent_usb_delaytime();
        key_password_long_valid_delaytime();
        IC_unlock_delaytime();        
    }
    //------------------------------------------------------
    if (guc_1min_timer == 0)
    {
        guc_1min_timer = 60;
        //--------------------------------------------------
        //1min��ʱ���ó���

        //--------------------------------------------------
        prot2_TC_first_delaytime();     //���������״α�����ʱ����

        prot2_bat_low_delaytime();      //��ص������ӳٳ���

        alarm_buzz_off_delaytime();     //����������ȡ����ʱ����
        //--------------------------------------------------
        bat_charge_continue_delaytime();     //��������ʱ����

        bat_charge_space_delaytime();   //��س������ʱ����
        //ѪҺ���
        coompressor_stop_delaytime();
        coompressor_force_stop_delaytime();
        defrosting_delaytime();
        defrosting_interval_delaytime();
        led_on_delaytime();
        compressor_first_power_delaytime();
        compressor_continue_run_delaytime();

        protect_TE_boot_delaytime();
        protect_TE_interval_delaytime();
    }
}

//------------------------------------------------------------------------------
/*******************************************************************************
��������:  

����λ��: 1ms��ʱ��

���ʽ���: ADCR: ת������Ĵ���
          ADCR  �Ĵ����� A/D ת�������������� 10 λ���� 6 λ�̶�Ϊ 0��;
                ��Ϊ�õ���10λadת����
          
          ADS: ģ������ͨ��ѡ��Ĵ���       
********************************************************************************/
void ad_convert_deal(void)    //adת�����������1ms��ʱ�����е���
{
    gus_ad_val = (uint16_t)(ADCR >> 6U);
    //------------------------------------------------------
    ad_val_deal();  //adֵ�������
    //------------------------------------------------------
    ADS = (uint8_t)(guc_ad_index + 2); //adͨ��ѡ��peak��ΪӲ���ӵ�2ͨ����ʼ��
    //------------------------------------------------------
    R_ADC_Start();  //����adת�� peak ÿ�ζ�Ҫ��ʼ����ÿ�ı�һ��ͨ����Ҫ��ʼ��
}
//------------------------------------------------------------------------------
void timer_int(void)     //1ms��ʱ�жϳ���
{
    bflg_1ms_reach = 1;       //��1ms����־λ
    //----------------------------------
    guc_10ms_timer++;
    if (guc_10ms_timer >= 10) //10ms��ʱ��
    {
        guc_10ms_timer = 0;
        bflg_10ms_reach = 1;
    }      
}
//------------------------------------------------------------------------------
void init_ram_para2(void)
{
    uint8_t i;
    for (i = 0; i < MAX_PARA_NUMBER2; i++)
    {
        //ram_para2[i] = EEPROM_init2[i][0];
        eeprom_write(i + 256, EEPROM_init2[i][0]);
    }
}
//------------------------------------------------------------------------------
void init_ram_para(void)
{
    uint8_t i;
    for (i = 0; i < MAX_PARA_NUMBER; i++)
    {
        //ram_para[i] = EEPROM_init[i][0];
        eeprom_write(i, EEPROM_init[i][0]);
    }
}
//------------------------------------------------------------------------------
void eeprom2_read_deal(void)  //eeprom2��ȡ��������ڳ�ʼ�������е���
{
    int16_t i;
    uint8_t luc_read_para_cnt;
    int16_t lss_verify_word;
    
    do
    {
        bflg_askfor_read_eeprom = 0;    //�������EEP��־
        //----------------------------------------
        for (i = 0; i < MAX_PARA_NUMBER2; i++)
        {
            ram_para2[i] = eeprom_read(i + 256); //��EEP��������RAM��
            R_WDT_Restart();
            
            if ((ram_para2[i] < EEPROM_init2[i][min_row])
               || (ram_para2[i] > EEPROM_init2[i][max_row]))
            {
                bflg_read_eeprom_error = 1;  //�ö�EEPROM���־
                break;
            }
        }
        //--------------------------------------------------
        if (bflg_read_eeprom_error == 1)     //����ж�EEPROM���־
        {
            bflg_read_eeprom_error = 0;      //���EEPROM���־
            
            luc_read_para_cnt++;   //��EEP������1
            if (luc_read_para_cnt >= 3)      //�����3�δ�
            {
                //EEPROM���ϴ���
                bflg_eeprom_fault = 1;  //��EEP���ϱ�־
                break;
            }
            else
            {
                bflg_askfor_read_eeprom = 1; //�������EEPROM��־
            }
        }
    }
    while (bflg_askfor_read_eeprom == 1);
}
//------------------------------------------------------------------------------
void eeprom_read_deal(void)   //eeprom��ȡ��������ڳ�ʼ�������е���
{
    int16_t i;
    uint8_t luc_read_para_cnt;
    int16_t lss_verify_word;
    
    do
    {
        bflg_askfor_read_eeprom = 0;    //�������EEP��־
        //----------------------------------------
        for (i = 0; i < MAX_PARA_NUMBER; i++)
        {
            ram_para[i] = eeprom_read(i);     //��EEP��������RAM��
            R_WDT_Restart();
            
            if ((ram_para[i] < EEPROM_init[i][min_row])
               || (ram_para[i] > EEPROM_init[i][max_row]))
            {
                bflg_read_eeprom_error = 1;  //�ö�EEPROM���־
                break;
            }
        }
        //--------------------------------------------------
        if (bflg_read_eeprom_error == 1)     //����ж�EEPROM���־
        {
            bflg_read_eeprom_error = 0;      //���EEPROM���־
            
            luc_read_para_cnt++;   //��EEP������1
            if (luc_read_para_cnt >= 3)      //�����3�δ�
            {
                //EEPROM���ϴ���
                bflg_eeprom_fault = 1;  //��EEP���ϱ�־
                break;
            }
            else
            {
                bflg_askfor_read_eeprom = 1; //�������EEPROM��־
            }
        }
    }
    while (bflg_askfor_read_eeprom == 1);
}
//------------------------------------------------------------------------------
void delay_10us(uint16_t _10us)    //10us��ʱ������eeprom��д�����е���
{
    uint16_t i,j;
    for (i = 0; i < (_10us * 2); i++)
    {
        j = i * 3;
        R_WDT_Restart();
    }
}
//------------------------------------------------------------------------------
void delay_1ms(uint16_t _1ms)      //1ms��ʱ������eeprom��д�����е���
{
    uint16_t i,j;
    for (i = 0; i < (_1ms * 159); i++)
    {
        j = i * 3;
        R_WDT_Restart();
    }
}
//------------------------------------------------------------------------------
void eeprom_write(uint16_t lus_addr, int16_t lss_eep_data)    //дeeprom����
{
    uint8_t luc_page_addr;
    uint8_t luc_unit_addr;
    uint8_t luc_driver_write;
    
    uint8_t luc_eep_data_H;
    uint8_t luc_eep_data_L;
    
    luc_page_addr = (uint8_t) (lus_addr * 2 / 256);
    luc_unit_addr = (uint8_t) (lus_addr * 2 % 256);
    luc_driver_write = (uint8_t) (( luc_page_addr << 1) & 0x0E | 0xA0);
    
    luc_eep_data_H = (uint8_t) (((uint16_t) lss_eep_data) / 256);
    luc_eep_data_L = (uint8_t) (((uint16_t) lss_eep_data) % 256);
    //------------------------------------------------------
    R_WDT_Restart();
    while (IICBSY0 == 1) {}        //����æ����ȴ�
    
    STT0 = 1;                      //����start�ź�
    WREL0 = 0;                     //����ģʽ
    ACKE0 = 0U;                    //��ACK�ź�
    IICA0 = luc_driver_write;      //д������ַ
    delay_10us(20);                //�ȴ�
    
    IICA0 = luc_unit_addr;         //��Ԫ��ַ
    delay_10us(20);                //�ȴ�
    
    IICA0 = luc_eep_data_H;            
    delay_10us(20);                //�ȴ�
    SPT0 = 1;                      //����stop�ź�
    //------------------------------------------------------
    delay_1ms(3);
    //------------------------------------------------------
    R_WDT_Restart();
    while (IICBSY0 == 1){}         //����æ����ȴ�
    
    STT0 = 1;                      //����start�ź�
    WREL0 = 0;                     //����ģʽ
    ACKE0 = 0U;                    //��ACK�ź�
    IICA0 = luc_driver_write;      //д������ַ
    delay_10us(20);                //�ȴ�
    
    IICA0 = (uint8_t)(luc_unit_addr + 1);     //��Ԫ��ַ
    delay_10us(20);                //�ȴ�
    
    IICA0 = luc_eep_data_L;
    delay_10us(20);                //�ȴ�
    SPT0 = 1;                      //����stop�ź�
    //------------------------------------------------------
    delay_1ms(3);
}
//------------------------------------------------------------------------------
int16_t eeprom_read(uint16_t lus_addr)  //��eeprom����
{
    uint8_t luc_page_addr;
    uint8_t luc_unit_addr;
    uint8_t luc_driver_write;
    uint8_t luc_driver_read;
    uint8_t luc_eep_data_H;
    uint8_t luc_eep_data_L;
    
    luc_page_addr = (uint8_t) (lus_addr * 2 / 256);
    luc_unit_addr = (uint8_t) (lus_addr * 2 % 256);
    luc_driver_write = (uint8_t) (( luc_page_addr << 1) & 0x0E | 0xA0);
    luc_driver_read = (uint8_t) (( luc_page_addr << 1) & 0x0E | 0xA1);
    //------------------------------------------------------
    R_WDT_Restart();
    while (IICBSY0 == 1) {}        //����æ����ȴ�
    
    STT0 = 1;                      //����start�ź�
    WREL0 = 0;                     //����ģʽ
    ACKE0 = 0U;                    //��ACK�ź�
    IICA0 = luc_driver_write;      //д������ַ
    delay_10us(20);                //�ȴ�
    
    IICA0 = luc_unit_addr;         //��Ԫ��ַ
    delay_10us(20);                //�ȴ�
    
    STT0 = 1;                      //����start�ź�
    IICA0 = luc_driver_read;       //��������ַ    
    delay_10us(20);                //�ȴ�
    WREL0 = 1;                     //����ģʽ
    
    delay_10us(20);                //�ȴ�
    luc_eep_data_H = IICA0;        //�õ�����
    ACKE0 = 1;                     //����ACK�ź�
    SPT0 = 1;                      //����stop�ź�
    //------------------------------------------------------
    delay_1ms(3);
    //------------------------------------------------------
    R_WDT_Restart();
    while (IICBSY0 == 1){}         //����æ����ȴ�
    
    STT0 = 1;                      //����start�ź�
    WREL0 = 0;                     //����ģʽ
    ACKE0 = 0U;                    //��ACK�ź�
    IICA0 = luc_driver_write;      //д������ַ
    delay_10us(20);                //�ȴ�
    
    IICA0 = (uint8_t)(luc_unit_addr + 1);     //��Ԫ��ַ
    delay_10us(20);                //�ȴ�
    
    STT0 = 1;                      //����start�ź�
    IICA0 = luc_driver_read;       //��������ַ
    delay_10us(20);                //�ȴ�
    WREL0 = 1;                     //����ģʽ
    
    delay_10us(20);                //�ȴ�
    luc_eep_data_L = IICA0;        //������
    ACKE0 = 1;                     //����ACK�ź�
    SPT0 = 1;                      //����stop�ź�
    //------------------------------------------------------
    delay_1ms(3);
    //------------------------------------------------------
    return ((int16_t) (luc_eep_data_H) * 256 + luc_eep_data_L);
}






//------------------------------------------------------------------------------
void RL_output_delaytime(void)     //�̵��������ʱ������1ms��ʱ�����е���
{
    if (bflg_RL_output_delaytime == 1)
    {
        guc_RL_output_delaytimer++;
        if (guc_RL_output_delaytimer >= 200)
        {
            guc_RL_output_delaytimer = 0;
            bflg_RL_output_delaytime = 0;
        }
    }
}
//------------------------------------------------------------------------------
void trip_lampblink_deal(void)     //���ϵ���˸������10ms��ʱ�����е���
{
/*
    if (guc_trip_code != 0)       //���������ͣ����ת
    {
        if (guc_trip_flash_cnt > 0)     //�����˸�����Ѿ������㣬��ʼ3������ʱ
        {
            guc_trip_flash_timer++;
            if (guc_trip_flash_timer >= 30)  //ÿ0.3����˸һ��
            {
                guc_trip_flash_timer = 0;
                TRIP_LAMP_PIN = ~TRIP_LAMP_PIN;
                guc_trip_flash_cnt--;
            }
        }
        else
        {
            TRIP_LAMP_PIN = 0;
            guc_trip_flash_timer++;
            if (guc_trip_flash_timer >= 200)   //2������ʱ
            {
                guc_trip_flash_timer = 0;
                guc_trip_flash_cnt = guc_trip_code;
                guc_trip_flash_cnt <<= 1;
            }
        }
    }
    else
    {
        TRIP_LAMP_PIN = 0;
        guc_trip_flash_timer = 0;
        guc_trip_flash_cnt = 0;
    }
    */
}
//------------------------------------------------------------------------------
/*******************************************************************************
function:   �������ŵĵ�ƽ��⣻---������ز巴��

            
*******************************************************************************/
/*void input_pin_delaytime(void)     //����������ʱ������10ms��ʱ�����е���
{
	// ��ط��Ӵ���
	
    if (BAT_FED_IN_PIN == 1)  //peak ���δ����
    {
        guc_bat_fed_pin_high_delaytimer = 0;
        
        if (bflg_alarm_bat_fed == 1)       // peak ���ӱ�����־
        {
            guc_bat_fed_pin_low_delaytimer++;
            if (guc_bat_fed_pin_low_delaytimer >= 100) //1s
            {
                guc_bat_fed_pin_low_delaytimer = 0;
                
                bflg_alarm_bat_fed = 0;     //peak �巴�ӱ�����־
                gus_trip_code1 &= ~0x0800;
            }  
        }
    }
    else     //peak ��ط���  
    {
        guc_bat_fed_pin_low_delaytimer = 0;
        
        if (bflg_alarm_bat_fed == 0)
        {
            guc_bat_fed_pin_high_delaytimer++;
            if (guc_bat_fed_pin_high_delaytimer >= 100) //1s
            {
                guc_bat_fed_pin_high_delaytimer = 0;
                
                bflg_alarm_bat_fed = 1;
                gus_trip_code1 |= 0x0800;
            }
        }
    }
}*/
/******************************************************************************************************************************
��������:     �������ŵĵ�ƽ��⣺1������ 2����ز巴

����λ�ã�  10ms---------------------------------------ok          
******************************************************************************************************************************/
void input_pin_delaytime(void)      
{
    if (DOOR1_IN_PIN == 0)                   // 0����
    {
        guc_door1_pin_high_delaytimer = 0;
        
        if (bflg_prot2_door1_pin == 1)
        {
            guc_door1_pin_low_delaytimer++;
            if (guc_door1_pin_low_delaytimer >= 100)   //1s
            {
                guc_door1_pin_low_delaytimer = 0;
                
                bflg_prot2_door1_pin = 0;
            }
        }
    }
    else                                      // 1����      
    { 
        guc_door1_pin_low_delaytimer = 0;
        
        if (bflg_prot2_door1_pin == 0)
        {
            guc_door1_pin_high_delaytimer++;
            if (guc_door1_pin_high_delaytimer >= 100)  //1s
            {
                guc_door1_pin_high_delaytimer = 0;
                
                bflg_prot2_door1_pin = 1;
            }
        }
    }
    
    //------------------------------------------------------
	// ��ط��Ӵ���
    if (BAT_FED_IN_PIN == 1)  //���δ����
    {
        guc_bat_fed_pin_high_delaytimer = 0;
        
        if (bflg_alarm_bat_fed == 1)       // ���ӱ�����־
        {
            guc_bat_fed_pin_low_delaytimer++;
            if (guc_bat_fed_pin_low_delaytimer >= 100) //1s
            {
                guc_bat_fed_pin_low_delaytimer = 0;
                
                bflg_alarm_bat_fed = 0;    
                gus_trip_code1 &= ~BAT_FED_CODE;
            }  
        }
    }
    else     // ��ط���  
    {
        guc_bat_fed_pin_low_delaytimer = 0;
        
        if (bflg_alarm_bat_fed == 0)
        {
            guc_bat_fed_pin_high_delaytimer++;
            if (guc_bat_fed_pin_high_delaytimer >= 100) //1s
            {
                guc_bat_fed_pin_high_delaytimer = 0;
                
                bflg_alarm_bat_fed = 1;
                gus_trip_code1 |= BAT_FED_CODE;
            }
        }
    }
}

//------------------------------------------------------------------------------
#endif
