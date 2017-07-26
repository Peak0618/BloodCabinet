/**************************************************************************************************** 
overview:20170315
         ���ļ��Ƕ�ѹ�����Ŀ��ƣ�1��ѹ������ͣ
                                 2����Ƶ�Ŀ���
                                 3����Ƶ��ת�ٿ���   
*****************************************************************************************************/
//------------------------------------------------------------------------------------------
//�������ļ�
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
//��������
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
//�궨��
#define   COMPRESSOR_RL_OUT_PIN     P11.1      //ѹ����1�̵�������� ԭ�����ſ��ƿ�    
      

//------------------------------------------------------------------------------------------
//��־����
flag_type bflg_compressor;


//------------------------------------------------------------------------------------------
//�������� 
int16_t  gss_compressor_freq;                    //ѹ��ת��
int16_t  gss_compressor_start_delaytimer;        //��Ƶ����ʱ��ʱ
int16_t  gss_compressor_update_delaytimer;
int16_t  gss_compressor_delt_freq;
uint16_t gus_compressor_freq_cnt;               //��ʱ���ļ�������.����Ƶ��f=1/T
uint8_t  guc_compressor_first_power_delaytime;  //��ʼ�ϵ���ʱ
uint16_t guc_compressor_fault_runing_time;      //������������ͣѭ��ʱ��
uint8_t  guc_compressor_stop_delaytime;         //ѹ��ͣ��ʱ��  �ŵ��ֶ�ʱ
uint8_t  guc_compressor_force_stop_delaytime;   //ѹ��ǿ��ͣ��ʱ�� �ŵ��ֶ�ʱ
uint8_t  guc_compressor_continue_delaytime;     //ѹ����������ʱ���ʱ

int16_t  gss_compressor_TS;                     //�����¶��趨ֵ
int16_t  gss_factory_parameter_r0;              //���������еĿ����²�

int16_t   gss_para[28];



/**********************************************************************************************************************************************
��������: �����ϵ�ѹ��������ʱ(Ĭ��1min)��������

����λ��: 1min��ʱ��----------------------------------ok
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
��������: ѹ����ͣ����

����λ��: 1s��ʱ��----------------------------------ok
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
        if(bflg_compressor_runing == 1)               //����
        {
            if(bflg_defrosting_status == 1)           //ǿ�ƻ�˪״̬
            {
                bflg_compressor_runing = 0;           
                bflg_compressor_stop_delaytime = 1;
                guc_compressor_stop_delaytime = 0;
            }
            else if(bflg_compressor_continue == 1)    //��������4h
            {
                bflg_compressor_continue = 0;
                bflg_compressor_runing = 0;
                bflg_compressor_force_stop_delaytime = 1;
                guc_compressor_force_stop_delaytime = 0;
            }
            else if(bflg_up_down_stop == 1)           //��Ϊ������ͣ��
            {
                bflg_up_down_stop = 0;
                
                bflg_compressor_runing = 0;           
                bflg_compressor_stop_delaytime = 1;
                guc_compressor_stop_delaytime = 0;
            }
            else if(gss_TA <= (gss_user_set_parm_value[DISP_USER_SET_TS] - gss_factory_parm_value[DISP_FAC_r0])) //������ͣ��
            {
                bflg_compressor_runing = 0;
                bflg_compressor_stop_delaytime = 1;
                guc_compressor_stop_delaytime = 0;
            }
        }
        else
        {
            if((bflg_defrosting_status == 0) && (gss_TU >=20) && (gss_TD >= 20))  //δ��˪״̬�Ҽ�������ͼ��
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
�������ܣ�ѹ����������4hǿ��ͣ��
         

����λ��:  1min��ʱ��---------------------------------ok
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
��������: ��Ƶѹ��ʵ�ʶ˿����
         

����λ��:  ��ѭ��---------------------------------ok
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
��������: ѹ�����ͣ��ʱ��
         

����λ��:  1min��ʱ��---------------------------------ok
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
��������: ѹ��ǿ��ͣ��ʱ��
         

����λ��:  1min��ʱ��---------------------------------ok
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
��������: ��Ƶѹ��ת�ٿ��ơ�����ѹ����������ѹ������������ֹͣ�����붨Ƶ����ʽһ����
         

����λ��:  1s��ʱ��--------------------------------ok
***********************************************************************************************************************************************/
/*
void compressor_freq_ctrl_deal(void)            
{
    int32_t lss_tmp;
    
    //----------------------------------                            //����ѹ���Ŀ���
    if (bflg_compressor_runing == 0)                                //����ѹ��δ��������   
    {
        if (gss_compressor_freq != 0)
        {
            gss_compressor_freq = 0;
        }
    }
    else                                                                //����ѹ����������
    {
        if (gss_compressor_freq == 0)                                    //����ѹ����δ����
        {
            gss_compressor_freq = gss_para[COMP21_START_FREQ];           //��ʼת��
            
            bflg_compressor_start_delaytime = 1;                          //������ʱ��2000rpm����1����
            gss_compressor_start_delaytimer = 0;
        }
        else                                                         //����ѹ���Ѿ�����
        {
            if (bflg_compressor_start_delaytime == 0)                //������ʱ��2000rpm����1���ӽ���
            {
                if((gss_TA - gss_user_set_parm_value[DISP_USER_SET_TS]) > 50)                  //s1  5��
                {

                    gss_compressor_freq = gss_para[COMP21_FREQ_MAX];     //peak ���ת��
                    bflg_compressor_full_speed = 1;                                //ȫ�����б�־
                }
                else 
                {
                    if(bflg_compressor_full_speed == 1)                        //��ȫ������
                    {   
                        if((gss_TA - gss_user_set_parm_value[DISP_USER_SET_TS] - 5 ) <= 0)      // s2 0.5�� �ﵽ�˳�ȫ�ٵ�����
                        {
                            bflg_compressor_full_speed = 0;
                            gss_compressor_freq = gss_para[COMP21_FREQ_SET];      //���趨��ת��ת������
                        }                   
                    }
                    else
                    {
                        if (bflg_compressor_update_delaytime == 0)           //ÿ80s �����жϸ���һ��ת��
                        {
                            bflg_compressor_update_delaytime = 1; 
                            gss_compressor_update_delaytimer = 0;
                            
                            lss_tmp = gss_TA;                                //���㹫ʽ
                            lss_tmp -= gss_user_set_parm_value[DISP_USER_SET_TS];
                            lss_tmp += (gss_para[COMP21_FREQ_DELT_TEMP] * 10);//��ʽ���� ��-------?
                            lss_tmp *= gss_para[COMP21_FREQ_DELT_K];
                            lss_tmp /= 10;                                   //��Ϊ��PT100��أ�֮ǰ���Ŵ�10�������Դ˴�/10,֮ǰ����ѹ���Ĳ��ã�����Ϊ�õ��Ƚ��������¶�
                            
                            if(lss_tmp < gss_para[COMP21_FREQ_DELT_MIN])     //��Χ -20 ~ 20
                            {
                                lss_tmp = gss_para[COMP21_FREQ_DELT_MIN];
                            }
                            
                            if (lss_tmp > gss_para[COMP21_FREQ_DELT_MAX])     
                            {
                                lss_tmp = gss_para[COMP21_FREQ_DELT_MAX];
                            }
                            
                            gss_compressor_delt_freq = (int16_t) (lss_tmp);       
                            gss_compressor_freq += gss_compressor_delt_freq;           //������Ľ���
                            
                            if (gss_compressor_freq >= gss_para[COMP21_FREQ_MAX])      //ת�ٷ�Χ 2000 ~ 4500
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
    	  gus_compressor_freq_cnt = (60000 - 1);                               //cntԽ������Խ��Ƶ��ԽС��
    }
    else
    {
    	  gus_compressor_freq_cnt = (uint16_t) (60000000 / gss_compressor_freq);   // 3��Ϊ---> 6
    }
}
*/

/**********************************************************************************************************************************************
��������: ��Ƶѹ��ת�ٿ���-----�µı�Ƶ��ʽ
         

����λ��:  1s��ʱ��--------------------------------ok
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
        if (gss_compressor_freq == 0)                                    //����ѹ����δ����
        {
            gss_compressor_freq = gss_para[COMP21_START_FREQ];           //��ʼת��
            
            bflg_compressor_start_delaytime = 1;                         //������ʱ��1200rpm����1����
            gss_compressor_start_delaytimer = 0;
        }
        else
        {
            if (bflg_compressor_start_delaytime == 0)                    //��������2000rpm����1min
            {
                if(bflg_compressor_update_delaytime == 0)
                {
                    bflg_compressor_update_delaytime = 1; 
                    gss_compressor_update_delaytimer = 0;
                    
                    if((bflg_THW_fault == 1) || (bflg_TA_fault == 1))     //����/pt100���϶����⵵
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
    //16��
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
    }        //�õĶ�ʱ���ǣ�1_3
    if (gss_compressor_freq == 0)               //6000��Ӧ��С���Ƶ��Ϊ33hz,��ײ�����15ms�������
    {
        gus_compressor_freq_cnt = (40000 - 1);  //60000 --->40000                             //cntԽ������Խ��Ƶ��ԽС��
    }
    else
    {
        gus_compressor_freq_cnt = (uint16_t) (30000000 / gss_compressor_freq);   // 3��Ϊ---> 6-----��3��
    }                    
}




/**********************************************************************************************************************************************
��������: ��Ƶѹ������ʼת������60s��ʱ


����λ��:  1s��ʱ------------------------------ok
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
��������: ��Ƶ����ʱʹ�õ�һЩ����


����λ��:  ��ʼ����------------------------------ok
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

   /* ��Щֵ��E2ʱ                                                                      //Ĭ��ֵ
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
�������ܣ�ѹ����ת��ÿ���80s����һ�θ���

����λ�ã�1s��ʱ��--------------------------------------ok
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
�������ܣ��������޹���ʱ����һ·<2�棬ѹ��ͣ��

����λ�ã���--------------------------------------ok
*********************************************************************************************************************************************************/
void compressor_up_down_stop(void)
{
    if(bflg_compressor_runing == 1)
    {
        if(bflg_up_down_stop == 0)
        {
            if((bflg_TU_fault == 0)&&(bflg_TD_fault == 0))       //���´������޹���ʱ
            {
                if((gss_TU < 20) || (gss_TD < 20))               //����һ·<2��
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
