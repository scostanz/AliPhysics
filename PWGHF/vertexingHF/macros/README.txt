- ComputeEfficiencyFromCombinHF.C
macro che calcola l'efficienza. Bisogna indicare nel config file i runs di input e tutto quello che serve, tipo bin e pt limits, ecc.
Nella macro, settare il nome del database.
Va compilata

- MergeEffNew13TeV.C
macro che calcola l'efficienza pesata sul numero di entries per periodo. 
L'output si chiama "*Prompt*" ma io poi lo rinomino a mano "weighted".
Produce gia' i rapporti e i confronti del totale con i vari periodi.

- ratio_eff.C
produce il rapporto tra le efficienze dei vari periodi e quella totale. 
Controllare se si usa la totale pesata o meno.
Per il 2016, 3 periodi distinti, con k e l separati.

- ratio_eff_2016.C
come prima, rapporto tra efficienze dei vari periodi e quella totale (pesata o no, da controllare).
Per il 2016, un solo periodo: tutti i runs sono sommati.

- compare_eff.C 
macro vecchia che serviva per fare il confronto tra le efficienze dei singoli 
periodi dei dati 2016.

- ProjectCombinHFAndFit.C
macro che produce gli spettri di massa invariante.
Ho modificato l'enumerator kLS in kLs altrimenti va in conflitto con un'altra 
definizione di kLS in un altro punto di alidock...
Produce molto output: far girare in background e ridirezionando l'output di 
testo in modo da salvarlo e controllarlo piu' tardi.

- compare_parameters_periods.C
macro che confronta i parametri (media, sigma, significativita' e S/B) dei 3
anni e quello merged.

- compare_yields_periods_pid.C
macro che confronta il raw yield normalizzato (su numero di eventi del normalization counter) di 2016-2017-2018 e merged.

- HFPtSpectrum.C
macro che calcola la sezione d'urto, prendendo come input l'output di alcune 
delle precedenti macro.

- ratio_cs.C
fa il confronto tra le sezioni d'urto con le diverse opzioni per il template 
delle riflessioni

- ratio_cs_periods.C
fa il confronto tra le sezioni d'urto dei diversi dataset.