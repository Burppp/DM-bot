/**
 ******************************************************************************
 * @file    kalman filter.h
 * @author  Wang Hongxi
 * @version V1.2.2
 * @date    2022/1/8
 * @brief
 ******************************************************************************
 * @attention
 *
 ******************************************************************************
 */
#ifndef __KALMAN_FILTER_H
#define __KALMAN_FILTER_H


#include "arm_math.h"

#include "math.h"
#include "stdint.h"
#include "stdlib.h"

#ifndef user_malloc
#ifdef _CMSIS_OS_H
#define user_malloc pvPortMalloc
#else
#define user_malloc malloc
#endif
#endif

#define mat arm_matrix_instance_f32
#define Matrix_Init arm_mat_init_f32
#define Matrix_Add arm_mat_add_f32
#define Matrix_Subtract arm_mat_sub_f32
#define Matrix_Multiply arm_mat_mult_f32
#define Matrix_Transpose arm_mat_trans_f32
#define Matrix_Inverse arm_mat_inverse_f32

typedef struct kf_t
{
    float *FilteredValue;
    float *MeasuredVector;
    float *ControlVector;

    uint8_t xhatSize;
    uint8_t uSize;
    uint8_t zSize;

    uint8_t UseAutoAdjustment;
    uint8_t MeasurementValidNum;

    uint8_t *MeasurementMap;      // ???????????? how measurement relates to the state
    float *MeasurementDegree;     // ????????H???????? elements of each measurement in H
    float *MatR_DiagonalElements; // ?????? variance for each measurement
    float *StateMinVariance;      // ??��???? ????????????? suppress filter excessive convergence
    uint8_t *temp;

    // ?????????��?????,??????��?????��????????????KF??????????��????????
    uint8_t SkipEq1, SkipEq2, SkipEq3, SkipEq4, SkipEq5;

    // definiion of struct mat: rows & cols & pointer to vars
    mat xhat;      // x(k|k)
    mat xhatminus; // x(k|k-1)
    mat u;         // control vector u
    mat z;         // measurement vector z
    mat P;         // covariance matrix P(k|k)
    mat Pminus;    // covariance matrix P(k|k-1)
    mat F, FT,temp_F;     // state transition matrix F FT
    mat B;         // control matrix B
    mat H, HT;     // measurement matrix H
    mat Q;         // process noise covariance matrix Q
    mat R;         // measurement noise covariance matrix R
    mat K;         // kalman gain  K
    mat S, temp_matrix, temp_matrix1, temp_vector, temp_vector1;

    int8_t MatStatus;

    // ??????��??,?????�I????????KF?????
    void (*User_Func0_f)(struct kf_t *kf);
    void (*User_Func1_f)(struct kf_t *kf);
    void (*User_Func2_f)(struct kf_t *kf);
    void (*User_Func3_f)(struct kf_t *kf);
    void (*User_Func4_f)(struct kf_t *kf);
    void (*User_Func5_f)(struct kf_t *kf);
    void (*User_Func6_f)(struct kf_t *kf);
    
    // ????��??????
    float *xhat_data, *xhatminus_data;
    float *u_data;
    float *z_data;
    float *P_data, *Pminus_data;
    float *F_data, *FT_data,*temp_F_data;
    float *B_data;
    float *H_data, *HT_data;
    float *Q_data;
    float *R_data;
    float *K_data;
    float *S_data, *temp_matrix_data, *temp_matrix_data1, *temp_vector_data, *temp_vector_data1;
} KalmanFilter_t;

extern uint16_t sizeof_float, sizeof_double;

void Kalman_Filter_Init(KalmanFilter_t *kf, uint8_t xhatSize, uint8_t uSize, uint8_t zSize);
void Kalman_Filter_Measure(KalmanFilter_t *kf);
void Kalman_Filter_xhatMinusUpdate(KalmanFilter_t *kf);
void Kalman_Filter_PminusUpdate(KalmanFilter_t *kf);
void Kalman_Filter_SetK(KalmanFilter_t *kf);
void Kalman_Filter_xhatUpdate(KalmanFilter_t *kf);
void Kalman_Filter_P_Update(KalmanFilter_t *kf);
float *Kalman_Filter_Update(KalmanFilter_t *kf);

#endif //__KALMAN_FILTER_H
