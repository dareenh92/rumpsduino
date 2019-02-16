while(noc_NC_rxbuff0_av!=1)__NOP(); Idata = NC_NOC_RX_BUFF0;
      while(noc_NC_rxbuff0_av!=1)__NOP(); Qdata = NC_NOC_RX_BUFF0;

      digitalWrite_low(ledpin);

      Isamp[nSamples] = Idata;
      Qsamp[nSamples] = Qdata;

      // Isamp[nSamples].accum_cont = Idata;
      // Isamp[nSamples].accum_cont >>= 10; //divide by 1024

      // Qsamp[nSamples].accum_cont = Qdata;
      // Qsamp[nSamples].accum_cont >>= 10; //divide by 1024
    
      if(++nSamples==3){
        nSamples = 0;

        // TED calc - Sign version 
        // I-part
        tempcalc = sign_f(Isamp[2])- sign_f(Isamp[0]);
        tempcalc = mac_smul_32((int32_t)(sign_f(Isamp[1])),
                               (int32_t)(tempcalc));
        ted += tempcalc;

        // Q-part
        tempcalc = sign_f(Qsamp[2])- sign_f(Qsamp[0]);
        tempcalc = mac_smul_32((int32_t)(sign_f(Qsamp[1])),
                               (int32_t)(tempcalc));
        ted += tempcalc;

        // determine step correction
        stepCorrection = sign_f(ted);

        /* 
        // TED calc - Value version

        ted_total = 0;

        // I-part
        temp_accumint.accum_cont = Isamp[2].accum_cont-Isamp[0].accum_cont; //L-E
        int64_t temp_64b = mac_smul_32((int32_t)(Isamp[1].int_cont),
                                       (int32_t)(temp_accumint.int_cont));
        temp_accumint.int_cont = ( temp_64b >> 15);

        ted_total += temp_accumint.accum_cont;

        // Q-part
        temp_accumint.accum_cont = Qsamp[2].accum_cont-Qsamp[0].accum_cont; //L-E
        temp_64b = mac_smul_32((int32_t)(Qsamp[1].int_cont),
                               (int32_t)(temp_accumint.int_cont));
        temp_accumint.int_cont = ( temp_64b >> 15);

        ted_total += temp_accumint.accum_cont;
        
        // rounding - determine correction step
        stepCorrection = ted_total;
        if( (ted_total-=stepCorrection)>=0.5 )
          stepCorrection++; 
        */

        //send back synchronization result
        while(noc_NC_txbuff_isfull==1)__NOP();
        NC_NOC_TX_BUFF0 = stepCorrection;
      }