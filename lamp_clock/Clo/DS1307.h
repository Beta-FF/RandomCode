// ---------------------------------------------------------------------------
// ������������, ����������� ��� ������� � DS1307 RTC
//#include "TWI.h"

#define DS1307 0xD0           // ����� ���� I2C ��� ���������� DS1307 RTC
#define SECONDS_REGISTER   0x00
#define MINUTES_REGISTER   0x01
#define HOURS_REGISTER     0x02
#define DAYOFWK_REGISTER   0x03
#define DAYS_REGISTER      0x04
#define MONTHS_REGISTER    0x05
#define YEARS_REGISTER     0x06
#define CONTROL_REGISTER   0x07
#define RAM_BEGIN          0x08
#define RAM_END            0x3F

uint16_t	tmp;

void DS1307_init (void)
{
	I2C_WriteRegister(DS1307, CONTROL_REGISTER, 0b00010000); // ��������� ��������� ������� � �������������� ���������� 
		tmp = I2C_ReadRegister(DS1307,SECONDS_REGISTER); // ���������� �������� ������
			if (tmp > 0b01111111)		//	�������� ��������� ���� 7 (���������� ������� ���������), ��������� ���� ��� ������� � 0 
			{
			tmp = (tmp ^ 0b10000000);						 // ���� ��� ���������� � 1, �� ���������� ��� ����� (XOR)
			I2C_WriteRegister(DS1307,SECONDS_REGISTER, tmp); // ������ ��������, ������ �� ��������� 7 ����� 	
			}
	 
}

void DS1307_GetTime(uint8_t *hours, uint8_t *minutes, uint8_t *seconds)
// ������������ ������ ����, ������, ������� � ������� BCD.
{
   *hours = I2C_ReadRegister(DS1307,HOURS_REGISTER);
   *minutes = I2C_ReadRegister(DS1307,MINUTES_REGISTER);
   *seconds = I2C_ReadRegister(DS1307,SECONDS_REGISTER);
   if (*hours & 0x40) // 12-������� �����:
      *hours &= 0x1F; // ������������ ������� 5 ��� (��� pm ���������� ������ 0x20)
   else
      *hours &= 0x3F; // 24-������� �����: ������������ ������� 6 ���
}

void DS1307_GetDate(uint8_t *months, uint8_t *days, uint8_t *years)
// ������������ ������ �����, ����, ��� � ������� BCD.
{
   *months = I2C_ReadRegister(DS1307,MONTHS_REGISTER);
   *days = I2C_ReadRegister(DS1307,DAYS_REGISTER);
   *years = I2C_ReadRegister(DS1307,YEARS_REGISTER);
}
void Set_Minutes_DS1307(uint8_t data)
{
I2C_WriteRegister(DS1307,MINUTES_REGISTER, data);	
}

void Set_Hours_DS1307(uint8_t data)
// ������� ������ ��������� ���� � �������.
{
	I2C_WriteRegister(DS1307,HOURS_REGISTER, data);
	
  /* I2C_WriteRegister(DS1307,MONTHS_REGISTER, 0x08);
   I2C_WriteRegister(DS1307,DAYS_REGISTER, 0x31);
   I2C_WriteRegister(DS1307,YEARS_REGISTER, 0x13);
   I2C_WriteRegister(DS1307,HOURS_REGISTER, 0x08+0x40); // ���������� 0x40 ��� PM
   I2C_WriteRegister(DS1307,MINUTES_REGISTER, 0x51);
   I2C_WriteRegister(DS1307,SECONDS_REGISTER, 0x00);*/
}
