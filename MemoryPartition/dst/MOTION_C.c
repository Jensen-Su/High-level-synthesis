int 	BankIndex0, BankOffset0;
int 	BankIndex1, BankOffset1;
int 	BankIndex2, BankOffset2;
int 	BankIndex3, BankOffset3;

for(int i = 0; i < 1920; i++)
{
	for(int j = 0; j < 1080; j++)
	{
		BankIndex0 = (1 * i + 2 * j + 0) % 13;
		BankOffset0 = 83 * ( i + 0 ) + j / 13;

		BankIndex1 = (1 * i + 2 * j + 2) % 13;
		BankOffset1 = 83 * ( i + 0 ) + j / 13;

		BankIndex2 = (1 * i + 2 * j + 1) % 13;
		BankOffset2 = 83 * ( i + 1 ) + j / 13;

		BankIndex3 = (1 * i + 2 * j + 3) % 13;
		BankOffset3 = 83 * ( i + 1 ) + j / 13;

	}
}
