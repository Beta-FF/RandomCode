#define HIGH	1
#define LOW		0

/* HAL Read/Write/Toggle */
#define writePin(cube_mx_pin_name, state) HAL_GPIO_WritePin(cube_mx_pin_name##_GPIO_Port, cube_mx_pin_name##_Pin, state)
#define readPin(cube_mx_pin_name) HAL_GPIO_ReadPin(cube_mx_pin_name##_GPIO_Port, cube_mx_pin_name##_Pin)
#define togglePin(cube_mx_pin_name) HAL_GPIO_TogglePin(cube_mx_pin_name##_GPIO_Port, cube_mx_pin_name##_Pin)

/* Fast register Read/Write/Toggle */
#define setPin(port, pin)     ((port)->BSRR = (pin))
#define clearPin(port, pin)     ((port)->BSRR = (pin << 16u))
//#define togglePin(port, pin)     ((port)->ODR  ^= (pin))

/* Other */
#define constrain(amt, low, high) ((amt)<(low)?(low):((amt)>(high)?(high):(amt)))
