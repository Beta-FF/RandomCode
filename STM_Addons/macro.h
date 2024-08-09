#define HIGH	1
#define LOW		0

#define writePin(cube_mx_pin_name, state) HAL_GPIO_WritePin(cube_mx_pin_name##_GPIO_Port, cube_mx_pin_name##_Pin, state)
#define readPin(cube_mx_pin_name) HAL_GPIO_ReadPin(cube_mx_pin_name##_GPIO_Port, cube_mx_pin_name##_Pin)

#define constrain(amt, low, high) ((amt)<(low)?(low):((amt)>(high)?(high):(amt)))
