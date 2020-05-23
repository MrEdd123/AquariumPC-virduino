// timer.h

#ifndef _TIMER_h
#define _TIMER_h

void ProgrammTimer() 
{
	/*************** Timer Programme ******************/

	uint32_t local_t = nowLocal();							//für Zeitumstellung nötig
	Zeit = (minute(local_t)) + (hour(local_t) * 100);		//RTC zeit in vier stellig Umrechnen (z.B. 12:30 nach 1230)

	uint16_t SoTimerAu;
	SoTimerAu = ((SoAuMin)+(SoAuStd) * 100);

	if ((Zeit == SoTimerAu) & (second(local_t) == 0))
	{
		SonneIndex = 1;
	}

	uint16_t SoTimerUn;
	SoTimerUn = ((SoUnMin)+(SoUnStd) * 100);

	if ((Zeit == SoTimerUn) & (second(local_t) == 0))
	{
		SonneIndex = 2;
	}

	uint16_t SoTimerMiAn;
	SoTimerMiAn = ((SoMiAnMin)+(SoMiAnStd) * 100);
	if ((Zeit == SoTimerMiAn) & (second(local_t) == 0))
	{
		SonneIndex = 3;
	}

	uint16_t SoTimerMiAus;
	SoTimerMiAus = ((SoMiAusMin)+(SoMiAusStd) * 100);
	if ((Zeit == SoTimerMiAus) & (second(local_t) == 0))
	{
		SonneIndex = 4;
	}

	uint16_t SoTimerNaAn;
	SoTimerNaAn = ((SoNaMin)+(SoNaStd) * 100);
	if ((Zeit == SoTimerNaAn) & (second(local_t) == 0))
	{
		SonneIndex = 5;
	}

	uint16_t FutterTimer;
	FutterTimer = ((FutterMin)+(FutterStd) * 100);
	if ((Zeit == FutterTimer) & (second(local_t) == 0))
	{
		FutterIndex = 1;
	}

	uint16_t CO2AN;
	uint16_t CO2AUS;
	CO2AN = ((CO2AnMin)+(CO2AnStd) * 100);
	CO2AUS = ((CO2AusMin)+(CO2AusStd) * 100);
	if ((Zeit > (CO2AN - 1)) && (Zeit < CO2AUS))
	{
		digitalWrite(co2, HIGH);
		
	}
	else
	{
		digitalWrite(co2, LOW);
		
	}
}


#endif