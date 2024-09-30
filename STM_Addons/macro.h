#define HIGH	GPIO_PIN_SET
#define LOW		GPIO_PIN_RESET

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

union data_conversion_2byte {
	uint8_t 	data_uint8_t[2];
	uint16_t 	data_uint16_t;
};
union data_conversion_4byte {
	uint8_t 	data_uint8_t[4];
	uint16_t 	data_uint16_t[2];
	uint32_t 	data_uint32_t[1];
	float 		data_float;
};
union data_conversion_8byte {
	uint8_t 	data_uint8_t[8];
	uint16_t 	data_uint16_t[4];
	uint32_t 	data_uint32_t[2];
	float 		data_float[2];
	double 		data_double[1];
};

