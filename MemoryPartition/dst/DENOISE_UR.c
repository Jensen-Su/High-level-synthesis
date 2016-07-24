int 	BankIndex0, BankOffset0;
int 	BankIndex1, BankOffset1;
int 	BankIndex2, BankOffset2;
int 	BankIndex3, BankOffset3;
int 	BankIndex4, BankOffset4;
int 	BankIndex5, BankOffset5;
int 	BankIndex6, BankOffset6;
int 	BankIndex7, BankOffset7;

for(int i = 0; i < 1920; i++)
{
	for(int j = 0; j < 1080; j++)
	{
		BankIndex0 = (1 * i + 3 * j + 3) % 13;
		BankOffset0 = 83 * ( i + 0 ) + j / 13;

		BankIndex1 = (1 * i + 3 * j + 6) % 13;
		BankOffset1 = 83 * ( i + 0 ) + j / 13;

		BankIndex2 = (1 * i + 3 * j + 1) % 13;
		BankOffset2 = 83 * ( i + 1 ) + j / 13;

		BankIndex3 = (1 * i + 3 * j + 4) % 13;
		BankOffset3 = 83 * ( i + 1 ) + j / 13;

		BankIndex4 = (1 * i + 3 * j + 7) % 13;
		BankOffset4 = 83 * ( i + 1 ) + j / 13;

		BankIndex5 = (1 * i + 3 * j + 10) % 13;
		BankOffset5 = 83 * ( i + 1 ) + j / 13;

		BankIndex6 = (1 * i + 3 * j + 5) % 13;
		BankOffset6 = 83 * ( i + 2 ) + j / 13;

		BankIndex7 = (1 * i + 3 * j + 8) % 13;
		BankOffset7 = 83 * ( i + 2 ) + j / 13;

	}
}
