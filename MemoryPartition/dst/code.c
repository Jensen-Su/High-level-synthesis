for(int i = 0; i < 1920; i++)
{
	for(int j = 0; j < 1080; j++)
	{
		BankIndex0 = (1 * i + 4 * j + 8) % 13;
		BankOffset0 = 83 * ( i + 0 ) + j;

		BankIndex1 = (1 * i + 4 * j + 5) % 13;
		BankOffset1 = 83 * ( i + 1 ) + j;

		BankIndex2 = (1 * i + 4 * j + 9) % 13;
		BankOffset2 = 83 * ( i + 1 ) + j;

		BankIndex3 = (1 * i + 4 * j + 13) % 13;
		BankOffset3 = 83 * ( i + 1 ) + j;

		BankIndex4 = (1 * i + 4 * j + 2) % 13;
		BankOffset4 = 83 * ( i + 2 ) + j;

		BankIndex5 = (1 * i + 4 * j + 6) % 13;
		BankOffset5 = 83 * ( i + 2 ) + j;

		BankIndex6 = (1 * i + 4 * j + 10) % 13;
		BankOffset6 = 83 * ( i + 2 ) + j;

		BankIndex7 = (1 * i + 4 * j + 14) % 13;
		BankOffset7 = 83 * ( i + 2 ) + j;

		BankIndex8 = (1 * i + 4 * j + 18) % 13;
		BankOffset8 = 83 * ( i + 2 ) + j;

		BankIndex9 = (1 * i + 4 * j + 7) % 13;
		BankOffset9 = 83 * ( i + 3 ) + j;

		BankIndex10 = (1 * i + 4 * j + 11) % 13;
		BankOffset10 = 83 * ( i + 3 ) + j;

		BankIndex11 = (1 * i + 4 * j + 15) % 13;
		BankOffset11 = 83 * ( i + 3 ) + j;

		BankIndex12 = (1 * i + 4 * j + 12) % 13;
		BankOffset12 = 83 * ( i + 4 ) + j;

	}
}
