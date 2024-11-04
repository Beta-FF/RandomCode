/* Private variables ---------------------------------------------------------*/
uint8_t arr[16]; //временный массив (может быть любым)

uint16_t G_X;
uint16_t G_Y;
uint16_t G_Z;

uint16_t A_X;
uint16_t A_Y;
uint16_t A_Z;

uint16_t Temperature;

// инициализация MPU
arr[0] = 0x00;
HAL_I2C_Mem_Write(&hi2c1, 0x68<<1, 0x6B, I2C_MEMADD_SIZE_8BIT, arr, 1, 100); //поенять на нужный I2C

//чтение сырых данных акселерометра
HAL_I2C_Mem_Read(&hi2c1, 0x68<<1, 0x3B, I2C_MEMADD_SIZE_8BIT, arr, 6, 100);
A_X = (uint16_t)(arr[0] << 8) | arr[1]; //конвертация в полезные ланные
A_Y = (uint16_t)(arr[2] << 8) | arr[3];
A_Z = (uint16_t)(arr[4] << 8) | arr[5];
//HAL_UART_Transmit(&huart3, str, sprintf(str, " A_X=%d\r\n A_Y=%d\r\n A_Z=%d\r\n\r\n", A_X, A_Y, A_Z), 100);// вывод сырых значений в уарт для проверки

//чтение данных термометра
HAL_I2C_Mem_Read(&hi2c1, 0x68<<1, 0x41, I2C_MEMADD_SIZE_8BIT, arr, 2, 100);
Temperature = (uint16_t)(arr[0] << 8) | arr[1];
//HAL_UART_Transmit(&huart3, str, sprintf(str, " T=%d\r\n\r\n", Temperature), 100); //проверка температуры

//чтение сырых данных гироскопа
HAL_I2C_Mem_Read(&hi2c1, 0x68<<1, 0x43, I2C_MEMADD_SIZE_8BIT, arr, 6, 100);
G_X = (uint16_t)(arr[0] << 8) | arr[1];
G_Y = (uint16_t)(arr[2] << 8) | arr[3];
G_Z = (uint16_t)(arr[4] << 8) | arr[5];
//HAL_UART_Transmit(&huart3, str, sprintf(str, " G_X=%d\r\n G_Y=%d\r\n G_Z=%d\r\n\r\n", G_X, G_Y, G_Z), 100);//проверка данных гироскопа
