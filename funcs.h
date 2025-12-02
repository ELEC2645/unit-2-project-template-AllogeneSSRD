// ELEC2645 Unit 2 Project
// Auther: Xiangcheng Tao
// Filename: funcs.h
// Create date: 2025-11-19

#ifndef FUNCS_H
#define FUNCS_H

void input_float(double *value, const char * description);

void buck_ccm_duty_cycle(void);
void buck_ccm_inductor_Iripple(void);
void buck_ccm_capacitor_Vripple(void);
void buck_dcm_duty_cycle(void);
void buck_dcm_boundary_constant_vi(void);
void buck_dcm_boundary_constant_vo(void);
void boost_ccm_duty_cycle(void);
void boost_ccm_inductor_Iripple(void);
void boost_ccm_capacitor_Vripple(void);
void boost_dcm_duty_cycle(void);
void boost_dcm_boundary_constant_vi(void);
void boost_dcm_boundary_constant_vo(void);

#endif