#ifndef _COM_C_
#define _COM_C_
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
//��������
void com_init(void);//ͨѶ��ʼ�������ڳ�ʼ�������е���

void com_tx_init(void);  //ͨѶ���ͳ�ʼ����������ѭ�������е���

void com_rx_init(void);  //ͨѶ���ճ�ʼ����������ѭ�������е���

void com_rx_data_deal(void);  //ͨѶ�������ݴ����������ѭ�������е���

void com_tx_delaytime(void);  //ͨѶ������ʱ������1ms��ʱ�����е���

void com_rx_delaytime(void);  //ͨѶ������ʱ������1ms��ʱ�����е���

void com_rx_end_delaytime(void);   //ͨѶ���������ʱ������1ms��ʱ�����е���

void com_fault_delaytime(void);    //ͨѶ������ʱ������1s��ʱ�����е���

void com_tx_int(void);   //ͨѶ�����жϳ���

void com_rx_int(uint8_t rx_data);  //ͨѶ�����жϳ���

void com_485_bus_switch(void);

unsigned int CRC16(unsigned char *puchmsg, unsigned int usDataLen);    //����CRCУ��ĳ���

void com_tx_usb_rank(void);

void com_usb_delay_time(void);  

//------------------------------------------------------------------------------
//��־����
flag_type flg_com;
flag_type flg_read_write,flg_sent_usb_control,flg_sent_usb_event,flg_sent_usb_event_1,
          flg_usb_event_temp,flg_usb_event_temp_1,flg_usb_event_attribute;
//------------------------------------------------------------------------------
//��������
uint8_t   guc_com_tx_buffer[120];        //ͨѶ���ͻ�����
uint8_t   guc_com_rx_buffer[80];         //ͨѶ���ջ�����

int16_t   gss_com_tx_delaytimer;        //ͨѶ������ʱ��ʱ��
int16_t   gss_com_rx_delaytimer;        //ͨѶ������ʱ��ʱ��

int16_t   gss_com_rx_end_delaytimer;    //ͨѶ�������־
int16_t   gss_com_fault_delaytimer;     //ͨѶ������ʱ��ʱ��

uint8_t   guc_com_tx_point;   //ͨѶ��������
uint8_t   guc_com_rx_point;   //ͨѶ��������

uint8_t   guc_com_type_cnt;   //ͨѶ���ͼ�����

//peak add
uint8_t  guc_IC_access_state = 0;    // ���յ�IC����ָ��

uint16_t guc_flash_add;              // ���յ�IC��ע�����õĵ�ַ��������ʾ�ж����ſ�ע��
uint16_t guc_temp_flash_add;         // ���յ�IC��ע����ʱ����ĵ�ַ�����������ڵĵ�ַ�Ա��Ƿ����µ�IC��ע�ᣬ����ʱ���������
uint8_t  guc_IC_erasure_tx_ok_count; // ע��IC��ָ���bflg_com_tx_ok�����÷��Ͷ�μ���,��ֹ����

uint8_t   guc_com_bus_add;            //�ܵĴӻ���ַ
uint16_t  guc_dis_bus_delaytime;      //��ʾ��ռ��������ʱ
uint16_t  guc_expand_bus_delaytime;   //��չ��ռ��������ʱ

int16_t gss_liquid_led_button;              //Һ�����Ŀ��ư�ť  ��1�� 0��
int16_t gss_liquid_ozone_disinfect_button;  //Һ����������ɱ����ť��1�� 0��

uint8_t guc_led_comm_cnt;                   //��led��ͨѶx�β�ת������չ�壬��ΪledҪ��ʵʱ
uint8_t gss_usb_delaytime;                  //�����ϵ���ʱxs���ڷ���usb�¼�
uint8_t guc_U_disk_state;                   //��ʾ�巢�͹�����U��״̬
uint8_t guc_usb_state;                      //��ʾ�巢�͹�����usb���״̬


#define   LED_DIS_ADD         0     //LED�ӻ���ַ
#define   LIQUID_DIS_ADD      1     //Һ���ӻ���ַ
#define   EXPAND_ADD          2     //��չ��ӻ���ַ

#define   WRITE_STATE_CNT     50

#define   READ_USER_CNT       30
#define   READ_ADMIN_CNT      30




//------------------------------------------------------------------------------
//��������
//------------------------------------------------------------------------------
/*******************************************************************************
function:  ��ʼ��ʱ(��һ��)��Ϊ�˸�����ʱ�򿪷����жϣ��������ݣ�
    

*******************************************************************************/
void com_init(void) //ͨѶ��ʼ�������ڳ�ʼ�������е���
{
    bflg_com_tx_delaytime = 1;     //peak ��ʱʱ�䵽���򿪷����жϣ��������ݣ�
    gss_com_tx_delaytimer = 3000;
    
    bflg_com_rx_delaytime = 0;
    gss_com_rx_delaytimer = 0;
    
    bflg_read_write_data = 1; //�״�д����
}
//------------------------------------------------------------------------------
/*******************************************************************************
function: ��ÿһ�η�������ʱ�������ʼ����
         LED: ������ʾ��������Ҫ��ʾ������(txt)

         ��ʼ������:
           1�����͵Ĳ�ѯָ��(�̶�)
           2�����͵��趨����---->|a�� "����״̬"��"����"��"����ָʾ"��"��ѹ"��"����"
                                 |b��"�¶��趨ֵ" "���±����趨ֵ" "���±����趨ֵ"

           a---�������ذ屾������ݲɼ����жϣ�
           
           b---����ʾ�尴�����úõ�ֵ�� ��������
               ��ʾ�尴�����ú�--->���͸�����-->�����ٽ���ֵ���͸���ʾ����ʾ��  
               
********************************************************************************/
void com_tx_init(void)   //ͨѶ���ͳ�ʼ����������ѭ�������е���
{
    int8_t i;
    word_type gss_tmp;
    word_type com_crc;
    //----------------------------------------------------------LED��-----------------------------------
    if (bflg_com_type == 0)   //�����LED��ʾ��ͨѶ
    {
        if(guc_com_bus_add == LED_DIS_ADD)
        {
            if (bflg_read_write_data == 0)  //����Ƕ�����  //peak ������ֵ
            {
                guc_com_tx_buffer[0] = 0x00;   //00  //�豸��ַ
                guc_com_tx_buffer[1] = 0x03;     //������
                
                guc_com_tx_buffer[2] = 0x00;
                guc_com_tx_buffer[3] = 0x07;     //��ʼ��ַ7
                
                guc_com_tx_buffer[4] = 0x00;
                guc_com_tx_buffer[5] = 2;      //2  //��ַ����
                //----------------------------------------------
                com_crc.uword = CRC16(guc_com_tx_buffer, 6);
                guc_com_tx_buffer[6] = com_crc.ubyte.low;
                guc_com_tx_buffer[7] = com_crc.ubyte.high;
            }
            else   //�����д����   //peak �趨����ܡ�LED��ʾ����
            {
                guc_com_tx_buffer[0] = 0x00;     //�豸��ַ
                guc_com_tx_buffer[1] = 0x10;     //д����
                
                guc_com_tx_buffer[2] = 0x00;
                guc_com_tx_buffer[3] = 0x00;     //��ʼ��ַ0
                
                guc_com_tx_buffer[4] = 0x00;
                guc_com_tx_buffer[5] = 22;    //7    //��ַ����  peak Ҫд�����Ĵ�����ÿ���Ĵ���ռ2���ֽڣ�7*2 = 14 ���ֽڣ�
                
                guc_com_tx_buffer[6] = 44;    //14   //�ֽ���    peak [7]--[20] = 14 ���������
        
                for (i = 0; i < 12; i++)         // peak [7]~[18] = 12 ;
                {
                    guc_com_tx_buffer[i + 7] = guc_led_buffer[i];    //peak ����ܼĴ���
                }           
                
                //----------------------------------------------
                if(bflg_IC_erasure_order == 1)  //����ע��ָ��,ע��IC��ָ��_��bflg_com_tx_ok�����÷��Ͷ�μ���,��ֹ����
                {               
                    guc_com_tx_buffer[19] = 1;
                }
                else
                {
                    guc_com_tx_buffer[19] = 0;
                }
                
                if (bflg_alarm_lamp == 0)         //peak ��ʾ���ϵ���λ������ 
                {
                    //guc_com_tx_buffer[19] = 0;
                    guc_com_tx_buffer[20] = 0x00;
                }
                else
                {
                    //guc_com_tx_buffer[19] = 0;
                    guc_com_tx_buffer[20] = 0x07;     //peak ��ʾ������� "����" �������� 
                }
                
                //--------------------------------------------------------------------------------------------
                //usbʹ�õ�����֡
                gss_tmp.sword = gss_user_set_parm_value[DISP_USER_SET_TS]*10;     //�����¶�
                guc_com_tx_buffer[21] = gss_tmp.ubyte.low;
                guc_com_tx_buffer[22] = gss_tmp.ubyte.high;
                gss_tmp.sword = gss_TU*10;                                        //����ʾ�¶�
                guc_com_tx_buffer[23] = gss_tmp.ubyte.low;
                guc_com_tx_buffer[24] = gss_tmp.ubyte.high;
                gss_tmp.sword = gss_TD*10;                                        //����ʾ�¶�
                guc_com_tx_buffer[25] = gss_tmp.ubyte.low;
                guc_com_tx_buffer[26] = gss_tmp.ubyte.high;
                gss_tmp.sword = gss_TA*10;                                        //�����¶�
                guc_com_tx_buffer[27] = gss_tmp.ubyte.low;
                guc_com_tx_buffer[28] = gss_tmp.ubyte.high;
                gss_tmp.sword = gss_TE*10;                                        //��˪(�Ƚ���)�¶�
                guc_com_tx_buffer[29] = gss_tmp.ubyte.low;
                guc_com_tx_buffer[30] = gss_tmp.ubyte.high;    
                //                                                                //Ԥ��
                guc_com_tx_buffer[31] = 0;
                guc_com_tx_buffer[32] = 0;

                //--------------------------------------------------------------------------------------------
                //�¼�֡
                if(bflg_sent_usb_ALH == 1)
                {
                    bflg_sent_usb_ALH = 0;
                    guc_com_tx_buffer[20] |= 0x80;  //[20]�ĵ�7λ�����Ƿ����¼�֡�ı�־

                    guc_com_tx_buffer[33] = 0x00;                      //�¼������  
                    gss_tmp.sword = gss_TU*10;
                    guc_com_tx_buffer[34] = gss_tmp.ubyte.low;         //�¼�ֵ1 (��ʪ�ȡ���ѹֵ��*100,��ΪTA�ѷŴ�10,С��)
                    guc_com_tx_buffer[35] = gss_tmp.ubyte.high;   
                    gss_tmp.sword = gss_TD*10;
                    guc_com_tx_buffer[36] = gss_tmp.ubyte.low;         //�¼�ֵ2 ��*100
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
                    if(bflg_usb_DOR_open == 1)    //����
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
                    gss_tmp.sword = (guc_flash_add>>2);   //guc_flash_add>>2 = ����4������ַת��Ϊ����
                    //guc_com_tx_buffer[34] = (uint8_t)(guc_flash_add>>2);       //guc_flash_add>>2 = ����4������ַת��Ϊ����
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
                //ʱ��֡
                if(bflg_usb_set_time == 1)           //���޸�����ʱ��/�����Ҳ��һֱ����ʱ��
                {
                    bflg_usb_set_time = 0;
                    guc_com_tx_buffer[20] |= 0x20;   //[20]�ĵ�5λ�����Ƿ����޸�����ʱ��ı�־
                }
                guc_com_tx_buffer[39] = guc_RTC_r_year;    //��
                guc_com_tx_buffer[40] = guc_RTC_r_month;   //��
                guc_com_tx_buffer[41] = guc_RTC_r_day;     //��
                guc_com_tx_buffer[42] = guc_RTC_r_hour;    //ʱ
                guc_com_tx_buffer[43] = guc_RTC_r_minute;  //��
                guc_com_tx_buffer[44] = guc_RTC_r_second;  //��
                //--------------------------------------------------------------------------------------------
                //����֡    
                if(bflg_sent_usb_CL2 == 1)          //���usb����
                {
                    bflg_sent_usb_CL2 = 0;
                    guc_com_tx_buffer[20] |= 0x40;   //[20]�ĵ�6λ�����Ƿ��п���֡�ı�־
                    
                    guc_com_tx_buffer[45] = 0x01;    //���ƴ���
                    guc_com_tx_buffer[46] = 0x32;    //ֵ1
                    guc_com_tx_buffer[47] = 0x33;    //ֵ2
                    guc_com_tx_buffer[48] = 0x34;    //ֵ3
                }
                else if(bflg_sent_usb_T1 == 1)       //usb��¼Ƶ��
                {
                    bflg_sent_usb_T1 = 0;
                    guc_com_tx_buffer[20] |= 0x40; 
                    
                    guc_com_tx_buffer[45] = 0x02;   
                    guc_com_tx_buffer[46] = (uint8_t)gss_user_set_parm_value[DISP_USER_SET_T1];  
                    guc_com_tx_buffer[47] = 0x00;    
                    guc_com_tx_buffer[48] = 0x00;    
                }
                else if(bflg_sent_usb_P6 == 1)       //usb ����ģʽ
                {
                    bflg_sent_usb_P6 = 0;
                    guc_com_tx_buffer[20] |= 0x40;
                    
                    guc_com_tx_buffer[45] = 0x03;   
                    guc_com_tx_buffer[46] = (uint8_t)gss_user_set_parm_value[DISP_USER_SET_P6];
                    guc_com_tx_buffer[47] = 0x00;    
                    guc_com_tx_buffer[48] = 0x00;    
                }
                else if(bflg_sent_usb_UL == 1)     //usb��������
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
                //����֡
                guc_com_tx_buffer[49] = 0x02;    //����ʶ���� ѪҺΪ 0x02
                guc_com_tx_buffer[50] = 0x00;    //Ԥ��
                
                //--------------------------------------------------------------------------------------
                com_crc.uword = CRC16(guc_com_tx_buffer, 51);  //21
                guc_com_tx_buffer[51] = com_crc.ubyte.low;     //21
                guc_com_tx_buffer[52] = com_crc.ubyte.high;    //22
            }
        }
        else if(guc_com_bus_add == EXPAND_ADD)
        {
            if (bflg_read_write_data == 0)       //����Ƕ�����  
            {
                guc_com_tx_buffer[0] = 0x02;     //�豸��ַ
                guc_com_tx_buffer[1] = 0x03;     //������
                
                guc_com_tx_buffer[2] = 0x00;
                guc_com_tx_buffer[3] = 0x07;     //��ʼ��ַ7
                
                guc_com_tx_buffer[4] = 0x00;
                guc_com_tx_buffer[5] = 3;        //��ַ����
                //----------------------------------------------
                com_crc.uword = CRC16(guc_com_tx_buffer, 6);
                guc_com_tx_buffer[6] = com_crc.ubyte.low;
                guc_com_tx_buffer[7] = com_crc.ubyte.high;
            }
            else    
            {
                guc_com_tx_buffer[0] = 0x02;     //�豸��ַ
                guc_com_tx_buffer[1] = 0x10;     //д����
                
                guc_com_tx_buffer[2] = 0x00;
                guc_com_tx_buffer[3] = 0x00;     //��ʼ��ַ0
                
                guc_com_tx_buffer[4] = 0x00;
                guc_com_tx_buffer[5] = 7;        //��ַ����  peak Ҫд�����Ĵ�����ÿ���Ĵ���ռ2���ֽڣ�7*2 = 14 ���ֽڣ�
                
                guc_com_tx_buffer[6] = 14;       //���͵������ֽ���

                //upgrade--> gss_tmp.ubyte.low
                guc_com_tx_buffer[7] |= (uint8_t)(gss_factory_parm_value[DISP_FAC_Ct6]>>8);            //����У׼H-�Ŵ�10
                guc_com_tx_buffer[8] |= (uint8_t)gss_factory_parm_value[DISP_FAC_Ct6];                 //L
                guc_com_tx_buffer[9] |= (uint8_t)(gss_factory_parm_value[DISP_FAC_Ct7]>>8)��            //ʪ��У׼
                guc_com_tx_buffer[10]|= (uint8_t)gss_factory_parm_value[DISP_FAC_Ct7];                 //L
                guc_com_tx_buffer[11]|= (uint8_t)(gss_user_manage_parm_value[DISP_USER_MANAGE_S2]>>8); //���ɱ��ʱ��
                guc_com_tx_buffer[12]|= (uint8_t)gss_user_manage_parm_value[DISP_USER_MANAGE_S2];      //L
                guc_com_tx_buffer[13] = (uint8_t)gss_user_manage_parm_value[DISP_USER_MANAGE_S1];      //ɱ��ģʽ
                if(bflg_expand_unlock == 1)     //����
                {
                    guc_com_tx_buffer[14] = 0x01;     
                }
                else
                {
                    guc_com_tx_buffer[14] = 0x00;    
                }
                if(DOOR1_IN_PIN == 1)   //����
                {
                    guc_com_tx_buffer[15] = 0x01; 
                }
                else
                {
                    guc_com_tx_buffer[15] = 0x00; 
                }
                guc_com_tx_buffer[16] = 0x00; //Ԥ��
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

//---------------------------------------------------------------------Һ����------------------
    else  //�����Һ����ͨѶ
    {
        if(guc_com_bus_add == LIQUID_DIS_ADD)
        {
            if (bflg_read_write_data == 0)        //���û���Ϣ
            {
                if (bflg_read_user_admin == 0)   //����Ƕ��û���Ϣ
                {
                    guc_com_tx_buffer[0] = 0x01;      //�豸��ַ
                    guc_com_tx_buffer[1] = 0x03;      //������
                    
                    guc_com_tx_buffer[2] = 0xA4;
                    guc_com_tx_buffer[3] = 0x1F;      //��ʼ��ַ8100
                    
                    guc_com_tx_buffer[4] = READ_USER_CNT;
                    guc_com_tx_buffer[5] = 0X00;       //��ַ����
                    //------------------------------------------
                    com_crc.uword = CRC16(guc_com_tx_buffer, 6);
                    guc_com_tx_buffer[6] = com_crc.ubyte.low;
                    guc_com_tx_buffer[7] = com_crc.ubyte.high;
                }
                else    //������Ա����
                {
                    guc_com_tx_buffer[0] = 0x01;      //�豸��ַ
                    guc_com_tx_buffer[1] = 0x03;      //������
                    
                    guc_com_tx_buffer[2] = 0xD6;
                    guc_com_tx_buffer[3] = 0x1F;      //��ʼ��ַ8150
                    
                    guc_com_tx_buffer[4] = READ_ADMIN_CNT;
                    guc_com_tx_buffer[5] = 0X00;       //��ַ����
                    //------------------------------------------
                    com_crc.uword = CRC16(guc_com_tx_buffer, 6);
                    guc_com_tx_buffer[6] = com_crc.ubyte.low;
                    guc_com_tx_buffer[7] = com_crc.ubyte.high;
                }
            }
            else   //�����д����
            {
                guc_com_tx_buffer[0] = 0x01;     //�豸��ַ
                guc_com_tx_buffer[1] = 0x10;     //д����
                
                guc_com_tx_buffer[2] = 0x40;
                guc_com_tx_buffer[3] = 0x1F;     //��ʼ��ַ10000
                
                guc_com_tx_buffer[4] = WRITE_STATE_CNT;
                guc_com_tx_buffer[5] = 0x00;    //��ַ����
                
                guc_com_tx_buffer[6] = (WRITE_STATE_CNT * 2);   //�ֽ���
                //----------------------------------------------
                {
                    //
                    //debug
                    //gss_THW = -1997;
                    
                    gss_tmp.sword = gss_THW;     //�����¶�
                    guc_com_tx_buffer[7] = gss_tmp.ubyte.low;
                    guc_com_tx_buffer[8] = gss_tmp.ubyte.high;
                    //------------------------------------------
                    gss_tmp.sword = guc_humidity; //����ʪ��
                    guc_com_tx_buffer[9] = gss_tmp.ubyte.low;
                    guc_com_tx_buffer[10] = gss_tmp.ubyte.high;
                    //------------------------------------------
                    gss_tmp.sword = gss_TA;      //PT100
                    guc_com_tx_buffer[11] = gss_tmp.ubyte.low;
                    guc_com_tx_buffer[12] = gss_tmp.ubyte.high;
                    //------------------------------------------
                    gss_tmp.sword = gss_TE;     //�Ƚ�����(��˪������)�¶�
                    guc_com_tx_buffer[13] = gss_tmp.ubyte.low;
                    guc_com_tx_buffer[14] = gss_tmp.ubyte.high;
                    //------------------------------------------
                    gss_tmp.sword = gss_TC;    //�������¶�
                    guc_com_tx_buffer[15] = gss_tmp.ubyte.low;
                    guc_com_tx_buffer[16] = gss_tmp.ubyte.high;
                    //------------------------------------------
                    gss_tmp.sword = gss_TU;   //�ϴ������¶�
                    guc_com_tx_buffer[17] = gss_tmp.ubyte.low;
                    guc_com_tx_buffer[18] = gss_tmp.ubyte.high;
                    //------------------------------------------
                    gss_tmp.sword = gss_TD;  //�´������¶�
                    guc_com_tx_buffer[19] = gss_tmp.ubyte.low;
                    guc_com_tx_buffer[20] = gss_tmp.ubyte.high;
                    //------------------------------------------
                    gss_tmp.sword = gss_VAC; //�е��Դ��ѹ
                    guc_com_tx_buffer[21] = gss_tmp.ubyte.low;
                    guc_com_tx_buffer[22] = gss_tmp.ubyte.high;
                    //------------------------------------------
                    gss_tmp.sword = guc_bat_Vout; //��ص���
                    guc_com_tx_buffer[23] = gss_tmp.ubyte.low;
                    guc_com_tx_buffer[24] = gss_tmp.ubyte.high;
            
                    //------------------------------------------
                    if (bflg_compressor_runing == 0)    //ѹ����״̬
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
                    if (bflg_inside_fan_runing == 0)    //�ڷ��״̬
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
                    if (bflg_compressor_runing == 0)       //����״̬(�����������״̬ͬѹ��)
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
                    if (bflg_heat_runing == 0)       //�������˿״̬
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
                    if (bflg_led_on == 0)           //������״̬
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
                    if (bflg_sterilization_state == 0)     //����ɱ��״̬
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
                    if (bflg_beam_heat_runing == 0)   //��������˿״̬
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
                    if (bflg_water_box_heat_runing == 0)   //��ˮ�м���˿״̬
                    {
                        guc_com_tx_buffer[39] = 1;         //�Ҫ��һֱ��
                        guc_com_tx_buffer[40] = 0;
                    }
                    else
                    {
                        guc_com_tx_buffer[39] = 1;
                        guc_com_tx_buffer[40] = 0;
                    }
                    //------------------------------------------
                    if (bflg_locd_runing == 0)      //�����״̬-ʵ�ʻ�δ����
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
                    if (bflg_prot2_door1_pin == 0)    //�ſ���״̬
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
                    gss_tmp.sword = gus_trip_code1;   //����״̬��Ϣ1
                    guc_com_tx_buffer[45] = gss_tmp.ubyte.low;
                    guc_com_tx_buffer[46] = gss_tmp.ubyte.high;
                    //------------------------------------------
                    gss_tmp.sword = gus_trip_code2;   //����״̬��Ϣ2
                    guc_com_tx_buffer[47] = gss_tmp.ubyte.low;
                    guc_com_tx_buffer[48] = gss_tmp.ubyte.high;
            
                    //Ԥ��
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
                    //Ԥ��
                    guc_com_tx_buffer[81] = 0;
                    guc_com_tx_buffer[82] = 0;
                    //------------------------------------------
                    //Ԥ��
                    guc_com_tx_buffer[83] = 0;
                    guc_com_tx_buffer[84] = 0;
                    //------------------------------------------
                    //Ԥ��
                    guc_com_tx_buffer[85] = 0;
                    guc_com_tx_buffer[86] = 0;
                    //------------------------------------------
                    //Ԥ��
                    guc_com_tx_buffer[87] = 0;
                    guc_com_tx_buffer[88] = 0;
                    //------------------------------------------
                    //Ԥ��
                    guc_com_tx_buffer[89] = 0;
                    guc_com_tx_buffer[90] = 0;
                    //------------------------------------------
                    //Ԥ��
                    guc_com_tx_buffer[91] = 0;
                    guc_com_tx_buffer[92] = 0;
                    //------------------------------------------
                    //Ԥ��
                    guc_com_tx_buffer[93] = 0;
                    guc_com_tx_buffer[94] = 0;
                    //------------------------------------------
                    //Ԥ��
                    guc_com_tx_buffer[95] = 0;
                    guc_com_tx_buffer[96] = 0;
                    //------------------------------------------
                    //Ԥ��
                    guc_com_tx_buffer[97] = 0;
                    guc_com_tx_buffer[98] = 0;
                    //------------------------------------------
                    //Ԥ��
                    guc_com_tx_buffer[99] = 0;
                    guc_com_tx_buffer[100] = 0;
                    //------------------------------------------
                    //Ԥ��
                    guc_com_tx_buffer[101] = 0;
                    guc_com_tx_buffer[102] = 0;
                    //------------------------------------------
                    //Ԥ��
                    guc_com_tx_buffer[103] = 0;
                    guc_com_tx_buffer[104] = 0;
                    //------------------------------------------
                    //Ԥ��
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
            if (bflg_read_write_data == 0)       //����Ƕ�����  
            {
                guc_com_tx_buffer[0] = 0x02;     //�豸��ַ
                guc_com_tx_buffer[1] = 0x03;     //������
                
                guc_com_tx_buffer[2] = 0x00;
                guc_com_tx_buffer[3] = 0x07;     //��ʼ��ַ7
                
                guc_com_tx_buffer[4] = 0x00;
                guc_com_tx_buffer[5] = 3;        //��ַ����
                //----------------------------------------------
                com_crc.uword = CRC16(guc_com_tx_buffer, 6);
                guc_com_tx_buffer[6] = com_crc.ubyte.low;
                guc_com_tx_buffer[7] = com_crc.ubyte.high;
            }
            else    
            {
                guc_com_tx_buffer[0] = 0x02;     //�豸��ַ
                guc_com_tx_buffer[1] = 0x10;     //д����
                
                guc_com_tx_buffer[2] = 0x00;
                guc_com_tx_buffer[3] = 0x00;     //��ʼ��ַ0
                
                guc_com_tx_buffer[4] = 0x00;
                guc_com_tx_buffer[5] = 7;        //��ַ����  peak Ҫд�����Ĵ�����ÿ���Ĵ���ռ2���ֽڣ�7*2 = 14 ���ֽڣ�
                
                guc_com_tx_buffer[6] = 14;       //���͵������ֽ���
                
                guc_com_tx_buffer[7] |= (uint8_t)(gss_factory_parm_value[DISP_FAC_Ct6]>>8);  //����У׼H-�Ŵ�10
                guc_com_tx_buffer[8] |= (uint8_t)gss_factory_parm_value[DISP_FAC_Ct6];       //L
                guc_com_tx_buffer[9] |= (uint8_t)(gss_factory_parm_value[DISP_FAC_Ct7]>>8)��  //ʪ��У׼
                guc_com_tx_buffer[10]|= (uint8_t)gss_factory_parm_value[DISP_FAC_Ct7];       //L
                guc_com_tx_buffer[11]|= (uint8_t)(gss_user_manage_parm_value[DISP_USER_MANAGE_S2]>>8); //���ɱ��ʱ��--��Һ������
                guc_com_tx_buffer[12]|= (uint8_t)gss_user_manage_parm_value[DISP_USER_MANAGE_S2];      //L
                guc_com_tx_buffer[13] = (uint8_t)gss_user_manage_parm_value[DISP_USER_MANAGE_S1];      //ɱ��ģʽ
                guc_com_tx_buffer[14] = 0x00;    //Ԥ��
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
function:  ��ÿ�ν�������ʱ�������ʼ����
           ����ʱ����Ҫ���յ�������գ�׼�����������ݣ�

******************************************************************************/
void com_rx_init(void)   //ͨѶ���ճ�ʼ����������ѭ�������е���
{
    uint8_t i;
    
    for (i = 0; i < 65; i++)       //20
    {
        guc_com_rx_buffer[i] = 0;  //����ջ�����
    }
    
    bflg_com_rx_busy = 0;
    guc_com_rx_point = 0;
}
//------------------------------------------------------------------------------
/*******************************************************************************
function : ���ճɹ��󣬶����ݽ��д��� ��

           bflg_read_write_data--- �������͸���ʾ�尴��:���Ͳ�ѯָ������趨ֵ
      ����������ѭ�����͵ķ�ʽ��

        ��Ϊ������Ҫ��ͣ���жϴ�ʱ���µİ�������������"���±����趨"��Ҫ�ı��жϵķ�ֵ�ˣ�
            ��ʾ��Ҳ��Ҫ֪����ʱ��������Щ���ϣ���������ʾ��
        ���Զ�/дҪѭ��ִ�У�
********************************************************************************/
void com_rx_data_deal(void)   //ͨѶ�������ݴ����������ѭ�������е���
{
    uint8_t i,msb_byte = 0,lsb_byte = 0;
    uint16_t lus_tmp,lus_THW_tmp;
    word_type com_crc;
    word_type gss_tmp;
    //------------------------------------------------------
    if (bflg_com_type == 0)   //�����LED��ʾ��ͨѶ
    {
        if(guc_com_bus_add == LED_DIS_ADD)
        {
            if ((guc_com_rx_buffer[1] == 0x03))  //����Ƕ�����  //peak ��������ֵ
            {
                com_crc.uword = CRC16(guc_com_rx_buffer, 7);
                if ((com_crc.ubyte.low == guc_com_rx_buffer[7])
                 && (com_crc.ubyte.high == guc_com_rx_buffer[8]))
                {
                    bflg_com_type_ok = 1;   //ͨѶ�����ж�ok
                    bflg_bus_rx_busying = 0;
                    
                    bflg_com_tx_delaytime = 1;         
                    gss_com_tx_delaytimer = 15;       //ԭ10 
                    
                    bflg_com_fault = 0;               //��ͨѶ���ϱ�־
                    gss_com_fault_delaytimer = 0;     //��ͨѶ���ϼ�ʱ��
                    //------------------------------------------
                    //���ݴ���
                    bflg_read_write_data = 1;         //��Ϊд����

                    guc_led_comm_cnt++;
                    if(guc_led_comm_cnt >= 20)  
                    {
                        guc_led_comm_cnt = 0;
                        guc_com_bus_add = EXPAND_ADD;     //�л�����չ��
                    }
                    
                    if (guc_key_val != guc_com_rx_buffer[3])
                    {
                        bflg_read_key_delaytime = 0; //peak ����ֵ�иı䣬����Ҫ�ȴ�һ��ʱ������жϼ�ֵ������ֱ�Ӵ��� read_key_deal()
                        gss_read_key_delaytimer = 0;
                    }
                    guc_key_val = guc_com_rx_buffer[3];    
            
                    //ע��״̬ʱ��ʾ��IC����ע��ĵ�ַ����ʾ��ת��Ϊ��Ƭ������ = xx/4;
                    guc_flash_add = 0;
                    msb_byte = guc_com_rx_buffer[4];
                    lsb_byte = guc_com_rx_buffer[5];
                    guc_flash_add = ((guc_flash_add | msb_byte)<<8) | lsb_byte;
                     
                    //peak add ����ָ��
                    if ((guc_com_rx_buffer[6] & 0x80) != 0)
                    {
                          guc_IC_access_state = 1;
                    }
                    else
                    {
                          guc_IC_access_state = 0;
                    }
                    //--------------------------------------------------------
                    //U��״̬
                    guc_U_disk_state = (uint8_t)(guc_com_rx_buffer[6]&0x03);
                    guc_usb_state    = (uint8_t)(guc_com_rx_buffer[6]&0x04);
                    //------------------------------------------------------
                    //���԰汾��ʾ��ȷ��
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
            else if (guc_com_rx_buffer[1] == 0x10)    //�����д����  //peak �趨����ܡ�LED��ʾ  ����IC���ڴ˽���
            {
                com_crc.uword = CRC16(guc_com_rx_buffer, 6);
                if ((com_crc.ubyte.low == guc_com_rx_buffer[6])        //peak ���CRCУ����ȷ
                 && (com_crc.ubyte.high == guc_com_rx_buffer[7]))
                {
                    bflg_com_type_ok = 1;           //ͨѶ�����ж�ok      //peak ??
                    bflg_bus_rx_busying = 0;
                    
                    bflg_com_tx_delaytime = 1;      //�÷�����ʱ��־      //peak ֮��ʱ�䵽���ٴ�������
                    gss_com_tx_delaytimer = 10;     //������ʱ��ʱ����ֵ
                    
                    bflg_com_fault = 0;             //��ͨѶ���ϱ�־      //peak ?? δ��
                    gss_com_fault_delaytimer = 0;   //��ͨѶ���ϼ�ʱ��
                    //------------------------------------------
                    //���ݴ���
                    bflg_read_write_data = 0;       //��Ϊ������          //peak                     
                }
            }
        }
        else if(guc_com_bus_add == EXPAND_ADD)
        {
            if ((guc_com_rx_buffer[1] == 0x03))  //����Ƕ�����  
            {
                com_crc.uword = CRC16(guc_com_rx_buffer, 9);
                if ((com_crc.ubyte.low == guc_com_rx_buffer[9])
                 && (com_crc.ubyte.high == guc_com_rx_buffer[10]))
                {
                    bflg_com_type_ok = 1;   //ͨѶ�����ж�ok
                    bflg_bus_rx_busying = 0;
                    
                    bflg_com_tx_delaytime = 1;         
                    gss_com_tx_delaytimer = 15;        
                    
                    bflg_com_fault = 0;               //��ͨѶ���ϱ�־
                    gss_com_fault_delaytimer = 0;     //��ͨѶ���ϼ�ʱ��
                    //------------------------------------------
                    //���ݴ���
                    bflg_read_write_data = 1;         //��Ϊд����

                    guc_com_bus_add = LED_DIS_ADD;    //�л�����ʾ��
                    
                    lus_THW_tmp = 0;
                    lus_THW_tmp |= guc_com_rx_buffer[3];  //����H
                    lus_THW_tmp = lus_THW_tmp<<8;
                    lus_THW_tmp |= guc_com_rx_buffer[4];
                    gss_THW = (int16_t)lus_THW_tmp + gss_factory_parm_value[DISP_FAC_Ct6];
		    
                    //guc_humidity = 0x00;                           //ʪ��H
                    guc_humidity = guc_com_rx_buffer[6];             //L

                    if((guc_com_rx_buffer[7]&0x01) == 0x01)
                    {
                        bflg_sterilization_state = 1;                //ɱ��״̬
                    }
                    else
                    {
                        bflg_sterilization_state = 0;
                    }
                    if((guc_com_rx_buffer[7]&0x02) == 0x02)
                    {
                        bflg_THW_fault = 1;                         //���´���������
                        gus_trip_code1 |= THW_S_CODE;
                    }
                    else
                    {
                        bflg_THW_fault = 0;
                        gus_trip_code1 &= ~THW_S_CODE;
                    }
                    if((guc_com_rx_buffer[7]&0x04) == 0x04)
                    {
                        bflg_humidity_fault = 1;                   //ʪ�ȹ���
                    }
                    else
                    {
                        bflg_humidity_fault = 0;
                    }
                    if((guc_com_rx_buffer[7]&0x08) == 0x08)
                    {
                        bflg_sterilize_fault = 1;                   //����ɱ��ģ�����
                        gus_trip_code2 |= OZONE_F_CODE;                        
                    }
                    else
                    {
                        bflg_sterilize_fault = 0;
                        gus_trip_code2 &= ~OZONE_F_CODE;                        
                    }
                    //guc_com_rx_buffer[8];                         //Ԥ��
                }
            }
            else if (guc_com_rx_buffer[1] == 0x10)    //�����д����  //peak �趨����ܡ�LED��ʾ  ����IC���ڴ˽���
            {
                com_crc.uword = CRC16(guc_com_rx_buffer, 6);
                if ((com_crc.ubyte.low == guc_com_rx_buffer[6])        //peak ���CRCУ����ȷ
                 && (com_crc.ubyte.high == guc_com_rx_buffer[7]))
                {
                    bflg_com_type_ok = 1;           //ͨѶ�����ж�ok      //peak ??
                    bflg_bus_rx_busying = 0;
                    
                    bflg_com_tx_delaytime = 1;      //�÷�����ʱ��־      //peak ֮��ʱ�䵽���ٴ�������
                    gss_com_tx_delaytimer = 10;     //������ʱ��ʱ����ֵ
                    
                    bflg_com_fault = 0;             //��ͨѶ���ϱ�־      //peak ?? δ��
                    gss_com_fault_delaytimer = 0;   //��ͨѶ���ϼ�ʱ��
                    //------------------------------------------
                    //���ݴ���
                    bflg_read_write_data = 0;       //��Ϊ������          //peak 
                }
            }
        }       
    }
    else  //�����Һ����ͨѶ
    {
        if(guc_com_bus_add == LIQUID_DIS_ADD)    
        {
            if ((guc_com_rx_buffer[1] == 0x03))  //����Ƕ�����
            {
                if (bflg_read_user_admin == 0)   //����Ƕ��û���Ϣ
                {
                    com_crc.uword = CRC16(guc_com_rx_buffer, ((READ_USER_CNT * 2) + 3));
                    if ((com_crc.ubyte.low == guc_com_rx_buffer[(READ_USER_CNT * 2) + 3])
                     && (com_crc.ubyte.high == guc_com_rx_buffer[(READ_USER_CNT * 2) + 4]))
                    {
                        bflg_com_type_ok = 1;              //ͨѶ�����ж�ok
                        bflg_bus_rx_busying = 0;
                        
                        bflg_com_tx_delaytime = 1;         //�÷�����ʱ��־
                        gss_com_tx_delaytimer = 15;        //������ʱ��ʱ����ֵ
                        
                        bflg_com_fault = 0;                //��ͨѶ���ϱ�־
                        gss_com_fault_delaytimer = 0;      //��ͨѶ���ϼ�ʱ��
                        //----------------------------------------------------
                        //���ݴ���
                        //bflg_read_write_data = 1;        //��Ϊд����
                        bflg_read_user_admin = 1;          //��Ϊ������Ա��Ϣ
                        
                        //--------------------------------------
                        gss_tmp.ubyte.low = guc_com_rx_buffer[3];
                        gss_tmp.ubyte.high = guc_com_rx_buffer[4];
                        gss_user_set_parm_value[DISP_USER_SET_TS] = gss_tmp.sword;           //�õ��趨�¶�ok
                        //ram_para[num_user_set_TS] = gss_user_set_parm_value[DISP_USER_SET_TS];  
                        //--------------------------------------
                        gss_tmp.ubyte.low = guc_com_rx_buffer[5];
                        gss_tmp.ubyte.high = guc_com_rx_buffer[6];
                        gss_user_set_parm_value[DISP_USER_SET_ALH] = gss_tmp.sword;          //�õ����±����¶�ok
                        //ram_para[num_user_set_ALH] = gss_user_set_parm_value[DISP_USER_SET_ALH];
                        //--------------------------------------
                        gss_tmp.ubyte.low = guc_com_rx_buffer[7];
                        gss_tmp.ubyte.high = guc_com_rx_buffer[8];
                        gss_user_set_parm_value[DISP_USER_SET_ALL] = gss_tmp.sword;          //�õ����±����¶�ok
                        //ram_para[num_user_set_ALL] = gss_user_set_parm_value[DISP_USER_SET_ALL];
                        //--------------------------------------
                        gss_tmp.ubyte.low = guc_com_rx_buffer[9];
                        gss_tmp.ubyte.high = guc_com_rx_buffer[10];
                        gss_liquid_led_button = gss_tmp.sword;                           //���������ƿ��ư�ťok
                        //--------------------------------------
                        gss_tmp.ubyte.low = guc_com_rx_buffer[11];
                        gss_tmp.ubyte.high = guc_com_rx_buffer[12];
                        gss_liquid_ozone_disinfect_button = gss_tmp.sword;               //����ɱ��������ťok
                        //--------------------------------------
                        gss_tmp.ubyte.low = guc_com_rx_buffer[13];
                        gss_tmp.ubyte.high = guc_com_rx_buffer[14];
                        gss_user_manage_parm_value[DISP_USER_MANAGE_Lt] = gss_tmp.sword; //������������ʱ�ر�ʱ��ok
                        //--------------------------------------
                        gss_tmp.ubyte.low = guc_com_rx_buffer[15];
                        gss_tmp.ubyte.high = guc_com_rx_buffer[16];
                        gss_user_manage_parm_value[DISP_USER_MANAGE_dA] = gss_tmp.sword; //���ŷ�����������ʱʱ��ok--������Һ������
                        //--------------------------------------
                        gss_tmp.ubyte.low = guc_com_rx_buffer[17];                 
                        gss_tmp.ubyte.high = guc_com_rx_buffer[18];
                        gss_user_manage_parm_value[DISP_USER_MANAGE_FA] = gss_tmp.sword; //����������ok--������Һ������
                        //--------------------------------------
                        gss_tmp.ubyte.low = guc_com_rx_buffer[19];
                        gss_tmp.ubyte.high = guc_com_rx_buffer[20];
                        gss_user_manage_parm_value[DISP_USER_MANAGE_S1] = gss_tmp.sword; //ɱ��ģʽok
                        //--------------------------------------
                        gss_tmp.ubyte.low = guc_com_rx_buffer[21];
                        gss_tmp.ubyte.high = guc_com_rx_buffer[22];
                        gss_user_manage_parm_value[DISP_USER_MANAGE_S2] = gss_tmp.sword; //���ɱ��ʱ��ok
                        //--------------------------------------
                        gss_tmp.ubyte.low = guc_com_rx_buffer[23];
                        gss_tmp.ubyte.high = guc_com_rx_buffer[24];
                        gss_user_manage_parm_value[DISP_USER_MANAGE_TT] = gss_tmp.sword; //�¶ȵ�λok--������Һ������
                        //--------------------------------------
                        gss_tmp.ubyte.low = guc_com_rx_buffer[25];
                        gss_tmp.ubyte.high = guc_com_rx_buffer[26];
                        ram_para[num_comp2_backup_TS] = gss_tmp.sword;         //��--��+
                        //--------------------------------------
                        gss_tmp.ubyte.low = guc_com_rx_buffer[27];
                        gss_tmp.ubyte.high = guc_com_rx_buffer[28];
                        ram_para[num_comp2_backup_TS] = gss_tmp.sword;         //��--��+
                        //--------------------------------------
                        gss_tmp.ubyte.low = guc_com_rx_buffer[29];
                        gss_tmp.ubyte.high = guc_com_rx_buffer[30];
                        ram_para[num_comp2_backup_TS] = gss_tmp.sword;         //��--��+
                        //--------------------------------------
                        gss_tmp.ubyte.low = guc_com_rx_buffer[31];
                        gss_tmp.ubyte.high = guc_com_rx_buffer[32];
                        ram_para[num_comp2_backup_TS] = gss_tmp.sword;         //ʱ--��+
                        //--------------------------------------
                        gss_tmp.ubyte.low = guc_com_rx_buffer[33];
                        gss_tmp.ubyte.high = guc_com_rx_buffer[34];
                        ram_para[num_comp2_backup_TS] = gss_tmp.sword;         //��--��+
                        //--------------------------------------
                        gss_tmp.ubyte.low = guc_com_rx_buffer[35];
                        gss_tmp.ubyte.high = guc_com_rx_buffer[36];
                        ram_para[num_comp2_backup_TS] = gss_tmp.sword;         //��--��                   
                    }
                }
                else    //����Ƕ�����Ա��Ϣ
                {
                    com_crc.uword = CRC16(guc_com_rx_buffer, ((READ_ADMIN_CNT * 2) + 3));
                    if ((com_crc.ubyte.low == guc_com_rx_buffer[(READ_ADMIN_CNT * 2) + 3])
                     && (com_crc.ubyte.high == guc_com_rx_buffer[(READ_ADMIN_CNT * 2) + 4]))
                    {
                        bflg_com_type_ok = 1;              //ͨѶ�����ж�ok
                        bflg_bus_rx_busying = 0;
                        
                        bflg_com_tx_delaytime = 1;         //�÷�����ʱ��־
                        gss_com_tx_delaytimer = 15;        //������ʱ��ʱ����ֵ
                        
                        bflg_com_fault = 0;                //��ͨѶ���ϱ�־
                        gss_com_fault_delaytimer = 0;      //��ͨѶ���ϼ�ʱ��
                        //--------------------------------------------------------
                        //���ݴ���
                        bflg_read_write_data = 1;          //��Ϊд����
                        bflg_read_user_admin = 0;          //��Ϊ���û���Ϣ

                        guc_com_bus_add = EXPAND_ADD;      //�л���չ���ַ
                        
                        //--------------------------------------
                        gss_tmp.ubyte.low = guc_com_rx_buffer[3];
                        gss_tmp.ubyte.high = guc_com_rx_buffer[4];
                        gss_factory_parm_value[DISP_FAC_r0] = gss_tmp.sword;     //�����²�ok
                        //--------------------------------------
                        gss_tmp.ubyte.low = guc_com_rx_buffer[5];
                        gss_tmp.ubyte.high = guc_com_rx_buffer[6];
                        gss_factory_parm_value[DISP_FAC_Ct1] = gss_tmp.sword;    //�ϴ�����У׼ֵok
                        //--------------------------------------
                        gss_tmp.ubyte.low = guc_com_rx_buffer[7];
                        gss_tmp.ubyte.high = guc_com_rx_buffer[8];
                        gss_factory_parm_value[DISP_FAC_Ct2] = gss_tmp.sword;    //�´�����У׼ֵok
                        //--------------------------------------
                        gss_tmp.ubyte.low = guc_com_rx_buffer[9];
                        gss_tmp.ubyte.high = guc_com_rx_buffer[10];
                        gss_factory_parm_value[DISP_FAC_Ct3] = gss_tmp.sword;    //���ƴ�����У׼ֵok
                        //--------------------------------------
                        gss_tmp.ubyte.low = guc_com_rx_buffer[11];
                        gss_tmp.ubyte.high = guc_com_rx_buffer[12];
                        gss_factory_parm_value[DISP_FAC_Ct4] = gss_tmp.sword;    //��˪������У׼ֵok
                        //--------------------------------------
                        gss_tmp.ubyte.low = guc_com_rx_buffer[13];
                        gss_tmp.ubyte.high = guc_com_rx_buffer[14];
                        gss_factory_parm_value[DISP_FAC_Ct5] = gss_tmp.sword;    //����������У׼ֵok
                        //--------------------------------------
                        gss_tmp.ubyte.low = guc_com_rx_buffer[15];
                        gss_tmp.ubyte.high = guc_com_rx_buffer[16];
                        gss_factory_parm_value[DISP_FAC_Ct6] = gss_tmp.sword;    //���´�����У׼ֵok
                        //--------------------------------------
                        gss_tmp.ubyte.low = guc_com_rx_buffer[17];
                        gss_tmp.ubyte.high = guc_com_rx_buffer[18];
                        gss_factory_parm_value[DISP_FAC_Ct7] = gss_tmp.sword;    //ʪ�ȴ�����У׼ֵok
                        //--------------------------------------
                        gss_tmp.ubyte.low = guc_com_rx_buffer[19];
                        gss_tmp.ubyte.high = guc_com_rx_buffer[20];
                        gss_factory_parm_value[DISP_FAC_CU] = gss_tmp.sword;     //�е��ѹУ׼ֵok
                        //--------------------------------------
                        gss_tmp.ubyte.low = guc_com_rx_buffer[21];
                        gss_tmp.ubyte.high = guc_com_rx_buffer[22];
                        gss_factory_parm_value[DISP_FAC_Ad] = gss_tmp.sword;     //����������ʱok
                        //-------------------------------------- 
                        gss_tmp.ubyte.low = guc_com_rx_buffer[23];
                        gss_tmp.ubyte.high = guc_com_rx_buffer[24];
                        gss_factory_parm_value[DISP_FAC_AAH] = gss_tmp.sword;    //�������±���ֵok
                        //--------------------------------------
                        gss_tmp.ubyte.low = guc_com_rx_buffer[25];
                        gss_tmp.ubyte.high = guc_com_rx_buffer[26];
                        gss_factory_parm_value[DISP_FAC_AAL] = gss_tmp.sword;    //�������±���ֵok
                        //--------------------------------------
                        gss_tmp.ubyte.low = guc_com_rx_buffer[27];
                        gss_tmp.ubyte.high = guc_com_rx_buffer[28];
                        gss_factory_parm_value[DISP_FAC_r1] = gss_tmp.sword;     //�������౨���뻷���²�ok                   
                        //--------------------------------------
                        gss_tmp.ubyte.low = guc_com_rx_buffer[29];
                        gss_tmp.ubyte.high = guc_com_rx_buffer[30];
                        gss_factory_parm_value[DISP_FAC_AHH] = gss_tmp.sword;    //���¹��߱���ֵok
                        //--------------------------------------
                        gss_tmp.ubyte.low = guc_com_rx_buffer[31];
                        gss_tmp.ubyte.high = guc_com_rx_buffer[32];
                        gss_factory_parm_value[DISP_FAC_T3] = gss_tmp.sword;     //ѹ�������ͣ��ʱ��ok
                        //--------------------------------------
                        gss_tmp.ubyte.low = guc_com_rx_buffer[33];
                        gss_tmp.ubyte.high = guc_com_rx_buffer[34];
                        gss_factory_parm_value[DISP_FAC_Cd] = gss_tmp.sword;     //��ʼ�ϵ�ѹ��������ʱok
                        //--------------------------------------
                        gss_tmp.ubyte.low = guc_com_rx_buffer[35];
                        gss_tmp.ubyte.high = guc_com_rx_buffer[36];
                        gss_factory_parm_value[DISP_FAC_Fd] = gss_tmp.sword;     //������ȡ���������������ʱok
                        //--------------------------------------
                        gss_tmp.ubyte.low = guc_com_rx_buffer[37];
                        gss_tmp.ubyte.high = guc_com_rx_buffer[38];
                        gss_factory_parm_value[DISP_FAC_LC] = gss_tmp.sword;     //���������ƿ��Ʒ�ʽok
                        //--------------------------------------
                        gss_tmp.ubyte.low = guc_com_rx_buffer[39];
                        gss_tmp.ubyte.high = guc_com_rx_buffer[40];
                        gss_factory_parm_value[DISP_FAC_dFL] = gss_tmp.sword;    //���뻯˪�¶�ok
                        //--------------------------------------
                        gss_tmp.ubyte.low = guc_com_rx_buffer[41];
                        gss_tmp.ubyte.high = guc_com_rx_buffer[42];
                        gss_factory_parm_value[DISP_FAC_dFH] = gss_tmp.sword;    //�˳���˪�¶�ok
                        //--------------------------------------
                        gss_tmp.ubyte.low = guc_com_rx_buffer[43];
                        gss_tmp.ubyte.high = guc_com_rx_buffer[44];
                        gss_factory_parm_value[DISP_FAC_HT1] = gss_tmp.sword;    //�ż�������ʪ��ok
                        //--------------------------------------
                        gss_tmp.ubyte.low = guc_com_rx_buffer[45];
                        gss_tmp.ubyte.high = guc_com_rx_buffer[46];
                        gss_factory_parm_value[DISP_FAC_HT2] = gss_tmp.sword;    //�ż���ֹͣʪ��ok
                        //--------------------------------------
                        gss_tmp.ubyte.low = guc_com_rx_buffer[47];
                        gss_tmp.ubyte.high = guc_com_rx_buffer[48];
                        gss_factory_parm_value[DISP_FAC_PCH] = gss_tmp.sword;    //ѪҺ��/ҩƷ������л�ok                 
                    }
                }
            }
            else if (guc_com_rx_buffer[1] == 0x10)    //�����д����
            {
                com_crc.uword = CRC16(guc_com_rx_buffer, 6);
                if ((com_crc.ubyte.low == guc_com_rx_buffer[6])
                 && (com_crc.ubyte.high == guc_com_rx_buffer[7]))
                {
                    bflg_com_type_ok = 1;             //ͨѶ�����ж�ok
                    bflg_bus_rx_busying = 0;
                    
                    bflg_com_tx_delaytime = 1;        //�÷�����ʱ��־
                    gss_com_tx_delaytimer = 15;       //������ʱ��ʱ����ֵ
                    
                    bflg_com_fault = 0;               //��ͨѶ���ϱ�־
                    gss_com_fault_delaytimer = 0;     //��ͨѶ���ϼ�ʱ��
                    //----------------------------------------------------
                    //���ݴ���
                    bflg_read_write_data = 0;         //��Ϊ������
                }
            }
        }
        else if(guc_com_bus_add == EXPAND_ADD)
        {
            if ((guc_com_rx_buffer[1] == 0x03))  //����Ƕ�����  
            {
                com_crc.uword = CRC16(guc_com_rx_buffer, 9);
                if ((com_crc.ubyte.low == guc_com_rx_buffer[9])
                 && (com_crc.ubyte.high == guc_com_rx_buffer[10]))
                {
                    bflg_com_type_ok = 1;   //ͨѶ�����ж�ok
                    bflg_bus_rx_busying = 0;
                    
                    bflg_com_tx_delaytime = 1;         
                    gss_com_tx_delaytimer = 15;        
                    
                    bflg_com_fault = 0;               //��ͨѶ���ϱ�־
                    gss_com_fault_delaytimer = 0;     //��ͨѶ���ϼ�ʱ��
                    //------------------------------------------
                    //���ݴ���
                    bflg_read_write_data = 1;         //��Ϊд����

                    guc_com_bus_add = LIQUID_DIS_ADD;    //�л���Һ����
                    
                    lus_THW_tmp = 0;
                    lus_THW_tmp |= guc_com_rx_buffer[3];  //����H
                    lus_THW_tmp = lus_THW_tmp<<8;
                    lus_THW_tmp |= guc_com_rx_buffer[4];
                    gss_THW = (int16_t)lus_THW_tmp + gss_factory_parm_value[DISP_FAC_Ct6];
		    
                    //guc_humidity = 0x00;                           //ʪ��H
                    guc_humidity = guc_com_rx_buffer[6];             //L

                    if((guc_com_rx_buffer[7]&0x01) == 0x01)
                    {
                        bflg_sterilization_state = 1;                //ɱ��״̬
                    }
                    else
                    {
                        bflg_sterilization_state = 0;
                    }
                    if((guc_com_rx_buffer[7]&0x02) == 0x02)
                    {
                        bflg_THW_fault = 1;                         //���´���������
                        gus_trip_code1 |= THW_S_CODE;
                    }
                    else
                    {
                        bflg_THW_fault = 0;
                        gus_trip_code1 &= ~THW_S_CODE;
                    }
                    if((guc_com_rx_buffer[7]&0x04) == 0x04)
                    {
                        bflg_humidity_fault = 1;                   //ʪ�ȹ���
                    }
                    else
                    {
                        bflg_humidity_fault = 0;
                    }
                    if((guc_com_rx_buffer[7]&0x08) == 0x08)
                    {
                        bflg_sterilize_fault = 1;                   //����ɱ��ģ�����
                        gus_trip_code2 |= OZONE_F_CODE;                        
                    }
                    else
                    {
                        bflg_sterilize_fault = 0;
                        gus_trip_code2 &= ~OZONE_F_CODE;                        
                    }
                    //guc_com_rx_buffer[8];                         //Ԥ��
                }
            }
            else if (guc_com_rx_buffer[1] == 0x10)    //�����д����  //peak �趨����ܡ�LED��ʾ  ����IC���ڴ˽���
            {
                com_crc.uword = CRC16(guc_com_rx_buffer, 6);
                if ((com_crc.ubyte.low == guc_com_rx_buffer[6])        //peak ���CRCУ����ȷ
                 && (com_crc.ubyte.high == guc_com_rx_buffer[7]))
                {
                    bflg_com_type_ok = 1;           //ͨѶ�����ж�ok      //peak ??
                    bflg_bus_rx_busying = 0;
                    
                    bflg_com_tx_delaytime = 1;      //�÷�����ʱ��־      //peak ֮��ʱ�䵽���ٴ�������
                    gss_com_tx_delaytimer = 15;     //������ʱ��ʱ����ֵ
                    
                    bflg_com_fault = 0;             //��ͨѶ���ϱ�־      //peak ?? δ��
                    gss_com_fault_delaytimer = 0;   //��ͨѶ���ϼ�ʱ��
                    //------------------------------------------------------------
                    //���ݴ���
                    bflg_read_write_data = 0;       //��Ϊ������           
                }
            }
        }
    }
}
//------------------------------------------------------------------------------
/*******************************************************************************
function:  bflg_com_rx_delaytime ��1����ʱgss_com_rx_delaytimer �趨��ʱ���
           ���Դ��ж�������գ�
*******************************************************************************/
void com_rx_delaytime(void)     //ͨѶ������ʱ������1ms��ʱ�����е���
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
function:  bflg_com_tx_delaytime �˱�־��1����ʱgss_com_tx_delaytimer �趨��ʱ��
           �󣬴򿪷����жϣ������ͣ�
          
*******************************************************************************/
void com_tx_delaytime(void)     //ͨѶ������ʱ������1ms��ʱ�����е���
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
function: ��������ʱ���೤ʱ�����δ��ȷ���յ����ݣ�����û�н����꣬
          ��ֹͣ���ݵĽ��գ�Ҳ���Խ��յ����ݽ����κεĴ���
          
*******************************************************************************/
void com_rx_end_delaytime(void) //ͨѶ���������ʱ������1ms��ʱ�����е���
{
    /*if (bflg_com_rx_busy == 1)  //�����ǰ����æ
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
function: û�жԷ�������ʱ������½�����Ӧ�Ĵ���
            

����λ��:1s��ʱ����-------------------------ok
******************************************************************************/
void com_fault_delaytime(void)     //ͨѶ������ʱ������1s��ʱ�����е���
{
    if (bflg_com_fault == 0)                //peak ����ж��˲�û��������
    {
        gss_com_fault_delaytimer++;
        if (gss_com_fault_delaytimer >= 30)  //30
        {
            gss_com_fault_delaytimer = 0;
            bflg_com_fault = 1;
        }
    }
    //---------------------------------------------------------------------
    //ͨѶ�������ж�
    if (bflg_com_type_ok == 0)               //ͨѶ��ȷ�� = 1������ȷ��Ϊ0��
    {
        guc_com_type_cnt++;                  //peak ��ʼͨѶʱ������5������
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
function :�ж��д���
          ����Ҫ���͵�����----> |1�����Ͷ���ʾ�����ݵ�ָ��  (8���ֽ�)
                                |2����������ʾ��д������    (23���ֽ�)
    
******************************************************************************/
void com_tx_int(void)    //ͨѶ�����жϳ���
{
    if (bflg_com_tx_busy == 1)
    {
        if (bflg_com_type == 0)    //�����LED��ʾ��ͨѶ
        {
            if(guc_com_bus_add == LED_DIS_ADD)
            {
                if (bflg_read_write_data == 0)   //����Ƕ�����
                {
                    if (guc_com_tx_point < 8)
                    {
                        TXD0 = guc_com_tx_buffer[guc_com_tx_point++];     //��������
                    }
                    else
                    {
                        guc_com_tx_point = 0;    //�巢���ֽ�����
                        bflg_com_tx_busy = 0;    //�巢��æ��־
                        bflg_com_tx_ok = 1;
                        
                        bflg_com_rx_delaytime = 1;    //��comͨѶ������ʱ��־
                        gss_com_rx_delaytimer = 5;    //comͨѶ������ʱ��ʱ����ֵ

                        /*if(bflg_com_type_ok == 0)
                        {
                            bflg_com_tx_delaytime = 1;    //�ŵ�485�л���-----����Һ��ȥ��ע��
                            gss_com_tx_delaytimer = 100;
                        }*/
                    }
                }
                else    //�����д����
                {
                    if (guc_com_tx_point < 53)  //23
                    {
                        TXD0 = guc_com_tx_buffer[guc_com_tx_point++];     //��������
                    }
                    else
                    {
                        guc_com_tx_point = 0;    //�巢���ֽ�����  
                        bflg_com_tx_busy = 0;    //�巢��æ��־    
                        bflg_com_tx_ok = 1;
                        
                        bflg_com_rx_delaytime = 1;    //��ͨѶ������ʱ��־
                        gss_com_rx_delaytimer = 5;    //ͨѶ������ʱ��ʱ����ֵ
                        
                        if(bflg_com_type_ok == 0)
                        {
                            bflg_com_tx_delaytime = 1;    //δȷ����ʾ���ã���ȷ����ʾ������дָ������ֻ�ڴ˴�
                            gss_com_tx_delaytimer = 100;
                        }
                    }
                }
            }
            else if(guc_com_bus_add == EXPAND_ADD)
            {
                if (bflg_read_write_data == 0)   //����Ƕ�����
                {
                    if (guc_com_tx_point < 8)
                    {
                        TXD0 = guc_com_tx_buffer[guc_com_tx_point++];     //��������
                    }
                    else
                    {
                        guc_com_tx_point = 0;    //�巢���ֽ�����
                        bflg_com_tx_busy = 0;    //�巢��æ��־
                        bflg_com_tx_ok = 1;
                        
                        bflg_com_rx_delaytime = 1;    //��comͨѶ������ʱ��־
                        gss_com_rx_delaytimer = 5;    //comͨѶ������ʱ��ʱ����ֵ

                        //bflg_com_tx_delaytime = 1;
                        //gss_com_tx_delaytimer = 100;
                    }
                }
                else    //�����д����
                {
                    if (guc_com_tx_point < 23)
                    {
                        TXD0 = guc_com_tx_buffer[guc_com_tx_point++];     //��������
                    }
                    else
                    {
                        guc_com_tx_point = 0;    //�巢���ֽ�����
                        bflg_com_tx_busy = 0;    //�巢��æ��־
                        bflg_com_tx_ok = 1;
                        
                        bflg_com_rx_delaytime = 1;    //��ͨѶ������ʱ��־
                        gss_com_rx_delaytimer = 5;    //ͨѶ������ʱ��ʱ����ֵ
                        
                        //bflg_com_tx_delaytime = 1;
                        //gss_com_tx_delaytimer = 100;
                    }
                }
            }
        }
        else   //�����Һ����ͨѶ
        {
            if(guc_com_bus_add == LIQUID_DIS_ADD)
            {
                if (bflg_read_write_data == 0)   //����Ƕ�����
                {
                    if (guc_com_tx_point < 8)
                    {
                        TXD0 = guc_com_tx_buffer[guc_com_tx_point++];     //��������
                    }
                    else
                    {
                        guc_com_tx_point = 0;    //�巢���ֽ�����
                        bflg_com_tx_busy = 0;    //�巢��æ��־
                        bflg_com_tx_ok = 1;
                        
                        bflg_com_rx_delaytime = 1;    //��comͨѶ������ʱ��־
                        gss_com_rx_delaytimer = 5;    //comͨѶ������ʱ��ʱ����ֵ
                        
                        //bflg_com_tx_delaytime = 1;
                        //gss_com_tx_delaytimer = 1000;
                    }
                }
                else    //�����д����
                {
                    if (guc_com_tx_point < ((WRITE_STATE_CNT * 2) + 9))   //109
                    {
                        TXD0 = guc_com_tx_buffer[guc_com_tx_point++];     //��������
                    }
                    else
                    {
                        guc_com_tx_point = 0;     //�巢���ֽ�����
                        bflg_com_tx_busy = 0;     //�巢��æ��־
                        bflg_com_tx_ok = 1;
                        
                        bflg_com_rx_delaytime = 1;     //��ͨѶ������ʱ��־
                        gss_com_rx_delaytimer = 5;     //ͨѶ������ʱ��ʱ����ֵ
                        
                        if(bflg_com_type_ok == 0)
                        {
                            bflg_com_tx_delaytime = 1;    
                            gss_com_tx_delaytimer = 1000;
                        }
                        //debug ͨѶָʾ��
                        TRIP_LAMP_PIN = ~TRIP_LAMP_PIN;
                    }
                }
            }
            else if(guc_com_bus_add == EXPAND_ADD)
            {
                if (bflg_read_write_data == 0)   //����Ƕ�����
                {
                    if (guc_com_tx_point < 8)
                    {
                        TXD0 = guc_com_tx_buffer[guc_com_tx_point++];     //��������
                    }
                    else
                    {
                        guc_com_tx_point = 0;    //�巢���ֽ�����
                        bflg_com_tx_busy = 0;    //�巢��æ��־
                        bflg_com_tx_ok = 1;
                        
                        bflg_com_rx_delaytime = 1;    //��comͨѶ������ʱ��־
                        gss_com_rx_delaytimer = 5;    //comͨѶ������ʱ��ʱ����ֵ

                        //bflg_com_tx_delaytime = 1;
                        //gss_com_tx_delaytimer = 100;
                    }
                }
                else    //�����д����
                {
                    if (guc_com_tx_point < 23)
                    {
                        TXD0 = guc_com_tx_buffer[guc_com_tx_point++];     //��������
                    }
                    else
                    {
                        guc_com_tx_point = 0;    //�巢���ֽ�����
                        bflg_com_tx_busy = 0;    //�巢��æ��־
                        bflg_com_tx_ok = 1;
                        
                        bflg_com_rx_delaytime = 1;    //��ͨѶ������ʱ��־
                        gss_com_rx_delaytimer = 5;    //ͨѶ������ʱ��ʱ����ֵ
                        
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
function: �жϽ��յ�����

          *****LED****
          ����յ����Ƿ������ѯ����󷵻ص�ֵ:
          ���յ�9���ֽڡ��������п���������չ�ģ����Ǵ˳����Ͳ�ѯ�趨Ϊ�̶���     
      ��ѯ2���Ĵ��������Է���ʱҲ�ǹ̶���4���ֽ�(һ���Ĵ�������2���ֽ�);
      
       ----------------------------------------   
          ����յ����Ƿ������趨����󷵻ص�ֵ:
          ���յ�8���ֽڣ��������о���8����

*******************************************************************************/
void com_rx_int(uint8_t rx_data)   //ͨѶ�����жϳ���
{
    uint8_t luc_com_rx_byte;  //�����ֽ���ʱ������
    
    gss_com_rx_end_delaytimer = 0; //���������ʱ��ʱ��
    //--------------------------------------------------
    luc_com_rx_byte = rx_data;
    
    if (bflg_com_type == 0)   //�����LED��ʾ��ͨѶ
    {
        if ((bflg_com_rx_busy == 0) && ((luc_com_rx_byte == 0x00)||(luc_com_rx_byte == 0x02)))     //����޽���æ���Ҵӵ�ַ��ȷ
        {
            bflg_com_rx_busy = 1;
            
            guc_com_rx_buffer[0] = luc_com_rx_byte;
            guc_com_rx_point = 1;
        }      
        else if (bflg_com_rx_busy == 1)      //�������æ
        {
            guc_com_rx_buffer[guc_com_rx_point++] = luc_com_rx_byte;
            if(guc_com_rx_buffer[0] == 0x00)   //����LED��ʾ��
            {
                if ((guc_com_rx_buffer[1] == 0x03) && (guc_com_rx_point >= 9))       //����ǲ�ѯ����ҽ����������
                {
                    guc_com_rx_point = 0;   //������ֽ�����
                    bflg_com_rx_busy = 0;   //�����æ��־
                    bflg_com_rx_ok = 1;     //�ý��ճɹ���־
                }
                else if ((guc_com_rx_buffer[1] == 0x10) && (guc_com_rx_point >= 8))  //����Ƕ���趨����ҽ��յ��ֽ�������֡
                {
                    guc_com_rx_point = 0;    
                    bflg_com_rx_busy = 0;    
                    bflg_com_rx_ok = 1;      
                }
                else if(guc_com_rx_point > 9)
                {
                    guc_com_rx_point = 0;    
                    bflg_com_rx_busy = 0;   
                    bflg_com_rx_ok = 1;    //ͬ������Ĵ���
                }                
            }
            else if(guc_com_rx_buffer[0] == 0x02)   //������չ��
            {
                if ((guc_com_rx_buffer[1] == 0x03) && (guc_com_rx_point >= 11))       //����ǲ�ѯ����ҽ����������
                {
                    guc_com_rx_point = 0;   //������ֽ�����
                    bflg_com_rx_busy = 0;   //�����æ��־
                    bflg_com_rx_ok = 1;     //�ý��ճɹ���־
                }
                else if ((guc_com_rx_buffer[1] == 0x10) && (guc_com_rx_point >= 8))  //����Ƕ���趨����ҽ��յ��ֽ�������֡
                {
                    guc_com_rx_point = 0;    
                    bflg_com_rx_busy = 0;    
                    bflg_com_rx_ok = 1;      
                }
                else if(guc_com_rx_point > 11)
                {
                    guc_com_rx_point = 0;    
                    bflg_com_rx_busy = 0;   
                    bflg_com_rx_ok = 1;       //ͬ������Ĵ���
                }                 
            }
        }
    }
    else  //�����Һ����ͨѶ
    {
        if ((bflg_com_rx_busy == 0) && ((luc_com_rx_byte == 0x01)||(luc_com_rx_byte == 0x02)))     //����޽���æ���Ҵӵ�ַ��ȷ
        {
            bflg_com_rx_busy = 1;
            
            guc_com_rx_buffer[0] = luc_com_rx_byte;
            guc_com_rx_point = 1;
        }
        else if (bflg_com_rx_busy == 1)      //�������æ
        {
            guc_com_rx_buffer[guc_com_rx_point++] = luc_com_rx_byte;
            
            if(guc_com_rx_buffer[0] == 0x01)          //Һ����ַ
            {
                if (guc_com_rx_buffer[1] == 0x10)     //����Ƕ���趨����
                {
                    if (guc_com_rx_point >= 8)   //���յ��ֽ�������֡
                    {
                        guc_com_rx_point = 0;    //������ֽ�����
                        bflg_com_rx_busy = 0;    //�����æ��־
                        bflg_com_rx_ok = 1;      //�ý��ճɹ���־
                    }
                }
                else if (guc_com_rx_buffer[1] == 0x03)     //����Ƕ���Ϣ
                {
                    if (bflg_read_user_admin == 0)    //����Ƕ��û���Ϣ
                    {
                        if (guc_com_rx_point >= ((READ_USER_CNT * 2) + 5))      //65
                        {
                            guc_com_rx_point = 0;     //������ֽ�����
                            bflg_com_rx_busy = 0;     //�����æ��־
                            bflg_com_rx_ok = 1;       //�ý��ճɹ���־
                        }
                    }
                    else     //����Ƕ�����Ա��Ϣ
                    {
                        if (guc_com_rx_point >= ((READ_ADMIN_CNT * 2) + 5))     //65
                        {
                            guc_com_rx_point = 0;     //������ֽ�����
                            bflg_com_rx_busy = 0;     //�����æ��־
                            bflg_com_rx_ok = 1;       //�ý��ճɹ���־
                        }
                    }
                }
                else if(guc_com_rx_point > 65)
                {
                    guc_com_rx_point = 0;    
                    bflg_com_rx_busy = 0;   
                    bflg_com_rx_ok = 1;    //ͬ������Ĵ���
                }
            }
            else if(guc_com_rx_buffer[0] == 0x02)     //��չ���ַ
            {
                if ((guc_com_rx_buffer[1] == 0x03) && (guc_com_rx_point >= 11))       //����ǲ�ѯ����ҽ����������
                {
                    guc_com_rx_point = 0;   //������ֽ�����
                    bflg_com_rx_busy = 0;   //�����æ��־
                    bflg_com_rx_ok = 1;     //�ý��ճɹ���־
                }
                else if ((guc_com_rx_buffer[1] == 0x10) && (guc_com_rx_point >= 8))  //����Ƕ���趨����ҽ��յ��ֽ�������֡
                {
                    guc_com_rx_point = 0;    
                    bflg_com_rx_busy = 0;    
                    bflg_com_rx_ok = 1;      
                }
                else if(guc_com_rx_point > 11)
                {
                    guc_com_rx_point = 0;    
                    bflg_com_rx_busy = 0;   
                    bflg_com_rx_ok = 1;       //ͬ������Ĵ���
                }                 
            }  
        }
    }
}

/*********************************************************************************************************************************************
��������:�����ӻ��ĵ�ַ���л�(��δ�ɹ����յ�����һ���ӻ�����ʱ���Զ��л�����һ���ӻ�)
         1��û�н��յ����ݣ���ʱ���ַ(��δ����չ��)
         2���յ���ַ����(��һ���ֽ�)����ʱ���ַ��
         3���յ�����CRC���󣬳�ʱ���ַ��
         
         
����λ�ã�1ms��ʱ��--------------------------------------------------ok
**********************************************************************************************************************************************/
void com_485_bus_switch(void)
{ 
    if(bflg_com_type_ok == 1)                        //δ�ж���LED����Һ���������к���չ��ĵ�ַ�л�
    {
        if(bflg_bus_rx_busying == 1)                 //�����ڽ���״̬
        {
            if(bflg_com_type == 0)                   //��LED������
            {
                if(guc_com_bus_add == LED_DIS_ADD)   
                {
                    guc_dis_bus_delaytime++;
                    if(guc_dis_bus_delaytime >= 50)    //�����led��9������
                    {
                        guc_dis_bus_delaytime = 0;
                        
                        R_UART0_Stop();
                        bflg_com_rx_ok = 0;            //��ʹ������������Ҳ����
                        
                        guc_led_comm_cnt = 0;
                        
                        guc_com_bus_add = EXPAND_ADD;
                        bflg_read_write_data = 1;      //------------ÿ�λ���ַ��ͷ��ʼ
                        bflg_bus_rx_busying = 0;

                        bflg_com_tx_delaytime = 1;     //��ͣƵ��   
                        gss_com_tx_delaytimer = 5; 
                    }
                }
                else 
                {
                    guc_dis_bus_delaytime = 0;
                }
                //---------------------------------------------------------------------
                if(guc_com_bus_add == EXPAND_ADD)         //��չ��ռ������ 
                {
                    guc_expand_bus_delaytime++;
                    if(guc_expand_bus_delaytime >= 50)   //�������չ��11������
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
            else                                     //��Һ��������
            {
                if(guc_com_bus_add == LIQUID_DIS_ADD)   
                {
                    guc_dis_bus_delaytime++;
                    if(guc_dis_bus_delaytime >= 500)  //������Һ����65������
                    {
                        guc_dis_bus_delaytime = 0;
                        
                        R_UART0_Stop();
                        bflg_com_rx_ok = 0;            //��ʹ������������Ҳ����
                        
                        guc_com_bus_add = EXPAND_ADD;
                        bflg_read_write_data = 1;      //------------ÿ�λ���ַ��ͷ��ʼ
                        bflg_read_user_admin = 0;
                        bflg_bus_rx_busying = 0;
                        
                        bflg_com_tx_delaytime = 1;     //��ͣƵ��   
                        gss_com_tx_delaytimer = 5;   
                    }
                }
                else 
                {
                    guc_dis_bus_delaytime = 0;
                }
                //---------------------------------------------------------------
                if(guc_com_bus_add == EXPAND_ADD)        //��չ��ռ������ 
                {
                    guc_expand_bus_delaytime++;
                    if(guc_expand_bus_delaytime >= 100)   //�������չ��11������
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
unsigned int CRC16(unsigned char *puchmsg, unsigned int usDataLen)    //����CRCУ��ĳ���
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
��������: �ж��Ƿ��б����¼����ͣ�����������ģ�ͨ���ֶ�������֪���Ƿ���

����λ��: bflg_com_allow_tx == 1 �� ----------------------------ok
**********************************************************************************************************************************************/
void com_tx_usb_rank(void)
{   
    //---------------------------------------------------------------------------���±��� 1
    if(bflg_alarm_ALH != bflg_usb_ALH_temp)      //���±���     
    {  
        bflg_usb_ALH_temp = bflg_alarm_ALH;

        if(bflg_alarm_ALH == 1)           //Ҫ��ֻ�ڱ���ʱ����
        {
            bflg_sent_usb_ALH = 1;        //���ͳ�ʼ�����¼�
        }
    }
    //---------------------------------------------------------------------------���±��� 2
    else if(bflg_alarm_ALL != bflg_usb_ALL_temp)  //���±���
    {
        bflg_usb_ALL_temp  = bflg_alarm_ALL;

        if(bflg_alarm_ALL == 1)
        {
            bflg_sent_usb_ALL = 1;
        }
    }
    //---------------------------------------------------------------------------���³��걨�� 3
    else if(bflg_alarm_THW_high != bflg_usb_THW_temp) //���³��걨��
    {
        bflg_usb_THW_temp  = bflg_alarm_THW_high;

        if(bflg_alarm_THW_high == 1)
        {
            bflg_sent_usb_THW = 1;
        }
    }
    //---------------------------------------------------------------------------������ 8
    else if(bflg_prot2_door1_pin != bflg_usb_DOR_temp)  //������״̬
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
    //---------------------------------------------------------------------------�ϵ�ϵ� 9
    else if(bflg_alarm_power_off != bflg_usb_POW_temp)  //��/�ϵ�״̬
    {
        bflg_usb_POW_temp = bflg_alarm_power_off;
        bflg_sent_usb_POW = 1;

        if(bflg_alarm_power_off == 0)
        {
            bflg_usb_POW_on = 1;     //�ϵ�
        }
        else
        {
            bflg_usb_POW_on = 0;
        }
    }
    //---------------------------------------------------------------------------��ر��� 10
    else if(bflg_alarm_bat_discnnt != bflg_usb_BAT_discnnt_temp) 
    {
        bflg_usb_BAT_discnnt_temp = bflg_alarm_bat_discnnt;//���δ����
        if(bflg_alarm_bat_discnnt == 1)
        {
            bflg_sent_usb_BAT = 1;
        }
    }
    else if(bflg_alarm_bat_fed != bflg_usb_BAT_fed_temp)
    {
        bflg_usb_BAT_fed_temp = bflg_alarm_bat_fed;       //��ز巴
        if(bflg_alarm_bat_fed == 1)
        {
            bflg_sent_usb_BAT = 1;
        }
    }
    else if(bflg_alarm_bat_low != bflg_usb_BAT_low_temp)
    {
        bflg_usb_BAT_low_temp = bflg_alarm_bat_low;       //��ص�����
        if(bflg_alarm_bat_low == 1)
        {
            bflg_sent_usb_BAT = 1;
        }
    }
    //---------------------------------------------------------------------------���������� 12
    else if(bflg_THW_fault != bflg_usb_THW_fault_temp)//���´���������
    {
        bflg_usb_THW_fault_temp = bflg_THW_fault;
        if(bflg_THW_fault == 1)
        {
            bflg_sent_usb_FAT_THW = 1;
            //bflg_sent_usb_FAT = 1;
        }
    }
    /*else if(bflg_TC_2_fault != bflg_usb_TC_2_fault_temp) //�Ƚ�����
    {
        bflg_usb_TC_2_fault_temp = bflg_TC_2_fault;
        if(bflg_TC_2_fault == 1)
        {
             bflg_sent_usb_FAT_TC = 1;   //bflg_sent_usb_FAT_TE = 1; //˫ϵͳ���Ƚ�����Ϊ����
            //bflg_sent_usb_FAT = 1;
        }
    }*/
    else if(bflg_TC_fault != bflg_usb_TC_fault_temp) //����������������
    {
        bflg_usb_TC_fault_temp = bflg_TC_fault;
        if(bflg_TC_fault == 1)
        {
            bflg_sent_usb_FAT_TC = 1;
            //bflg_sent_usb_FAT = 1;
        }
    }
    else if(bflg_TA_fault != bflg_usb_TA_fault_temp) //������������
    {
        bflg_usb_TA_fault_temp = bflg_TA_fault;
        if(bflg_TA_fault == 1)
        {
            bflg_sent_usb_FAT_TA = 1;
            //bflg_sent_usb_FAT = 1;
        }
    }
    //---------------------------------------------------------------------------�������౨��16
    else if(bflg_alarm_cond_dirty != bflg_usb_TC_temp)    //��������       
    {
        bflg_usb_TC_temp = bflg_alarm_cond_dirty;
        if(bflg_alarm_cond_dirty == 1)
        {
            bflg_sent_usb_TC = 1;
        }
    }
    //---------------------------------------------------------------------------�ϵ籨��
    else if(bflg_alarm_power_off != bflg_usb_power_off_temp) //�ϵ籨��
    {
        bflg_usb_power_off_temp = bflg_alarm_power_off;
        if(bflg_alarm_power_off == 1)
        {
            bflg_sent_usb_POW_OFF = 1;
        }
    }
    //---------------------------------------------------------------------------���ű���
    else if(bflg_alarm_open_door != bflg_usb_open_door_temp)  //���ű���
    {
        bflg_usb_open_door_temp = bflg_alarm_open_door;
        if(bflg_alarm_open_door == 1)
        {
            bflg_sent_usb_DOOR_OPEN = 1;
        }
    }
    //---------------------------------------------------------------------------�������±���
    
    else if(bflg_alarm_high_AIR != bflg_usb_high_AIR_temp)  //�������±���
    {
        bflg_usb_high_AIR_temp = bflg_alarm_high_AIR;
        if(bflg_alarm_high_AIR ==1)
        {
            bflg_sent_usb_AIR_H = 1;
        }
    }
    //---------------------------------------------------------------------------�������±���
    else if(bflg_alarm_low_AIR != bflg_usb_low_AIR_temp)  //�������±���
    {
        bflg_usb_low_AIR_temp = bflg_alarm_low_AIR;
        if(bflg_alarm_low_AIR ==1)
        {
            bflg_sent_usb_AIR_L = 1;
        }
    }    
    //---------------------------------------------------------------------------��˪���������±���
    else if(bflg_alarm_higt_TE != bflg_usb_higt_TE_temp)  //��˪���������±���
    {
        bflg_usb_higt_TE_temp = bflg_alarm_higt_TE;
        if(bflg_alarm_higt_TE ==1)
        {
            bflg_sent_usb_TE_H = 1;
        }
    }    
    //---------------------------------------------------------------------------����ģ�����
    else if(bflg_sterilize_fault != bflg_usb_sterilize_fault_temp)  //����ģ�����
    {
        bflg_usb_sterilize_fault_temp = bflg_sterilize_fault;
        if(bflg_sterilize_fault ==1)
        {
            bflg_sent_usb_ozone_FAT = 1;
        }        
    }    
}
/**********************************************************************************************************************
��������:  ��ʼ�ϵ�ʱ��������ʱһ��ʱ���ٷ���usb�¼����ȴ�usb���ʱ��ͬ����ɣ�
           �����¼��Щ�¼���ʱ�� �����
           
����λ��:  1s��ʱ������ ------------------------------------------ok
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
