//
char str[32]; //временный массив


HAL_RTC_GetTime(&hrtc, &sTime, RTC_FORMAT_BIN); //чтение времени
HAL_UART_Transmit(&huart3, str, sprintf(str, "%d:%d:%d\r\n", sTime.Hours, sTime.Minutes, sTime.Seconds), 100); //вывод времени в формате Х:Х:Х
HAL_RTC_GetDate(&hrtc, &sDate, RTC_FORMAT_BIN); //чтение даты ОБЯЗАТЕЛЬНО, иначе не будет работать чтение времени
HAL_UART_Transmit(&huart3, str, sprintf(str, "%d.%d.20%d\r\n\r\n", sDate.Date, sDate.Month, sDate.Year), 100); //вывод даты в формате Х:Х:20Х