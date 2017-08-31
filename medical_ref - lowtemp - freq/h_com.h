#ifndef _COM_H_
#define _COM_H_
//------------------------------------------------------------------------------
//�ⲿ��������
extern void com_init(void);   //ͨѶ��ʼ�������ڳ�ʼ�������е���

extern void com_rx_init(void);     //ͨѶ���ճ�ʼ����������ѭ�������е���

extern void com_rx_data_deal(void);     //ͨѶ�������ݴ����������ѭ�������е���

extern void com_tx_init(void);     //ͨѶ�������ݴ����������ѭ�������е���

extern void com_rx_delaytime(void);     //ͨѶ������ʱ������1ms��ʱ�����е���

extern void com_tx_delaytime(void);     //ͨѶ������ʱ������1ms��ʱ�����е���

extern void com_rx_end_delaytime(void); //ͨѶ���ս�����ʱ������1ms��ʱ�����е���

extern void com_fault_delaytime(void);  //ͨѶ������ʱ������1s��ʱ�����е���

extern void com_tx_int(void); //ͨѶ�����жϳ���

extern void com_rx_int(uint8_t rx_data);//ͨѶ�����жϳ���

extern void com_485_bus_switch(void);

void com_tx_usb_rank(void);

void com_usb_delay_time(void);  


//------------------------------------------------------------------------------
//�ⲿ��������//
extern flag_type flg_com;

          #define   bflg_com_tx_delaytime    flg_com.bits.bit0   //ͨѶ������ʱ��־
          #define   bflg_com_rx_delaytime    flg_com.bits.bit1   //ͨѶ������ʱ��־
          #define   bflg_com_allow_tx        flg_com.bits.bit2   //ͨѶ�����ͱ�־
          #define   bflg_com_allow_rx        flg_com.bits.bit3   //ͨѶ������ձ�־
          #define   bflg_com_tx_busy         flg_com.bits.bit4   //ͨѶ����æ��־
          #define   bflg_com_rx_busy         flg_com.bits.bit5   //ͨѶ����æ��־
          #define   bflg_com_tx_ok           flg_com.bits.bit6   //ͨѶ���ͳɹ���־
          #define   bflg_com_rx_ok           flg_com.bits.bit7   //ͨѶ���ճɹ���־
          #define   bflg_com_rx_end          flg_com.bits.bit8   //ͨѶ���ս�����־
          
          #define   bflg_com_fault           flg_com.bits.bit9   //ͨѶ���ϱ�־
          
          #define   bflg_com_type            flg_com.bits.bit10  //ͨѶ���ͱ�־     //peak  0:LED  1:Һ��
          #define   bflg_com_type_ok         flg_com.bits.bit11  //ͨѶ�����ж�ok��־

          //peak add
          #define   bflg_lock_delaytimer     flg_com.bits.bit12  //��������ʱ��־
          #define   bflg_bus_rx_busying      flg_com.bits.bit13  //�������ڴ��ڽ���״̬�����ڳ�ʱ����
//-----------------------------------------------------------------------------------------------------------------------------------------
extern flag_type flg_read_write;
          #define   bflg_read_write_data     flg_read_write.bits.bit0      //��д���ݱ�־
          #define   bflg_read_user_admin     flg_read_write.bits.bit1      //���û�����Ա��־
          
//-----------------------------------------------------------------------------------------------------------------------------------------
extern flag_type flg_sent_usb_control;      //����usb�Ŀ�����
          #define   bflg_sent_usb_CL2        flg_sent_usb_control.bits.bit0  //���͸����usb����  ��-->��-->usb
          #define   bflg_sent_usb_T1         flg_sent_usb_control.bits.bit1  //���͸�����usb��¼Ƶ��
          #define   bflg_sent_usb_P6         flg_sent_usb_control.bits.bit2  //���͸�����usb����ģʽ
		  #define   bflg_sent_usb_UL         flg_sent_usb_control.bits.bit3  //���͸�����usb�������� 
          
extern flag_type flg_sent_usb_event;        //����usb���¼���
         #define   bflg_sent_usb_ALH         flg_sent_usb_event.bits.bit0    //���±���
         #define   bflg_sent_usb_ALL         flg_sent_usb_event.bits.bit1    //���±���
         #define   bflg_sent_usb_THW         flg_sent_usb_event.bits.bit2    //���³��걨��
         #define   bflg_sent_usb_ALHS        flg_sent_usb_event.bits.bit3    //���±����趨
         #define   bflg_sent_usb_ALLS        flg_sent_usb_event.bits.bit4    //���±����趨
         #define   bflg_sent_usb_TS          flg_sent_usb_event.bits.bit5    //�����¶��趨
         #define   bflg_sent_usb_DOR         flg_sent_usb_event.bits.bit6    //����/����
         #define   bflg_sent_usb_POW         flg_sent_usb_event.bits.bit7    //�ϵ�/�ϵ� ״̬
         #define   bflg_sent_usb_BAT         flg_sent_usb_event.bits.bit8    //��ر���
         //....
         #define   bflg_sent_usb_FAT_TA      flg_sent_usb_event.bits.bit9    //������������
         #define   bflg_sent_usb_FAT_TE      flg_sent_usb_event.bits.bit10   //��˪(�Ƚ���)..
         #define   bflg_sent_usb_FAT_THW     flg_sent_usb_event.bits.bit11   //����..
         #define   bflg_sent_usb_FAT_TC      flg_sent_usb_event.bits.bit12   //����..
         #define   bflg_sent_usb_FAT_TU      flg_sent_usb_event.bits.bit13   //����..
         #define   bflg_sent_usb_FAT_TD      flg_sent_usb_event.bits.bit14   //����..
         #define   bflg_sent_usb_IC          flg_sent_usb_event.bits.bit15   //IC��ע��
                  
extern flag_type flg_sent_usb_event_1;        //����usb���¼���
         #define   bflg_sent_usb_TC          flg_sent_usb_event_1.bits.bit0   //�������౨��
         #define   bflg_sent_usb_POW_OFF     flg_sent_usb_event_1.bits.bit1   //�ϵ籨��
         #define   bflg_sent_usb_DOOR_OPEN   flg_sent_usb_event_1.bits.bit2   //���ű���
         #define   bflg_sent_usb_AIR_H       flg_sent_usb_event_1.bits.bit3   //�������±���
         #define   bflg_sent_usb_AIR_L       flg_sent_usb_event_1.bits.bit4   //�������±���
         #define   bflg_sent_usb_TE_H        flg_sent_usb_event_1.bits.bit5   //��˪���������±���
         #define   bflg_sent_usb_ozone_FAT   flg_sent_usb_event_1.bits.bit6   //����ģ�����
          
extern flag_type flg_usb_event_temp;
        #define   bflg_usb_ALH_temp          flg_usb_event_temp.bits.bit0    //���±���
        #define   bflg_usb_ALL_temp          flg_usb_event_temp.bits.bit1    //���±���
        #define   bflg_usb_THW_temp          flg_usb_event_temp.bits.bit2    //���³��걨��
        #define   bflg_usb_ALHS_temp         flg_usb_event_temp.bits.bit3    //���±����趨
        #define   bflg_usb_ALLS_temp         flg_usb_event_temp.bits.bit4    //���±����趨
        #define   bflg_usb_TS_temp           flg_usb_event_temp.bits.bit5    //�����¶��趨
        #define   bflg_usb_DOR_temp          flg_usb_event_temp.bits.bit6    //����/����
        #define   bflg_usb_POW_temp          flg_usb_event_temp.bits.bit7    //�ϵ�/�ϵ� ״̬
        #define   bflg_usb_BAT_discnnt_temp  flg_usb_event_temp.bits.bit8    //���δ�ӱ���
        #define   bflg_usb_BAT_fed_temp      flg_usb_event_temp.bits.bit9    //��ز巴����
        #define   bflg_usb_BAT_low_temp      flg_usb_event_temp.bits.bit10   //��ص����ͱ���
        #define   bflg_usb_TA_fault_temp     flg_usb_event_temp.bits.bit11   //������������
        #define   bflg_usb_TE_fault_temp     flg_usb_event_temp.bits.bit12   //��˪(�Ƚ���)..
        #define   bflg_usb_THW_fault_temp    flg_usb_event_temp.bits.bit13   //����..
        #define   bflg_usb_TC_fault_temp     flg_usb_event_temp.bits.bit14   //����..
        #define   bflg_usb_TU_fault_temp     flg_usb_event_temp.bits.bit15   //����..
          
extern flag_type flg_usb_event_temp_1;
        #define   bflg_usb_TD_fault_temp         flg_usb_event_temp_1.bits.bit0   //����..
        #define   bflg_usb_IC_temp               flg_usb_event_temp_1.bits.bit1   //IC��ע��
        #define   bflg_usb_TC_temp               flg_usb_event_temp_1.bits.bit2   //�������౨��
        #define   bflg_usb_power_off_temp        flg_usb_event_temp_1.bits.bit3   //�ϵ籨��
        #define   bflg_usb_open_door_temp        flg_usb_event_temp_1.bits.bit4   //���ű���
        #define   bflg_usb_high_AIR_temp         flg_usb_event_temp_1.bits.bit5   //�������±���
        #define   bflg_usb_low_AIR_temp          flg_usb_event_temp_1.bits.bit6   //�������±���
        #define   bflg_usb_higt_TE_temp          flg_usb_event_temp_1.bits.bit7   //��˪���������±���
        #define   bflg_usb_sterilize_fault_temp  flg_usb_event_temp_1.bits.bit8   //����ģ�����

extern flag_type flg_usb_event_attribute;       
        #define   bflg_usb_DOR_open              flg_usb_event_attribute.bits.bit0    //���ǿ�or��
        #define   bflg_usb_POW_on                flg_usb_event_attribute.bits.bit1    //�ϵ�or�ϵ�
        #define   bflg_usb_delaytime             flg_usb_event_attribute.bits.bit2    //���͸�usb�Ĺ����¼���ʱ���ٷ���--�ں�������
                  

//------------------------------------------------------------------------------
//�ⲿ���� 
extern uint8_t  guc_IC_access_state;          // ������״̬
extern uint16_t guc_flash_add;                // ���յ�IC��ע�����õĵ�ַ
extern uint16_t guc_temp_flash_add;           // ���յ�IC��ע����ʱ����ĵ�ַ
extern uint8_t  guc_IC_erasure_tx_ok_count;   // ע��IC��ָ���bflg_com_tx_ok�����÷��Ͷ�μ���,��ֹ����
extern int16_t  gss_liquid_led_button;        // Һ�����Ŀ���ָ��
extern uint8_t  guc_com_bus_add;
extern uint8_t  gss_usb_delaytime;             //�����ϵ���ʱxs���ڷ���usb�¼�
extern uint8_t  guc_U_disk_state;              //��ʾ�巢�͹�����U��״̬
extern uint8_t  guc_usb_state;                 //��ʾ�巢�͹�����usb���״̬
extern int16_t  gss_liquid_fingerprint_unlocked;    //Һ������ָ�ƿ���

//------------------------------------------------------------------------------
#endif
