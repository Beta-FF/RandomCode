// ---------------------------------------------------------------------------
// Подпрограммы I2C (TWI)
// ---------------------------------------------------------------------------
// Глобальные определения:
#define F_CPU 	14745600UL //14.7456MHz

#define ClearBit(x,y) x &= ~_BV(y)  // Макрос, эквивалентный cbi(x,y)
#define SetBit(x,y) x |= _BV(y)     // Макрос, эквивалентный sbi(x,y)



// На микроконтроллерах серии AVRmega часто (но не всегда) PA4 является
// сигналом данных (SDA) и PA5 является сигналом тактов (SCL).
// Стандартная частота тактов 100 кГц, что устанавливается подпрограммой
// I2C_Init. Тактовая частота I2C зависит от рабочей тактовой частоты AVR.
#define F_SCL 100000L   // тактовая частота I2C равна 100 кГц
#define READ 1
#define TW_START 0xA4   // отправка start condition (TWINT,TWSTA,TWEN)
#define TW_STOP 0x94    // отправка stop condition (TWINT,TWSTO,TWEN)
#define TW_ACK 0xC4     // возврат ACK для подчиненного устройства (slave)
#define TW_NACK 0x84    // нет возврата ACK для slave
#define TW_SEND 0x84    // отправка данных (TWINT,TWEN)
#define TW_READY (TWCR & 0x80)      // готовность, когда TWINT вернется в лог. 1
#define TW_STATUS (TWSR & 0xF8)     // вернет значение из регистра статуса
#define I2C_Stop() TWCR = TW_STOP   // inline-макрос для отправки stop condition

void I2C_Init()
// На частоте 16 MHz частота SCL будет равна 16/(16+2(TWBR)), если предположить,
// что пресклер обнулен (не делит частоту). Тогда для 100 кГц SCL:
// TWBR = ((F_CPU/F_SCL)-16)/2 = ((16/0.1)-16)/2 = 144/2 = 72
{
   TWSR = 0;            // отключает прескалер
   TWBR = ((F_CPU/F_SCL)-16)/2;  // установка частоты SCL
}

uint8_t I2C_Detect(uint8_t addr)
// Функция ищет slave-устройство по указанному адресу, вернет 1 если
// устройство найдено, иначе вернет 0.
{
   TWCR = TW_START;           // отправка start condition
   while (!TW_READY);         // ожидание готовности
   TWDR = addr;               // передача устройству
   TWCR = TW_SEND;            //  адреса шины
   while (!TW_READY);         // ожидание готовности
   return (TW_STATUS==0x18);  // вернет 1, если устройство найдено
}

uint8_t I2C_FindDevice(uint8_t start)
// Функция возвратит адрес первого найденного устройства, начиная
// с адреса start. Если ничего не найдено на шине I2C, то вернет 0.
{
   //Поиск по всем 256 адресам:
   for (uint8_t addr=start;addr<0xFF;addr++)
   {
      if (I2C_Detect(addr))
         return addr; // возврат, как только что-то найдено
   }
   return 0;          // ничего не найдено, возврат 0.
}

void I2C_Start (uint8_t slaveAddr)
{
   I2C_Detect(slaveAddr);
}

uint8_t I2C_Write (uint8_t data)
// Функция посылает байт данных slave-устройству.
{
   TWDR = data;         // загрузка данных
   TWCR = TW_SEND;      //  и отправка их
   while (!TW_READY);   // ожидание готовности
   return (TW_STATUS!=0x28);
}

uint8_t I2C_ReadACK ()
// Функция прочитает байт данных из slave-устройства в блочном режиме.
{
   TWCR = TW_ACK;       // ack означает, что далее 
                        // будут еще читаться данные
   while (!TW_READY);   // ожидание готовности
   return TWDR;
}

uint8_t I2C_ReadNACK ()
// Функция прочитает байт данных из slave-устройства.
{
   TWCR = TW_NACK;      // nack означает, что будет прочитан только 1 байт
   while (!TW_READY);   // ожидание готовности
   return TWDR;
}

void I2C_WriteByte(uint8_t busAddr, uint8_t data)
// Подпрограмма записывает байт в slave-устройство.
{
   I2C_Start(busAddr);  // отправка адреса шины I2C
   I2C_Write(data);     // затем отправка байта данных
   I2C_Stop();
}

void I2C_WriteRegister(uint8_t busAddr, uint8_t deviceRegister, uint8_t data)
// Подпрограмма записывает байт во внутренний регистр slave-устройства.
{
   I2C_Start(busAddr);        // отправка адреса шины I2C
   I2C_Write(deviceRegister); // первый байт = внутреннему адресу регистра
   I2C_Write(data);           // второй байт = данным для этого регистра
   I2C_Stop();
}

uint8_t I2C_ReadRegister(uint8_t busAddr, uint8_t deviceRegister)
// Функция читает байт из внутреннего регистра slave-устройства.
{
   uint8_t data = 0;
   I2C_Start(busAddr);        // отправка адреса шины I2C
   I2C_Write(deviceRegister); // установка указателя на регистр
   I2C_Start(busAddr+READ);   // перезапуск для операции чтения
   data = I2C_ReadNACK();     // чтение данных регистра
   I2C_Stop();
   return data;
}

