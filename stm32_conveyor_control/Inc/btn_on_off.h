#ifndef INC_BTN_ON_OFF_H_
#define INC_BTN_ON_OFF_H_

#include "main.h"

// 전원 상태를 관리하는 함수
int Check_Power_Button(void);      //void에서 int로 수정
uint8_t Get_Power_Status(void); // 현재 전원이 켜졌는지(1) 꺼졌는지(0) 확인

#endif
