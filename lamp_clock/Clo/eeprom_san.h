/*
������ � ����������������� ������� EEPROM
*/


void EEPROM_Write (uint16_t adr, uint8_t value)
{
	while(EECR & (1<<EEWE));	//���� ������������ ����� ��������� ��������� ��������� � �������
	EEAR = adr;					//������������� �����
	EEDR =value;				//����� ������ � �������
	EECR |= (1<<EEMWE);			//��������� ������
	EECR |= (1<<EEWE);			//����� ���� � ������
}

uint8_t EEPROM_read (uint16_t adr)
{
	while(EECR & (1<<EEWE));	//���� ������������ ����� ��������� ��������� ��������� � �������
	EEAR = adr;					//������������� �����
	EECR |= (1<<EERE);			//��������� �������� ���������� �� ������ � ������� ������
	return EEDR;				//���������� ���������
}

