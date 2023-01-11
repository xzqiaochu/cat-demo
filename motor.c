#include <stdint.h>
#include <stdio.h>
#include "mxc.h"
#include "motor.h"

#define OUT1_PORT MXC_GPIO0
#define OUT1_PIN MXC_GPIO_PIN_16

#define OUT2_PORT MXC_GPIO0
#define OUT2_PIN MXC_GPIO_PIN_17

#define IN_PORT MXC_GPIO0
#define IN_PIN MXC_GPIO_PIN_19

mxc_gpio_cfg_t out1;
mxc_gpio_cfg_t out2;
mxc_gpio_cfg_t in;

void gpio_isr(void *cbdata)
{
    motor_stop();
}

void motor_init() {
    
    out1.port = OUT1_PORT;
    out1.mask = OUT1_PIN;
    out1.pad = MXC_GPIO_PAD_NONE;
    out1.func = MXC_GPIO_FUNC_OUT;
    out1.vssel = MXC_GPIO_VSSEL_VDDIOH; // 3.3V
    MXC_GPIO_Config(&out1);

    out2.port = OUT2_PORT;
    out2.mask = OUT2_PIN;
    out2.pad = MXC_GPIO_PAD_NONE;
    out2.func = MXC_GPIO_FUNC_OUT;
    out2.vssel = MXC_GPIO_VSSEL_VDDIOH;
    MXC_GPIO_Config(&out2);
    
    in.port = IN_PORT;
    in.mask = IN_PIN;
    in.pad = MXC_GPIO_PAD_PULL_UP;
    in.func = MXC_GPIO_FUNC_IN;
    in.vssel = MXC_GPIO_VSSEL_VDDIOH;
    MXC_GPIO_Config(&in);
    MXC_GPIO_RegisterCallback(&in, gpio_isr, NULL);
    MXC_GPIO_IntConfig(&in, MXC_GPIO_INT_FALLING);
    MXC_GPIO_EnableInt(in.port, in.mask);
    NVIC_EnableIRQ(MXC_GPIO_GET_IRQ(MXC_GPIO_GET_IDX(IN_PORT)));

    motor_stop();
}

void motor_go() {
    MXC_GPIO_OutClr(out1.port, out1.mask);
    MXC_GPIO_OutSet(out2.port, out2.mask);
}

void motor_stop()
{
    MXC_GPIO_OutClr(out1.port, out1.mask);
    MXC_GPIO_OutClr(out2.port, out2.mask);
}
