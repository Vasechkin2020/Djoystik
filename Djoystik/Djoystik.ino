
#include <SoftwareSerial.h>
SoftwareSerial mySerial(9, 10); // RX, TX


#define R1 A1      // ��� ��� ������� ���������
int R1Ser;       // ��� ��� ������� ���������
#define R2 A0      // ��� ��� ������� ���������
int R2Ser;      // ��� ��� ������� ���������
#define R3 PD7      // ��� ��� ������ ������

#define C1 A2      // ��� ��� ������������ ���������
int C1Ser;      // ��� ��� ������� ���������
#define C2 A3      // ��� ��� ������������ ���������
int C2Ser;      // ��� ��� ������� ���������
#define C3 PD6      // ��� ��� ����������� ������

#define L1 A6      // ��� ��� ������ ���������
int L1Ser;       // ��� ��� ������� ���������
#define L2 A7        // ��� ��� ������ ���������
int L2Ser;      // ��� ��� ������� ���������
#define L3 PD5      // ��� ��� ����� ������

int8_t reg1 = 0;	  //�������� � ������� ����� ������� ������
int8_t reg2 = 0;
int8_t reg3 = 0;
int8_t reg4 = 0;
int8_t reg5 = 0;
int8_t reg6 = 0;
int8_t reg7 = 0;
int8_t reg8 = 0;
int8_t reg9 = 0;


void Init_AnalogPin()					   //����� �� � �� ���� ��������� �� ���� ???
{
	//pinMode(R1, INPUT);	  pinMode(R2, INPUT);
	//pinMode(C1, INPUT);	  pinMode(C2, INPUT);
	//pinMode(L1, INPUT);
	//pinMode(L2, INPUT);	  // � ���� ����� �� �������� ����������� ����
}
void Init_DigitalPin()
{
	pinMode(R3, INPUT_PULLUP);
	pinMode(C3, INPUT_PULLUP);
	pinMode(L3, INPUT_PULLUP);
}

void ColobrovkaAnalogPin()
{
	long r1=0, r2=0, c1=0, c2=0, l1=0, l2=0;
	for (size_t i = 0; i < 1000; i++)
	{
		r1 += analogRead(R1);
		r2 += analogRead(R2);
		c1 += analogRead(C1);
		c2 += analogRead(C2);
		l1 += analogRead(L1);
		l2 += analogRead(L2);
	}
	R1Ser = r1 / 1000;
	R2Ser = r2 / 1000;
	C1Ser = c1 / 1000;
	C2Ser = c2 / 1000;
	L1Ser = l1 / 1000;
	L2Ser = l2 / 1000;
}

int8_t Read_Normal_Pin(int Pin, int seredina)
{
	//long a = micros();
	int r1 = analogRead(Pin);
	int8_t r2;
	if (r1 > seredina) { r2 = map(r1, seredina, 1023, 0, 127); }
	if (r1 < seredina) { r2 = map(r1, 0, seredina, -127, 0); }
	if (r1 == seredina) { r2 = (((float)r1 / (seredina)) - 1); }
	//long b = micros();
	//Serial.print(" time3333= "); Serial.println(b - a);
	return r2;
}

void Read_AnalogPin()
{
	reg1 = Read_Normal_Pin(R1, R1Ser);
	reg2 = Read_Normal_Pin(R2, R2Ser);
	reg3 = Read_Normal_Pin(C1, C1Ser);
	reg4 = Read_Normal_Pin(C2, C2Ser);
	reg5 = Read_Normal_Pin(L1, L1Ser);
	reg6 = Read_Normal_Pin(L2, L2Ser);

}
void Read_DigitalPin()
{
	reg7 = digitalRead(R3);
	reg8 = digitalRead(C3);
	reg9 = digitalRead(L3);

}
void Print_Data()
{
	Serial.print(" R1= "); Serial.print(reg1);
	Serial.print(" R2= "); Serial.println(reg2);

	Serial.print(" C1= "); Serial.print(reg3);
	Serial.print(" C2= "); Serial.println(reg4);

	Serial.print(" L1= "); Serial.print(reg5);
	Serial.print(" L2= "); Serial.println(reg6);
	Serial.println("---");

	Serial.print(" R3= "); Serial.print(reg7);
	Serial.print(" C3= "); Serial.print(reg8);
	Serial.print(" L3= "); Serial.println(reg9);
	Serial.println("---");
}



// the setup function runs once when you press reset or power the board
void setup() 
{
	// �������������� ���������� ���������������� ��������� � ���� �������� �����:
	Serial.begin(115200);
	Serial.println("==============");
	Serial.println("Start Djoystik");
	Serial.println("==============");

	Init_AnalogPin();   // ������������� �����
	Init_DigitalPin();
	ColobrovkaAnalogPin();	  //���������� ����������� ��������� ��� ������ �� ������� ������

	// ������������� �������� �������� ������ ��� ����������������� �����, ���������� ����������� SoftwareSerial
	mySerial.begin(57600);
}

long time_print = 0;
long time_read = 0;

// the loop function runs over and over again until power down or reset
void loop() 
{

	if (mySerial.available() > 0)
	{
		int8_t Rx= mySerial.read();
		//Serial.print(" Rx= ");	Serial.println(Rx,HEX);
		if (Rx == 0x1D)				 //���� �������� ������� � ������ ������ �� ����� ������� ��������
		{
			mySerial.write(reg1);
			mySerial.write(reg2);
			mySerial.write(reg3);
			mySerial.write(reg4);
			mySerial.write(reg5);
			mySerial.write(reg6);
			mySerial.write(reg7);
			mySerial.write(reg8);
			mySerial.write(reg9);
			//Print_Data();
		}
	}
	if (millis() - time_read > 50)		// ��������� 1 ��� � 50 ����������	 ����� ������  	1 �����������
	{
		//long a = micros();
		time_read = millis();
		Read_AnalogPin();
		Read_DigitalPin();
		//long b = micros();
		//Serial.print(" time= "); Serial.println(b - a);
	}
	//if (millis() - time_print > 250)		// ��������� 1 ��� � 50 ����������
	//{
	//	time_print = millis();
	//	//mySerial.write(0x11);
	//		//Print_Data();
	//}
}
