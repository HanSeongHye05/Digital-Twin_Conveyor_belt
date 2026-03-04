#include "con_motor.h"
#include "gpio.h"
#include "tim.h"

//[외부 타이머 핸들 선언]
//CubeMX(main.c)에서 설정된 TIM3 핸들을 참조하여 PWM 제어에 사용합니다.
extern TIM_HandleTypeDef htim3;

/* =================================================================
 * 핀 및 파라미터 정의 (L298N 또는 모터 드라이버 연결 기준)
 * ================================================================= */
#define IN3_GPIO_Port   GPIOF
#define IN3_Pin         GPIO_PIN_1   // 모터 방향 제어 핀 A
#define IN4_GPIO_Port   GPIOF
#define IN4_Pin         GPIO_PIN_2   // 모터 방향 제어 핀 B

/* * PWM_MAX_DUTY: 타이머 설정값(ARR)과 일치해야 하며, 속도의 100%를 의미
 * 현재 설정값: 500 (이 값을 넘기면 비교값이 의미가 없어지므로 제한이 필요함)
 */
#define PWM_MAX_DUTY   500


/* =================================================================
 * 모터 제어 함수 구현
 * ================================================================= */

 //@brief 컨베이어 모터 초기화 및 초기 방향 설정
 //- PWM 신호 출력을 시작하고 모터의 회전 방향을 정방향으로 고정

void Conveyor_Init(void)
{
    // PWM 시작 (ENB 핀 연결: PA6 / TIM3_CH1)
    HAL_TIM_PWM_Start(&htim3, TIM_CHANNEL_1);

    //. 방향 설정 (정방향: IN3=High, IN4=Low)
    // 역방향이 필요할 경우 두 핀의 상태를 반대로 설정. */
    HAL_GPIO_WritePin(IN3_GPIO_Port, IN3_Pin, GPIO_PIN_SET);
    HAL_GPIO_WritePin(IN4_GPIO_Port, IN4_Pin, GPIO_PIN_RESET);
}


 // @brief 컨베이어 속도(Duty Cycle) 설정
 // @param duty: 0 ~ 500 사이의 값 (0: 정지, 500: 최대 속도)

void Conveyor_SetDuty(uint16_t duty)
{
    // 입력값이 최대 범위를 초과하지 않도록 방어 코드(Hard Limiter) 적용
    if (duty > PWM_MAX_DUTY)
    {
        duty = PWM_MAX_DUTY;
    }

    // 타이머의 Compare 레지스터를 직접 조작하여 PWM 출력값 변경
    __HAL_TIM_SET_COMPARE(&htim3, TIM_CHANNEL_1, duty);
}


 // @brief 저속 모드로 모터 가동
 // 시스템 시작 시 또는 특정 조건에서 안전한 속도(약 40%)로 구동할 때 사용.

void Conveyor_Start_Slow(void)
{
    Conveyor_SetDuty(200);  // 500 대비 약 40%의 속도
}

 // @brief 모터 즉시 정지
 // PWM 출력을 0으로 설정하여 모터의 동력을 차단.

void Conveyor_Stop(void)
{
    __HAL_TIM_SET_COMPARE(&htim3, TIM_CHANNEL_1, 0);
}
